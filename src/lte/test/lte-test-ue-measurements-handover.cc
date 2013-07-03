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

#include "lte-test-ue-measurements-handover.h"

#include <ns3/simulator.h>
#include <ns3/log.h>
#include <ns3/callback.h>
#include <ns3/config.h>
#include <ns3/boolean.h>

#include <ns3/lte-helper.h>
#include <ns3/epc-helper.h>
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


NS_LOG_COMPONENT_DEFINE ("LteUeMeasurementsHandoverTest");

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

LteUeMeasurementsHandoverTestSuite::LteUeMeasurementsHandoverTestSuite ()
  : TestSuite ("lte-ue-measurements-handover", SYSTEM)
{
  //LogComponentEnableAll (LOG_PREFIX_ALL);
  //LogComponentEnable ("LteUeMeasurementsHandoverTest", LOG_INFO);
  //LogComponentEnable ("LteUeMeasurementsHandoverTest", LOG_DEBUG);
  //LogComponentEnable ("LteUeMeasurementsHandoverTest", LOG_FUNCTION);
  //LogComponentEnable ("LteEnbRrc", LOG_FUNCTION);
  //LogComponentEnable ("LteEnbRrc", LOG_LOGIC);
  //LogComponentEnable ("LteUeRrc", LOG_INFO);
  //LogComponentEnable ("LteUeRrc", LOG_WARN);
  //LogComponentEnable ("LteUeRrc", LOG_LOGIC);

  std::vector<Time> expectedTime;
  std::vector<uint8_t> expectedRsrp;

  // === Report interval difference ===

  LteRrcSap::ReportConfigEutra sourceConfig;
  sourceConfig.triggerType = LteRrcSap::ReportConfigEutra::EVENT;
  sourceConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A1;
  sourceConfig.threshold1.choice = LteRrcSap::ThresholdEutra::THRESHOLD_RSRP;
  sourceConfig.threshold1.range = 0;
  sourceConfig.triggerQuantity = LteRrcSap::ReportConfigEutra::RSRP;

  LteRrcSap::ReportConfigEutra targetConfig;
  targetConfig.triggerType = LteRrcSap::ReportConfigEutra::EVENT;
  targetConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A1;
  targetConfig.threshold1.choice = LteRrcSap::ThresholdEutra::THRESHOLD_RSRP;
  targetConfig.threshold1.range = 0;
  targetConfig.triggerQuantity = LteRrcSap::ReportConfigEutra::RSRP;

  // decreasing report interval
  sourceConfig.reportInterval = LteRrcSap::ReportConfigEutra::MS480;
  targetConfig.reportInterval = LteRrcSap::ReportConfigEutra::MS240;
  expectedTime.clear ();
  expectedTime << 200 << 680 << 1200 << 1440 << 1680 << 1920;
  expectedRsrp.clear ();
  expectedRsrp << 55 << 55 << 53 << 53 << 53 << 53;
  AddTestCase (new LteUeMeasurementsHandoverTestCase ("Handover test case - decreasing report interval",
                                                      sourceConfig, targetConfig,
                                                      expectedTime, expectedRsrp),
               TestCase::TAKES_FOREVER);

  // increasing report interval
  sourceConfig.reportInterval = LteRrcSap::ReportConfigEutra::MS120;
  targetConfig.reportInterval = LteRrcSap::ReportConfigEutra::MS640;
  expectedTime.clear ();
  expectedTime << 200 << 320 << 440 << 560 << 680 << 800 << 920 << 1200 << 1840;
  expectedRsrp.clear ();
  expectedRsrp << 55 << 55 << 55 << 55 << 55 << 55 << 55 << 53 << 53;
  AddTestCase (new LteUeMeasurementsHandoverTestCase ("Handover test case - increasing report interval",
                                                      sourceConfig, targetConfig,
                                                      expectedTime, expectedRsrp),
               TestCase::QUICK);

  // === Event difference ===

  sourceConfig.reportInterval = LteRrcSap::ReportConfigEutra::MS240;
  targetConfig.reportInterval = LteRrcSap::ReportConfigEutra::MS240;
  sourceConfig.threshold1.range = 54;
  sourceConfig.threshold2.range = 54;
  sourceConfig.a3Offset = 1;
  targetConfig.threshold1.range = 54;
  targetConfig.threshold2.range = 54;
  targetConfig.a3Offset = 1;

  // Event A1 to Event A2
  sourceConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A1;
  targetConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A2;
  expectedTime.clear ();
  expectedTime << 200 << 440 << 680 << 920 << 1200 << 1440 << 1680 << 1920;
  expectedRsrp.clear ();
  expectedRsrp << 55 << 55 << 55 << 55 << 53 << 53 << 53 << 53;
  AddTestCase (new LteUeMeasurementsHandoverTestCase ("Handover test case - A1 to A2",
                                                      sourceConfig, targetConfig,
                                                      expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // Event A2 to Event A1
  sourceConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A2;
  targetConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A1;
  expectedTime.clear ();
  expectedRsrp.clear ();
  AddTestCase (new LteUeMeasurementsHandoverTestCase ("Handover test case - A2 to A1",
                                                      sourceConfig, targetConfig,
                                                      expectedTime, expectedRsrp),
               TestCase::TAKES_FOREVER);

  // Event A3 to Event A4
  sourceConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A3;
  targetConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A4;
  expectedTime.clear ();
  expectedTime << 1200 << 1440 << 1680 << 1920;
  expectedRsrp.clear ();
  expectedRsrp << 53 << 53 << 53 << 53;
  AddTestCase (new LteUeMeasurementsHandoverTestCase ("Handover test case - A3 to A4",
                                                      sourceConfig, targetConfig,
                                                      expectedTime, expectedRsrp),
               TestCase::TAKES_FOREVER);

  // Event A4 to Event A3
  sourceConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A4;
  targetConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A3;
  expectedTime.clear ();
  expectedTime << 1200 << 1440 << 1680 << 1920;
  expectedRsrp.clear ();
  expectedRsrp << 53 << 53 << 53 << 53;
  AddTestCase (new LteUeMeasurementsHandoverTestCase ("Handover test case - A4 to A3",
                                                      sourceConfig, targetConfig,
                                                      expectedTime, expectedRsrp),
               TestCase::QUICK);

  // Event A2 to Event A3
  sourceConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A2;
  targetConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A3;
  expectedTime.clear ();
  expectedTime << 1200 << 1440 << 1680 << 1920;
  expectedRsrp.clear ();
  expectedRsrp << 53 << 53 << 53 << 53;
  AddTestCase (new LteUeMeasurementsHandoverTestCase ("Handover test case - A2 to A3",
                                                      sourceConfig, targetConfig,
                                                      expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // Event A3 to Event A2
  sourceConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A3;
  targetConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A2;
  expectedTime.clear ();
  expectedTime << 1200 << 1440 << 1680 << 1920;
  expectedRsrp.clear ();
  expectedRsrp << 53 << 53 << 53 << 53;
  AddTestCase (new LteUeMeasurementsHandoverTestCase ("Handover test case - A3 to A2",
                                                      sourceConfig, targetConfig,
                                                      expectedTime, expectedRsrp),
               TestCase::TAKES_FOREVER);

  // Event A4 to Event A5
  sourceConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A4;
  targetConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A5;
  expectedTime.clear ();
  expectedTime << 1200 << 1440 << 1680 << 1920;
  expectedRsrp.clear ();
  expectedRsrp << 53 << 53 << 53 << 53;
  AddTestCase (new LteUeMeasurementsHandoverTestCase ("Handover test case - A4 to A5",
                                                      sourceConfig, targetConfig,
                                                      expectedTime, expectedRsrp),
               TestCase::TAKES_FOREVER);

  // Event A5 to Event A4
  sourceConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A5;
  targetConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A4;
  expectedTime.clear ();
  expectedTime << 1200 << 1440 << 1680 << 1920;
  expectedRsrp.clear ();
  expectedRsrp << 53 << 53 << 53 << 53;
  AddTestCase (new LteUeMeasurementsHandoverTestCase ("Handover test case - A5 to A4",
                                                      sourceConfig, targetConfig,
                                                      expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // === Threshold/offset difference ===

  sourceConfig.threshold1.range = 52;
  targetConfig.threshold1.range = 56;

  // Event A1
  sourceConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A1;
  targetConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A1;
  expectedTime.clear ();
  expectedTime << 200 << 440 << 680 << 920;
  expectedRsrp.clear ();
  expectedRsrp << 55 << 55 << 55 << 55;
  AddTestCase (new LteUeMeasurementsHandoverTestCase ("Handover test case - A1 threshold difference",
                                                      sourceConfig, targetConfig,
                                                      expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // Event A2
  sourceConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A2;
  targetConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A2;
  expectedTime.clear ();
  expectedTime << 1200 << 1440 << 1680 << 1920;
  expectedRsrp.clear ();
  expectedRsrp << 53 << 53 << 53 << 53;
  AddTestCase (new LteUeMeasurementsHandoverTestCase ("Handover test case - A2 threshold difference",
                                                      sourceConfig, targetConfig,
                                                      expectedTime, expectedRsrp),
               TestCase::QUICK);

  // Event A3
  sourceConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A3;
  sourceConfig.a3Offset = -30;
  targetConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A3;
  targetConfig.a3Offset = 30;
  expectedTime.clear ();
  expectedTime << 200 << 440 << 680 << 920;
  expectedRsrp.clear ();
  expectedRsrp << 55 << 55 << 55 << 55;
  AddTestCase (new LteUeMeasurementsHandoverTestCase ("Handover test case - A3 offset difference",
                                                      sourceConfig, targetConfig,
                                                      expectedTime, expectedRsrp),
               TestCase::QUICK);

  // Event A4
  sourceConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A4;
  targetConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A4;
  expectedTime.clear ();
  expectedTime << 200 << 440 << 680 << 920;
  expectedRsrp.clear ();
  expectedRsrp << 55 << 55 << 55 << 55;
  AddTestCase (new LteUeMeasurementsHandoverTestCase ("Handover test case - A4 threshold difference",
                                                      sourceConfig, targetConfig,
                                                      expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

  // Event A5
  sourceConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A5;
  sourceConfig.threshold2.range = 52;
  targetConfig.eventId = LteRrcSap::ReportConfigEutra::EVENT_A5;
  targetConfig.threshold2.range = 56;
  expectedTime.clear ();
  expectedRsrp.clear ();
  AddTestCase (new LteUeMeasurementsHandoverTestCase ("Handover test case - A5 threshold difference",
                                                      sourceConfig, targetConfig,
                                                      expectedTime, expectedRsrp),
               TestCase::EXTENSIVE);

} // end of LteUeMeasurementsHandoverTestSuite::LteUeMeasurementsHandoverTestSuite


static LteUeMeasurementsHandoverTestSuite lteUeMeasurementsHandoverTestSuite;



/*
 * Test Case
 */

LteUeMeasurementsHandoverTestCase::LteUeMeasurementsHandoverTestCase (
  std::string name,
  LteRrcSap::ReportConfigEutra sourceConfig,
  LteRrcSap::ReportConfigEutra targetConfig,
  std::vector<Time> expectedTime,
  std::vector<uint8_t> expectedRsrp)
  : TestCase (name),
    m_sourceConfig (sourceConfig),
    m_targetConfig (targetConfig),
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


LteUeMeasurementsHandoverTestCase::~LteUeMeasurementsHandoverTestCase ()
{
  NS_LOG_FUNCTION (this);
}


void
LteUeMeasurementsHandoverTestCase::DoRun ()
{
  NS_LOG_INFO (this << " " << GetName ());

  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  Ptr<EpcHelper> epcHelper = CreateObject<EpcHelper> ();
  lteHelper->SetEpcHelper (epcHelper);
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
   * eNodeB                   UE                     eNodeB
   *    |                     |                         |
   *    x ------------------- x ----------------------- x
   *             400 m                   500 m
   */

  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0)); // Source eNodeB
  positionAlloc->Add (Vector (900.0, 0.0, 0.0)); // Target eNodeB
  positionAlloc->Add (Vector (400.0, 0.0, 0.0)); // UE
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (enbNodes);
  mobility.Install (ueNodes);

  // Create P-GW node
  Ptr<Node> pgw = epcHelper->GetPgwNode ();

  // Create a single RemoteHost
  NodeContainer remoteHostContainer;
  remoteHostContainer.Create (1);
  Ptr<Node> remoteHost = remoteHostContainer.Get (0);
  InternetStackHelper internet;
  internet.Install (remoteHostContainer);

  // Create the Internet
  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Gb/s")));
  p2ph.SetDeviceAttribute ("Mtu", UintegerValue (1500));
  p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.010)));
  NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);
  Ipv4AddressHelper ipv4h;
  ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
  Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);

  // Routing of the Internet Host (towards the LTE network)
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
  remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);

  // Enable layer-3 filtering
  Config::SetDefault ("ns3::LteEnbRrc::RsrpFilterCoefficient",
                      UintegerValue (4));

  // Disable control channel error model
  Config::SetDefault ("ns3::LteSpectrumPhy::CtrlErrorModelEnabled",
                      BooleanValue (false));

  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
  enbDevs = lteHelper->InstallEnbDevice (enbNodes);
  ueDevs = lteHelper->InstallUeDevice (ueNodes);

  // Setup UE measurement configuration in source eNodeB
  Ptr<LteEnbRrc> enbRrc1 = enbDevs.Get (0)->GetObject<LteEnbNetDevice> ()->GetRrc ();
  uint8_t measId = enbRrc1->AddUeMeasReportConfig (m_sourceConfig);
  NS_ASSERT (measId == 1);

  // Setup UE measurement configuration in target eNodeB
  Ptr<LteEnbRrc> enbRrc2 = enbDevs.Get (1)->GetObject<LteEnbNetDevice> ()->GetRrc ();
  measId = enbRrc2->AddUeMeasReportConfig (m_targetConfig);
  NS_ASSERT (measId == 1);

  // Install the IP stack on the UEs
  internet.Install (ueNodes);
  Ipv4InterfaceContainer ueIpIfaces;
  ueIpIfaces = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueDevs));

  // Assign IP address to UEs
  for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
      Ptr<Node> ueNode = ueNodes.Get (u);
      // Set the default gateway for the UE
      Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
      ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
    }

  // Attach UE to serving eNodeB
  lteHelper->Attach (ueDevs.Get (0), enbDevs.Get (0));

  // Add X2 interface
  lteHelper->AddX2Interface (enbNodes);

  // Connect to trace sources in source eNodeB
  Config::Connect ("/NodeList/1/DeviceList/0/LteEnbRrc/RecvMeasurementReport",
                   MakeCallback (&LteUeMeasurementsHandoverTestCase::RecvMeasurementReportCallback,
                                 this));

  // Connect to trace sources in target eNodeB
  Config::Connect ("/NodeList/2/DeviceList/0/LteEnbRrc/RecvMeasurementReport",
                   MakeCallback (&LteUeMeasurementsHandoverTestCase::RecvMeasurementReportCallback,
                                 this));

  // Schedule handover
  lteHelper->HandoverRequest (Seconds (1), ueDevs.Get (0), enbDevs.Get (0),
                              enbDevs.Get (1));

  // Run simulation
  Simulator::Stop (Seconds (2));
  Simulator::Run ();
  Simulator::Destroy ();

} // end of void LteUeMeasurementsHandoverTestCase::DoRun ()


void
LteUeMeasurementsHandoverTestCase::DoTeardown ()
{
  NS_LOG_FUNCTION (this);
  bool hasEnded = m_itExpectedTime == m_expectedTime.end ();
  NS_TEST_ASSERT_MSG_EQ (hasEnded, true,
                         "Reporting should have occurred at " << m_itExpectedTime->GetSeconds () << "s");
  hasEnded = m_itExpectedRsrp == m_expectedRsrp.end ();
  NS_ASSERT (hasEnded);
}


void
LteUeMeasurementsHandoverTestCase::RecvMeasurementReportCallback (
  std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti,
  LteRrcSap::MeasurementReport report)
{
  NS_LOG_FUNCTION (this << context);

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
      NS_ASSERT (it->physCellId != cellId);
      NS_ASSERT (it->physCellId <= 2);
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

} // end of void LteUeMeasurementsHandoverTestCase::RecvMeasurementReportCallback


} // end of namespace ns3
