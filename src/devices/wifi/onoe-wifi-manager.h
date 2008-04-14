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
#ifndef ONOE_WIFI_MANAGER_H
#define ONOE_WIFI_MANAGER_H

#include "wifi-remote-station-manager.h"
#include "ns3/nstime.h"

namespace ns3 {

/**
 * \brief an implementation of rate control algorithm developed 
 *        by Atsushi Onoe
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
  friend class OnoeWifiRemoteStation;
  virtual WifiRemoteStation *CreateStation (void);

  Time m_updatePeriod;
  uint32_t m_addCreditThreshold;
  uint32_t m_raiseThreshold;
};

class OnoeWifiRemoteStation : public WifiRemoteStation
{
public:
  OnoeWifiRemoteStation (Ptr<OnoeWifiManager> stations);

  virtual ~OnoeWifiRemoteStation ();

protected:
  virtual void DoReportRxOk (double rxSnr, WifiMode txMode);
  virtual void DoReportRtsFailed (void);
  virtual void DoReportDataFailed (void);
  virtual void DoReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr);
  virtual void DoReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr);
  virtual void DoReportFinalRtsFailed (void);
  virtual void DoReportFinalDataFailed (void);

private:
  virtual Ptr<WifiRemoteStationManager> GetManager (void) const;
  virtual WifiMode DoGetDataMode (uint32_t size);
  virtual WifiMode DoGetRtsMode (void);

  void UpdateRetry (void);
  void UpdateMode (void);

  Ptr<OnoeWifiManager> m_stations;
  Time m_nextModeUpdate;
  uint32_t m_shortRetry;
  uint32_t m_longRetry;
  uint32_t m_tx_ok;
  uint32_t m_tx_err;
  uint32_t m_tx_retr;
  uint32_t m_tx_upper;
  uint32_t m_txrate;
};

} // namespace ns3

#endif /* ONOE_WIFI_MANAGER_H */
