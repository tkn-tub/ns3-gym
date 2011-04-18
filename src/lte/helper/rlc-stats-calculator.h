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
  

typedef std::map<LteFlowId_t, uint32_t> uint32Map;
typedef std::map<LteFlowId_t, uint64_t> uint64Map;
typedef std::map<LteFlowId_t, Ptr<MinMaxAvgTotalCalculator<uint32_t> > > uint32StatsMap;
typedef std::map<LteFlowId_t, Ptr<MinMaxAvgTotalCalculator<uint64_t> > > uint64StatsMap;
typedef std::map<LteFlowId_t, double> doubleMap;


class RlcStatsCalculator : public Object
{
public:
  RlcStatsCalculator();
  virtual
  ~RlcStatsCalculator();
  static TypeId GetTypeId (void);

  void SetUlOutputFilename (std::string outputFilename);
  void SetDlOutputFilename (std::string outputFilename);

  void UlTxPdu (uint16_t rnti, uint8_t lcid, uint32_t packetSize);
  void UlRxPdu (uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay);

  void DlTxPdu (uint16_t rnti, uint8_t lcid, uint32_t packetSize);
  void DlRxPdu (uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay);

  uint32_t GetUlTxPackets (LteFlowId_t p);
  uint32_t GetUlRxPackets (LteFlowId_t p);
  uint64_t GetUlTxData (LteFlowId_t p);
  uint64_t GetUlRxData (LteFlowId_t p);
  double   GetUlDelay (LteFlowId_t p);
  std::vector<double> GetUlDelayStats (LteFlowId_t p);
  std::vector<double> GetUlPduSizeStats (LteFlowId_t p);

  uint32_t GetUlTxPackets (uint16_t rnti, uint8_t lcid);
  uint32_t GetUlRxPackets (uint16_t rnti, uint8_t lcid);
  uint64_t GetUlTxData (uint16_t rnti, uint8_t lcid);
  uint64_t GetUlRxData (uint16_t rnti, uint8_t lcid);
  double   GetUlDelay (uint16_t rnti, uint8_t lcid);
  std::vector<double> GetUlDelayStats (uint16_t rnti, uint8_t lcid);
  std::vector<double> GetUlPduSizeStats (uint16_t rnti, uint8_t lcid);


  uint32_t GetDlTxPackets (LteFlowId_t p);
  uint32_t GetDlRxPackets (LteFlowId_t p);
  uint64_t GetDlTxData (LteFlowId_t p);
  uint64_t GetDlRxData (LteFlowId_t p);
  double   GetDlDelay (LteFlowId_t p);
  std::vector<double> GetDlDelayStats (LteFlowId_t p);
  std::vector<double> GetDlPduSizeStats (LteFlowId_t p);

  uint32_t GetDlTxPackets (uint16_t rnti, uint8_t lcid);
  uint32_t GetDlRxPackets (uint16_t rnti, uint8_t lcid);
  uint64_t GetDlTxData (uint16_t rnti, uint8_t lcid);
  uint64_t GetDlRxData (uint16_t rnti, uint8_t lcid);
  double   GetDlDelay (uint16_t rnti, uint8_t lcid);
  std::vector<double> GetDlDelayStats (uint16_t rnti, uint8_t lcid);
  std::vector<double> GetDlPduSizeStats (uint16_t rnti, uint8_t lcid);

private:
  void ShowResults (void);
  void WriteUlResults (std::ofstream& outFile);
  void WriteDlResults (std::ofstream& outFile);
  void ResetResults (void);

  void StartEpoch (void);
  void CheckEpoch (void);

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
