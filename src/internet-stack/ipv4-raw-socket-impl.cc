#include "ipv4-raw-socket-impl.h"
#include "ipv4-l3-protocol.h"
#include "icmpv4.h"
#include "ns3/inet-socket-address.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("Ipv4RawSocketImpl");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (Ipv4RawSocketImpl);

TypeId 
Ipv4RawSocketImpl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ipv4RawSocketImpl")
    .SetParent<Socket> ()
    .AddAttribute ("Protocol", "Protocol number to match.",
		   UintegerValue (0),
		   MakeUintegerAccessor (&Ipv4RawSocketImpl::m_protocol),
		   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("IcmpFilter", 
		   "Any icmp header whose type field matches a bit in this filter is dropped.",
		   UintegerValue (0),
		   MakeUintegerAccessor (&Ipv4RawSocketImpl::m_icmpFilter),
		   MakeUintegerChecker<uint32_t> ())
    ;
  return tid;
}

Ipv4RawSocketImpl::Ipv4RawSocketImpl ()
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
Ipv4RawSocketImpl::SetNode (Ptr<Node> node)
{
  m_node = node;
}

void
Ipv4RawSocketImpl::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_node = 0;
  Socket::DoDispose ();
}

enum Socket::SocketErrno 
Ipv4RawSocketImpl::GetErrno (void) const
{
  NS_LOG_FUNCTION (this);
  return m_err;
}
Ptr<Node> 
Ipv4RawSocketImpl::GetNode (void) const
{
  NS_LOG_FUNCTION (this);
  return m_node;
}
int 
Ipv4RawSocketImpl::Bind (const Address &address)
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
Ipv4RawSocketImpl::Bind (void)
{
  NS_LOG_FUNCTION (this);
  m_src = Ipv4Address::GetAny ();
  return 0;
}
int 
Ipv4RawSocketImpl::GetSockName (Address &address) const
{
  address = InetSocketAddress (m_src, 0);
  return 0;
}
int 
Ipv4RawSocketImpl::Close (void)
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
Ipv4RawSocketImpl::ShutdownSend (void)
{
  NS_LOG_FUNCTION (this);
  m_shutdownSend = true;
  return 0;
}
int 
Ipv4RawSocketImpl::ShutdownRecv (void)
{
  NS_LOG_FUNCTION (this);
  m_shutdownRecv = true;
  return 0;
}
int 
Ipv4RawSocketImpl::Connect (const Address &address)
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
Ipv4RawSocketImpl::Listen (void)
{
  NS_LOG_FUNCTION (this);
  m_err = Socket::ERROR_OPNOTSUPP;
  return -1;
}
uint32_t 
Ipv4RawSocketImpl::GetTxAvailable (void) const
{
  NS_LOG_FUNCTION (this);
  return 0xffffffff;
}
int 
Ipv4RawSocketImpl::Send (Ptr<Packet> p, uint32_t flags)
{
  NS_LOG_FUNCTION (this << p << flags);
  InetSocketAddress to = InetSocketAddress (m_dst, m_protocol);
  return SendTo (p, flags, to);
}
int 
Ipv4RawSocketImpl::SendTo (Ptr<Packet> p, uint32_t flags, 
			   const Address &toAddress)
{
  NS_LOG_FUNCTION (this << p << flags << toAddress);
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
  Ipv4Address dst = ad.GetIpv4 ();
  uint32_t localIfIndex;
  if (ipv4->GetIfIndexForDestination(dst, localIfIndex))
    {
      ipv4->Send (p, ipv4->GetAddress (localIfIndex), dst, m_protocol);
    }
  else
    {
      NS_LOG_DEBUG ("dropped because no outgoing route.");
    }
  return 0;
}
uint32_t 
Ipv4RawSocketImpl::GetRxAvailable (void) const
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
Ipv4RawSocketImpl::Recv (uint32_t maxSize, uint32_t flags)
{
  NS_LOG_FUNCTION (this << maxSize << flags);
  Address tmp;
  return RecvFrom (maxSize, flags, tmp);
}
Ptr<Packet> 
Ipv4RawSocketImpl::RecvFrom (uint32_t maxSize, uint32_t flags,  
			     Address &fromAddress)
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
Ipv4RawSocketImpl::SetProtocol (uint16_t protocol)
{
  NS_LOG_FUNCTION (this << protocol);
  m_protocol = protocol;
}

bool 
Ipv4RawSocketImpl::ForwardUp (Ptr<const Packet> p, Ipv4Header ipHeader, Ptr<NetDevice> device)
{
  NS_LOG_FUNCTION (this << *p << ipHeader << device);
  if (m_shutdownRecv)
    {
      return false;
    }
  if ((m_src == Ipv4Address::GetAny () || ipHeader.GetDestination () == m_src) &&
      (m_dst == Ipv4Address::GetAny () || ipHeader.GetSource () == m_dst) &&
      ipHeader.GetProtocol () == m_protocol)
    {
      Ptr<Packet> copy = p->Copy ();
      if (m_protocol == 1)
	{
	  Icmpv4Header icmpHeader;
	  copy->PeekHeader (icmpHeader);
	  uint8_t type = icmpHeader.GetType ();
	  if (type < 32 && 
	      ((1 << type) & m_icmpFilter))
	    {
	      // filter out icmp packet.
	      return false;
	    }
	}
      copy->AddHeader (ipHeader);
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
