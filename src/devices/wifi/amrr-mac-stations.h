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
#ifndef AMRR_MAC_STATIONS_H
#define AMRR_MAC_STATIONS_H

#include "mac-stations.h"
#include "ns3/nstime.h"

namespace ns3 {

class AmrrMacStations : public MacStations
{
public:
  AmrrMacStations (WifiMode defaultTxMode);

private:
  friend class AmrrMacStation;
  virtual MacStation *CreateStation (void);

  Time m_updatePeriod;
  double m_failureRatio;
  double m_successRatio;
  uint32_t m_maxSuccessThreshold;
  uint32_t m_minSuccessThreshold;
};

/**
 */
class AmrrMacStation : public MacStation
{
public:
  AmrrMacStation (AmrrMacStations *stations);

  virtual ~AmrrMacStation ();

  virtual void ReportRxOk (double rxSnr, WifiMode txMode);
  virtual void ReportRtsFailed (void);
  virtual void ReportDataFailed (void);
  virtual void ReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr);
  virtual void ReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr);
  virtual void ReportFinalRtsFailed (void);
  virtual void ReportFinalDataFailed (void);

private:
  virtual AmrrMacStations *GetStations (void) const;
  virtual WifiMode DoGetDataMode (uint32_t size);
  virtual WifiMode DoGetRtsMode (void);

  void UpdateRetry (void);
  void UpdateMode (void);
  void ResetCnt (void);
  void IncreaseRate (void);
  void DecreaseRate (void);
  bool IsMinRate (void) const;
  bool IsMaxRate (void) const;
  bool IsSuccess (void) const;
  bool IsFailure (void) const;
  bool IsEnough (void) const;

  AmrrMacStations *m_stations;
  Time m_nextModeUpdate;
  uint32_t m_tx_ok;
  uint32_t m_tx_err;
  uint32_t m_tx_retr;
  uint32_t m_retry;
  uint32_t m_txrate;
  uint32_t m_successThreshold;
  uint32_t m_success;
  bool m_recovery;
};

} // namespace ns3

#endif /* AMRR_MAC_STATIONS_H */
