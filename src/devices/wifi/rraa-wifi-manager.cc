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
 * Author: Federico Maguolo <maguolof@dei.unipd.it>
 */

#include "rraa-wifi-manager.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE ("RraaWifiManager");

namespace ns3 {
  
RraaWifiRemoteStation::RraaWifiRemoteStation (Ptr<RraaWifiManager> stations)
  : m_stations (stations)
{
  m_initialized = false;
  m_rtsWnd = 0;
  m_rtsCounter = 0;
  m_rtsOn = false;
  m_lastFrameFail = false;
}

RraaWifiRemoteStation::~RraaWifiRemoteStation ()
{}

void
RraaWifiRemoteStation::ResetCountersBasic (void)
{
  if (!m_initialized) {
    m_rate = GetMaxRate ();
    m_initialized = true;
  }
  m_failed = 0;
  m_counter = GetThresholds (m_rate).ewnd;
  m_lastReset = Simulator::Now ();
}

Ptr<WifiRemoteStationManager>
RraaWifiRemoteStation::GetManager (void) const
{
  return m_stations;
}

uint32_t
RraaWifiRemoteStation::GetMaxRate (void)
{
  return GetNSupportedModes () - 1;
}
uint32_t
RraaWifiRemoteStation::GetMinRate (void)
{
  return 0;
}

ThresholdsItem
RraaWifiRemoteStation::GetThresholds (uint32_t rate) 
{
  WifiMode mode = GetSupportedMode (rate);
  return m_stations->GetThresholds (mode);
}


void 
RraaWifiRemoteStation::DoReportRtsFailed (void)
{}

void 
RraaWifiRemoteStation::DoReportDataFailed (void)
{
  m_lastFrameFail = true;
  CheckTimeout ();
  m_counter--;
  m_failed++;
  RunBasicAlgorithm ();
}
void 
RraaWifiRemoteStation::DoReportRxOk (double rxSnr, WifiMode txMode)
{}
void 
RraaWifiRemoteStation::DoReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
  NS_LOG_DEBUG ("self="<<this<<" rts ok");
}
void 
RraaWifiRemoteStation::DoReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr)
{
  m_lastFrameFail = false;
  CheckTimeout ();
  m_counter--;
  RunBasicAlgorithm ();
}
void 
RraaWifiRemoteStation::DoReportFinalRtsFailed (void)
{}
void 
RraaWifiRemoteStation::DoReportFinalDataFailed (void)
{}

WifiMode
RraaWifiRemoteStation::DoGetDataMode (uint32_t size)
{
  if (!m_initialized)
    ResetCountersBasic ();
  return GetSupportedMode (m_rate);
}
WifiMode
RraaWifiRemoteStation::DoGetRtsMode (void)
{
  return GetSupportedMode (0);
}

bool
RraaWifiRemoteStation::NeedRts (Ptr<const Packet> packet)
{
  if (m_stations->OnlyBasic ())
    return WifiRemoteStation::NeedRts (packet);
  ARts ();
  return m_rtsOn;
}

void
RraaWifiRemoteStation::CheckTimeout (void)
{
  Time d = Simulator::Now () - m_lastReset;
  if (m_counter == 0 || d > m_stations->GetTimeout ()) {
    ResetCountersBasic ();
  }
}

void
RraaWifiRemoteStation::RunBasicAlgorithm (void)
{
  ThresholdsItem thresholds = GetThresholds (m_rate);
  double ploss = (double) m_failed / (double) thresholds.ewnd;
  if (m_counter == 0 || ploss > thresholds.pmtl) {
    if (m_rate > GetMinRate () && ploss > thresholds.pmtl) {
      m_rate--;
    }
    else if (m_rate < GetMaxRate () && ploss < thresholds.pori) {
      m_rate++;
    }
    ResetCountersBasic ();
  }
}

void
RraaWifiRemoteStation::ARts (void)
{
  if (!m_rtsOn && m_lastFrameFail) {
    m_rtsWnd++;
    m_rtsCounter = m_rtsWnd;
  }
  else if ((m_rtsOn && m_lastFrameFail) || 
           (!m_rtsOn && !m_lastFrameFail)) {
    m_rtsWnd = m_rtsWnd / 2;
    m_rtsCounter = m_rtsWnd;
  }
  if (m_rtsCounter > 0) {
    m_rtsOn = true;
    m_rtsCounter--;
  }
  else {
    m_rtsOn = false;
  }
}


TypeId 
RraaWifiManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("RraaWifiManager")
    .SetParent<WifiRemoteStationManager> ()
    .AddConstructor<RraaWifiManager> ()
    .AddAttribute ("Basic",
                   "If true the RRAA-BASIC algorithm will be used, otherwise the RRAA wil be used",
                   Boolean (false),
                   MakeBooleanAccessor (&RraaWifiManager::m_basic),
                   MakeBooleanChecker ())
    .AddAttribute ("Timeout",
                   "Timeout for the RRAA BASIC loss estimaton block (s)",
                   Seconds (0.05),
                   MakeTimeAccessor (&RraaWifiManager::m_timeout),
                   MakeTimeChecker ())
    .AddAttribute ("ewndFor54mbps",
                   "ewnd parameter for 54 Mbs data mode",
                   Uinteger (40),
                   MakeUintegerAccessor (&RraaWifiManager::m_ewndfor54),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ewndFor48mbps",
                   "ewnd parameter for 48 Mbs data mode",
                   Uinteger (40),
                   MakeUintegerAccessor (&RraaWifiManager::m_ewndfor48),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ewndFor36mbps",
                   "ewnd parameter for 36 Mbs data mode",
                   Uinteger (40),
                   MakeUintegerAccessor (&RraaWifiManager::m_ewndfor36),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ewndFor24mbps",
                   "ewnd parameter for 24 Mbs data mode",
                   Uinteger (40),
                   MakeUintegerAccessor (&RraaWifiManager::m_ewndfor24),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ewndFor18mbps",
                   "ewnd parameter for 18 Mbs data mode",
                   Uinteger (20),
                   MakeUintegerAccessor (&RraaWifiManager::m_ewndfor18),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ewndFor12mbps",
                   "ewnd parameter for 12 Mbs data mode",
                   Uinteger (20),
                   MakeUintegerAccessor (&RraaWifiManager::m_ewndfor12),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ewndFor9mbps",
                   "ewnd parameter for 9 Mbs data mode",
                   Uinteger (10),
                   MakeUintegerAccessor (&RraaWifiManager::m_ewndfor9),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ewndFor6mbps",
                   "ewnd parameter for 6 Mbs data mode",
                   Uinteger (6),
                   MakeUintegerAccessor (&RraaWifiManager::m_ewndfor6),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("poriFor48mbps",
                   "Pori parameter for 48 Mbs data mode",
                   Double (0.047),
                   MakeDoubleAccessor (&RraaWifiManager::m_porifor48),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("poriFor36mbps",
                   "Pori parameter for 36 Mbs data mode",
                   Double (0.115),
                   MakeDoubleAccessor (&RraaWifiManager::m_porifor36),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("poriFor24mbps",
                   "Pori parameter for 24 Mbs data mode",
                   Double (0.1681),
                   MakeDoubleAccessor (&RraaWifiManager::m_porifor24),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("poriFor18mbps",
                   "Pori parameter for 18 Mbs data mode",
                   Double (0.1325),
                   MakeDoubleAccessor (&RraaWifiManager::m_porifor18),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("poriFor12mbps",
                   "Pori parameter for 12 Mbs data mode",
                   Double (0.1861),
                   MakeDoubleAccessor (&RraaWifiManager::m_porifor12),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("poriFor9mbps",
                   "Pori parameter for 9 Mbs data mode",
                   Double (0.1434),
                   MakeDoubleAccessor (&RraaWifiManager::m_porifor9),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("poriFor6mbps",
                   "Pori parameter for 6 Mbs data mode",
                   Double (0.5),
                   MakeDoubleAccessor (&RraaWifiManager::m_porifor6),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("pmtlFor54mbps",
                   "Pmtl parameter for 54 Mbs data mode",
                   Double (0.094),
                   MakeDoubleAccessor (&RraaWifiManager::m_pmtlfor54),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("pmtlFor48mbps",
                   "Pmtl parameter for 48 Mbs data mode",
                   Double (0.23),
                   MakeDoubleAccessor (&RraaWifiManager::m_pmtlfor48),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("pmtlFor36mbps",
                   "Pmtl parameter for 36 Mbs data mode",
                   Double (0.3363),
                   MakeDoubleAccessor (&RraaWifiManager::m_pmtlfor36),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("pmtlFor24mbps",
                   "Pmtl parameter for 24 Mbs data mode",
                   Double (0.265),
                   MakeDoubleAccessor (&RraaWifiManager::m_pmtlfor24),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("pmtlFor18mbps",
                   "Pmtl parameter for 18 Mbs data mode",
                   Double (0.3722),
                   MakeDoubleAccessor (&RraaWifiManager::m_pmtlfor18),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("pmtlFor12mbps",
                   "Pmtl parameter for 12 Mbs data mode",
                   Double(0.2868),
                   MakeDoubleAccessor (&RraaWifiManager::m_pmtlfor12),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("pmtlFor9mbps",
                   "Pmtl parameter for 9 Mbs data mode",
                   Double (0.3932),
                   MakeDoubleAccessor (&RraaWifiManager::m_pmtlfor9),
                   MakeDoubleChecker<double> ())
    ;
  return tid;
}


RraaWifiManager::RraaWifiManager ()
{}
RraaWifiManager::~RraaWifiManager ()
{}

WifiRemoteStation *
RraaWifiManager::CreateStation (void)
{
  return new RraaWifiRemoteStation (this);
}

bool
RraaWifiManager::OnlyBasic (void)
{
  return m_basic;
}

Time
RraaWifiManager::GetTimeout (void) const
{
  return m_timeout;
}

ThresholdsItem
RraaWifiManager::GetThresholds (WifiMode mode) const
{
  switch (mode.GetDataRate () / 1000000) {
  case 54: {
    ThresholdsItem mode54 = {54000000, 
                             0.0, 
                             m_pmtlfor54, 
                             m_ewndfor54};
    return mode54;
  } break;
  case 48: {
    ThresholdsItem mode48 = {48000000, 
                             m_porifor48, 
                             m_pmtlfor48, 
                             m_ewndfor48};
    return mode48;
  } break;
  case 36: {
    ThresholdsItem mode36 = {36000000, 
                             m_porifor36, 
                             m_pmtlfor36, 
                             m_ewndfor36};
    return mode36;
  } break;
  case 24: {
    ThresholdsItem mode24 = {24000000, 
                             m_porifor24, 
                             m_pmtlfor24, 
                             m_ewndfor24};
    return mode24;
  } break;
  case 18: {
    ThresholdsItem mode18 = {18000000, 
                             m_porifor18, 
                             m_pmtlfor18, 
                             m_ewndfor18};
    return mode18;
  } break;
  case 12: {
    ThresholdsItem mode12 = {12000000, 
                             m_porifor12, 
                             m_pmtlfor12, 
                             m_ewndfor12};
    return mode12;
  } break;
  case 9: {
    ThresholdsItem mode9 =  {9000000, 
                             m_porifor9, 
                             m_pmtlfor9, 
                             m_ewndfor9};
    return mode9;
  } break;
  case 6: {
    ThresholdsItem mode6 =  {6000000, 
                             m_porifor6, 
                             1.0, 
                             m_ewndfor6};
    return mode6;
  } break;
  }
  NS_ASSERT("Thresholds for an unknown mode are asked");
  return ThresholdsItem ();
}

} // namespace ns3
