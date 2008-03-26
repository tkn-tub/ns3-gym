#include "ipv4-interface-container.h"
#include "ns3/node-list.h"

namespace ns3 {

Ipv4InterfaceContainer::Ipv4InterfaceContainer ()
{}

uint32_t 
Ipv4InterfaceContainer::GetN (void) const
{
  return m_interfaces.size ();
}
Ipv4Address 
Ipv4InterfaceContainer::GetAddress (uint32_t i) const
{
  Ptr<Ipv4> ipv4 = m_interfaces[i].first;
  uint32_t interface = m_interfaces[i].second;
  return ipv4->GetAddress (interface);
}
void 
Ipv4InterfaceContainer::Add (Ptr<Ipv4> ipv4, uint32_t interface)
{
  m_interfaces.push_back (std::make_pair (ipv4, interface));
}

} // namespace ns3
