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

ConstantRateWifiRemoteStation::ConstantRateWifiRemoteStation (Ptr<ConstantRateWifiManager> manager)
  : m_manager (manager)
{}
ConstantRateWifiRemoteStation::~ConstantRateWifiRemoteStation ()
{}

void 
ConstantRateWifiRemoteStation::ReportRxOk (double rxSnr, WifiMode txMode)
{}
void 
ConstantRateWifiRemoteStation::ReportRtsFailed (void)
{}
void 
ConstantRateWifiRemoteStation::ReportDataFailed (void)
{}
void 
ConstantRateWifiRemoteStation::ReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr)
{}
void 
ConstantRateWifiRemoteStation::ReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr)
{}
void 
ConstantRateWifiRemoteStation::ReportFinalRtsFailed (void)
{}
void 
ConstantRateWifiRemoteStation::ReportFinalDataFailed (void)
{}

WifiMode 
ConstantRateWifiRemoteStation::DoGetDataMode (uint32_t size)
{
  return m_manager->GetDataMode ();
}
WifiMode 
ConstantRateWifiRemoteStation::DoGetRtsMode (void)
{
  return m_manager->GetCtlMode ();
}
Ptr<WifiRemoteStationManager>
ConstantRateWifiRemoteStation::GetManager (void) const
{
  return m_manager;
}

NS_OBJECT_ENSURE_REGISTERED (ConstantRateWifiManager);

TypeId 
ConstantRateWifiManager::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ConstantRateWifiManager")
    .SetParent<WifiRemoteStationManager> ()
    .AddConstructor<ConstantRateWifiManager> ()
    .AddAttribute ("DataMode", "XXX",
                   String ("wifia-6mbs"),
                   MakeWifiModeAccessor (&ConstantRateWifiManager::m_dataMode),
                   MakeWifiModeChecker ())
    .AddAttribute ("ControlMode", "XXX",
                   String ("wifia-6mbs"),
                   MakeWifiModeAccessor (&ConstantRateWifiManager::m_ctlMode),
                   MakeWifiModeChecker ())
    ;
  return tid;
}

ConstantRateWifiManager::ConstantRateWifiManager ()
{}
ConstantRateWifiManager::~ConstantRateWifiManager ()
{}

WifiMode 
ConstantRateWifiManager::GetDataMode (void) const
{
  return m_dataMode;
}
WifiMode 
ConstantRateWifiManager::GetCtlMode (void) const
{
  return m_ctlMode;
}


WifiRemoteStation *
ConstantRateWifiManager::CreateStation (void)
{
  return new ConstantRateWifiRemoteStation (this);
}

} // namespace ns3
