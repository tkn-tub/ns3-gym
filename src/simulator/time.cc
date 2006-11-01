/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
int64_t 
Time::ApproximateToMilliSeconds (void) const
{
  int64_t ms = m_ns;
  ms /= 1000000;
  return ms;
}
int64_t 
Time::ApproximateToMicroSeconds (void) const
{
  int64_t us = m_ns;
  us /= 1000;
  return us;
}
int64_t 
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

Time Scale (Time const &time, double scale)
{
  return NanoSeconds ((int64_t) (GetNs (time) * scale));
}
Time Abs (Time const &time)
{
  int64_t retval = GetNs (time);
  retval = (retval<0)?(-retval):(retval);
  return NanoSeconds (retval);
}
Time Max (Time const &ta, Time const &tb)
{
  int64_t a = GetNs (ta);
  int64_t b = GetNs (tb);
  int64_t retval = (a>b)?(a):(b);
  return NanoSeconds (retval);
}
Time Min (Time const &ta, Time const &tb)
{
  int64_t a = GetNs (ta);
  int64_t b = GetNs (tb);
  int64_t retval = (a<b)?(a):(b);
  return NanoSeconds (retval);
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
