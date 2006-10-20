/* -*- Mode:NS3; -*- */
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
Time::S (void) const
{
    double ns = m_ns;
    ns /= 1000000000;
    return ns;
}
uint64_t 
Time::Us (void) const
{
    uint64_t us = m_ns / 1000;
    return us;
}

uint64_t 
Time::Ns (void) const
{
    return m_ns;
}

bool
Time::IsDestroy (void) const
{
    return m_isDestroy;
}

Time 
Time::AbsS (double s)
{
    int64_t ns = (int64_t)(s * 1000000000.0);
    return Time (ns);
}
Time 
Time::AbsUs (uint64_t us)
{
    int64_t ns = us * 1000;
    return Time (ns);
}
Time 
Time::AbsNs (uint64_t ns)
{
    return Time (ns);
}
Time 
Time::RelS (double s)
{
    int64_t ns = (int64_t)(s * 1000000000.0);
    return Time (Simulator::Now ().Ns () + ns);
}
Time 
Time::RelUs (uint64_t us)
{
    return Time (Simulator::Now ().Ns () + us * 1000);
}
Time 
Time::RelNs (uint64_t ns)
{
    return Time (Simulator::Now ().Ns () + ns);
}

Time 
Time::Now (void)
{
    return Time (Simulator::Now ().Ns ());
}
Time 
Time::Destroy (void)
{
    return Time ();
}

}; // namespace ns3
