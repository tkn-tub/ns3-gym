/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
#include "ns3/assert.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"

#include "arp-cache.h"
#include "arp-header.h"
#include "ipv4-interface.h"

namespace ns3 {

ArpCache::ArpCache (Ptr<NetDevice> device, Ptr<Ipv4Interface> interface)
  : m_device (device), 
    m_interface (interface),
    m_aliveTimeout (Seconds (120)),
    m_deadTimeout (Seconds (100)),
    m_waitReplyTimeout (Seconds (1))
{}

ArpCache::~ArpCache ()
{
  Flush ();
}

Ptr<NetDevice>
ArpCache::GetDevice (void) const
{
  return m_device;
}

Ptr<Ipv4Interface>
ArpCache::GetInterface (void) const
{
  return m_interface;
}

void 
ArpCache::SetAliveTimeout (Time aliveTimeout)
{
  m_aliveTimeout = aliveTimeout;
}
void 
ArpCache::SetDeadTimeout (Time deadTimeout)
{
  m_deadTimeout = deadTimeout;
}
void 
ArpCache::SetWaitReplyTimeout (Time waitReplyTimeout)
{
  m_waitReplyTimeout = waitReplyTimeout;
}

Time
ArpCache::GetAliveTimeout (void) const
{
  return m_aliveTimeout;
}
Time
ArpCache::GetDeadTimeout (void) const
{
  return m_deadTimeout;
}
Time
ArpCache::GetWaitReplyTimeout (void) const
{
  return m_waitReplyTimeout;
}

void 
ArpCache::Flush (void)
{
  for (CacheI i = m_arpCache.begin (); i != m_arpCache.end (); i++) 
    {
      delete (*i).second;
    }
  m_arpCache.erase (m_arpCache.begin (), m_arpCache.end ());
}

ArpCache::Entry *
ArpCache::Lookup (Ipv4Address to)
{
  if (m_arpCache.find (to) != m_arpCache.end ()) 
    {
      ArpCache::Entry *entry = m_arpCache[to];
      return entry;
    }
  return 0;
}

ArpCache::Entry *
ArpCache::Add (Ipv4Address to)
{
  NS_ASSERT (m_arpCache.find (to) == m_arpCache.end ());

  ArpCache::Entry *entry = new ArpCache::Entry (this);
  m_arpCache[to] = entry;  
  return entry;
}

ArpCache::Entry::Entry (ArpCache *arp)
  : m_arp (arp),
    m_state (ALIVE),
    m_waiting ()
{}


bool 
ArpCache::Entry::IsDead (void)
{
  return (m_state == DEAD)?true:false;
}
bool 
ArpCache::Entry::IsAlive (void)
{
  return (m_state == ALIVE)?true:false;
}
bool
ArpCache::Entry::IsWaitReply (void)
{
  return (m_state == WAIT_REPLY)?true:false;
}


void 
ArpCache::Entry::MarkDead (void) 
{
  m_state = DEAD;
  //NS_ASSERT (m_waiting != 0);
  UpdateSeen ();
}
Ptr<Packet>
ArpCache::Entry::MarkAlive (Address macAddress) 
{
  NS_ASSERT (m_state == WAIT_REPLY);
  //NS_ASSERT (m_waiting != 0);
  m_macAddress = macAddress;
  m_state = ALIVE;
  UpdateSeen ();
  Ptr<Packet> waiting = m_waiting;
  //m_waiting = 0;
  return waiting;
}

Ptr<Packet>
ArpCache::Entry::UpdateWaitReply (Ptr<Packet> waiting)
{
  NS_ASSERT (m_state == WAIT_REPLY);
  /* We are already waiting for an answer so
   * we dump the previously waiting packet and
   * replace it with this one.
   */
  Ptr<Packet> old = m_waiting;
  m_waiting = waiting;
  return old;
}
void 
ArpCache::Entry::MarkWaitReply (Ptr<Packet> waiting)
{
  NS_ASSERT (m_state == ALIVE || m_state == DEAD);
  //NS_ASSERT (m_waiting == 0);
  m_state = WAIT_REPLY;
  m_waiting = waiting;
  UpdateSeen ();
}

Address
ArpCache::Entry::GetMacAddress (void)
{
  NS_ASSERT (m_state == ALIVE);
  return m_macAddress;
}
bool 
ArpCache::Entry::IsExpired (void)
{
  Time timeout;
  switch (m_state) {
  case ArpCache::Entry::WAIT_REPLY:
    timeout = m_arp->GetWaitReplyTimeout ();
    break;
  case ArpCache::Entry::DEAD:
    timeout = m_arp->GetDeadTimeout ();
    break;
  case ArpCache::Entry::ALIVE:
    timeout = m_arp->GetAliveTimeout ();
    break;
  default:
    NS_ASSERT (false);
    timeout = Seconds (0);
    /* NOTREACHED */
    break;
  }
  Time delta = Simulator::Now () - m_lastSeen;
  if (delta >= timeout) 
    {
      return true;
    } 
  else 
    {
      return false;
    }
}
void 
ArpCache::Entry::UpdateSeen (void)
{
  m_lastSeen = Simulator::Now ();
}

} // namespace ns3

