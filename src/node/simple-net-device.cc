#include "simple-net-device.h"
#include "simple-channel.h"
#include "node.h"
#include "ns3/packet.h"

namespace ns3 {

TypeId 
SimpleNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SimpleNetDevice")
    .SetParent<NetDevice> ()
    .AddConstructor<SimpleNetDevice> ()
    ;
  return tid;
}

SimpleNetDevice::SimpleNetDevice ()
  : m_channel (0),
    m_node (0),
    m_mtu (0xffff),
    m_name (""),
    m_ifIndex (0)
{}

void 
SimpleNetDevice::Receive (Ptr<Packet> packet, uint16_t protocol, 
			  Mac48Address to, Mac48Address from)
{
  if (to == m_address || to == Mac48Address::GetBroadcast ())
    {
      m_rxCallback (this, packet, protocol, from);
    }
}

void 
SimpleNetDevice::SetChannel (Ptr<SimpleChannel> channel)
{
  m_channel = channel;
  m_channel->Add (this);
}

void 
SimpleNetDevice::SetAddress (Mac48Address address)
{
  m_address = address;
}

void 
SimpleNetDevice::SetName(const std::string name)
{
  m_name = name;
}
std::string 
SimpleNetDevice::GetName(void) const
{
  return m_name;
}
void 
SimpleNetDevice::SetIfIndex(const uint32_t index)
{
  m_ifIndex = index;
}
uint32_t 
SimpleNetDevice::GetIfIndex(void) const
{
  return m_ifIndex;
}
Ptr<Channel> 
SimpleNetDevice::GetChannel (void) const
{
  return m_channel;
}
Address 
SimpleNetDevice::GetAddress (void) const
{
  return m_address;
}
bool 
SimpleNetDevice::SetMtu (const uint16_t mtu)
{
  m_mtu = mtu;
  return true;
}
uint16_t 
SimpleNetDevice::GetMtu (void) const
{
  return m_mtu;
}
bool 
SimpleNetDevice::IsLinkUp (void) const
{
  return true;
}
void 
SimpleNetDevice::SetLinkChangeCallback (Callback<void> callback)
{}
bool 
SimpleNetDevice::IsBroadcast (void) const
{
  return true;
}
Address
SimpleNetDevice::GetBroadcast (void) const
{
  return Mac48Address ("ff:ff:ff:ff:ff:ff");
}
bool 
SimpleNetDevice::IsMulticast (void) const
{
  return false;
}
Address 
SimpleNetDevice::GetMulticast (void) const
{
  return Mac48Address ("01:00:5e:00:00:00");
}
Address 
SimpleNetDevice::MakeMulticastAddress (Ipv4Address multicastGroup) const
{
  return Mac48Address ("01:00:5e:00:00:00");
}
bool 
SimpleNetDevice::IsPointToPoint (void) const
{
  return false;
}
bool 
SimpleNetDevice::Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  Mac48Address to = Mac48Address::ConvertFrom (dest);
  m_channel->Send (packet, protocolNumber, to, m_address, this);
  return true;
}
Ptr<Node> 
SimpleNetDevice::GetNode (void) const
{
  return m_node;
}
void 
SimpleNetDevice::SetNode (Ptr<Node> node)
{
  m_node = node;
}
bool 
SimpleNetDevice::NeedsArp (void) const
{
  return false;
}
void 
SimpleNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  m_rxCallback = cb;
}

void
SimpleNetDevice::DoDispose (void)
{
  m_channel = 0;
  m_node = 0;
  NetDevice::DoDispose ();
}



} // namespace ns3
