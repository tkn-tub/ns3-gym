/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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
#include "ns3/ipv4-address.h"
#include "ns3/net-device.h"
#include "ns3/log.h"
#include "ns3/packet.h"

NS_LOG_COMPONENT_DEFINE ("Ipv4Interface");

namespace ns3 {

TypeId 
Ipv4Interface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ipv4Interface")
    .SetParent<Object> ()
    ;
  return tid;
}

  /**
   * By default, Ipv4 interface are created in the "down" state
   * with ip address 192.168.0.1 and a matching mask. Before
   * becoming useable, the user must invoke SetUp on them
   * once the final Ipv4 address and mask has been set.
   */
Ipv4Interface::Ipv4Interface () 
  : m_ifup(false),
    m_metric(1)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAMS (this);
}

Ipv4Interface::~Ipv4Interface ()
{
  NS_LOG_FUNCTION;
}

void
Ipv4Interface::DoDispose (void)
{
  NS_LOG_FUNCTION;
  Object::DoDispose ();
}

void 
Ipv4Interface::SetAddress (Ipv4Address a)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAMS (this << a);
  m_address = a;
}

void 
Ipv4Interface::SetNetworkMask (Ipv4Mask mask)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAMS (this << mask);
  m_netmask = mask;
}

Ipv4Address
Ipv4Interface::GetBroadcast (void) const
{
  NS_LOG_FUNCTION;
  uint32_t mask = m_netmask.GetHostOrder ();
  uint32_t address = m_address.GetHostOrder ();
  Ipv4Address broadcast = Ipv4Address (address | (~mask));
  return broadcast;
}

Ipv4Mask 
Ipv4Interface::GetNetworkMask (void) const
{
  NS_LOG_FUNCTION;
  return m_netmask;
}

void
Ipv4Interface::SetMetric (uint16_t metric)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAMS ("(" << metric << ")");
  m_metric = metric;
}

uint16_t
Ipv4Interface::GetMetric (void) const
{
  NS_LOG_FUNCTION;
  return m_metric;
}

Ipv4Address 
Ipv4Interface::GetAddress (void) const
{
  NS_LOG_FUNCTION;
  return m_address;
}

uint16_t 
Ipv4Interface::GetMtu (void) const
{
  NS_LOG_FUNCTION;
  if (GetDevice () == 0)
    {
      uint32_t mtu = (1<<16) - 1;
      return mtu;
    }
  return GetDevice ()->GetMtu ();
}

/**
 * These are IP interface states and may be distinct from 
 * NetDevice states, such as found in real implementations
 * (where the device may be down but IP interface state is still up).
 */
bool 
Ipv4Interface::IsUp (void) const
{
  NS_LOG_FUNCTION;
  return m_ifup;
}

bool 
Ipv4Interface::IsDown (void) const
{
  NS_LOG_FUNCTION;
  return !m_ifup;
}

void 
Ipv4Interface::SetUp (void)
{
  NS_LOG_FUNCTION;
  m_ifup = true;
}

void 
Ipv4Interface::SetDown (void)
{
  NS_LOG_FUNCTION;
  m_ifup = false;
}

// public wrapper on private virtual function
void 
Ipv4Interface::Send(Ptr<Packet> p, Ipv4Address dest)
{
  NS_LOG_FUNCTION;
  if (IsUp()) {
    NS_LOG_LOGIC ("SendTo");
    SendTo(p, dest);
  }
}

}; // namespace ns3

