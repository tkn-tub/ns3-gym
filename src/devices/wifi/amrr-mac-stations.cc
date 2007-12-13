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
    m_shortRetry (0),
    m_longRetry (0),
    m_tx_ok (0),
    m_tx_err (0),
    m_tx_retr (0),
    m_tx_upper (0)
{}
AmrrMacStation::~AmrrMacStation ()
{}

void 
AmrrMacStation::ReportRxOk (double rxSnr, WifiMode txMode)
{}
void 
AmrrMacStation::ReportRtsFailed (void)
{
  m_shortRetry++;
}
void 
AmrrMacStation::ReportDataFailed (void)
{
  m_longRetry++;
}
void 
AmrrMacStation::ReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr)
{}
void 
AmrrMacStation::ReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr)
{
  UpdateRetry ();
  m_tx_ok++;
}
void 
AmrrMacStation::ReportFinalRtsFailed (void)
{
  UpdateRetry ();
  m_tx_err++;
}
void 
AmrrMacStation::ReportFinalDataFailed (void)
{
  UpdateRetry ();
  m_tx_err++;
}
void
AmrrMacStation::UpdateRetry (void)
{
  m_tx_retr += m_shortRetry + m_longRetry;
  m_shortRetry = 0;
  m_longRetry = 0;
}
void
AmrrMacStation::UpdateMode (void)
{
  if (Simulator::Now () < m_nextModeUpdate)
    {
      return;
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
