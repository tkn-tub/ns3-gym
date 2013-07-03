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

#ifndef LTE_TEST_UE_MEASUREMENTS_PIECEWISE_1_H
#define LTE_TEST_UE_MEASUREMENTS_PIECEWISE_1_H

#include <ns3/test.h>
#include <ns3/ptr.h>
#include <ns3/lte-rrc-sap.h>
#include <vector>

namespace ns3 {

class MobilityModel;
class Time;


/**
 * \brief Test suite for generating calls to UE measurements test case
 *        ns3::LteUeMeasurementsPiecewiseTestCase1.
 */
class LteUeMeasurementsPiecewiseTestSuite1 : public TestSuite
{
public:
  LteUeMeasurementsPiecewiseTestSuite1 ();
};


/**
 * \brief Testing UE measurements in LTE with simulation of 1 eNodeB and 1 UE in
 *        piecewise configuration and 120 ms report interval.
 */
class LteUeMeasurementsPiecewiseTestCase1 : public TestCase
{
public:
  LteUeMeasurementsPiecewiseTestCase1 (std::string name,
                                       LteRrcSap::ReportConfigEutra config,
                                       std::vector<Time> expectedTime,
                                       std::vector<uint8_t> expectedRsrp);

  virtual ~LteUeMeasurementsPiecewiseTestCase1 ();

  /**
   * \brief Triggers when eNodeB receives measurement report from UE, then
   *        perform verification on it.
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

  void TeleportVeryNear ();
  void TeleportNear ();
  void TeleportFar ();
  void TeleportVeryFar ();

  /**
   * \brief The active report triggering configuration.
   */
  LteRrcSap::ReportConfigEutra m_config;

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

  Ptr<MobilityModel> m_ueMobility;

}; // end of class LteUeMeasurementsPiecewiseTestCase1


} // end of namespace ns3

#endif /* LTE_TEST_UE_MEASUREMENTS_PIECEWISE_1_H */
