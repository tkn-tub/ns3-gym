#include "inet-address.h"
#include "ns3/assert.h"

namespace ns3 {

InetAddress::InetAddress (Ipv4Address ipv4, uint16_t port)
  : m_ipv4 (ipv4),
    m_port (port)
{}
InetAddress::InetAddress (Ipv4Address ipv4)
  : m_ipv4 (ipv4),
    m_port (0)
{}
InetAddress::InetAddress (uint16_t port)
  : m_ipv4 (Ipv4Address::GetAny ()),
    m_port (port)
{}
uint16_t 
InetAddress::GetPort (void) const
{
  return m_port;
}
Ipv4Address 
InetAddress::GetIpv4 (void) const
{
  return m_ipv4;
}

void 
InetAddress::SetPort (uint16_t port)
{
  m_port = port;
}
void 
InetAddress::SetIpv4 (Ipv4Address address)
{
  m_ipv4 = address;
}

Address 
InetAddress::ConvertTo (void) const
{
  uint8_t buf[6];
  m_ipv4.Serialize (buf);
  buf[4] = m_port & 0xff;
  buf[5] = (m_port >> 8) & 0xff;
  return Address (GetType (), buf, 6);
}
InetAddress 
InetAddress::ConvertFrom (const Address &address)
{
  NS_ASSERT (address.CheckCompatible (GetType (), 6));
  uint8_t buf[6];
  address.CopyTo (buf);
  Ipv4Address ipv4 = Ipv4Address::Deserialize (buf);
  uint16_t port = buf[0] | (buf[1] << 8);
  return InetAddress (ipv4, port);
}
uint8_t 
InetAddress::GetType (void)
{
  static uint8_t type = Address::Register ();
  return type;
}

} // namespace ns3
