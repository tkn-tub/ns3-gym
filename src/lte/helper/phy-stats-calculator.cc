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

#include "phy-stats-calculator.h"
#include "ns3/string.h"
#include <ns3/simulator.h>
#include <ns3/log.h>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("PhyStatsCalculator");

NS_OBJECT_ENSURE_REGISTERED (PhyStatsCalculator);

PhyStatsCalculator::PhyStatsCalculator ()
  :  m_RsrpSinrFirstWrite (true),
    m_UeSinrFirstWrite (true),
    m_InterferenceFirstWrite (true)
{
  NS_LOG_FUNCTION (this);

}

PhyStatsCalculator::~PhyStatsCalculator ()
{
  NS_LOG_FUNCTION (this);
}

TypeId
PhyStatsCalculator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PhyStatsCalculator")
    .SetParent<LteStatsCalculator> ()
    .AddConstructor<PhyStatsCalculator> ()
    .AddAttribute ("DlRsrpSinrFilename",
                   "Name of the file where the RSRP/SINR statistics will be saved.",
                   StringValue ("DlRsrpSinrStats.txt"),
                   MakeStringAccessor (&PhyStatsCalculator::SetCurrentCellRsrpSinrFilename),
                   MakeStringChecker ())
    .AddAttribute ("UlSinrFilename",
                   "Name of the file where the UE SINR statistics will be saved.",
                   StringValue ("UlSinrStats.txt"),
                   MakeStringAccessor (&PhyStatsCalculator::SetUeSinrFilename),
                   MakeStringChecker ())
    .AddAttribute ("UlInterferenceFilename",
                   "Name of the file where the interference statistics will be saved.",
                   StringValue ("UlInterferenceStats.txt"),
                   MakeStringAccessor (&PhyStatsCalculator::SetInterferenceFilename),
                   MakeStringChecker ())
  ;
  return tid;
}

void
PhyStatsCalculator::SetCurrentCellRsrpSinrFilename (std::string filename)
{
  m_RsrpSinrFilename = filename;
}

std::string
PhyStatsCalculator::GetCurrentCellRsrpSinrFilename (void)
{
  return m_RsrpSinrFilename;
}

void
PhyStatsCalculator::SetUeSinrFilename (std::string filename)
{
  m_ueSinrFilename = filename;
}

std::string
PhyStatsCalculator::GetUeSinrFilename (void)
{
  return m_ueSinrFilename;
}

void
PhyStatsCalculator::SetInterferenceFilename (std::string filename)
{
  m_interferenceFilename = filename;
}

std::string
PhyStatsCalculator::GetInterferenceFilename (void)
{
  return m_interferenceFilename;
}



void
PhyStatsCalculator::ReportCurrentCellRsrpSinr (uint16_t cellId, uint64_t imsi, uint16_t rnti,
                                               double rsrp, double sinr)
{
  NS_LOG_FUNCTION (this << cellId <<  imsi << rnti  << rsrp << sinr);
  NS_LOG_INFO ("Write RSRP/SINR Phy Stats in " << GetCurrentCellRsrpSinrFilename ().c_str ());

  std::ofstream outFile;
  if ( m_RsrpSinrFirstWrite == true )
    {
      outFile.open (GetCurrentCellRsrpSinrFilename ().c_str ());
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << GetCurrentCellRsrpSinrFilename ().c_str ());
          return;
        }
      m_RsrpSinrFirstWrite = false;
      outFile << "% time\tcellId\tIMSI\tRNTI\trsrp\tsinr";
      outFile << std::endl;
    }
  else
    {
      outFile.open (GetCurrentCellRsrpSinrFilename ().c_str (),  std::ios_base::app);
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << GetCurrentCellRsrpSinrFilename ().c_str ());
          return;
        }
    }

  outFile << Simulator::Now ().GetNanoSeconds () / (double) 1e9 << "\t";
  outFile << cellId << "\t";
  outFile << imsi << "\t";
  outFile << rnti << "\t";
  outFile << rsrp << "\t";
  outFile << sinr << std::endl;
  outFile.close ();
}

void
PhyStatsCalculator::ReportUeSinr (uint16_t cellId, uint64_t imsi, uint16_t rnti,  double sinrLinear)
{
  NS_LOG_FUNCTION (this << cellId <<  imsi << rnti  << sinrLinear);
  NS_LOG_INFO ("Write SINR Linear Phy Stats in " << GetUeSinrFilename ().c_str ());

  std::ofstream outFile;
  if ( m_UeSinrFirstWrite == true )
    {
      outFile.open (GetUeSinrFilename ().c_str ());
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << GetUeSinrFilename ().c_str ());
          return;
        }
      m_UeSinrFirstWrite = false;
      outFile << "% time\tcellId\tIMSI\tRNTI\tsinrLinear";
      outFile << std::endl;
    }
  else
    {
      outFile.open (GetUeSinrFilename ().c_str (),  std::ios_base::app);
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << GetUeSinrFilename ().c_str ());
          return;
        }
    }

  outFile << Simulator::Now ().GetNanoSeconds () / (double) 1e9 << "\t";
  outFile << cellId << "\t";
  outFile << imsi << "\t";
  outFile << rnti << "\t";
  outFile << sinrLinear << std::endl;
  outFile.close ();
}

void
PhyStatsCalculator::ReportInterference (uint16_t cellId, Ptr<SpectrumValue> interference)
{
  NS_LOG_FUNCTION (this << cellId <<  interference);
  NS_LOG_INFO ("Write Interference Phy Stats in " << GetInterferenceFilename ().c_str ());

  std::ofstream outFile;
  if ( m_InterferenceFirstWrite == true )
    {
      outFile.open (GetInterferenceFilename ().c_str ());
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << GetInterferenceFilename ().c_str ());
          return;
        }
      m_InterferenceFirstWrite = false;
      outFile << "% time\tcellId\tInterference";
      outFile << std::endl;
    }
  else
    {
      outFile.open (GetInterferenceFilename ().c_str (),  std::ios_base::app);
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << GetInterferenceFilename ().c_str ());
          return;
        }
    }

  outFile << Simulator::Now ().GetNanoSeconds () / (double) 1e9 << "\t";
  outFile << cellId << "\t";
  outFile << *interference;
  outFile.close ();
}

} // namespace ns3
