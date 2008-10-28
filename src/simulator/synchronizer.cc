/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Washington
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
 */

#include "synchronizer.h"

namespace ns3 {

TypeId 
Synchronizer::GetTypeId (void)
{
  static TypeId tid = TypeId ("Synchronizer")
    .SetParent<Object> ()
    ;
  return tid;
}

Synchronizer::Synchronizer ()
  : m_realtimeOriginNano (0), m_simOriginNano (0)
{
}

Synchronizer::~Synchronizer ()
{
}

  bool
Synchronizer::Realtime (void)
{
  return DoRealtime ();
}

  uint64_t
Synchronizer::GetCurrentRealtime (void)
{
  return NanosecondToTimeStep (DoGetCurrentRealtime ());
}

  void
Synchronizer::SetOrigin (uint64_t ts)
{
  m_simOriginNano = TimeStepToNanosecond (ts);
  DoSetOrigin (m_simOriginNano);
}

  uint64_t
Synchronizer::GetOrigin (void)
{
  return NanosecondToTimeStep (m_simOriginNano);
}

  int64_t
Synchronizer::GetDrift (uint64_t ts)
{
  int64_t tDrift = DoGetDrift (TimeStepToNanosecond (ts));

  if (tDrift < 0) 
    {
      return -NanosecondToTimeStep (-tDrift);
    } else {
      return NanosecondToTimeStep (tDrift);
  }
}

  bool
Synchronizer::Synchronize (uint64_t tsCurrent, uint64_t tsDelay)
{
  return DoSynchronize (TimeStepToNanosecond (tsCurrent), 
    TimeStepToNanosecond (tsDelay));
}

  void
Synchronizer::Signal (void)
{
  DoSignal ();
}

  void
Synchronizer::SetCondition (bool cond)
{
  DoSetCondition (cond);
}

  void
Synchronizer::EventStart (void)
{
  DoEventStart ();
}

  uint64_t
Synchronizer::EventEnd (void)
{
  return NanosecondToTimeStep (DoEventEnd ());
}

  uint64_t
Synchronizer::TimeStepToNanosecond (uint64_t ts)
{
  switch (TimeStepPrecision::Get ()) {
  case TimeStepPrecision::S:
    return ts * 1000000000;
  case TimeStepPrecision::MS:
    return ts * 1000000;
  case TimeStepPrecision::US:
    return ts * 1000;
  case TimeStepPrecision::NS:
    return ts;
  case TimeStepPrecision::PS:
    return ts / 1000;
  case TimeStepPrecision::FS:
    return ts / 1000000;
  default:
    NS_ASSERT_MSG (false, "Synchronizer::TimeStepToNanosecond: "
        "Unexpected precision not implemented");
    return 0;
  }
}

  uint64_t
Synchronizer::NanosecondToTimeStep (uint64_t ns)
{
  switch (TimeStepPrecision::Get ()) {
  case TimeStepPrecision::S:
    return ns / 1000000000;
  case TimeStepPrecision::MS:
    return ns / 1000000;
  case TimeStepPrecision::US:
    return ns / 1000;
  case TimeStepPrecision::NS:
    return ns;
  case TimeStepPrecision::PS:
    return ns * 1000;
  case TimeStepPrecision::FS:
    return ns * 1000000;
  default:
    NS_ASSERT_MSG (false, "Synchronizer::NanosecondToTimeStep: "
        "Unexpected precision not implemented");
    return 0;
  }
}

}; // namespace ns3


