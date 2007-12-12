/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2004,2005 INRIA
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
#include "cr-mac-stations.h"

#include "ns3/assert.h"

namespace ns3 {

CrMacStation::CrMacStation (CrMacStations *stations)
  : m_stations (stations)
{}
CrMacStation::~CrMacStation ()
{}

void 
CrMacStation::ReportRxOk (double rxSnr, WifiMode txMode)
{}
void 
CrMacStation::ReportRtsFailed (void)
{}
void 
CrMacStation::ReportDataFailed (void)
{}
void 
CrMacStation::ReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr)
{}
void 
CrMacStation::ReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr)
{}
WifiMode 
CrMacStation::DoGetDataMode (uint32_t size)
{
  return m_stations->GetDataMode ();
}
WifiMode 
CrMacStation::DoGetRtsMode (void)
{
  return m_stations->GetCtlMode ();
}
CrMacStations *
CrMacStation::GetStations (void) const
{
  return m_stations;
}



CrMacStations::CrMacStations (WifiMode dataMode, WifiMode ctlMode)
  : MacStations (ctlMode),
    m_dataMode (dataMode),
    m_ctlMode (ctlMode)
{}
CrMacStations::~CrMacStations ()
{}

WifiMode 
CrMacStations::GetDataMode (void) const
{
  return m_dataMode;
}
WifiMode 
CrMacStations::GetCtlMode (void) const
{
  return m_ctlMode;
}


MacStation *
CrMacStations::CreateStation (void)
{
  return new CrMacStation (this);
}

} // namespace ns3
