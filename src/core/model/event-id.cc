/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include "event-id.h"
#include "simulator.h"
#include "event-impl.h"

namespace ns3 {

EventId::EventId ()
  : m_eventImpl (0),
    m_ts (0),
    m_context (0),
    m_uid (0)
{
}

EventId::EventId (const Ptr<EventImpl> &impl, uint64_t ts, uint32_t context, uint32_t uid)
  : m_eventImpl (impl),
    m_ts (ts),
    m_context (context),
    m_uid (uid)
{
}
void
EventId::Cancel (void)
{
  Simulator::Cancel (*this);
}
bool
EventId::IsExpired (void) const
{
  return Simulator::IsExpired (*this);
}
bool
EventId::IsRunning (void) const
{
  return !IsExpired ();
}
EventImpl *
EventId::PeekEventImpl (void) const
{
  return PeekPointer (m_eventImpl);
}
uint64_t 
EventId::GetTs (void) const
{
  return m_ts;
}
uint32_t 
EventId::GetContext (void) const
{
  return m_context;
}
uint32_t 
EventId::GetUid (void) const
{
  return m_uid;
}

bool operator == (const EventId &a, const EventId &b)
{
  return 
    a.m_uid == b.m_uid && 
    a.m_context == b.m_context && 
    a.m_ts == b.m_ts && 
    a.m_eventImpl == b.m_eventImpl;
}
bool operator != (const EventId &a, const EventId &b)
{
  return !(a == b);
}



}; // namespace ns3
