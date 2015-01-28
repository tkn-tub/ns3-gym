/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
 * Author: Federico Maguolo <maguolof@dei.unipd.it>
 */

#include "rraa-wifi-manager.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/simulator.h"

#define Min(a,b) ((a < b) ? a : b)

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RraaWifiManager");

/**
 * \brief hold per-remote-station state for RRAA Wifi manager.
 *
 * This struct extends from WifiRemoteStation struct to hold additional
 * information required by the RRAA Wifi manager
 */
struct RraaWifiRemoteStation : public WifiRemoteStation
{
  uint32_t m_counter;
  uint32_t m_failed;
  uint32_t m_rtsWnd;
  uint32_t m_rtsCounter;
  Time m_lastReset;
  bool m_rtsOn;
  bool m_lastFrameFail;
  bool m_initialized;

  uint32_t m_rate;
};

NS_OBJECT_ENSURE_REGISTERED (RraaWifiManager);

TypeId
RraaWifiManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RraaWifiManager")
    .SetParent<WifiRemoteStationManager> ()
    .AddConstructor<RraaWifiManager> ()
    .AddAttribute ("Basic",
                   "If true the RRAA-BASIC algorithm will be used, otherwise the RRAA wil be used",
                   BooleanValue (false),
                   MakeBooleanAccessor (&RraaWifiManager::m_basic),
                   MakeBooleanChecker ())
    .AddAttribute ("Timeout",
                   "Timeout for the RRAA BASIC loss estimaton block (s)",
                   TimeValue (Seconds (0.05)),
                   MakeTimeAccessor (&RraaWifiManager::m_timeout),
                   MakeTimeChecker ())
    .AddAttribute ("ewndFor54mbps",
                   "ewnd parameter for 54 Mbs data mode",
                   UintegerValue (40),
                   MakeUintegerAccessor (&RraaWifiManager::m_ewndfor54),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ewndFor48mbps",
                   "ewnd parameter for 48 Mbs data mode",
                   UintegerValue (40),
                   MakeUintegerAccessor (&RraaWifiManager::m_ewndfor48),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ewndFor36mbps",
                   "ewnd parameter for 36 Mbs data mode",
                   UintegerValue (40),
                   MakeUintegerAccessor (&RraaWifiManager::m_ewndfor36),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ewndFor24mbps",
                   "ewnd parameter for 24 Mbs data mode",
                   UintegerValue (40),
                   MakeUintegerAccessor (&RraaWifiManager::m_ewndfor24),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ewndFor18mbps",
                   "ewnd parameter for 18 Mbs data mode",
                   UintegerValue (20),
                   MakeUintegerAccessor (&RraaWifiManager::m_ewndfor18),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ewndFor12mbps",
                   "ewnd parameter for 12 Mbs data mode",
                   UintegerValue (20),
                   MakeUintegerAccessor (&RraaWifiManager::m_ewndfor12),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ewndFor9mbps",
                   "ewnd parameter for 9 Mbs data mode",
                   UintegerValue (10),
                   MakeUintegerAccessor (&RraaWifiManager::m_ewndfor9),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ewndFor6mbps",
                   "ewnd parameter for 6 Mbs data mode",
                   UintegerValue (6),
                   MakeUintegerAccessor (&RraaWifiManager::m_ewndfor6),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("poriFor48mbps",
                   "Pori parameter for 48 Mbs data mode",
                   DoubleValue (0.047),
                   MakeDoubleAccessor (&RraaWifiManager::m_porifor48),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("poriFor36mbps",
                   "Pori parameter for 36 Mbs data mode",
                   DoubleValue (0.115),
                   MakeDoubleAccessor (&RraaWifiManager::m_porifor36),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("poriFor24mbps",
                   "Pori parameter for 24 Mbs data mode",
                   DoubleValue (0.1681),
                   MakeDoubleAccessor (&RraaWifiManager::m_porifor24),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("poriFor18mbps",
                   "Pori parameter for 18 Mbs data mode",
                   DoubleValue (0.1325),
                   MakeDoubleAccessor (&RraaWifiManager::m_porifor18),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("poriFor12mbps",
                   "Pori parameter for 12 Mbs data mode",
                   DoubleValue (0.1861),
                   MakeDoubleAccessor (&RraaWifiManager::m_porifor12),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("poriFor9mbps",
                   "Pori parameter for 9 Mbs data mode",
                   DoubleValue (0.1434),
                   MakeDoubleAccessor (&RraaWifiManager::m_porifor9),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("poriFor6mbps",
                   "Pori parameter for 6 Mbs data mode",
                   DoubleValue (0.5),
                   MakeDoubleAccessor (&RraaWifiManager::m_porifor6),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("pmtlFor54mbps",
                   "Pmtl parameter for 54 Mbs data mode",
                   DoubleValue (0.094),
                   MakeDoubleAccessor (&RraaWifiManager::m_pmtlfor54),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("pmtlFor48mbps",
                   "Pmtl parameter for 48 Mbs data mode",
                   DoubleValue (0.23),
                   MakeDoubleAccessor (&RraaWifiManager::m_pmtlfor48),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("pmtlFor36mbps",
                   "Pmtl parameter for 36 Mbs data mode",
                   DoubleValue (0.3363),
                   MakeDoubleAccessor (&RraaWifiManager::m_pmtlfor36),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("pmtlFor24mbps",
                   "Pmtl parameter for 24 Mbs data mode",
                   DoubleValue (0.265),
                   MakeDoubleAccessor (&RraaWifiManager::m_pmtlfor24),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("pmtlFor18mbps",
                   "Pmtl parameter for 18 Mbs data mode",
                   DoubleValue (0.3722),
                   MakeDoubleAccessor (&RraaWifiManager::m_pmtlfor18),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("pmtlFor12mbps",
                   "Pmtl parameter for 12 Mbs data mode",
                   DoubleValue (0.2868),
                   MakeDoubleAccessor (&RraaWifiManager::m_pmtlfor12),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("pmtlFor9mbps",
                   "Pmtl parameter for 9 Mbs data mode",
                   DoubleValue (0.3932),
                   MakeDoubleAccessor (&RraaWifiManager::m_pmtlfor9),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}


RraaWifiManager::RraaWifiManager ()
{
}
RraaWifiManager::~RraaWifiManager ()
{
}


WifiRemoteStation *
RraaWifiManager::DoCreateStation (void) const
{
  RraaWifiRemoteStation *station = new RraaWifiRemoteStation ();
  station->m_initialized = false;
  station->m_rtsWnd = 0;
  station->m_rtsCounter = 0;
  station->m_rtsOn = false;
  station->m_lastFrameFail = false;
  return station;
}

void
RraaWifiManager::ResetCountersBasic (RraaWifiRemoteStation *station)
{
  if (!station->m_initialized)
    {
      station->m_rate = GetMaxRate (station);
      station->m_initialized = true;
    }
  station->m_failed = 0;
  station->m_counter = GetThresholds (station, station->m_rate).ewnd;
  station->m_lastReset = Simulator::Now ();
}

uint32_t
RraaWifiManager::GetMaxRate (RraaWifiRemoteStation *station)
{
  return GetNSupported (station) - 1;
}
uint32_t
RraaWifiManager::GetMinRate (RraaWifiRemoteStation *station)
{
  return 0;
}


void
RraaWifiManager::DoReportRtsFailed (WifiRemoteStation *st)
{
}

void
RraaWifiManager::DoReportDataFailed (WifiRemoteStation *st)
{
  RraaWifiRemoteStation *station = (RraaWifiRemoteStation *) st;
  station->m_lastFrameFail = true;
  CheckTimeout (station);
  station->m_counter--;
  station->m_failed++;
  RunBasicAlgorithm (station);
}
void
RraaWifiManager::DoReportRxOk (WifiRemoteStation *st,
                               double rxSnr, WifiMode txMode)
{
}
void
RraaWifiManager::DoReportRtsOk (WifiRemoteStation *st,
                                double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
  NS_LOG_DEBUG ("self=" << st << " rts ok");
}
void
RraaWifiManager::DoReportDataOk (WifiRemoteStation *st,
                                 double ackSnr, WifiMode ackMode, double dataSnr)
{
  RraaWifiRemoteStation *station = (RraaWifiRemoteStation *) st;
  station->m_lastFrameFail = false;
  CheckTimeout (station);
  station->m_counter--;
  RunBasicAlgorithm (station);
}
void
RraaWifiManager::DoReportFinalRtsFailed (WifiRemoteStation *st)
{
}
void
RraaWifiManager::DoReportFinalDataFailed (WifiRemoteStation *st)
{
}

WifiTxVector
RraaWifiManager::DoGetDataTxVector (WifiRemoteStation *st,
                                uint32_t size)
{
  RraaWifiRemoteStation *station = (RraaWifiRemoteStation *) st;
  if (!station->m_initialized)
    {
      ResetCountersBasic (station);
    }
  return WifiTxVector (GetSupported (station, station->m_rate), GetDefaultTxPowerLevel (), GetLongRetryCount (station), GetShortGuardInterval (station), Min (GetNumberOfReceiveAntennas (station),GetNumberOfTransmitAntennas()), GetNess (station), GetStbc (station));
}
WifiTxVector
RraaWifiManager::DoGetRtsTxVector (WifiRemoteStation *st)
{
  return WifiTxVector (GetSupported (st, 0), GetDefaultTxPowerLevel (), GetShortRetryCount (st), GetShortGuardInterval (st), Min (GetNumberOfReceiveAntennas (st),GetNumberOfTransmitAntennas()), GetNess (st), GetStbc (st));
}

bool
RraaWifiManager::DoNeedRts (WifiRemoteStation *st,
                            Ptr<const Packet> packet, bool normally)
{
  RraaWifiRemoteStation *station = (RraaWifiRemoteStation *) st;
  if (m_basic)
    {
      return normally;
    }
  ARts (station);
  return station->m_rtsOn;
}

void
RraaWifiManager::CheckTimeout (RraaWifiRemoteStation *station)
{
  Time d = Simulator::Now () - station->m_lastReset;
  if (station->m_counter == 0 || d > m_timeout)
    {
      ResetCountersBasic (station);
    }
}

void
RraaWifiManager::RunBasicAlgorithm (RraaWifiRemoteStation *station)
{
  ThresholdsItem thresholds = GetThresholds (station, station->m_rate);
  double ploss = (double) station->m_failed / (double) thresholds.ewnd;
  if (station->m_counter == 0
      || ploss > thresholds.pmtl)
    {
      if (station->m_rate > GetMinRate (station)
          && ploss > thresholds.pmtl)
        {
          station->m_rate--;
        }
      else if (station->m_rate < GetMaxRate (station)
               && ploss < thresholds.pori)
        {
          station->m_rate++;
        }
      ResetCountersBasic (station);
    }
}

void
RraaWifiManager::ARts (RraaWifiRemoteStation *station)
{
  if (!station->m_rtsOn
      && station->m_lastFrameFail)
    {
      station->m_rtsWnd++;
      station->m_rtsCounter = station->m_rtsWnd;
    }
  else if ((station->m_rtsOn && station->m_lastFrameFail)
           || (!station->m_rtsOn && !station->m_lastFrameFail))
    {
      station->m_rtsWnd = station->m_rtsWnd / 2;
      station->m_rtsCounter = station->m_rtsWnd;
    }
  if (station->m_rtsCounter > 0)
    {
      station->m_rtsOn = true;
      station->m_rtsCounter--;
    }
  else
    {
      station->m_rtsOn = false;
    }
}

struct RraaWifiManager::ThresholdsItem
RraaWifiManager::GetThresholds (RraaWifiRemoteStation *station,
                                uint32_t rate) const
{
  WifiMode mode = GetSupported (station, rate);
  return GetThresholds (mode);
}

struct RraaWifiManager::ThresholdsItem
RraaWifiManager::GetThresholds (WifiMode mode) const
{
  switch (mode.GetDataRate () / 1000000)
    {
    case 54:
      {
        ThresholdsItem mode54 = {
          54000000,
          0.0,
          m_pmtlfor54,
          m_ewndfor54
        };
        return mode54;
      } break;
    case 48:
      {
        ThresholdsItem mode48 = {
          48000000,
          m_porifor48,
          m_pmtlfor48,
          m_ewndfor48
        };
        return mode48;
      } break;
    case 36:
      {
        ThresholdsItem mode36 = {
          36000000,
          m_porifor36,
          m_pmtlfor36,
          m_ewndfor36
        };
        return mode36;
      } break;
    case 24:
      {
        ThresholdsItem mode24 = {
          24000000,
          m_porifor24,
          m_pmtlfor24,
          m_ewndfor24
        };
        return mode24;
      } break;
    case 18:
      {
        ThresholdsItem mode18 = {
          18000000,
          m_porifor18,
          m_pmtlfor18,
          m_ewndfor18
        };
        return mode18;
      } break;
    case 12:
      {
        ThresholdsItem mode12 = {
          12000000,
          m_porifor12,
          m_pmtlfor12,
          m_ewndfor12
        };
        return mode12;
      } break;
    case 9:
      {
        ThresholdsItem mode9 =  {
          9000000,
          m_porifor9,
          m_pmtlfor9,
          m_ewndfor9
        };
        return mode9;
      } break;
    case 6:
      {
        ThresholdsItem mode6 =  {
          6000000,
          m_porifor6,
          1.0,
          m_ewndfor6
        };
        return mode6;
      } break;
    }
  NS_ASSERT_MSG (false, "Thresholds for an unknown mode are asked (" << mode << ")");
  return ThresholdsItem ();
}

bool
RraaWifiManager::IsLowLatency (void) const
{
  return true;
}

} // namespace ns3
