/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
 * All rights reserved.
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

#include "ipv4-interface.h"
#include "ipv4-address.h"
#include "net-device.h"

namespace ns3 {

  /**
   * By default, Ipv4 interface are created in the "down" state
   * with ip address 192.168.0.1 and a matching mask. Before
   * becoming useable, the user must invoke SetUp on them
   * once the final Ipv4 address and mask has been set.
   */
Ipv4Interface::Ipv4Interface (NetDevice *nd) 
  : m_netdevice (nd), 
    m_ifup(false)
{}

Ipv4Interface::~Ipv4Interface ()
{}

NetDevice*
Ipv4Interface::GetDevice (void) const
{
  return m_netdevice;
}

void 
Ipv4Interface::SetAddress (Ipv4Address a)
{
  m_address = a;
}
void 
Ipv4Interface::SetNetworkMask (Ipv4Mask mask)
{
  m_netmask = mask;
}

Ipv4Address
Ipv4Interface::GetBroadcast (void) const
{
  uint32_t mask = m_netmask.GetHostOrder ();
  uint32_t address = m_address.GetHostOrder ();
  Ipv4Address broadcast = Ipv4Address (address | (~mask));
  return broadcast;
}
Ipv4Mask 
Ipv4Interface::GetNetworkMask (void) const
{
  return m_netmask;
}
Ipv4Address 
Ipv4Interface::GetAddress (void) const
{
  return m_address;
}

uint16_t 
Ipv4Interface::GetMtu (void) const
{
  if (m_netdevice == 0)
    {
      uint32_t mtu = (1<<16) - 1;
      return mtu;
    }
  return m_netdevice->GetMtu ();
}

  /**
   * These are IP interface states and may be distinct from 
   * NetDevice states, such as found in real implementations
   * (where the device may be down but IP interface state is still up).
   */
bool 
Ipv4Interface::IsUp (void) const
{
  return m_ifup;
}

bool 
Ipv4Interface::IsDown (void) const
{
  return !m_ifup;
}

void 
Ipv4Interface::SetUp (void)
{
  m_ifup = true;
}

void 
Ipv4Interface::SetDown (void)
{
  m_ifup = false;
}

// public wrapper on private virtual function
void 
Ipv4Interface::Send(Packet p, Ipv4Address dest)
{
  if (IsUp()) {
    SendTo(p, dest);
  }
}

}; // namespace ns3

