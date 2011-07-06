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

ImsiLcidPair::ImsiLcidPair ()
{
}

ImsiLcidPair::ImsiLcidPair (const uint64_t a, const uint8_t b)
  : m_imsi (a),
    m_lcId (b)
{
}

bool
operator == (const ImsiLcidPair &a, const ImsiLcidPair &b)
{
  return ((a.m_imsi == b.m_imsi) && (a.m_lcId == b.m_lcId));
}

bool
operator < (const ImsiLcidPair& a, const ImsiLcidPair& b)
{
  return ((a.m_imsi < b.m_imsi) || ((a.m_imsi == b.m_imsi) && (a.m_lcId
                                                               < b.m_lcId)));
}

NS_LOG_COMPONENT_DEFINE ("RlcStatsCalculator");

NS_OBJECT_ENSURE_REGISTERED (RlcStatsCalculator);

RlcStatsCalculator::RlcStatsCalculator ()
  : m_dlOutputFilename (""),
    m_ulOutputFilename (""),
    m_firstWrite (true)
{
  NS_LOG_FUNCTION (this);

}

RlcStatsCalculator::~RlcStatsCalculator ()
{
  NS_LOG_FUNCTION (this);
  ShowResults ();
}

TypeId
RlcStatsCalculator::GetTypeId (void)
{
  static TypeId tid =
    TypeId ("ns3::RlcStatsCalculator")
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
                   TimeValue (Seconds (0.)),
                   MakeTimeAccessor (&RlcStatsCalculator::m_startTime),
                   MakeTimeChecker ())
    .AddAttribute ("EpochDuration",
                   "Epoch duration.", 
                   TimeValue (Seconds (0.25)), 
                   MakeTimeAccessor (&RlcStatsCalculator::m_epochDuration), 
                   MakeTimeChecker ());
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
RlcStatsCalculator::UlTxPdu (uint64_t imsi, uint16_t rnti,
                             uint8_t lcid, uint32_t packetSize)
{
  NS_LOG_FUNCTION (this << "UlTxPDU" << imsi << rnti << (uint32_t) lcid << packetSize);
  ImsiLcidPair p (imsi, lcid);
  if (Simulator::Now () > m_startTime)
    {
      m_flowId[p] = LteFlowId_t (rnti, lcid);
      m_ulTxPackets[p]++;
      m_ulTxData[p] += packetSize;
    }
  CheckEpoch ();
}

void
RlcStatsCalculator::DlTxPdu (uint16_t cellId, uint64_t imsi, uint16_t rnti,
                             uint8_t lcid, uint32_t packetSize)
{
  NS_LOG_FUNCTION (this << "DlTxPDU" << imsi << rnti << (uint32_t) lcid << packetSize);
  ImsiLcidPair p (imsi, lcid);
  bool forceEpoch = false;
  if (Simulator::Now () > m_startTime)
    {

      // If the UE hands off to another cell, restart the epoch automatically
      if (m_dlCellId[p] != 0 && m_dlCellId[p] != cellId)
        {
          forceEpoch = true;
        }
      else
        {
          m_dlCellId[p] = cellId;
        }
      m_flowId[p] = LteFlowId_t (rnti, lcid);
      m_dlTxPackets[p]++;
      m_dlTxData[p] += packetSize;
    }
  CheckEpoch (forceEpoch);
}

void
RlcStatsCalculator::UlRxPdu (uint16_t cellId, uint64_t imsi, uint16_t rnti,
                             uint8_t lcid, uint32_t packetSize, uint64_t delay)
{
  NS_LOG_FUNCTION (this << "UlRxPDU" << imsi << rnti << (uint32_t) lcid << packetSize << delay);
  ImsiLcidPair p (imsi, lcid);
  bool forceEpoch = false;

  if (Simulator::Now () > m_startTime)
    {
      // If the UE hands off to another cell, restart the epoch automatically
      if (m_ulCellId[p] != 0 && m_ulCellId[p] != cellId)
        {
          forceEpoch = true;
        }
      else
        {
          m_ulCellId[p] = cellId;
        }

      m_ulRxPackets[p]++;
      m_ulRxData[p] += packetSize;

      Uint64StatsMap::iterator it = m_ulDelay.find (p);
      if (it == m_ulDelay.end ())
        {
          NS_LOG_DEBUG (this << " Creating UL stats calculators for IMSI " << p.m_imsi << " and LCI " << (uint32_t) p.m_lcId );
          m_ulDelay[p] = CreateObject<MinMaxAvgTotalCalculator<uint64_t> > ();
          m_ulPduSize[p] = CreateObject<MinMaxAvgTotalCalculator<uint32_t> > ();
        }
      m_ulDelay[p]->Update (delay);
      m_ulPduSize[p]->Update (packetSize);
    }

  CheckEpoch (forceEpoch);
}

void
RlcStatsCalculator::DlRxPdu (uint64_t imsi, uint16_t rnti,
                             uint8_t lcid, uint32_t packetSize, uint64_t delay)
{
  NS_LOG_FUNCTION (this << "DlRxPDU" << imsi << rnti << (uint32_t) lcid << packetSize << delay);
  ImsiLcidPair p (imsi, lcid);
  if (Simulator::Now () > m_startTime)
    {
      m_dlRxPackets[p]++;
      m_dlRxData[p] += packetSize;

      Uint64StatsMap::iterator it = m_dlDelay.find (p);
      if (it == m_dlDelay.end ())
        {
          NS_LOG_DEBUG (this << " Creating DL stats calculators for IMSI " << p.m_imsi << " and LCI " << (uint32_t) p.m_lcId );
          m_dlDelay[p] = CreateObject<MinMaxAvgTotalCalculator<uint64_t> > ();
          m_dlPduSize[p] = CreateObject<MinMaxAvgTotalCalculator<uint32_t> > ();
        }
      m_dlDelay[p]->Update (delay);
      m_dlPduSize[p]->Update (packetSize);
    }
  CheckEpoch ();
}

void
RlcStatsCalculator::ShowResults (void)
{

  NS_LOG_FUNCTION (this << m_ulOutputFilename.c_str () << m_dlOutputFilename.c_str () );
  NS_LOG_INFO ("Write Rlc Stats in " << m_ulOutputFilename.c_str () <<
               " and in " << m_dlOutputFilename.c_str ());

  std::ofstream ulOutFile;
  std::ofstream dlOutFile;

  if (m_firstWrite == true)
    {
      ulOutFile.open (m_ulOutputFilename.c_str ());
      if (!ulOutFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << m_ulOutputFilename.c_str ());
          return;
        }

      dlOutFile.open (m_dlOutputFilename.c_str ());
      if (!dlOutFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << m_dlOutputFilename.c_str ());
          return;
        }
      m_firstWrite = false;
      ulOutFile
      << "% start\tend\tCellId\tIMSI\tRNTI\tLCID\tnTxPDUs\tTxBytes\tnRxPDUs\tRxBytes\t";
      ulOutFile << "delay\tstdDev\tmin\tmax\t";
      ulOutFile << "PduSize\tstdDev\tmin\tmax";
      ulOutFile << std::endl;
      dlOutFile
      << "% start\tend\tCellId\tIMSI\tRNTI\tLCID\tnTxPDUs\tTxBytes\tnRxPDUs\tRxBytes\t";
      dlOutFile << "delay\tstdDev\tmin\tmax\t";
      dlOutFile << "PduSize\tstdDev\tmin\tmax";
      dlOutFile << std::endl;
    }
  else
    {
      ulOutFile.open (m_ulOutputFilename.c_str (), std::ios_base::app);
      if (!ulOutFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << m_ulOutputFilename.c_str ());
          return;
        }

      dlOutFile.open (m_dlOutputFilename.c_str (), std::ios_base::app);
      if (!dlOutFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << m_dlOutputFilename.c_str ());
          return;
        }
    }

  WriteUlResults (ulOutFile);
  WriteDlResults (dlOutFile);

}

void
RlcStatsCalculator::WriteUlResults (std::ofstream& outFile)
{
  // Get the unique IMSI / LCID list

  std::vector<ImsiLcidPair> pairVector;
  for (Uint32Map::iterator it = m_ulTxPackets.begin (); it
       != m_ulTxPackets.end (); ++it)
    {
      if (find (pairVector.begin (), pairVector.end (), (*it).first)
          == pairVector.end ())
        {
          pairVector.push_back ((*it).first);
        }
    }

  Time endTime = m_startTime + m_epochDuration;
  for (std::vector<ImsiLcidPair>::iterator it = pairVector.begin (); it
       != pairVector.end (); ++it)
    {
      ImsiLcidPair p = *it;
      outFile << m_startTime.GetNanoSeconds () / 1.0e9 << "\t";
      outFile << endTime.GetNanoSeconds () / 1.0e9 << "\t";
      outFile << GetUlCellId (p.m_imsi, p.m_lcId) << "\t";
      outFile << p.m_imsi << "\t";
      outFile << m_flowId[p].m_rnti << "\t";
      outFile << (uint32_t) m_flowId[p].m_lcId << "\t";
      outFile << GetUlTxPackets (p.m_imsi, p.m_lcId) << "\t";
      outFile << GetUlTxData (p.m_imsi, p.m_lcId) << "\t";
      outFile << GetUlRxPackets (p.m_imsi, p.m_lcId) << "\t";
      outFile << GetUlRxData (p.m_imsi, p.m_lcId) << "\t";
      std::vector<double> stats = GetUlDelayStats (p.m_imsi, p.m_lcId);
      for (std::vector<double>::iterator it = stats.begin (); it != stats.end (); ++it)
        {
          outFile << (*it) * 1e-9 << "\t";
        }
      stats = GetUlPduSizeStats (p.m_imsi, p.m_lcId);
      for (std::vector<double>::iterator it = stats.begin (); it != stats.end (); ++it)
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
  std::vector<ImsiLcidPair> pairVector;
  for (Uint32Map::iterator it = m_dlTxPackets.begin (); it
       != m_dlTxPackets.end (); ++it)
    {
      if (find (pairVector.begin (), pairVector.end (), (*it).first)
          == pairVector.end ())
        {
          pairVector.push_back ((*it).first);
        }
    }

  Time endTime = m_startTime + m_epochDuration;
  for (std::vector<ImsiLcidPair>::iterator pair = pairVector.begin (); pair
       != pairVector.end (); ++pair)
    {
      ImsiLcidPair p = *pair;
      outFile << m_startTime.GetNanoSeconds () / 1.0e9 << "\t";
      outFile << endTime.GetNanoSeconds () / 1.0e9 << "\t";
      outFile << GetDlCellId (p.m_imsi, p.m_lcId) << "\t";
      outFile << p.m_imsi << "\t";
      outFile << m_flowId[p].m_rnti << "\t";
      outFile << (uint32_t) m_flowId[p].m_lcId << "\t";
      outFile << GetDlTxPackets (p.m_imsi, p.m_lcId) << "\t";
      outFile << GetDlTxData (p.m_imsi, p.m_lcId) << "\t";
      outFile << GetDlRxPackets (p.m_imsi, p.m_lcId) << "\t";
      outFile << GetDlRxData (p.m_imsi, p.m_lcId) << "\t";
      std::vector<double> stats = GetDlDelayStats (p.m_imsi, p.m_lcId);
      for (std::vector<double>::iterator it = stats.begin (); it != stats.end (); ++it)
        {
          outFile << (*it) * 1e-9 << "\t";
        }
      stats = GetDlPduSizeStats (p.m_imsi, p.m_lcId);
      for (std::vector<double>::iterator it = stats.begin (); it != stats.end (); ++it)
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
  m_ulTxPackets.erase (m_ulTxPackets.begin (), m_ulTxPackets.end ());
  m_ulRxPackets.erase (m_ulRxPackets.begin (), m_ulRxPackets.end ());
  m_ulRxData.erase (m_ulRxData.begin (), m_ulRxData.end ());
  m_ulTxData.erase (m_ulTxData.begin (), m_ulTxData.end ());
  m_ulDelay.erase (m_ulDelay.begin (), m_ulDelay.end ());
  m_ulPduSize.erase (m_ulPduSize.begin (), m_ulPduSize.end ());

  m_dlTxPackets.erase (m_dlTxPackets.begin (), m_dlTxPackets.end ());
  m_dlRxPackets.erase (m_dlRxPackets.begin (), m_dlRxPackets.end ());
  m_dlRxData.erase (m_dlRxData.begin (), m_dlRxData.end ());
  m_dlTxData.erase (m_dlTxData.begin (), m_dlTxData.end ());
  m_dlDelay.erase (m_dlDelay.begin (), m_dlDelay.end ());
  m_dlPduSize.erase (m_dlPduSize.begin (), m_dlPduSize.end ());
}

void
RlcStatsCalculator::CheckEpoch (bool forceEpoch)
{
  if (Simulator::Now () > m_startTime + m_epochDuration /*|| forceEpoch == true*/)
    {
      ShowResults ();
      ResetResults ();
      StartEpoch ();
    }

}

void
RlcStatsCalculator::StartEpoch (void)
{
  m_startTime += m_epochDuration;
}

uint32_t
RlcStatsCalculator::GetUlTxPackets (uint64_t imsi, uint8_t lcid)
{
  ImsiLcidPair p (imsi, lcid);
  return m_ulTxPackets[p];
}

uint32_t
RlcStatsCalculator::GetUlRxPackets (uint64_t imsi, uint8_t lcid)
{
  ImsiLcidPair p (imsi, lcid);
  return m_ulRxPackets[p];
}

uint64_t
RlcStatsCalculator::GetUlTxData (uint64_t imsi, uint8_t lcid)
{
  ImsiLcidPair p (imsi, lcid);
  return m_ulTxData[p];
}

uint64_t
RlcStatsCalculator::GetUlRxData (uint64_t imsi, uint8_t lcid)
{
  ImsiLcidPair p (imsi, lcid);
  return m_ulRxData[p];
}

double
RlcStatsCalculator::GetUlDelay (uint64_t imsi, uint8_t lcid)
{
  ImsiLcidPair p (imsi, lcid);
  Uint64StatsMap::iterator it = m_ulDelay.find (p);
  if (it == m_ulDelay.end ())
    {
      NS_LOG_ERROR ("UL delay for " << imsi << " - " << lcid << " not found");
      return 0;

    }
  return m_ulDelay[p]->getMean ();
}

std::vector<double>
RlcStatsCalculator::GetUlDelayStats (uint64_t imsi, uint8_t lcid)
{
  ImsiLcidPair p (imsi, lcid);
  std::vector<double> stats;
  Uint64StatsMap::iterator it = m_ulDelay.find (p);
  if (it == m_ulDelay.end ())
    {
      NS_LOG_ERROR ("UL delay for " << imsi << " - " << lcid << " not found");
      return stats;

    }
  stats.push_back (m_ulDelay[p]->getMean ());
  stats.push_back (m_ulDelay[p]->getStddev ());
  stats.push_back (m_ulDelay[p]->getMin ());
  stats.push_back (m_ulDelay[p]->getMax ());
  return stats;
}

std::vector<double>
RlcStatsCalculator::GetUlPduSizeStats (uint64_t imsi, uint8_t lcid)
{
  ImsiLcidPair p (imsi, lcid);
  std::vector<double> stats;
  Uint32StatsMap::iterator it = m_ulPduSize.find (p);
  if (it == m_ulPduSize.end ())
    {
      NS_LOG_ERROR ("UL PDU Size for " << imsi << " - " << lcid << " not found");
      return stats;

    }
  stats.push_back (m_ulPduSize[p]->getMean ());
  stats.push_back (m_ulPduSize[p]->getStddev ());
  stats.push_back (m_ulPduSize[p]->getMin ());
  stats.push_back (m_ulPduSize[p]->getMax ());
  return stats;
}

uint32_t
RlcStatsCalculator::GetDlTxPackets (uint64_t imsi, uint8_t lcid)
{
  ImsiLcidPair p (imsi, lcid);
  return m_dlTxPackets[p];
}

uint32_t
RlcStatsCalculator::GetDlRxPackets (uint64_t imsi, uint8_t lcid)
{
  ImsiLcidPair p (imsi, lcid);
  return m_dlRxPackets[p];
}

uint64_t
RlcStatsCalculator::GetDlTxData (uint64_t imsi, uint8_t lcid)
{
  ImsiLcidPair p (imsi, lcid);
  return m_dlTxData[p];
}

uint64_t
RlcStatsCalculator::GetDlRxData (uint64_t imsi, uint8_t lcid)
{
  ImsiLcidPair p (imsi, lcid);
  return m_dlRxData[p];
}

uint32_t
RlcStatsCalculator::GetUlCellId (uint64_t imsi, uint8_t lcid)
{
  ImsiLcidPair p (imsi, lcid);
  return m_ulCellId[p];
}

uint32_t
RlcStatsCalculator::GetDlCellId (uint64_t imsi, uint8_t lcid)
{
  ImsiLcidPair p (imsi, lcid);
  return m_dlCellId[p];
}

double
RlcStatsCalculator::GetDlDelay (uint64_t imsi, uint8_t lcid)
{
  ImsiLcidPair p (imsi, lcid);
  Uint64StatsMap::iterator it = m_dlDelay.find (p);
  if (it == m_dlDelay.end ())
    {
      NS_LOG_ERROR ("DL delay for " << imsi << " not found");
      return 0;
    }
  return m_dlDelay[p]->getMean ();
}

std::vector<double>
RlcStatsCalculator::GetDlDelayStats (uint64_t imsi, uint8_t lcid)
{
  ImsiLcidPair p (imsi, lcid);
  std::vector<double> stats;
  Uint64StatsMap::iterator it = m_dlDelay.find (p);
  if (it == m_dlDelay.end ())
    {

      NS_LOG_ERROR ("DL delay for " << imsi << " not found");
      return stats;

    }
  stats.push_back (m_dlDelay[p]->getMean ());
  stats.push_back (m_dlDelay[p]->getStddev ());
  stats.push_back (m_dlDelay[p]->getMin ());
  stats.push_back (m_dlDelay[p]->getMax ());
  return stats;
}

std::vector<double>
RlcStatsCalculator::GetDlPduSizeStats (uint64_t imsi, uint8_t lcid)
{
  ImsiLcidPair p (imsi, lcid);
  std::vector<double> stats;
  Uint32StatsMap::iterator it = m_dlPduSize.find (p);
  if (it == m_dlPduSize.end ())
    {

      NS_LOG_ERROR ("DL delay for " << imsi << " not found");
      return stats;

    }
  stats.push_back (m_dlPduSize[p]->getMean ());
  stats.push_back (m_dlPduSize[p]->getStddev ());
  stats.push_back (m_dlPduSize[p]->getMin ());
  stats.push_back (m_dlPduSize[p]->getMax ());
  return stats;
}

} // namespace ns3
