/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2004,2005,2006 INRIA
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

#include "mac-stations.h"
#include "mac-station.h"
#include "arf-mac-stations.h"

#include <cassert>

namespace ns3 {
  
ArfMacStation::ArfMacStation (ArfMacStations *stations,
                              int minTimerTimeout,
                              int minSuccessThreshold)
{
  m_stations = stations;
  m_minTimerTimeout = minTimerTimeout;
  m_minSuccessThreshold = minSuccessThreshold;
  m_successThreshold = m_minSuccessThreshold;
  m_timerTimeout = m_minTimerTimeout;
  m_rate = GetMinRate ();

  m_success = 0;
  m_failed = 0;
  m_recovery = false;
  m_retry = 0;
  m_timer = 0;
}
ArfMacStation::~ArfMacStation ()
{}

int 
ArfMacStation::GetMaxRate (void)
{
  return m_stations->GetNModes ();
}
int 
ArfMacStation::GetMinRate (void)
{
  return 0;
}

bool 
ArfMacStation::NeedRecoveryFallback (void)
{
  if (m_retry == 1) 
    {
      return true;
    } 
  else 
    {
      return false;
    }
}
bool 
ArfMacStation::NeedNormalFallback (void)
{
  int retryMod = (m_retry - 1) % 2;
  if (retryMod == 1) 
    {
      return true;
    } 
  else 
    {
      return false;
    }
}



void 
ArfMacStation::ReportRtsFailed (void)
{}
/**
 * It is important to realize that "recovery" mode starts after failure of
 * the first transmission after a rate increase and ends at the first successful
 * transmission. Specifically, recovery mode transcends retransmissions boundaries.
 * Fundamentally, ARF handles each data transmission independently, whether it
 * is the initial transmission of a packet or the retransmission of a packet.
 * The fundamental reason for this is that there is a backoff between each data
 * transmission, be it an initial transmission or a retransmission.
 */
void 
ArfMacStation::ReportDataFailed (void)
{
  m_timer++;
  m_failed++;
  m_retry++;
  m_success = 0;

  if (m_recovery) 
    {
      assert (m_retry >= 1);
      if (NeedRecoveryFallback ()) 
        {
          ReportRecoveryFailure ();
          if (m_rate != GetMinRate ()) 
            {
              m_rate--;
            }
        }
      m_timer = 0;
    } 
  else 
    {
      assert (m_retry >= 1);
      if (NeedNormalFallback ()) 
        {
          ReportFailure ();
          if (m_rate != GetMinRate ()) 
            {
              m_rate--;
            }
        }
      if (m_retry >= 2) 
        {
          m_timer = 0;
        }
    }
}
void 
ArfMacStation::ReportRxOk (double rxSnr, uint8_t txMode)
{}
void ArfMacStation::ReportRtsOk (double ctsSnr, uint8_t ctsMode, uint8_t rtsSnr)
{
  assert (rtsSnr == 0);
}
void ArfMacStation::ReportDataOk (double ackSnr, uint8_t ackMode, uint8_t dataSnr)
{
  assert (dataSnr == 0);
  m_timer++;
  m_success++;
  m_failed = 0;
  m_recovery = false;
  m_retry = 0;
  if ((m_success == GetSuccessThreshold () ||
       m_timer == GetTimerTimeout ()) &&
      (m_rate < (GetMaxRate () - 1))) 
    {
      m_rate++;
      m_timer = 0;
      m_success = 0;
      m_recovery = true;
    }
}
uint8_t 
ArfMacStation::SnrToSnr (double snr)
{
  return 0;
}
uint8_t 
ArfMacStation::GetDataMode (int size)
{
  return m_rate;
}
uint8_t 
ArfMacStation::GetRtsMode (void)
{
  return 0;
}

void ArfMacStation::ReportRecoveryFailure (void)
{}
void ArfMacStation::ReportFailure (void)
{}
int ArfMacStation::GetMinTimerTimeout (void)
{
  return m_minTimerTimeout;
}
int ArfMacStation::GetMinSuccessThreshold (void)
{
  return m_minSuccessThreshold;
}
int ArfMacStation::GetTimerTimeout (void)
{
  return m_timerTimeout;
}
int ArfMacStation::GetSuccessThreshold (void)
{
  return m_successThreshold;
}
void ArfMacStation::SetTimerTimeout (int timerTimeout)
{
  assert (timerTimeout >= m_minTimerTimeout);
  m_timerTimeout = timerTimeout;
}
void ArfMacStation::SetSuccessThreshold (int successThreshold)
{
  assert (successThreshold >= m_minSuccessThreshold);
  m_successThreshold = successThreshold;
}





ArfMacStations::ArfMacStations (uint8_t nModes)
  : m_nModes (nModes)
{}
ArfMacStations::~ArfMacStations ()
{}
MacStation *
ArfMacStations::CreateStation (void)
{
  /* XXX: use mac to access user and PHY params. */
  return new ArfMacStation (this, 15, 10);
}

uint8_t
ArfMacStations::GetNModes (void) const
{
  return m_nModes;
}


}; // namespace ns3
