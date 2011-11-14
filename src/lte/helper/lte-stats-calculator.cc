/*
 * lte-stats-calculator.cpp
 *
 *  Created on: Nov 4, 2011
 *      Author: jnin
 */

#include "lte-stats-calculator.h"

namespace ns3 {


LteStatsCalculator::LteStatsCalculator ()
  : m_dlOutputFilename (""),
    m_ulOutputFilename ("")
{
  // Nothing to do here

}

LteStatsCalculator::~LteStatsCalculator ()
{
  // TODO Auto-generated destructor stub
}


TypeId
LteStatsCalculator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteStatsCalculator")
    .SetParent<Object> ()
    .AddConstructor<LteStatsCalculator> ()
    .AddAttribute ("DlOutputFilename",
                   "Name of the file where the downlink results will be saved.",
                   StringValue ("DlStats.csv"),
                   MakeStringAccessor (&LteStatsCalculator::SetDlOutputFilename),
                   MakeStringChecker ())
    .AddAttribute ("UlOutputFilename",
                   "Name of the file where the uplink results will be saved.",
                   StringValue ("UlStats.csv"),
                   MakeStringAccessor (&LteStatsCalculator::SetUlOutputFilename),
                   MakeStringChecker ())
  ;
  return tid;
}


void
LteStatsCalculator::SetUlOutputFilename (std::string outputFilename)
{
  m_ulOutputFilename = outputFilename;
}

std::string
LteStatsCalculator::GetUlOutputFilename (void)
{
  return m_ulOutputFilename;
}

void
LteStatsCalculator::SetDlOutputFilename (std::string outputFilename)
{
  m_dlOutputFilename = outputFilename;
}

std::string
LteStatsCalculator::GetDlOutputFilename (void)
{
  return m_dlOutputFilename;
}


bool
LteStatsCalculator::ExistsImsiPath (std::string path)
{
  if (m_pathImsiMap.find(path) == m_pathImsiMap.end () )
    {
      return false;
    }
  else
    {
      return true;
    }
}

void
LteStatsCalculator::SetImsiPath (std::string path, uint64_t imsi)
{
  m_pathImsiMap[path] = imsi;
}

uint64_t
LteStatsCalculator::GetImsiPath (std::string path)
{
  return m_pathImsiMap.find(path)->second;
}

bool
LteStatsCalculator::ExistsCellIdPath (std::string path)
{
  if (m_pathCellIdMap.find(path) == m_pathCellIdMap.end () )
    {
      return false;
    }
  else
    {
      return true;
    }
}

void
LteStatsCalculator::SetCellIdPath (std::string path, uint16_t cellId)
{
  m_pathCellIdMap[path] = cellId;
}

uint16_t
LteStatsCalculator::GetCellIdPath (std::string path)
{
  return m_pathCellIdMap.find(path)->second;
}


} // namespace ns3
