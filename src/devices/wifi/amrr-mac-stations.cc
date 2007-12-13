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

#include "amrr-mac-stations.h"
#include "ns3/default-value.h"
#include "ns3/time-default-value.h"
#include "ns3/simulator.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("AmrrMacStation");

namespace ns3 {

static TimeDefaultValue g_updatePeriod
("WifiAmrrUpdatePeriod",
 "The interval between decisions about rate control changes",
 Seconds (1.0));
static NumericDefaultValue<double> g_failureRatio
("WifiAmrrFailureRatio",
 "Ratio of erronous transmissions needed to switch to a lower rate",
 1.0/3.0);
static NumericDefaultValue<double> g_successRatio
("WifiAmrrSuccessRatio",
 "Ratio of successful transmissions needed to switch to a higher rate",
 1.0/10.0);
static NumericDefaultValue<uint32_t> g_maxSuccessThreshold
("WifiAmrrMaxSuccessThreshold",
 "maximum number of consecutive success periods needed to switch to a higher rate",
 10);
static NumericDefaultValue<uint32_t> g_minSuccessThreshold
("WifiAmrrMinSuccessThreshold",
 "minimum number of consecutive success periods needed to switch to a higher rate",
 1);

AmrrMacStations::AmrrMacStations (WifiMode defaultTxMode)
  : MacStations (defaultTxMode),
    m_updatePeriod (g_updatePeriod.GetValue ())
{}
MacStation *
AmrrMacStations::CreateStation (void)
{
  return new AmrrMacStation (this);
}

AmrrMacStation::AmrrMacStation (AmrrMacStations *stations)
  : m_stations (stations),
    m_nextModeUpdate (Simulator::Now () + stations->m_updatePeriod),
    m_tx_ok (0),
    m_tx_err (0),
    m_tx_retr (0),
    m_successThreshold (m_stations->m_minSuccessThreshold),
    m_success (0),
    m_recovery (false)
{}
AmrrMacStation::~AmrrMacStation ()
{}

void 
AmrrMacStation::ReportRxOk (double rxSnr, WifiMode txMode)
{}
void 
AmrrMacStation::ReportRtsFailed (void)
{}
void 
AmrrMacStation::ReportDataFailed (void)
{
  m_tx_retr++;
}
void 
AmrrMacStation::ReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr)
{}
void 
AmrrMacStation::ReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr)
{
  m_tx_ok++;
}
void 
AmrrMacStation::ReportFinalRtsFailed (void)
{}
void 
AmrrMacStation::ReportFinalDataFailed (void)
{
  m_tx_err++;
}
bool
AmrrMacStation::IsMinRate (void) const
{
  return (m_txrate == 0);
}
bool
AmrrMacStation::IsMaxRate (void) const
{
  return (m_txrate + 1 < GetNSupportedModes ());
}
bool
AmrrMacStation::IsSuccess (void) const
{
  return m_tx_ok > (m_tx_retr + m_tx_err) * m_stations->m_successRatio;
}
bool
AmrrMacStation::IsFailure (void) const
{
  return (m_tx_retr + m_tx_err) > m_tx_ok * m_stations->m_failureRatio;
}
bool
AmrrMacStation::IsEnough (void) const
{
  return (m_tx_retr + m_tx_err + m_tx_ok) > 10;
}
void 
AmrrMacStation::ResetCnt (void)
{
  m_tx_ok = 0;
  m_tx_err = 0;
  m_tx_retr = 0;
}
void 
AmrrMacStation::IncreaseRate (void)
{
  m_txrate++;
}
void 
AmrrMacStation::DecreaseRate (void)
{
  m_txrate--;
}

void
AmrrMacStation::UpdateMode (void)
{
  if (Simulator::Now () < m_nextModeUpdate)
    {
      return;
    }

  bool needChange = false;

  if (IsSuccess () && IsEnough ()) 
    {
      m_success++;
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
      ResetCnt ();
    }
}

AmrrMacStations *
AmrrMacStation::GetStations (void) const
{
  return m_stations;
}
WifiMode 
AmrrMacStation::DoGetDataMode (uint32_t size)
{
  UpdateMode ();
  //XXX
  return GetSupportedMode (0);
}
WifiMode 
AmrrMacStation::DoGetRtsMode (void)
{
  UpdateMode ();
  // XXX: can we implement something smarter ?
  return GetSupportedMode (0);
}

} // namespace ns3
