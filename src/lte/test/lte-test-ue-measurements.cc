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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 *         Nicola Baldo <nbaldo@cttc.es>
 *         Marco Miozzo <mmiozzo@cttc.es>
 */

#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/double.h"
#include <ns3/enum.h>
#include "ns3/boolean.h"
#include "ns3/mobility-helper.h"
#include "ns3/lte-helper.h"
#include "ns3/ff-mac-scheduler.h"

#include "ns3/lte-enb-phy.h"
#include "ns3/lte-enb-net-device.h"

#include "ns3/lte-ue-phy.h"
#include "ns3/lte-ue-net-device.h"
#include <ns3/lte-enb-rrc.h>

#include "lte-test-ue-measurements.h"

#include "lte-test-sinr-chunk-processor.h"
#include <ns3/lte-common.h>

#include <list>

NS_LOG_COMPONENT_DEFINE ("LteUeMeasurementsTest");

namespace ns3 {


void
ReportUeMeasurementsCallback (LteUeMeasurementsTestCase *testcase,
                              std::string path, uint16_t rnti, uint16_t cellId,
                              double rsrp, double rsrq, bool servingCell)
{
  testcase->ReportUeMeasurements (rnti, cellId, rsrp, rsrq, servingCell);
}

void
RecvMeasurementReportCallback (LteUeMeasurementsTestCase *testcase,
                               std::string path, uint64_t imsi, uint16_t cellId,
                               uint16_t rnti, LteRrcSap::MeasurementReport meas)
{
  testcase->RecvMeasurementReport (imsi, cellId, rnti, meas);
}


/**
 * Overloaded operators, for the convenience of defining test cases
 */

std::vector<Time>&
operator<< (std::vector<Time>& v, const uint64_t& ms)
{
  /*
   * Prior attempt to use seconds as unit of choice resulted in precision lost.
   * Therefore milliseconds are used now instead.
   */
  v.push_back (MilliSeconds (ms));
  return v;
}


std::vector<uint8_t>&
operator<< (std::vector<uint8_t>& v, const uint8_t& range)
{
  v.push_back (range);
  return v;
}



/**
 * TestSuite
 */

LteUeMeasurementsTestSuite::LteUeMeasurementsTestSuite ()
  : TestSuite ("lte-ue-measurements", SYSTEM)
{
  //LogComponentEnableAll (LOG_PREFIX_ALL);
  //LogComponentEnable ("LteUeMeasurementsTest", LOG_INFO);
  //LogComponentEnable ("LteUeMeasurementsTest", LOG_DEBUG);
  //LogComponentEnable ("LteEnbRrc", LOG_FUNCTION);
  //LogComponentEnable ("LteEnbRrc", LOG_LOGIC);
  //LogComponentEnable ("LteUeRrc", LOG_INFO);
  //LogComponentEnable ("LteUeRrc", LOG_WARN);
  //LogComponentEnable ("LteUeRrc", LOG_LOGIC);

  RunOriginalTestCase ();
  RunPiecewiseTestCase1 ();
  RunPiecewiseTestCase2 ();
  RunHandoverTestCase ();
}


void
LteUeMeasurementsTestSuite::RunOriginalTestCase ()
{
  NS_LOG_FUNCTION (this);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=10, d2=10000",  10.000000, 10000.000000, -53.739702, -113.739702, -3.010305, -63.010305), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=20, d2=10000",  20.000000, 10000.000000, -59.760302, -113.739702, -3.010319, -56.989719), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=50, d2=10000",  50.000000, 10000.000000, -67.719102, -113.739702, -3.010421, -49.031021), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=100, d2=10000",  100.000000, 10000.000000, -73.739702, -113.739702, -3.010783, -43.010783), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=200, d2=10000",  200.000000, 10000.000000, -79.760302, -113.739702, -3.012232, -36.991632), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=100, d2=10000",  100.000000, 10000.000000, -73.739702, -113.739702, -3.010783, -43.010783), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=200, d2=10000",  200.000000, 10000.000000, -79.760302, -113.739702, -3.012232, -36.991632), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=500, d2=10000",  500.000000, 10000.000000, -87.719102, -113.739702, -3.022359, -29.042959), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=1000, d2=10000",  1000.000000, 10000.000000, -93.739702, -113.739702, -3.058336, -23.058336), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=2000, d2=10000",  2000.000000, 10000.000000, -99.760302, -113.739702, -3.199337, -17.178738), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=5000, d2=10000",  5000.000000, 10000.000000, -107.719102, -113.739702, -4.075793, -10.096393), TestCase::QUICK);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=10000, d2=10000",  10000.000000, 10000.000000, -113.739702, -113.739702, -6.257687, -6.257687), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=20000, d2=10000",  20000.000000, 10000.000000, -119.760302, -113.739702, -10.373365, -4.352765), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=50000, d2=10000",  50000.000000, 10000.000000, -127.719102, -113.739702, -17.605046, -3.625645), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=100000, d2=10000",  100000.000000, 10000.000000, -133.739702, -113.739702, -23.511071, -3.511071), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=200000, d2=10000",  200000.000000, 10000.000000, -139.760302, -113.739702, -29.502549, -3.481949), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=500000, d2=10000",  500000.000000, 10000.000000, -147.719102, -113.739702, -37.453160, -3.473760), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=1000000, d2=10000",  1000000.000000, 10000.000000, -153.739702, -113.739702, -43.472589, -3.472589), TestCase::EXTENSIVE);
}


void
LteUeMeasurementsTestSuite::RunPiecewiseTestCase1 ()
{
  NS_LOG_FUNCTION (this);
  std::vector<Time> expectedTime;
  std::vector<uint8_t> expectedRsrp;

  // === Event A1 (serving cell becomes better than threshold) ===

  // With very low threshold
  LteRrcSap::ReportConfigEutra config;
  config.triggerType = LteRrcSap::ReportConfigEutra::EVENT;
  config.eventId = LteRrcSap::ReportConfigEutra::EVENT_A1;
  config.threshold1.choice = LteRrcSap::ThresholdEutra::THRESHOLD_RSRP;
  config.threshold1.range = 0;
  config.triggerQuantity = LteRrcSap::ReportConfigEutra::RSRP;
  config.reportInterval = LteRrcSap::ReportConfigEutra::MS120;
  expectedTime.clear ();
  expectedTime << 200 << 320 << 440 << 560 << 680 << 800 << 920 << 1040 << 1160 << 1280
               << 1400 << 1520 << 1640 << 1760 << 1880 << 2000 << 2120;
  expectedRsrp.clear ();
  expectedRsrp << 67 << 67 << 57 << 57 << 66 << 47 << 47 << 66 << 66 << 57
               << 51 << 51 << 47 << 47 << 51 << 57 << 57;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A1 with very low threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With normal threshold
  config.threshold1.range = 54;
  expectedTime.clear ();
  expectedTime << 200 << 320 << 440 << 560 << 680 << 1000 << 1120 << 1240 << 1360 << 2000
               << 2120;
  expectedRsrp.clear ();
  expectedRsrp << 67 << 67 << 57 << 57 << 66 << 66 << 66 << 57 << 57 << 57
               << 57;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A1 with normal threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With very high threshold
  config.threshold1.range = 97;
  expectedTime.clear ();
  expectedRsrp.clear ();
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A1 with very high threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // === Event A2 (serving cell becomes worse than threshold) ===

  // With very low threshold
  config.eventId = LteRrcSap::ReportConfigEutra::EVENT_A2;
  config.threshold1.range = 0;
  expectedTime.clear ();
  expectedRsrp.clear ();
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A2 with very low threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With normal threshold
  config.threshold1.range = 54;
  expectedTime.clear ();
  expectedTime << 800 << 920 << 1400 << 1520 << 1640 << 1760 << 1880;
  expectedRsrp.clear ();
  expectedRsrp << 47 << 47 << 51 << 51 << 47 << 47 << 51;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A2 with normal threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With very high threshold
  config.threshold1.range = 97;
  expectedTime.clear ();
  expectedTime << 200 << 320 << 440 << 560 << 680 << 800 << 920 << 1040 << 1160 << 1280
               << 1400 << 1520 << 1640 << 1760 << 1880 << 2000 << 2120;
  expectedRsrp.clear ();
  expectedRsrp << 67 << 67 << 57 << 57 << 66 << 47 << 47 << 66 << 66 << 57
               << 51 << 51 << 47 << 47 << 51 << 57 << 57;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A2 with very high threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // === Event A3 (neighbour becomes offset better than PCell) ===

  // With positive offset
  config.eventId = LteRrcSap::ReportConfigEutra::EVENT_A3;
  config.threshold1.range = 0;
  config.a3Offset = 7;
  expectedTime.clear ();
  expectedRsrp.clear ();
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A3 with positive offset",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With zero offset
  config.a3Offset = 0;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A3 with zero offset",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With negative offset
  config.a3Offset = -7;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A3 with negative offset",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // === Event A4 (neighbour becomes better than threshold) ===

  // With very low threshold
  config.eventId = LteRrcSap::ReportConfigEutra::EVENT_A4;
  config.threshold1.range = 0;
  config.a3Offset = 0;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A4 with very low threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With normal threshold
  config.threshold1.range = 54;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A4 with normal threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With very high threshold
  config.threshold1.range = 97;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A4 with very high threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // === Event A5 (PCell becomes worse than absolute threshold1 AND neighbour becomes better than another absolute threshold2) ===

  // With low-low threshold
  config.eventId = LteRrcSap::ReportConfigEutra::EVENT_A5;
  config.threshold1.range = 0;
  config.threshold2.range = 0;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A5 with low-low threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With low-normal threshold
  config.threshold2.range = 58;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A5 with low-normal threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With low-high threshold
  config.threshold2.range = 97;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A5 with low-high threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With normal-low threshold
  config.threshold1.range = 58;
  config.threshold2.range = 0;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A5 with normal-low threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With normal-normal threshold
  config.threshold2.range = 58;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A5 with normal-normal threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With normal-high threshold
  config.threshold2.range = 97;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A5 with normal-high threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With high-low threshold
  config.threshold1.range = 97;
  config.threshold2.range = 0;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A5 with high-low threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With high-normal threshold
  config.threshold2.range = 58;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A5 with high-normal threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With high-high threshold
  config.threshold2.range = 97;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase1 ("Piecewise test case 1 - Event A5 with high-high threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

} // end of void LteUeMeasurementsTestSuite::RunPiecewiseTestCase1 ()


void
LteUeMeasurementsTestSuite::RunPiecewiseTestCase2 ()
{
  NS_LOG_FUNCTION (this);
  std::vector<Time> expectedTime;
  std::vector<uint8_t> expectedRsrp;

  // === Event A1 (serving cell becomes better than threshold) ===

  // With very low threshold
  LteRrcSap::ReportConfigEutra config;
  config.triggerType = LteRrcSap::ReportConfigEutra::EVENT;
  config.eventId = LteRrcSap::ReportConfigEutra::EVENT_A1;
  config.threshold1.choice = LteRrcSap::ThresholdEutra::THRESHOLD_RSRP;
  config.threshold1.range = 0;
  config.triggerQuantity = LteRrcSap::ReportConfigEutra::RSRP;
  config.reportInterval = LteRrcSap::ReportConfigEutra::MS240;
  expectedTime.clear ();
  expectedTime << 200 << 440 << 680 << 920 << 1160 << 1400 << 1640 << 1880 << 2120;
  expectedRsrp.clear ();
  expectedRsrp << 73 << 63 << 72 << 52 << 72 << 59 << 52 << 56 << 59;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A1 with very low threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With normal threshold
  config.threshold1.range = 58;
  expectedTime.clear ();
  expectedTime << 200 << 440 << 680 << 1000 << 1240 << 2000;
  expectedRsrp.clear ();
  expectedRsrp << 73 << 63 << 72 << 72 << 59 << 59;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A1 with normal threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With very high threshold
  config.threshold1.range = 97;
  expectedTime.clear ();
  expectedRsrp.clear ();
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A1 with very high threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // === Event A2 (serving cell becomes worse than threshold) ===

  // With very low threshold
  config.eventId = LteRrcSap::ReportConfigEutra::EVENT_A2;
  config.threshold1.range = 0;
  expectedTime.clear ();
  expectedRsrp.clear ();
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A2 with very low threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With normal threshold
  config.threshold1.range = 58;
  expectedTime.clear ();
  expectedTime << 800 << 1400 << 1640 << 1880;
  expectedRsrp.clear ();
  expectedRsrp << 52 << 56 << 52 << 56;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A2 with normal threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With very high threshold
  config.threshold1.range = 97;
  expectedTime.clear ();
  expectedTime << 200 << 440 << 680 << 920 << 1160 << 1400 << 1640 << 1880 << 2120;
  expectedRsrp.clear ();
  expectedRsrp << 73 << 63 << 72 << 52 << 72 << 59 << 52 << 56 << 59;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A2 with very high threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // === Event A3 (neighbour becomes offset better than PCell) ===

  // With positive offset
  config.eventId = LteRrcSap::ReportConfigEutra::EVENT_A3;
  config.threshold1.range = 0;
  config.a3Offset = 7;
  expectedTime.clear ();
  expectedTime << 800 << 1600;
  expectedRsrp.clear ();
  expectedRsrp << 52 << 52;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A3 with positive offset",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With zero offset
  config.a3Offset = 0;
  expectedTime.clear ();
  expectedTime << 800 << 1400 << 1640 << 1880;
  expectedRsrp.clear ();
  expectedRsrp << 52 << 56 << 52 << 56;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A3 with zero offset",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With negative offset
  config.a3Offset = -7;
  expectedTime.clear ();
  expectedTime << 400 << 800 << 1200 << 1440 << 1680 << 1920 << 2160;
  expectedRsrp.clear ();
  expectedRsrp << 63 << 52 << 59 << 56 << 52 << 56 << 59;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A3 with negative offset",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // === Event A4 (neighbour becomes better than threshold) ===

  // With very low threshold
  config.eventId = LteRrcSap::ReportConfigEutra::EVENT_A4;
  config.threshold1.range = 0;
  config.a3Offset = 0;
  expectedTime.clear ();
  expectedTime << 200 << 440 << 680 << 920 << 1160 << 1400 << 1640 << 1880 << 2120;
  expectedRsrp.clear ();
  expectedRsrp << 73 << 63 << 72 << 52 << 72 << 59 << 52 << 56 << 59;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A4 with very low threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With normal threshold
  config.threshold1.range = 58;
  expectedTime.clear ();
  expectedTime << 400 << 800 << 1400 << 1640 << 1880;
  expectedRsrp.clear ();
  expectedRsrp << 63 << 52 << 56 << 52 << 56;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A4 with normal threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With very high threshold
  config.threshold1.range = 97;
  expectedTime.clear ();
  expectedRsrp.clear ();
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A4 with very high threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // === Event A5 (PCell becomes worse than absolute threshold1 AND neighbour becomes better than another absolute threshold2) ===

  // With low-low threshold
  config.eventId = LteRrcSap::ReportConfigEutra::EVENT_A5;
  config.threshold1.range = 0;
  config.threshold2.range = 0;
  expectedTime.clear ();
  expectedRsrp.clear ();
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A5 with low-low threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With low-normal threshold
  config.threshold2.range = 58;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A5 with low-normal threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With low-high threshold
  config.threshold2.range = 97;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A5 with low-high threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With normal-low threshold
  config.threshold1.range = 58;
  config.threshold2.range = 0;
  expectedTime.clear ();
  expectedTime << 800 << 1400 << 1640 << 1880;
  expectedRsrp.clear ();
  expectedRsrp << 52 << 56 << 52 << 56;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A5 with normal-low threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With normal-normal threshold
  config.threshold2.range = 58;
  expectedTime.clear ();
  expectedTime << 800 << 1400 << 1640 << 1880;
  expectedRsrp.clear ();
  expectedRsrp << 52 << 56 << 52 << 56;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A5 with normal-normal threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With normal-high threshold
  config.threshold2.range = 97;
  expectedTime.clear ();
  expectedRsrp.clear ();
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A5 with normal-high threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With high-low threshold
  config.threshold1.range = 97;
  config.threshold2.range = 0;
  expectedTime.clear ();
  expectedTime << 200 << 440 << 680 << 920 << 1160 << 1400 << 1640 << 1880 << 2120;
  expectedRsrp.clear ();
  expectedRsrp << 73 << 63 << 72 << 52 << 72 << 59 << 52 << 56 << 59;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A5 with high-low threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With high-normal threshold
  config.threshold2.range = 58;
  expectedTime.clear ();
  expectedTime << 400 << 800 << 1400 << 1640 << 1880;
  expectedRsrp.clear ();
  expectedRsrp << 63 << 52 << 56 << 52 << 56;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A5 with high-normal threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With high-high threshold
  config.threshold2.range = 97;
  expectedTime.clear ();
  expectedRsrp.clear ();
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A5 with high-high threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

} // end of void LteUeMeasurementsTestSuite::RunPiecewiseTestCase2 ()


void
LteUeMeasurementsTestSuite::RunHandoverTestCase ()
{
  NS_LOG_FUNCTION (this);

  // TODO
} // end of void LteUeMeasurementsTestSuite::RunHandoverTestCase ()


static LteUeMeasurementsTestSuite lteUeMeasurementsTestSuite;


/**
 * TestCase
 */

LteUeMeasurementsTestCase::LteUeMeasurementsTestCase (std::string name,
                                                      double d1, double d2,
                                                      double rsrpDbmUe1,
                                                      double rsrpDbmUe2,
                                                      double rsrqDbUe1,
                                                      double rsrqDbUe2)
  : TestCase (name),
    m_d1 (d1),
    m_d2 (d2),
    m_rsrpDbmUeServingCell (rsrpDbmUe1),
    m_rsrpDbmUeNeighborCell (rsrpDbmUe2),
    m_rsrqDbUeServingCell (rsrqDbUe1),
    m_rsrqDbUeNeighborCell (rsrqDbUe2)
{
  NS_LOG_INFO ("Test UE Measurements d1 = " << d1 << " m. and d2 = " << d2 << " m.");
}

LteUeMeasurementsTestCase::~LteUeMeasurementsTestCase ()
{
}

void
LteUeMeasurementsTestCase::DoRun (void)
{
  NS_LOG_INFO (this << " " << GetName ());

  Config::SetDefault ("ns3::LteSpectrumPhy::CtrlErrorModelEnabled", BooleanValue (false));
  Config::SetDefault ("ns3::LteSpectrumPhy::DataErrorModelEnabled", BooleanValue (false));
  Config::SetDefault ("ns3::LteAmc::AmcModel", EnumValue (LteAmc::PiroEW2010));
  Config::SetDefault ("ns3::LteAmc::Ber", DoubleValue (0.00005));
  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::FriisSpectrumPropagationLossModel"));
  lteHelper->SetAttribute ("UseIdealRrc", BooleanValue (false));

  LogComponentEnable ("LteUeMeasurementsTest", LOG_LEVEL_ALL);

  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes1;
  NodeContainer ueNodes2;
  enbNodes.Create (2);
  ueNodes1.Create (1);
  ueNodes2.Create (1);
  NodeContainer allNodes = NodeContainer (enbNodes, ueNodes1, ueNodes2);

  // the topology is the following:
  //         d2
  //  UE1-----------eNB2
  //   |             |
  // d1|             |d1
  //   |     d2      |
  //  eNB1----------UE2
  //
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));   // eNB1
  positionAlloc->Add (Vector (m_d2, m_d1, 0.0)); // eNB2
  positionAlloc->Add (Vector (0.0, m_d1, 0.0));  // UE1
  positionAlloc->Add (Vector (m_d2, 0.0, 0.0));  // UE2
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (allNodes);

  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs1;
  NetDeviceContainer ueDevs2;
  lteHelper->SetSchedulerType ("ns3::RrFfMacScheduler");
  lteHelper->SetSchedulerAttribute ("UlCqiFilter", EnumValue (FfMacScheduler::PUSCH_UL_CQI));
  enbDevs = lteHelper->InstallEnbDevice (enbNodes);
  ueDevs1 = lteHelper->InstallUeDevice (ueNodes1);
  ueDevs2 = lteHelper->InstallUeDevice (ueNodes2);

  // Setup pre-GSOC UE measurement configuration to the eNodeBs

  // Event A2
  LteRrcSap::ReportConfigEutra reportConfigA2;
  reportConfigA2.triggerType = LteRrcSap::ReportConfigEutra::EVENT;
  reportConfigA2.eventId = LteRrcSap::ReportConfigEutra::EVENT_A2;
  reportConfigA2.threshold1.choice = LteRrcSap::ThresholdEutra::THRESHOLD_RSRQ;
  reportConfigA2.threshold1.range = 34;
  reportConfigA2.hysteresis = 0;
  reportConfigA2.timeToTrigger = 0;
  reportConfigA2.triggerQuantity = LteRrcSap::ReportConfigEutra::RSRQ;
  reportConfigA2.reportQuantity = LteRrcSap::ReportConfigEutra::SAME_AS_TRIGGER_QUANTITY;
  reportConfigA2.maxReportCells = LteRrcSap::MaxReportCells;
  reportConfigA2.reportInterval = LteRrcSap::ReportConfigEutra::MS480;
  reportConfigA2.reportAmount = 255;

  // Event A4
  LteRrcSap::ReportConfigEutra reportConfigA4;
  reportConfigA4.triggerType = LteRrcSap::ReportConfigEutra::EVENT;
  reportConfigA4.eventId = LteRrcSap::ReportConfigEutra::EVENT_A4;
  reportConfigA4.threshold1.choice = LteRrcSap::ThresholdEutra::THRESHOLD_RSRQ;
  reportConfigA4.threshold1.range = 0;
  reportConfigA4.hysteresis = 0;
  reportConfigA4.timeToTrigger = 0;
  reportConfigA4.triggerQuantity = LteRrcSap::ReportConfigEutra::RSRQ;
  reportConfigA4.reportQuantity = LteRrcSap::ReportConfigEutra::SAME_AS_TRIGGER_QUANTITY;
  reportConfigA4.maxReportCells = LteRrcSap::MaxReportCells;
  reportConfigA4.reportInterval = LteRrcSap::ReportConfigEutra::MS480;
  reportConfigA4.reportAmount = 255;

  uint8_t measId;
  Ptr<LteEnbRrc> enbRrc1 = enbDevs.Get (0)->GetObject<LteEnbNetDevice> ()->GetRrc ();
  Ptr<LteEnbRrc> enbRrc2 = enbDevs.Get (1)->GetObject<LteEnbNetDevice> ()->GetRrc ();
  measId = enbRrc1->AddUeMeasReportConfig (reportConfigA2);
  NS_ASSERT (measId == 1);
  measId = enbRrc1->AddUeMeasReportConfig (reportConfigA4);
  NS_ASSERT (measId == 2);
  measId = enbRrc2->AddUeMeasReportConfig (reportConfigA2);
  NS_ASSERT (measId == 1);
  measId = enbRrc2->AddUeMeasReportConfig (reportConfigA4);
  NS_ASSERT (measId == 2);

  // Attach UEs to eNodeBs
  lteHelper->Attach (ueDevs1, enbDevs.Get (0));
  lteHelper->Attach (ueDevs2, enbDevs.Get (1));

  // Activate an EPS bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lteHelper->ActivateDataRadioBearer (ueDevs1, bearer);
  lteHelper->ActivateDataRadioBearer (ueDevs2, bearer);

  // Use testing chunk processor in the PHY layer
  // It will be used to test that the SNR is as intended
  // we plug in two instances, one for DL and one for UL

  Ptr<LtePhy> ue1Phy = ueDevs1.Get (0)->GetObject<LteUeNetDevice> ()->GetPhy ()->GetObject<LtePhy> ();
  Ptr<LteTestSinrChunkProcessor> testDlSinr1 = Create<LteTestSinrChunkProcessor> (ue1Phy);
  ue1Phy->GetDownlinkSpectrumPhy ()->AddDataSinrChunkProcessor (testDlSinr1);

  Ptr<LtePhy> enb1phy = enbDevs.Get (0)->GetObject<LteEnbNetDevice> ()->GetPhy ()->GetObject<LtePhy> ();
  Ptr<LteTestSinrChunkProcessor> testUlSinr1 = Create<LteTestSinrChunkProcessor> (enb1phy);
  enb1phy->GetUplinkSpectrumPhy ()->AddDataSinrChunkProcessor (testUlSinr1);

  Config::Connect ("/NodeList/2/DeviceList/0/LteUePhy/ReportUeMeasurements",
                   MakeBoundCallback (&ReportUeMeasurementsCallback, this));
  Config::Connect ("/NodeList/0/DeviceList/0/LteEnbRrc/RecvMeasurementReport",
                   MakeBoundCallback (&RecvMeasurementReportCallback, this));

  // same as above for eNB2 and UE2

  Ptr<LtePhy> ue2Phy = ueDevs2.Get (0)->GetObject<LteUeNetDevice> ()->GetPhy ()->GetObject<LtePhy> ();
  Ptr<LteTestSinrChunkProcessor> testDlSinr2 = Create<LteTestSinrChunkProcessor> (ue2Phy);
  ue2Phy->GetDownlinkSpectrumPhy ()->AddDataSinrChunkProcessor (testDlSinr2);

  Ptr<LtePhy> enb2phy = enbDevs.Get (1)->GetObject<LteEnbNetDevice> ()->GetPhy ()->GetObject<LtePhy> ();
  Ptr<LteTestSinrChunkProcessor> testUlSinr2 = Create<LteTestSinrChunkProcessor> (enb2phy);
  enb1phy->GetUplinkSpectrumPhy ()->AddDataSinrChunkProcessor (testUlSinr2);

  Config::Connect ("/NodeList/3/DeviceList/0/LteUePhy/ReportUeMeasurements",
                   MakeBoundCallback (&ReportUeMeasurementsCallback, this));
  Config::Connect ("/NodeList/1/DeviceList/0/LteEnbRrc/RecvMeasurementReport",
                   MakeBoundCallback (&RecvMeasurementReportCallback, this));

  // need to allow for RRC connection establishment + SRS
  Simulator::Stop (Seconds (0.800));
  Simulator::Run ();

  Simulator::Destroy ();

}


void
LteUeMeasurementsTestCase::ReportUeMeasurements (uint16_t rnti, uint16_t cellId,
                                                 double rsrp, double rsrq,
                                                 bool servingCell)
{
  // need to allow for RRC connection establishment + CQI feedback reception + UE measurements filtering (200 ms)
  if (Simulator::Now () > MilliSeconds (400))
    {
      if (servingCell)
        {
          NS_LOG_DEBUG ("UE serving cellId " << cellId << " Rxed RSRP " << rsrp << " thr " << m_rsrpDbmUeServingCell << " RSRQ " << rsrq << " thr " << m_rsrqDbUeServingCell);
          NS_TEST_ASSERT_MSG_EQ_TOL (m_rsrpDbmUeServingCell, rsrp, 0.2, "Wrong RSRP UE 1");
          NS_TEST_ASSERT_MSG_EQ_TOL (m_rsrqDbUeServingCell, rsrq, 0.2, "Wrong RSRQ UE 1");
        }
      else
        {
          NS_LOG_DEBUG ("UE neighbor cellId " << cellId << " Rxed RSRP " << rsrp << " thr " << m_rsrpDbmUeNeighborCell << " RSRQ " << rsrq << " thr " << m_rsrqDbUeNeighborCell);
          NS_TEST_ASSERT_MSG_EQ_TOL (m_rsrpDbmUeNeighborCell, rsrp, 0.2, "Wrong RSRP UE 2");
          NS_TEST_ASSERT_MSG_EQ_TOL (m_rsrqDbUeNeighborCell, rsrq, 0.2, "Wrong RSRQ UE ");
        }
    }
}


void
LteUeMeasurementsTestCase::RecvMeasurementReport (uint64_t imsi, uint16_t cellId, uint16_t rnti,
                                                  LteRrcSap::MeasurementReport meas)
{
  // need to allow for RRC connection establishment + CQI feedback reception + UE measurements filtering (200 ms)
  if (Simulator::Now () > MilliSeconds (400))
    {
      if (cellId == imsi)
        {
          NS_LOG_DEBUG (this << "Serving Cell: received IMSI " << imsi << " CellId " << cellId << " RNTI " << rnti
                             << " thr " << (uint16_t) EutranMeasurementMapping::Dbm2RsrpRange (m_rsrpDbmUeServingCell)
                             << " RSRP " << (uint16_t) meas.measResults.rsrpResult
                             << " RSRQ " << (uint16_t)meas.measResults.rsrqResult
                             << " thr " << (uint16_t) EutranMeasurementMapping::Db2RsrqRange (m_rsrqDbUeServingCell));
          NS_TEST_ASSERT_MSG_EQ (meas.measResults.rsrpResult,
                                 EutranMeasurementMapping::Dbm2RsrpRange (m_rsrpDbmUeServingCell),
                                 "Wrong RSRP ");
          NS_TEST_ASSERT_MSG_EQ (meas.measResults.rsrqResult,
                                 EutranMeasurementMapping::Db2RsrqRange (m_rsrqDbUeServingCell),
                                 "Wrong RSRQ ");
        }
      else
        {
          NS_LOG_DEBUG (this << "Neighbor cell: received IMSI " << imsi << " CellId " << cellId << " RNTI " << rnti
                             << " thr " << (uint16_t) EutranMeasurementMapping::Dbm2RsrpRange (m_rsrpDbmUeNeighborCell)
                             << " RSRP " << (uint16_t) meas.measResults.rsrpResult
                             << " RSRQ " << (uint16_t)meas.measResults.rsrqResult
                             << " thr " << (uint16_t) EutranMeasurementMapping::Db2RsrqRange (m_rsrqDbUeNeighborCell));
          NS_TEST_ASSERT_MSG_EQ (meas.measResults.rsrpResult,
                                 EutranMeasurementMapping::Dbm2RsrpRange (m_rsrpDbmUeNeighborCell),
                                 "Wrong RSRP ");
          NS_TEST_ASSERT_MSG_EQ (meas.measResults.rsrqResult,
                                 EutranMeasurementMapping::Db2RsrqRange (m_rsrqDbUeNeighborCell),
                                 "Wrong RSRQ ");
        }
    }
}


/*
 * Piecewise configuration test case with 1 eNodeB and 1 UE
 */

LteUeMeasurementsPiecewiseTestCase1::LteUeMeasurementsPiecewiseTestCase1 (
  std::string name, LteRrcSap::ReportConfigEutra config,
  std::vector<Time> expectedTime, std::vector<uint8_t> expectedRsrp)
  : TestCase (name),
    m_config (config),
    m_expectedTime (expectedTime),
    m_expectedRsrp (expectedRsrp)
{
  // input sanity check
  uint16_t size = m_expectedTime.size ();

  if (size != m_expectedRsrp.size ())
    {
      NS_FATAL_ERROR ("Vectors of expected results are not of the same size");
    }

  m_itExpectedTime = m_expectedTime.begin ();
  m_itExpectedRsrp = m_expectedRsrp.begin ();

  NS_LOG_INFO (this << " name=" << name);
}


LteUeMeasurementsPiecewiseTestCase1::~LteUeMeasurementsPiecewiseTestCase1 ()
{
  NS_LOG_FUNCTION (this);
}


void
LteUeMeasurementsPiecewiseTestCase1::DoRun ()
{
  NS_LOG_INFO (this << " " << GetName ());

  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  lteHelper->SetAttribute ("PathlossModel",
                           StringValue ("ns3::FriisSpectrumPropagationLossModel"));
  lteHelper->SetAttribute ("UseIdealRrc", BooleanValue (true));

  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes;
  enbNodes.Create (1);
  ueNodes.Create (1);

  /*
   * The topology is the following:
   *
   * eNodeB     UE
   *    |       |
   *    x ----- x --------- x --------------- x ------------------- x
   *      100 m |   200 m   |      300 m      |        400 m        |
   *            |           |                 |                     |
   *         VeryNear      Near              Far                 VeryFar
   */

  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0)); // eNodeB
  positionAlloc->Add (Vector (100.0, 0.0, 0.0)); // UE
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (enbNodes);
  mobility.Install (ueNodes);
  m_ueMobility = ueNodes.Get (0)->GetObject<MobilityModel> ();

  // Disable layer-3 filtering
  Config::SetDefault ("ns3::LteEnbRrc::RsrpFilterCoefficient",
                      UintegerValue (0));

  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
  lteHelper->SetSchedulerType ("ns3::RrFfMacScheduler");
  lteHelper->SetSchedulerAttribute ("UlCqiFilter",
                                    EnumValue (FfMacScheduler::PUSCH_UL_CQI));
  enbDevs = lteHelper->InstallEnbDevice (enbNodes);
  ueDevs = lteHelper->InstallUeDevice (ueNodes);

  // Setup UE measurement configuration
  Ptr<LteEnbRrc> enbRrc = enbDevs.Get (0)->GetObject<LteEnbNetDevice> ()->GetRrc ();
  uint8_t measId = enbRrc->AddUeMeasReportConfig (m_config);
  NS_ASSERT (measId == 1);

  // Attach UE to eNodeB
  lteHelper->Attach (ueDevs.Get (0), enbDevs.Get (0));

  // Activate an EPS bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lteHelper->ActivateDataRadioBearer (ueDevs, bearer);

  // Connect to trace sources
  Config::Connect ("/NodeList/0/DeviceList/0/LteEnbRrc/RecvMeasurementReport",
                   MakeCallback (&LteUeMeasurementsPiecewiseTestCase1::RecvMeasurementReportCallback,
                                 this));

  /*
   * Schedule "teleports"
   *          0                   1                   2
   *          +-------------------+-------------------+---------> time
   * VeryNear |------  ----    ----                    --------
   *     Near |                    ----            ----
   *      Far |                        ----    ----
   *  VeryFar |      --    ----            ----
   */
  Simulator::Schedule (MilliSeconds (301),
                       &LteUeMeasurementsPiecewiseTestCase1::TeleportVeryFar, this);
  Simulator::Schedule (MilliSeconds (401),
                       &LteUeMeasurementsPiecewiseTestCase1::TeleportVeryNear, this);
  Simulator::Schedule (MilliSeconds (601),
                       &LteUeMeasurementsPiecewiseTestCase1::TeleportVeryFar, this);
  Simulator::Schedule (MilliSeconds (801),
                       &LteUeMeasurementsPiecewiseTestCase1::TeleportVeryNear, this);
  Simulator::Schedule (MilliSeconds (1001),
                       &LteUeMeasurementsPiecewiseTestCase1::TeleportNear, this);
  Simulator::Schedule (MilliSeconds (1201),
                       &LteUeMeasurementsPiecewiseTestCase1::TeleportFar, this);
  Simulator::Schedule (MilliSeconds (1401),
                       &LteUeMeasurementsPiecewiseTestCase1::TeleportVeryFar, this);
  Simulator::Schedule (MilliSeconds (1601),
                       &LteUeMeasurementsPiecewiseTestCase1::TeleportFar, this);
  Simulator::Schedule (MilliSeconds (1801),
                       &LteUeMeasurementsPiecewiseTestCase1::TeleportNear, this);
  Simulator::Schedule (MilliSeconds (2001),
                       &LteUeMeasurementsPiecewiseTestCase1::TeleportVeryNear, this);

  // Run simulation
  Simulator::Stop (Seconds (2.201));
  Simulator::Run ();
  Simulator::Destroy ();

} // end of void LteUeMeasurementsPiecewiseTestCase1::DoRun ()


void
LteUeMeasurementsPiecewiseTestCase1::DoTeardown ()
{
  NS_LOG_FUNCTION (this);
  bool hasEnded = m_itExpectedTime == m_expectedTime.end ();
  NS_TEST_ASSERT_MSG_EQ (hasEnded, true,
                         "Reporting should have occurred at " << m_itExpectedTime->GetSeconds () << "s");
  hasEnded = m_itExpectedRsrp == m_expectedRsrp.end ();
  NS_ASSERT (hasEnded);
}


void
LteUeMeasurementsPiecewiseTestCase1::RecvMeasurementReportCallback (
  std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti,
  LteRrcSap::MeasurementReport report)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (rnti == 1);
  NS_ASSERT (cellId == 1);

  // verifying the report completeness
  LteRrcSap::MeasResults measResults = report.measResults;
  NS_TEST_ASSERT_MSG_EQ (measResults.measId, 1,
                         "Unexpected measurement identity");
  double rsrpDbm = EutranMeasurementMapping::RsrpRange2Dbm (measResults.rsrpResult);
  double rsrqDb = EutranMeasurementMapping::RsrqRange2Db (measResults.rsrqResult);
  NS_LOG_DEBUG (this << " rsrp=" << (uint16_t) measResults.rsrpResult
                     << " (" << rsrpDbm << " dBm)"
                     << " rsrq=" << (uint16_t) measResults.rsrqResult
                     << " (" << rsrqDb << " dB)");
  NS_TEST_ASSERT_MSG_EQ (measResults.haveMeasResultNeighCells, true,
                         "Report does not have neighboring cells information");
  NS_TEST_ASSERT_MSG_EQ (measResults.measResultListEutra.size (), 0,
                         "Unexpected report size");

  // verifying the report timing
  bool hasEnded = m_itExpectedTime == m_expectedTime.end ();
  NS_TEST_ASSERT_MSG_EQ (hasEnded, false,
                         "Reporting should not have occurred at "
                         << Simulator::Now ().GetSeconds () << "s");
  if (!hasEnded)
    {
      // comparison with milliseconds to avoid floating-point comparison
      uint64_t timeNowMs = Simulator::Now ().GetMilliSeconds ();
      uint64_t timeExpectedMs = m_itExpectedTime->GetMilliSeconds ();
      NS_TEST_ASSERT_MSG_EQ (timeNowMs, timeExpectedMs,
                             "Reporting should not have occurred at this time");
      m_itExpectedTime++;

      // verifying the report RSRP content
      hasEnded = m_itExpectedRsrp == m_expectedRsrp.end ();
      NS_ASSERT (!hasEnded);
      uint16_t observedRsrp = measResults.rsrpResult;
      uint16_t referenceRsrp = *m_itExpectedRsrp;
      NS_TEST_ASSERT_MSG_EQ (observedRsrp, referenceRsrp,
                             "The RSRP observed differs with the reference RSRP");
      m_itExpectedRsrp++;
    }

} // end of LteUeMeasurementsPiecewiseTestCase1::RecvMeasurementReportCallback


void
LteUeMeasurementsPiecewiseTestCase1::TeleportVeryNear ()
{
  NS_LOG_FUNCTION (this);
  m_ueMobility->SetPosition (Vector (100.0, 0.0, 0.0));
}


void
LteUeMeasurementsPiecewiseTestCase1::TeleportNear ()
{
  NS_LOG_FUNCTION (this);
  m_ueMobility->SetPosition (Vector (300.0, 0.0, 0.0));
}


void
LteUeMeasurementsPiecewiseTestCase1::TeleportFar ()
{
  NS_LOG_FUNCTION (this);
  m_ueMobility->SetPosition (Vector (600.0, 0.0, 0.0));
}


void
LteUeMeasurementsPiecewiseTestCase1::TeleportVeryFar ()
{
  NS_LOG_FUNCTION (this);
  m_ueMobility->SetPosition (Vector (1000.0, 0.0, 0.0));
}


/*
 * Piecewise configuration test case with 2 eNodeB and 1 UE
 */

LteUeMeasurementsPiecewiseTestCase2::LteUeMeasurementsPiecewiseTestCase2 (
  std::string name, LteRrcSap::ReportConfigEutra config,
  std::vector<Time> expectedTime, std::vector<uint8_t> expectedRsrp)
  : TestCase (name),
    m_config (config),
    m_expectedTime (expectedTime),
    m_expectedRsrp (expectedRsrp)
{
  // input sanity check
  uint16_t size = m_expectedTime.size ();

  if (size != m_expectedRsrp.size ())
    {
      NS_FATAL_ERROR ("Vectors of expected results are not of the same size");
    }

  m_itExpectedTime = m_expectedTime.begin ();
  m_itExpectedRsrp = m_expectedRsrp.begin ();

  NS_LOG_INFO (this << " name=" << name);
}


LteUeMeasurementsPiecewiseTestCase2::~LteUeMeasurementsPiecewiseTestCase2 ()
{
  NS_LOG_FUNCTION (this);
}


void
LteUeMeasurementsPiecewiseTestCase2::DoRun ()
{
  NS_LOG_INFO (this << " " << GetName ());

  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  lteHelper->SetAttribute ("PathlossModel",
                           StringValue ("ns3::FriisSpectrumPropagationLossModel"));
  lteHelper->SetAttribute ("UseIdealRrc", BooleanValue (true));

  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes;
  enbNodes.Create (2);
  ueNodes.Create (1);

  /*
   * The topology is the following:
   *
   * eNodeB    UE                                                eNodeB
   *    |      |                                                    |
   *    x ---- x --------------- x ------- x --------------- x ---- x
   *      50 m |      200 m      |  100 m  |      200 m      | 50 m
   *           |                 |         |                 |
   *        VeryNear            Near      Far             VeryFar
   */

  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0)); // Serving eNodeB
  positionAlloc->Add (Vector (600.0, 0.0, 0.0)); // Neighbour eNodeB
  positionAlloc->Add (Vector (50.0, 0.0, 0.0)); // UE
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (enbNodes);
  mobility.Install (ueNodes);
  m_ueMobility = ueNodes.Get (0)->GetObject<MobilityModel> ();

  // Disable layer-3 filtering
  Config::SetDefault ("ns3::LteEnbRrc::RsrpFilterCoefficient",
                      UintegerValue (0));

  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
  lteHelper->SetSchedulerType ("ns3::RrFfMacScheduler");
  lteHelper->SetSchedulerAttribute ("UlCqiFilter",
                                    EnumValue (FfMacScheduler::PUSCH_UL_CQI));
  enbDevs = lteHelper->InstallEnbDevice (enbNodes);
  ueDevs = lteHelper->InstallUeDevice (ueNodes);

  // Setup UE measurement configuration in serving cell
  Ptr<LteEnbRrc> enbRrc1 = enbDevs.Get (0)->GetObject<LteEnbNetDevice> ()->GetRrc ();
  uint8_t measId = enbRrc1->AddUeMeasReportConfig (m_config);
  NS_ASSERT (measId == 1);

  // Disable handover in neighbour cell
  Ptr<LteEnbRrc> enbRrc2 = enbDevs.Get (1)->GetObject<LteEnbNetDevice> ()->GetRrc ();
  enbRrc2->SetAttribute ("AdmitHandoverRequest", BooleanValue (false));

  // Attach UE to serving eNodeB
  lteHelper->Attach (ueDevs.Get (0), enbDevs.Get (0));

  // Activate an EPS bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lteHelper->ActivateDataRadioBearer (ueDevs, bearer);

  // Connect to trace sources in serving eNodeB
  Config::Connect ("/NodeList/0/DeviceList/0/LteEnbRrc/RecvMeasurementReport",
                   MakeCallback (&LteUeMeasurementsPiecewiseTestCase2::RecvMeasurementReportCallback,
                                 this));

  /*
   * Schedule "teleports"
   *          0                   1                   2
   *          +-------------------+-------------------+---------> time
   * VeryNear |------  ----    ----                    --------
   *     Near |                    ----            ----
   *      Far |                        ----    ----
   *  VeryFar |      --    ----            ----
   */
  Simulator::Schedule (MilliSeconds (301),
                       &LteUeMeasurementsPiecewiseTestCase2::TeleportVeryFar, this);
  Simulator::Schedule (MilliSeconds (401),
                       &LteUeMeasurementsPiecewiseTestCase2::TeleportVeryNear, this);
  Simulator::Schedule (MilliSeconds (601),
                       &LteUeMeasurementsPiecewiseTestCase2::TeleportVeryFar, this);
  Simulator::Schedule (MilliSeconds (801),
                       &LteUeMeasurementsPiecewiseTestCase2::TeleportVeryNear, this);
  Simulator::Schedule (MilliSeconds (1001),
                       &LteUeMeasurementsPiecewiseTestCase2::TeleportNear, this);
  Simulator::Schedule (MilliSeconds (1201),
                       &LteUeMeasurementsPiecewiseTestCase2::TeleportFar, this);
  Simulator::Schedule (MilliSeconds (1401),
                       &LteUeMeasurementsPiecewiseTestCase2::TeleportVeryFar, this);
  Simulator::Schedule (MilliSeconds (1601),
                       &LteUeMeasurementsPiecewiseTestCase2::TeleportFar, this);
  Simulator::Schedule (MilliSeconds (1801),
                       &LteUeMeasurementsPiecewiseTestCase2::TeleportNear, this);
  Simulator::Schedule (MilliSeconds (2001),
                       &LteUeMeasurementsPiecewiseTestCase2::TeleportVeryNear, this);

  // Run simulation
  Simulator::Stop (Seconds (2.201));
  Simulator::Run ();
  Simulator::Destroy ();

} // end of void LteUeMeasurementsPiecewiseTestCase2::DoRun ()


void
LteUeMeasurementsPiecewiseTestCase2::DoTeardown ()
{
  NS_LOG_FUNCTION (this);
  bool hasEnded = m_itExpectedTime == m_expectedTime.end ();
  NS_TEST_ASSERT_MSG_EQ (hasEnded, true,
                         "Reporting should have occurred at " << m_itExpectedTime->GetSeconds () << "s");
  hasEnded = m_itExpectedRsrp == m_expectedRsrp.end ();
  NS_ASSERT (hasEnded);
}


void
LteUeMeasurementsPiecewiseTestCase2::RecvMeasurementReportCallback (
  std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti,
  LteRrcSap::MeasurementReport report)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (rnti == 1);
  NS_ASSERT (cellId == 1);

  // verifying the report completeness
  LteRrcSap::MeasResults measResults = report.measResults;
  NS_TEST_ASSERT_MSG_EQ (measResults.measId, 1,
                         "Unexpected measurement identity");
  double rsrpDbm = EutranMeasurementMapping::RsrpRange2Dbm (measResults.rsrpResult);
  double rsrqDb = EutranMeasurementMapping::RsrqRange2Db (measResults.rsrqResult);
  NS_LOG_DEBUG (this << " Serving cellId=" << cellId
                     << " rsrp=" << (uint16_t) measResults.rsrpResult
                     << " (" << rsrpDbm << " dBm)"
                     << " rsrq=" << (uint16_t) measResults.rsrqResult
                     << " (" << rsrqDb << " dB)");
  NS_TEST_ASSERT_MSG_EQ (measResults.haveMeasResultNeighCells, true,
                         "Report does not have neighboring cells information");

  // verifying reported best cells
  if (measResults.measResultListEutra.size () == 1)
    {
      std::list<LteRrcSap::MeasResultEutra>::iterator it = measResults.measResultListEutra.begin ();
      NS_ASSERT (it != measResults.measResultListEutra.end ());
      NS_ASSERT (it->physCellId == 2);
      NS_TEST_ASSERT_MSG_EQ (it->haveCgiInfo, false,
                             "Report contains cgi-info, which is not supported");
      NS_TEST_ASSERT_MSG_EQ (it->haveRsrpResult, true,
                             "Report does not contain measured RSRP result");
      NS_TEST_ASSERT_MSG_EQ (it->haveRsrqResult, true,
                             "Report does not contain measured RSRQ result");
      rsrpDbm = EutranMeasurementMapping::RsrpRange2Dbm (it->rsrpResult);
      rsrqDb = EutranMeasurementMapping::RsrqRange2Db (it->rsrqResult);
      NS_LOG_DEBUG (this << " Neighbour cellId=" << it->physCellId
                         << " rsrp=" << (uint16_t) it->rsrpResult
                         << " (" << rsrpDbm << " dBm)"
                         << " rsrq=" << (uint16_t) it->rsrqResult
                         << " (" << rsrqDb << " dB)");
    }

  // verifying the report timing
  bool hasEnded = m_itExpectedTime == m_expectedTime.end ();
  NS_TEST_ASSERT_MSG_EQ (hasEnded, false,
                         "Reporting should not have occurred at "
                         << Simulator::Now ().GetSeconds () << "s");
  if (!hasEnded)
    {
      // comparison with milliseconds to avoid floating-point comparison
      uint64_t timeNowMs = Simulator::Now ().GetMilliSeconds ();
      uint64_t timeExpectedMs = m_itExpectedTime->GetMilliSeconds ();
      NS_TEST_ASSERT_MSG_EQ (timeNowMs, timeExpectedMs,
                             "Reporting should not have occurred at this time");
      m_itExpectedTime++;

      // verifying the report RSRP content
      hasEnded = m_itExpectedRsrp == m_expectedRsrp.end ();
      NS_ASSERT (!hasEnded);
      uint16_t observedRsrp = measResults.rsrpResult;
      uint16_t referenceRsrp = *m_itExpectedRsrp;
      NS_TEST_ASSERT_MSG_EQ (observedRsrp, referenceRsrp,
                             "The RSRP observed differs with the reference RSRP");
      m_itExpectedRsrp++;
    }

} // end of void LteUeMeasurementsPiecewiseTestCase2::RecvMeasurementReportCallback


void
LteUeMeasurementsPiecewiseTestCase2::TeleportVeryNear ()
{
  NS_LOG_FUNCTION (this);
  m_ueMobility->SetPosition (Vector (50.0, 0.0, 0.0));
}


void
LteUeMeasurementsPiecewiseTestCase2::TeleportNear ()
{
  NS_LOG_FUNCTION (this);
  m_ueMobility->SetPosition (Vector (250.0, 0.0, 0.0));
}


void
LteUeMeasurementsPiecewiseTestCase2::TeleportFar ()
{
  NS_LOG_FUNCTION (this);
  m_ueMobility->SetPosition (Vector (350.0, 0.0, 0.0));
}


void
LteUeMeasurementsPiecewiseTestCase2::TeleportVeryFar ()
{
  NS_LOG_FUNCTION (this);
  m_ueMobility->SetPosition (Vector (550.0, 0.0, 0.0));
}


} // namespace ns3

