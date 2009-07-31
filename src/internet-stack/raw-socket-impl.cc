#include "raw-socket-impl.h"
#include "ipv4-l3-protocol.h"
#include "icmpv4.h"
#include "ns3/inet-socket-address.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/log.h"
#include "ipv4-interface.h"
#include "udp-header.h"

NS_LOG_COMPONENT_DEFINE ("RawSocketImpl");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED (RawSocketImpl);

TypeId
RawSocketImpl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RawSocketImpl") .SetParent<Socket> () .AddAttribute ("Protocol", "Protocol number to match.", UintegerValue (0),
      MakeUintegerAccessor (&RawSocketImpl::m_protocol), MakeUintegerChecker<uint16_t> ()) .AddAttribute ("IcmpFilter",
      "Any icmp header whose type field matches a bit in this filter is dropped.", UintegerValue (0), MakeUintegerAccessor (
          &RawSocketImpl::m_icmpFilter), MakeUintegerChecker<uint32_t> ());
  return tid;
}

RawSocketImpl::RawSocketImpl ()
{
  NS_LOG_FUNCTION (this);
  m_err = Socket::ERROR_NOTERROR;
  m_node = 0;
  m_src = Ipv4Address::GetAny ();
  m_dst = Ipv4Address::GetAny ();
  m_protocol = 0;
  m_shutdownSend = false;
  m_shutdownRecv = false;
}

void
RawSocketImpl::SetNode (Ptr<Node> node)
{
  m_node = node;
  m_ipv4 = m_node->GetObject<Ipv4L3Protocol> ();
  NS_ASSERT(m_ipv4 != 0);
  for(uint32_t i = 0; i < m_ipv4->GetNInterfaces(); ++i)
  {
    m_interfaces.push_back (m_ipv4->GetInterface(i));
  }
}

void
RawSocketImpl::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_node = 0;
  Socket::DoDispose ();
}

enum Socket::SocketErrno
RawSocketImpl::GetErrno (void) const
{
  NS_LOG_FUNCTION (this);
  return m_err;
}
Ptr<Node>
RawSocketImpl::GetNode (void) const
{
  NS_LOG_FUNCTION (this);
  return m_node;
}
int
RawSocketImpl::Bind (const Address &address)
{
  NS_LOG_FUNCTION (this << address);
  if (!InetSocketAddress::IsMatchingType (address))
  {
    m_err = Socket::ERROR_INVAL;
    return -1;
  }
  InetSocketAddress ad = InetSocketAddress::ConvertFrom (address);
  m_src = ad.GetIpv4 ();
  m_sport = ad.GetPort();
  return 0;
}
int
RawSocketImpl::Bind (void)
{
  NS_LOG_FUNCTION (this);
  m_src = Ipv4Address::GetAny ();
  m_sport = 0;
  return 0;
}
int
RawSocketImpl::GetSockName (Address &address) const
{
  address = InetSocketAddress (m_src, 0);
  return 0;
}
int
RawSocketImpl::Close (void)
{
  NS_LOG_FUNCTION (this);
  Ptr<Ipv4L3Protocol> ipv4 = m_node->GetObject<Ipv4L3Protocol> ();
  if (ipv4 != 0)
  {
    ipv4->DeleteRawSocket (this);
  }
  return 0;
}
int
RawSocketImpl::ShutdownSend (void)
{
  NS_LOG_FUNCTION (this);
  m_shutdownSend = true;
  return 0;
}
int
RawSocketImpl::ShutdownRecv (void)
{
  NS_LOG_FUNCTION (this);
  m_shutdownRecv = true;
  return 0;
}
int
RawSocketImpl::Connect (const Address &address)
{
  NS_LOG_FUNCTION (this << address);
  if (!InetSocketAddress::IsMatchingType (address))
  {
    m_err = Socket::ERROR_INVAL;
    return -1;
  }
  InetSocketAddress ad = InetSocketAddress::ConvertFrom (address);
  m_dst = ad.GetIpv4 ();
  return 0;
}
int
RawSocketImpl::Listen (void)
{
  NS_LOG_FUNCTION (this);
  m_err = Socket::ERROR_OPNOTSUPP;
  return -1;
}
uint32_t
RawSocketImpl::GetTxAvailable (void) const
{
  NS_LOG_FUNCTION (this);
  return 0xffffffff;
}
int
RawSocketImpl::Send (Ptr<Packet> p, uint32_t flags)
{
  NS_LOG_FUNCTION (this << " packet " << p << flags);
  InetSocketAddress to = InetSocketAddress (m_dst, m_protocol);
  return SendTo (p, flags, to);
}
int
RawSocketImpl::SendTo (Ptr<Packet> packet, uint32_t flags, const Address &toAddress)
{

  if (!InetSocketAddress::IsMatchingType (toAddress))
  {
    m_err = Socket::ERROR_INVAL;
    return -1;
  }
  if (m_shutdownSend)
  {
    return 0;
  }


  if (m_ipv4->GetRoutingProtocol ())
  {
    Ipv4Header header;
    Ptr<Packet> copy = packet->Copy();
    copy->RemoveHeader (header);
    NS_LOG_LOGIC ("RawSocketImpl::SendTo packet uid " << packet->GetUid() << " address " << header.GetDestination());
    SocketErrno errno_ = ERROR_NOTERROR;//do not use errno as it is the standard C last error number
    Ptr<Ipv4Route> route;
    uint32_t oif = 0; //specify non-zero if bound to a source address
    route = m_ipv4->GetRoutingProtocol ()->RouteOutput (copy, header, oif, errno_);
    if (route != 0)
    {
      NS_LOG_LOGIC ("Route exists");
      SendByInterface(packet, route);
    }
    else
    {
      NS_LOG_LOGIC ("dropped because no outgoing route.");
    }
  }
  return 0;
}

void
RawSocketImpl::SendByInterface (Ptr<Packet> p, Ptr<const Ipv4Route> route)
{
  NS_ASSERT(m_node != 0);
  Ipv4Header ipv4Header;
  Ptr<Packet> packet = p->Copy();
  packet->RemoveHeader(ipv4Header);
  Ipv4Address source = ipv4Header.GetSource ();
  Ipv4Address destination = ipv4Header.GetDestination ();
  NS_LOG_LOGIC ("RawSocketImpl::SendByInterface to " << destination << " from " << source);

  // Handle a few cases:
  // 1) packet is destined to limited broadcast address
  // 2) packet is destined to a subnet-directed broadcast address
  // 3) packet is not broadcast, and is passed in with a route entry
  const Ipv4Address loopback ("127.0.0.1");
  // 1) packet is destined to limited broadcast address
  if (destination.IsBroadcast ())
    {
    NS_LOG_LOGIC ("RawSocketImpl::Send case 1:  limited broadcast");
      for (std::list< Ptr<Ipv4Interface> >::iterator ifaceIter = m_interfaces.begin (); ifaceIter != m_interfaces.end (); ifaceIter++)
        {
          Ptr<Ipv4Interface> outInterface = *ifaceIter;
          Ptr<Packet> packetCopy = p->Copy ();

          NS_ASSERT (packet->GetSize () <= outInterface->GetDevice()->GetMtu ());
          outInterface->Send (packetCopy, destination);
        }
      return;
    }

  // 2) check: packet is destined to a subnet-directed broadcast address
  bool result = false;
  uint32_t ifaceIndex = 0;
  NS_LOG_LOGIC("number of interfaces " << m_interfaces.size ());
  for (std::list< Ptr<Ipv4Interface> >::iterator ifaceIter = m_interfaces.begin (); ifaceIter != m_interfaces.end (); ifaceIter++, ifaceIndex++)
  {
    Ptr<Ipv4Interface> outInterface = *ifaceIter;
    for (uint32_t j = 0; j < m_ipv4->GetNAddresses (ifaceIndex); j++)
    {
      Ipv4InterfaceAddress ifAddr = m_ipv4->GetAddress (ifaceIndex, j);
      NS_LOG_LOGIC ("Testing address " << ifAddr.GetLocal () << " with mask " << ifAddr.GetMask ());
      if (destination.IsSubnetDirectedBroadcast (ifAddr.GetMask ()) &&
          (destination.CombineMask (ifAddr.GetMask ()) == ifAddr.GetLocal ().CombineMask (ifAddr.GetMask ())) )
        //if (ifAddr.GetLocal() == source)
        {
          NS_LOG_LOGIC ("Send case 2:  subnet directed bcast to " << ifAddr.GetLocal ());
          Ptr<Packet> packetCopy = p->Copy ();
          outInterface->Send (packetCopy, destination);
          result = true;
        }
    }
  }
  if(result) return;

  // 3) packet is not broadcast, and is passed in with a route entry
  //    with a valid Ipv4Address as the gateway
  if (route && route->GetGateway () != Ipv4Address ())
  {
    NS_LOG_UNCOND ("RawSocketImpl::Send case 3:  passed in with route");
    Ptr<NetDevice> outDev = route->GetOutputDevice ();
    int32_t interface = m_ipv4->GetInterfaceForDevice (outDev);
    NS_ASSERT (interface >= 0);
    Ptr<Ipv4Interface> outInterface = m_ipv4->GetInterface (interface);
    NS_LOG_UNCOND ("Send via NetDevice ifIndex " << outDev->GetIfIndex () << " ipv4InterfaceIndex " << interface);

    NS_ASSERT (p->GetSize () <= outInterface->GetDevice ()->GetMtu ());
    if (!route->GetGateway ().IsEqual (Ipv4Address ("0.0.0.0")))
    {
      if (outInterface->IsUp ())
      {
        NS_LOG_UNCOND ("Send to gateway " << route->GetGateway ());
        Ptr<Packet> packetCopy = p->Copy ();
        outInterface->Send (packetCopy, route->GetGateway ());
      }
      else
      {
        NS_LOG_UNCOND ("Dropping-- outgoing interface is down: " << route->GetGateway ());
      }
    }
       else
         {
           if (outInterface->IsUp ())
             {
             Ptr<Packet> packetCopy = p->Copy ();
             NS_LOG_UNCOND ("Send to destination " << destination);
               outInterface->Send (packetCopy, destination);
             }
           else
             {
             NS_LOG_UNCOND ("Dropping-- outgoing interface is down: " << destination);
             }
         }      return;
    }
}

uint32_t
RawSocketImpl::GetRxAvailable (void) const
{
  NS_LOG_FUNCTION (this);
  uint32_t rx = 0;
  for (std::list<Data>::const_iterator i = m_recv.begin (); i != m_recv.end (); ++i)
  {
    rx += (i->packet)->GetSize ();
  }
  return rx;
}
Ptr<Packet>
RawSocketImpl::Recv (uint32_t maxSize, uint32_t flags)
{
  NS_LOG_UNCOND ("RawSocketImpl::Recv" << maxSize << flags);
  Address tmp;
  return RecvFrom (maxSize, flags, tmp);
}
Ptr<Packet>
RawSocketImpl::RecvFrom (uint32_t maxSize, uint32_t flags, Address &fromAddress)
{
  NS_LOG_UNCOND ("RawSocketImpl::RecvFrom" << " maxSize " << maxSize << " flags " << flags << " address " <<fromAddress);
  if (m_recv.empty ())
  {
    return 0;
  }
  struct Data data = m_recv.front ();
  m_recv.pop_front ();
  if (data.packet->GetSize () > maxSize)
  {
    Ptr<Packet> first = data.packet->CreateFragment (0, maxSize);
    data.packet->RemoveAtStart (maxSize);
    m_recv.push_front (data);
    return first;
  }

  InetSocketAddress inet = InetSocketAddress (data.fromIp, data.fromProtocol);
  fromAddress = inet;
  return data.packet;
}

void
RawSocketImpl::SetProtocol (uint16_t protocol)
{
  NS_LOG_FUNCTION (this << protocol);
  m_protocol = protocol;
}

bool
RawSocketImpl::ForwardUp (Ptr<const Packet> p, Ptr<NetDevice> device)
{
  if (m_shutdownRecv)
  {
    return false;
  }
  Ptr<Packet> copy = p->Copy ();
  Ipv4Header ipHeader;
  copy->RemoveHeader(ipHeader);
  for (std::list< Ptr<Ipv4Interface> >::const_iterator ifaceIter = m_interfaces.begin (); ifaceIter != m_interfaces.end (); ifaceIter++)
  {
    for (uint32_t i = 0; i < (*ifaceIter)->GetNAddresses(); ++i)
    {
      if ((*ifaceIter)->GetAddress(i).GetLocal() == ipHeader.GetSource())
      return true;
    }
  }
  UdpHeader udpHeader;
  copy->RemoveHeader(udpHeader);
  NS_LOG_LOGIC ("src = " << m_src << " dst = " << m_dst);
  if ( m_sport == udpHeader.GetDestinationPort ()  && ipHeader.GetProtocol () == m_protocol)
  {

    if (m_protocol == 1)
    {
      Icmpv4Header icmpHeader;
      copy->PeekHeader (icmpHeader);
      uint8_t type = icmpHeader.GetType ();
      if (type < 32 && ((1 << type) & m_icmpFilter))
      {
        // filter out icmp packet.
        return false;
      }
    }
    Ptr<Packet> packet = p->Copy();
    struct Data data;
    data.packet = packet;
    data.fromIp = ipHeader.GetSource ();
    data.fromProtocol = ipHeader.GetProtocol ();
    m_recv.push_back (data);
    NotifyDataRecv ();
    return true;
  }
  return false;
}

} // namespace ns3
