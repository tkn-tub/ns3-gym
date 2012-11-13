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

#ifndef MAC_STATS_CALCULATOR_H_
#define MAC_STATS_CALCULATOR_H_

#include "ns3/lte-stats-calculator.h"
#include "ns3/nstime.h"
#include "ns3/uinteger.h"
#include <string>
#include <fstream>

namespace ns3 {

/**
 * Takes care of storing the information generated at MAC layer. Metrics saved are:
 *   - Timestamp (in seconds)
 *   - Frame index
 *   - Subframe index
 *   - C-RNTI
 *   - MCS for transport block 1
 *   - Size of transport block 1
 *   - MCS for transport block 2 (0 if not used)
 *   - Size of transport block 2 (0 if not used)
 */
class MacStatsCalculator : public LteStatsCalculator
{
public:
  /**
   * Constructor
   */
  MacStatsCalculator ();

  /**
   * Destructor
   */
  virtual ~MacStatsCalculator ();

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
   * Get the name of the file where the uplink statistics will be stored.
   */
  std::string GetUlOutputFilename (void);

  /**
   * Set the name of the file where the downlink statistics will be stored.
   *
   * @param outputFilename string with the name of the file
   */
  void SetDlOutputFilename (std::string outputFilename);

  /**
   * Get the name of the file where the downlink statistics will be stored.
   */
  std::string GetDlOutputFilename (void);

  /**
   * Notifies the stats calculator that an downlink scheduling has occurred.
   * @param cellId Cell ID of the attached Enb
   * @param imsi IMSI of the scheduled UE
   * @param frameNo Frame number
   * @param subframeNo Subframe number
   * @param rnti C-RNTI scheduled
   * @param mcsTb1 MCS for transport block 1
   * @param sizeTb1 Size of transport block 1
   * @param mcsTb2 MCS for transport block 2 (0 if not used)
   * @param sizeTb2 Size of transport block 2 (0 if not used)
   */
  void DlScheduling (uint16_t cellId, uint64_t imsi, uint32_t frameNo, uint32_t subframeNo,
                     uint16_t rnti, uint8_t mcsTb1, uint16_t sizeTb1, uint8_t mcsTb2, uint16_t sizeTb2);

  /**
   * Notifies the stats calculator that an uplink scheduling has occurred.
   * @param cellId Cell ID of the attached Enb
   * @param imsi IMSI of the scheduled UE
   * @param frameNo Frame number
   * @param subframeNo Subframe number
   * @param rnti C-RNTI scheduled
   * @param mcsTb MCS for transport block
   * @param sizeTb Size of transport block
   */
  void UlScheduling (uint16_t cellId, uint64_t imsi,uint32_t frameNo, uint32_t subframeNo,
                     uint16_t rnti, uint8_t mcs, uint16_t sizeTb);

private:

  bool m_dlFirstWrite;
  bool m_ulFirstWrite;

};

} // namespace ns3

#endif /* MAC_STATS_CALCULATOR_H_ */
