#include "v4ping.h"
#include "ns3/icmpv4.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/socket.h"
#include "ns3/uinteger.h"
#include "ns3/inet-socket-address.h"
#include "ns3/packet.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/simulator.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("V4Ping");
NS_OBJECT_ENSURE_REGISTERED (V4Ping);

TypeId 
V4Ping::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::V4Ping")
    .SetParent<Application> ()
    .AddConstructor<V4Ping> ()
    .AddAttribute ("Remote", 
		   "The address of the machine we want to ping.",
		   Ipv4AddressValue (),
		   MakeIpv4AddressAccessor (&V4Ping::m_remote),
		   MakeIpv4AddressChecker ())
    .AddTraceSource ("Rtt",
		     "The rtt calculated by the ping.",
		     MakeTraceSourceAccessor (&V4Ping::m_traceRtt));
    ;
  return tid;
}

V4Ping::V4Ping ()
  : m_socket (0),
    m_seq (0)
{}
V4Ping::~V4Ping ()
{}

void
V4Ping::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_socket = 0;
  Application::DoDispose ();
}

uint32_t
V4Ping::GetApplicationId (void) const
{
  Ptr<Node> node = GetNode ();
  for (uint32_t i = 0; i < node->GetNApplications (); ++i)
    {
      if (node->GetApplication (i) == this)
	{
	  return i;
	}
    }
  NS_ASSERT_MSG (false, "forgot to add application to node");
  return 0; // quiet compiler
}

void
V4Ping::Receive (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);
  while (m_socket->GetRxAvailable () > 0)
    {
      Address from;
      Ptr<Packet> p = m_socket->RecvFrom (0xffffffff, 0, from);
      NS_LOG_DEBUG ("recv " << p->GetSize () << " bytes");
      NS_ASSERT (InetSocketAddress::IsMatchingType (from));
      InetSocketAddress realFrom = InetSocketAddress::ConvertFrom (from);
      NS_ASSERT (realFrom.GetPort () == 1); // protocol should be icmp.
      Ipv4Header ipv4;
      p->RemoveHeader (ipv4);
      NS_ASSERT (ipv4.GetProtocol () == 1); // protocol should be icmp.
      Icmpv4Header icmp;
      p->RemoveHeader (icmp);
      if (icmp.GetType () == Icmpv4Header::ECHO_REPLY)
	{
	  Icmpv4Echo echo;
	  p->RemoveHeader (echo);
	  if (echo.GetSequenceNumber () == (m_seq - 1) &&
	      echo.GetIdentifier () == 0)
	    {
	      Ptr<const Packet> data = echo.GetData ();
	      if (data->GetSize () == 16)
		{
		  uint32_t *buf = (uint32_t *)data->PeekData ();
		  if (buf[0] == GetNode ()->GetId () &&
		      buf[1] == GetApplicationId ())
		    {
		      int64_t ts = buf[3];
		      ts <<= 32;
		      ts |= buf[2];
		      Time sendTime = TimeStep (ts);
		      NS_ASSERT (Simulator::Now () > sendTime);
		      Time delta = Simulator::Now () - sendTime;
		      m_traceRtt (delta);
		    }
		}
	    }
	}
    }
}

void 
V4Ping::StartApplication (void)
{
  NS_LOG_FUNCTION (this);
  m_socket = Socket::CreateSocket (GetNode (), TypeId::LookupByName ("ns3::Ipv4RawSocketFactory"));
  NS_ASSERT (m_socket != 0);
  m_socket->SetAttribute ("Protocol", UintegerValue (1)); // icmp
  m_socket->SetRecvCallback (MakeCallback (&V4Ping::Receive, this));
  InetSocketAddress src = InetSocketAddress (Ipv4Address::GetAny (), 0);
  int status;
  status = m_socket->Bind (src);
  NS_ASSERT (status != -1);
  InetSocketAddress dst = InetSocketAddress (m_remote, 0);
  status = m_socket->Connect (dst);
  NS_ASSERT (status != -1);
  Ptr<Packet> p = Create<Packet> ();
  Icmpv4Echo echo;
  echo.SetSequenceNumber (m_seq);
  m_seq++;
  echo.SetIdentifier (0);
  uint32_t data[4];
  data[0] = GetNode ()->GetId ();
  data[1] = GetApplicationId ();
  int64_t now = Simulator::Now ().GetTimeStep ();
  data[2] = now & 0xffffffff;
  now >>= 32;
  data[3] = now & 0xffffffff;
  Ptr<Packet> dataPacket = Create<Packet> ((uint8_t *) &data, 16);
  echo.SetData (dataPacket);
  p->AddHeader (echo);
  Icmpv4Header header;
  header.SetType (Icmpv4Header::ECHO);
  header.SetCode (0);
  p->AddHeader (header);
  m_socket->Send (p, 0);
  
}
void 
V4Ping::StopApplication (void)
{
  NS_LOG_FUNCTION (this);
}


} // namespace ns3
