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

#include "rraa-mac-stations.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/default-value.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE ("Rraa");

namespace ns3 {

static BooleanDefaultValue g_basicRraa
("RRAA-BASIC",
 "If true the RRAA-BASIC algorithm will be used, otherwise the RRAA wil be used",
 false);

static NumericDefaultValue<double> g_rraaTimeout
("RraaTimeout",
 "Timeout for the RRAA BASIC loss estimaton block (s)",
 0.05);

static NumericDefaultValue<uint32_t> g_ewndfor54
("ewndFor54mbps",
 "ewnd parameter for 54 Mbs data mode",
 40);

static NumericDefaultValue<uint32_t> g_ewndfor48
("ewndFor48mbps",
 "ewnd parameter for 48 Mbs data mode",
 40);

static NumericDefaultValue<uint32_t> g_ewndfor36
("ewndFor36mbps",
 "ewnd parameter for 36 Mbs data mode",
 40);

static NumericDefaultValue<uint32_t> g_ewndfor24
("ewndFor24mbps",
 "ewnd parameter for 24 Mbs data mode",
 40);

static NumericDefaultValue<uint32_t> g_ewndfor18
("ewndFor18mbps",
 "ewnd parameter for 18 Mbs data mode",
 20);

static NumericDefaultValue<uint32_t> g_ewndfor12
("ewndFor12mbps",
 "ewnd parameter for 12 Mbs data mode",
 20);

static NumericDefaultValue<uint32_t> g_ewndfor9
("ewndFor9mbps",
 "ewnd parameter for 9 Mbs data mode",
 10);

static NumericDefaultValue<uint32_t> g_ewndfor6
("ewndFor6mbps",
 "ewnd parameter for 6 Mbs data mode",
 6);

static NumericDefaultValue<double> g_porifor48
("poriFor48mbps",
 "Pori parameter for 48 Mbs data mode",
 0.047);

static NumericDefaultValue<double> g_porifor36
("poriFor36mbps",
 "Pori parameter for 36 Mbs data mode",
 0.115);

static NumericDefaultValue<double> g_porifor24
("poriFor24mbps",
 "Pori parameter for 24 Mbs data mode",
 0.1681);

static NumericDefaultValue<double> g_porifor18
("poriFor18mbps",
 "Pori parameter for 18 Mbs data mode",
 0.1325);

static NumericDefaultValue<double> g_porifor12
("poriFor12mbps",
 "Pori parameter for 12 Mbs data mode",
 0.1861);

static NumericDefaultValue<double> g_porifor9
("poriFor9mbps",
 "Pori parameter for 9 Mbs data mode",
 0.1434);

static NumericDefaultValue<double> g_porifor6
("poriFor6mbps",
 "Pori parameter for 6 Mbs data mode",
 0.5);
 
static NumericDefaultValue<double> g_pmtlfor54
("pmtlFor54mbps",
 "Pmtl parameter for 54 Mbs data mode",
 0.094);
 
static NumericDefaultValue<double> g_pmtlfor48
("pmtlFor48mbps",
 "Pmtl parameter for 48 Mbs data mode",
 0.23);

static NumericDefaultValue<double> g_pmtlfor36
("pmtlFor36mbps",
 "Pmtl parameter for 36 Mbs data mode",
 0.3363);

static NumericDefaultValue<double> g_pmtlfor24
("pmtlFor24mbps",
 "Pmtl parameter for 24 Mbs data mode",
 0.265);

static NumericDefaultValue<double> g_pmtlfor18
("pmtlFor18mbps",
 "Pmtl parameter for 18 Mbs data mode",
 0.3722);

static NumericDefaultValue<double> g_pmtlfor12
("pmtlFor12mbps",
 "Pmtl parameter for 12 Mbs data mode",
 0.2868);

static NumericDefaultValue<double> g_pmtlfor9
("pmtlFor9mbps",
 "Pmtl parameter for 9 Mbs data mode",
 0.3932);

  
RraaMacStation::RraaMacStation (RraaMacStations *stations, Thresholds thresholds)
  : m_stations (stations)
{
  m_initialized = false;
  m_rtsWnd = 0;
  m_rtsCounter = 0;
  m_rtsOn = false;
  m_lastFrameFail = false;
}

RraaMacStation::~RraaMacStation ()
{}

void
RraaMacStation::ResetCountersBasic (void)
{
  if (!m_initialized) {
    m_rate = GetMaxRate ();
    m_initialized = true;
  }
  m_failed = 0;
  m_counter = GetThresholds (m_rate).ewnd;
  m_lastReset = Simulator::Now ();
}

RraaMacStations *
RraaMacStation::GetStations (void) const
{
  return m_stations;
}

uint32_t
RraaMacStation::GetMaxRate (void)
{
  return GetNSupportedModes () - 1;
}
uint32_t
RraaMacStation::GetMinRate (void)
{
  return 0;
}

ThresholdsItem
RraaMacStation::GetThresholds (uint32_t rate) 
{
  WifiMode mode = GetSupportedMode (rate);
  return m_stations->GetThresholds (mode);
}


void 
RraaMacStation::ReportRtsFailed (void)
{}

void 
RraaMacStation::ReportDataFailed (void)
{
  m_lastFrameFail = true;
  CheckTimeout ();
  m_counter--;
  m_failed++;
  RunBasicAlgorithm ();
}
void 
RraaMacStation::ReportRxOk (double rxSnr, WifiMode txMode)
{}
void 
RraaMacStation::ReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
  NS_LOG_DEBUG ("self="<<this<<" rts ok");
}
void 
RraaMacStation::ReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr)
{
  m_lastFrameFail = false;
  CheckTimeout ();
  m_counter--;
  RunBasicAlgorithm ();
}
void 
RraaMacStation::ReportFinalRtsFailed (void)
{}
void 
RraaMacStation::ReportFinalDataFailed (void)
{}

WifiMode
RraaMacStation::DoGetDataMode (uint32_t size)
{
  if (!m_initialized)
    ResetCountersBasic ();
  return GetSupportedMode (m_rate);
}
WifiMode
RraaMacStation::DoGetRtsMode (void)
{
  return GetSupportedMode (0);
}

bool
RraaMacStation::NeedRts (Ptr<const Packet> packet)
{
  if (m_stations->OnlyBasic ())
    return MacStation::NeedRts (packet);
  ARts ();
  return m_rtsOn;
}

void
RraaMacStation::CheckTimeout (void)
{
  Time d = Simulator::Now () - m_lastReset;
  if (m_counter == 0 || d.GetSeconds () > m_stations->GetTimeout ()) {
    ResetCountersBasic ();
  }
}

void
RraaMacStation::RunBasicAlgorithm (void)
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
RraaMacStation::ARts (void)
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




RraaMacStations::RraaMacStations (WifiMode defaultTxMode)
  : MacStations (defaultTxMode)
{
  DoConstruct ();
}
RraaMacStations::~RraaMacStations ()
{}

void
RraaMacStations::DoConstruct ()
{
  ThresholdsItem mode54 = {54000000, 
                          0.0, 
			  g_pmtlfor54.GetValue (), 
			  g_ewndfor54.GetValue ()};
  ThresholdsItem mode48 = {48000000, 
                          g_porifor48.GetValue (), 
			  g_pmtlfor48.GetValue (), 
			  g_ewndfor48.GetValue ()};
  ThresholdsItem mode36 = {36000000, 
                          g_porifor36.GetValue (), 
			  g_pmtlfor36.GetValue (), 
			  g_ewndfor36.GetValue ()};
  ThresholdsItem mode24 = {24000000, 
                          g_porifor24.GetValue (), 
			  g_pmtlfor24.GetValue (), 
			  g_ewndfor24.GetValue ()};
  ThresholdsItem mode18 = {18000000, 
                          g_porifor18.GetValue (), 
			  g_pmtlfor18.GetValue (), 
			  g_ewndfor18.GetValue ()};
  ThresholdsItem mode12 = {12000000, 
                          g_porifor12.GetValue (), 
			  g_pmtlfor12.GetValue (), 
			  g_ewndfor12.GetValue ()};
  ThresholdsItem mode9 =  {9000000, 
                          g_porifor9.GetValue (), 
			  g_pmtlfor9.GetValue (), 
			  g_ewndfor9.GetValue ()};
  ThresholdsItem mode6 =  {6000000, 
                          g_porifor6.GetValue (), 
			  1.0, 
			  g_ewndfor6.GetValue ()};
  m_thresholds.push_back (mode54);
  m_thresholds.push_back (mode48);
  m_thresholds.push_back (mode36);
  m_thresholds.push_back (mode24);
  m_thresholds.push_back (mode18);
  m_thresholds.push_back (mode12);
  m_thresholds.push_back (mode9);
  m_thresholds.push_back (mode6);
  m_basic = g_basicRraa.GetValue ();
  m_timeout = g_rraaTimeout.GetValue ();
}

MacStation *
RraaMacStations::CreateStation (void)
{
  return new RraaMacStation (this, m_thresholds);
}

bool
RraaMacStations::OnlyBasic (void)
{
  return m_basic;
}

double
RraaMacStations::GetTimeout (void)
{
  return m_timeout;
}

ThresholdsItem
RraaMacStations::GetThresholds (WifiMode mode) 
{
  switch (mode.GetDataRate () / 1000000) {
    case 54:
      return m_thresholds[0];
    case 48:
      return m_thresholds[1];
    case 36:
      return m_thresholds[2];
    case 24:
      return m_thresholds[3];
    case 18:
      return m_thresholds[4];
    case 12:
      return m_thresholds[5];
    case 9:
      return m_thresholds[6];
    case 6:
      return m_thresholds[7];
  }
  NS_ASSERT("Thresholds for an unknown mode are asked");
  return m_thresholds[0];
}

} // namespace ns3
