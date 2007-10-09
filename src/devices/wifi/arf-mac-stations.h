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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef ARF_MAC_STATIONS_H
#define ARF_MAC_STATIONS_H

#include "mac-stations.h"

namespace ns3 {

class ArfMacStations : public MacStations {
public:
  ArfMacStations (WifiMode defaultTxMode);
  virtual ~ArfMacStations ();

private:
  virtual class MacStation *CreateStation (WifiMode defaultTxMode);
};


class ArfMacStation : public MacStation
{
public:
  ArfMacStation (WifiMode defaultTxMode,
                 int minTimerTimeout,
                 int minSuccessThreshold);
  virtual ~ArfMacStation ();

  virtual void ReportRxOk (double rxSnr, WifiMode txMode);
  virtual void ReportRtsFailed (void);
  virtual void ReportDataFailed (void);
  virtual void ReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr);
  virtual void ReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr);
  virtual WifiMode GetDataMode (uint32_t size);
  virtual WifiMode GetRtsMode (void);

private:
  int m_timer;
  int m_success;
  int m_failed;
  bool m_recovery;
  int m_retry;
  
  int m_timerTimeout;
  int m_successThreshold;

  int m_rate;
  
  int m_minTimerTimeout;
  int m_minSuccessThreshold;
  
private:
  virtual void ReportRecoveryFailure (void);
  virtual void ReportFailure (void);

  int GetMaxRate (void);
  int GetMinRate (void);

  bool NeedRecoveryFallback (void);
  bool NeedNormalFallback (void);
  
protected:
  int GetMinTimerTimeout (void);
  int GetMinSuccessThreshold (void);
  
  int GetTimerTimeout (void);
  int GetSuccessThreshold (void);
  
  void SetTimerTimeout (int timerTimeout);
  void SetSuccessThreshold (int successThreshold);
};

}; // namespace ns3

#endif /* ARF_MAC_STATIONS_H */
