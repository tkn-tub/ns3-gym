#include "net-device-container.h"

namespace ns3 {

NetDeviceContainer::Iterator 
NetDeviceContainer::Begin (void) const
{
  return m_nodes.begin ();
}
NetDeviceContainer::Iterator 
NetDeviceContainer::End (void) const
{
  return m_nodes.end ();
}

uint32_t 
NetDeviceContainer::GetN (void) const
{
  return m_nodes.size ();
}
Ptr<NetDevice> 
NetDeviceContainer::Get (uint32_t i) const
{
  return m_nodes[i];
}
void 
NetDeviceContainer::Add (NetDeviceContainer other)
{
  for (Iterator i = other.Begin (); i != other.End (); i++)
    {
      m_nodes.push_back (*i);
    }
}
void 
NetDeviceContainer::Add (Ptr<NetDevice> node)
{
  m_nodes.push_back (node);
}

} // namespace ns3
