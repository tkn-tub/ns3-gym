/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2004,2005,2006 INRIA
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
#include "arf-mac-stations.h"
#include "ns3/assert.h"


namespace ns3 {
  
ArfMacStation::ArfMacStation (WifiMode defaultTxMode,
                              int minTimerTimeout,
                              int minSuccessThreshold)
  : MacStation (defaultTxMode)
{
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
  return GetNModes ();
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
      NS_ASSERT (m_retry >= 1);
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
      NS_ASSERT (m_retry >= 1);
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
ArfMacStation::ReportRxOk (double rxSnr, WifiMode txMode)
{}
void ArfMacStation::ReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
  NS_ASSERT (rtsSnr == 0);
}
void ArfMacStation::ReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr)
{
  NS_ASSERT (dataSnr == 0);
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
WifiMode
ArfMacStation::GetDataMode (uint32_t size)
{
  return GetMode (m_rate);
}
WifiMode
ArfMacStation::GetRtsMode (void)
{
  return GetMode (0);
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
  NS_ASSERT (timerTimeout >= m_minTimerTimeout);
  m_timerTimeout = timerTimeout;
}
void ArfMacStation::SetSuccessThreshold (int successThreshold)
{
  NS_ASSERT (successThreshold >= m_minSuccessThreshold);
  m_successThreshold = successThreshold;
}





ArfMacStations::ArfMacStations (WifiMode defaultTxMode)
  : MacStations (defaultTxMode)
{}
ArfMacStations::~ArfMacStations ()
{}
MacStation *
ArfMacStations::CreateStation (WifiMode defaultTxMode)
{
  return new ArfMacStation (defaultTxMode, 15, 10);
}

} // namespace ns3
