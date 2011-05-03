/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Jaume Nin <jnin@cttc.es>
 */

#ifndef RLC_STATS_CALCULATOR_H_
#define RLC_STATS_CALCULATOR_H_

#include "ns3/uinteger.h"
#include "ns3/object.h"
#include "ns3/basic-data-calculators.h"
#include "ns3/lte-common.h"
#include <string>
#include <map>
#include <fstream>

namespace ns3 {
  

typedef std::map<uint64_t, uint32_t> uint32Map;
typedef std::map<uint64_t, uint64_t> uint64Map;
typedef std::map<uint64_t, Ptr<MinMaxAvgTotalCalculator<uint32_t> > > uint32StatsMap;
typedef std::map<uint64_t, Ptr<MinMaxAvgTotalCalculator<uint64_t> > > uint64StatsMap;
typedef std::map<uint64_t, double> doubleMap;
typedef std::map<uint64_t, LteFlowId_t> flowIdMap;


class RlcStatsCalculator : public Object
{
public:
  RlcStatsCalculator();
  virtual
  ~RlcStatsCalculator();
  static TypeId GetTypeId (void);

  void SetUlOutputFilename (std::string outputFilename);
  void SetDlOutputFilename (std::string outputFilename);

  void UlTxPdu (uint64_t imsi, uint16_t rnti, uint8_t lcid, uint32_t packetSize);
  void UlRxPdu (uint64_t imsi, uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay);

  void DlTxPdu (uint64_t imsi, uint16_t rnti, uint8_t lcid, uint32_t packetSize);
  void DlRxPdu (uint64_t imsi, uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay);

  uint32_t GetUlTxPackets (uint64_t imsi);
  uint32_t GetUlRxPackets (uint64_t imsi);
  uint64_t GetUlTxData (uint64_t imsi);
  uint64_t GetUlRxData (uint64_t imsi);
  double   GetUlDelay (uint64_t imsi);
  std::vector<double> GetUlDelayStats (uint64_t imsi);
  std::vector<double> GetUlPduSizeStats (uint64_t imsi);


  uint32_t GetDlTxPackets (uint64_t imsi);
  uint32_t GetDlRxPackets (uint64_t imsi);
  uint64_t GetDlTxData (uint64_t imsi);
  uint64_t GetDlRxData (uint64_t imsi);
  double   GetDlDelay (uint64_t imsi);
  std::vector<double> GetDlDelayStats (uint64_t imsi);
  std::vector<double> GetDlPduSizeStats (uint64_t imsi);

private:
  void ShowResults (void);
  void WriteUlResults (std::ofstream& outFile);
  void WriteDlResults (std::ofstream& outFile);
  void ResetResults (void);

  void StartEpoch (void);
  void CheckEpoch (void);

  flowIdMap m_flowId;

  std::string m_dlOutputFilename;
  uint32Map m_dlTxPackets;
  uint32Map m_dlRxPackets;
  uint64Map m_dlTxData;
  uint64Map m_dlRxData;
  uint64StatsMap m_dlDelay;
  uint32StatsMap m_dlPduSize;

  std::string m_ulOutputFilename;
  uint32Map m_ulTxPackets;
  uint32Map m_ulRxPackets;
  uint64Map m_ulTxData;
  uint64Map m_ulRxData;
  uint64StatsMap m_ulDelay;
  uint32StatsMap m_ulPduSize;

  /**
   * Start time of the on going epoch
   */
  Time m_startTime;

  /**
   * Epoch duration
   */
  Time m_epochDuration;

  bool m_firstWrite;

};

} // namespace ns3

#endif /* RLC_STATS_CALCULATOR_H_ */
