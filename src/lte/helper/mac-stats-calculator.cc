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

#include "mac-stats-calculator.h"
#include "ns3/string.h"
#include <ns3/simulator.h>
#include <ns3/log.h>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("MacStatsCalculator");

NS_OBJECT_ENSURE_REGISTERED (MacStatsCalculator);

MacStatsCalculator::MacStatsCalculator ()
  : m_dlOutputFilename (""),
    m_dlFirstWrite (true),
    m_ulOutputFilename (""),
    m_ulFirstWrite (true)
{
  NS_LOG_FUNCTION (this);

}

MacStatsCalculator::~MacStatsCalculator ()
{
  NS_LOG_FUNCTION (this);
}

TypeId
MacStatsCalculator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MacStatsCalculator")
    .SetParent<Object> ()
    .AddConstructor<MacStatsCalculator> ()
    .AddAttribute ("DlOutputFilename",
                   "Name of the file where the downlink results will be saved.",
                   StringValue ("DlMacStats.csv"),
                   MakeStringAccessor (&MacStatsCalculator::SetDlOutputFilename),
                   MakeStringChecker ())
    .AddAttribute ("UlOutputFilename",
                   "Name of the file where the uplink results will be saved.",
                   StringValue ("UlMacStats.csv"),
                   MakeStringAccessor (&MacStatsCalculator::SetUlOutputFilename),
                   MakeStringChecker ())
  ;
  return tid;
}

void
MacStatsCalculator::SetUlOutputFilename (std::string outputFilename)
{
  m_ulOutputFilename = outputFilename;
}

void
MacStatsCalculator::SetDlOutputFilename (std::string outputFilename)
{
  m_dlOutputFilename = outputFilename;
}

void
MacStatsCalculator::DlScheduling (uint16_t cellId, uint64_t imsi, uint32_t frameNo, uint32_t subframeNo,
                                  uint16_t rnti, uint8_t mcsTb1, uint16_t sizeTb1, uint8_t mcsTb2, uint16_t sizeTb2)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_INFO ("Write DL Mac Stats in " << m_dlOutputFilename.c_str ());

  std::ofstream outFile;
  if ( m_dlFirstWrite == true )
    {
      outFile.open (m_dlOutputFilename.c_str ());
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << m_dlOutputFilename.c_str ());
          return;
        }
      m_dlFirstWrite = false;
      outFile << "# time\tcellId\tIMSI\tframe\tsframe\tRNTI\tmcsTb1\tsizeTb1\tmcsTb2\tsizeTb2";
      outFile << std::endl;
    }
  else
    {
      outFile.open (m_dlOutputFilename.c_str (),  std::ios_base::app);
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << m_dlOutputFilename.c_str ());
          return;
        }
    }

  outFile << Simulator::Now ().GetNanoSeconds () / (double) 1e9 << "\t";
  outFile << (uint32_t) cellId << "\t";
  outFile << imsi << "\t";
  outFile << frameNo << "\t";
  outFile << subframeNo << "\t";
  outFile << rnti << "\t";
  outFile << (uint32_t) mcsTb1 << "\t";
  outFile << sizeTb1 << "\t";
  outFile << (uint32_t) mcsTb2 << "\t";
  outFile << sizeTb2 << std::endl;
  outFile.close ();
}

void
MacStatsCalculator::UlScheduling (uint16_t cellId, uint64_t imsi, uint32_t frameNo,
                                  uint32_t subframeNo, uint16_t rnti,uint8_t mcs, uint16_t size)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_INFO ("Write UL Mac Stats in " << m_ulOutputFilename.c_str ());

  std::ofstream outFile;
  if ( m_ulFirstWrite == true )
    {
      outFile.open (m_ulOutputFilename.c_str ());
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << m_ulOutputFilename.c_str ());
          return;
        }
      m_ulFirstWrite = false;
      outFile << "# time\tcellId\tIMSI\tframe\tsframe\tRNTI\tmcs\tsize";
      outFile << std::endl;
    }
  else
    {
      outFile.open (m_ulOutputFilename.c_str (),  std::ios_base::app);
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << m_ulOutputFilename.c_str ());
          return;
        }
    }

  outFile << Simulator::Now ().GetNanoSeconds () / (double) 1e9 << "\t";
  outFile << (uint32_t) cellId << "\t";
  outFile << imsi << "\t";
  outFile << frameNo << "\t";
  outFile << subframeNo << "\t";
  outFile << rnti << "\t";
  outFile << (uint32_t) mcs << "\t";
  outFile << size << std::endl;
  outFile.close ();
}


} // namespace ns3
