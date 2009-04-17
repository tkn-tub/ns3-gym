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

#include "cara-wifi-manager.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE ("Cara");


namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(CaraWifiManager);

CaraWifiRemoteStation::CaraWifiRemoteStation (Ptr<CaraWifiManager> manager)
  : m_manager (manager)
{
  m_rate = GetMinRate ();

  m_success = 0;
  m_failed = 0;
  m_timer = 0;
}
CaraWifiRemoteStation::~CaraWifiRemoteStation ()
{}

uint32_t
CaraWifiRemoteStation::GetMaxRate (void)
{
  return GetNSupportedModes () - 1;
}
uint32_t
CaraWifiRemoteStation::GetMinRate (void)
{
  return 0;
}

bool 
CaraWifiRemoteStation::NeedNormalFallback (void)
{
  return (m_failed >= m_manager->m_failureThreshold);
}

void 
CaraWifiRemoteStation::DoReportRtsFailed (void) 
{}

void 
CaraWifiRemoteStation::DoReportDataFailed (void)
{
  m_timer++;
  m_failed++;
  m_success = 0;
  if (NeedNormalFallback ()) 
    {
      if (m_rate != GetMinRate ())
        {
          m_rate--;
        }
      m_failed = 0;
      m_timer = 0;
    }
}
void 
CaraWifiRemoteStation::DoReportRxOk (double rxSnr, WifiMode txMode)
{}
void 
CaraWifiRemoteStation::DoReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr)
{
  NS_LOG_DEBUG ("self="<<this<<" rts ok");
}
void 
CaraWifiRemoteStation::DoReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr)
{
  m_timer++;
  m_success++;
  m_failed = 0;
  NS_LOG_DEBUG ("self="<<this<<" data ok success="<<m_success<<", timer="<<m_timer);
  if ((m_success == m_manager->m_successThreshold ||
       m_timer >= m_manager->m_timerTimeout))
    {
      NS_LOG_DEBUG ("self="<<this<<" inc rate");
      if (m_rate < GetMaxRate ())
        {
          m_rate++;
        }
      m_timer = 0;
      m_success = 0;
    }
}
void 
CaraWifiRemoteStation::DoReportFinalRtsFailed (void)
{}
void 
CaraWifiRemoteStation::DoReportFinalDataFailed (void)
{}

WifiMode
CaraWifiRemoteStation::DoGetDataMode (uint32_t size)
{
  return GetSupportedMode (m_rate);
}
WifiMode
CaraWifiRemoteStation::DoGetRtsMode (void)
{
  // XXX: we could/should implement the Arf algorithm for
  // RTS only by picking a single rate within the BasicRateSet.
  return GetSupportedMode (0);
}

Ptr<WifiRemoteStationManager>
CaraWifiRemoteStation::GetManager (void) const
{
  return m_manager;
}

bool
CaraWifiRemoteStation::DoNeedRts (Ptr<const Packet> packet)
{
  bool rts = WifiRemoteStation::NeedRts (packet);
  if (rts || m_failed >= m_manager->m_probeThreshold)
    {
      return true;
    }

  return false;
}



TypeId 
CaraWifiManager::GetTypeId (void)
{
 static TypeId tid = TypeId ("ns3::CaraWifiManager")
   .SetParent<WifiRemoteStationManager> ()
   .AddConstructor<CaraWifiManager> ()
   .AddAttribute ("ProbeThreshold",
                  "The number of consecutive transmissions failure to activate the RTS probe.",
                  UintegerValue (1),
                  MakeUintegerAccessor (&CaraWifiManager::m_probeThreshold),
                  MakeUintegerChecker<uint32_t> ())
   .AddAttribute ("FailureThreshold",
                  "The number of consecutive transmissions failure to decrease the rate.",
                  UintegerValue (2),
                  MakeUintegerAccessor (&CaraWifiManager::m_failureThreshold),
                  MakeUintegerChecker<uint32_t> ())
   .AddAttribute ("SuccessThreshold",
                  "The minimum number of sucessfull transmissions to try a new rate.",
                  UintegerValue (10),
                  MakeUintegerAccessor (&CaraWifiManager::m_failureThreshold),
                  MakeUintegerChecker<uint32_t> ())
   .AddAttribute ("Timeout",
                  "The 'timer' in the CARA algorithm",
                  UintegerValue (15),
                  MakeUintegerAccessor (&CaraWifiManager::m_timerTimeout),
                  MakeUintegerChecker<uint32_t> ())
 ;
 return tid;
}

CaraWifiManager::CaraWifiManager ()
  : WifiRemoteStationManager ()
{}
CaraWifiManager::~CaraWifiManager ()
{}

WifiRemoteStation *
CaraWifiManager::CreateStation (void)
{
  return new CaraWifiRemoteStation (this);
}

} // namespace ns3
