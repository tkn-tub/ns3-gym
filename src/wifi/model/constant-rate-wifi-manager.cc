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

#include "constant-rate-wifi-manager.h"

#include "ns3/string.h"
#include "ns3/assert.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (ConstantRateWifiManager);

TypeId
ConstantRateWifiManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ConstantRateWifiManager")
    .SetParent<WifiRemoteStationManager> ()
    .AddConstructor<ConstantRateWifiManager> ()
    .AddAttribute ("DataMode", "The transmission mode to use for every data packet transmission",
                   StringValue ("OfdmRate6Mbps"),
                   MakeWifiModeAccessor (&ConstantRateWifiManager::m_dataMode),
                   MakeWifiModeChecker ())
    .AddAttribute ("ControlMode", "The transmission mode to use for every control packet transmission.",
                   StringValue ("OfdmRate6Mbps"),
                   MakeWifiModeAccessor (&ConstantRateWifiManager::m_ctlMode),
                   MakeWifiModeChecker ())
  ;
  return tid;
}

ConstantRateWifiManager::ConstantRateWifiManager ()
{
}
ConstantRateWifiManager::~ConstantRateWifiManager ()
{
}


WifiRemoteStation *
ConstantRateWifiManager::DoCreateStation (void) const
{
  WifiRemoteStation *station = new WifiRemoteStation ();
  return station;
}


void
ConstantRateWifiManager::DoReportRxOk (WifiRemoteStation *station,
                                       double rxSnr, WifiMode txMode)
{
}
void
ConstantRateWifiManager::DoReportRtsFailed (WifiRemoteStation *station)
{
}
void
ConstantRateWifiManager::DoReportDataFailed (WifiRemoteStation *station)
{
}
void
ConstantRateWifiManager::DoReportRtsOk (WifiRemoteStation *st,
                                        double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
}
void
ConstantRateWifiManager::DoReportDataOk (WifiRemoteStation *st,
                                         double ackSnr, WifiMode ackMode, double dataSnr)
{
}
void
ConstantRateWifiManager::DoReportFinalRtsFailed (WifiRemoteStation *station)
{
}
void
ConstantRateWifiManager::DoReportFinalDataFailed (WifiRemoteStation *station)
{
}

WifiMode
ConstantRateWifiManager::DoGetDataMode (WifiRemoteStation *st, uint32_t size)
{
  return m_dataMode;
}
WifiMode
ConstantRateWifiManager::DoGetRtsMode (WifiRemoteStation *st)
{
  return m_ctlMode;
}

bool
ConstantRateWifiManager::IsLowLatency (void) const
{
  return true;
}

} // namespace ns3
