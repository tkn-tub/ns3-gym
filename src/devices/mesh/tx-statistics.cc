/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */


#include "ns3/tx-statistics.h"
#include "ns3/assert.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("WifiTxStatistics");

namespace ns3 {

TypeId
WifiTxStatistics::GetTypeId(void)
{
  static TypeId tid = TypeId("ns3::WifiTxStatistics")
                      .SetParent<Object> ()
                      .AddConstructor<WifiTxStatistics> ();
  return tid;
}
WifiTxStatistics::WifiTxStatistics():
    m_numOfGroups(10),
    m_maxLength(2500)
{
}
WifiTxStatistics::~WifiTxStatistics()
{
}
void
WifiTxStatistics::SetLengthDivisionParams(uint16_t maxLength, uint8_t numOfGroups)
{

}
void
WifiTxStatistics::NotifyDataSent(uint16_t length, uint32_t dataRate)
{
  m_currentSize = length;
  m_currentRate = dataRate;
}

WifiTxStatistics::RATE_STAT::iterator
WifiTxStatistics::FillCurrentStatPosition(uint16_t length, uint32_t dataRate)
{
  uint16_t group = (length/(m_maxLength/m_numOfGroups)+1)* (m_maxLength/m_numOfGroups);
#if 0
  for (RATE_LENGTH_STAT::iterator i = m_stats.begin(); i != m_stats.end(); i ++)
    if (i->first == )
#endif
      RATE_LENGTH_STAT::iterator lengthPos = m_stats.find(group);
  if (lengthPos == m_stats.end())
    {
      RATE_STAT newStat;
      m_stats[group] = newStat;
    }
  lengthPos = m_stats.find(group);
  NS_ASSERT(lengthPos != m_stats.end());
  RATE_STAT::iterator ratePos = lengthPos->second.find(dataRate);
  if (ratePos == lengthPos->second.end())
    {
      SIMPLE_STAT newStat;
      newStat.packetsFailed =0;
      newStat.packetsRetried = 0;
      newStat.packetsAcked = 0;
      newStat.rtsFailed = 0;
      newStat.rtsRetried = 0;
      newStat.rtsAcked = 0;
      newStat.bytesFailed = 0;
      newStat.bytesRetried = 0;
      newStat.bytesAcked = 0;
      lengthPos->second[dataRate] = newStat;
    }
  ratePos = lengthPos->second.find(dataRate);
  NS_ASSERT(ratePos != lengthPos->second.end());
  return  ratePos;
}

void
WifiTxStatistics::NotifyDataFailed()
{
  RATE_STAT::iterator ratePos =  FillCurrentStatPosition(m_currentSize, m_currentRate);
  ratePos->second.packetsFailed++;
  ratePos->second.bytesFailed += m_currentSize;
}

void
WifiTxStatistics::NotifyGotAck(uint32_t retryCounter)
{
  RATE_STAT::iterator ratePos =  FillCurrentStatPosition(m_currentSize, m_currentRate);
  ratePos->second.packetsAcked++;
  ratePos->second.packetsRetried += retryCounter;
  ratePos->second.bytesAcked+= m_currentSize;
  ratePos->second.bytesRetried += (m_currentSize*retryCounter);
}

void
WifiTxStatistics::NotifyRtsSend(uint32_t rtsRate, uint32_t dataLength)
{
  m_currentSize = dataLength;
  m_currentRate = rtsRate;
}

void
WifiTxStatistics::NotifyRtsFailed()
{
  RATE_STAT::iterator ratePos =  FillCurrentStatPosition(m_currentSize, m_currentRate);
  ratePos->second.rtsFailed++;
  ratePos->second.bytesFailed += m_currentSize;
}

void
WifiTxStatistics::NotifyRtsSuccess(uint32_t retryCounter)
{
  RATE_STAT::iterator ratePos =  FillCurrentStatPosition(m_currentSize, m_currentRate);
  ratePos->second.rtsAcked++;
  ratePos->second.rtsRetried += retryCounter;
  ratePos->second.bytesAcked += m_currentSize;
  ratePos->second.bytesRetried += (m_currentSize*retryCounter);
}

void
WifiTxStatistics::ResetStatistics()
{
  for (RATE_LENGTH_STAT::iterator lengthPos = m_stats.begin(); lengthPos != m_stats.end(); lengthPos++)
    lengthPos->second.clear();
}
#if 0
WifiTxStatistics::SIMPLE_STAT
WifiTxStatistics::GetTxStatCommon()
{
}

WifiTxStatistics::RATE_STAT
WifiTxStatistics::GetTxStatRate()
{
}

WifiTxStatistics::LENGTH_STAT
WifiTxStatistics::GetTxStatLength()
{
}
#endif
WifiTxStatistics::TX_STATISTICS
WifiTxStatistics::GetTxStatRateLength()
{
  TX_STATISTICS retval;
  retval.statistics = m_stats;
  retval.lengthInterval = m_maxLength / m_numOfGroups;
  retval.maxLength = m_maxLength;
  return retval;
}
#if 0
void
WifiTxStatistics::Print()
{
  for (RATE_LENGTH_STAT::iterator lengthPos = m_stats.begin(); lengthPos != m_stats.end(); lengthPos++)
    {
      NS_LOG_UNCOND("\tGROUP = " <<lengthPos->first);
      for (RATE_STAT::iterator ratePos = lengthPos->second.begin(); ratePos != lengthPos->second.end(); ratePos ++)
        {
          NS_LOG_UNCOND("Rate is "<<ratePos->first
                        <<": SUCCESS = "<<ratePos->second.packetsAcked
                        <<", RRETRY = " <<ratePos->second.packetsRetried
                        <<", FAILURE = "<<ratePos->second.packetsFailed);
        }
    }
}
#endif
} //namespace ns3
