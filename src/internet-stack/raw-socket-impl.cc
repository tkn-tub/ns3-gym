#include "raw-socket-impl.h"
#include "ipv4-l3-protocol.h"
#include "icmpv4.h"
#include "ns3/inet-socket-address.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/log.h"
#include "ipv4-interface.h"


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
  return 0;
}
int
RawSocketImpl::Bind (void)
{
  NS_LOG_FUNCTION (this);
  m_src = Ipv4Address::GetAny ();
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
  InetSocketAddress ad = InetSocketAddress::ConvertFrom (toAddress);
  Ptr<Ipv4L3Protocol> ipv4 = m_node->GetObject<Ipv4L3Protocol> ();
  NS_ASSERT(ipv4 != 0);
  Ipv4Address dst = ad.GetIpv4 ();
  NS_LOG_LOGIC ("RawSocketImpl::SendTo packet uid " << packet->GetUid() << " address "  << dst);
  if (ipv4->GetRoutingProtocol ())
  {
    Ipv4Header header;
    packet->PeekHeader(header);
    SocketErrno errno_ = ERROR_NOTERROR;//do not use errno as it is the standard C last error number
    Ptr<Ipv4Route> route;
    uint32_t oif = 0; //specify non-zero if bound to a source address
    route = ipv4->GetRoutingProtocol ()->RouteOutput (packet, header, oif, errno_);
    if (route != 0)
    {
      NS_LOG_LOGIC ("Route exists");
      Ptr<NetDevice> outDev = route->GetOutputDevice ();
      int32_t interface = ipv4->GetInterfaceForDevice (outDev);
      NS_ASSERT (interface >= 0);
      Ptr<Ipv4Interface> outInterface = ipv4->GetInterface (interface);
      NS_LOG_LOGIC ("Send via NetDevice ifIndex " << outDev->GetIfIndex () << " ipv4InterfaceIndex " << interface);

      NS_ASSERT (packet->GetSize () <= outInterface->GetDevice ()->GetMtu ());
      if (!route->GetGateway ().IsEqual (Ipv4Address ("0.0.0.0")))
      {
        if (outInterface->IsUp ())
        {
          NS_LOG_LOGIC ("Send to gateway " << route->GetGateway ());
          outInterface->Send (packet, route->GetGateway ());
        }
        else
        {
          NS_LOG_LOGIC ("Dropping-- outgoing interface is down: " << route->GetGateway ());
        }
      }
      else
      {
        if (outInterface->IsUp ())
        {
          NS_LOG_LOGIC ("Send to destination " << header.GetDestination ());
          outInterface->Send (packet, header.GetDestination ());
        }
        else
        {
          NS_LOG_LOGIC ("Dropping-- outgoing interface is down: " << header.GetDestination ());
        }
      }
    }
    else
    {
      NS_LOG_DEBUG ("dropped because no outgoing route.");
    }
  }
  return 0;
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
  NS_LOG_FUNCTION (this << maxSize << flags);
  Address tmp;
  return RecvFrom (maxSize, flags, tmp);
}
Ptr<Packet>
RawSocketImpl::RecvFrom (uint32_t maxSize, uint32_t flags, Address &fromAddress)
{
  NS_LOG_FUNCTION (this << maxSize << flags << fromAddress);
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
  NS_LOG_FUNCTION (this << *p  << device);
  if (m_shutdownRecv)
  {
    return false;
  } NS_LOG_LOGIC ("src = " << m_src << " dst = " << m_dst);
  Ptr<Packet> copy = p->Copy ();
  Ipv4Header ipHeader;
  copy->RemoveHeader (ipHeader);
  if ((m_src == Ipv4Address::GetAny () || ipHeader.GetDestination () == m_src) && (m_dst == Ipv4Address::GetAny () || ipHeader.GetSource () == m_dst)
      && ipHeader.GetProtocol () == m_protocol)
  {
    Ptr<Packet> copy = p->Copy ();
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
    struct Data data;
    data.packet = copy;
    data.fromIp = ipHeader.GetSource ();
    data.fromProtocol = ipHeader.GetProtocol ();
    m_recv.push_back (data);
    NotifyDataRecv ();
    return true;
  }
  return false;
}

} // namespace ns3
