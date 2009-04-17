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
#ifndef AARFCD_WIFI_MANAGER_H
#define AARFCD_WIFI_MANAGER_H

#include "wifi-remote-station-manager.h"

namespace ns3 {

class AarfcdWifiManager : public WifiRemoteStationManager 
{
public:
  static TypeId GetTypeId (void);
  AarfcdWifiManager ();
  virtual ~AarfcdWifiManager ();

private:
  friend class AarfcdWifiRemoteStation;
  virtual WifiRemoteStation *CreateStation (void);
  uint32_t m_minTimerThreshold;
  uint32_t m_minSuccessThreshold;
  double m_successK;
  uint32_t m_maxSuccessThreshold;
  double m_timerK;
  uint32_t m_minRtsWnd;
  uint32_t m_maxRtsWnd;
  bool m_rtsFailsAsDataFails;
  bool m_turnOffRtsAfterRateDecrease;
  bool m_turnOnRtsAfterRateIncrease;
};


class AarfcdWifiRemoteStation : public WifiRemoteStation
{
public:
  AarfcdWifiRemoteStation (Ptr<AarfcdWifiManager> manager,
                           int minTimerThreshold, 
                           int minSuccessThreshold, 
                           double successK,
                           int maxSuccessThreshold,
                           double timerK);
  virtual ~AarfcdWifiRemoteStation ();


private:
  virtual void DoReportRxOk (double rxSnr, WifiMode txMode);
  virtual void DoReportRtsFailed (void);
  virtual void DoReportDataFailed (void);
  virtual void DoReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr);
  virtual void DoReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr);
  virtual void DoReportFinalRtsFailed (void);
  virtual void DoReportFinalDataFailed (void);
  virtual bool DoNeedRts (Ptr<const Packet> packet);
  virtual Ptr<WifiRemoteStationManager> GetManager (void) const;
  virtual WifiMode DoGetDataMode (uint32_t size);
  virtual WifiMode DoGetRtsMode (void);

  void ReportRecoveryFailure (void);
  void ReportFailure (void);
  uint32_t GetMaxRate (void);
  uint32_t GetMinRate (void);
  void CheckRts (void);
  void IncreaseRtsWnd (void);
  void ResetRtsWnd (void);
  void TurnOffRts (void);
  void TurnOnRts (void);

  bool NeedRecoveryFallback (void);
  bool NeedNormalFallback (void);
  
  uint32_t GetTimerTimeout (void);
  uint32_t GetSuccessThreshold (void);
  uint32_t GetMinSuccessThreshold (void);
  uint32_t GetMinTimerTimeout (void);
  
  void SetTimerTimeout (uint32_t timerTimeout);
  void SetSuccessThreshold (uint32_t successThreshold);


  uint32_t m_timer;
  uint32_t m_success;
  uint32_t m_failed;
  bool m_recovery;
  bool m_justModifyRate;
  uint32_t m_retry;
  
  uint32_t m_minTimerThreshold;
  uint32_t m_minSuccessThreshold;
  double m_successK;
  uint32_t m_maxSuccessThreshold;
  double m_timerK;
  uint32_t m_successThreshold;
  uint32_t m_timerTimeout;

  uint32_t m_rate;
  bool m_rtsOn;
  uint32_t m_rtsWnd;
  uint32_t m_rtsCounter;
  bool m_haveASuccess;
  
  Ptr<AarfcdWifiManager> m_manager;
};

} // namespace ns3

#endif /* MAARF_MAC_STATIONS_H */
