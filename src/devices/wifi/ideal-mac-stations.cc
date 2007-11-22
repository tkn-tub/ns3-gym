/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
#include "ideal-mac-stations.h"
#include "ns3/assert.h"
#include <math.h>

#define noIDEAL_DEBUG 1

#ifdef IDEAL_DEBUG
#include <iostream>
#  define TRACE(x) \
std::cout << "IDEAL TRACE " << x << std::endl;
#else
#  define TRACE(x)
#endif


namespace ns3 {

IdealMacStations::IdealMacStations (WifiMode defaultTxMode)
  : MacStations (defaultTxMode)
{}
IdealMacStations::~IdealMacStations ()
{}

MacStation *
IdealMacStations::CreateStation (void)
{
  return new IdealMacStation (this);
}

double 
IdealMacStations::GetSnrThreshold (WifiMode mode) const
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
IdealMacStations::AddModeSnrThreshold (WifiMode mode, double snr)
{
  m_thresholds.push_back (std::make_pair (snr,mode));
}

IdealMacStation::IdealMacStation (IdealMacStations *stations)
  : m_stations (stations),
    m_lastSnr (0.0)
{}
IdealMacStation::~IdealMacStation ()
{}
void 
IdealMacStation::ReportRxOk (double rxSnr, WifiMode txMode)
{}
void 
IdealMacStation::ReportRtsFailed (void)
{}
void 
IdealMacStation::ReportDataFailed (void)
{}
void 
IdealMacStation::ReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
  TRACE ("got cts for rts snr="<<rtsSnr);
  m_lastSnr = rtsSnr;
}
void 
IdealMacStation::ReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr)
{
  TRACE ("got cts for rts snr="<<dataSnr);
  m_lastSnr = dataSnr;
}
WifiMode
IdealMacStation::GetDataMode (uint32_t size)
{
  // We search within the Supported rate set the mode with the 
  // highest snr threshold possible which is smaller than m_lastSnr 
  // to ensure correct packet delivery.
  double maxThreshold = 0.0;
  WifiMode maxMode = m_stations->GetDefaultMode ();
  for (uint32_t i = 0; i < GetNSupportedModes (); i++)
    {
      WifiMode mode = GetSupportedMode (i);
      double threshold = m_stations->GetSnrThreshold (mode);
      if (threshold > maxThreshold && 
          threshold < m_lastSnr)
        {
          maxThreshold = threshold;
          maxMode = mode;
        }
    }
  return maxMode;
}
WifiMode
IdealMacStation::GetRtsMode (void)
{
  // We search within the Basic rate set the mode with the highest 
  // snr threshold possible which is smaller than m_lastSnr to 
  // ensure correct packet delivery.
  double maxThreshold = 0.0;
  WifiMode maxMode = m_stations->GetDefaultMode ();
  for (uint32_t i = 0; i < m_stations->GetNBasicModes (); i++)
    {
      WifiMode mode = m_stations->GetBasicMode (i);
      double threshold = m_stations->GetSnrThreshold (mode);
      if (threshold > maxThreshold && 
          threshold < m_lastSnr)
        {
          maxThreshold = threshold;
          maxMode = mode;
        }
    }
  return maxMode;
}
IdealMacStations *
IdealMacStation::GetStations (void) const
{
  return m_stations;
}

} // namespace ns3
