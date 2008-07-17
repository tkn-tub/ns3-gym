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
#include "ns3/uinteger.h"
#include "ns3/log.h"

#include "arp-cache.h"
#include "arp-header.h"
#include "ipv4-interface.h"

NS_LOG_COMPONENT_DEFINE ("ArpCache");

namespace ns3 {

TypeId 
ArpCache::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ArpCache")
    .SetParent<Object> ()
    .AddAttribute ("AliveTimeout",
                   "When this timeout expires, the matching cache entry needs refreshing",
                   TimeValue (Seconds (120)),
                   MakeTimeAccessor (&ArpCache::m_aliveTimeout),
                   MakeTimeChecker ())
    .AddAttribute ("DeadTimeout",
                   "When this timeout expires, a new attempt to resolve the matching entry is made",
                   TimeValue (Seconds (100)),
                   MakeTimeAccessor (&ArpCache::m_deadTimeout),
                   MakeTimeChecker ())
    .AddAttribute ("WaitReplyTimeout",
                   "When this timeout expires, the matching cache entry is marked dead",
                   TimeValue (Seconds (1)),
                   MakeTimeAccessor (&ArpCache::m_waitReplyTimeout),
                   MakeTimeChecker ())
    .AddAttribute ("PendingQueueSize",
                   "The size of the queue for packets pending an arp reply.",
                   UintegerValue (3),
                   MakeUintegerAccessor (&ArpCache::m_pendingQueueSize),
                   MakeUintegerChecker<uint32_t> ())
    ;
  return tid;
}

ArpCache::ArpCache ()
  : m_device (0), 
    m_interface (0)
{
  NS_LOG_FUNCTION (this);
}

ArpCache::~ArpCache ()
{
  NS_LOG_FUNCTION (this);
}

void 
ArpCache::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Flush ();
  m_device = 0;
  m_interface = 0;
  Object::DoDispose ();
}

void
ArpCache::SetDevice (Ptr<NetDevice> device, Ptr<Ipv4Interface> interface)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_device = device;
  m_interface = interface;
}

Ptr<NetDevice>
ArpCache::GetDevice (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_device;
}

Ptr<Ipv4Interface>
ArpCache::GetInterface (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_interface;
}

void 
ArpCache::SetAliveTimeout (Time aliveTimeout)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_aliveTimeout = aliveTimeout;
}
void 
ArpCache::SetDeadTimeout (Time deadTimeout)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_deadTimeout = deadTimeout;
}
void 
ArpCache::SetWaitReplyTimeout (Time waitReplyTimeout)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_waitReplyTimeout = waitReplyTimeout;
}

Time
ArpCache::GetAliveTimeout (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_aliveTimeout;
}
Time
ArpCache::GetDeadTimeout (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_deadTimeout;
}
Time
ArpCache::GetWaitReplyTimeout (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_waitReplyTimeout;
}

void 
ArpCache::Flush (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  for (CacheI i = m_arpCache.begin (); i != m_arpCache.end (); i++) 
    {
      delete (*i).second;
    }
  m_arpCache.erase (m_arpCache.begin (), m_arpCache.end ());
}

ArpCache::Entry *
ArpCache::Lookup (Ipv4Address to)
{
  NS_LOG_FUNCTION_NOARGS ();
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
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT (m_arpCache.find (to) == m_arpCache.end ());

  ArpCache::Entry *entry = new ArpCache::Entry (this);
  m_arpCache[to] = entry;  
  return entry;
}

ArpCache::Entry::Entry (ArpCache *arp)
  : m_arp (arp),
    m_state (ALIVE)
{
  NS_LOG_FUNCTION_NOARGS ();
}


bool 
ArpCache::Entry::IsDead (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return (m_state == DEAD)?true:false;
}
bool 
ArpCache::Entry::IsAlive (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return (m_state == ALIVE)?true:false;
}
bool
ArpCache::Entry::IsWaitReply (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return (m_state == WAIT_REPLY)?true:false;
}


void 
ArpCache::Entry::MarkDead (void) 
{
  NS_LOG_FUNCTION_NOARGS ();
  m_state = DEAD;
  UpdateSeen ();
}
void
ArpCache::Entry::MarkAlive (Address macAddress) 
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT (m_state == WAIT_REPLY);
  m_macAddress = macAddress;
  m_state = ALIVE;
  UpdateSeen ();
}

bool
ArpCache::Entry::UpdateWaitReply (Ptr<Packet> waiting)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT (m_state == WAIT_REPLY);
  /* We are already waiting for an answer so
   * we dump the previously waiting packet and
   * replace it with this one.
   */
  if (m_pending.size () >= m_arp->m_pendingQueueSize)
    {
      return false;
    }
  m_pending.push_back (waiting);
  return true;
}
void 
ArpCache::Entry::MarkWaitReply (Ptr<Packet> waiting)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT (m_state == ALIVE || m_state == DEAD);
  NS_ASSERT (m_pending.empty ());
  m_state = WAIT_REPLY;
  m_pending.push_back (waiting);
  UpdateSeen ();
}

Address
ArpCache::Entry::GetMacAddress (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT (m_state == ALIVE);
  return m_macAddress;
}
bool 
ArpCache::Entry::IsExpired (void)
{
  NS_LOG_FUNCTION_NOARGS ();
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
Ptr<Packet> 
ArpCache::Entry::DequeuePending (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_pending.empty ())
    {
      return 0;
    }
  else
    {
      Ptr<Packet> p = m_pending.front ();
      m_pending.pop_front ();
      return p;
    }
}
void 
ArpCache::Entry::UpdateSeen (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_lastSeen = Simulator::Now ();
}

} // namespace ns3

