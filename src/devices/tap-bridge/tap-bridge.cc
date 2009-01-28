/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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

#include "tap-bridge.h"
#include "tap-encode-decode.h"

#include "ns3/node.h"
#include "ns3/channel.h"
#include "ns3/packet.h"
#include "ns3/ethernet-header.h"
#include "ns3/ethernet-trailer.h"
#include "ns3/log.h"
#include "ns3/boolean.h"
#include "ns3/string.h"
#include "ns3/simulator.h"
#include "ns3/realtime-simulator-impl.h"
#include "ns3/system-thread.h"

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

NS_LOG_COMPONENT_DEFINE ("TapBridge");

namespace ns3 {

#define TAP_MAGIC 95549

NS_OBJECT_ENSURE_REGISTERED (TapBridge);

TypeId
TapBridge::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TapBridge")
    .SetParent<NetDevice> ()
    .AddConstructor<TapBridge> ()
    .AddAttribute ("DeviceName", 
                   "The name of the tap device to create.",
                   StringValue (""),
                   MakeStringAccessor (&TapBridge::m_tapDeviceName),
                   MakeStringChecker ())
    .AddAttribute ("TapGateway", 
                   "The IP address of the default gateway to assign to the tap device.",
                   StringValue (""),
                   MakeStringAccessor (&TapBridge::m_tapGateway),
                   MakeStringChecker ())
    .AddAttribute ("TapIp", 
                   "The IP address to assign to the tap device.",
                   StringValue (""),
                   MakeStringAccessor (&TapBridge::m_tapIp),
                   MakeStringChecker ())
    .AddAttribute ("TapMac", 
                   "The MAC address to assign to the tap device.",
                   StringValue (""),
                   MakeStringAccessor (&TapBridge::m_tapMac),
                   MakeStringChecker ())
    .AddAttribute ("TapNetmask", 
                   "The network mask to assign to the tap device.",
                   StringValue (""),
                   MakeStringAccessor (&TapBridge::m_tapMac),
                   MakeStringChecker ())
    .AddAttribute ("Start", 
                   "The simulation time at which to spin up the tap device read thread.",
                   TimeValue (Seconds (0.)),
                   MakeTimeAccessor (&TapBridge::m_tStart),
                   MakeTimeChecker ())
    .AddAttribute ("Stop", 
                   "The simulation time at which to tear down the tap device read thread.",
                   TimeValue (Seconds (0.)),
                   MakeTimeAccessor (&TapBridge::m_tStop),
                   MakeTimeChecker ())
    ;
  return tid;
}

TapBridge::TapBridge ()
: m_node (0),
  m_ifIndex (0),
  m_mtu (0),
  m_sock (-1),
  m_startEvent (),
  m_stopEvent (),
  m_readThread (0)
{
  NS_LOG_FUNCTION_NOARGS ();
  Start (m_tStart);
}

TapBridge::~TapBridge()
{
  NS_LOG_FUNCTION_NOARGS ();
}

  void 
TapBridge::DoDispose ()
{
  NS_LOG_FUNCTION_NOARGS ();
  NetDevice::DoDispose ();
}

void
TapBridge::Start (Time tStart)
{
  NS_LOG_FUNCTION (tStart);

  //
  // Cancel any pending start event and schedule a new one at some relative time in the future.
  //
  Simulator::Cancel (m_startEvent);
  m_startEvent = Simulator::Schedule (tStart, &TapBridge::StartTapDevice, this);
}

  void
TapBridge::Stop (Time tStop)
{
  NS_LOG_FUNCTION (tStop);
  //
  // Cancel any pending stop event and schedule a new one at some relative time in the future.
  //
  Simulator::Cancel (m_stopEvent);
  m_startEvent = Simulator::Schedule (tStop, &TapBridge::StopTapDevice, this);
}

  void
TapBridge::StartTapDevice (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  //
  // Spin up the tap bridge and start receiving packets.
  //
  if (m_sock != -1)
    {
      NS_FATAL_ERROR ("TapBridge::StartTapDevice(): Tap is already started");
    }

  NS_LOG_LOGIC ("Creating tap device");

  //
  // Call out to a separate process running as suid root in order to get the 
  // tap device allocated and set up.  We do this to avoid having the entire 
  // simulation running as root.  If this method returns, we'll have a socket
  // waiting for us in m_sock that we can use to talk to the newly created 
  // tap device.
  //
  CreateTap ();

  //
  // Now spin up a read thread to read packets from the tap device.
  //
  if (m_readThread != 0)
    {
      NS_FATAL_ERROR ("TapBridge::StartTapDevice(): Receive thread is already running");
    }

  NS_LOG_LOGIC ("Spinning up read thread");

  m_readThread = Create<SystemThread> (MakeCallback (&TapBridge::ReadThread, this));
  m_readThread->Start ();
}

void
TapBridge::StopTapDevice (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  close (m_sock);
  m_sock = -1;

  NS_ASSERT_MSG (m_readThread != 0, "TapBridge::StopTapDevice(): Receive thread is not running");

  NS_LOG_LOGIC ("Joining read thread");
  m_readThread->Join ();
  m_readThread = 0;
}

void
TapBridge::CreateTap (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  //
  // We want to create a tap device on the host.  Unfortunately for us
  // you have to have root privileges to do that.  Instead of running the 
  // entire simulation as root, we decided to make a small program who's whole
  // reason for being is to run as suid root and do what it takes to create the
  // tap.  We're going to fork and exec that program soon, but we need to have 
  // a socket to talk to it with.  So we create a local interprocess (Unix) 
  // socket for that purpose.
  //
  int sock = socket (PF_UNIX, SOCK_DGRAM, 0);
  if (sock == -1)
    {
      NS_FATAL_ERROR ("TapBridge::CreateTap(): Unix socket creation error, errno = " << strerror (errno));
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
      NS_FATAL_ERROR ("TapBridge::CreateTap(): Could not bind(): errno = " << strerror (errno));
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
      NS_FATAL_ERROR ("TapBridge::CreateTap(): Could not getsockname(): errno = " << strerror (errno));
    }

  //
  // Now encode that socket name (family and path) as a string of hex digits
  //
  std::string path = TapBufferToString((uint8_t *)&un, len);
  NS_LOG_INFO ("Encoded Unix socket as \"" << path << "\"");
  //
  // Fork and exec the process to create our socket.  If we're us (the parent)
  // we wait for the child (the creator) to complete and read the  socket it created using the ancillary data mechanism.
  //
  pid_t pid = ::fork ();
  if (pid == 0)
    {
      NS_LOG_DEBUG ("Child process");

      //
      // build a command line argument from the encoded endpoint string that 
      // the socket creation process will use to figure out how to respond to
      // the (now) parent process.  We're going to have to give this program
      // quite a bit of information and we use program arguments to do so.
      //
      // -d<device-name> The name of the tap device we want to create;
      // -g<gateway-address> The IP address to use as the default gateway;
      // -i<IP-address> The IP address to assign to the new tap device;
      // -m<MAC-address> The MAC-48 address to assign to the new tap device;
      // -n<network-mask> The network mask to assign to the new tap device;
      // -p<path> the path to the unix socket described above.
      //
      // Example tap-sock-creator -dnewdev -g1.2.3.2 -i1.2.3.1 -m08:00:2e:00:01:23 -n255.255.255.0 -pblah
      //
      std::ostringstream oss;
      oss << "-d" << m_tapDeviceName << " -g" << m_tapGateway << " -i" << m_tapIp << " -m" << m_tapMac
          << " -n" << m_tapNetmask << " -p" << path;
      NS_LOG_INFO ("creator arguments set to \"" << oss.str () << "\"");

      //
      // Execute the socket creation process image.
      //
      status = ::execl (FindCreator ().c_str (), "tap-sock-creator", oss.str ().c_str (), (char *)NULL);

      //
      // If the execl successfully completes, it never returns.  If it returns it failed or the OS is
      // broken.  In either case, we bail.
      //
      NS_FATAL_ERROR ("TapBridge::CreateTap(): Back from execl(), errno = " << ::strerror (errno));
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
	  NS_FATAL_ERROR ("TapBridge::CreateTap(): waitpid() fails, errno = " << strerror (errno));
	}
      NS_ASSERT_MSG (pid == waited, "TapBridge::CreateTap(): pid mismatch");

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
              NS_FATAL_ERROR ("TapBridge::CreateTap(): socket creator exited normally with status " << exitStatus);
            }
	}
      else 
	{
          NS_FATAL_ERROR ("TapBridge::CreateTap(): socket creator exited abnormally");
	}

      //
      // At this point, the socket creator has run successfully and should 
      // have created our tap device, initialized it with the information we
      // passed and sent it back to the socket address we provided.  A socket
      // (fd) we can use to talk to this tap device should be waiting on the 
      // Unix socket we set up to receive information back from the creator
      // program.  We've got to do a bunch of grunt work to get at it, though.
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
      // Now we can actually receive the interesting bits from the tap
      // creator process.
      //
      ssize_t bytesRead = recvmsg (sock, &msg, 0);
      if (bytesRead != sizeof(int))
	{
          NS_FATAL_ERROR ("TapBridge::CreateTap(): Wrong byte count from socket creator");
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
              if (magic == TAP_MAGIC)
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
TapBridge::FindCreator (void)
{
  struct stat st;
  std::string debug = "./build/debug/src/devices/tap-bridge/tap-sock-creator";
  std::string optimized = "./build/optimized/src/devices/tap-bridge/tap-sock-creator";

  if (::stat (debug.c_str (), &st) == 0)
    {
      return debug;
    }

  if (::stat (optimized.c_str (), &st) == 0)
    {
      return optimized;
    }

  NS_FATAL_ERROR ("TapBridge::FindCreator(): Couldn't find creator");
  return ""; // quiet compiler
}

void
TapBridge::ReadThread (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  //
  // It's important to remember that we're in a completely different thread than the simulator is running in.  We
  // need to synchronize with that other thread to get the packet up into ns-3.  What we will need to do is to schedule 
  // a method to deal with the packet using the multithreaded simulator we are most certainly running.  However, I just 
  // said it -- we are talking about two threads here, so it is very, very dangerous to do any kind of reference couning
  // on a shared object.  Just don't do it.  So what we're going to do is to allocate a buffer on the heap and pass that
  // buffer into the ns-3 context thread where it will create the packet.
  //
  int32_t len = -1;

  for (;;) 
    {
      uint32_t bufferSize = 65536;
      uint8_t *buf = (uint8_t *)malloc (bufferSize);
      if (buf == 0)
        {
          NS_FATAL_ERROR ("TapBridge::ReadThread(): malloc packet buffer failed");
        }

      NS_LOG_LOGIC ("Calling read on tap device socket fd");
      len = read (m_sock, buf, bufferSize);

      if (len == -1)
        {
          free (buf);
          buf = 0;
          return;
        }

      NS_LOG_INFO ("TapBridge::ReadThread(): Received packet");
      NS_LOG_INFO ("TapBridge::ReadThread(): Scheduling handler");
      DynamicCast<RealtimeSimulatorImpl> (Simulator::GetImplementation ())->ScheduleRealtimeNow (
        MakeEvent (&TapBridge::ForwardToBridgedDevice, this, buf, len));
      buf = 0;
    }
}

void
TapBridge::ForwardToBridgedDevice (uint8_t *buf, uint32_t len)
{
  NS_LOG_FUNCTION (buf << len);

  //
  // Create a packet out of the buffer we received and free that buffer.
  //
  Ptr<Packet> packet = Create<Packet> (reinterpret_cast<const uint8_t *> (buf), len);
  free (buf);
  buf = 0;

  //
  // Checksum the packet
  //
  EthernetTrailer trailer;
  packet->RemoveTrailer (trailer);
  trailer.CheckFcs (packet);

  //
  // Get rid of the MAC header
  //
  EthernetHeader header (false);
  packet->RemoveHeader (header);

  NS_LOG_LOGIC ("Pkt source is " << header.GetSource ());
  NS_LOG_LOGIC ("Pkt destination is " << header.GetDestination ());

  m_bridgedDevice->SendFrom (packet, header.GetSource (), header.GetDestination (), 0x800);
}

void 
TapBridge::SetBridgedDevice (Ptr<NetDevice> bridgedDevice)
{
  NS_LOG_FUNCTION_NOARGS ();

  NS_ASSERT_MSG (m_node != 0, "TapBridge::SetBridgedDevice:  Bridge not installed in a node");
  NS_ASSERT_MSG (bridgedDevice != this, "TapBridge::SetBridgedDevice:  Cannot bridge to self");
  NS_ASSERT_MSG (m_bridgedDevice == 0, "TapBridge::SetBridgedDevice:  Already bridged");
  
  if (!Mac48Address::IsMatchingType (bridgedDevice->GetAddress ()))
    {
      NS_FATAL_ERROR ("TapBridge::SetBridgedDevice: Device does not support eui 48 addresses: cannot be added to bridge.");
    }
  
  if (!bridgedDevice->SupportsSendFrom ())
    {
      NS_FATAL_ERROR ("TapBridge::SetBridgedDevice: Device does not support SendFrom: cannot be added to bridge.");
    }

  //
  // Tell the bridged device to forward its received packets here.
  //
  m_node->RegisterProtocolHandler (MakeCallback (&TapBridge::ReceiveFromBridgedDevice, this), 0, bridgedDevice, true);
  m_bridgedDevice = bridgedDevice;
}

void
TapBridge::ReceiveFromBridgedDevice (
  Ptr<NetDevice> device, 
  Ptr<const Packet> packet, 
  uint16_t protocol,
  Address const &src, 
  Address const &dst, 
  PacketType packetType)
{
  NS_LOG_FUNCTION (device << packet << protocol << src << dst << packetType);
  NS_ASSERT_MSG (device == m_bridgedDevice, "TapBridge::SetBridgedDevice:  Received packet from unexpected device");
  
  NS_LOG_DEBUG ("Packet UID is " << packet->GetUid ());

  Mac48Address from = Mac48Address::ConvertFrom (src);
  Mac48Address to = Mac48Address::ConvertFrom (dst);

  //
  // We have received a packet from the ns-3 net device that has been associated
  // with this bridge.  We want to take these bits and send them off to the 
  // Tap device on the Linux host.  Once we do this, the bits in the packet will
  // percolate up through the stack on the Linux host.
  //
  // The ns-3 net device that is the source of these bits has removed the MAC 
  // header, so we have to put one back on.
  //
  Ptr<Packet> p = packet->Copy ();
  EthernetHeader header = EthernetHeader (false);
  header.SetSource (from);
  header.SetDestination (to);
  header.SetLengthType (0x800);
  p->AddHeader (header);

  write (m_sock, p->PeekData (), p->GetSize ());
}

void 
TapBridge::SetName(const std::string name)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_name = name;
}

std::string 
TapBridge::GetName(void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_name;
}

void 
TapBridge::SetIfIndex(const uint32_t index)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_ifIndex = index;
}

uint32_t 
TapBridge::GetIfIndex(void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_ifIndex;
}

Ptr<Channel> 
TapBridge::GetChannel (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return 0;
}

Address 
TapBridge::GetAddress (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_address;
}

bool 
TapBridge::SetMtu (const uint16_t mtu)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_mtu = mtu;
  return true;
}

uint16_t 
TapBridge::GetMtu (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_mtu;
}


bool 
TapBridge::IsLinkUp (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return true;
}

void 
TapBridge::SetLinkChangeCallback (Callback<void> callback)
{
  NS_LOG_FUNCTION_NOARGS ();
}

bool 
TapBridge::IsBroadcast (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return true;
}

Address
TapBridge::GetBroadcast (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return Mac48Address ("ff:ff:ff:ff:ff:ff");
}

bool
TapBridge::IsMulticast (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return true;
}

Address
TapBridge::GetMulticast (Ipv4Address multicastGroup) const
{
 NS_LOG_FUNCTION (this << multicastGroup);
 Mac48Address multicast = Mac48Address::GetMulticast (multicastGroup);
 return multicast;
}

bool 
TapBridge::IsPointToPoint (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return false;
}

bool 
TapBridge::IsBridge (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return true;
}

bool 
TapBridge::Send (Ptr<Packet> packet, const Address& dst, uint16_t protocol)
{
  NS_LOG_FUNCTION (packet << dst << protocol);
  NS_FATAL_ERROR ("TapBridge::Send: You may not call Send on a TapBridge directly");
  return false;
}

bool 
TapBridge::SendFrom (Ptr<Packet> packet, const Address& src, const Address& dst, uint16_t protocol)
{
  NS_LOG_FUNCTION (packet << src << dst << protocol);
  NS_FATAL_ERROR ("TapBridge::Send: You may not call SendFrom on a TapBridge directly");
  return false;
}

Ptr<Node> 
TapBridge::GetNode (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_node;
}

void 
TapBridge::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = node;
}

bool 
TapBridge::NeedsArp (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return true;
}

void 
TapBridge::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_rxCallback = cb;
}

void 
TapBridge::SetPromiscReceiveCallback (NetDevice::PromiscReceiveCallback cb)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_promiscRxCallback = cb;
}

bool
TapBridge::SupportsSendFrom () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return true;
}

Address TapBridge::GetMulticast (Ipv6Address addr) const
{
  NS_LOG_FUNCTION (this << addr);
  return Mac48Address::GetMulticast (addr);
}

} // namespace ns3
