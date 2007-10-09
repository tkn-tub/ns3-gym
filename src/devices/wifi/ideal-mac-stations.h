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
#ifndef IDEAL_MAC_STATIONS_H
#define IDEAL_MAC_STATIONS_H

#include <stdint.h>
#include <vector>
#include "mac-stations.h"
#include "wifi-mode.h"

namespace ns3 {

class WifiPhy;

class IdealMacStations : public MacStations {
public:
  IdealMacStations (WifiMode defaultTxMode);
  virtual ~IdealMacStations ();
  WifiMode GetMode (double snr) const;
  // return the min snr needed to successfully transmit
  // data with this mode at the specified ber.
  double GetSnrThreshold (WifiMode mode) const;
  void InitializeThresholds (const WifiPhy *phy, double ber);
private:
  virtual class MacStation *CreateStation (void);

  typedef std::vector<std::pair<double,WifiMode> > Thresholds;

  Thresholds m_thresholds;
  double m_minSnr;
  double m_maxSnr;
};

class IdealMacStation : public MacStation 
{
public:
  IdealMacStation (IdealMacStations *stations);

  virtual ~IdealMacStation ();

  virtual void ReportRxOk (double rxSnr, WifiMode txMode);
  virtual void ReportRtsFailed (void);
  virtual void ReportDataFailed (void);
  virtual void ReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr);
  virtual void ReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr);
  virtual WifiMode GetDataMode (uint32_t size);
  virtual WifiMode GetRtsMode (void);

private:
  virtual IdealMacStations *GetStations (void) const;
  IdealMacStations *m_stations;
  double m_lastSnr;
};

}; // namespace ns3

#endif /* MAC_STA_H */
