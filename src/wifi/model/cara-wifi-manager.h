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

#ifndef CARA_WIFI_MANAGER_H
#define CARA_WIFI_MANAGER_H

#include "ns3/traced-value.h"
#include "wifi-remote-station-manager.h"

namespace ns3 {

/**
 * \brief implement the CARA rate control algorithm
 * \ingroup wifi
 *
 * Implement the CARA algorithm from:
 * J. Kim, S. Kim, S. Choi, and D. Qiao.
 * "CARA: Collision-Aware Rate Adaptation for IEEE 802.11 WLANs."
 *
 * Originally implemented by Federico Maguolo for a very early
 * prototype version of ns-3.
 *
 * This RAA does not support HT, VHT nor HE modes and will error
 * exit if the user tries to configure this RAA with a Wi-Fi MAC
 * that has VhtSupported, HtSupported or HeSupported set.
 */
class CaraWifiManager : public WifiRemoteStationManager
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  CaraWifiManager ();
  virtual ~CaraWifiManager ();

  // Inherited from WifiRemoteStationManager
  void SetHtSupported (bool enable);
  void SetVhtSupported (bool enable);
  void SetHeSupported (bool enable);


private:
  //overriden from base class
  WifiRemoteStation * DoCreateStation (void) const;
  void DoReportRxOk (WifiRemoteStation *station,
                     double rxSnr, WifiMode txMode);
  void DoReportRtsFailed (WifiRemoteStation *station);
  void DoReportDataFailed (WifiRemoteStation *station);
  void DoReportRtsOk (WifiRemoteStation *station,
                      double ctsSnr, WifiMode ctsMode, double rtsSnr);
  void DoReportDataOk (WifiRemoteStation *station,
                       double ackSnr, WifiMode ackMode, double dataSnr);
  void DoReportFinalRtsFailed (WifiRemoteStation *station);
  void DoReportFinalDataFailed (WifiRemoteStation *station);
  WifiTxVector DoGetDataTxVector (WifiRemoteStation *station);
  WifiTxVector DoGetRtsTxVector (WifiRemoteStation *station);
  bool DoNeedRts (WifiRemoteStation *station,
                  Ptr<const Packet> packet, bool normally);
  bool IsLowLatency (void) const;

  uint32_t m_timerTimeout; ///< timer threshold
  uint32_t m_successThreshold; ///< success threshold
  uint32_t m_failureThreshold; ///< failure threshold
  uint32_t m_probeThreshold; ///< probe threshold

  TracedValue<uint64_t> m_currentRate; //!< Trace rate changes
};

} //namespace ns3

#endif /* CARA_WIFI_MANAGER_H */
