#include "udp-echo-helper.h"
#include "ns3/udp-echo-server.h"
#include "ns3/udp-echo-client.h"
#include "ns3/uinteger.h"

namespace ns3 {

UdpEchoServerHelper::UdpEchoServerHelper ()
  : m_port (9)
{}

void 
UdpEchoServerHelper::SetPort (uint16_t port)
{
  m_port = port;
}
ApplicationContainer 
UdpEchoServerHelper::Build (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<UdpEchoServer> server = CreateObject<UdpEchoServer> ("Port", Uinteger (m_port));
      node->AddApplication (server);
      apps.Add (server);
    }
  return apps;
}

UdpEchoClientHelper::UdpEchoClientHelper ()
{
  m_factory.SetTypeId (UdpEchoClient::GetTypeId ());
}
void 
UdpEchoClientHelper::SetRemote (Ipv4Address ip, uint16_t port)
{
  m_remoteIp = ip;
  m_remotePort = port;
}
void 
UdpEchoClientHelper::SetAppAttribute (std::string name, Attribute value)
{
  m_factory.Set (name, value);
}

ApplicationContainer 
UdpEchoClientHelper::Build (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<UdpEchoClient> client = m_factory.Create<UdpEchoClient> ();
      client->SetRemote (m_remoteIp, m_remotePort);
      node->AddApplication (client);
      apps.Add (client);
    }
  return apps;  
}



} // namespace ns3
