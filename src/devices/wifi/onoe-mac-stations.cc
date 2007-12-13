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

#include "onoe-mac-stations.h"
#include "ns3/default-value.h"
#include "ns3/time-default-value.h"
#include "ns3/simulator.h"

namespace ns3 {

static TimeDefaultValue g_updatePeriod
("WifiOnoeUpdatePeriod",
 "The interval between decisions about rate control changes",
 Seconds (1.0));
static NumericDefaultValue<uint32_t> g_addCreditThreshold
("WifiOnoeAddCreditThreshold",
 "Add credit threshold",
 10);
static NumericDefaultValue<uint32_t> g_raiseThreshold
("WifiOnoeRaiseThreshold",
 "Raise threshold",
 10);

OnoeMacStations::OnoeMacStations (WifiMode defaultTxMode)
  : MacStations (defaultTxMode),
    m_updatePeriod (g_updatePeriod.GetValue ()),
    m_addCreditThreshold (g_addCreditThreshold.GetValue ()),
    m_raiseThreshold (g_raiseThreshold.GetValue ())
{}
MacStation *
OnoeMacStations::CreateStation (void)
{
  return new OnoeMacStation (this);
}

OnoeMacStation::OnoeMacStation (OnoeMacStations *stations)
  : m_stations (stations),
    m_nextModeUpdate (Simulator::Now () + stations->m_updatePeriod)
{}
OnoeMacStation::~OnoeMacStation ()
{}

void 
OnoeMacStation::ReportRxOk (double rxSnr, WifiMode txMode)
{}
void 
OnoeMacStation::ReportRtsFailed (void)
{
  m_shortRetry++;
}
void 
OnoeMacStation::ReportDataFailed (void)
{
  m_longRetry++;
}
void 
OnoeMacStation::ReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr)
{}
void 
OnoeMacStation::ReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr)
{
  UpdateRetry ();
  m_tx_ok++;
}
void 
OnoeMacStation::ReportFinalRtsFailed (void)
{
  UpdateRetry ();
  m_tx_err++;
}
void 
OnoeMacStation::ReportFinalDataFailed (void)
{
  UpdateRetry ();
  m_tx_err++;
}
void
OnoeMacStation::UpdateRetry (void)
{
  m_tx_retr += m_shortRetry + m_longRetry;
  m_shortRetry = 0;
  m_longRetry = 0;
}
void
OnoeMacStation::UpdateMode (void)
{
  if (Simulator::Now () < m_nextModeUpdate)
    {
      return;
    }
}

OnoeMacStations *
OnoeMacStation::GetStations (void) const
{
  return m_stations;
}
WifiMode 
OnoeMacStation::DoGetDataMode (uint32_t size)
{
  UpdateMode ();
  // XXX
  return GetSupportedMode (0);
}
WifiMode 
OnoeMacStation::DoGetRtsMode (void)
{
  UpdateMode ();
  // XXX: can we implement something smarter ?
  return GetSupportedMode (0);
}

} // namespace ns3
