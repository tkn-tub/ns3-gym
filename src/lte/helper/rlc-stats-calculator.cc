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
RlcStatsCalculator::UlTxPdu (uint64_t imsi, uint16_t rnti, uint8_t lcid, uint32_t packetSize)
{
  NS_LOG_FUNCTION (this << "UlTxPDU" << imsi << rnti << (uint32_t) lcid << packetSize);
  if (Simulator::Now () > m_startTime )
    {
      m_flowId[imsi] = LteFlowId_t (rnti, lcid);
      m_ulTxPackets[imsi]++;
      m_ulTxData[imsi] += packetSize;
    }
  CheckEpoch ();
}

void
RlcStatsCalculator::DlTxPdu (uint64_t imsi, uint16_t rnti, uint8_t lcid, uint32_t packetSize)
{
  NS_LOG_FUNCTION (this << "DlTxPDU" << imsi  << rnti << (uint32_t) lcid << packetSize);
  if (Simulator::Now () > m_startTime )
    {
      m_flowId[imsi] = LteFlowId_t (rnti, lcid);
      m_dlTxPackets[imsi]++;
      m_dlTxData[imsi] += packetSize;
    }
  CheckEpoch ();
}

void
RlcStatsCalculator::UlRxPdu (uint64_t imsi, uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay)
{
  NS_LOG_FUNCTION (this << "UlRxPDU" << imsi << rnti << (uint32_t) lcid << packetSize << delay);
  if (Simulator::Now () > m_startTime )
    {
      m_ulRxPackets[imsi]++;
      m_ulRxData[imsi] += packetSize;

      uint64StatsMap::iterator it = m_ulDelay.find(imsi);
      if (it == m_ulDelay.end())
        {
          m_ulDelay[imsi] = CreateObject<MinMaxAvgTotalCalculator<uint64_t> > ();
          m_ulPduSize[imsi] = CreateObject<MinMaxAvgTotalCalculator<uint32_t> > ();
        }
      m_ulDelay[imsi]->Update (delay);
      m_ulPduSize[imsi]->Update (packetSize);
    }
  CheckEpoch ();
}

void
RlcStatsCalculator::DlRxPdu (uint64_t imsi, uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay)
{
  NS_LOG_FUNCTION (this << "DlRxPDU" << imsi << rnti << (uint32_t) lcid << packetSize << delay);
  if (Simulator::Now () > m_startTime )
    {
      m_dlRxPackets[imsi]++;
      m_dlRxData[imsi] += packetSize;

      uint64StatsMap::iterator it = m_dlDelay.find(imsi);
      if (it == m_dlDelay.end())
        {
          m_dlDelay[imsi] = CreateObject<MinMaxAvgTotalCalculator<uint64_t> > ();
          m_dlPduSize[imsi] = CreateObject<MinMaxAvgTotalCalculator<uint32_t> > ();
        }
      m_dlDelay[imsi]->Update(delay);
      m_dlPduSize[imsi]->Update (packetSize);
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
      ulOutFile << "# start\tend\tIMSI\tRNTI\tLCID\tnTxPDUs\tTxBytes\tnRxPDUs\tRxBytes\t";
      ulOutFile << "delay\tstdDev\tmin\tmax\t";
      ulOutFile << "PduSize\tstdDev\tmin\tmax";
      ulOutFile << std::endl;
      dlOutFile << "# start\tend\tIMSI\tRNTI\tLCID\tnTxPDUs\tTxBytes\tnRxPDUs\tRxBytes\t";
      dlOutFile << "delay\tstdDev\tmin\tmax\t";
      dlOutFile << "PduSize\tstdDev\tmin\tmax";
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
  // Get the unique IMSI list
  std::vector<uint64_t> imsiVector;
  for (uint32Map::iterator it = m_ulTxPackets.begin(); it != m_ulTxPackets.end(); ++it)
    {
      if (find (imsiVector.begin (), imsiVector.end (),  (*it).first ) == imsiVector.end () )
        {
          imsiVector.push_back ((*it).first);
        }
    }

  Time endTime = m_startTime + m_epochDuration;
  for (std::vector<uint64_t>::iterator imsi = imsiVector.begin(); imsi != imsiVector.end(); ++imsi)
    {
      outFile << m_startTime.GetNanoSeconds () / 1.0e9 << "\t";
      outFile << endTime.GetNanoSeconds() / 1.0e9      << "\t";
      outFile << (*imsi)                               << "\t";
      outFile << m_flowId[*imsi].m_rnti                << "\t";
      outFile << (uint32_t) m_flowId[*imsi].m_lcId     << "\t";
      outFile << GetUlTxPackets (*imsi)                << "\t";
      outFile << GetUlTxData (*imsi)                   << "\t";
      outFile << GetUlRxPackets (*imsi)                << "\t";
      outFile << GetUlRxData (*imsi)                   << "\t";
      std::vector<double> stats = GetUlDelayStats (*imsi);
      for( std::vector<double>::iterator it = stats.begin (); it != stats.end (); ++it )
        {
          outFile << (*it) * 1e-9 << "\t";
        }
      stats = GetUlPduSizeStats (*imsi);
      for( std::vector<double>::iterator it = stats.begin (); it != stats.end (); ++it )
        {
          outFile << (*it) << "\t";
        }
      outFile << std::endl;
    }

  outFile.close ();
}

void
RlcStatsCalculator::WriteDlResults (std::ofstream& outFile)
{
  // Get the unique IMSI list
  std::vector<uint64_t> imsiVector;
  for (uint32Map::iterator it = m_ulTxPackets.begin(); it != m_ulTxPackets.end(); ++it)
    {
      if (find (imsiVector.begin (), imsiVector.end (),  (*it).first ) == imsiVector.end () )
        {
          imsiVector.push_back ((*it).first);
        }
    }

  Time endTime = m_startTime + m_epochDuration;
  for (std::vector<uint64_t>::iterator imsi = imsiVector.begin(); imsi != imsiVector.end(); ++imsi)
    {
      outFile << m_startTime.GetNanoSeconds () / 1.0e9 << "\t";
      outFile << endTime.GetNanoSeconds() / 1.0e9      << "\t";
      outFile << (*imsi)                               << "\t";
      outFile << m_flowId[*imsi].m_rnti                << "\t";
      outFile << (uint32_t) m_flowId[*imsi].m_lcId     << "\t";
      outFile << GetDlTxPackets (*imsi)                << "\t";
      outFile << GetDlTxData (*imsi)                   << "\t";
      outFile << GetDlRxPackets (*imsi)                << "\t";
      outFile << GetDlRxData (*imsi)                   << "\t";
      std::vector<double> stats = GetDlDelayStats (*imsi);
      for( std::vector<double>::iterator it = stats.begin (); it != stats.end (); ++it )
        {
          outFile << (*it) * 1e-9 << "\t";
        }
      stats = GetDlPduSizeStats (*imsi);
      for( std::vector<double>::iterator it = stats.begin (); it != stats.end (); ++it )
        {
          outFile << (*it) << "\t";
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
   m_ulTxData.erase (m_ulTxData.begin (), m_ulTxData.end () );
   m_ulDelay.erase (m_ulDelay.begin (), m_ulDelay.end () );
   m_ulPduSize.erase (m_ulPduSize.begin (), m_ulPduSize.end () );

   m_dlTxPackets.erase (m_dlTxPackets.begin (), m_dlTxPackets.end () );
   m_dlRxPackets.erase (m_dlRxPackets.begin (), m_dlRxPackets.end () );
   m_dlRxData.erase (m_dlRxData.begin (), m_dlRxData.end () );
   m_dlTxData.erase (m_dlTxData.begin (), m_dlTxData.end () );
   m_dlDelay.erase (m_dlDelay.begin (), m_dlDelay.end () );
   m_dlPduSize.erase (m_dlPduSize.begin (), m_dlPduSize.end () );
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
RlcStatsCalculator::GetUlTxPackets (uint64_t imsi)
{
  return m_ulTxPackets[imsi];
}

uint32_t
RlcStatsCalculator::GetUlRxPackets (uint64_t imsi)
{
  return m_ulRxPackets[imsi];
}

uint64_t
RlcStatsCalculator::GetUlTxData (uint64_t imsi)
{
  return m_ulTxData[imsi];
}

uint64_t
RlcStatsCalculator::GetUlRxData (uint64_t imsi)
{
  return m_ulRxData[imsi];
}

double
RlcStatsCalculator::GetUlDelay (uint64_t imsi)
{
  uint64StatsMap::iterator it = m_ulDelay.find (imsi);
  if ( it == m_ulDelay.end () )
    {
      NS_LOG_ERROR("UL delay for " << imsi << " not found");
      return 0;

    }
  return m_ulDelay[imsi]->getMean ();
}

std::vector<double>
RlcStatsCalculator::GetUlDelayStats (uint64_t imsi)
{
  std::vector<double> stats;
  uint64StatsMap::iterator it = m_ulDelay.find (imsi);
  if ( it == m_ulDelay.end () )
    {
      NS_LOG_ERROR("UL delay for " << imsi << " not found");
      return stats;

    }
  stats.push_back(m_ulDelay[imsi]->getMean ());
  stats.push_back(m_ulDelay[imsi]->getStddev ());
  stats.push_back(m_ulDelay[imsi]->getMin ());
  stats.push_back(m_ulDelay[imsi]->getMax ());
  return stats;
}

std::vector<double>
RlcStatsCalculator::GetUlPduSizeStats (uint64_t imsi)
{
  std::vector<double> stats;
  uint32StatsMap::iterator it = m_ulPduSize.find (imsi);
  if ( it == m_ulPduSize.end () )
    {
      NS_LOG_ERROR("UL PDU Size for " << imsi << " not found");
      return stats;

    }
  stats.push_back (m_ulPduSize[imsi]->getMean ());
  stats.push_back (m_ulPduSize[imsi]->getStddev ());
  stats.push_back (m_ulPduSize[imsi]->getMin ());
  stats.push_back (m_ulPduSize[imsi]->getMax ());
  return stats;
}

uint32_t
RlcStatsCalculator::GetDlTxPackets (uint64_t imsi)
{
  return m_dlTxPackets[imsi];
}

uint32_t
RlcStatsCalculator::GetDlRxPackets (uint64_t imsi)
{
  return m_dlRxPackets[imsi];
}

uint64_t
RlcStatsCalculator::GetDlTxData (uint64_t imsi)
{
  return m_dlTxData[imsi];
}

uint64_t
RlcStatsCalculator::GetDlRxData (uint64_t imsi)
{
  return m_dlRxData[imsi];
}

double
RlcStatsCalculator::GetDlDelay (uint64_t imsi)
{
  uint64StatsMap::iterator it = m_dlDelay.find (imsi);
  if ( it == m_dlDelay.end () )
    {
      NS_LOG_ERROR("DL delay for " << imsi << " not found");
      return 0;
    }
  return m_dlDelay[imsi]->getMean ();
}

std::vector<double>
RlcStatsCalculator::GetDlDelayStats (uint64_t imsi)
{
  std::vector<double> stats;
  uint64StatsMap::iterator it = m_dlDelay.find (imsi);
  if ( it == m_dlDelay.end () )
    {

      NS_LOG_ERROR("DL delay for " << imsi  << " not found");
      return stats;

    }
  stats.push_back(m_dlDelay[imsi]->getMean ());
  stats.push_back(m_dlDelay[imsi]->getStddev ());
  stats.push_back(m_dlDelay[imsi]->getMin ());
  stats.push_back(m_dlDelay[imsi]->getMax ());
  return stats;
}

std::vector<double>
RlcStatsCalculator::GetDlPduSizeStats (uint64_t imsi)
{
  std::vector<double> stats;
  uint32StatsMap::iterator it = m_dlPduSize.find (imsi);
  if ( it == m_dlPduSize.end () )
    {

      NS_LOG_ERROR("DL delay for " << imsi << " not found");
      return stats;

    }
  stats.push_back(m_dlPduSize[imsi]->getMean ());
  stats.push_back(m_dlPduSize[imsi]->getStddev ());
  stats.push_back(m_dlPduSize[imsi]->getMin ());
  stats.push_back(m_dlPduSize[imsi]->getMax ());
  return stats;
}

} // namespace ns3
