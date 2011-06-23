/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Washington
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "emu-net-device.h"
#include "emu-encode-decode.h"

#include "ns3/log.h"
#include "ns3/queue.h"
#include "ns3/simulator.h"
#include "ns3/ethernet-header.h"
#include "ns3/ethernet-trailer.h"
#include "ns3/llc-snap-header.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"
#include "ns3/string.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/channel.h"
#include "ns3/system-thread.h"
#include "ns3/mac48-address.h"
#include "ns3/enum.h"

#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netpacket/packet.h>
#include <arpa/inet.h>
#include <errno.h>
#include <limits>
#include <stdlib.h>
#include <time.h>

NS_LOG_COMPONENT_DEFINE ("EmuNetDevice");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (EmuNetDevice);

#define EMU_MAGIC 65867

TypeId 
EmuNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EmuNetDevice")
    .SetParent<NetDevice> ()
    .AddConstructor<EmuNetDevice> ()
    .AddAttribute ("Mtu", "The MAC-level Maximum Transmission Unit",
                   UintegerValue (0), // arbitrary un-used value because no setter
                   MakeUintegerAccessor (&EmuNetDevice::GetMtu),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("Address", 
                   "The ns-3 MAC address of this (virtual) device.",
                   Mac48AddressValue (Mac48Address ("ff:ff:ff:ff:ff:ff")),
                   MakeMac48AddressAccessor (&EmuNetDevice::m_address),
                   MakeMac48AddressChecker ())
    .AddAttribute ("DeviceName", 
                   "The name of the underlying real device (e.g. eth1).",
                   StringValue ("eth1"),
                   MakeStringAccessor (&EmuNetDevice::m_deviceName),
                   MakeStringChecker ())
    .AddAttribute ("Start", 
                   "The simulation time at which to spin up the device thread.",
                   TimeValue (Seconds (0.)),
                   MakeTimeAccessor (&EmuNetDevice::m_tStart),
                   MakeTimeChecker ())
    .AddAttribute ("Stop", 
                   "The simulation time at which to tear down the device thread.",
                   TimeValue (Seconds (0.)),
                   MakeTimeAccessor (&EmuNetDevice::m_tStop),
                   MakeTimeChecker ())
    .AddAttribute ("EncapsulationMode", 
                   "The link-layer encapsulation type to use.",
                   EnumValue (DIX),
                   MakeEnumAccessor (&EmuNetDevice::SetEncapsulationMode),
                   MakeEnumChecker (DIX, "Dix",
                                    LLC, "Llc"))

    //
    // Transmit queueing discipline for the device which includes its own set
    // of trace hooks.  Note that this is really going to run "on top of" the 
    // queueing discipline that will most likely be present in the devices of
    // the underlying operating system.
    //
    .AddAttribute ("TxQueue", 
                   "A queue to use as the transmit queue in the device.",
                   PointerValue (),
                   MakePointerAccessor (&EmuNetDevice::m_queue),
                   MakePointerChecker<Queue> ())

    .AddAttribute ("RxQueueSize", "Maximum size of the read queue.  "
                   "This value limits number of packets that have been read "
                   "from the network into a memory buffer but have not yet "
                   "been processed by the simulator.",
                   UintegerValue (1000),
                   MakeUintegerAccessor (&EmuNetDevice::m_maxPendingReads),
                   MakeUintegerChecker<uint32_t> ())

    //
    // Trace sources at the "top" of the net device, where packets transition
    // to/from higher layers.  These points do not really correspond to the 
    // MAC layer of the underlying operating system, but exist to provide 
    // a consitent tracing environment.  These trace hooks should really be
    // interpreted as the points at which a packet leaves the ns-3 environment
    // destined for the underlying operating system or vice-versa.
    //
    .AddTraceSource ("MacTx", 
                     "Trace source indicating a packet has arrived for transmission by this device",
                     MakeTraceSourceAccessor (&EmuNetDevice::m_macTxTrace))
    .AddTraceSource ("MacTxDrop", 
                     "Trace source indicating a packet has been dropped by the device before transmission",
                     MakeTraceSourceAccessor (&EmuNetDevice::m_macTxDropTrace))
    .AddTraceSource ("MacPromiscRx", 
                     "A packet has been received by this device, has been passed up from the physical layer "
                     "and is being forwarded up the local protocol stack.  This is a promiscuous trace,",
                     MakeTraceSourceAccessor (&EmuNetDevice::m_macPromiscRxTrace))
    .AddTraceSource ("MacRx", 
                     "A packet has been received by this device, has been passed up from the physical layer "
                     "and is being forwarded up the local protocol stack.  This is a non-promiscuous trace,",
                     MakeTraceSourceAccessor (&EmuNetDevice::m_macRxTrace))
#if 0
    // Not currently implemented for this device
    .AddTraceSource ("MacRxDrop", 
                     "Trace source indicating a packet was dropped before being forwarded up the stack",
                     MakeTraceSourceAccessor (&EmuNetDevice::m_macRxDropTrace))
#endif
    //
    // In normal ns-3 net devices, these trace souces correspond to the "bottom"
    // of the net device, where packets transition to/from the channel.  In 
    // the case of the emu device, there is no physical layer access -- all we
    // do is to send packets to another device that is really at a "real" MAC
    // level.  Since it could be misleading to call anything here PHY, we do not
    // implement these trace sources.
    //
#if 0
    .AddTraceSource ("PhyTxBegin", 
                     "Trace source indicating a packet has begun transmitting over the channel",
                     MakeTraceSourceAccessor (&EmuNetDevice::m_phyTxBeginTrace))
    .AddTraceSource ("PhyTxEnd", 
                     "Trace source indicating a packet has been completely transmitted over the channel",
                     MakeTraceSourceAccessor (&EmuNetDevice::m_phyTxEndTrace))
    .AddTraceSource ("PhyTxDrop", 
                     "Trace source indicating a packet has been dropped by the device during transmission",
                     MakeTraceSourceAccessor (&EmuNetDevice::m_phyTxDropTrace))
    .AddTraceSource ("PhyRxBegin", 
                     "Trace source indicating a packet has begun being received by the device",
                     MakeTraceSourceAccessor (&EmuNetDevice::m_phyRxBeginTrace))
    .AddTraceSource ("PhyRxEnd", 
                     "Trace source indicating a packet has been completely received by the device",
                     MakeTraceSourceAccessor (&EmuNetDevice::m_phyRxEndTrace))
    .AddTraceSource ("PhyRxDrop", 
                     "Trace source indicating a packet has been dropped by the device during reception",
                     MakeTraceSourceAccessor (&EmuNetDevice::m_phyRxDropTrace))
#endif
    //
    // Trace sources designed to simulate a packet sniffer facility (tcpdump). 
    //
    .AddTraceSource ("Sniffer", 
                     "Trace source simulating a non-promiscuous packet sniffer attached to the device",
                     MakeTraceSourceAccessor (&EmuNetDevice::m_snifferTrace))
    .AddTraceSource ("PromiscSniffer", 
                     "Trace source simulating a promiscuous packet sniffer attached to the device",
                     MakeTraceSourceAccessor (&EmuNetDevice::m_promiscSnifferTrace))
  ;
  return tid;
}

EmuNetDevice::EmuNetDevice () 
  :
    m_startEvent (),
    m_stopEvent (),
    m_sock (-1),
    m_readThread (0),
    m_ifIndex (std::numeric_limits<uint32_t>::max ()), // absurdly large value
    m_sll_ifindex (-1),
    m_isBroadcast (true),
    m_isMulticast (false),
    m_pendingReadCount (0)
{
  NS_LOG_FUNCTION (this);
  m_packetBuffer = new uint8_t[65536];
  Start (m_tStart);
}

EmuNetDevice::~EmuNetDevice ()
{
  delete [] m_packetBuffer;
  m_packetBuffer = 0;
}

void 
EmuNetDevice::DoDispose ()
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_readThread != 0)
    {
      StopDevice ();
    }
  m_node = 0;
  NetDevice::DoDispose ();
}

void 
EmuNetDevice::SetEncapsulationMode (enum EncapsulationMode mode)
{
  NS_LOG_FUNCTION (mode);
  m_encapMode = mode;
  NS_LOG_LOGIC ("m_encapMode = " << m_encapMode);
}

EmuNetDevice::EncapsulationMode
EmuNetDevice::GetEncapsulationMode (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_encapMode;
}

void
EmuNetDevice::Start (Time tStart)
{
  NS_LOG_FUNCTION (tStart);

  //
  // Cancel any pending start event and schedule a new one at some relative time in the future.
  //
  Simulator::Cancel (m_startEvent);
  m_startEvent = Simulator::Schedule (tStart, &EmuNetDevice::StartDevice, this);
}

void
EmuNetDevice::Stop (Time tStop)
{
  NS_LOG_FUNCTION (tStop);
  //
  // Cancel any pending stop event and schedule a new one at some relative time in the future.
  //
  Simulator::Cancel (m_stopEvent);
  m_startEvent = Simulator::Schedule (tStop, &EmuNetDevice::StopDevice, this);
}

void
EmuNetDevice::StartDevice (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  //
  // Spin up the emu net device and start receiving packets.
  //
  if (m_sock != -1)
    {
      NS_FATAL_ERROR ("EmuNetDevice::StartDevice(): Device is already started");
    }

  //
  // We're going to need a pointer to the realtime simulator implementation.
  // It's important to remember that access to that implementation may happen 
  // in a completely different thread than the simulator is running in (we're 
  // going to spin up that thread below).  We are talking about multiple threads
  // here, so it is very, very dangerous to do any kind of reference couning on
  // a shared object that is unaware of what is happening.  What we are going to 
  // do to address that is to get a reference to the realtime simulator here 
  // where we are running in the context of a running simulator scheduler --
  // recall we did a Simulator::Schedule of this method above.  We get the
  // simulator implementation pointer in a single-threaded way and save the
  // underlying raw pointer for use by the (other) read thread.  We must not
  // free this pointer or we may delete the simulator out from under us an 
  // everyone else.  We assume that the simulator implementation cannot be 
  // replaced while the emu device is running and so will remain valid through
  // the time during which the read thread is running.
  //
  Ptr<RealtimeSimulatorImpl> impl = DynamicCast<RealtimeSimulatorImpl> (Simulator::GetImplementation ());
  m_rtImpl = GetPointer (impl);

  //
  // A similar story exists for the node ID.  We can't just naively do a
  // GetNode ()->GetId () since GetNode is going to give us a Ptr<Node> which
  // is reference counted.  We need to stash away the node ID for use in the
  // read thread.
  //
  m_nodeId = GetNode ()->GetId ();

  NS_LOG_LOGIC ("Creating socket");

  //
  // Call out to a separate process running as suid root in order to get a raw 
  // socket.  We do this to avoid having the entire simulation running as root.
  // If this method returns, we'll have a raw socket waiting for us in m_sock.
  //
  CreateSocket ();

  //
  // Figure out which interface index corresponds to the device name in the corresponding attribute.
  //
  struct ifreq ifr;
  bzero (&ifr, sizeof(ifr));
  strncpy ((char *)ifr.ifr_name, m_deviceName.c_str (), IFNAMSIZ);

  NS_LOG_LOGIC ("Getting interface index");
  int32_t rc = ioctl (m_sock, SIOCGIFINDEX, &ifr);
  if (rc == -1)
    {
      NS_FATAL_ERROR ("EmuNetDevice::StartDevice(): Can't get interface index");
    }

  //
  // Save the real interface index for later calls to sendto
  //
  m_sll_ifindex = ifr.ifr_ifindex;

  //
  // Bind the socket to the interface we just found.
  //
  struct sockaddr_ll ll;
  bzero (&ll, sizeof(ll));

  ll.sll_family = AF_PACKET;
  ll.sll_ifindex = m_sll_ifindex;
  ll.sll_protocol = htons (ETH_P_ALL);

  NS_LOG_LOGIC ("Binding socket to interface");

  rc = bind (m_sock, (struct sockaddr *)&ll, sizeof (ll));
  if (rc == -1)
    {
      NS_FATAL_ERROR ("EmuNetDevice::StartDevice(): Can't bind to specified interface");
    }

  rc = ioctl (m_sock, SIOCGIFFLAGS, &ifr);
  if (rc == -1)
    {
      NS_FATAL_ERROR ("EmuNetDevice::StartDevice(): Can't get interface flags");
    }

  //
  // This device only works if the underlying interface is up in promiscuous 
  // mode.  We could have turned it on in the socket creator, but the situation
  // is that we expect these devices to be used in conjunction with virtual 
  // machines with connected host-only (simulated) networks, or in a testbed.
  // There is a lot of setup and configuration happening outside of this one 
  // issue, and we expect that configuration to include choosing a valid
  // interface (e.g, "ath1"), ensuring that the device supports promiscuous 
  // mode, and placing it in promiscuous mode.  We just make sure of the
  // end result.
  //
  if ((ifr.ifr_flags & IFF_PROMISC) == 0)
    {
      NS_FATAL_ERROR ("EmuNetDevice::StartDevice(): " << m_deviceName << " is not in promiscuous mode");
    }
  if ((ifr.ifr_flags & IFF_BROADCAST) != IFF_BROADCAST)
    {
      // We default m_isBroadcast to true but turn it off here if not
      // supported, because in the common case, overlying IP code will 
      // assert during configuration time if this is false, before this
      // method has a chance to set it during runtime
      m_isBroadcast = false;
    }
  if ((ifr.ifr_flags & IFF_MULTICAST) == IFF_MULTICAST)
    {
      // This one is OK to enable at runtime
      m_isMulticast = true;
    }

  //
  // Now spin up a read thread to read packets.
  //
  if (m_readThread != 0)
    {
      NS_FATAL_ERROR ("EmuNetDevice::StartDevice(): Receive thread is already running");
    }

  NS_LOG_LOGIC ("Spinning up read thread");

  m_readThread = Create<SystemThread> (MakeCallback (&EmuNetDevice::ReadThread, this));
  m_readThread->Start ();

  NotifyLinkUp ();
}

void
EmuNetDevice::CreateSocket (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  //
  // We want to create a raw socket for our net device.  Unfortunately for us
  // you have to have root privileges to do that.  Instead of running the 
  // entire simulation as root, we decided to make a small program who's whole
  // reason for being is to run as suid root and create a raw socket.  We're
  // going to fork and exec that program soon, but we need to have a socket
  // to talk to it with.  So we create a local interprocess (Unix) socket 
  // for that purpose.
  //
  int sock = socket (PF_UNIX, SOCK_DGRAM, 0);
  if (sock == -1)
    {
      NS_FATAL_ERROR ("EmuNetDevice::CreateSocket(): Unix socket creation error, errno = " << strerror (errno));
    }

  //
  // Bind to that socket and let the kernel allocate an endpoint
  //
  struct sockaddr_un un;
  memset (&un, 0, sizeof (un));
  un.sun_family = AF_UNIX;
  int status = bind (sock, (struct sockaddr*)&un, sizeof (sa_family_t));
  if (status == -1)
    {
      NS_FATAL_ERROR ("EmuNetDevice::CreateSocket(): Could not bind(): errno = " << strerror (errno));
    }

  NS_LOG_INFO ("Created Unix socket");
  NS_LOG_INFO ("sun_family = " << un.sun_family);
  NS_LOG_INFO ("sun_path = " << un.sun_path);

  //
  // We have a socket here, but we want to get it there -- to the program we're
  // going to exec.  What we'll do is to do a getsockname and then encode the
  // resulting address information as a string, and then send the string to the
  // program as an argument.  So we need to get the sock name.
  //
  socklen_t len = sizeof (un);
  status = getsockname (sock, (struct sockaddr*)&un, &len);
  if (status == -1)
    {
      NS_FATAL_ERROR ("EmuNetDevice::CreateSocket(): Could not getsockname(): errno = " << strerror (errno));
    }

  //
  // Now encode that socket name (family and path) as a string of hex digits
  //
  std::string path = EmuBufferToString ((uint8_t *)&un, len);
  NS_LOG_INFO ("Encoded Unix socket as \"" << path << "\"");
  //
  // Fork and exec the process to create our socket.  If we're us (the parent)
  // we wait for the child (the socket creator) to complete and read the 
  // socket it created using the ancillary data mechanism.
  //
  // Tom Goff reports the possiblility of a deadlock when trying to acquire the
  // python GIL here.  He says that this might be due to trying to access Python
  // objects after fork() without calling PyOS_AfterFork() to properly reset 
  // Python state (including the GIL).  There is no code to cause the problem
  // here in emu, but this was visible in similar code in tap-bridge.
  //
  pid_t pid = ::fork ();
  if (pid == 0)
    {
      NS_LOG_DEBUG ("Child process");

      //
      // build a command line argument from the encoded endpoint string that 
      // the socket creation process will use to figure out how to respond to
      // the (now) parent process.
      //
      std::ostringstream oss;
      oss << "-p" << path;
      NS_LOG_INFO ("Parameters set to \"" << oss.str () << "\"");

      //
      // Execute the socket creation process image.
      //
      status = ::execlp ("emu-sock-creator", 
                         "emu-sock-creator",                            // argv[0] (filename)
                         oss.str ().c_str (),                           // argv[1] (-p<path?
                         (char *)NULL);

      //
      // If the execlp successfully completes, it never returns.  If it returns it failed or the OS is
      // broken.  In either case, we bail.
      //
      NS_FATAL_ERROR ("EmuNetDevice::CreateSocket(): Back from execlp(), errno = " << ::strerror (errno));
    }
  else
    {
      NS_LOG_DEBUG ("Parent process");
      //
      // We're the process running the emu net device.  We need to wait for the
      // socket creator process to finish its job.
      //
      int st;
      pid_t waited = waitpid (pid, &st, 0);
      if (waited == -1)
        {
          NS_FATAL_ERROR ("EmuNetDevice::CreateSocket(): waitpid() fails, errno = " << strerror (errno));
        }
      NS_ASSERT_MSG (pid == waited, "EmuNetDevice::CreateSocket(): pid mismatch");

      //
      // Check to see if the socket creator exited normally and then take a 
      // look at the exit code.  If it bailed, so should we.  If it didn't
      // even exit normally, we bail too.
      //
      if (WIFEXITED (st))
        {
          int exitStatus = WEXITSTATUS (st);
          if (exitStatus != 0)
            {
              NS_FATAL_ERROR ("EmuNetDevice::CreateSocket(): socket creator exited normally with status " << exitStatus);
            }
        }
      else 
        {
          NS_FATAL_ERROR ("EmuNetDevice::CreateSocket(): socket creator exited abnormally");
        }

      //
      // At this point, the socket creator has run successfully and should 
      // have created our raw socket and sent it back to the socket address
      // we provided.  Our socket should be waiting on the Unix socket.  We've
      // got to do a bunch of grunto work to get at it, though.
      //
      // The struct iovec below is part of a scatter-gather list.  It describes a
      // buffer.  In this case, it describes a buffer (an integer) that will
      // get the data that comes back from the socket creator process.  It will
      // be a magic number that we use as a consistency/sanity check.
      // 
      struct iovec iov;
      uint32_t magic;
      iov.iov_base = &magic;
      iov.iov_len = sizeof(magic);

      //
      // The CMSG macros you'll see below are used to create and access control 
      // messages (which is another name for ancillary data).  The ancillary 
      // data is made up of pairs of struct cmsghdr structures and associated
      // data arrays.
      //
      // First, we're going to allocate a buffer on the stack to receive our 
      // data array (that contains the socket).  Sometimes you'll see this called
      // an "ancillary element" but the msghdr uses the control message termimology
      // so we call it "control."
      //
      size_t msg_size = sizeof(int);
      char control[CMSG_SPACE (msg_size)];

      //
      // There is a msghdr that is used to minimize the number of parameters
      // passed to recvmsg (which we will use to receive our ancillary data).
      // This structure uses terminology corresponding to control messages, so
      // you'll see msg_control, which is the pointer to the ancillary data and 
      // controllen which is the size of the ancillary data array.
      //
      // So, initialize the message header that describes the ancillary/control
      // data we expect to receive and point it to buffer.
      //
      struct msghdr msg;
      msg.msg_name = 0;
      msg.msg_namelen = 0;
      msg.msg_iov = &iov;
      msg.msg_iovlen = 1;
      msg.msg_control = control;
      msg.msg_controllen = sizeof (control);
      msg.msg_flags = 0;

      //
      // Now we can actually receive the interesting bits from the socket
      // creator process.
      //
      ssize_t bytesRead = recvmsg (sock, &msg, 0);
      if (bytesRead != sizeof(int))
        {
          NS_FATAL_ERROR ("EmuNetDevice::CreateSocket(): Wrong byte count from socket creator");
        }

      //
      // There may be a number of message headers/ancillary data arrays coming in.
      // Let's look for the one with a type SCM_RIGHTS which indicates it' the
      // one we're interested in.
      //
      struct cmsghdr *cmsg;
      for (cmsg = CMSG_FIRSTHDR (&msg); cmsg != NULL; cmsg = CMSG_NXTHDR (&msg, cmsg))
        {
          if (cmsg->cmsg_level == SOL_SOCKET &&
              cmsg->cmsg_type == SCM_RIGHTS)
            {
              //
              // This is the type of message we want.  Check to see if the magic 
              // number is correct and then pull out the socket we care about if
              // it matches
              //
              if (magic == EMU_MAGIC)
                {
                  NS_LOG_INFO ("Got SCM_RIGHTS with correct magic " << magic);
                  int *rawSocket = (int*)CMSG_DATA (cmsg);
                  NS_LOG_INFO ("Got the socket from the socket creator = " << *rawSocket);
                  m_sock = *rawSocket;
                  return;
                }
              else
                {
                  NS_LOG_INFO ("Got SCM_RIGHTS, but with bad magic " << magic);
                }
            }
        }
      NS_FATAL_ERROR ("Did not get the raw socket from the socket creator");
    }
}

void
EmuNetDevice::StopDevice (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  close (m_sock);
  m_sock = -1;

  NS_ASSERT_MSG (m_readThread != 0, "EmuNetDevice::StopDevice(): Receive thread is not running");

  NS_LOG_LOGIC ("Joining read thread");
  m_readThread->Join ();
  m_readThread = 0;
}

void
EmuNetDevice::ForwardUp (uint8_t *buf, uint32_t len)
{
  NS_LOG_FUNCTION (buf << len);

  //
  // Create a packet out of the buffer we received and free that buffer.
  //
  Ptr<Packet> packet = Create<Packet> (reinterpret_cast<const uint8_t *> (buf), len);
  free (buf);
  buf = 0;

  {
    CriticalSection cs (m_pendingReadMutex);
    //std::cerr << std::endl << "EmuNetDevice main thread: m_pendingReadCount is " << m_pendingReadCount << std::endl;
    --m_pendingReadCount;
  }

  //
  // Trace sinks will expect complete packets, not packets without some of the
  // headers.
  //
  Ptr<Packet> originalPacket = packet->Copy ();

  EthernetHeader header (false);

  //
  // This device could be running in an environment where completely unexpected
  // kinds of packets are flying around, so we need to harden things a bit and
  // filter out packets we think are completely bogus, so we always check to see
  // that the packet is long enough to contain the header we want to remove.
  //
  if (packet->GetSize () < header.GetSerializedSize ())
    {
      m_phyRxDropTrace (originalPacket);
      return;
    }

  packet->RemoveHeader (header);

  NS_LOG_LOGIC ("Pkt source is " << header.GetSource ());
  NS_LOG_LOGIC ("Pkt destination is " << header.GetDestination ());

  uint16_t protocol;

  switch (m_encapMode)
    {
    case LLC:
      //
      // If the length/type is less than 1500, it corresponds to a length 
      // interpretation packet.  In this case, it is an 802.3 packet and 
      // will also have an 802.2 LLC header.  If greater than 1500, we
      // find the protocol number (Ethernet type) directly.
      //
      if (header.GetLengthType () <= 1500)
        {
          LlcSnapHeader llc;
          //
          // Check to see that the packet is long enough to possibly contain the
          // header we want to remove before just naively calling.
          //
          if (packet->GetSize () < llc.GetSerializedSize ())
            {
              m_phyRxDropTrace (originalPacket);
              return;
            }

          packet->RemoveHeader (llc);
          protocol = llc.GetType ();
        }
      else
        {
          protocol = header.GetLengthType ();
        }
      break;

    case DIX:
      protocol = header.GetLengthType ();
      break;

    default:
      NS_FATAL_ERROR ("invalid encapsulation mode");
      protocol = 0; /* quiet compiler */
    }

  PacketType packetType;

  if (header.GetDestination ().IsBroadcast ())
    {
      packetType = NS3_PACKET_BROADCAST;
    }
  else if (header.GetDestination ().IsGroup ())
    {
      packetType = NS3_PACKET_MULTICAST;
    }
  else if (header.GetDestination () == m_address)
    {
      packetType = NS3_PACKET_HOST;
    }
  else
    {
      packetType = NS3_PACKET_OTHERHOST;
    }

  // 
  // For all kinds of packetType we receive, we hit the promiscuous sniffer
  // hook and pass a copy up to the promiscuous callback.  Pass a copy to 
  // make sure that nobody messes with our packet.
  //
  m_promiscSnifferTrace (originalPacket);

  if (!m_promiscRxCallback.IsNull ())
    {
      m_macPromiscRxTrace (originalPacket);
      m_promiscRxCallback (this, packet, protocol, header.GetSource (), header.GetDestination (), packetType);
    }

  //
  // If this packet is not destined for some other host, it must be for us
  // as either a broadcast, multicast or unicast.  We need to hit the mac
  // packet received trace hook and forward the packet up the stack.
  //
  if (packetType != NS3_PACKET_OTHERHOST)
    {
      m_snifferTrace (originalPacket);
      m_macRxTrace (originalPacket);
      m_rxCallback (this, packet, protocol, header.GetSource ());
    }
}

void
EmuNetDevice::ReadThread (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  // It's important to remember that we're in a completely different thread than the simulator is running in.
  // We are talking about multiple threads here, so it is very, very dangerous to do any kind of reference couning
  // on a shared object.
  //

  int32_t len = -1;
  struct sockaddr_ll addr;
  socklen_t addrSize = sizeof (addr);

  for (;;) 
    {
      //
      // Too many pending reads at the same time leads to excessive memory allocations.  This counter prevents it.
      // 
      bool skip = false;

      {
        CriticalSection cs (m_pendingReadMutex);
        //std::cerr << std::endl << "EmuNetDevice read thread: m_pendingReadCount is " << m_pendingReadCount << std::endl;
        if (m_pendingReadCount >= m_maxPendingReads)
          {
            skip = true;
          }
        else
          {
            ++m_pendingReadCount;
          }
      }

      if (skip)
        {
          struct timespec time = { 0, 100000000L }; // 100 ms
          nanosleep (&time, NULL);
          continue;
        }

      //
      // to avoid any issues with a shared reference counted packet, we allocate a buffer on the heap and pass that
      // buffer into the ns-3 context thread where it will create the packet, copy the buffer and then free it.
      //
      uint32_t bufferSize = 65536;
      uint8_t *buf = (uint8_t *)malloc (bufferSize);
      if (buf == 0)
        {
          NS_FATAL_ERROR ("EmuNetDevice::ReadThread(): malloc packet buffer failed");
        }

      NS_LOG_LOGIC ("Calling recvfrom");
      len = recvfrom (m_sock, buf, bufferSize, 0, (struct sockaddr *)&addr, &addrSize);

      if (len == -1)
        {
          free (buf);
          buf = 0;
          return;
        }

      NS_LOG_INFO ("EmuNetDevice::EmuNetDevice(): Received packet on node " << m_nodeId);
      NS_LOG_INFO ("EmuNetDevice::ReadThread(): Scheduling handler");
      NS_ASSERT_MSG (m_rtImpl, "EmuNetDevice::ReadThread(): Realtime simulator implementation pointer not set");
      m_rtImpl->ScheduleRealtimeNowWithContext (m_nodeId, MakeEvent (&EmuNetDevice::ForwardUp, this, buf, len));
      buf = 0;
    }
}

bool 
EmuNetDevice::Send (Ptr<Packet> packet, const Address &dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (packet << dest << protocolNumber);
  //
  // The immediate questions here are how are we going to encapsulate packets and what do we use as the MAC source and 
  // destination (hardware) addresses?
  //
  // If we return false from EmuNetDevice::NeedsArp, the ArpIpv4Interface will pass the broadcast address as the 
  // hardware (Ethernet) destination by default.  If we return true from EmuNetDevice::NeedsArp, then the hardware
  // destination is actually meaningful, but we'll have an ns-3 ARP running on this device.  There can also be an ARP
  // running on the underlying OS so we have to be very careful, both about multiple ARPs and also about TCP, UDP, etc.
  //
  // We are operating in promiscuous mode on the receive side (all ns-3 net devices are required to implement the 
  // promiscuous callback in a meaningful way), so we have an option regarding the hardware addresses.  We don't actually have
  // to use the real hardware addresses and IP addresses of the underlying system.  We can completely use MAC-spoofing to
  // fake out the OS by using the ns-3 assigned MAC address (and also the ns-3 assigned IP addresses).  Ns-3 starts its 
  // MAC address allocation using the OUI (vendor-code) 00:00:00 which is unassigned to any organization and is a globally
  // administered address, so there shouldn't be any collisions with real hardware.
  //
  // So what we do is we return true from EmuNetDevice::NeedsArp which tells ns-3 to use its own ARP.  We spoof the 
  // MAC address of the device and use promiscuous mode to receive traffic destined to that address.
  //
  return SendFrom (packet, m_address, dest, protocolNumber);
}

bool 
EmuNetDevice::SendFrom (Ptr<Packet> packet, const Address &src, const Address &dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (packet << src << dest << protocolNumber);
  NS_LOG_LOGIC ("packet =" << packet);
  NS_LOG_LOGIC ("UID is " << packet->GetUid () << ")");

  if (IsLinkUp () == false)
    {
      m_macTxDropTrace (packet);
      return false;
    }

  Mac48Address destination = Mac48Address::ConvertFrom (dest);
  Mac48Address source = Mac48Address::ConvertFrom (src);

  NS_LOG_LOGIC ("Transmit packet with UID " << packet->GetUid ());
  NS_LOG_LOGIC ("Transmit packet from " << source);
  NS_LOG_LOGIC ("Transmit packet to " << destination);

  EthernetHeader header (false);
  header.SetSource (source);
  header.SetDestination (destination);

  switch (m_encapMode)
    {
    case LLC:
      {
        LlcSnapHeader llc;
        llc.SetType (protocolNumber);
        packet->AddHeader (llc);

        header.SetLengthType (packet->GetSize ());
      }
      break;

    case DIX:
      header.SetLengthType (protocolNumber);
      break;

    default:
      NS_FATAL_ERROR ("invalid encapsulation mode");
    }

  packet->AddHeader (header);

  //
  // there's not much meaning associated with the different layers in this
  // device, so don't be surprised when they're all stacked together in 
  // essentially one place.  We do this for trace consistency across devices.
  //
  m_macTxTrace (packet);

  // 
  // Enqueue and dequeue the packet to hit the queue tracing hooks.
  //
  m_queue->Enqueue (packet);
  packet = m_queue->Dequeue ();
  NS_ASSERT_MSG (packet, "EmuNetDevice::SendFrom(): packet zero from queue");

  m_promiscSnifferTrace (packet);
  m_snifferTrace (packet);

  struct sockaddr_ll ll;
  bzero (&ll, sizeof (ll));

  ll.sll_family = AF_PACKET;
  ll.sll_ifindex = m_sll_ifindex;
  ll.sll_protocol = htons (ETH_P_ALL);

  NS_LOG_LOGIC ("calling sendto");

  NS_ASSERT_MSG (packet->GetSize () <= 65536, "EmuNetDevice::SendFrom(): Packet too big " << packet->GetSize ());
  packet->CopyData (m_packetBuffer, packet->GetSize ());

  int32_t rc = sendto (m_sock, m_packetBuffer, packet->GetSize (), 0, reinterpret_cast<struct sockaddr *> (&ll), sizeof (ll));
  NS_LOG_LOGIC ("sendto returns " << rc);

  return rc == -1 ? false : true;
}

void 
EmuNetDevice::SetDataRate (DataRate bps)
{
  NS_LOG_FUNCTION (this << bps);
  NS_FATAL_ERROR ("EmuNetDevice::SetDataRate():  Unable."); 
}

void
EmuNetDevice::SetQueue (Ptr<Queue> q)
{
  NS_LOG_FUNCTION (this << q);
  m_queue = q;
}

Ptr<Queue> 
EmuNetDevice::GetQueue (void) const
{ 
  NS_LOG_FUNCTION_NOARGS ();
  return m_queue;
}

void
EmuNetDevice::NotifyLinkUp (void)
{
  m_linkUp = true;
  m_linkChangeCallbacks ();
}

void 
EmuNetDevice::SetIfIndex (const uint32_t index)
{
  m_ifIndex = index;
}

uint32_t 
EmuNetDevice::GetIfIndex (void) const
{
  return m_ifIndex;
}

Ptr<Channel> 
EmuNetDevice::GetChannel (void) const
{
  NS_FATAL_ERROR ("EmuNetDevice::GetChannel():  Unable."); 
  return 0;
}

void 
EmuNetDevice::SetAddress (Address address)
{
  NS_LOG_FUNCTION (address);
  m_address = Mac48Address::ConvertFrom (address);
}

Address 
EmuNetDevice::GetAddress (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_address;
}

bool 
EmuNetDevice::SetMtu (const uint16_t mtu)
{
  NS_FATAL_ERROR ("EmuNetDevice::SetMtu():  Unable."); 
  return false;
}

uint16_t 
EmuNetDevice::GetMtu (void) const
{
  struct ifreq ifr;
  bzero (&ifr, sizeof (ifr));
  strcpy (ifr.ifr_name, m_deviceName.c_str ());

  int32_t fd = socket (PF_INET, SOCK_DGRAM, IPPROTO_IP);


  int32_t rc = ioctl (fd, SIOCGIFMTU, &ifr);
  if (rc == -1)
    {
      NS_FATAL_ERROR ("EmuNetDevice::GetMtu(): Can't ioctl SIOCGIFMTU");
    }

  close (fd);

  return ifr.ifr_mtu;
}

bool 
EmuNetDevice::IsLinkUp (void) const
{
  return m_linkUp;
}

void 
EmuNetDevice::AddLinkChangeCallback (Callback<void> callback)
{
  m_linkChangeCallbacks.ConnectWithoutContext (callback);
}

bool 
EmuNetDevice::IsBroadcast (void) const
{
  return m_isBroadcast;
}

Address
EmuNetDevice::GetBroadcast (void) const
{
  return Mac48Address ("ff:ff:ff:ff:ff:ff");
}

bool 
EmuNetDevice::IsMulticast (void) const
{
  return m_isMulticast;
}

Address
EmuNetDevice::GetMulticast (Ipv4Address multicastGroup) const
{
  NS_LOG_FUNCTION (multicastGroup);

  Mac48Address ad = Mac48Address::GetMulticast (multicastGroup);

  //
  // Implicit conversion (operator Address ()) is defined for Mac48Address, so
  // use it by just returning the EUI-48 address which is automagically converted
  // to an Address.
  //
  NS_LOG_LOGIC ("multicast address is " << ad);

  return ad;
}

Address
EmuNetDevice::GetMulticast (Ipv6Address addr) const
{
  NS_LOG_FUNCTION (this << addr);

  Mac48Address ad = Mac48Address::GetMulticast (addr);
  NS_LOG_LOGIC ("MAC IPv6 multicast address is " << ad);

  return ad;
}

bool 
EmuNetDevice::IsPointToPoint (void) const
{
  return false;
}

bool 
EmuNetDevice::IsBridge (void) const
{
  return false;
}

void
EmuNetDevice::SetPromiscReceiveCallback (PromiscReceiveCallback cb)
{
  m_promiscRxCallback = cb;
}

bool
EmuNetDevice::SupportsSendFrom () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return true;
}


Ptr<Node> 
EmuNetDevice::GetNode (void) const
{
  return m_node;
}

void 
EmuNetDevice::SetNode (Ptr<Node> node)
{
  m_node = node;
}

bool 
EmuNetDevice::NeedsArp (void) const
{
  return true;
}

void 
EmuNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  m_rxCallback = cb;
}

} // namespace ns3
