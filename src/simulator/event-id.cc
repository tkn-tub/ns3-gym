/* -*- Mode:NS3; -*- */
/*
 * Copyright (c) 2005 INRIA
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
#include "event-id.h"
#include "simulator.h"

namespace ns3 {

EventId::EventId ()
    : m_eventImpl (0),
      m_ns (0),
      m_uid (0)
{}
    
EventId::EventId (EventImpl *impl, uint64_t ns, uint32_t uid)
    : m_eventImpl (impl),
      m_ns (ns),
      m_uid (uid)
{}
void 
EventId::Cancel (void)
{
    Simulator::Cancel (*this);
}
bool 
EventId::IsExpired (void)
{
    return Simulator::IsExpired (*this);
}
EventImpl *
EventId::GetEventImpl (void) const
{
    return m_eventImpl;
}
uint64_t 
EventId::GetNs (void) const
{
    return m_ns;
}
uint32_t 
EventId::GetUid (void) const
{
    return m_uid;
}


}; // namespace ns3
