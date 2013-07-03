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

#include "lte-test-ue-measurements-piecewise-2.h"

#include <ns3/simulator.h>
#include <ns3/log.h>
#include <ns3/callback.h>
#include <ns3/config.h>
#include <ns3/boolean.h>
#include <ns3/enum.h>

#include <ns3/lte-helper.h>
#include <ns3/internet-stack-helper.h>
#include <ns3/point-to-point-helper.h>
#include <ns3/ipv4-address-helper.h>
#include <ns3/ipv4-static-routing-helper.h>
#include <ns3/mobility-helper.h>
#include <ns3/node-container.h>
#include <ns3/net-device-container.h>
#include <ns3/ipv4-interface-container.h>

#include <ns3/lte-ue-net-device.h>
#include <ns3/lte-enb-net-device.h>
#include <ns3/lte-enb-rrc.h>
#include <ns3/ff-mac-scheduler.h>

NS_LOG_COMPONENT_DEFINE ("LteUeMeasurementsPiecewiseTest2");

namespace ns3 {


/*
 * Overloaded operators, for the convenience of defining test cases, within a
 * macro guard to avoid defining multiple times.
 */

#ifndef LTE_TEST_UE_MEASUREMENTS_OPERATORS
#define LTE_TEST_UE_MEASUREMENTS_OPERATORS

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

#endif /* LTE_TEST_UE_MEASUREMENTS_OPERATORS */



/*
 * Test Suite
 */

LteUeMeasurementsPiecewiseTestSuite2::LteUeMeasurementsPiecewiseTestSuite2 ()
  : TestSuite ("lte-ue-measurements-piecewise-2", SYSTEM)
{
  //LogComponentEnableAll (LOG_PREFIX_ALL);
  //LogComponentEnable ("LteUeMeasurementsPiecewiseTest2", LOG_INFO);
  //LogComponentEnable ("LteUeMeasurementsPiecewiseTest2", LOG_DEBUG);
  //LogComponentEnable ("LteUeMeasurementsPiecewiseTest2", LOG_FUNCTION);
  //LogComponentEnable ("LteEnbRrc", LOG_FUNCTION);
  //LogComponentEnable ("LteEnbRrc", LOG_LOGIC);
  //LogComponentEnable ("LteUeRrc", LOG_INFO);
  //LogComponentEnable ("LteUeRrc", LOG_WARN);
  //LogComponentEnable ("LteUeRrc", LOG_LOGIC);

  std::vector<Time> expectedTime;
  std::vector<uint8_t> expectedRsrp;

  /*
   * Higher level of fullness/duration are given to Event A1 and A2 because they
   * are supposed to be more intensively tested in Piecewise configuration #1.
   */

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
               TestCase::TAKES_FOREVER);

  // With hysteresis
  config.hysteresis = 6;
  expectedTime.clear ();
  expectedTime << 200 << 440 << 680 << 1000 << 1240 << 1480 << 2200;
  expectedRsrp.clear ();
  expectedRsrp << 73 << 63 << 72 << 72 << 59 << 56 << 72;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A1 with hysteresis",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With very high threshold
  config.threshold1.range = 97;
  config.hysteresis = 0;
  expectedTime.clear ();
  expectedRsrp.clear ();
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A1 with very high threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::TAKES_FOREVER);

  // === Event A2 (serving cell becomes worse than threshold) ===

  // With very low threshold
  config.eventId = LteRrcSap::ReportConfigEutra::EVENT_A2;
  config.threshold1.range = 0;
  expectedTime.clear ();
  expectedRsrp.clear ();
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A2 with very low threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::TAKES_FOREVER);

  // With normal threshold
  config.threshold1.range = 58;
  expectedTime.clear ();
  expectedTime << 800 << 1400 << 1640 << 1880;
  expectedRsrp.clear ();
  expectedRsrp << 52 << 56 << 52 << 56;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A2 with normal threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::TAKES_FOREVER);

  // With hysteresis
  config.hysteresis = 6;
  expectedTime.clear ();
  expectedTime << 800 << 1600 << 1840 << 2080;
  expectedRsrp.clear ();
  expectedRsrp << 52 << 52 << 56 << 59;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A2 with hysteresis",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With very high threshold
  config.threshold1.range = 97;
  config.hysteresis = 0;
  expectedTime.clear ();
  expectedTime << 200 << 440 << 680 << 920 << 1160 << 1400 << 1640 << 1880 << 2120;
  expectedRsrp.clear ();
  expectedRsrp << 73 << 63 << 72 << 52 << 72 << 59 << 52 << 56 << 59;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A2 with very high threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::TAKES_FOREVER);

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
               TestCase::QUICK);

  // With zero offset
  config.a3Offset = 0;
  expectedTime.clear ();
  expectedTime << 800 << 1400 << 1640 << 1880;
  expectedRsrp.clear ();
  expectedRsrp << 52 << 56 << 52 << 56;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A3 with zero offset",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With hysteresis
  config.hysteresis = 6;
  expectedTime.clear ();
  expectedTime << 800 << 1600 << 1840 << 2080;
  expectedRsrp.clear ();
  expectedRsrp << 52 << 52 << 56 << 59;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A3 with hysteresis",
                                                        config, expectedTime, expectedRsrp),
               TestCase::QUICK);

  // With negative offset
  config.a3Offset = -7;
  config.hysteresis = 0;
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
               TestCase::QUICK);

  // With normal threshold
  config.threshold1.range = 58;
  expectedTime.clear ();
  expectedTime << 400 << 800 << 1400 << 1640 << 1880;
  expectedRsrp.clear ();
  expectedRsrp << 63 << 52 << 56 << 52 << 56;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A4 with normal threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // With hysteresis
  config.hysteresis = 6;
  expectedTime.clear ();
  expectedTime << 400 << 800 << 1600 << 1840 << 2080;
  expectedRsrp.clear ();
  expectedRsrp << 63 << 52 << 52 << 56 << 59;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A4 with hysteresis",
                                                        config, expectedTime, expectedRsrp),
               TestCase::QUICK);

  // With very high threshold
  config.threshold1.range = 97;
  config.hysteresis = 0;
  expectedTime.clear ();
  expectedRsrp.clear ();
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A4 with very high threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::TAKES_FOREVER);

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
               TestCase::TAKES_FOREVER);

  // With low-high threshold
  config.threshold2.range = 97;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A5 with low-high threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::TAKES_FOREVER);

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

  // With hysteresis
  config.hysteresis = 6;
  expectedTime.clear ();
  expectedTime << 800 << 1600 << 1840 << 2080;
  expectedRsrp.clear ();
  expectedRsrp << 52 << 52 << 56 << 59;
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A5 with hysteresis",
                                                        config, expectedTime, expectedRsrp),
               TestCase::QUICK);

  // With normal-high threshold
  config.threshold2.range = 97;
  config.hysteresis = 0;
  expectedTime.clear ();
  expectedRsrp.clear ();
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A5 with normal-high threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::TAKES_FOREVER);

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
               TestCase::TAKES_FOREVER);

  // With high-high threshold
  config.threshold2.range = 97;
  expectedTime.clear ();
  expectedRsrp.clear ();
  AddTestCase (new LteUeMeasurementsPiecewiseTestCase2 ("Piecewise test case 2 - Event A5 with high-high threshold",
                                                        config, expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

} // end of LteUeMeasurementsPiecewiseTestSuite2::LteUeMeasurementsPiecewiseTestSuite2


static LteUeMeasurementsPiecewiseTestSuite2 lteUeMeasurementsPiecewiseTestSuite2;



/*
 * Test Case
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
  NS_LOG_FUNCTION (this << context);
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


} // end of namespace ns3
