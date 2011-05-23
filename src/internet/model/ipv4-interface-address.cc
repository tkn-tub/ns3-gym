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

#include "ns3/log.h"
#include "ns3/assert.h"
#include "ipv4-interface-address.h"

NS_LOG_COMPONENT_DEFINE ("Ipv4InterfaceAddress");

namespace ns3 {

Ipv4InterfaceAddress::Ipv4InterfaceAddress ()
  : m_scope (GLOBAL), 
    m_secondary (false)
{
  NS_LOG_FUNCTION (this);
}

Ipv4InterfaceAddress::Ipv4InterfaceAddress (Ipv4Address local, Ipv4Mask mask)
  : m_scope (GLOBAL), 
    m_secondary (false)
{
  NS_LOG_FUNCTION (this);
  m_local = local;
  m_mask = mask;
  m_broadcast = Ipv4Address (local.Get () | (~mask.Get ()));
}

Ipv4InterfaceAddress::Ipv4InterfaceAddress (const Ipv4InterfaceAddress &o)
  : m_local (o.m_local),
    m_mask (o.m_mask),
    m_broadcast (o.m_broadcast),
    m_scope (o.m_scope),
    m_secondary (o.m_secondary)
{
  NS_LOG_FUNCTION (this);
}

void 
Ipv4InterfaceAddress::SetLocal (Ipv4Address local)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_local = local;
}

Ipv4Address 
Ipv4InterfaceAddress::GetLocal (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_local; 
}

void 
Ipv4InterfaceAddress::SetMask (Ipv4Mask mask) 
{
  NS_LOG_FUNCTION_NOARGS ();
  m_mask = mask;
}

Ipv4Mask 
Ipv4InterfaceAddress::GetMask (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_mask;
}

void 
Ipv4InterfaceAddress::SetBroadcast (Ipv4Address broadcast)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_broadcast = broadcast;
}

Ipv4Address 
Ipv4InterfaceAddress::GetBroadcast (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_broadcast;
}

void 
Ipv4InterfaceAddress::SetScope (Ipv4InterfaceAddress::InterfaceAddressScope_e scope)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_scope = scope;
}

Ipv4InterfaceAddress::InterfaceAddressScope_e 
Ipv4InterfaceAddress::GetScope (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_scope;
}

bool 
Ipv4InterfaceAddress::IsSecondary (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_secondary;
}

void 
Ipv4InterfaceAddress::SetSecondary (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_secondary = true;
}

void 
Ipv4InterfaceAddress::SetPrimary (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_secondary = false;
}

std::ostream& operator<< (std::ostream& os, const Ipv4InterfaceAddress &addr)
{ 
  os << "m_local=" << addr.GetLocal () << "; m_mask=" <<
  addr.GetMask () << "; m_broadcast=" << addr.GetBroadcast () << "; m_scope=" << addr.GetScope () <<
  "; m_secondary=" << addr.IsSecondary ();
  return os;
} 

} // namespace ns3
