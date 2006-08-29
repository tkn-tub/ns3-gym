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
#include "timeout.h"
#include "yans/simulator.h"
#include "yans/event.tcc"

namespace yans {

Timeout::Timeout (ExpireCallback callback)
	: m_interval_us (0),
	  m_count (0),
	  m_current_count (0),
	  m_stop (false),
	  m_callback (callback)
{}
Timeout::~Timeout ()
{}
void 
Timeout::set_interval (uint64_t us)
{
	m_interval_us = us;
}
void 
Timeout::set_count (uint32_t count)
{
	m_count = count;
}


void Timeout::start (void)
{
	Simulator::schedule_rel_us (m_interval_us,
				 make_event (&Timeout::expire, this));
}
void Timeout::stop (void)
{
	m_stop = true;
}
void Timeout::restart (void)
{
	m_current_count = m_count;
}
void
Timeout::expire (void)
{
	if (m_stop) {
		return;
	}
	m_current_count--;
	if (m_current_count == 0) {
		m_callback ();
		return;
	}
	Simulator::schedule_rel_us (m_interval_us,
				 make_event (&Timeout::expire, this));
}


}; // namespace yans
