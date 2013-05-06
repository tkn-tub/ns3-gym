/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */



#ifndef RADIO_BEARER_STATS_CONNECTOR_H
#define RADIO_BEARER_STATS_CONNECTOR_H


#include <ns3/traced-callback.h>
#include <ns3/config.h>
#include <ns3/simple-ref-count.h>
#include <ns3/ptr.h>

#include <set>
#include <map>

namespace ns3 {

class RadioBearerStatsCalculator;

class RadioBearerStatsConnector
{
public:

  RadioBearerStatsConnector ();

  void EnableRlcStats (Ptr<RadioBearerStatsCalculator> rlcStats);
  void EnablePdcpStats (Ptr<RadioBearerStatsCalculator> pdcpStats);
  void EnsureConnected ();

  // trace sinks, to be used with MakeBoundCallback
  static void NotifyRandomAccessSuccessfulUe (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti);
  static void NotifyConnectionSetupUe (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti);
  static void NotifyConnectionReconfigurationUe (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti);
  static void NotifyHandoverStartUe (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti, uint16_t targetCellId);
  static void NotifyHandoverEndOkUe (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti);
  static void NotifyNewUeContextEnb (RadioBearerStatsConnector* c, std::string context, uint16_t cellid, uint16_t rnti);
  static void NotifyConnectionReconfigurationEnb (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti);
  static void NotifyHandoverStartEnb (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti, uint16_t targetCellId);
  static void NotifyHandoverEndOkEnb (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti);

private:

  void StoreUeManagerPath (std::string ueManagerPath, uint16_t cellId, uint16_t rnti);
  void ConnectSrb0Traces (std::string ueRrcPath, uint64_t imsi, uint16_t cellId, uint16_t rnti);
  void ConnectSrb1TracesUe (std::string ueRrcPath, uint64_t imsi, uint16_t cellId, uint16_t rnti);
  void ConnectTracesUeIfFirstTime (std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti);
  void ConnectTracesEnbIfFirstTime (std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti);
  void ConnectTracesUe (std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti);
  void DisconnectTracesUe (std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti);
  void ConnectTracesEnb (std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti);
  void DisconnectTracesEnb (std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti);


  Ptr<RadioBearerStatsCalculator> m_rlcStats;
  Ptr<RadioBearerStatsCalculator> m_pdcpStats;

  bool m_connected;
  std::set<uint64_t> m_imsiSeenUe;
  std::set<uint64_t> m_imsiSeenEnb;
  
  struct CellIdRnti
  {
    uint16_t cellId;
    uint16_t rnti;
  };
  friend bool operator < (const CellIdRnti &a, const CellIdRnti &b);
  std::map<CellIdRnti, std::string> m_ueManagerPathByCellIdRnti;

};



} // namespace ns3


#endif // RADIO_BEARER_STATS_CONNECTOR_H
