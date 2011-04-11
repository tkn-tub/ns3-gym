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
    m_dlOutputFilename (""),
    m_ulOutputFilename (""),
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
    .AddAttribute ("DlOutputFilename",
                   "Name of the file where the downlink results will be saved.",
                   StringValue ("DlRlcStats.csv"),
                   MakeStringAccessor (&RlcStatsCalculator::SetDlOutputFilename),
                   MakeStringChecker ())
    .AddAttribute ("UlOutputFilename",
                   "Name of the file where the uplink results will be saved.",
                   StringValue ("UlRlcStats.csv"),
                   MakeStringAccessor (&RlcStatsCalculator::SetUlOutputFilename),
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
RlcStatsCalculator::SetUlOutputFilename (std::string outputFilename)
{
  m_ulOutputFilename = outputFilename;
}

void
RlcStatsCalculator::SetDlOutputFilename (std::string outputFilename)
{
  m_dlOutputFilename = outputFilename;
}

void
RlcStatsCalculator::UlTxPdu (uint16_t rnti, uint8_t lcid, uint32_t packetSize)
{
  NS_LOG_FUNCTION (this << "UlTxPDU" << rnti << (uint32_t) lcid << packetSize);
  if (Simulator::Now () > m_startTime )
    {
      lteFlowId_t pair (rnti, lcid);
      m_ulTxPackets[pair]++;
    }
  CheckEpoch ();
}

void
RlcStatsCalculator::DlTxPdu (uint16_t rnti, uint8_t lcid, uint32_t packetSize)
{
  NS_LOG_FUNCTION (this << "DlTxPDU" << rnti << (uint32_t) lcid << packetSize);
  if (Simulator::Now () > m_startTime )
    {
      lteFlowId_t pair (rnti, lcid);
      m_dlTxPackets[pair]++;
    }
  CheckEpoch ();
}

void
RlcStatsCalculator::UlRxPdu (uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay)
{
  NS_LOG_FUNCTION (this << "UlRxPDU" << rnti << (uint32_t) lcid << packetSize << delay);
  if (Simulator::Now () > m_startTime )
    {
      lteFlowId_t pair(rnti, lcid);
      m_ulRxPackets[pair]++;
      m_ulRxData[pair] += packetSize;

      uint64StatsMap::iterator it = m_ulDelay.find(pair);
      if (it == m_ulDelay.end())
        {
          m_ulDelay[pair] = CreateObject<MinMaxAvgTotalCalculator<uint64_t> > ();
          m_ulPduSize[pair] = CreateObject<MinMaxAvgTotalCalculator<uint32_t> > ();
        }
      m_ulDelay[pair]->Update (delay);
      m_ulPduSize[pair]->Update (packetSize);
    }
  CheckEpoch ();
}

void
RlcStatsCalculator::DlRxPdu (uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay)
{
  NS_LOG_FUNCTION (this << "UlRxPDU" << rnti << (uint32_t) lcid << packetSize << delay);
  if (Simulator::Now () > m_startTime )
    {
      lteFlowId_t pair(rnti, lcid);
      m_dlRxPackets[pair]++;
      m_dlRxData[pair] += packetSize;

      uint64StatsMap::iterator it = m_dlDelay.find(pair);
      if (it == m_dlDelay.end())
        {
          m_dlDelay[pair] = CreateObject<MinMaxAvgTotalCalculator<uint64_t> > ();
          m_dlPduSize[pair] = CreateObject<MinMaxAvgTotalCalculator<uint32_t> > ();
        }
      m_dlDelay[pair]->Update(delay);
      m_dlPduSize[pair]->Update (packetSize);
    }
  CheckEpoch ();
}

void
RlcStatsCalculator::ShowResults (void)
{

  NS_LOG_FUNCTION (this <<  m_ulOutputFilename.c_str () << m_dlOutputFilename.c_str () );
  NS_LOG_INFO ("Write Rlc Stats in " << m_ulOutputFilename.c_str () <<
               " and in " << m_dlOutputFilename.c_str ());
  
  std::ofstream ulOutFile;
  std::ofstream dlOutFile;

  if ( m_firstWrite == true )
    {
      ulOutFile.open (m_ulOutputFilename.c_str ());
      if (! ulOutFile.is_open ())
      {
        NS_LOG_ERROR ("Can't open file " << m_ulOutputFilename.c_str ());
        return;
      }

      dlOutFile.open (m_dlOutputFilename.c_str ());
      if (! dlOutFile.is_open ())
      {
        NS_LOG_ERROR ("Can't open file " << m_dlOutputFilename.c_str ());
        return;
      }
      m_firstWrite = false;
      ulOutFile << "# startTime, endTime, RNTI, LCID, nTxPDUs, TxBytes, nRxPDUs, RxBytes, ";
      ulOutFile << "delay mean, delay std dev, delay min, delay max, ";
      ulOutFile << "PDU size mean, PDU size std dev, PDU size min, PDU size max, ";
      ulOutFile << std::endl;
      dlOutFile << "# startTime, endTime, RNTI, LCID, nTxPDUs, TxBytes, nRxPDUs, RxBytes, ";
      dlOutFile << "delay mean, delay std dev, delay min, delay max, ";
      dlOutFile << "PDU size mean, PDU size std dev, PDU size min, PDU size max, ";
      dlOutFile << std::endl;
    }
  else
    {
      ulOutFile.open (m_ulOutputFilename.c_str (),  std::ios_base::app);
      if (! ulOutFile.is_open ())
      {
        NS_LOG_ERROR ("Can't open file " << m_ulOutputFilename.c_str ());
        return;
      }

      dlOutFile.open (m_dlOutputFilename.c_str (),  std::ios_base::app);
      if (! dlOutFile.is_open ())
      {
        NS_LOG_ERROR ("Can't open file " << m_dlOutputFilename.c_str ());
        return;
      }
    }

  WriteUlResults(ulOutFile);
  WriteDlResults(dlOutFile);

}

void
RlcStatsCalculator::WriteUlResults (std::ofstream& outFile)
{
  uint32Map::iterator it;

  // Get all the unique lteFlowIds in the calculator
  std::vector<lteFlowId_t> lteFlowIds;
  for ( it = m_ulTxPackets.begin(); it != m_ulTxPackets.end(); ++it)
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
      outFile << m_startTime.GetNanoSeconds () / 1.0e9  << " ";
      outFile << endTime.GetNanoSeconds() / 1.0e9       << " ";
      outFile << (*itFlow).m_rnti                       << " ";
      outFile << (uint32_t) (*itFlow).m_lcId            << " ";
      outFile << GetUlTxPackets (*itFlow)               << " ";
      outFile << GetUlTxData (*itFlow)                  << " ";
      outFile << GetUlRxPackets (*itFlow)               << " ";
      outFile << GetUlRxData (*itFlow)                  << " ";
      std::vector<double> stats = GetUlDelayStats (*itFlow);
      for( std::vector<double>::iterator it = stats.begin (); it != stats.end (); ++it )
        {
          outFile << (*it) * 1e-9 << " ";
        }
      stats = GetUlPduSizeStats (*itFlow);
      for( std::vector<double>::iterator it = stats.begin (); it != stats.end (); ++it )
        {
          outFile << (*it) << " ";
        }
      outFile << std::endl;
    }

  outFile.close ();
}

void
RlcStatsCalculator::WriteDlResults (std::ofstream& outFile)
{
  uint32Map::iterator it;

  // Get all the unique lteFlowIds in the calculator
  std::vector<lteFlowId_t> lteFlowIds;
  for ( it = m_dlTxPackets.begin(); it != m_dlTxPackets.end(); ++it)
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
      outFile << m_startTime.GetNanoSeconds () / 1.0e9  << " ";
      outFile << endTime.GetNanoSeconds() / 1.0e9       << " ";
      outFile << (*itFlow).m_rnti                       << " ";
      outFile << (uint32_t) (*itFlow).m_lcId            << " ";
      outFile << GetDlTxPackets (*itFlow)               << " ";
      outFile << GetDlTxData (*itFlow)                  << " ";
      outFile << GetDlRxPackets (*itFlow)               << " ";
      outFile << GetDlRxData (*itFlow)                  << " ";
      std::vector<double> stats = GetDlDelayStats (*itFlow);
      for( std::vector<double>::iterator it = stats.begin (); it != stats.end (); ++it )
        {
          outFile << (*it) * 1e-9 << " ";
        }
      stats = GetDlPduSizeStats (*itFlow);
      for( std::vector<double>::iterator it = stats.begin (); it != stats.end (); ++it )
        {
          outFile << (*it) << " ";
        }
      outFile << std::endl;
    }
  outFile.close ();
}

void
RlcStatsCalculator::ResetResults (void)
{
   m_ulTxPackets.erase (m_ulTxPackets.begin (), m_ulTxPackets.end () );
   m_ulRxPackets.erase (m_ulRxPackets.begin (), m_ulRxPackets.end () );
   m_ulRxData.erase (m_ulRxData.begin (), m_ulRxData.end () );
   m_ulDelay.erase (m_ulDelay.begin (), m_ulDelay.end () );

   m_dlTxPackets.erase (m_dlTxPackets.begin (), m_dlTxPackets.end () );
   m_dlRxPackets.erase (m_dlRxPackets.begin (), m_dlRxPackets.end () );
   m_dlRxData.erase (m_dlRxData.begin (), m_dlRxData.end () );
   m_dlDelay.erase (m_dlDelay.begin (), m_dlDelay.end () );
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
RlcStatsCalculator::GetUlTxPackets (lteFlowId_t p)
{
  return m_ulTxPackets[p];
}

uint32_t
RlcStatsCalculator::GetUlRxPackets (lteFlowId_t p)
{
  return m_ulRxPackets[p];
}

uint64_t
RlcStatsCalculator::GetUlTxData (lteFlowId_t p)
{
  return m_ulTxData[p];
}

uint64_t
RlcStatsCalculator::GetUlRxData (lteFlowId_t p)
{
  return m_ulRxData[p];
}

double
RlcStatsCalculator::GetUlDelay (lteFlowId_t p)
{
  uint64StatsMap::iterator it = m_ulDelay.find (p);
  if ( it == m_ulDelay.end () )
    {
      NS_LOG_ERROR("UL delay for " << p.m_rnti << ", " <<
                   (uint32_t) p.m_lcId << " not found");
      return 0;

    }
  return m_ulDelay[p]->getMean ();
}

std::vector<double>
RlcStatsCalculator::GetUlDelayStats (lteFlowId_t p)
{
  std::vector<double> stats;
  uint64StatsMap::iterator it = m_ulDelay.find (p);
  if ( it == m_ulDelay.end () )
    {
      NS_LOG_ERROR("UL delay for " << p.m_rnti << ", " <<
                   (uint32_t) p.m_lcId << " not found");
      return stats;

    }
  stats.push_back(m_ulDelay[p]->getMean ());
  stats.push_back(m_ulDelay[p]->getStddev ());
  stats.push_back(m_ulDelay[p]->getMin ());
  stats.push_back(m_ulDelay[p]->getMax ());
  return stats;
}

std::vector<double>
RlcStatsCalculator::GetUlPduSizeStats (lteFlowId_t p)
{
  std::vector<double> stats;
  uint32StatsMap::iterator it = m_ulPduSize.find (p);
  if ( it == m_ulPduSize.end () )
    {
      NS_LOG_ERROR("UL PDU Size for " << p.m_rnti << ", " <<
                   (uint32_t) p.m_lcId << " not found");
      return stats;

    }
  stats.push_back (m_ulPduSize[p]->getMean ());
  stats.push_back (m_ulPduSize[p]->getStddev ());
  stats.push_back (m_ulPduSize[p]->getMin ());
  stats.push_back (m_ulPduSize[p]->getMax ());
  return stats;
}

uint32_t
RlcStatsCalculator::GetDlTxPackets (lteFlowId_t p)
{
  return m_dlTxPackets[p];
}

uint32_t
RlcStatsCalculator::GetDlRxPackets (lteFlowId_t p)
{
  return m_dlRxPackets[p];
}

uint64_t
RlcStatsCalculator::GetDlTxData (lteFlowId_t p)
{
  return m_dlTxData[p];
}

uint64_t
RlcStatsCalculator::GetDlRxData (lteFlowId_t p)
{
  return m_dlRxData[p];
}

double
RlcStatsCalculator::GetDlDelay (lteFlowId_t p)
{
  uint64StatsMap::iterator it = m_dlDelay.find (p);
  if ( it == m_dlDelay.end () )
    {
      NS_LOG_ERROR("DL delay for " << p.m_rnti << ", " <<
                   (uint32_t) p.m_lcId << " not found");
      return 0;
    }
  return m_dlDelay[p]->getMean ();
}

std::vector<double>
RlcStatsCalculator::GetDlDelayStats (lteFlowId_t p)
{
  std::vector<double> stats;
  uint64StatsMap::iterator it = m_dlDelay.find (p);
  if ( it == m_dlDelay.end () )
    {

      NS_LOG_ERROR("DL delay for " << p.m_rnti << ", " <<
                   (uint32_t) p.m_lcId << " not found");
      return stats;

    }
  stats.push_back(m_dlDelay[p]->getMean ());
  stats.push_back(m_dlDelay[p]->getStddev ());
  stats.push_back(m_dlDelay[p]->getMin ());
  stats.push_back(m_dlDelay[p]->getMax ());
  return stats;
}

std::vector<double>
RlcStatsCalculator::GetDlPduSizeStats (lteFlowId_t p)
{
  std::vector<double> stats;
  uint32StatsMap::iterator it = m_dlPduSize.find (p);
  if ( it == m_dlPduSize.end () )
    {

      NS_LOG_ERROR("DL delay for " << p.m_rnti << ", " <<
                   (uint32_t) p.m_lcId << " not found");
      return stats;

    }
  stats.push_back(m_dlPduSize[p]->getMean ());
  stats.push_back(m_dlPduSize[p]->getStddev ());
  stats.push_back(m_dlPduSize[p]->getMin ());
  stats.push_back(m_dlPduSize[p]->getMax ());
  return stats;
}

uint32_t
RlcStatsCalculator::GetUlTxPackets (uint16_t rnti, uint8_t lcid)
{
  lteFlowId_t p (rnti, lcid);
  return GetUlTxPackets (p);
}

uint32_t
RlcStatsCalculator::GetUlRxPackets (uint16_t rnti, uint8_t lcid)
{
  lteFlowId_t p (rnti, lcid);
  return GetUlRxPackets (p);
}

uint64_t
RlcStatsCalculator::GetUlTxData (uint16_t rnti, uint8_t lcid)
{
  lteFlowId_t p (rnti, lcid);
  return GetUlTxData (p);
}

uint64_t
RlcStatsCalculator::GetUlRxData (uint16_t rnti, uint8_t lcid)
{
  lteFlowId_t p (rnti, lcid);
  return GetUlRxData (p);
}

double
RlcStatsCalculator::GetUlDelay (uint16_t rnti, uint8_t lcid)
{
  lteFlowId_t p (rnti, lcid);
  return GetUlDelay (p);
}

std::vector<double>
RlcStatsCalculator::GetUlDelayStats (uint16_t rnti, uint8_t lcid)
{
  lteFlowId_t p (rnti, lcid);
  return GetUlDelayStats (p);
}

std::vector<double>
RlcStatsCalculator::GetUlPduSizeStats (uint16_t rnti, uint8_t lcid)
{
  lteFlowId_t p (rnti, lcid);
  return GetUlPduSizeStats (p);
}


uint32_t
RlcStatsCalculator::GetDlTxPackets (uint16_t rnti, uint8_t lcid)
{
  lteFlowId_t p (rnti, lcid);
  return GetDlTxPackets (p);
}

uint32_t
RlcStatsCalculator::GetDlRxPackets (uint16_t rnti, uint8_t lcid)
{
  lteFlowId_t p (rnti, lcid);
  return GetDlRxPackets (p);
}

uint64_t
RlcStatsCalculator::GetDlTxData (uint16_t rnti, uint8_t lcid)
{
  lteFlowId_t p (rnti, lcid);
  return GetDlTxData (p);
}

uint64_t
RlcStatsCalculator::GetDlRxData (uint16_t rnti, uint8_t lcid)
{
  lteFlowId_t p (rnti, lcid);
  return GetDlRxData (p);
}

double
RlcStatsCalculator::GetDlDelay (uint16_t rnti, uint8_t lcid)
{
  lteFlowId_t p (rnti, lcid);
  return GetDlDelay (p);
}

std::vector<double>
RlcStatsCalculator::GetDlDelayStats (uint16_t rnti, uint8_t lcid)
{
  lteFlowId_t p (rnti, lcid);
  return GetDlDelayStats (p);
}

std::vector<double>
RlcStatsCalculator::GetDlPduSizeStats (uint16_t rnti, uint8_t lcid)
{
  lteFlowId_t p (rnti, lcid);
  return GetDlPduSizeStats (p);
}

} // namespace ns3
