/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#ifndef AMRR_WIFI_MANAGER_H
#define AMRR_WIFI_MANAGER_H

#include "wifi-remote-station-manager.h"
#include "ns3/nstime.h"

namespace ns3 {

struct AmrrWifiRemoteStation;

/**
 * \brief AMRR Rate control algorithm
 * \ingroup wifi
 *
 * This class implements the AMRR rate control algorithm which
 * was initially described in <i>IEEE 802.11 Rate Adaptation:
 * A Practical Approach</i>, by M. Lacage, M.H. Manshaei, and
 * T. Turletti.
 */
class AmrrWifiManager : public WifiRemoteStationManager
{
public:
  static TypeId GetTypeId (void);

  AmrrWifiManager ();

private:
  // overriden from base class
  virtual WifiRemoteStation * DoCreateStation (void) const;
  virtual void DoReportRxOk (WifiRemoteStation *station,
                             double rxSnr, WifiMode txMode);
  virtual void DoReportRtsFailed (WifiRemoteStation *station);
  virtual void DoReportDataFailed (WifiRemoteStation *station);
  virtual void DoReportRtsOk (WifiRemoteStation *station,
                              double ctsSnr, WifiMode ctsMode, double rtsSnr);
  virtual void DoReportDataOk (WifiRemoteStation *station,
                               double ackSnr, WifiMode ackMode, double dataSnr);
  virtual void DoReportFinalRtsFailed (WifiRemoteStation *station);
  virtual void DoReportFinalDataFailed (WifiRemoteStation *station);
  virtual WifiMode DoGetDataMode (WifiRemoteStation *station, uint32_t size);
  virtual WifiMode DoGetRtsMode (WifiRemoteStation *station);
  virtual bool IsLowLatency (void) const;

  void UpdateRetry (AmrrWifiRemoteStation *station);
  void UpdateMode (AmrrWifiRemoteStation *station);
  void ResetCnt (AmrrWifiRemoteStation *station);
  void IncreaseRate (AmrrWifiRemoteStation *station);
  void DecreaseRate (AmrrWifiRemoteStation *station);
  bool IsMinRate (AmrrWifiRemoteStation *station) const;
  bool IsMaxRate (AmrrWifiRemoteStation *station) const;
  bool IsSuccess (AmrrWifiRemoteStation *station) const;
  bool IsFailure (AmrrWifiRemoteStation *station) const;
  bool IsEnough (AmrrWifiRemoteStation *station) const;

  Time m_updatePeriod;
  double m_failureRatio;
  double m_successRatio;
  uint32_t m_maxSuccessThreshold;
  uint32_t m_minSuccessThreshold;
};

} // namespace ns3

#endif /* AMRR_WIFI_MANAGER_H */
