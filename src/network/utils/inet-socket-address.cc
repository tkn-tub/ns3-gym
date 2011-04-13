/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "inet-socket-address.h"
#include "ns3/assert.h"

namespace ns3 {

InetSocketAddress::InetSocketAddress (Ipv4Address ipv4, uint16_t port)
  : m_ipv4 (ipv4),
    m_port (port)
{}
InetSocketAddress::InetSocketAddress (Ipv4Address ipv4)
  : m_ipv4 (ipv4),
    m_port (0)
{}
InetSocketAddress::InetSocketAddress (const char *ipv4, uint16_t port)
  : m_ipv4 (Ipv4Address (ipv4)),
    m_port (port)
{}
InetSocketAddress::InetSocketAddress (const char * ipv4)
  : m_ipv4 (Ipv4Address (ipv4)),
    m_port (0)
{}
InetSocketAddress::InetSocketAddress (uint16_t port)
  : m_ipv4 (Ipv4Address::GetAny ()),
    m_port (port)
{}
uint16_t 
InetSocketAddress::GetPort (void) const
{
  return m_port;
}
Ipv4Address 
InetSocketAddress::GetIpv4 (void) const
{
  return m_ipv4;
}

void 
InetSocketAddress::SetPort (uint16_t port)
{
  m_port = port;
}
void 
InetSocketAddress::SetIpv4 (Ipv4Address address)
{
  m_ipv4 = address;
}

bool 
InetSocketAddress::IsMatchingType (const Address &address)
{
  return address.CheckCompatible (GetType (), 6);
}

InetSocketAddress::operator Address () const
{
  return ConvertTo ();
}

Address 
InetSocketAddress::ConvertTo (void) const
{
  uint8_t buf[6];
  m_ipv4.Serialize (buf);
  buf[4] = m_port & 0xff;
  buf[5] = (m_port >> 8) & 0xff;
  return Address (GetType (), buf, 6);
}
InetSocketAddress 
InetSocketAddress::ConvertFrom (const Address &address)
{
  NS_ASSERT (address.CheckCompatible (GetType (), 6));
  uint8_t buf[6];
  address.CopyTo (buf);
  Ipv4Address ipv4 = Ipv4Address::Deserialize (buf);
  uint16_t port = buf[4] | (buf[5] << 8);
  return InetSocketAddress (ipv4, port);
}
uint8_t 
InetSocketAddress::GetType (void)
{
  static uint8_t type = Address::Register ();
  return type;
}

} // namespace ns3
