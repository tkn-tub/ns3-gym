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

#include "aarf-mac-stations.h"

#define Min(a,b) ((a<b)?a:b)
#define Max(a,b) ((a>b)?a:b)

namespace ns3 {

AarfMacStations::AarfMacStations (WifiMode defaultTxMode)
  : ArfMacStations (defaultTxMode)
{}
AarfMacStations::~AarfMacStations ()
{}
MacStation *
AarfMacStations::CreateStation (WifiMode defaultTxMode)
{
  return new AarfMacStation (defaultTxMode, 2.0, 60, 2.0);
}




AarfMacStation::AarfMacStation (WifiMode defaultTxMode,
                                double successK,
                                int maxSuccessThreshold,
                                double timerK)
  : ArfMacStation (defaultTxMode, 15, 10),
    m_successK (successK),
    m_maxSuccessThreshold (maxSuccessThreshold),
    m_timerK (timerK)
{}


AarfMacStation::~AarfMacStation ()
{}

void 
AarfMacStation::ReportRecoveryFailure (void)
{
  SetSuccessThreshold ((int)(Min (GetSuccessThreshold () * m_successK,
                                  m_maxSuccessThreshold)));
  SetTimerTimeout ((int)(Max (GetMinTimerTimeout (),
                              GetSuccessThreshold () * m_timerK)));
}

void 
AarfMacStation::ReportFailure (void)
{
  SetTimerTimeout (GetMinTimerTimeout ());
  SetSuccessThreshold (GetMinSuccessThreshold ());
}

} // namespace ns3
