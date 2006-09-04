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
	: m_ns (0),
	  m_is_destroy (true)
{}
Time::Time (Time const &o)
	: m_ns (o.m_ns),
	  m_is_destroy (o.m_is_destroy)
{}
Time &
Time::operator = (Time const &o)
{
	m_ns = o.m_ns;
	m_is_destroy = o.m_is_destroy;
	return *this;
}
Time::Time (uint64_t ns)
	: m_ns (ns),
	  m_is_destroy (false)
{}

double 
Time::s (void) const
{
	double ns = m_ns;
	ns /= 1000000000;
	return ns;
}
uint64_t 
Time::us (void) const
{
	uint64_t us = m_ns / 1000;
	return us;
}

uint64_t 
Time::ns (void) const
{
	return m_ns;
}

bool
Time::is_destroy (void) const
{
	return m_is_destroy;
}

Time 
Time::abs_s (double s)
{
	int64_t ns = (int64_t)(s * 1000000000.0);
	return Time (ns);
}
Time 
Time::abs_us (uint64_t us)
{
	int64_t ns = us * 1000;
	return Time (ns);
}
Time 
Time::abs_ns (uint64_t ns)
{
	return Time (ns);
}
Time 
Time::rel_s (double s)
{
	int64_t ns = (int64_t)(s * 1000000000.0);
	return Time (Simulator::now ().ns () + ns);
}
Time 
Time::rel_us (uint64_t us)
{
	return Time (Simulator::now ().ns () + us * 1000);
}
Time 
Time::rel_ns (uint64_t ns)
{
	return Time (Simulator::now ().ns () + ns);
}

Time 
Time::now (void)
{
	return Time (Simulator::now ().ns ());
}
Time 
Time::destroy (void)
{
	return Time ();
}

class AbsTimeS : public Time {
public:
	AbsTimeS (double s);
};
class AbsTimeUs : public Time {
public:
	AbsTimeUs (uint64_t us);
};
class RelTimeS : public Time {
public:
	RelTimeS (double s);
};
class RelTimeUs : public Time {
public:
	RelTimeUs (uint64_t us);
};

class NowTime : public Time {
public:
	NowTime ();
};

class DestroyTime : public Time {
public:
	DestroyTime ();
};


AbsTimeS::AbsTimeS (double s)
	: Time ((uint64_t)(int64_t)(s * 1000000000.0))
{}
AbsTimeUs::AbsTimeUs (uint64_t us)
	: Time (us*1000)
{}
RelTimeS::RelTimeS (double s)
	: Time (Simulator::now ().ns () + (int64_t)(s * 1000000000.0))
{}
RelTimeUs::RelTimeUs (uint64_t us)
	: Time (Simulator::now ().ns () + us*1000)
{}

NowTime::NowTime ()
	: Time (Simulator::now ().ns ())
{}

DestroyTime::DestroyTime ()
	: Time ()
{}

}; // namespace ns3
