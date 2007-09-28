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
#ifndef PACKET_SOCKET_ADDRESS_H
#define PACKET_SOCKET_ADDRESS_H

#include "ns3/ptr.h"
#include "address.h"
#include "mac48-address.h"
#include "mac64-address.h"
#include "net-device.h"

namespace ns3 {

class NetDevice;

class PacketSocketAddress
{
 public:
  PacketSocketAddress ();
  void SetProtocol (uint16_t protocol);

  void SetAllDevices (void);
  void SetSingleDevice (uint32_t device);
  void SetPhysicalAddress (const Address address);

  uint16_t GetProtocol (void) const;
  uint32_t GetSingleDevice (void) const;
  bool IsSingleDevice (void) const;
  Address GetPhysicalAddress (void) const;

  /**
   * \returns a new Address instance
   *
   * Convert an instance of this class to a polymorphic Address instance.
   */
  operator Address () const;
  /**
   * \param address a polymorphic address
   *
   * Convert a polymorphic address to an Mac48Address instance.
   * The conversion performs a type check.
   */
  static PacketSocketAddress ConvertFrom (const Address &address);
  /**
   * \returns true if the address matches, false otherwise.
   */
  static bool IsMatchingType (const Address &address);
 private:
  static uint8_t GetType (void);
  Address ConvertTo (void) const;
  uint16_t m_protocol;
  bool m_isSingleDevice;
  uint32_t m_device;
  Address m_address;
};


} // namespace ns3

#endif /* PACKET_SOCKET_ADDRESS_H */
