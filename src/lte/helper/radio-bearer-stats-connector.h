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

namespace ns3 {

class RadioBearerStatsCalculator;

class RadioBearerStatsConnector
{
public:

  static uint64_t FindImsiFromEnbRlcPath (std::string path);
  static uint16_t FindCellIdFromEnbRlcPath (std::string path);
  static uint64_t FindImsiFromUeRlcPath (std::string path);
  static uint16_t FindCellIdFromUeRlcPath (std::string path);

  RadioBearerStatsConnector ();

  void EnableRlcStats (Ptr<RadioBearerStatsCalculator> rlcStats);
  void EnablePdcpStats (Ptr<RadioBearerStatsCalculator> pdcpStats);
  void EnsureConnected ();

  // trace sinks, to be used with MakeBoundCallback
  static void NotifyConnectionReconfigurationUe (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti);
  static void NotifyHandoverStartUe (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti, uint16_t targetCellId);
  static void NotifyHandoverEndOkUe (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti);
  static void NotifyConnectionReconfigurationEnb (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti);
  static void NotifyHandoverStartEnb (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti, uint16_t targetCellId);
  static void NotifyHandoverEndOkEnb (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellid, uint16_t rnti);

private:
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

};



} // namespace ns3


#endif // RADIO_BEARER_STATS_CONNECTOR_H
