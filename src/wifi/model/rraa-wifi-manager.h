/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
 * Author: Federico Maguolo <maguolof@dei.unipd.it>
 */

#ifndef RRAA_WIFI_MANAGER_H
#define RRAA_WIFI_MANAGER_H

#include "ns3/nstime.h"
#include "wifi-remote-station-manager.h"

namespace ns3 {

struct RraaWifiRemoteStation;

/**
 * \brief Robust Rate Adaptation Algorithm
 * \ingroup wifi
 *
 * This is an implementation of RRAA as described in
 * "Robust rate adaptation for 802.11 wireless networks"
 * by "Starsky H. Y. Wong", "Hao Yang", "Songwu Lu", and,
 * "Vaduvur Bharghavan" published in Mobicom 06.
 */
class RraaWifiManager : public WifiRemoteStationManager
{
public:
  static TypeId GetTypeId (void);

  RraaWifiManager ();
  virtual ~RraaWifiManager ();


private:
  struct ThresholdsItem
  {
    uint32_t datarate;
    double pori;
    double pmtl;
    uint32_t ewnd;
  };

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
  virtual WifiTxVector DoGetDataTxVector (WifiRemoteStation *station, uint32_t size);
  virtual WifiTxVector DoGetRtsTxVector (WifiRemoteStation *station);
  virtual bool DoNeedRts (WifiRemoteStation *st,
                          Ptr<const Packet> packet, bool normally);
  virtual bool IsLowLatency (void) const;

  /**
   * Return the index for the maximum transmission rate for
   * the given station.
   *
   * \param station
   *
   * \return the index for the maximum transmission rate
   */
  uint32_t GetMaxRate (RraaWifiRemoteStation *station);
  /**
   * Return the index for the minimum transmission rate for
   * the given station.
   *
   * \param station
   *
   * \return the index for the minimum transmission rate
   */
  uint32_t GetMinRate (RraaWifiRemoteStation *station);
  /**
   * Check if the counter should be resetted.
   *
   * \param station
   */
  void CheckTimeout (RraaWifiRemoteStation *station);
  /**
   * Find an appropriate rate for the given station, using
   * a basic algorithm.
   *
   * \param station
   */
  void RunBasicAlgorithm (RraaWifiRemoteStation *station);
  /**
   * Activate the use of RTS for the given station if the conditions are met.
   *
   * \param station
   */
  void ARts (RraaWifiRemoteStation *station);
  /**
   * Reset the counters of the given station.
   *
   * \param station
   */
  void ResetCountersBasic (RraaWifiRemoteStation *station);
  /**
   * Get a threshold for the given mode.
   *
   * \param mode
   *
   * \return threshold
   */
  struct ThresholdsItem GetThresholds (WifiMode mode) const;
  /**
   * Get a threshold for the given station and mode index.
   *
   * \param station
   * \param rate
   *
   * \return threshold
   */
  struct ThresholdsItem GetThresholds (RraaWifiRemoteStation *station, uint32_t rate) const;

  bool m_basic;
  Time m_timeout;
  uint32_t m_ewndfor54;
  uint32_t m_ewndfor48;
  uint32_t m_ewndfor36;
  uint32_t m_ewndfor24;
  uint32_t m_ewndfor18;
  uint32_t m_ewndfor12;
  uint32_t m_ewndfor9;
  uint32_t m_ewndfor6;
  double m_porifor48;
  double m_porifor36;
  double m_porifor24;
  double m_porifor18;
  double m_porifor12;
  double m_porifor9;
  double m_porifor6;
  double m_pmtlfor54;
  double m_pmtlfor48;
  double m_pmtlfor36;
  double m_pmtlfor24;
  double m_pmtlfor18;
  double m_pmtlfor12;
  double m_pmtlfor9;
};

} //namespace ns3

#endif /* RRAA_WIFI_MANAGER_H */
