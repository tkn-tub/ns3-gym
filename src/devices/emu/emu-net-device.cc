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
#include "ns3/realtime-simulator-impl.h"
#include "ns3/mac48-address.h"

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
    .AddAttribute ("Address", 
                   "The ns-3 MAC address of this (virtual) device.",
                   Mac48AddressValue (Mac48Address ("ff:ff:ff:ff:ff:ff")),
                   MakeMac48AddressAccessor (&EmuNetDevice::m_address),
                   MakeMac48AddressChecker ())
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
  m_ifIndex (std::numeric_limits<uint32_t>::max ()),  // absurdly large value
  m_sll_ifindex (-1)
{
  NS_LOG_FUNCTION (this);
  Start (m_tStart);
}

EmuNetDevice::~EmuNetDevice ()
{
}

void 
EmuNetDevice::DoDispose()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = 0;
  NetDevice::DoDispose ();
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
  ll.sll_protocol = htons(ETH_P_ALL); 

  NS_LOG_LOGIC ("Binding socket to interface");

  rc = bind (m_sock, (struct sockaddr *)&ll, sizeof (ll));
  if (rc == -1)
    {
      NS_FATAL_ERROR ("EmuNetDevice::StartDevice(): Can't bind to specified interface");
    }

  rc = ioctl(m_sock, SIOCGIFFLAGS, &ifr);
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
  std::string path = EmuBufferToString((uint8_t *)&un, len);
  NS_LOG_INFO ("Encoded Unix socket as \"" << path << "\"");
  //
  // Fork and exec the process to create our socket.  If we're us (the parent)
  // we wait for the child (the socket creator) to complete and read the 
  // socket it created using the ancillary data mechanism.
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
      status = ::execl (FindCreator ().c_str (), "emu-sock-creator", oss.str ().c_str (), (char *)NULL);

      //
      // If the execl successfully completes, it never returns.  If it returns it failed or the OS is
      // broken.  In either case, we bail.
      //
      NS_FATAL_ERROR ("EmuNetDevice::CreateSocket(): Back from execl(), errno = " << ::strerror (errno));
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
      char control[CMSG_SPACE(msg_size)];

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
      for (cmsg = CMSG_FIRSTHDR(&msg); cmsg != NULL; cmsg = CMSG_NXTHDR(&msg, cmsg)) 
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

std::string
EmuNetDevice::FindCreator (void)
{
  struct stat st;
  std::string debug = "./build/debug/src/devices/emu/emu-sock-creator";
  std::string optimized = "./build/optimized/src/devices/emu/emu-sock-creator";

  if (::stat (debug.c_str (), &st) == 0)
    {
      return debug;
    }

  if (::stat (optimized.c_str (), &st) == 0)
    {
      return optimized;
    }

  NS_FATAL_ERROR ("EmuNetDevice::FindCreator(): Couldn't find creator");
  return ""; // quiet compiler
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

  /* IPv6 support*/
  uint8_t mac[6];
  Mac48Address multicast6AllNodes("33:33:00:00:00:01");
  Mac48Address multicast6AllRouters("33:33:00:00:00:02");
  Mac48Address multicast6AllHosts("33:33:00:00:00:03");
  Mac48Address multicast6Node; /* multicast address addressed to our MAC address */

  /* generate IPv6 multicast ethernet destination that nodes will accept */
  GetAddress().CopyTo(mac);
  mac[0]=0x33;
  mac[1]=0x33;
  multicast6Node.CopyFrom(mac);

  //
  // Create a packet out of the buffer we received and free that buffer.
  //
  Ptr<Packet> packet = Create<Packet> (reinterpret_cast<const uint8_t *> (buf), len);
  free (buf);
  buf = 0;

  //
  // Trace sinks will expect complete packets, not packets without some of the
  // headers.
  //
  Ptr<Packet> originalPacket = packet->Copy ();

  //
  // Checksum the packet
  //
  EthernetTrailer trailer;
  packet->RemoveTrailer (trailer);
  trailer.CheckFcs (packet);

  EthernetHeader header (false);
  packet->RemoveHeader (header);

  NS_LOG_LOGIC ("Pkt source is " << header.GetSource ());
  NS_LOG_LOGIC ("Pkt destination is " << header.GetDestination ());

  uint16_t protocol;
  
  //
  // If the length/type is less than 1500, it corresponds to a length 
  // interpretation packet.  In this case, it is an 802.3 packet and 
  // will also have an 802.2 LLC header.  If greater than 1500, we
  // find the protocol number (Ethernet type) directly.
  //
  if (header.GetLengthType () <= 1500)
    {
      LlcSnapHeader llc;
      packet->RemoveHeader (llc);
      protocol = llc.GetType ();
    }
  else
    {
      protocol = header.GetLengthType ();
    }

  PacketType packetType;
      
  if (header.GetDestination ().IsBroadcast ())
    {
      packetType = NS3_PACKET_BROADCAST;
    }
  else if (header.GetDestination ().IsMulticast () ||
           header.GetDestination() == multicast6Node ||
           header.GetDestination() == multicast6AllNodes ||
           header.GetDestination() == multicast6AllRouters ||
           header.GetDestination() == multicast6AllHosts)
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

  //
  // It's important to remember that we're in a completely different thread than the simulator is running in.  We
  // need to synchronize with that other thread to get the packet up into ns-3.  What we will need to do is to schedule 
  // a method to forward up the packet using the multithreaded simulator we are most certainly running.  However, I just 
  // said it -- we are talking about two threads here, so it is very, very dangerous to do any kind of reference couning
  // on a shared object.  Just don't do it.  So what we're going to do is to allocate a buffer on the heap and pass that
  // buffer into the ns-3 context thread where it will create the packet.
  //

  int32_t len = -1;
  struct sockaddr_ll addr;
  socklen_t addrSize = sizeof (addr);

  for (;;) 
    {
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

      NS_LOG_INFO ("EmuNetDevice::ReadThread(): Received packet");
      NS_LOG_INFO ("EmuNetDevice::ReadThread(): Scheduling handler");
      DynamicCast<RealtimeSimulatorImpl> (Simulator::GetImplementation ())->ScheduleRealtimeNow (
        MakeEvent (&EmuNetDevice::ForwardUp, this, buf, len));
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

  //
  // We've got to pick either DIX (Ethernet) or LLC/SNAP (IEEE 802.3) as a 
  // packet format.  IEEE 802.3 is slightly more formally correct, so we 
  // go that route.
  //
  LlcSnapHeader llc;
  llc.SetType (protocolNumber);
  packet->AddHeader (llc);

  EthernetHeader header (false);
  header.SetSource (source);
  header.SetDestination (destination);
  header.SetLengthType (packet->GetSize ());
  packet->AddHeader (header);

  EthernetTrailer trailer;
  trailer.CalcFcs (packet);
  packet->AddTrailer (trailer);

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
  ll.sll_protocol = htons(ETH_P_ALL); 

  NS_LOG_LOGIC ("calling sendto");

  int32_t rc;
  rc = sendto (m_sock, packet->PeekData (), packet->GetSize (), 0, reinterpret_cast<struct sockaddr *> (&ll), sizeof (ll));
  NS_LOG_LOGIC ("sendto returns " << rc);

  return rc == -1 ? false : true;
}

void 
EmuNetDevice::SetDataRate(DataRate bps)
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
EmuNetDevice::GetQueue(void) const 
{ 
  NS_LOG_FUNCTION_NOARGS ();
  return m_queue;
}

void
EmuNetDevice::NotifyLinkUp (void)
{
  m_linkUp = true;
  if (!m_linkChangeCallback.IsNull ())
    {
      m_linkChangeCallback ();
    }
}

void 
EmuNetDevice::SetIfIndex(const uint32_t index)
{
  m_ifIndex = index;
}

uint32_t 
EmuNetDevice::GetIfIndex(void) const
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
EmuNetDevice::SetAddress (Mac48Address addr)
{
  NS_LOG_FUNCTION (addr);
  m_address = addr;
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
  strcpy(ifr.ifr_name, m_deviceName.c_str ());

  int32_t fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);


  int32_t rc = ioctl(fd, SIOCGIFMTU, &ifr);
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
EmuNetDevice::SetLinkChangeCallback (Callback<void> callback)
{
  m_linkChangeCallback = callback;
}

bool 
EmuNetDevice::IsBroadcast (void) const
{
  return true;
}

Address
EmuNetDevice::GetBroadcast (void) const
{
  return Mac48Address ("ff:ff:ff:ff:ff:ff");
}

bool 
EmuNetDevice::IsMulticast (void) const
{
  return false;
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
  NS_LOG_FUNCTION(this << addr);

  Mac48Address ad = Mac48Address::GetMulticast (addr);
  NS_LOG_LOGIC("MAC IPv6 multicast address is " << ad);

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
  NS_FATAL_ERROR ("EmuNetDevice::SetPromiscReceiveCallback(): Not implemented");
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
