/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
#include "ideal-wifi-manager.h"
#include "wifi-phy.h"
#include "ns3/assert.h"
#include "ns3/double.h"
#include <cmath>

#define Min(a,b) ((a < b) ? a : b)

namespace ns3 {

/**
 * \brief hold per-remote-station state for Ideal Wifi manager.
 *
 * This struct extends from WifiRemoteStation struct to hold additional
 * information required by the Ideal Wifi manager
 */
struct IdealWifiRemoteStation : public WifiRemoteStation
{
  double m_lastSnr;
};

NS_OBJECT_ENSURE_REGISTERED (IdealWifiManager)
  ;

TypeId
IdealWifiManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::IdealWifiManager")
    .SetParent<WifiRemoteStationManager> ()
    .AddConstructor<IdealWifiManager> ()
    .AddAttribute ("BerThreshold",
                   "The maximum Bit Error Rate acceptable at any transmission mode",
                   DoubleValue (10e-6),
                   MakeDoubleAccessor (&IdealWifiManager::m_ber),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

IdealWifiManager::IdealWifiManager ()
{
}
IdealWifiManager::~IdealWifiManager ()
{
}

void
IdealWifiManager::SetupPhy (Ptr<WifiPhy> phy)
{
  uint32_t nModes = phy->GetNModes ();
  for (uint32_t i = 0; i < nModes; i++)
    {
      WifiMode mode = phy->GetMode (i);
      AddModeSnrThreshold (mode, phy->CalculateSnr (mode, m_ber));
    }

  WifiRemoteStationManager::SetupPhy (phy);
}

double
IdealWifiManager::GetSnrThreshold (WifiMode mode) const
{
  for (Thresholds::const_iterator i = m_thresholds.begin (); i != m_thresholds.end (); i++)
    {
      if (mode == i->second)
        {
          return i->first;
        }
    }
  NS_ASSERT (false);
  return 0.0;
}

void
IdealWifiManager::AddModeSnrThreshold (WifiMode mode, double snr)
{
  m_thresholds.push_back (std::make_pair (snr,mode));
}

WifiRemoteStation *
IdealWifiManager::DoCreateStation (void) const
{
  IdealWifiRemoteStation *station = new IdealWifiRemoteStation ();
  station->m_lastSnr = 0.0;
  return station;
}


void
IdealWifiManager::DoReportRxOk (WifiRemoteStation *station,
                                double rxSnr, WifiMode txMode)
{
}
void
IdealWifiManager::DoReportRtsFailed (WifiRemoteStation *station)
{
}
void
IdealWifiManager::DoReportDataFailed (WifiRemoteStation *station)
{
}
void
IdealWifiManager::DoReportRtsOk (WifiRemoteStation *st,
                                 double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
  IdealWifiRemoteStation *station = (IdealWifiRemoteStation *)st;
  station->m_lastSnr = rtsSnr;
}
void
IdealWifiManager::DoReportDataOk (WifiRemoteStation *st,
                                  double ackSnr, WifiMode ackMode, double dataSnr)
{
  IdealWifiRemoteStation *station = (IdealWifiRemoteStation *)st;
  station->m_lastSnr = dataSnr;
}
void
IdealWifiManager::DoReportFinalRtsFailed (WifiRemoteStation *station)
{
}
void
IdealWifiManager::DoReportFinalDataFailed (WifiRemoteStation *station)
{
}

WifiTxVector
IdealWifiManager::DoGetDataTxVector (WifiRemoteStation *st, uint32_t size)
{
  IdealWifiRemoteStation *station = (IdealWifiRemoteStation *)st;
  // We search within the Supported rate set the mode with the
  // highest snr threshold possible which is smaller than m_lastSnr
  // to ensure correct packet delivery.
  double maxThreshold = 0.0;
  WifiMode maxMode = GetDefaultMode ();
  for (uint32_t i = 0; i < GetNSupported (station); i++)
    {
      WifiMode mode = GetSupported (station, i);
      double threshold = GetSnrThreshold (mode);
      if (threshold > maxThreshold
          && threshold < station->m_lastSnr)
        {
          maxThreshold = threshold;
          maxMode = mode;
        }
    }
  return WifiTxVector (maxMode, GetDefaultTxPowerLevel (), GetLongRetryCount (station), GetShortGuardInterval (station), Min (GetNumberOfReceiveAntennas (station),GetNumberOfTransmitAntennas()), GetNumberOfTransmitAntennas (station), GetStbc (station));
}
WifiTxVector
IdealWifiManager::DoGetRtsTxVector (WifiRemoteStation *st)
{
  IdealWifiRemoteStation *station = (IdealWifiRemoteStation *)st;
  // We search within the Basic rate set the mode with the highest
  // snr threshold possible which is smaller than m_lastSnr to
  // ensure correct packet delivery.
  double maxThreshold = 0.0;
  WifiMode maxMode = GetDefaultMode ();
  for (uint32_t i = 0; i < GetNBasicModes (); i++)
    {
      WifiMode mode = GetBasicMode (i);
      double threshold = GetSnrThreshold (mode);
      if (threshold > maxThreshold
          && threshold < station->m_lastSnr)
        {
          maxThreshold = threshold;
          maxMode = mode;
        }
    }
  return WifiTxVector (maxMode, GetDefaultTxPowerLevel (), GetShortRetryCount (station), GetShortGuardInterval (station), Min (GetNumberOfReceiveAntennas (station),GetNumberOfTransmitAntennas()), GetNumberOfTransmitAntennas (station), GetStbc (station));
}

bool
IdealWifiManager::IsLowLatency (void) const
{
  return true;
}

} // namespace ns3
