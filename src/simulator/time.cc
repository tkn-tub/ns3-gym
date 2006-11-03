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
  : TimeUnit<1> ()
{}
Time::Time (TimeUnit<1> time)
  : TimeUnit<1> (time)
{}

double 
Time::ApproximateToSeconds (void) const
{
  HighPrecision seconds = GetHighPrecision ();
  seconds.Div (HighPrecision (1000000000, 0));
  return seconds.GetDouble ();
}
int32_t 
Time::ApproximateToMilliSeconds (void) const
{
  HighPrecision ms = GetHighPrecision ();
  ms.Div (HighPrecision (1000000, 0));
  return (int32_t) ms.GetHigh ();
}
int64_t 
Time::ApproximateToMicroSeconds (void) const
{
  HighPrecision us = GetHighPrecision ();
  us.Div (HighPrecision (1000, 0));
  return us.GetHigh ();
}
int64_t 
Time::ApproximateToNanoSeconds (void) const
{
  return GetHighPrecision ().GetHigh ();
}


Seconds::Seconds ()
  : TimeUnit<1> ()
{}
Seconds::Seconds (double seconds)
  : TimeUnit<1> (HighPrecision (seconds * 1000000000.0))
{}
MilliSeconds::MilliSeconds ()
  : TimeUnit<1> ()
{}
MilliSeconds::MilliSeconds (uint32_t ms)
  : TimeUnit<1> (HighPrecision (ms * 1000000, 0))
{}
MicroSeconds::MicroSeconds ()
  : TimeUnit<1> ()
{}
MicroSeconds::MicroSeconds (uint32_t us)
  : TimeUnit<1> (HighPrecision (us * 1000, 0))
{}
NanoSeconds::NanoSeconds ()
  : TimeUnit<1> ()
{}
NanoSeconds::NanoSeconds (uint32_t ns)
  : TimeUnit<1> (HighPrecision (ns, 0))
{}

Now::Now ()
  : Time (Simulator::Now ())
{}

Scalar::Scalar ()
  : TimeUnit<0> ()
{}
Scalar::Scalar (TimeUnit<0> scalar)
  : TimeUnit<0> (scalar)
{}

Scalar::Scalar (double scalar)
  : TimeUnit<0> (HighPrecision (scalar))
{}

}; // namespace ns3


