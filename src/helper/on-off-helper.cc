#include "on-off-helper.h"
#include "ns3/inet-socket-address.h"
#include "ns3/packet-socket-address.h"
#include "ns3/string.h"

namespace ns3 {

OnOffHelper::OnOffHelper ()
{
  m_factory.SetTypeId ("ns3::OnOffApplication");
}

void 
OnOffHelper::SetUdpRemote (Ipv4Address ip, uint16_t port)
{
  m_factory.Set ("Protocol", String ("ns3::Udp"));
  m_factory.Set ("Remote", Address (InetSocketAddress (ip, port)));
}
void 
OnOffHelper::SetTcpRemote (Ipv4Address ip, uint16_t port)
{
  m_factory.Set ("Protocol", String ("ns3::Tcp"));
  m_factory.Set ("Remote", Address (InetSocketAddress (ip, port)));
}
void 
OnOffHelper::SetPacketRemote (Ptr<NetDevice> source, Address destination, uint16_t protocol)
{
  PacketSocketAddress packet;
  packet.SetSingleDevice (source->GetIfIndex ());
  packet.SetPhysicalAddress (destination);
  packet.SetProtocol (protocol);
  m_factory.Set ("Protocol", String ("ns3::PacketSocketFactory"));
  m_factory.Set ("Remote", Address (packet));
}
  
void 
OnOffHelper::SetAppAttribute (std::string name, Attribute value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
OnOffHelper::Build (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<Application> app = m_factory.Create<Application> ();
      node->AddApplication (app);
      apps.Add (app);
    }
  return apps;
}


} // namespace ns3
