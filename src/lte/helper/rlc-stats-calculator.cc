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

#include "rlc-stats-calculator.h"
#include "ns3/string.h"
#include "ns3/nstime.h"
#include <ns3/log.h>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RlcStatsCalculator");

NS_OBJECT_ENSURE_REGISTERED (RlcStatsCalculator);

RlcStatsCalculator::RlcStatsCalculator() :
    m_outputFilename ("")
{
  NS_LOG_FUNCTION (this);

}

RlcStatsCalculator::~RlcStatsCalculator()
{
  NS_LOG_FUNCTION (this);
  ShowResults ();
}

TypeId
RlcStatsCalculator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RlcStatsCalculator")
    .SetParent<Object> ()
    .AddConstructor<RlcStatsCalculator> ()
    .AddAttribute ("OutputFilename",
                   "Name of the file where the output will be saved.",
                   StringValue ("RlcStats.csv"),
                   MakeStringAccessor (&RlcStatsCalculator::SetOutputFilename),
                   MakeStringChecker ())
    ;
  return tid;
}

void
RlcStatsCalculator::SetOutputFilename (std::string outputFilename)
{
  m_outputFilename = outputFilename;
}

void
RlcStatsCalculator::TxPdu (uint16_t rnti, uint8_t lcid, uint32_t packetSize)
{
  NS_LOG_FUNCTION (this << " TxPDU " << rnti << (uint32_t) lcid << packetSize);
  RntiLcidPair pair = RntiLcidPair(rnti, lcid);
  uint32Map::iterator it = m_txPackets.find (pair);
  m_txPackets[pair]++;
}

void
RlcStatsCalculator::RxPdu (uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay)
{
  NS_LOG_FUNCTION (this << " RxPDU " << rnti << (uint32_t) lcid << packetSize << delay);
  RntiLcidPair pair = RntiLcidPair(rnti, lcid);

  m_rxPackets [pair]++;
  m_rxData [pair] += packetSize;

  uint64StatsMap::iterator it = m_delay.find (pair);
  if ( it == m_delay.end () )
    {
      m_delay[pair] = CreateObject <MinMaxAvgTotalCalculator<uint64_t> > ();
    }
  m_delay[pair]->Update (delay);
}

void
RlcStatsCalculator::ShowResults (void)
{
  uint32Map::iterator it;
  uint64Map::iterator itData;
  uint64StatsMap::iterator itDelay;

  NS_LOG_FUNCTION (this <<  m_outputFilename.c_str ());
  NS_LOG_INFO ("Write Rlc Stats in: " << m_outputFilename.c_str ());
  
  std::ofstream m_outFile;
  m_outFile.open (m_outputFilename.c_str ());
  if (! m_outFile.is_open ())
  {
    NS_LOG_ERROR ("Can't open file " << m_outputFilename.c_str ());
    return;
  }

  m_outFile << "# Parameter, RNTI, LCID, value" << std::endl;
  for ( it = m_txPackets.begin(); it != m_txPackets.end(); ++it)
    {
      m_outFile << "TxPackets, " << (*it).first.rnti << ", "
                                 << (uint32_t) (*it).first.lcid << ", "
                                 << (*it).second << std::endl;
    }
  for ( it = m_rxPackets.begin(); it != m_rxPackets.end(); ++it)
    {
      m_outFile << "RxPackets, " << (*it).first.rnti << ", "
                                 << (uint32_t) (*it).first.lcid << ", "
                                 << (*it).second << std::endl;
    }
  for ( itData = m_rxData.begin(); itData != m_rxData.end(); ++itData)
    {
      m_outFile << "RxData, " << (*itData).first.rnti << ", "
                              << (uint32_t) (*itData).first.lcid << ", "
                              << (*itData).second << std::endl;
    }
  for ( itData = m_rxData.begin(); itData != m_rxData.end(); ++itData)
    {
      m_outFile << "Throughput, " << (*itData).first.rnti << ", "
                                  << (uint32_t) (*itData).first.lcid << ", "
                                  << GetThroughput ((*itData).first) << std::endl;
    }
  for ( itDelay = m_delay.begin (); itDelay != m_delay.end (); ++itDelay)
    {
      m_outFile << "Delay, " << (*itDelay).first.rnti << ", "
                             << (uint32_t) (*itDelay).first.lcid << ", "
                             << GetDelay ((*itDelay).first) << std::endl;
    }

  m_outFile.close ();
}

uint32_t
RlcStatsCalculator::GetTxPackets (RntiLcidPair p)
{
  return m_txPackets[p];
}

uint32_t
RlcStatsCalculator::GetRxPackets (RntiLcidPair p)
{
  return m_rxPackets[p];
}

uint64_t
RlcStatsCalculator::GetRxData (RntiLcidPair p)
{
  return m_rxData[p];
}

uint64_t
RlcStatsCalculator::GetDelay (RntiLcidPair p)
{
  uint64StatsMap::iterator it = m_delay.find (p);
  if ( it == m_delay.end () )
    {
      return 0;
    }
  return m_delay[p]->getMean ();
}

double
RlcStatsCalculator::GetThroughput (RntiLcidPair p)
{
  // TODO: Fix throughput calculation with the correct time
  //return (double) m_rxData[p] / Simulator::Now().GetSeconds();
  return 0;
}

uint32_t
RlcStatsCalculator::GetTxPackets (uint16_t rnti, uint8_t lcid)
{
  RntiLcidPair p = RntiLcidPair (rnti, lcid);
  return GetTxPackets(p);
}

uint32_t
RlcStatsCalculator::GetRxPackets (uint16_t rnti, uint8_t lcid)
{
  RntiLcidPair p = RntiLcidPair (rnti, lcid);
  return GetRxPackets(p);
}

uint64_t
RlcStatsCalculator::GetRxData (uint16_t rnti, uint8_t lcid)
{
  RntiLcidPair p = RntiLcidPair (rnti, lcid);
  return GetRxData(p);
}

uint64_t
RlcStatsCalculator::GetDelay (uint16_t rnti, uint8_t lcid)
{
  RntiLcidPair p = RntiLcidPair (rnti, lcid);
  return GetDelay(p);
}

double
RlcStatsCalculator::GetThroughput (uint16_t rnti, uint8_t lcid)
{
  RntiLcidPair p = RntiLcidPair (rnti, lcid);
  return GetThroughput(p);
}

} // namespace ns3
