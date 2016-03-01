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

#ifndef ONOE_WIFI_MANAGER_H
#define ONOE_WIFI_MANAGER_H

#include "wifi-remote-station-manager.h"
#include "ns3/nstime.h"

namespace ns3 {

struct OnoeWifiRemoteStation;

/**
 * \brief an implementation of the rate control algorithm developed
 *        by Atsushi Onoe
 *
 * \ingroup wifi
 *
 * This algorithm is well known because it has been used as the default
 * rate control algorithm for the madwifi driver. I am not aware of
 * any publication or reference about this algorithm beyond the madwifi
 * source code.
 */
class OnoeWifiManager : public WifiRemoteStationManager
{
public:
  static TypeId GetTypeId (void);

  OnoeWifiManager ();


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

  /**
   * Update the number of retry (both short and long).
   *
   * \param station
   */
  void UpdateRetry (OnoeWifiRemoteStation *station);
  void UpdateMode (OnoeWifiRemoteStation *station);

  Time m_updatePeriod;
  uint32_t m_addCreditThreshold;
  uint32_t m_raiseThreshold;
};

} //namespace ns3

#endif /* ONOE_WIFI_MANAGER_H */
