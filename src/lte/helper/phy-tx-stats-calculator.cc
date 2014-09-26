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
 * modified by: Marco Miozzo <mmiozzo@cttc.es>
 *        Convert MacStatsCalculator in PhyTxStatsCalculator
 */

#include "phy-tx-stats-calculator.h"
#include "ns3/string.h"
#include <ns3/simulator.h>
#include <ns3/log.h>

NS_LOG_COMPONENT_DEFINE ("PhyTxStatsCalculator");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (PhyTxStatsCalculator);

PhyTxStatsCalculator::PhyTxStatsCalculator ()
  : m_dlTxFirstWrite (true),
    m_ulTxFirstWrite (true)
{
  NS_LOG_FUNCTION (this);

}

PhyTxStatsCalculator::~PhyTxStatsCalculator ()
{
  NS_LOG_FUNCTION (this);
}

TypeId
PhyTxStatsCalculator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PhyTxStatsCalculator")
    .SetParent<LteStatsCalculator> ()
    .AddConstructor<PhyTxStatsCalculator> ()
    .AddAttribute ("DlTxOutputFilename",
                   "Name of the file where the downlink results will be saved.",
                   StringValue ("DlTxPhyStats.txt"),
                   MakeStringAccessor (&PhyTxStatsCalculator::SetDlTxOutputFilename),
                   MakeStringChecker ())
    .AddAttribute ("UlTxOutputFilename",
                   "Name of the file where the uplink results will be saved.",
                   StringValue ("UlTxPhyStats.txt"),
                   MakeStringAccessor (&PhyTxStatsCalculator::SetUlTxOutputFilename),
                   MakeStringChecker ())
  ;
  return tid;
}

void
PhyTxStatsCalculator::SetUlTxOutputFilename (std::string outputFilename)
{
  LteStatsCalculator::SetUlOutputFilename (outputFilename);
}

std::string
PhyTxStatsCalculator::GetUlTxOutputFilename (void)
{
  return LteStatsCalculator::GetUlOutputFilename ();
}

void
PhyTxStatsCalculator::SetDlTxOutputFilename (std::string outputFilename)
{
  LteStatsCalculator::SetDlOutputFilename (outputFilename);
}

std::string
PhyTxStatsCalculator::GetDlTxOutputFilename (void)
{
  return LteStatsCalculator::GetDlOutputFilename ();
}

void
PhyTxStatsCalculator::DlPhyTransmission (PhyTransmissionStatParameters params)
{
  NS_LOG_FUNCTION (this << params.m_cellId << params.m_imsi << params.m_timestamp << params.m_rnti << params.m_layer << params.m_mcs << params.m_size << params.m_rv << params.m_ndi);
  NS_LOG_INFO ("Write DL Tx Phy Stats in " << GetDlTxOutputFilename ().c_str ());

  std::ofstream outFile;
  if ( m_dlTxFirstWrite == true )
    {
      outFile.open (GetDlOutputFilename ().c_str ());
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << GetDlTxOutputFilename ().c_str ());
          return;
        }
      m_dlTxFirstWrite = false;
      //outFile << "% time\tcellId\tIMSI\tRNTI\tlayer\tmcs\tsize\trv\tndi"; // txMode is not available at dl tx side
      outFile << "% time\tcellId\tIMSI\tRNTI\tlayer\tmcs\tsize\trv\tndi";
      outFile << std::endl;
    }
  else
    {
      outFile.open (GetDlTxOutputFilename ().c_str (),  std::ios_base::app);
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << GetDlTxOutputFilename ().c_str ());
          return;
        }
    }

//   outFile << Simulator::Now ().GetNanoSeconds () / (double) 1e9 << "\t";
  outFile << params.m_timestamp << "\t";
  outFile << (uint32_t) params.m_cellId << "\t";
  outFile << params.m_imsi << "\t";
  outFile << params.m_rnti << "\t";
  //outFile << (uint32_t) params.m_txMode << "\t"; // txMode is not available at dl tx side
  outFile << (uint32_t) params.m_layer << "\t";
  outFile << (uint32_t) params.m_mcs << "\t";
  outFile << params.m_size << "\t";
  outFile << (uint32_t) params.m_rv << "\t";
  outFile << (uint32_t) params.m_ndi << std::endl;
  outFile.close ();
}

void
PhyTxStatsCalculator::UlPhyTransmission (PhyTransmissionStatParameters params)
{
  NS_LOG_FUNCTION (this << params.m_cellId << params.m_imsi << params.m_timestamp << params.m_rnti << params.m_layer << params.m_mcs << params.m_size << params.m_rv << params.m_ndi);
  NS_LOG_INFO ("Write UL Tx Phy Stats in " << GetUlTxOutputFilename ().c_str ());

  std::ofstream outFile;
  if ( m_ulTxFirstWrite == true )
    {
      outFile.open (GetUlTxOutputFilename ().c_str ());
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << GetUlTxOutputFilename ().c_str ());
          return;
        }
      m_ulTxFirstWrite = false;
//       outFile << "% time\tcellId\tIMSI\tRNTI\ttxMode\tlayer\tmcs\tsize\trv\tndi";
      outFile << "% time\tcellId\tIMSI\tRNTI\tlayer\tmcs\tsize\trv\tndi";
      outFile << std::endl;
    }
  else
    {
      outFile.open (GetUlTxOutputFilename ().c_str (),  std::ios_base::app);
      if (!outFile.is_open ())
        {
          NS_LOG_ERROR ("Can't open file " << GetUlTxOutputFilename ().c_str ());
          return;
        }
    }

//   outFile << Simulator::Now ().GetNanoSeconds () / (double) 1e9 << "\t";
  outFile << params.m_timestamp << "\t";
  outFile << (uint32_t) params.m_cellId << "\t";
  outFile << params.m_imsi << "\t";
  outFile << params.m_rnti << "\t";
  //outFile << (uint32_t) params.m_txMode << "\t";
  outFile << (uint32_t) params.m_layer << "\t";
  outFile << (uint32_t) params.m_mcs << "\t";
  outFile << params.m_size << "\t";
  outFile << (uint32_t) params.m_rv << "\t";
  outFile << (uint32_t) params.m_ndi << std::endl;
  outFile.close ();
}

void
PhyTxStatsCalculator::DlPhyTransmissionCallback (Ptr<PhyTxStatsCalculator> phyTxStats,
                      std::string path, PhyTransmissionStatParameters params)
{
  NS_LOG_FUNCTION (phyTxStats << path);
  uint64_t imsi = 0;
  std::ostringstream pathAndRnti;
  pathAndRnti << path << "/" << params.m_rnti;
  if (phyTxStats->ExistsImsiPath (pathAndRnti.str ()) == true)
    {
      imsi = phyTxStats->GetImsiPath (pathAndRnti.str ());
    }
  else
    {
      imsi = FindImsiForEnb (path, params.m_rnti);
      phyTxStats->SetImsiPath (pathAndRnti.str (), imsi);
    }

  params.m_imsi = imsi;
  phyTxStats->DlPhyTransmission (params);
}

void
PhyTxStatsCalculator::UlPhyTransmissionCallback (Ptr<PhyTxStatsCalculator> phyTxStats,
                      std::string path, PhyTransmissionStatParameters params)
{
  NS_LOG_FUNCTION (phyTxStats << path);
  uint64_t imsi = 0;
  std::ostringstream pathAndRnti;
  pathAndRnti << path << "/" << params.m_rnti;
  if (phyTxStats->ExistsImsiPath (pathAndRnti.str ()) == true)
    {
      imsi = phyTxStats->GetImsiPath (pathAndRnti.str ());
    }
  else
    {
      imsi = FindImsiForUe (path, params.m_rnti);
      phyTxStats->SetImsiPath (pathAndRnti.str (), imsi);
    }

  params.m_imsi = imsi;
  phyTxStats->UlPhyTransmission (params);
}


} // namespace ns3

