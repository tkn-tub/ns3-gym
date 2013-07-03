/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Magister Solutions
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
 * Author: Budiarto Herman <budiarto.herman@magister.fi>
 */

#ifndef LTE_TEST_UE_MEASUREMENTS_HANDOVER_H
#define LTE_TEST_UE_MEASUREMENTS_HANDOVER_H

#include <ns3/test.h>
#include <ns3/lte-rrc-sap.h>
#include <vector>

namespace ns3 {

class Time;


/**
 * \brief Test suite for generating calls to UE measurements test case
 *        ns3::LteUeMeasurementsHandoverTestCase.
 */
class LteUeMeasurementsHandoverTestSuite : public TestSuite
{
public:
  LteUeMeasurementsHandoverTestSuite ();
};


/**
 * \brief Testing UE measurements in LTE with simulation of 2 eNodeB and 1 UE in
 *        a handover configuration.
 *
 * The simulation will run for 2 seconds, while the handover command will be
 * issued at second +1s.
 */
class LteUeMeasurementsHandoverTestCase : public TestCase
{
public:
  LteUeMeasurementsHandoverTestCase (std::string name,
                                     LteRrcSap::ReportConfigEutra sourceConfig,
                                     LteRrcSap::ReportConfigEutra targetConfig,
                                     std::vector<Time> expectedTime,
                                     std::vector<uint8_t> expectedRsrp);

  virtual ~LteUeMeasurementsHandoverTestCase ();

  /**
   * \brief Triggers when either one of the eNodeBs receives measurement report
   *        from UE, then perform verification on it.
   *
   * The trigger is set up beforehand by connecting to the
   * `LteUeRrc::RecvMeasurementReport` trace source.
   *
   * Verification consists of checking whether the report carries the right
   * value of RSRP or not, and whether it occurs at the expected time or not.
   */
  void RecvMeasurementReportCallback (std::string context, uint64_t imsi,
                                      uint16_t cellId, uint16_t rnti,
                                      LteRrcSap::MeasurementReport report);

private:
  /**
   * \brief Setup the simulation with the intended UE measurement reporting
   *        configuration, run it, and connect the
   *        `RecvMeasurementReportCallback` function to the
   *        `LteUeRrc::RecvMeasurementReport` trace source.
   */
  virtual void DoRun ();

  /**
   * \brief Runs at the end of the simulation, verifying that all expected
   *        measurement reports have been examined.
   */
  virtual void DoTeardown ();

  /**
   * \brief The active report triggering configuration for the source eNodeB.
   */
  LteRrcSap::ReportConfigEutra m_sourceConfig;

  /**
   * \brief The active report triggering configuration for the target eNodeB.
   */
  LteRrcSap::ReportConfigEutra m_targetConfig;

  /**
   * \brief The list of expected time when measurement reports are received by
   *        eNodeB.
   */
  std::vector<Time> m_expectedTime;

  /**
   * \brief The list of expected values of RSRP (in 3GPP range unit) from the
   *        measurement reports received.
   */
  std::vector<uint8_t> m_expectedRsrp;

  /**
   * \brief Pointer to the element of `m_expectedTime` which is expected to
   *        occur next in the simulation.
   */
  std::vector<Time>::iterator m_itExpectedTime;

  /**
   * \brief Pointer to the element of `m_expectedRsrp` which is expected to
   *        occur next in the simulation.
   */
  std::vector<uint8_t>::iterator m_itExpectedRsrp;

}; // end of class LteUeMeasurementsHandoverTestCase


} // end of namespace ns3

#endif /* LTE_TEST_UE_MEASUREMENTS_HANDOVER_H */
