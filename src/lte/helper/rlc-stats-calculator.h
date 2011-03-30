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
#include <string>
#include <map>
#include <fstream>


namespace ns3 {

struct RntiLcidPair
  {
    uint16_t rnti;
    uint8_t lcid;

    RntiLcidPair (const uint16_t a, const uint8_t b) :
        rnti(a),
        lcid(b)
    {  }

    friend bool operator == (const RntiLcidPair &a, const RntiLcidPair &b)
        {
          return (a.lcid == b.lcid) && (a.rnti == b.rnti);
        }
    friend bool operator < (const RntiLcidPair &a, const RntiLcidPair &b)
      {
        return (a.lcid < b.lcid) || (a.rnti < b.rnti);
      }
  };

typedef std::map<RntiLcidPair, uint32_t> uint32Map;
typedef std::map<RntiLcidPair, uint64_t> uint64Map;
typedef std::map<RntiLcidPair, Ptr<MinMaxAvgTotalCalculator<uint64_t> > > uint64StatsMap;
typedef std::pair<RntiLcidPair, std::string> uint32Pair;
typedef std::pair<RntiLcidPair, uint64_t> uint64Pair;
typedef std::pair<RntiLcidPair, Ptr<MinMaxAvgTotalCalculator<uint64_t> > > uint64StatsPair;

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

  uint32_t GetTxPackets (RntiLcidPair p);
  uint32_t GetRxPackets (RntiLcidPair p);
  uint64_t GetRxData (RntiLcidPair p);
  uint64_t GetDelay (RntiLcidPair p);
  double   GetThroughput (RntiLcidPair p);

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
