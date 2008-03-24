#include "packet-sink-helper.h"
#include "ns3/string.h"
#include "ns3/inet-socket-address.h"

namespace ns3 {

PacketSinkHelper::PacketSinkHelper ()
{
  m_factory.SetTypeId ("ns3::PacketSink");
}

void 
PacketSinkHelper::SetupUdp (Ipv4Address ip, uint16_t port)
{
  m_factory.Set ("Protocol", String ("ns3::Udp"));
  m_factory.Set ("Local", Address (InetSocketAddress (ip, port)));
}
void 
PacketSinkHelper::SetupTcp (Ipv4Address ip, uint16_t port)
{
  m_factory.Set ("Protocol", String ("ns3::Tcp"));
  m_factory.Set ("Local", Address (InetSocketAddress (ip, port)));
}

ApplicationContainer 
PacketSinkHelper::Build (NodeContainer c)
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
