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
 *        Convert MacStatsCalculator in PhyRxStatsCalculator
 */

#ifndef PHY_RX_STATS_CALCULATOR_H_
#define PHY_RX_STATS_CALCULATOR_H_

#include "ns3/lte-stats-calculator.h"
#include "ns3/nstime.h"
#include "ns3/uinteger.h"
#include <string>
#include <fstream>
#include <ns3/lte-common.h>

namespace ns3 {

/**
 * Takes care of storing the information generated at PHY layer regarding 
 * reception. Metrics saved are:
 *time\tframe\tsframe\tRNTI\tmcsTb1\tsizeTb1\tmcsTb2\tsizeTb2
 *   - Timestamp (in seconds)
 *   - Frame index
 *   - Subframe index
 *   - C-RNTI
 *   - MCS for transport block 1
 *   - Size of transport block 1
 *   - MCS for transport block 2 (0 if not used)
 *   - Size of transport block 2 (0 if not used)
 */
class PhyRxStatsCalculator : public LteStatsCalculator
{
public:
  /**
   * Constructor
   */
  PhyRxStatsCalculator ();

  /**
   * Destructor
   */
  virtual ~PhyRxStatsCalculator ();

  /**
   * Inherited from ns3::Object
   */
  static TypeId GetTypeId (void);

  /**
   * Set the name of the file where the UL Rx PHY statistics will be stored.
   *
   * \param outputFilename string with the name of the file
   */
  void SetUlRxOutputFilename (std::string outputFilename);

  /**
   * Get the name of the file where the UL RX PHY statistics will be stored.
   */
  std::string GetUlRxOutputFilename (void);

  /**
   * Set the name of the file where the DL RX PHY statistics will be stored.
   *
   * @param outputFilename string with the name of the file
   */
  void SetDlRxOutputFilename (std::string outputFilename);

  /**
   * Get the name of the file where the DL RX PHY statistics will be stored.
   */
  std::string GetDlRxOutputFilename (void);

  /**
   * Notifies the stats calculator that an downlink reception has occurred.
   * @param cellId Cell ID of the attached Enb
   * @param imsi IMSI of the scheduled UE
   * @param frameNo Frame number
   * @param subframeNo Subframe number
   * @param rnti C-RNTI scheduled
   * @param layer the layer (cw) of the transmission
   * @param txMode the transmission Mode
   * @param mcs MCS for transport block
   * @param size Size of transport block
   * @param rv the redundancy version (HARQ)
   * @param ndi new data indicator flag
   * @param correctness correctness of the TB received
   */
  void DlPhyReception (PhyReceptionStatParameters params);

  /**
   * Notifies the stats calculator that an uplink reception has occurred.
   * @param cellId Cell ID of the attached Enb
   * @param imsi IMSI of the scheduled UE
   * @param frameNo Frame number
   * @param subframeNo Subframe number
   * @param rnti C-RNTI scheduled
   * @param layer the layer (cw) of the transmission
   * @param txMode the transmission Mode
   * @param mcs MCS for transport block
   * @param size Size of transport block
   * @param rv the redundancy version (HARQ)
   * @param ndi new data indicator flag
   * @param correctness correctness of the TB received
   */
  void UlPhyReception (PhyReceptionStatParameters params);

  /** 
   * trace sink
   * 
   * \param phyRxStats 
   * \param path 
   * \param params 
   */
  static void DlPhyReceptionCallback (Ptr<PhyRxStatsCalculator> phyRxStats,
                               std::string path, PhyReceptionStatParameters params);

  /** 
   * trace sink
   * 
   * \param phyRxStats 
   * \param path 
   * \param params 
   */
  static void UlPhyReceptionCallback (Ptr<PhyRxStatsCalculator> phyRxStats,
                               std::string path, PhyReceptionStatParameters params);
private:

  bool m_dlRxFirstWrite;
  bool m_ulRxFirstWrite;

};

} // namespace ns3

#endif /* PHY_RX_STATS_CALCULATOR_H_ */
