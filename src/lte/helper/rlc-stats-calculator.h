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
  

typedef std::map<lteFlowId_t, uint32_t> uint32Map;
typedef std::map<lteFlowId_t, uint64_t> uint64Map;
typedef std::map<lteFlowId_t, Ptr<MinMaxAvgTotalCalculator<uint64_t> > > uint64StatsMap;


class RlcStatsCalculator : public Object
{
public:
  RlcStatsCalculator();
  virtual
  ~RlcStatsCalculator();
  static TypeId GetTypeId (void);

  void SetOutputFilename (std::string outputFilename);

  void TxPdu (uint16_t rnti, uint8_t lcid, uint32_t packetSize);
  void RxPdu (uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay);

  uint32_t GetTxPackets (lteFlowId_t p);
  uint32_t GetRxPackets (lteFlowId_t p);
  uint64_t GetRxData (lteFlowId_t p);
  uint64_t GetDelay (lteFlowId_t p);
  double   GetThroughput (lteFlowId_t p);

  uint32_t GetTxPackets (uint16_t rnti, uint8_t lcid);
  uint32_t GetRxPackets (uint16_t rnti, uint8_t lcid);
  uint64_t GetRxData (uint16_t rnti, uint8_t lcid);
  uint64_t GetDelay (uint16_t rnti, uint8_t lcid);
  double   GetThroughput (uint16_t rnti, uint8_t lcid);

private:
  void ShowResults (void);
  std::string m_outputFilename;
  std::ofstream m_outFile;
  uint32Map m_txPackets;
  uint32Map m_rxPackets;
  uint64Map m_rxData;
  uint64StatsMap m_delay;
};

} // namespace ns3

#endif /* RLC_STATS_CALCULATOR_H_ */
