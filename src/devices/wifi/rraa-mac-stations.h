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
#ifndef RRAA_MAC_STATIONS_H
#define RRAA_MAC_STATIONS_H

#include "ns3/nstime.h"
#include "mac-stations.h"

namespace ns3 {

struct ThresholdsItem {
  uint32_t datarate;
  double pori;
  double pmtl;
  uint32_t ewnd;
};

typedef std::vector<ThresholdsItem> Thresholds;

/**
 * \brief Robust Rate Adaptation Algorithm
 *
 * This is an implementation of RRAA as described in
 * "Robust rate adaptation for 802.11 wireless networks"
 * by "Starsky H. Y. Wong", "Hao Yang", "Songwu Lu", and,
 * "Vaduvur Bharghavan" published in Mobicom 06.
 */
class RraaMacStations : public MacStations {
public:
  RraaMacStations (WifiMode defaultTxMode);
  virtual ~RraaMacStations ();
  void DoConstruct ();
  bool OnlyBasic (void);
  double GetTimeout (void);
  ThresholdsItem GetThresholds (WifiMode mode);
private:
  virtual class MacStation *CreateStation (void);  
  Thresholds m_thresholds;
  bool m_basic;
  double m_timeout;
};


class RraaMacStation : public MacStation
{
public:
  RraaMacStation (RraaMacStations *stations, Thresholds thresholds);
  virtual ~RraaMacStation ();

  virtual void ReportRxOk (double rxSnr, WifiMode txMode);
  virtual void ReportRtsFailed (void);
  virtual void ReportDataFailed (void);
  virtual void ReportRtsOk (double ctsSnr, WifiMode ctsMode, double rtsSnr);
  virtual void ReportDataOk (double ackSnr, WifiMode ackMode, double dataSnr);
  virtual void ReportFinalRtsFailed (void);
  virtual void ReportFinalDataFailed (void);
  virtual bool NeedRts (Ptr<const Packet> packet);

private:
  virtual RraaMacStations *GetStations (void) const;
  virtual WifiMode DoGetDataMode (uint32_t size);
  virtual WifiMode DoGetRtsMode (void);
  uint32_t GetMaxRate (void);
  uint32_t GetMinRate (void);
  ThresholdsItem GetThresholds (uint32_t rate);
  void CheckTimeout (void);
  void RunBasicAlgorithm (void);
  void ARts (void);
  void ResetCountersBasic (void);

  uint32_t m_counter;
  uint32_t m_failed;
  uint32_t m_rtsWnd;
  uint32_t m_rtsCounter;
  Time m_lastReset;
  bool m_rtsOn;
  bool m_lastFrameFail;
  bool m_initialized;

  uint32_t m_rate;

  RraaMacStations *m_stations;
};

} // namespace ns3

#endif /* RRAA_MAC_STATIONS_H */
