/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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

/**
 * \brief use constant rates for data and control transmissions
 *
 * This class uses always the same transmission rate for every
 * packet sent.
 */
class CrMacStations : public MacStations 
{
public:
  CrMacStations (WifiMode dataMode, WifiMode ctlMode);
  virtual ~CrMacStations ();

  WifiMode GetDataMode (void) const;
  WifiMode GetCtlMode (void) const;
private:
  virtual class MacStation *CreateStation (void);

  WifiMode m_dataMode;
  WifiMode m_ctlMode;
};


class CrMacStation : public MacStation
{
public:
  CrMacStation (CrMacStations *stations);
  virtual ~CrMacStation ();

  virtual void ReportRxOk (double rxSnr, WifiMode txMode);
  virtual void ReportRtsFailed (void);
  virtual void ReportDataFailed (void);
  virtual void ReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr);
  virtual void ReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr);
  virtual void ReportFinalRtsFailed (void);
  virtual void ReportFinalDataFailed (void);

private:
  virtual CrMacStations *GetStations (void) const;
  virtual WifiMode DoGetDataMode (uint32_t size);
  virtual WifiMode DoGetRtsMode (void);
  CrMacStations *m_stations;
};

} // namespace ns3



#endif /* CR_MAC_STATIONS_H */
