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
#ifndef RRAA_WIFI_MANAGER_H
#define RRAA_WIFI_MANAGER_H

#include "ns3/nstime.h"
#include "wifi-remote-station-manager.h"

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
class RraaWifiManager : public WifiRemoteStationManager 
{
public:
  static TypeId GetTypeId (void);

  RraaWifiManager ();
  virtual ~RraaWifiManager ();
  bool OnlyBasic (void);
  Time GetTimeout (void) const;
  ThresholdsItem GetThresholds (WifiMode mode) const;
private:
  virtual class WifiRemoteStation *CreateStation (void);  
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


class RraaWifiRemoteStation : public WifiRemoteStation
{
public:
  RraaWifiRemoteStation (Ptr<RraaWifiManager> stations);
  virtual ~RraaWifiRemoteStation ();

  virtual bool NeedRts (Ptr<const Packet> packet);
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

  Ptr<RraaWifiManager> m_stations;
};

} // namespace ns3

#endif /* RRAA_WIFI_MANAGER_H */
