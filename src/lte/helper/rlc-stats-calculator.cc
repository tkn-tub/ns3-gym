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
#include <vector>
#include <algorithm>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RlcStatsCalculator");

NS_OBJECT_ENSURE_REGISTERED (RlcStatsCalculator);

RlcStatsCalculator::RlcStatsCalculator() :
    m_outputFilename (""),
    m_firstWrite(true)
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
    .AddAttribute ("StartTime",
                   "Start time of the on going epoch.",
                   TimeValue ( Seconds(0.) ),
                   MakeTimeAccessor (&RlcStatsCalculator::m_startTime),
                   MakeTimeChecker ())
    .AddAttribute("EpochDuration",
                  "Epoch duration.", TimeValue(Seconds(0.25)),
                  MakeTimeAccessor(&RlcStatsCalculator::m_epochDuration),
                  MakeTimeChecker())
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
  NS_LOG_FUNCTION (this << "TxPDU" << rnti << (uint32_t) lcid << packetSize);
  if (Simulator::Now () > m_startTime )
    {
      lteFlowId_t pair (rnti, lcid);
      m_txPackets[pair]++;
    }
  CheckEpoch ();
}

void
RlcStatsCalculator::RxPdu (uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay)
{
  NS_LOG_FUNCTION (this << "RxPDU" << rnti << (uint32_t) lcid << packetSize << delay);
  if (Simulator::Now () > m_startTime )
    {
      lteFlowId_t pair(rnti, lcid);
      m_rxPackets[pair]++;
      m_rxData[pair] += packetSize;
      m_throughput[pair] = 8 * m_rxData[pair] / 1.0e9 * (Simulator::Now().GetNanoSeconds() - m_startTime.GetNanoSeconds());

      uint64StatsMap::iterator it = m_delay.find(pair);
      if (it == m_delay.end())
        {
          m_delay[pair] = CreateObject<MinMaxAvgTotalCalculator<uint64_t> > ();
          }
      m_delay[pair]->Update(delay);
    }
  CheckEpoch ();
}

void
RlcStatsCalculator::ShowResults (void)
{
  uint32Map::iterator it;


  NS_LOG_FUNCTION (this <<  m_outputFilename.c_str ());
  NS_LOG_INFO ("Write Rlc Stats in: " << m_outputFilename.c_str ());
  
  std::ofstream m_outFile;
  if ( m_firstWrite == true )
    {
      m_outFile.open (m_outputFilename.c_str ());
      if (! m_outFile.is_open ())
      {
        NS_LOG_ERROR ("Can't open file " << m_outputFilename.c_str ());
        return;
      }
      m_firstWrite = false;
      m_outFile << "# startTime, endTime, RNTI, LCID, throughput (bps), delay (s), PDU loss ratio (%)" << std::endl;
    }
  else
    {
      m_outFile.open (m_outputFilename.c_str (),  std::ios_base::app);
      if (! m_outFile.is_open ())
      {
        NS_LOG_ERROR ("Can't open file " << m_outputFilename.c_str ());
        return;
      }
    }



  // Get all the unique lteFlowIds in the calculator
  std::vector<lteFlowId_t> lteFlowIds;
  for ( it = m_txPackets.begin(); it != m_txPackets.end(); ++it)
    {
      if (find (lteFlowIds.begin (), lteFlowIds.end (),  (*it).first ) == lteFlowIds.end () )
        {
          lteFlowIds.push_back ((*it).first);
        }
    }

  std::vector<lteFlowId_t>::iterator itFlow;
  Time endTime = m_startTime + m_epochDuration;
  for ( itFlow = lteFlowIds.begin(); itFlow != lteFlowIds.end(); ++itFlow)
    {
      m_outFile << m_startTime.GetNanoSeconds () / 1.0e9  << " " << endTime.GetNanoSeconds() / 1.0e9;
      m_outFile << " " << (*itFlow).m_rnti << " " << (uint32_t) (*itFlow).m_lcId << " " << GetThroughput (*itFlow);
      m_outFile << " " << GetDelay(*itFlow) << " " << GetPacketLossProbability (*itFlow) << std::endl;
    }
  m_outFile.close ();
}

void
RlcStatsCalculator::ResetResults (void)
{
   m_txPackets.erase (m_txPackets.begin (), m_txPackets.end () );
   m_rxPackets.erase (m_rxPackets.begin (), m_rxPackets.end () );
   m_rxData.erase (m_rxData.begin (), m_rxData.end () );
   m_throughput.erase (m_throughput.begin (), m_throughput.end () );
   m_delay.erase (m_delay.begin (), m_delay.end () );
}

void
RlcStatsCalculator::CheckEpoch (void)
{
  if ( Simulator::Now () > m_startTime + m_epochDuration )
    {
      ShowResults();
      ResetResults();
      StartEpoch();
    }

}

void
RlcStatsCalculator::StartEpoch (void)
{
  m_startTime += m_epochDuration;
}

uint32_t
RlcStatsCalculator::GetTxPackets (lteFlowId_t p)
{
  return m_txPackets[p];
}

uint32_t
RlcStatsCalculator::GetRxPackets (lteFlowId_t p)
{
  return m_rxPackets[p];
}

uint64_t
RlcStatsCalculator::GetRxData (lteFlowId_t p)
{
  return m_rxData[p];
}

uint64_t
RlcStatsCalculator::GetDelay (lteFlowId_t p)
{
  uint64StatsMap::iterator it = m_delay.find (p);
  if ( it == m_delay.end () )
    {
      return 0;
    }
  return m_delay[p]->getMean ();
}

double
RlcStatsCalculator::GetThroughput (lteFlowId_t p)
{
  return m_throughput[p];
}

double
RlcStatsCalculator::GetPacketLossProbability (lteFlowId_t p)
{
   return (GetTxPackets (p) - GetRxPackets (p)) / (double) GetTxPackets (p);
}

uint32_t
RlcStatsCalculator::GetTxPackets (uint16_t rnti, uint8_t lcid)
{
  lteFlowId_t p (rnti, lcid);
  return GetTxPackets (p);
}

uint32_t
RlcStatsCalculator::GetRxPackets (uint16_t rnti, uint8_t lcid)
{
  lteFlowId_t p (rnti, lcid);
  return GetRxPackets (p);
}

uint64_t
RlcStatsCalculator::GetRxData (uint16_t rnti, uint8_t lcid)
{
  lteFlowId_t p (rnti, lcid);
  return GetRxData (p);
}

uint64_t
RlcStatsCalculator::GetDelay (uint16_t rnti, uint8_t lcid)
{
  lteFlowId_t p (rnti, lcid);
  return GetDelay (p);
}

double
RlcStatsCalculator::GetThroughput (uint16_t rnti, uint8_t lcid)
{
  lteFlowId_t p (rnti, lcid);
  return GetThroughput (p);
}

double
RlcStatsCalculator::GetPacketLossProbability (uint16_t rnti, uint8_t lcid)
{
  lteFlowId_t p (rnti, lcid);
  return GetPacketLossProbability (p);
}

} // namespace ns3
