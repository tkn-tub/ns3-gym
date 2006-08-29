/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
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

#include "event-impl.h"


namespace ns3 {


EventImpl::~EventImpl ()
{}

EventImpl::EventImpl ()
	: m_id (0),
	  m_count (1),
	  m_cancel (0),
	  m_running (1)
{}
void 
EventImpl::invoke (void)
{
	if (m_cancel == 0) {
		notify ();
	}
	m_running = 0;
}
void 
EventImpl::set_tag (void *tag)
{
	m_id = tag;
}
void *
EventImpl::get_tag (void) const
{
	return m_id;
}
void 
EventImpl::cancel (void)
{
	m_cancel = 1;
	m_running = 0;
}
bool 
EventImpl::is_running (void)
{
	return (m_running == 1);
}

}; // namespace ns3
