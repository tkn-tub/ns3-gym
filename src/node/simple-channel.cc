#include "simple-channel.h"
#include "simple-net-device.h"
#include "ns3/packet.h"

namespace ns3 {

TypeId 
SimpleChannel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SimpleChannel")
    .SetParent<Channel> ()
    .AddConstructor<SimpleChannel> ()
    ;
  return tid;
}

SimpleChannel::SimpleChannel ()
{}

void
SimpleChannel::Send (Ptr<Packet> p, uint16_t protocol, 
		     Mac48Address to, Mac48Address from,
		     Ptr<SimpleNetDevice> sender)
{
  for (std::vector<Ptr<SimpleNetDevice> >::const_iterator i = m_devices.begin (); i != m_devices.end (); ++i)
    {
      Ptr<SimpleNetDevice> tmp = *i;
      if (tmp == sender)
	{
	  continue;
	}
      tmp->Receive (p->Copy (), protocol, to, from);
    }
}

void 
SimpleChannel::Add (Ptr<SimpleNetDevice> device)
{
  m_devices.push_back (device);
}

uint32_t 
SimpleChannel::GetNDevices (void) const
{
  return m_devices.size ();
}
Ptr<NetDevice> 
SimpleChannel::GetDevice (uint32_t i) const
{
  return m_devices[i];
}

} // namespace ns3
