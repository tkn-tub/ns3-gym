#include "net-device-container.h"

namespace ns3 {

NetDeviceContainer::Iterator 
NetDeviceContainer::Begin (void) const
{
  return m_devices.begin ();
}
NetDeviceContainer::Iterator 
NetDeviceContainer::End (void) const
{
  return m_devices.end ();
}

uint32_t 
NetDeviceContainer::GetN (void) const
{
  return m_devices.size ();
}
Ptr<NetDevice> 
NetDeviceContainer::Get (uint32_t i) const
{
  return m_devices[i];
}
void 
NetDeviceContainer::Add (NetDeviceContainer other)
{
  for (Iterator i = other.Begin (); i != other.End (); i++)
    {
      m_devices.push_back (*i);
    }
}
void 
NetDeviceContainer::Add (Ptr<NetDevice> device)
{
  m_devices.push_back (device);
}

} // namespace ns3
