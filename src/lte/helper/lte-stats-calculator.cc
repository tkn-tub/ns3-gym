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

#include "lte-stats-calculator.h"
#include <ns3/log.h>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LteStatsCalculator");

LteStatsCalculator::LteStatsCalculator ()
  : m_dlOutputFilename (""),
    m_ulOutputFilename ("")
{
  // Nothing to do here

}

LteStatsCalculator::~LteStatsCalculator ()
{
  // Nothing to do here
}


TypeId
LteStatsCalculator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteStatsCalculator")
    .SetParent<Object> ()
    .AddConstructor<LteStatsCalculator> ()
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
  if (m_pathImsiMap.find (path) == m_pathImsiMap.end () )
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
  NS_LOG_FUNCTION (this << path << imsi);
  m_pathImsiMap[path] = imsi;
}

uint64_t
LteStatsCalculator::GetImsiPath (std::string path)
{
  return m_pathImsiMap.find (path)->second;
}

bool
LteStatsCalculator::ExistsCellIdPath (std::string path)
{
  if (m_pathCellIdMap.find (path) == m_pathCellIdMap.end () )
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
  NS_LOG_FUNCTION (this << path << cellId);
  m_pathCellIdMap[path] = cellId;
}

uint16_t
LteStatsCalculator::GetCellIdPath (std::string path)
{
  return m_pathCellIdMap.find (path)->second;
}


} // namespace ns3
