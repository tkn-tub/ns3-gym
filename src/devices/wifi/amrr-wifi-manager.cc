/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2003,2007 INRIA
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

#include "amrr-wifi-manager.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"

NS_LOG_COMPONENT_DEFINE ("AmrrWifiRemoteStation");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (AmrrWifiManager);

TypeId
AmrrWifiManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::AmrrWifiManager")
    .SetParent<WifiRemoteStationManager> ()
    .AddConstructor<AmrrWifiManager> ()
    .AddAttribute ("UpdatePeriod",
                   "The interval between decisions about rate control changes",
                   Seconds (1.0),
                   MakeTimeAccessor (&AmrrWifiManager::m_updatePeriod),
                   MakeTimeChecker ())
    .AddAttribute ("FailureRatio",
                   "Ratio of minimum erronous transmissions needed to switch to a lower rate",
                   Double (1.0/3.0),
                   MakeDoubleAccessor (&AmrrWifiManager::m_failureRatio),
                   MakeDoubleChecker<double> (0.0, 1.0))
    .AddAttribute ("SuccessRatio",
                   "Ratio of maximum erronous transmissions needed to switch to a higher rate",
                   Double (1.0/10.0),
                   MakeDoubleAccessor (&AmrrWifiManager::m_successRatio),
                   MakeDoubleChecker<double> (0.0, 1.0))
    .AddAttribute ("MaxSuccessThreshold",
                   "Maximum number of consecutive success periods needed to switch to a higher rate",
                   Uinteger (10),
                   MakeUintegerAccessor (&AmrrWifiManager::m_maxSuccessThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MinSuccessThreshold",
                   "Minimum number of consecutive success periods needed to switch to a higher rate",
                   Uinteger (1),
                   MakeUintegerAccessor (&AmrrWifiManager::m_minSuccessThreshold),
                   MakeUintegerChecker<uint32_t> ())
    ;
  return tid;
}

AmrrWifiManager::AmrrWifiManager ()
{}
WifiRemoteStation *
AmrrWifiManager::CreateStation (void)
{
  return new AmrrWifiRemoteStation (this);
}

AmrrWifiRemoteStation::AmrrWifiRemoteStation (Ptr<AmrrWifiManager> stations)
  : m_stations (stations),
    m_nextModeUpdate (Simulator::Now () + stations->m_updatePeriod),
    m_tx_ok (0),
    m_tx_err (0),
    m_tx_retr (0),
    m_retry (0),
    m_txrate (0),
    m_successThreshold (m_stations->m_minSuccessThreshold),
    m_success (0),
    m_recovery (false)
{}
AmrrWifiRemoteStation::~AmrrWifiRemoteStation ()
{}

void 
AmrrWifiRemoteStation::DoReportRxOk (double rxSnr, WifiMode txMode)
{}
void 
AmrrWifiRemoteStation::DoReportRtsFailed (void)
{}
void 
AmrrWifiRemoteStation::DoReportDataFailed (void)
{
  m_retry++;
  m_tx_retr++;
}
void 
AmrrWifiRemoteStation::DoReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr)
{}
void 
AmrrWifiRemoteStation::DoReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr)
{
  m_retry = 0;
  m_tx_ok++;
}
void 
AmrrWifiRemoteStation::DoReportFinalRtsFailed (void)
{}
void 
AmrrWifiRemoteStation::DoReportFinalDataFailed (void)
{
  m_retry = 0;
  m_tx_err++;
}
bool
AmrrWifiRemoteStation::IsMinRate (void) const
{
  return (m_txrate == 0);
}
bool
AmrrWifiRemoteStation::IsMaxRate (void) const
{
  NS_ASSERT (m_txrate + 1 <= GetNSupportedModes ());
  return (m_txrate + 1 == GetNSupportedModes ());
}
bool
AmrrWifiRemoteStation::IsSuccess (void) const
{
  return (m_tx_retr + m_tx_err) < m_tx_ok * m_stations->m_successRatio;
}
bool
AmrrWifiRemoteStation::IsFailure (void) const
{
  return (m_tx_retr + m_tx_err) > m_tx_ok * m_stations->m_failureRatio;
}
bool
AmrrWifiRemoteStation::IsEnough (void) const
{
  return (m_tx_retr + m_tx_err + m_tx_ok) > 10;
}
void 
AmrrWifiRemoteStation::ResetCnt (void)
{
  m_tx_ok = 0;
  m_tx_err = 0;
  m_tx_retr = 0;
}
void 
AmrrWifiRemoteStation::IncreaseRate (void)
{
  m_txrate++;
  NS_ASSERT (m_txrate < GetNSupportedModes ());
}
void 
AmrrWifiRemoteStation::DecreaseRate (void)
{
  m_txrate--;
}

void
AmrrWifiRemoteStation::UpdateMode (void)
{
  if (Simulator::Now () < m_nextModeUpdate)
    {
      return;
    }
  m_nextModeUpdate = Simulator::Now () + m_stations->m_updatePeriod;
  NS_LOG_DEBUG ("Update");

  bool needChange = false;

  if (IsSuccess () && IsEnough ()) 
    {
      m_success++;
      NS_LOG_DEBUG ("++ success="<<m_success<<" successThreshold="<<m_successThreshold<<
                    " tx_ok="<<m_tx_ok<<" tx_err="<<m_tx_err<<" tx_retr="<<m_tx_retr<<
                    " rate="<<m_txrate<<" n-supported-rates="<<GetNSupportedModes ());
      if (m_success >= m_successThreshold &&
          !IsMaxRate ()) 
        {
          m_recovery = true;
          m_success = 0;
          IncreaseRate ();
          needChange = true;
        } 
      else 
        {
          m_recovery = false;
        }
    } 
  else if (IsFailure ()) 
    {
      m_success = 0;
      NS_LOG_DEBUG ("-- success="<<m_success<<" successThreshold="<<m_successThreshold<<
                    " tx_ok="<<m_tx_ok<<" tx_err="<<m_tx_err<<" tx_retr="<<m_tx_retr<<
                    " rate="<<m_txrate<<" n-supported-rates="<<GetNSupportedModes ());
      if (!IsMinRate ()) 
        {
          if (m_recovery) 
            {
              m_successThreshold *= 2;
              m_successThreshold = std::min (m_successThreshold,
                                             m_stations->m_maxSuccessThreshold);
            } 
          else 
            {
              m_successThreshold = m_stations->m_minSuccessThreshold;
            }
          m_recovery = false;
          DecreaseRate ();
          needChange = true;
        } 
      else 
        {
          m_recovery = false;
        }
    }
  if (IsEnough () || needChange) 
    {
      NS_LOG_DEBUG ("Reset");
      ResetCnt ();
    }
}

Ptr<WifiRemoteStationManager>
AmrrWifiRemoteStation::GetManager (void) const
{
  return m_stations;
}
WifiMode 
AmrrWifiRemoteStation::DoGetDataMode (uint32_t size)
{
  UpdateMode ();
  NS_ASSERT (m_txrate < GetNSupportedModes ());
  uint32_t rateIndex;
  if (m_retry < 1)
    {
      rateIndex = m_txrate;
    }
  else if (m_retry < 2)
    {
      if (m_txrate > 0)
        {
          rateIndex = m_txrate - 1;
        }
      else
        {
          rateIndex = m_txrate;
        }
    }
  else if (m_retry < 3)
    {
      if (m_txrate > 1)
        {
          rateIndex = m_txrate - 2;
        }
      else
        {
          rateIndex = m_txrate;
        }
    }
  else
    {
      if (m_txrate > 2)
        {
          rateIndex = m_txrate - 3;
        }
      else
        {
          rateIndex = m_txrate;
        }
    }

  return GetSupportedMode (rateIndex);
}
WifiMode 
AmrrWifiRemoteStation::DoGetRtsMode (void)
{
  UpdateMode ();
  // XXX: can we implement something smarter ?
  return GetSupportedMode (0);
}

} // namespace ns3
