/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2004,2005 INRIA
 * All rights reserved.
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
 * In addition, as a special exception, the copyright holders of
 * this module give you permission to Combine (via static or
 * dynamic linking) this module with free software programs or
 * libraries that are released under the GNU LGPL and with code
 * included in the standard release of ns-2 under the Apache 2.0
 * license or under otherwise-compatible licenses with advertising
 * Requirements (or modified versions of such code, with unchanged
 * license).  You may copy and distribute such a system following the
 * terms of the GNU GPL for this module and the licenses of the
 * other code concerned, provided that you include the source code of
 * that other code when and as the GNU GPL requires distribution of
 * source code.
 *
 * Note that people who make modified versions of this module
 * are not obligated to grant this special exception for their
 * modified versions; it is their choice whether to do so.  The GNU
 * General Public License gives permission to release a modified
 * version without this exception; this exception also makes it
 * possible to release a modified version which carries forward this
 * exception.
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "mac-stations.h"
#include "mac-station.h"
#include "cr-mac-stations.h"

#include <cassert>

namespace ns3 {

CrMacStation::CrMacStation (CrMacStations *stations)
  : m_stations (stations)
{}
CrMacStation::~CrMacStation ()
{}

void 
CrMacStation::ReportRtsFailed (void)
{}
void 
CrMacStation::ReportDataFailed (void)
{}
void 
CrMacStation::ReportRxOk (double SNR, uint8_t mode)
{}
void CrMacStation::ReportRtsOk (double ctsSNR, uint8_t ctsMode, uint8_t rtsSnr)
{
  assert (rtsSnr == 0);
}
void CrMacStation::ReportDataOk (double ackSNR, uint8_t ackMode, uint8_t dataSnr)
{
  assert (dataSnr == 0);
}
uint8_t
CrMacStation::GetDataMode (int size)
{
  return m_stations->GetDataMode ();
}
uint8_t
CrMacStation::GetRtsMode (void)
{
  return m_stations->GetCtlMode ();
}
uint8_t 
CrMacStation::SnrToSnr (double snr)
{
  return 0;
}






CrMacStations::CrMacStations (uint8_t dataMode, uint8_t ctlMode)
  : MacStations (),
    m_dataMode (dataMode),
    m_ctlMode (ctlMode)
{}
CrMacStations::~CrMacStations ()
{}
uint8_t 
CrMacStations::GetDataMode (void) const
{
  return m_dataMode;
}
uint8_t 
CrMacStations::GetCtlMode (void) const
{
  return m_ctlMode;
}

MacStation *
CrMacStations::CreateStation (void)
{
  return new CrMacStation (this);
}

}; // namespace ns3
