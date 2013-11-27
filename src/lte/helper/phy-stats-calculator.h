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

#ifndef PHY_STATS_CALCULATOR_H_
#define PHY_STATS_CALCULATOR_H_

#include "ns3/lte-stats-calculator.h"
#include "ns3/nstime.h"
#include "ns3/uinteger.h"
#include "ns3/spectrum-value.h"
#include <string>
#include <fstream>

namespace ns3 {

/**
 * Takes care of storing the information generated at PHY layer. Metrics saved are:
 * - RSRP and average SINR for DL
 *   - Timestamp (in seconds)
 *   - IMSI
 *   - C-RNTI
 *   - RSRP
 *   - SINR
 * - UE SINR
 *   - Timestamp (in seconds)
 *   - Cell ID of the reported Enb
 *   - IMSI
 *   - C-RNTI
 *   - measured and reported SINR value in linear
 * - Interference for UL
 *   - Cell ID of the reported Enb
 *   - IMSI of the scheduled UE
 *   - C-RNTI scheduled
 *   - Measured interference for each RB
 */
class PhyStatsCalculator : public LteStatsCalculator
{
public:
  /**
   * Constructor
   */
  PhyStatsCalculator ();

  /**
   * Destructor
   */
  virtual ~PhyStatsCalculator ();

  // Inherited from ns3::Object
  static TypeId GetTypeId (void);

  /**
   * Set the name of the file where the RSRP/SINR statistics will be stored.
   *
   * @param filename string with the name of the file
   */
  void SetCurrentCellRsrpSinrFilename (std::string filename);

  /**
   * Get the name of the file where the RSRP/SINR statistics will be stored.
   * @return the name of the file where the RSRP/SINR statistics will be stored
   */
  std::string GetCurrentCellRsrpSinrFilename  (void);

  /**
   * Set the name of the file where the UE SINR statistics will be stored.
   *
   * @param filename string with the name of the file
   */
  void SetUeSinrFilename (std::string filename);

  /**
   * Get the name of the file where the UE SINR statistics will be stored.
   * @return the name of the file where the UE SINR statistics will be stored
   */
  std::string GetUeSinrFilename (void);

  /**
   * Set the name of the file where the interference statistics will be stored.
   *
   * @param filename string with the name of the file
   */
  void SetInterferenceFilename (std::string filename);

  /**
   * Get the name of the file where the interference statistics will be stored.
   * @return the name of the file where the interference statistics will be stored
   */
  std::string GetInterferenceFilename (void);

  /**
   * Notifies the stats calculator that an RSRP and SINR report has occurred.
   * @param imsi IMSI of the scheduled UE
   * @param rnti C-RNTI scheduled
   * @param rsrp Reference Signal Received Power
   * @param sinr SINR averaged among RBs
   */
  void ReportCurrentCellRsrpSinr (uint16_t cellId, uint64_t imsi, uint16_t rnti, double rsrp, double sinr);

  /**
   * Notifies the stats calculator that an UE SINR report has occurred.
   * @param cellId Cell ID of the reported Enb
   * @param imsi IMSI of the scheduled UE
   * @param rnti C-RNTI scheduled
   * @param sinrLinear measured and reported SINR value in linear
   */
  void ReportUeSinr (uint16_t cellId, uint64_t imsi, uint16_t rnti, double sinrLinear);
  /**
   * Notifies the stats calculator that an interference report has occurred.
   * @param cellId Cell ID of the reported Enb
   * @param interference Measured interference for each RB
   */
  void ReportInterference (uint16_t cellId, Ptr<SpectrumValue> interference);

  /** 
   * trace sink
   * 
   * \param phyStats 
   * \param path 
   * \param cellId 
   * \param rnti 
   * \param rsrp 
   * \param sinr 
   */
  static void ReportCurrentCellRsrpSinrCallback (Ptr<PhyStatsCalculator> phyStats,
                                          std::string path, uint16_t cellId, uint16_t rnti,
                                          double rsrp, double sinr);
  
  /** 
   * trace sink
   * 
   * \param phyStats 
   * \param path 
   * \param cellId 
   * \param rnti 
   * \param sinrLinear 
   */
  static void ReportUeSinr (Ptr<PhyStatsCalculator> phyStats, std::string path,
                     uint16_t cellId, uint16_t rnti, double sinrLinear);

  /** 
   * trace sink
   * 
   * \param phyStats 
   * \param path 
   * \param cellId 
   * \param interference 
   */
  static void ReportInterference (Ptr<PhyStatsCalculator> phyStats, std::string path,
                           uint16_t cellId, Ptr<SpectrumValue> interference);


private:
  bool m_RsrpSinrFirstWrite;
  bool m_UeSinrFirstWrite;
  bool m_InterferenceFirstWrite;

  std::string m_RsrpSinrFilename;
  std::string m_ueSinrFilename;
  std::string m_interferenceFilename;

};

} // namespace ns3

#endif /* PHY_STATS_CALCULATOR_H_ */
