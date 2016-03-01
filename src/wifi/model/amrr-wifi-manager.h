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
  //overriden from base class
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
  virtual WifiTxVector DoGetDataTxVector (WifiRemoteStation *station);
  virtual WifiTxVector DoGetRtsTxVector (WifiRemoteStation *station);
  virtual bool IsLowLatency (void) const;

  //void UpdateRetry (AmrrWifiRemoteStation *station);
  /**
   * Update the mode used to send to the given station.
   *
   * \param station
   */
  void UpdateMode (AmrrWifiRemoteStation *station);
  /**
   * Reset transmission statistics of the given station.
   *
   * \param station
   */
  void ResetCnt (AmrrWifiRemoteStation *station);
  /**
   * Increase the transmission rate to the given station.
   *
   * \param station
   */
  void IncreaseRate (AmrrWifiRemoteStation *station);
  /**
   * Decrease the transmission rate to the given station.
   *
   * \param station
   */
  void DecreaseRate (AmrrWifiRemoteStation *station);
  /**
   * Check if the current rate for the given station is the
   * minimum rate.
   *
   * \param station
   *
   * \return true if the current rate is the minimum rate,
   *         false otherwise
   */
  bool IsMinRate (AmrrWifiRemoteStation *station) const;
  /**
   * Check if the current rate for the given station is the
   * maximum rate.
   *
   * \param station
   *
   * \return true if the current rate is the maximum rate,
   *         false otherwise
   */
  bool IsMaxRate (AmrrWifiRemoteStation *station) const;
  /**
   * Check if the number of retransmission and transmission error
   * is less than the number of successful transmission (times ratio).
   *
   * \param station
   *
   * \return true if the number of retransmission and transmission error
   *              is less than the number of successful transmission
   *              (times ratio), false otherwise
   */
  bool IsSuccess (AmrrWifiRemoteStation *station) const;
  /**
   * Check if the number of retransmission and transmission error
   * is greater than the number of successful transmission (times ratio).
   *
   * \param station
   *
   * \return true if the number of retransmission and transmission error
   *              is less than the number of successful transmission
   *              (times ratio), false otherwise
   */
  bool IsFailure (AmrrWifiRemoteStation *station) const;
  /**
   * Check if the number of retransmission, transmission error,
   * and successful transmission are greater than 10.
   *
   * \param station
   * \return true if the number of retransmission, transmission error,
   *         and successful transmission are greater than 10,
   *         false otherwise
   */
  bool IsEnough (AmrrWifiRemoteStation *station) const;

  Time m_updatePeriod;
  double m_failureRatio;
  double m_successRatio;
  uint32_t m_maxSuccessThreshold;
  uint32_t m_minSuccessThreshold;
};

} //namespace ns3

#endif /* AMRR_WIFI_MANAGER_H */
