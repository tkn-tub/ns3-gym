/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2003,2007 INRIA
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
#ifndef ONOE_MAC_STATIONS_H
#define ONOE_MAC_STATIONS_H

#include "mac-stations.h"
#include "ns3/nstime.h"

namespace ns3 {

class OnoeMacStations : public MacStations
{
public:
  OnoeMacStations (WifiMode defaultTxMode);

private:
  friend class OnoeMacStation;
  virtual MacStation *CreateStation (void);

  Time m_updatePeriod;
  uint32_t m_addCreditThreshold;
  uint32_t m_raiseThreshold;
};

/**
 * \brief an implementation of rate control algorithm developed 
 *        by Atsushi Onoe
 *
 * This algorithm is well known because it has been used as the default
 * rate control algorithm for the madwifi driver. I am not aware of
 * any publication or reference about this algorithm beyond the madwifi
 * source code.
 */
class OnoeMacStation : public MacStation
{
public:
  OnoeMacStation (OnoeMacStations *stations);

  virtual ~OnoeMacStation ();

  virtual void ReportRxOk (double rxSnr, WifiMode txMode);
  virtual void ReportRtsFailed (void);
  virtual void ReportDataFailed (void);
  virtual void ReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr);
  virtual void ReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr);
  virtual void ReportFinalRtsFailed (void);
  virtual void ReportFinalDataFailed (void);

private:
  virtual OnoeMacStations *GetStations (void) const;
  virtual WifiMode DoGetDataMode (uint32_t size);
  virtual WifiMode DoGetRtsMode (void);

  void UpdateRetry (void);
  void UpdateMode (void);

  OnoeMacStations *m_stations;
  uint32_t m_shortRetry;
  uint32_t m_longRetry;
  Time m_nextModeUpdate;
  uint32_t m_tx_ok;
  uint32_t m_tx_err;
  uint32_t m_tx_retr;
};

} // namespace ns3

#endif /* ONOE_MAC_STATIONS_H */
