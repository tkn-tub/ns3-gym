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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "aarf-wifi-manager.h"

#include "ns3/double.h"
#include "ns3/uinteger.h"

#define Min(a,b) ((a<b)?a:b)
#define Max(a,b) ((a>b)?a:b)

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (AarfWifiManager);

TypeId 
AarfWifiManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::AarfWifiManager")
    .SetParent<ArfWifiManager> ()
    .AddConstructor<AarfWifiManager> ()
    .AddAttribute ("SuccessK", "Multiplication factor for the success threshold in the AARF algorithm.",
                   DoubleValue (2.0),
                   MakeDoubleAccessor (&AarfWifiManager::m_successK),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("TimerK",
                   "Multiplication factor for the timer threshold in the AARF algorithm.",
                   DoubleValue (2.0),
                   MakeDoubleAccessor (&AarfWifiManager::m_timerK),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxSuccessThreshold",
                   "Maximum value of the success threshold in the AARF algorithm.",
                   UintegerValue (60),
                   MakeUintegerAccessor (&AarfWifiManager::m_maxSuccessThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MinTimerThreshold",
                   "The minimum value for the 'timer' threshold in the AARF algorithm.",
                   UintegerValue (15),
                   MakeUintegerAccessor (&AarfWifiManager::m_minTimerThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MinSuccessThreshold",
                   "The minimum value for the success threshold in the AARF algorithm.",
                   UintegerValue (10),
                   MakeUintegerAccessor (&AarfWifiManager::m_minSuccessThreshold),
                   MakeUintegerChecker<uint32_t> ())
    ;
  return tid;
}

AarfWifiManager::AarfWifiManager ()
{}
AarfWifiManager::~AarfWifiManager ()
{}
WifiRemoteStation *
AarfWifiManager::CreateStation (void)
{
  return new AarfWifiRemoteStation (this);
}




AarfWifiRemoteStation::AarfWifiRemoteStation (Ptr<AarfWifiManager> manager)
  : ArfWifiRemoteStation (manager),
    m_manager (manager)
{}


AarfWifiRemoteStation::~AarfWifiRemoteStation ()
{}

Ptr<WifiRemoteStationManager> 
AarfWifiRemoteStation::GetManager (void) const
{
  return m_manager;
}

void 
AarfWifiRemoteStation::ReportRecoveryFailure (void)
{
  SetSuccessThreshold ((int)(Min (GetSuccessThreshold () * m_manager->m_successK,
                                  m_manager->m_maxSuccessThreshold)));
  SetTimerTimeout ((int)(Max (GetMinTimerTimeout (),
                              GetSuccessThreshold () * m_manager->m_timerK)));
}

void 
AarfWifiRemoteStation::ReportFailure (void)
{
  SetTimerTimeout (GetMinTimerTimeout ());
  SetSuccessThreshold (GetMinSuccessThreshold ());
}

} // namespace ns3
