/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage.inria.fr>
 */

#include "system-wall-clock-ms.h"
#include <sys/times.h>
#include <unistd.h>
#include <limits.h>

namespace ns3 {

class SystemWallClockMsPrivate {
public:
  void Start (void);
  unsigned long long End (void);
  double GetElapsedReal (void) const;
  double GetElapsedUser (void) const;
  double GetElapsedSystem (void) const;

private:
  struct tms m_startTimes;
  clock_t m_startTime;
  double m_elapsedReal;
  double m_elapsedUser;
  double m_elapsedSystem;
};

void 
SystemWallClockMsPrivate::Start (void)
{
  m_startTime = times (&m_startTimes);
}

unsigned long long 
SystemWallClockMsPrivate::End (void)
{
  static long ticksPerSecond = sysconf (_SC_CLK_TCK);

  struct tms endTimes;
  clock_t endTime = times (&endTimes);

  m_elapsedReal = 1e3 * static_cast<double> (endTime - m_startTime) / ticksPerSecond;
  m_elapsedUser = 1e3 * static_cast<double> (endTimes.tms_utime - m_startTimes.tms_utime) / ticksPerSecond;
  m_elapsedSystem = 1e3 * static_cast<double> (endTimes.tms_stime - m_startTimes.tms_stime) / ticksPerSecond;

  return m_elapsedReal;
}

double
SystemWallClockMsPrivate::GetElapsedReal (void) const
{
  return m_elapsedReal;
}

double
SystemWallClockMsPrivate::GetElapsedUser (void) const
{
  return m_elapsedUser;
}

double
SystemWallClockMsPrivate::GetElapsedSystem (void) const
{
  return m_elapsedSystem;
}
  
SystemWallClockMs::SystemWallClockMs ()
  : m_priv (new SystemWallClockMsPrivate ())
{}

SystemWallClockMs::~SystemWallClockMs ()
{
  delete m_priv;
  m_priv = 0;
}

void
SystemWallClockMs::Start (void)
{
  m_priv->Start ();
}
unsigned long long
SystemWallClockMs::End (void)
{
  return m_priv->End ();
}

double
SystemWallClockMs::GetElapsedReal (void) const
{
  return m_priv->GetElapsedReal ();
}

double
SystemWallClockMs::GetElapsedUser (void) const
{
  return m_priv->GetElapsedUser ();
}

double
SystemWallClockMs::GetElapsedSystem (void) const
{
  return m_priv->GetElapsedSystem ();
}

}; // namespace ns3
