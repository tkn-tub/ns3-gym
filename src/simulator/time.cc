/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
#include "time.h"
#include "simulator.h"

namespace ns3 {

Time::Time ()
	: m_us (0),
	  m_is_destroy (true)
{}
Time::Time (Time const &o)
	: m_us (o.m_us),
	  m_is_destroy (o.m_is_destroy)
{}
Time &
Time::operator = (Time const &o)
{
	m_us = o.m_us;
	m_is_destroy = o.m_is_destroy;
	return *this;
}
Time::Time (uint64_t us)
	: m_us (us),
	  m_is_destroy (false)
{}

double 
Time::s (void) const
{
	double us = m_us;
	us /= 1000000;
	return us;
}
uint64_t 
Time::us (void) const
{
	return m_us;
}

bool
Time::is_destroy (void) const
{
	return m_is_destroy;
}

Time
operator + (Time const &lhs, uint64_t delta_us)
{
	return AbsTimeUs (lhs.us () + delta_us);
}
Time
operator + (Time const &lhs, double delta_s)
{
	uint64_t delta_us = (uint64_t)(int64_t)(delta_s * 1000000.0);
	return AbsTimeUs (lhs.us () + delta_us);
}


AbsTimeS::AbsTimeS (double s)
	: Time ((uint64_t)(int64_t)(s * 1000000.0))
{}
AbsTimeUs::AbsTimeUs (uint64_t us)
	: Time (us)
{}
RelTimeS::RelTimeS (double s)
	: Time (Simulator::now () + s)
{}
RelTimeUs::RelTimeUs (uint64_t us)
	: Time (Simulator::now () + us)
{}

NowTime::NowTime ()
	: Time (Simulator::now ().us ())
{}

DestroyTime::DestroyTime ()
	: Time ()
{}

}; // namespace ns3
