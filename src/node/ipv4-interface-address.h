/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Washington
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
 */

#ifndef IPV4_INTERFACE_ADDRESS_H
#define IPV4_INTERFACE_ADDRESS_H

#include <stdint.h>
#include <ostream>
#include "ipv4-address.h"

namespace ns3 {

/**
 * \brief a class to store IPv4 address information on an interface
 *
 * Corresponds to Linux struct in_ifaddr.  A list of these addresses
 * is stored in Ipv4Interface.  This class is modelled after how current
 * Linux handles IP aliasing for IPv4.  Notably, aliasing of IPv4 
 * interfaces (e.g., "eth0:1") is not used, and instead an interface
 * is assigned possibly multiple addresses, with each address being
 * classified as being primary and secondary.  See the iproute2 
 * documentation for this distinction.
 */
class Ipv4InterfaceAddress 
{
public:
  enum InterfaceAddressScope_e {
    HOST,
    LINK,
    GLOBAL
  };

  Ipv4InterfaceAddress ();
  // Configure m_local, m_mask, and m_broadcast from the below constructor
  Ipv4InterfaceAddress (Ipv4Address local, Ipv4Mask mask);
  Ipv4InterfaceAddress (const Ipv4InterfaceAddress &o);

  void SetLocal (Ipv4Address local);
  Ipv4Address GetLocal (void) const;
  void SetPeer (Ipv4Address peer);
  Ipv4Address GetPeer (void) const;
  void SetMask (Ipv4Mask mask);
  Ipv4Mask GetMask (void) const;
  void SetBroadcast (Ipv4Address broadcast);
  Ipv4Address GetBroadcast (void) const;
 
  void SetScope (Ipv4InterfaceAddress::InterfaceAddressScope_e scope);
  Ipv4InterfaceAddress::InterfaceAddressScope_e GetScope (void) const;
  
  bool IsSecondary (void) const;
  void SetSecondary (void);
  void SetPrimary (void);
  
private:
  Ipv4Address m_local;     // Interface address
  Ipv4Address m_peer;      // Peer destination address (in Linux:  m_address)
  Ipv4Mask m_mask;         // Network mask
  Ipv4Address m_broadcast; // Broadcast address

  InterfaceAddressScope_e m_scope;   
  bool m_secondary;        // For use in multihoming
};

std::ostream& operator<< (std::ostream& os, const Ipv4InterfaceAddress &addr);

} // namespace ns3

#endif /* IPV4_ADDRESS_H */
