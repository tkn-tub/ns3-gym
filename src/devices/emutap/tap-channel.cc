#include "tap-channel.h"
#include "host-tap-net-device.h"
#include "tap-net-device.h"

namespace ns3 {


TapChannel::TapChannel ()
  : m_hostDevice (0),
    m_device (0)
{}

void
TapChannel::DoDispose (void)
{
  m_device = 0;
  m_hostDevice = 0;
  Channel::DoDispose ();
}

void
TapChannel::SetDevice (Ptr<TapNetDevice> device)
{
  m_device = device;
}

void
TapChannel::SetHostDevice (Ptr<HostTapNetDevice> device)
{
  m_hostDevice = device;
}

Ptr<HostTapNetDevice> 
TapChannel::GetHostDevice (void) const
{
  return m_hostDevice;
}

uint32_t 
TapChannel::GetNDevices (void) const
{
  return 2;
}
Ptr<NetDevice> 
TapChannel::GetDevice (uint32_t i) const
{
  if (i == 0)
    {
      return m_device;
    }
  else if (i == 1)
    {
      return m_hostDevice;
    }
  else
    {
      return 0;
    }
}

} // namespace ns3
