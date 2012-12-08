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
 * Author: Marco Miozzo <marco.miozzo@cttc.es>,
 *         Nicola Baldo <nbaldo@cttc.es>,
 *         Dizhi Zhou <dizhi.zhou@gmail.com>
 */

#include <iostream>
#include <sstream>
#include <string>

#include <ns3/object.h>
#include <ns3/spectrum-interference.h>
#include <ns3/spectrum-error-model.h>
#include <ns3/log.h>
#include <ns3/test.h>
#include <ns3/simulator.h>
#include <ns3/packet.h>
#include <ns3/ptr.h>
#include "ns3/radio-bearer-stats-calculator.h"
#include <ns3/constant-position-mobility-model.h>
#include <ns3/eps-bearer.h>
#include <ns3/node-container.h>
#include <ns3/mobility-helper.h>
#include <ns3/net-device-container.h>
#include <ns3/lte-ue-net-device.h>
#include <ns3/lte-enb-net-device.h>
#include <ns3/lte-ue-rrc.h>
#include <ns3/lte-helper.h>
#include "ns3/string.h"
#include "ns3/double.h"
#include <ns3/lte-enb-phy.h>
#include <ns3/lte-ue-phy.h>
#include <ns3/boolean.h>
#include <ns3/enum.h>

#include "ns3/epc-helper.h"
#include "ns3/network-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-helper.h"

#include "lte-test-tdtbfq-ff-mac-scheduler.h"

NS_LOG_COMPONENT_DEFINE ("LenaTestTdTbfqFfMacCheduler");

namespace ns3 {

LenaTestTdTbfqFfMacSchedulerSuite::LenaTestTdTbfqFfMacSchedulerSuite ()
  : TestSuite ("lte-tdtbfq-ff-mac-scheduler", SYSTEM)
{
  NS_LOG_INFO ("creating LenaTestTdTbfqFfMacSchedulerSuite");

  // General config
  // Traffic: UDP traffic with fixed rate
  // Token generation rate = traffic rate
  // RLC header length = 2 bytes, PDCP header = 2 bytes
  // Simulation time = 1.0 sec
  // Throughput in this file is calculated in RLC layer

  //Test Case 1: homogeneous flow test in TDTBFQ (same distance)

  // DOWNLINK -> DISTANCE 0 -> MCS 28 -> Itbs 26 (from table 7.1.7.2.1-1 of 36.2    13)
  // Traffic info
  //   UDP traffic: payload size = 200 bytes, interval = 1 ms
  //   UDP rate in scheduler: (payload + RLC header + PDCP header + IP header + UDP header) * 1000 byte/sec -> 232000 byte/rate 
  // Totol bandwidth: 24 PRB at Itbs 26 -> 2196 -> 2196000 byte/sec
  // 1 user -> 232000 * 1 = 232000 < 2196000 -> throughput = 232000 byte/sec
  // 3 user -> 232000 * 3 = 696000 < 2196000 -> througphut = 232000 byte/sec
  // 6 user -> 232000 * 6 = 139200 < 2196000 -> throughput = 232000 byte/sec
  // 12 user -> 232000 * 12 = 2784000 > 2196000 -> throughput = 2196000 / 12 = 183000 byte/sec
  // UPLINK -> DISTANCE 0 -> MCS 28 -> Itbs 26 (from table 7.1.7.2.1-1 of 36.2    13)
  // 1 user -> 25 PRB at Itbs 26 -> 2292 -> 2292000 > 232000 -> throughput = 232000 bytes/sec
  // 3 users -> 8 PRB at Itbs 26 -> 749 -> 749000 > 232000 -> throughput = 232000 bytes/sec 
  // 6 users -> 4 PRB at Itbs 26 -> 373 -> 373000 > 232000 -> throughput = 232000 bytes/sec
  // 12 users -> 2 PRB at Itbs 26 -> 185 -> 185000 < 232000 -> throughput = 185000 bytes/sec
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase1 (1,0,0,232000,232000,200,1));
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase1 (3,0,0,232000,232000,200,1));
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase1 (6,0,0,232000,232000,200,1));
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase1 (12,0,0,183000,185000,200,1));

  // DOWNLINK - DISTANCE 3000 -> MCS 24 -> Itbs 20 (from table 7.1.7.2.1-1 of 36.213)
  // DOWNLINK -> DISTANCE 0 -> MCS 28 -> Itbs 26 (from table 7.1.7.2.1-1 of 36.2    13)
  // Traffic info
  //   UDP traffic: payload size = 200 bytes, interval = 1 ms
  //   UDP rate in scheduler: (payload + RLC header + PDCP header + IP header + UDP header) * 1000 byte/sec -> 232000 byte/rate 
  // Totol bandwidth: 24 PRB at Itbs 20 -> 1383 -> 1383000 byte/sec
  // 1 user -> 232000 * 1 = 232000 < 1383000 -> throughput = 232000 byte/sec
  // 3 user -> 232000 * 3 = 696000 < 1383000 -> througphut = 232000 byte/sec
  // 6 user -> 232000 * 6 = 139200 > 1383000 -> throughput = 1383000 / 6 = 230500 byte/sec
  // 12 user -> 232000 * 12 = 2784000 > 1383000 -> throughput = 1383000 / 12 = 115250 byte/sec
  // UPLINK - DISTANCE 3000 -> MCS 20 -> Itbs 18 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 25 PRB at Itbs 18 -> 1239 -> 1239000 > 232000 -> throughput = 232000 bytes/sec
  // 3 users -> 8 PRB at Itbs 18 -> 389 -> 389000 > 232000 -> throughput = 232000 bytes/sec
  // 6 users -> 4 PRB at Itbs 18 -> 193 -> 193000 < 232000 -> throughput = 193000 bytes/sec
  // 12 users -> 2 PRB at Itbs 18 -> 97 -> 97000 < 232000 -> throughput = 97000 bytes/sec
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase1 (1,0,3000,232000,232000,200,1));
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase1 (3,0,3000,232000,232000,200,1));
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase1 (6,0,3000,230500,193000,200,1));
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase1 (12,0,3000,115250,97000,200,1));

  // DOWNLINK - DISTANCE 6000 -> MCS 16 -> Itbs 15 (from table 7.1.7.2.1-1 of 36.213)
  // Traffic info
  //   UDP traffic: payload size = 200 bytes, interval = 1 ms
  //   UDP rate in scheduler: (payload + RLC header + PDCP header + IP header + UDP header) * 1000 byte/sec -> 232000 byte/rate 
  // Totol bandwidth: 24 PRB at Itbs 15 -> 903 -> 903000 byte/sec
  // 1 user -> 232000 * 1 = 232000 < 903000 -> throughput = 232000 byte/sec
  // 3 user -> 232000 * 3 = 696000 < 903000 -> througphut = 232000 byte/sec
  // 6 user -> 232000 * 6 = 139200 > 903000 -> throughput = 903000 / 6 = 150500 byte/sec
  // 12 user -> 232000 * 12 = 2784000 > 903000 -> throughput = 903000 / 12 = 75250 byte/sec
  // UPLINK - DISTANCE 6000 -> MCS 12 -> Itbs 11 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 25 PRB at Itbs 11 -> 621 -> 621000 > 232000 -> throughput = 232000  bytes/sec
  // 3 users -> 8 PRB at Itbs 11 -> 201 -> 201000 < 232000 -> throughput = 201000  bytes/sec
  // 6 users -> 4 PRB at Itbs 11 -> 97 -> 97000 < 232000 -> throughput = 97000 bytes/sec
  // 12 users -> 2 PRB at Itbs 11 -> 47 -> 47000 < 232000 -> throughput = 47000 bytes/sec
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase1 (1,0,6000,232000,232000,200,1));
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase1 (3,0,6000,232000,201000,200,1));
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase1 (6,0,6000,150500,97000,200,1));
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase1 (12,0,6000,75250,47000,200,1));

  // DOWNLINK - DISTANCE 9000 -> MCS 12 -> Itbs 11 (from table 7.1.7.2.1-1 of 36.213)
  // Traffic info
  //   UDP traffic: payload size = 200 bytes, interval = 1 ms
  //   UDP rate in scheduler: (payload + RLC header + PDCP header + IP header + UDP header) * 1000 byte/sec -> 232000 byte/rate 
  // Totol bandwidth: 24 PRB at Itbs 11 -> 597 -> 597000 byte/sec
  // 1 user -> 232000 * 1 = 232000 < 597000 -> throughput = 232000 byte/sec
  // 3 user -> 232000 * 3 = 696000 > 597000 -> througphut = 597000 / 3 = 199000byte/sec
  // 6 user -> 232000 * 6 = 139200 > 597000 -> throughput = 597000 / 6 = 99500 byte/sec
  // 12 user -> 232000 * 12 = 2784000 > 597000 -> throughput = 597000 / 12 = 49750 byte/sec
  // UPLINK - DISTANCE 9000 -> MCS 8 -> Itbs 8 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 24 PRB at Itbs 8 -> 437 -> 437000 > 232000 -> throughput = 232000 bytes/sec
  // 3 users -> 8 PRB at Itbs 8 -> 137 -> 137000 < 232000 -> throughput = 137000 bytes/sec
  // 6 users -> 4 PRB at Itbs 8 -> 67 -> 67000 < 232000 -> throughput = 67000 bytes/sec
  // 12 users -> 2 PRB at Itbs 8 -> 32 -> 32000 < 232000 -> throughput = 32000 bytes/sec
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase1 (1,0,9000,232000,232000,200,1));
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase1 (3,0,9000,199000,137000,200,1));
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase1 (6,0,9000,99500,67000,200,1));
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase1 (12,0,9000,49750,32000,200,1));
 
  // DONWLINK - DISTANCE 15000 -> MCS 6 -> Itbs 6 (from table 7.1.7.2.1-1 of 36.213)
  // Traffic info
  //   UDP traffic: payload size = 200 bytes, interval = 1 ms
  //   UDP rate in scheduler: (payload + RLC header + PDCP header + IP header + UDP header) * 1000 byte/sec -> 232000 byte/rate 
  // Totol bandwidth: 24 PRB at Itbs 6 -> 309 -> 309000 byte/sec
  // 1 user -> 232000 * 1 = 232000 < 309000 -> throughput = 232000 byte/sec
  // 3 user -> 232000 * 3 = 696000 > 309000 -> througphut = 309000 / 3 = 103000byte/sec
  // 6 user -> 232000 * 6 = 139200 > 309000 -> throughput = 309000 / 6 = 51500 byte/sec
  // 12 user -> 232000 * 12 = 2784000 > 309000 -> throughput = 309000 / 12 = 25750 byte/sec
  // UPLINK - DISTANCE 15000 -> MCS 6 -> Itbs 6 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 25 PRB at Itbs 6 -> 233 -> 233000 > 232000 -> throughput = 232000 bytes/sec
  // 3 users -> 8 PRB at Itbs 6 -> 69 -> 69000 < 232000 -> throughput = 69000 bytes/sec
  // 6 users -> 4 PRB at Itbs 6 -> 32 -> 32000 < 232000 -> throughput = 32000 bytes/sec
  // 12 users -> 2 PRB at Itbs 6 -> 15 -> 15000 < 232000 -> throughput = 15000 bytes/sec
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase1 (1,0,15000,232000,232000,200,1));
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase1 (3,0,15000,103000,69000,200,1));
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase1 (6,0,15000,51500,32000,200,1));
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase1 (12,0,15000,25750,15000,200,1));

  // Test Case 2: homogeneous flow test in TDTBFQ (different distance)
  // Traffic1 info
  //   UDP traffic: payload size = 100 bytes, interval = 1 ms
  //   UDP rate in scheduler: (payload + RLC header + PDCP header + IP header + UDP header) * 1000 byte/sec -> 132000 byte/rate 
  // Maximum throughput = 5 / ( 1/2196000 + 1/1383000 + 1/903000 + 1/597000 + 1/309000) = 694720  byte/s
  // 132000 * 5 = 660000 < 694720 -> estimated throughput in downlink = 132000 byte/sec
  std::vector<uint16_t> dist1;
  dist1.push_back (0);    // User 0 distance --> MCS 28
  dist1.push_back (3000);    // User 1 distance --> MCS 24
  dist1.push_back (6000);    // User 2 distance --> MCS 16
  dist1.push_back (9000);    // User 3 distance --> MCS 12
  dist1.push_back (15000);    // User 4 distance --> MCS 6
  std::vector<uint16_t> packetSize1;
  packetSize1.push_back (100);
  packetSize1.push_back (100);
  packetSize1.push_back (100);
  packetSize1.push_back (100);
  packetSize1.push_back (100);
  std::vector<uint32_t> estThrTdTbfqDl1;
  estThrTdTbfqDl1.push_back (132000); // User 0 estimated TTI throughput from TDTBFQ
  estThrTdTbfqDl1.push_back (132000); // User 1 estimated TTI throughput from TDTBFQ
  estThrTdTbfqDl1.push_back (132000); // User 2 estimated TTI throughput from TDTBFQ
  estThrTdTbfqDl1.push_back (132000); // User 3 estimated TTI throughput from TDTBFQ
  estThrTdTbfqDl1.push_back (132000); // User 4 estimated TTI throughput from TDTBFQ
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase2 (dist1,estThrTdTbfqDl1,packetSize1,1));

  // Traffic2 info
  //   UDP traffic: payload size = 200 bytes, interval = 1 ms
  //   UDP rate in scheduler: (payload + RLC header + PDCP header + IP header + UDP header) * 1000 byte/sec -> 232000 byte/rate 
  // Maximum throughput = 5 / ( 1/2196000 + 1/1383000 + 1/903000 + 1/597000 + 1/309000) = 694720  byte/s
  // 232000 * 5 = 1160000 > 694720 -> estimated throughput in downlink = 694720 / 5 = 138944 byte/sec
  std::vector<uint16_t> dist2;
  dist2.push_back (0);    // User 0 distance --> MCS 28
  dist2.push_back (3000);    // User 1 distance --> MCS 24
  dist2.push_back (6000);    // User 2 distance --> MCS 16
  dist2.push_back (9000);    // User 3 distance --> MCS 12
  dist2.push_back (15000);    // User 4 distance --> MCS 6
  std::vector<uint16_t> packetSize2;
  packetSize2.push_back (200);
  packetSize2.push_back (200);
  packetSize2.push_back (200);
  packetSize2.push_back (200);
  packetSize2.push_back (200);
  std::vector<uint32_t> estThrTdTbfqDl2;
  estThrTdTbfqDl2.push_back (138944); // User 0 estimated TTI throughput from TDTBFQ
  estThrTdTbfqDl2.push_back (138944); // User 1 estimated TTI throughput from TDTBFQ
  estThrTdTbfqDl2.push_back (138944); // User 2 estimated TTI throughput from TDTBFQ
  estThrTdTbfqDl2.push_back (138944); // User 3 estimated TTI throughput from TDTBFQ
  estThrTdTbfqDl2.push_back (138944); // User 4 estimated TTI throughput from TDTBFQ
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase2 (dist2,estThrTdTbfqDl2,packetSize2,1));

  // Test Case 3: heterogeneous flow test in TDTBFQ
  //   UDP traffic: payload size = [100,200,300] bytes, interval = 1 ms
  //   UDP rate in scheduler: (payload + RLC header + PDCP header + IP header + UDP header) * 1000 byte/sec -> [132000, 232000, 332000] byte/rate 
  // Maximum throughput = 5 / ( 1/2196000 + 1/1383000 + 1/903000 ) = 1312417  byte/s
  // 132000 + 232000 + 332000 = 696000 < 1312417 -> estimated throughput in downlink = [132000, 232000, 332000] byte/sec
  std::vector<uint16_t> dist3;
  dist3.push_back (0);    // User 0 distance --> MCS 28
  dist3.push_back (3000);    // User 1 distance --> MCS 24
  dist3.push_back (6000);    // User 2 distance --> MCS 16
  std::vector<uint16_t> packetSize3;
  packetSize3.push_back (100);
  packetSize3.push_back (200);
  packetSize3.push_back (300);
  std::vector<uint32_t> estThrTdTbfqDl3;
  estThrTdTbfqDl3.push_back (132000); // User 0 estimated TTI throughput from TDTBFQ
  estThrTdTbfqDl3.push_back (232000); // User 1 estimated TTI throughput from TDTBFQ
  estThrTdTbfqDl3.push_back (332000); // User 2 estimated TTI throughput from TDTBFQ
  AddTestCase (new LenaTdTbfqFfMacSchedulerTestCase2 (dist3,estThrTdTbfqDl3,packetSize3,1));

}

static LenaTestTdTbfqFfMacSchedulerSuite lenaTestTdTbfqFfMacSchedulerSuite;

// --------------- T E S T - C A S E   # 1 ------------------------------


std::string 
LenaTdTbfqFfMacSchedulerTestCase1::BuildNameString (uint16_t nUser, uint16_t dist)
{
  std::ostringstream oss;
  oss << nUser << " UEs, distance " << dist << " m";
  return oss.str ();
}


LenaTdTbfqFfMacSchedulerTestCase1::LenaTdTbfqFfMacSchedulerTestCase1 (uint16_t nUser, uint16_t nLc, uint16_t dist, double thrRefDl, double thrRefUl, uint16_t packetSize, uint16_t interval)
  : TestCase (BuildNameString (nUser, dist)),
    m_nUser (nUser),
    m_nLc (nLc),
    m_dist (dist),
    m_packetSize (packetSize),
    m_interval (interval),
    m_thrRefDl (thrRefDl),
    m_thrRefUl (thrRefUl)
{
}

LenaTdTbfqFfMacSchedulerTestCase1::~LenaTdTbfqFfMacSchedulerTestCase1 ()
{
}

void
LenaTdTbfqFfMacSchedulerTestCase1::DoRun (void)
{
  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  Ptr<EpcHelper>  epcHelper = CreateObject<EpcHelper> ();
  lteHelper->SetEpcHelper (epcHelper);

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
  p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.001)));
  NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);
  Ipv4AddressHelper ipv4h;
  ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
  Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);
  // interface 0 is localhost, 1 is the p2p device
  Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress (1);

  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
  remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);

  Config::SetDefault ("ns3::LteAmc::AmcModel", EnumValue (LteAmc::PiroEW2010));
  Config::SetDefault ("ns3::LteAmc::Ber", DoubleValue (0.00005));
  Config::SetDefault ("ns3::LteSpectrumPhy::CtrlErrorModelEnabled", BooleanValue (false));
  Config::SetDefault ("ns3::LteSpectrumPhy::DataErrorModelEnabled", BooleanValue (false));  

  lteHelper->SetAttribute ("EpsBearerToRlcMapping", EnumValue (LteHelper::RLC_UM_ALWAYS));

  LogComponentDisableAll (LOG_LEVEL_ALL);
  //LogComponentEnable ("LenaTestTdTbfqFfMacCheduler", LOG_LEVEL_ALL);
   
  lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::FriisSpectrumPropagationLossModel"));

  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes;
  enbNodes.Create (1);
  ueNodes.Create (m_nUser);

  // Install Mobility Model
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (enbNodes);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (ueNodes);

  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
  lteHelper->SetSchedulerType ("ns3::TdTbfqFfMacScheduler");
  enbDevs = lteHelper->InstallEnbDevice (enbNodes);
  ueDevs = lteHelper->InstallUeDevice (ueNodes);

  // Attach a UE to a eNB
  lteHelper->Attach (ueDevs, enbDevs.Get (0));

  Ptr<LteEnbNetDevice> lteEnbDev = enbDevs.Get (0)->GetObject<LteEnbNetDevice> ();
  Ptr<LteEnbPhy> enbPhy = lteEnbDev->GetPhy ();
  enbPhy->SetAttribute ("TxPower", DoubleValue (30.0));
  enbPhy->SetAttribute ("NoiseFigure", DoubleValue (5.0));

  // Set UEs' position and power
  for (int i = 0; i < m_nUser; i++)
    {
      Ptr<ConstantPositionMobilityModel> mm = ueNodes.Get (i)->GetObject<ConstantPositionMobilityModel> ();
      mm->SetPosition (Vector (m_dist, 0.0, 0.0));
      Ptr<LteUeNetDevice> lteUeDev = ueDevs.Get (i)->GetObject<LteUeNetDevice> ();
      Ptr<LteUePhy> uePhy = lteUeDev->GetPhy ();
      uePhy->SetAttribute ("TxPower", DoubleValue (23.0));
      uePhy->SetAttribute ("NoiseFigure", DoubleValue (9.0));
    }

  // Install the IP stack on the UEs
  internet.Install (ueNodes);
  Ipv4InterfaceContainer ueIpIface;
  ueIpIface = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueDevs));
  // Assign IP address to UEs, and install applications
  for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
      Ptr<Node> ueNode = ueNodes.Get (u);
      // Set the default gateway for the UE
      Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
      ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
    }

// Activate an EPS bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  GbrQosInformation qos;
  qos.gbrDl = (m_packetSize + 32) * (1000 / m_interval) * 8;  // bit/s, considering IP, UDP, RLC, PDCP header size
  qos.gbrUl = 0;
  qos.mbrDl = qos.gbrDl;
  qos.mbrUl = 0;
  EpsBearer bearer (q, qos);
  lteHelper->ActivateEpsBearer (ueDevs, bearer, EpcTft::Default ());

  lteHelper->EnableRlcTraces ();

  // Install downlind and uplink applications
  uint16_t dlPort = 1234;
  uint16_t ulPort = 2000;
  PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), dlPort));
  PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), ulPort));
  ApplicationContainer clientApps;
  ApplicationContainer serverApps;
  for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
      ++ulPort;
      serverApps.Add (dlPacketSinkHelper.Install (ueNodes.Get (u))); // receive packets from remotehost
      serverApps.Add (ulPacketSinkHelper.Install (remoteHost));  // receive packets from UEs

      UdpClientHelper dlClient (ueIpIface.GetAddress (u), dlPort); // uplink packets generator
      dlClient.SetAttribute ("Interval", TimeValue (MilliSeconds (m_interval)));
      dlClient.SetAttribute ("MaxPackets", UintegerValue (1000000));
      dlClient.SetAttribute ("PacketSize", UintegerValue (m_packetSize));

      UdpClientHelper ulClient (remoteHostAddr, ulPort);           // downlink packets generator
      ulClient.SetAttribute ("Interval", TimeValue (MilliSeconds (m_interval)));
      ulClient.SetAttribute ("MaxPackets", UintegerValue (1000000));
      ulClient.SetAttribute ("PacketSize", UintegerValue (m_packetSize));

      clientApps.Add (dlClient.Install (remoteHost));
      clientApps.Add (ulClient.Install (ueNodes.Get (u)));
   }

  serverApps.Start (Seconds (0.001));
  clientApps.Start (Seconds (0.001));

  double simulationTime = 2.0;
  double tolerance = 0.1;
  Simulator::Stop (Seconds (simulationTime));

  Ptr<RadioBearerStatsCalculator> rlcStats = lteHelper->GetRlcStats ();
  rlcStats->SetAttribute ("EpochDuration", TimeValue (Seconds (simulationTime)));

  Simulator::Run ();

  /**
   * Check that the downlink assignation is done in a "token bank fair queue" manner
   */
  NS_LOG_INFO ("DL - Test with " << m_nUser << " user(s) at distance " << m_dist);
  std::vector <uint64_t> dlDataRxed;
  for (int i = 0; i < m_nUser; i++)
    {
      // get the imsi
      uint64_t imsi = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetImsi ();
      // get the lcId
      uint8_t lcId = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetRrc ()->GetLcIdVector ().at (0);
      uint64_t data = rlcStats->GetDlRxData (imsi, lcId);
      dlDataRxed.push_back (data);
      NS_LOG_INFO ("\tUser " << i << " imsi " << imsi << " bytes rxed " << (double)dlDataRxed.at (i) << "  thr " << (double)dlDataRxed.at (i) / simulationTime << " ref " << m_thrRefDl);
    }

  for (int i = 0; i < m_nUser; i++)
    {
      NS_TEST_ASSERT_MSG_EQ_TOL ((double)dlDataRxed.at (i) / simulationTime, m_thrRefDl, m_thrRefDl * tolerance, " Unfair Throughput!");
    }

  /**
  * Check that the uplink assignation is done in a "round robin" manner
  */
  NS_LOG_INFO ("UL - Test with " << m_nUser << " user(s) at distance " << m_dist);
  std::vector <uint64_t> ulDataRxed;
  for (int i = 0; i < m_nUser; i++)
    {
      // get the imsi
      uint64_t imsi = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetImsi ();
      // get the lcId
      uint8_t lcId = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetRrc ()->GetLcIdVector ().at (0);
      ulDataRxed.push_back (rlcStats->GetUlRxData (imsi, lcId));
      NS_LOG_INFO ("\tUser " << i << " imsi " << imsi << " bytes rxed " << (double)ulDataRxed.at (i) << "  thr " << (double)ulDataRxed.at (i) / simulationTime << " ref " << m_thrRefUl);
    }

  for (int i = 0; i < m_nUser; i++)
    {
      NS_TEST_ASSERT_MSG_EQ_TOL ((double)ulDataRxed.at (i) / simulationTime, m_thrRefUl, m_thrRefUl * tolerance, " Unfair Throughput!");
    }
  Simulator::Destroy ();

}



// --------------- T E S T - C A S E   # 2 ------------------------------


std::string 
LenaTdTbfqFfMacSchedulerTestCase2::BuildNameString (uint16_t nUser, std::vector<uint16_t> dist)
{
  std::ostringstream oss;
  oss << "distances (m) = [ " ;
  for (std::vector<uint16_t>::iterator it = dist.begin (); it != dist.end (); ++it)
    {
      oss << *it << " ";
    }
  oss << "]";
  return oss.str ();
}


LenaTdTbfqFfMacSchedulerTestCase2::LenaTdTbfqFfMacSchedulerTestCase2 (std::vector<uint16_t> dist, std::vector<uint32_t> estThrTdTbfqDl, std::vector<uint16_t> packetSize, uint16_t interval)
  : TestCase (BuildNameString (dist.size (), dist)),
    m_nUser (dist.size ()),
    m_dist (dist),
    m_packetSize (packetSize),
    m_interval (interval),
    m_estThrTdTbfqDl (estThrTdTbfqDl)
{
}

LenaTdTbfqFfMacSchedulerTestCase2::~LenaTdTbfqFfMacSchedulerTestCase2 ()
{
}

void
LenaTdTbfqFfMacSchedulerTestCase2::DoRun (void)
{
  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  Ptr<EpcHelper>  epcHelper = CreateObject<EpcHelper> ();
  lteHelper->SetEpcHelper (epcHelper);

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
  p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.001)));
  NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);
  Ipv4AddressHelper ipv4h;
  ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
  Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);
  // interface 0 is localhost, 1 is the p2p device
  Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress (1);

  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
  remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);

  Config::SetDefault ("ns3::LteAmc::AmcModel", EnumValue (LteAmc::PiroEW2010));
  Config::SetDefault ("ns3::LteAmc::Ber", DoubleValue (0.00005));
  Config::SetDefault ("ns3::LteSpectrumPhy::CtrlErrorModelEnabled", BooleanValue (false));
  Config::SetDefault ("ns3::LteSpectrumPhy::DataErrorModelEnabled", BooleanValue (false));

  lteHelper->SetAttribute ("EpsBearerToRlcMapping", EnumValue (LteHelper::RLC_UM_ALWAYS));

  LogComponentDisableAll (LOG_LEVEL_ALL);
  //LogComponentEnable ("LenaTestTdTbfqFfMacCheduler", LOG_LEVEL_ALL);
   
  lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::FriisSpectrumPropagationLossModel"));

  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes;
  enbNodes.Create (1);
  ueNodes.Create (m_nUser);

  // Install Mobility Model
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (enbNodes);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (ueNodes);

  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
  lteHelper->SetSchedulerType ("ns3::TdTbfqFfMacScheduler");
  enbDevs = lteHelper->InstallEnbDevice (enbNodes);
  ueDevs = lteHelper->InstallUeDevice (ueNodes);

  // Attach a UE to a eNB
  lteHelper->Attach (ueDevs, enbDevs.Get (0));

  Ptr<LteEnbNetDevice> lteEnbDev = enbDevs.Get (0)->GetObject<LteEnbNetDevice> ();
  Ptr<LteEnbPhy> enbPhy = lteEnbDev->GetPhy ();
  enbPhy->SetAttribute ("TxPower", DoubleValue (30.0));
  enbPhy->SetAttribute ("NoiseFigure", DoubleValue (5.0));

  // Set UEs' position and power
  for (int i = 0; i < m_nUser; i++)
    {
      Ptr<ConstantPositionMobilityModel> mm = ueNodes.Get (i)->GetObject<ConstantPositionMobilityModel> ();
      mm->SetPosition (Vector (m_dist.at (i), 0.0, 0.0));
      Ptr<LteUeNetDevice> lteUeDev = ueDevs.Get (i)->GetObject<LteUeNetDevice> ();
      Ptr<LteUePhy> uePhy = lteUeDev->GetPhy ();
      uePhy->SetAttribute ("TxPower", DoubleValue (23.0));
      uePhy->SetAttribute ("NoiseFigure", DoubleValue (9.0));
    }

  // Install the IP stack on the UEs
  internet.Install (ueNodes);
  Ipv4InterfaceContainer ueIpIface;
  ueIpIface = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueDevs));
  // Assign IP address to UEs, and install applications
  for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
      Ptr<Node> ueNode = ueNodes.Get (u);
      // Set the default gateway for the UE
      Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
      ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
    }

// Activate an EPS bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  GbrQosInformation qos;
  uint16_t mbrDl = 0;
  for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
      mbrDl = mbrDl + m_packetSize.at (u);
    }
  mbrDl = mbrDl / ueNodes.GetN ();
  qos.gbrDl = (mbrDl + 32) * (1000 / m_interval) * 8;  // bit/s, considering IP, UDP, RLC, PDCP header size
  qos.gbrUl = 0;
  qos.mbrDl = qos.gbrDl;
  qos.mbrUl = 0;
  EpsBearer bearer (q, qos);
  lteHelper->ActivateEpsBearer (ueDevs, bearer, EpcTft::Default ());

  lteHelper->EnableRlcTraces ();

  // Install downlind and uplink applications
  uint16_t dlPort = 1234;
  uint16_t ulPort = 2000;
  PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), dlPort));
  PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), ulPort));
  ApplicationContainer clientApps;
  ApplicationContainer serverApps;
  for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
      ++ulPort;
      serverApps.Add (dlPacketSinkHelper.Install (ueNodes.Get (u))); // receive packets from remotehost
      serverApps.Add (ulPacketSinkHelper.Install (remoteHost));  // receive packets from UEs

      UdpClientHelper dlClient (ueIpIface.GetAddress (u), dlPort); // uplink packets generator
      dlClient.SetAttribute ("Interval", TimeValue (MilliSeconds (m_interval)));
      dlClient.SetAttribute ("MaxPackets", UintegerValue (1000000));
      dlClient.SetAttribute ("PacketSize", UintegerValue (m_packetSize.at (u)));

      UdpClientHelper ulClient (remoteHostAddr, ulPort);           // downlink packets generator
      ulClient.SetAttribute ("Interval", TimeValue (MilliSeconds (m_interval)));
      ulClient.SetAttribute ("MaxPackets", UintegerValue (1000000));
      ulClient.SetAttribute ("PacketSize", UintegerValue (m_packetSize.at (u)));

      clientApps.Add (dlClient.Install (remoteHost));
      clientApps.Add (ulClient.Install (ueNodes.Get (u)));
   }

  serverApps.Start (Seconds (0.001));
  clientApps.Start (Seconds (0.001));

  double simulationTime = 1.0;
  double tolerance = 0.1;
  Simulator::Stop (Seconds (simulationTime));

  Ptr<RadioBearerStatsCalculator> rlcStats = lteHelper->GetRlcStats ();
  rlcStats->SetAttribute ("EpochDuration", TimeValue (Seconds (simulationTime)));

  Simulator::Run ();
 
  /**
  * Check that the assignation is done in a "token bank fair queue" manner
  */
  NS_LOG_INFO ("DL - Test with " << m_nUser << " user(s)");
  std::vector <uint64_t> dlDataRxed;
  for (int i = 0; i < m_nUser; i++)
    {
      // get the imsi
      uint64_t imsi = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetImsi ();
      // get the lcId
      uint8_t lcId = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetRrc ()->GetLcIdVector ().at (0);
      dlDataRxed.push_back (rlcStats->GetDlRxData (imsi, lcId));
      NS_LOG_INFO ("\tUser " << i << " dist " << m_dist.at (i) << " imsi " << imsi << " bytes rxed " << (double)dlDataRxed.at (i) << "  thr " << (double)dlDataRxed.at (i) / simulationTime << " ref " << m_nUser);
    }

  for (int i = 0; i < m_nUser; i++)
    {
      NS_TEST_ASSERT_MSG_EQ_TOL ((double)dlDataRxed.at (i) / simulationTime, m_estThrTdTbfqDl.at (i), m_estThrTdTbfqDl.at (i) * tolerance, " Unfair Throughput!");
    }

  Simulator::Destroy ();

}


} // namespace ns3




