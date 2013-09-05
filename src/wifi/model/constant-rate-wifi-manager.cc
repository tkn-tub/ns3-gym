/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("ConstantRateWifiManager");

#define Min(a,b) ((a < b) ? a : b)

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
  NS_LOG_FUNCTION (this);
}
ConstantRateWifiManager::~ConstantRateWifiManager ()
{
  NS_LOG_FUNCTION (this);
}


WifiRemoteStation *
ConstantRateWifiManager::DoCreateStation (void) const
{
  NS_LOG_FUNCTION (this);
  WifiRemoteStation *station = new WifiRemoteStation ();
  return station;
}


void
ConstantRateWifiManager::DoReportRxOk (WifiRemoteStation *station,
                                       double rxSnr, WifiMode txMode)
{
  NS_LOG_FUNCTION (this << station << rxSnr << txMode);
}
void
ConstantRateWifiManager::DoReportRtsFailed (WifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
}
void
ConstantRateWifiManager::DoReportDataFailed (WifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
}
void
ConstantRateWifiManager::DoReportRtsOk (WifiRemoteStation *st,
                                        double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
  NS_LOG_FUNCTION (this << st << ctsSnr << ctsMode << rtsSnr);
}
void
ConstantRateWifiManager::DoReportDataOk (WifiRemoteStation *st,
                                         double ackSnr, WifiMode ackMode, double dataSnr)
{
  NS_LOG_FUNCTION (this << st << ackSnr << ackMode << dataSnr);
}
void
ConstantRateWifiManager::DoReportFinalRtsFailed (WifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
}
void
ConstantRateWifiManager::DoReportFinalDataFailed (WifiRemoteStation *station)
{
  NS_LOG_FUNCTION (this << station);
}

WifiTxVector
ConstantRateWifiManager::DoGetDataTxVector (WifiRemoteStation *st, uint32_t size)
{
  NS_LOG_FUNCTION (this << st << size);
  return WifiTxVector (m_dataMode, GetDefaultTxPowerLevel (), GetLongRetryCount (st), GetShortGuardInterval (st), Min (GetNumberOfReceiveAntennas (st),GetNumberOfTransmitAntennas()), GetNumberOfTransmitAntennas (st), GetStbc (st));
}
WifiTxVector
ConstantRateWifiManager::DoGetRtsTxVector (WifiRemoteStation *st)
{
  NS_LOG_FUNCTION (this << st);
  return WifiTxVector (m_ctlMode, GetDefaultTxPowerLevel (), GetShortRetryCount (st), GetShortGuardInterval (st), Min (GetNumberOfReceiveAntennas (st),GetNumberOfTransmitAntennas()), GetNumberOfTransmitAntennas (st), GetStbc (st));
}

bool
ConstantRateWifiManager::IsLowLatency (void) const
{
  NS_LOG_FUNCTION (this);
  return true;
}

} // namespace ns3
