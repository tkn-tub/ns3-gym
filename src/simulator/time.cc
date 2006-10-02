/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:nil -*- */
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
      m_isDestroy (true)
{}
Time::Time (Time const &o)
    : m_ns (o.m_ns),
      m_isDestroy (o.m_isDestroy)
{}
Time &
Time::operator = (Time const &o)
{
    m_ns = o.m_ns;
    m_isDestroy = o.m_isDestroy;
    return *this;
}
Time::Time (uint64_t ns)
    : m_ns (ns),
      m_isDestroy (false)
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
Time::isDestroy (void) const
{
    return m_isDestroy;
}

Time 
Time::absS (double s)
{
    int64_t ns = (int64_t)(s * 1000000000.0);
    return Time (ns);
}
Time 
Time::absUs (uint64_t us)
{
    int64_t ns = us * 1000;
    return Time (ns);
}
Time 
Time::absNs (uint64_t ns)
{
    return Time (ns);
}
Time 
Time::relS (double s)
{
    int64_t ns = (int64_t)(s * 1000000000.0);
    return Time (Simulator::now ().ns () + ns);
}
Time 
Time::relUs (uint64_t us)
{
    return Time (Simulator::now ().ns () + us * 1000);
}
Time 
Time::relNs (uint64_t ns)
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

}; // namespace ns3
