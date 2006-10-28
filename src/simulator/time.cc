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
    : m_ns (0)
{}
Time::Time (Time const &o)
    : m_ns (o.m_ns)
{}
Time &
Time::operator = (Time const &o)
{
  m_ns = o.m_ns;
  return *this;
}
Time::Time (int64_t ns)
    : m_ns (ns)
{}


bool 
Time::IsNegative (void) const
{
  return m_ns <= 0;
}
  
bool 
Time::IsPositive (void) const
{
  return m_ns >= 0;
}
  
bool 
Time::IsStrictlyNegative (void) const
{
  return m_ns < 0;
}
bool 
Time::IsStrictlyPositive (void) const
{
  return m_ns > 0;
}
bool 
Time::IsZero (void) const
{
  return m_ns == 0;
}

Time 
Time::operator += (Time const &o)
{
  m_ns += o.m_ns;
  return *this;
}
  
Time 
Time::operator -= (Time const &o)
{
  m_ns -= o.m_ns;
  return *this;
}

double 
Time::ApproximateToSeconds (void) const
{
  double s = m_ns;
  s /= 1000000000;
  return s;
}
uint64_t 
Time::ApproximateToMilliSeconds (void) const
{
  uint64_t ms = m_ns;
  ms /= 1000000;
  return ms;
}
uint64_t 
Time::ApproximateToMicroSeconds (void) const
{
  uint64_t us = m_ns;
  us /= 1000;
  return us;
}
uint64_t 
Time::ApproximateToNanoSeconds (void) const
{
  return m_ns;
}


/* To decrease the number of keystrokes
 */
static uint64_t 
GetNs (Time const &time)
{
  return time.ApproximateToNanoSeconds ();
}

Time operator + (Time const &lhs, Time const &rhs)
{
  return NanoSeconds (GetNs (lhs) + GetNs (rhs));
}  
Time operator - (Time const &lhs, Time const &rhs)
{
  return NanoSeconds (GetNs (lhs) - GetNs (rhs));
}
bool operator == (Time const &lhs, Time const &rhs)
{
  return GetNs (lhs) == GetNs (rhs);
}

bool operator != (Time const &lhs, Time const &rhs)
{
  return GetNs (lhs) != GetNs (rhs);
}
bool operator <  (Time const &lhs, Time const &rhs)
{
  return GetNs (lhs) < GetNs (rhs);
}
bool operator <= (Time const &lhs, Time const &rhs)
{
  return GetNs (lhs) <= GetNs (rhs);
}
bool operator >  (Time const &lhs, Time const &rhs)
{
  return GetNs (lhs) > GetNs (rhs);
}
bool operator >= (Time const &lhs, Time const &rhs)
{
  return GetNs (lhs) >= GetNs (rhs);
}

Now::Now ()
    : Time (Simulator::Now ())
{}
Seconds::Seconds (double s)
    : Time ((int64_t)(s * 1000000000))
{}
MilliSeconds::MilliSeconds (int32_t ms)
    : Time ((int64_t)(ms * 1000000))
{}
MicroSeconds::MicroSeconds (int32_t us)
    : Time ((int64_t)(us * 1000))
{}
NanoSeconds::NanoSeconds (int64_t ns)
    : Time (ns)
{}
  



}; // namespace ns3
