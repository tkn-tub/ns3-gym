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

/**
 * \brief ARF Rate control algorithm
 *
 * This class implements the so-called ARF algorithm which was
 * initially described in <i>WaveLAN-II: A High-performance wireless 
 * LAN for the unlicensed band</i>, by A. Kamerman and L. Monteban. in
 * Bell Lab Technical Journal, pages 118-133, Summer 1997.
 *
 * This implementation differs from the initial description in that it
 * uses a packet-based timer rather than a time-based timer as described 
 * in XXX (I cannot find back the original paper which described how
 * the time-based timer could be easily replaced with a packet-based 
 * timer.)
 */
class ArfMacStations : public MacStations {
public:
  ArfMacStations (WifiMode defaultTxMode, uint32_t timerThreshold, uint32_t successThreshold);
  virtual ~ArfMacStations ();

private:
  virtual class MacStation *CreateStation (void);
  uint32_t m_timerThreshold;
  uint32_t m_successThreshold;
};


class ArfMacStation : public MacStation
{
public:
  ArfMacStation (ArfMacStations *stations,
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
  virtual ArfMacStations *GetStations (void) const;

  uint32_t m_timer;
  uint32_t m_success;
  uint32_t m_failed;
  bool m_recovery;
  uint32_t m_retry;
  
  uint32_t m_timerTimeout;
  uint32_t m_successThreshold;

  uint32_t m_rate;
  
  uint32_t m_minTimerTimeout;
  uint32_t m_minSuccessThreshold;

  ArfMacStations *m_stations;
  
private:
  // overriden by AarfMacStation.
  virtual void ReportRecoveryFailure (void);
  virtual void ReportFailure (void);

  uint32_t GetMaxRate (void);
  uint32_t GetMinRate (void);

  bool NeedRecoveryFallback (void);
  bool NeedNormalFallback (void);
  
protected:
  // called by AarfMacStation.
  uint32_t GetMinTimerTimeout (void);
  uint32_t GetMinSuccessThreshold (void);
  
  uint32_t GetTimerTimeout (void);
  uint32_t GetSuccessThreshold (void);
  
  void SetTimerTimeout (uint32_t timerTimeout);
  void SetSuccessThreshold (uint32_t successThreshold);
};

} // namespace ns3

#endif /* ARF_MAC_STATIONS_H */
