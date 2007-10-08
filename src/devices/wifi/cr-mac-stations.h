/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef CR_MAC_STATIONS_H
#define CR_MAC_STATIONS_H

#include <stdint.h>

#include "mac-stations.h"

namespace ns3 {

class CrMacStations : public MacStations {
public:
  CrMacStations (WifiMode dataMode, WifiMode ctlMode);
  virtual ~CrMacStations ();

  uint8_t GetDataMode (void) const;
  uint8_t GetCtlMode (void) const;
private:
  virtual class MacStation *CreateStation (WifiMode defaultMode);

  WifiMode m_dataMode;
  WifiMode m_ctlMode;
};


class CrMacStation : public MacStation
{
public:
  CrMacStation (WifiMode dataMode, WifiMode ctlMode);
  virtual ~CrMacStation ();

  virtual void ReportRxOk (double rxSnr, WifiMode txMode);
  virtual void ReportRtsFailed (void);
  virtual void ReportDataFailed (void);
  virtual void ReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr);
  virtual void ReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr);
  virtual WifiMode GetDataMode (uint32_t size);
  virtual WifiMode GetRtsMode (void);
private:
  WifiMode m_dataMode;
  WifiMode m_ctlMode;
};

} // namespace ns3



#endif /* CR_MAC_STATIONS_H */
