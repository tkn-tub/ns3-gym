/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#include "packet-socket-address.h"
#include "net-device.h"

namespace ns3 {

PacketSocketAddress::PacketSocketAddress ()
{}
void 
PacketSocketAddress::SetProtocol (uint16_t protocol)
{
  m_protocol = protocol;
}
void 
PacketSocketAddress::SetDevice (uint32_t index)
{
  m_device = index;
}
void 
PacketSocketAddress::SetPhysicalAddress (const Address address)
{
  m_address = address;
}

uint16_t 
PacketSocketAddress::GetProtocol (void) const
{
  return m_protocol;
}
uint32_t
PacketSocketAddress::GetDevice (void) const
{
  return m_device;
}
Address 
PacketSocketAddress::GetPhysicalAddress (void) const
{
  return m_address;
}

Address 
PacketSocketAddress::ConvertTo (void) const
{
  Address address;
  uint8_t buffer[Address::MAX_SIZE];
  buffer[0] = m_protocol & 0xff;
  buffer[1] = (m_protocol >> 8) & 0xff;
  buffer[2] = (m_device >> 24) & 0xff;
  buffer[3] = (m_device >> 16) & 0xff;
  buffer[4] = (m_device >> 8) & 0xff;
  buffer[5] = (m_device >> 0) & 0xff;
  m_address.CopyAllTo (buffer + 6, Address::MAX_SIZE - 6);
  return Address (GetType (), buffer, GetSize ());
}
PacketSocketAddress 
PacketSocketAddress::ConvertFrom (const Address &address)
{
  NS_ASSERT (IsMatchingType (address));
  uint8_t buffer[Address::MAX_SIZE];
  address.CopyTo (buffer);
  uint16_t protocol = buffer[0] | (buffer[1] << 8);
  uint32_t device = 0;
  device |= buffer[2];
  device <<= 8;
  device |= buffer[3];
  device <<= 8;
  device |= buffer[4];
  device <<= 8;
  device |= buffer[5];
  Address physical;
  physical.CopyAllFrom (buffer + 6, Address::MAX_SIZE - 6);
  PacketSocketAddress ad;
  ad.SetProtocol (protocol);
  ad.SetDevice (device);
  ad.SetPhysicalAddress (physical);
  return ad;
}
bool 
PacketSocketAddress::IsMatchingType (const Address &address)
{
  return address.CheckCompatible (GetType (), GetSize ());
}
uint8_t 
PacketSocketAddress::GetType (void)
{
  static uint8_t type = Address::Register ();
  return type;
}
uint8_t
PacketSocketAddress::GetSize (void)
{
  return 2 + sizeof (NetDevice *) + 1 + 1 + 8;
}

} // namespace ns3
