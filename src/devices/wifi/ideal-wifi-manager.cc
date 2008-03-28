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
#include "ideal-wifi-manager.h"
#include "wifi-phy.h"
#include "ns3/assert.h"
#include "ns3/double.h"
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

NS_OBJECT_ENSURE_REGISTERED (IdealWifiManager);

TypeId 
IdealWifiManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::IdealWifiManager")
    .SetParent<WifiRemoteStationManager> ()
    .AddConstructor<IdealWifiManager> ()
    .AddAttribute ("BerThreshold",
                   "The maximum Bit Error Rate acceptable at any transmission mode",
                   Double (10e-6),
                   MakeDoubleAccessor (&IdealWifiManager::m_ber),
                   MakeDoubleChecker<double> ())
    ;
  return tid;
}

IdealWifiManager::IdealWifiManager ()
{}
IdealWifiManager::~IdealWifiManager ()
{}

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

WifiRemoteStation *
IdealWifiManager::CreateStation (void)
{
  return new IdealWifiRemoteStation (this);
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

IdealWifiRemoteStation::IdealWifiRemoteStation (Ptr<IdealWifiManager> manager)
  : m_manager (manager),
    m_lastSnr (0.0)
{}
IdealWifiRemoteStation::~IdealWifiRemoteStation ()
{}
void 
IdealWifiRemoteStation::DoReportRxOk (double rxSnr, WifiMode txMode)
{}
void 
IdealWifiRemoteStation::DoReportRtsFailed (void)
{}
void 
IdealWifiRemoteStation::DoReportDataFailed (void)
{}
void 
IdealWifiRemoteStation::DoReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
  TRACE ("got cts for rts snr="<<rtsSnr);
  m_lastSnr = rtsSnr;
}
void 
IdealWifiRemoteStation::DoReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr)
{
  TRACE ("got cts for rts snr="<<dataSnr);
  m_lastSnr = dataSnr;
}
void 
IdealWifiRemoteStation::DoReportFinalRtsFailed (void)
{}
void 
IdealWifiRemoteStation::DoReportFinalDataFailed (void)
{}

WifiMode
IdealWifiRemoteStation::DoGetDataMode (uint32_t size)
{
  // We search within the Supported rate set the mode with the 
  // highest snr threshold possible which is smaller than m_lastSnr 
  // to ensure correct packet delivery.
  double maxThreshold = 0.0;
  WifiMode maxMode = m_manager->GetDefaultMode ();
  for (uint32_t i = 0; i < GetNSupportedModes (); i++)
    {
      WifiMode mode = GetSupportedMode (i);
      double threshold = m_manager->GetSnrThreshold (mode);
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
IdealWifiRemoteStation::DoGetRtsMode (void)
{
  // We search within the Basic rate set the mode with the highest 
  // snr threshold possible which is smaller than m_lastSnr to 
  // ensure correct packet delivery.
  double maxThreshold = 0.0;
  WifiMode maxMode = m_manager->GetDefaultMode ();
  for (uint32_t i = 0; i < m_manager->GetNBasicModes (); i++)
    {
      WifiMode mode = m_manager->GetBasicMode (i);
      double threshold = m_manager->GetSnrThreshold (mode);
      if (threshold > maxThreshold && 
          threshold < m_lastSnr)
        {
          maxThreshold = threshold;
          maxMode = mode;
        }
    }
  return maxMode;
}
Ptr<WifiRemoteStationManager>
IdealWifiRemoteStation::GetManager (void) const
{
  return m_manager;
}

} // namespace ns3
