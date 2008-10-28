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

#include "emulated-net-device.h"

#include "ns3/log.h"
#include "ns3/queue.h"
#include "ns3/simulator.h"
#include "ns3/realtime-simulator-impl.h"
#include "ns3/mac48-address.h"
#include "ns3/ethernet-header.h"
#include "ns3/ethernet-trailer.h"
#include "ns3/llc-snap-header.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/pointer.h"
#include "ns3/channel.h"
#include "ns3/system-thread.h"
#include "ns3/string.h"
#include "ns3/boolean.h"

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netpacket/packet.h>
#include <arpa/inet.h>

NS_LOG_COMPONENT_DEFINE ("EmulatedNetDevice");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (EmulatedNetDevice);

TypeId 
EmulatedNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EmulatedNetDevice")
    .SetParent<NetDevice> ()
    .AddConstructor<EmulatedNetDevice> ()
    .AddAttribute ("Address", 
                   "The ns-3 MAC address of this (virtual) device.",
                   Mac48AddressValue (Mac48Address ("ff:ff:ff:ff:ff:ff")),
                   MakeMac48AddressAccessor (&EmulatedNetDevice::m_address),
                   MakeMac48AddressChecker ())
    .AddAttribute ("DeviceName", 
                   "The name of the underlying real device (e.g. eth1).",
                   StringValue ("eth1"),
                   MakeStringAccessor (&EmulatedNetDevice::m_deviceName),
                   MakeStringChecker ())
    .AddAttribute ("Start", 
                   "The simulation time at which to spin up the device thread.",
                   TimeValue (Seconds (0.)),
                   MakeTimeAccessor (&EmulatedNetDevice::m_tStart),
                   MakeTimeChecker ())
    .AddAttribute ("Stop", 
                   "The simulation time at which to tear down the device thread.",
                   TimeValue (Seconds (0.)),
                   MakeTimeAccessor (&EmulatedNetDevice::m_tStop),
                   MakeTimeChecker ())
    .AddAttribute ("TxQueue", 
                   "A queue to use as the transmit queue in the device.",
                   PointerValue (),
                   MakePointerAccessor (&EmulatedNetDevice::m_queue),
                   MakePointerChecker<Queue> ())
    .AddTraceSource ("Rx", 
                     "Trace source to fire on reception of a MAC packet.",
                     MakeTraceSourceAccessor (&EmulatedNetDevice::m_rxTrace))
    .AddTraceSource ("Drop", 
                     "Trace source to fire on when a MAC packet is dropped.",
                     MakeTraceSourceAccessor (&EmulatedNetDevice::m_dropTrace))
    ;
  return tid;
}


EmulatedNetDevice::EmulatedNetDevice () 
: 
  m_startEvent (),
  m_stopEvent (),
  m_sock (-1),
  m_readThread (0),
  m_ifIndex (-1),
  m_sll_ifindex (-1),
  m_name ("Emulated NetDevice")
{
  NS_LOG_FUNCTION (this);
  Start (m_tStart);
}

EmulatedNetDevice::~EmulatedNetDevice ()
{
}

void 
EmulatedNetDevice::DoDispose()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = 0;
  NetDevice::DoDispose ();
}

void
EmulatedNetDevice::Start (Time tStart)
{
  NS_LOG_FUNCTION (tStart);

  //
  // Cancel any pending start event and schedule a new one at some relative time in the future.
  //
  Simulator::Cancel (m_startEvent);
  m_startEvent = Simulator::Schedule (tStart, &EmulatedNetDevice::StartDevice, this);
}

  void
EmulatedNetDevice::Stop (Time tStop)
{
  NS_LOG_FUNCTION (tStop);
  //
  // Cancel any pending stop event and schedule a new one at some relative time in the future.
  //
  Simulator::Cancel (m_stopEvent);
  m_startEvent = Simulator::Schedule (tStop, &EmulatedNetDevice::StopDevice, this);
}

  void
EmulatedNetDevice::StartDevice (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  //
  // Spin up the emulated net device and start receiving packets.
  //
  NS_ASSERT_MSG (m_sock == -1, "EmulatedNetDevice::StartDevice(): Device is already started");

  NS_LOG_LOGIC ("Creating socket");

  m_sock = socket (PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  //  m_sock = socket (AF_INET, SOCK_PACKET, htons(ETH_P_ALL));
  NS_ASSERT_MSG (m_sock != -1, "EmulatedNetDevice::StartDevice(): Unable to open socket");

  //
  // Figure out which interface index corresponds to the device name in the corresponding attribute.
  //
  struct ifreq ifr;
  bzero (&ifr, sizeof(ifr));
  strncpy ((char *)ifr.ifr_name, m_deviceName.c_str (), IFNAMSIZ);

  NS_LOG_LOGIC ("Getting interface index");
  int32_t rc = ioctl (m_sock, SIOCGIFINDEX, &ifr);
  NS_ASSERT_MSG (rc != -1, "EmulatedNetDevice::StartDevice(): Can't get interface index");

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
  NS_ASSERT_MSG (rc != -1, "EmulatedNetDevice::StartDevice(): Can't bind to specified interface");

  //
  // Now spin up a read thread to read packets.
  //
  NS_ASSERT_MSG (m_readThread == 0, "EmulatedNetDevice::StartDevice(): Receive thread is already running");

  NS_LOG_LOGIC ("Spinning up read thread");

  m_readThread = Create<SystemThread> (MakeCallback (&EmulatedNetDevice::ReadThread, this));
  m_readThread->Start ();

  NotifyLinkUp ();
}

void
EmulatedNetDevice::StopDevice (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  close (m_sock);
  m_sock = -1;

  NS_ASSERT_MSG (m_readThread != 0, "EmulatedNetDevice::StopDevice(): Receive thread is not running");

  NS_LOG_LOGIC ("Joining read thread");
  m_readThread->Join ();
  m_readThread = 0;
}

void
EmulatedNetDevice::ForwardUp (uint8_t *buf, uint32_t len)
{
  NS_LOG_FUNCTION (buf << len);

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

  //
  // An IP host group address is mapped to an Ethernet multicast address
  // by placing the low-order 23-bits of the IP address into the low-order
  // 23 bits of the Ethernet multicast address 01-00-5E-00-00-00 (hex).
  //
  // We are going to receive all packets destined to any multicast address,
  // which means clearing the low-order 23 bits the header destination 
  //
  Mac48Address mcDest;
  uint8_t      mcBuf[6];

  header.GetDestination ().CopyTo (mcBuf);
  mcBuf[3] &= 0x80;
  mcBuf[4] = 0;
  mcBuf[5] = 0;
  mcDest.CopyFrom (mcBuf);

  Mac48Address multicast = Mac48Address::ConvertFrom (GetMulticast ());
  Mac48Address broadcast = Mac48Address::ConvertFrom (GetBroadcast ());
  Mac48Address destination = Mac48Address::ConvertFrom (GetAddress ());

  LlcSnapHeader llc;
  packet->RemoveHeader (llc);
  uint16_t protocol = llc.GetType ();

  PacketType packetType;
      
  if (header.GetDestination () == broadcast)
    {
      NS_LOG_LOGIC ("Pkt destination is PACKET_BROADCAST");
      packetType = NS3_PACKET_BROADCAST;
    }
  else if (mcDest == multicast)
    {
      NS_LOG_LOGIC ("Pkt destination is PACKET_MULTICAST");
      packetType = NS3_PACKET_MULTICAST;
    }
  else if (header.GetDestination () == destination)
    {
      NS_LOG_LOGIC ("Pkt destination is PACKET_HOST");
      packetType = NS3_PACKET_HOST;
    }
  else
    {
      NS_LOG_LOGIC ("Pkt destination is PACKET_OTHERHOST");
      packetType = NS3_PACKET_OTHERHOST;
    }

  if (!m_promiscRxCallback.IsNull ())
    {
      NS_LOG_LOGIC ("calling m_promiscRxCallback");
      m_promiscRxCallback (this, packet->Copy (), protocol, header.GetSource (), header.GetDestination (), packetType);
    }

  if (packetType != NS3_PACKET_OTHERHOST)
    {
      m_rxTrace (originalPacket);
      NS_LOG_LOGIC ("calling m_rxCallback");
      m_rxCallback (this, packet, protocol, header.GetSource ());
    }
}

void
EmulatedNetDevice::ReadThread (void)
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
      NS_ASSERT_MSG (buf, "EmulatedNetDevice::ReadThread(): malloc packet buffer failed");
      NS_LOG_LOGIC ("Calling recvfrom");
      len = recvfrom (m_sock, buf, bufferSize, 0, (struct sockaddr *)&addr, &addrSize);

      if (len == -1)
        {
          free (buf);
          buf = 0;
          return;
        }

      NS_LOG_INFO ("EmulatedNetDevice::ReadThread(): Received packet");
      NS_LOG_INFO ("EmulatedNetDevice::ReadThread(): Scheduling handler");
      DynamicCast<RealtimeSimulatorImpl> (Simulator::GetImplementation ())->ScheduleRealtimeNow (
        MakeEvent (&EmulatedNetDevice::ForwardUp, this, buf, len));
      buf = 0;
    }
}

bool 
EmulatedNetDevice::Send (Ptr<Packet> packet, const Address &dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (packet << dest << protocolNumber);
  //
  // The immediate questions here are how are we going to encapsulate packets and what do we use as the MAC source and 
  // destination (hardware) addresses?
  //
  // If we return false from EmulatedNetDevice::NeedsArp, the ArpIpv4Interface will pass the broadcast address as the 
  // hardware (Ethernet) destination by default.  If we return true from EmulatedNetDevice::NeedsArp, then the hardware
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
  // So what we do is we return true from EmulatedNetDevice::NeedsArp which tells ns-3 to use its own ARP.  We spoof the 
  // MAC address of the device and use promiscuous mode to receive traffic destined to that address.
  //
  return SendFrom (packet, m_address, dest, protocolNumber);
}

bool 
EmulatedNetDevice::SendFrom (Ptr<Packet> packet, const Address &src, const Address &dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (packet << src << dest << protocolNumber);

  if (IsLinkUp () == false)
    {
      NS_LOG_LOGIC ("Link is down, returning");
      return false;
    }

  Mac48Address destination = Mac48Address::ConvertFrom (dest);
  Mac48Address source = Mac48Address::ConvertFrom (src);

  NS_LOG_LOGIC ("Transmit packet with UID " << packet->GetUid ());
  NS_LOG_LOGIC ("Transmit packet from " << source);
  NS_LOG_LOGIC ("Transmit packet to " << destination);

#if 0
  {
    struct ifreq ifr;
    bzero (&ifr, sizeof(ifr));
    strncpy ((char *)ifr.ifr_name, m_deviceName.c_str (), IFNAMSIZ);

    NS_LOG_LOGIC ("Getting MAC address");
    int32_t rc = ioctl (m_sock, SIOCGIFHWADDR, &ifr);
    NS_ASSERT_MSG (rc != -1, "EmulatedNetDevice::SendFrom(): Can't get MAC address");

    std::ostringstream oss;
    oss << std::hex <<
      (ifr.ifr_hwaddr.sa_data[0] & 0xff) << ":" <<
      (ifr.ifr_hwaddr.sa_data[1] & 0xff) << ":" <<
      (ifr.ifr_hwaddr.sa_data[2] & 0xff) << ":" <<
      (ifr.ifr_hwaddr.sa_data[3] & 0xff) << ":" <<
      (ifr.ifr_hwaddr.sa_data[4] & 0xff) << ":" <<
      (ifr.ifr_hwaddr.sa_data[5] & 0xff) << std::dec;

    NS_LOG_LOGIC ("Fixup source to HW MAC " << oss.str ());
    source = Mac48Address (oss.str ().c_str ());
    NS_LOG_LOGIC ("source now " << source);
  }
#endif

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
  // Enqueue and dequeue the packet to hit the tracing hooks.
  //
  m_queue->Enqueue (packet);
  packet = m_queue->Dequeue ();

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
EmulatedNetDevice::SetDataRate(DataRate bps)
{
  NS_LOG_FUNCTION (this << bps);
  NS_ASSERT_MSG (false, "EmulatedNetDevice::SetDataRate():  Unable."); 
}

void
EmulatedNetDevice::SetQueue (Ptr<Queue> q)
{
  NS_LOG_FUNCTION (this << q);
  m_queue = q;
}

Ptr<Queue> 
EmulatedNetDevice::GetQueue(void) const 
{ 
  NS_LOG_FUNCTION_NOARGS ();
  return m_queue;
}

void
EmulatedNetDevice::NotifyLinkUp (void)
{
  m_linkUp = true;
  if (!m_linkChangeCallback.IsNull ())
    {
      m_linkChangeCallback ();
    }
}

void 
EmulatedNetDevice::SetName(const std::string name)
{
  m_name = name;
}

std::string 
EmulatedNetDevice::GetName(void) const
{
  return m_name;
}

void 
EmulatedNetDevice::SetIfIndex(const uint32_t index)
{
  m_ifIndex = index;
}

uint32_t 
EmulatedNetDevice::GetIfIndex(void) const
{
  return m_ifIndex;
}

Ptr<Channel> 
EmulatedNetDevice::GetChannel (void) const
{
  NS_ASSERT_MSG (false, "EmulatedNetDevice::GetChannel():  Unable."); 
  return 0;
}

void 
EmulatedNetDevice::SetAddress (Mac48Address addr)
{
  NS_LOG_FUNCTION (addr);
  m_address = addr;
}

Address 
EmulatedNetDevice::GetAddress (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_address;
}

bool 
EmulatedNetDevice::SetMtu (const uint16_t mtu)
{
  NS_ASSERT_MSG (false, "EmulatedNetDevice::SetMtu():  Unable."); 
  return false;
}

uint16_t 
EmulatedNetDevice::GetMtu (void) const
{
  struct ifreq ifr;
  bzero (&ifr, sizeof (ifr));
  strcpy(ifr.ifr_name, m_deviceName.c_str ());

  int32_t fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

  int32_t rc = ioctl(fd, SIOCGIFMTU, &ifr);
  NS_ASSERT_MSG (rc != -1, "EmulatedNetDevice::GetMtu(): Can't ioctl SIOCGIFMTU");

  close (fd);

  return ifr.ifr_mtu;
}

bool 
EmulatedNetDevice::IsLinkUp (void) const
{
  return m_linkUp;
}

void 
EmulatedNetDevice::SetLinkChangeCallback (Callback<void> callback)
{
  m_linkChangeCallback = callback;
}

bool 
EmulatedNetDevice::IsBroadcast (void) const
{
  return true;
}

Address
EmulatedNetDevice::GetBroadcast (void) const
{
  return Mac48Address ("ff:ff:ff:ff:ff:ff");
}

bool 
EmulatedNetDevice::IsMulticast (void) const
{
  return false;
}

Address 
EmulatedNetDevice::GetMulticast (void) const
{
  return Mac48Address ("01:00:5e:00:00:00");
}

Address 
EmulatedNetDevice::MakeMulticastAddress (Ipv4Address multicastGroup) const
{
  return Mac48Address ("01:00:5e:00:00:00");
}

bool 
EmulatedNetDevice::IsPointToPoint (void) const
{
  return false;
}

void
EmulatedNetDevice::SetPromiscReceiveCallback (PromiscReceiveCallback cb)
{
  NS_ASSERT_MSG (false, "EmulatedNetDevice::SetPromiscReceiveCallback(): Not implemented");
}

  bool 
EmulatedNetDevice::SupportsSendFrom () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return true;
}


Ptr<Node> 
EmulatedNetDevice::GetNode (void) const
{
  return m_node;
}

void 
EmulatedNetDevice::SetNode (Ptr<Node> node)
{
  m_node = node;
}

bool 
EmulatedNetDevice::NeedsArp (void) const
{
  return true;
}

void 
EmulatedNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  m_rxCallback = cb;
}

} // namespace ns3
