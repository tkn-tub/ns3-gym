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
 * Author: Federico Maguolo <maguolof@dei.unipd.it>
 */
#ifndef CARA_WIFI_MANAGER_H
#define CARA_WIFI_MANAGER_H

#include "wifi-remote-station-manager.h"

namespace ns3 {

/**
 * \brief implement the CARA rate control algorithm
 *
 * Implement the CARA algorithm from:
 * J. Kim, S. Kim, S. Choi, and D. Qiao. 
 * "CARA: Collision-Aware Rate Adaptation for IEEE 802.11 WLANs."
 *
 * Originally implemented by Federico Maguolo for a very early 
 * prototype version of ns-3.
 */
class CaraWifiManager : public WifiRemoteStationManager 
{
public:
  static TypeId GetTypeId (void);
  CaraWifiManager ();
  virtual ~CaraWifiManager ();

private:
  friend class CaraWifiRemoteStation;
  virtual class WifiRemoteStation *CreateStation (void);
  uint32_t m_timerTimeout;
  uint32_t m_successThreshold;
  uint32_t m_failureThreshold;
  uint32_t m_probeThreshold;
};


class CaraWifiRemoteStation : public WifiRemoteStation
{
public:
  CaraWifiRemoteStation (Ptr<CaraWifiManager> manager);
  virtual ~CaraWifiRemoteStation ();

private:
  virtual Ptr<WifiRemoteStationManager> GetManager (void) const;
  virtual void DoReportRtsFailed (void);
  virtual void DoReportDataFailed (void);
  virtual void DoReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr);
  virtual void DoReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr);
  virtual void DoReportFinalRtsFailed (void);
  virtual void DoReportFinalDataFailed (void);
  virtual void DoReportRxOk (double rxSnr, WifiMode txMode);
  virtual WifiMode DoGetDataMode (uint32_t size);
  virtual WifiMode DoGetRtsMode (void);

  virtual bool NeedRts (Ptr<const Packet> packet);

  uint32_t m_timer;
  uint32_t m_success;
  uint32_t m_failed;
  
  uint32_t m_rate;
  
  Ptr<CaraWifiManager> m_manager;
  
  uint32_t GetMaxRate (void);
  uint32_t GetMinRate (void);

  bool NeedNormalFallback (void);
  
};

} // namespace ns3

#endif /* CARA_WIFI_MANAGER_H */
