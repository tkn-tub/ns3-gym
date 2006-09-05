/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
/*
 * Copyright (c) 2006 INRIA
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

#include "scheduler.h"
#include <cassert>

namespace ns3 {

Scheduler::~Scheduler () 
{}

/* Note the invariants which this function must provide:
 * - irreflexibility: f (x,x) is false)
 * - antisymmetry: f(x,y) = !f(y,x)
 * - transitivity: f(x,y) and f(y,z) => f(x,z)
 */
bool
Scheduler::EventKeyCompare::operator () (struct EventKey a, struct EventKey b)
{
	assert (a.m_uid != b.m_uid);
	if (a.m_ns < b.m_ns) {
		return true;
	} else if (a.m_ns == b.m_ns && a.m_uid < b.m_uid) {
		return true;
	} else {
		return false;
	}
}


EventId 
Scheduler::insert (EventImpl *event, struct EventKey key)
{
	return realInsert (event, key);
}
bool 
Scheduler::isEmpty (void) const
{
	return realIsEmpty ();
}
EventImpl *
Scheduler::peekNext (void) const
{
	assert (!realIsEmpty ());
	return realPeekNext ();
}
Scheduler::EventKey 
Scheduler::peekNextKey (void) const 
{
	assert (!realIsEmpty ());
	return realPeekNextKey ();
}
void 
Scheduler::removeNext (void)
{
	assert (!realIsEmpty ());
	return realRemoveNext ();
}
EventImpl *
Scheduler::remove (EventId id, EventKey *key)
{
	assert (!realIsEmpty ());
	return realRemove (id, key);
}
bool 
Scheduler::isValid (EventId id)
{
	return realIsValid (id);
}

}; // namespace ns3
