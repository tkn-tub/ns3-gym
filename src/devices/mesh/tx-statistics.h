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


#ifndef TX_STAT_H
#define TX_STAT_H

#include <map>
#include <utility>

#include "ns3/mac48-address.h"
#include "ns3/packet.h"
#include "ns3/object.h"
#include "ns3/traced-value.h"
#include "ns3/wifi-mode.h"

namespace ns3 {
class WifiTxStatistics : public Object
{
public:
  static TypeId  GetTypeId (void);
  WifiTxStatistics ();
  ~WifiTxStatistics ();

  struct TxStat
  {
    /**
     * Packet counters:
     */
    uint32_t packetsFailed;
    uint32_t packetsRetried;
    uint32_t packetsAcked;
    /**
     * RTS counters:
     */
    uint32_t rtsFailed;
    uint32_t rtsRetried;
    uint32_t rtsAcked;
    /**
     * Byte counters:
     */
    uint64_t bytesFailed;
    uint64_t bytesRetried;
    uint64_t bytesAcked;
  };
  typedef struct TxStat   SIMPLE_STAT;
  typedef std::map<uint32_t, SIMPLE_STAT> RATE_STAT;
#if 0
  typedef std::map<uint16_t, SIMPLE_STAT> LENGTH_STAT;
#endif
  typedef std::map<uint16_t, RATE_STAT> RATE_LENGTH_STAT;

  void   SetLengthDivisionParams (uint16_t maxLength, uint8_t numOfGroups);

  void   NotifyDataSent (uint16_t length, uint32_t dataRate);
  void   NotifyDataFailed ();
  void   NotifyGotAck (uint32_t retryCounter);

  void   NotifyRtsSend (uint32_t rtsRate, uint32_t dataLength);
  void   NotifyRtsRetried ();
  void   NotifyRtsFailed ();
  void   NotifyRtsSuccess (uint32_t retryCounter);

  void   ResetStatistics ();
#if 0
  SIMPLE_STAT  GetTxStatCommon ();
  RATE_STAT  GetTxStatRate ();
  LENGTH_STAT  GetTxStatLength ();
#endif
  typedef struct {
    RATE_LENGTH_STAT statistics;
    uint16_t  lengthInterval;
    uint16_t  maxLength;
  } TX_STATISTICS;
  TX_STATISTICS  GetTxStatRateLength ();
private:
  RATE_STAT::iterator FillCurrentStatPosition (uint16_t length, uint32_t dataRate);
  //DEBUG PURPOSE
  //void   Print ();
  RATE_LENGTH_STAT m_stats;
  bool   m_isTx;
  uint8_t   m_numOfGroups;
  uint16_t  m_maxLength;
  uint16_t  m_currentSize;
  uint32_t  m_currentRate;
};
} //namespace ns3
#endif
