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

#ifndef RLC_STATS_CALCULATOR_H_
#define RLC_STATS_CALCULATOR_H_

#include "ns3/uinteger.h"
#include "ns3/object.h"
#include "ns3/basic-data-calculators.h"
#include "ns3/lte-common.h"
#include <string>
#include <map>
#include <fstream>

namespace ns3 {


struct ImsiLcidPair
{
  uint64_t  m_imsi;
  uint8_t   m_lcId;

public:
  ImsiLcidPair ();
  ImsiLcidPair (const uint64_t a, const uint8_t b);

  friend bool operator == (const ImsiLcidPair &a, const ImsiLcidPair &b);
  friend bool operator < (const ImsiLcidPair &a, const ImsiLcidPair &b);
};

typedef std::map<ImsiLcidPair, uint32_t> Uint32Map;
typedef std::map<ImsiLcidPair, uint64_t> Uint64Map;
typedef std::map<ImsiLcidPair, Ptr<MinMaxAvgTotalCalculator<uint32_t> > > Uint32StatsMap;
typedef std::map<ImsiLcidPair, Ptr<MinMaxAvgTotalCalculator<uint64_t> > > Uint64StatsMap;
typedef std::map<ImsiLcidPair, double> DoubleMap;
typedef std::map<ImsiLcidPair, LteFlowId_t> FlowIdMap;



/**
 * Calculation of statistics from the RLC layer for uplink and downlink, the data is dumped into a file periodically. Metrics considered are:
 *   - Number of transmitted PDUs
 *   - Number of received PDUs
 *   - Number of transmitted bytes
 *   - Number of received bytes
 *   - Average, min, max and standard deviation of RLC to RLC delay
 *   - Average, min, max and standard deviation of RLC PDU size
 */
class RlcStatsCalculator : public Object
{
public:
  /**
   * Class constructor
   */
  RlcStatsCalculator ();

  /**
   * Class destructor
   */
  virtual  ~RlcStatsCalculator ();

  /**
   * Inherited from ns3::Object
   */
  static TypeId GetTypeId (void);

  /**
   * Set the name of the file where the uplink statistics will be stored.
   *
   * \param outputFilename string with the name of the file
   */
  void SetUlOutputFilename (std::string outputFilename);

  /**
   * Set the name of the file where the downlink statistics will be stored.
   *
   * \param outputFilename string with the name of the file
   */
  void SetDlOutputFilename (std::string outputFilename);

  /**
   * Notifies the stats calculator that an uplink transmission has occurred.
   * @param imsi IMSI of the UE who transmitted the PDU
   * @param rnti C-RNTI of the UE who transmitted the PDU
   * @param lcid LCID through which the PDU has been transmitted
   * @param packetSize size of the PDU in bytes
   */
  void UlTxPdu (uint64_t imsi, uint16_t rnti, uint8_t lcid, uint32_t packetSize);

  /**
   * Notifies the stats calculator that an uplink reception has occurred.
   * @param cellId CellId of the attached Enb
   * @param imsi IMSI of the UE who received the PDU
   * @param rnti C-RNTI of the UE who received the PDU
   * @param lcid LCID through which the PDU has been received
   * @param packetSize size of the PDU in bytes
   * @param delay RLC to RLC delay in nanoseconds
   */
  void UlRxPdu (uint16_t cellId, uint64_t imsi, uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay);

  /**
   * Notifies the stats calculator that an downlink transmission has occurred.
   * @param cellId CellId of the attached Enb
   * @param imsi IMSI of the UE who is receiving the PDU
   * @param rnti C-RNTI of the UE who is receiving the PDU
   * @param lcid LCID through which the PDU has been transmitted
   * @param packetSize size of the PDU in bytes
   */
  void DlTxPdu (uint16_t cellId, uint64_t imsi, uint16_t rnti, uint8_t lcid, uint32_t packetSize);

  /**
   * Notifies the stats calculator that an downlink reception has occurred.
   * @param imsi IMSI of the UE who received the PDU
   * @param rnti C-RNTI of the UE who received the PDU
   * @param lcid LCID through which the PDU has been transmitted
   * @param packetSize size of the PDU in bytes
   * @param delay RLC to RLC delay in nanoseconds
   */
  void DlRxPdu (uint64_t imsi, uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay);

  /**
   * Gets the number of transmitted uplink packets.
   * @param imsi IMSI of the UE
   * @param lcid LCID
   * @return number of transmitted uplink packets
   */
  uint32_t GetUlTxPackets (uint64_t imsi, uint8_t lcid);

  /**
   * Gets the number of received uplink packets.
   * @param imsi IMSI of the UE
   * @param lcid LCID
   * @return number of received uplink packets
   */
  uint32_t GetUlRxPackets (uint64_t imsi, uint8_t lcid);

  /**
   * Gets the number of transmitted uplink data bytes.
   * @param imsi IMSI of the UE
   * @param lcid LCID
   * @return number of transmitted data bytes
   */
  uint64_t GetUlTxData (uint64_t imsi, uint8_t lcid);

  /**
   * Gets the number of received uplink data bytes.
   * @param imsi IMSI of the UE
   * @param lcid LCID
   * @return number of received data bytes
   */
  uint64_t GetUlRxData (uint64_t imsi, uint8_t lcid);

  /**
   * Gets the attached Enb cellId.
   * @param imsi IMSI of the UE
   * @param lcid LCID
   * @return Enb cellId
   */
  uint32_t GetUlCellId (uint64_t imsi, uint8_t lcid);

  /**
   * Gets the uplink RLC to RLC delay
   * @param imsi IMSI of the UE
   * @param lcid LCID
   * @return RLC to RLC delay in seconds
   */
  double   GetUlDelay (uint64_t imsi, uint8_t lcid);

  /**
   * Gets the uplink RLC to RLC statistics: average, min, max and standard deviation.
   * @param imsi IMSI of the UE
   * @param lcid LCID
   * @return RLC to RLC delay statistics average, min, max and standard deviation in seconds
   */
  std::vector<double> GetUlDelayStats (uint64_t imsi, uint8_t lcid);

  /**
   * Gets the uplink PDU size statistics: average, min, max and standard deviation.
   * @param imsi IMSI of the UE
   * @param lcid LCID
   * @return PDU size statistics average, min, max and standard deviation in seconds
   */
  std::vector<double> GetUlPduSizeStats (uint64_t imsi, uint8_t lcid);

  /**
   * Gets the number of transmitted downlink data bytes.
   * @param imsi IMSI of the UE
   * @param lcid LCID
   * @return number of transmitted data bytes
   */
  uint32_t GetDlTxPackets (uint64_t imsi, uint8_t lcid);

  /**
   * Gets the number of received downlink data bytes.
   * @param imsi IMSI of the UE
   * @param lcid LCID
   * @return number of received data bytes
   */
  uint32_t GetDlRxPackets (uint64_t imsi, uint8_t lcid);

  /**
   * Gets the number of transmitted downlink data bytes.
   * @param imsi IMSI of the UE
   * @param lcid LCID
   * @return number of transmitted data bytes
   */
  uint64_t GetDlTxData (uint64_t imsi, uint8_t lcid);

  /**
   * Gets the number of received downlink data bytes.
   * @param imsi IMSI of the UE
   * @param lcid LCID
   * @return number of received data bytes
   */
  uint64_t GetDlRxData (uint64_t imsi, uint8_t lcid);

  /**
   * Gets the attached Enb cellId.
   * @param imsi IMSI of the UE
   * @param lcid LCID
   * @return Enb cellId
   */
  uint32_t GetDlCellId (uint64_t imsi, uint8_t lcid);

  /**
   * Gets the downlink RLC to RLC delay
   * @param imsi IMSI of the UE
   * @param lcid LCID
   * @return RLC to RLC delay in seconds
   */
  double   GetDlDelay (uint64_t imsi, uint8_t lcid);

  /**
   * Gets the downlink RLC to RLC statistics: average, min, max and standard deviation.
   * @param imsi IMSI of the UE
   * @param lcid LCID
   * @return RLC to RLC delay statistics average, min, max and standard deviation in seconds
   */
  std::vector<double> GetDlDelayStats (uint64_t imsi, uint8_t lcid);

  /**
   * Gets the downlink PDU size statistics: average, min, max and standard deviation.
   * @param imsi IMSI of the UE
   * @param lcid LCID
   * @return PDU size statistics average, min, max and standard deviation in seconds
   */
  std::vector<double> GetDlPduSizeStats (uint64_t imsi, uint8_t lcid);

private:
  void ShowResults (void);
  void WriteUlResults (std::ofstream& outFile);
  void WriteDlResults (std::ofstream& outFile);
  void ResetResults (void);

  void StartEpoch (void);
  void CheckEpoch (bool forceEpoch = false);

  FlowIdMap m_flowId;

  std::string m_dlOutputFilename;
  Uint32Map m_dlCellId;
  Uint32Map m_dlTxPackets;
  Uint32Map m_dlRxPackets;
  Uint64Map m_dlTxData;
  Uint64Map m_dlRxData;
  Uint64StatsMap m_dlDelay;
  Uint32StatsMap m_dlPduSize;

  std::string m_ulOutputFilename;
  Uint32Map m_ulCellId;
  Uint32Map m_ulTxPackets;
  Uint32Map m_ulRxPackets;
  Uint64Map m_ulTxData;
  Uint64Map m_ulRxData;
  Uint64StatsMap m_ulDelay;
  Uint32StatsMap m_ulPduSize;

  /**
   * Start time of the on going epoch
   */
  Time m_startTime;

  /**
   * Epoch duration
   */
  Time m_epochDuration;

  bool m_firstWrite;

};

} // namespace ns3

#endif /* RLC_STATS_CALCULATOR_H_ */
