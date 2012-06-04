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
 * Author: Marco Miozzo <marco.miozzo@cttc.es>
 */

#include <ns3/object.h>
#include <ns3/spectrum-interference.h>
#include <ns3/spectrum-error-model.h>
#include <ns3/log.h>
#include <ns3/test.h>
#include <ns3/simulator.h>
#include <ns3/packet.h>
#include <ns3/ptr.h>
#include <iostream>
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

#include "lte-test-rr-ff-mac-scheduler.h"

NS_LOG_COMPONENT_DEFINE ("LenaTestRrFfMacCheduler");

namespace ns3 {


LenaTestRrFfMacSchedulerSuite::LenaTestRrFfMacSchedulerSuite ()
  : TestSuite ("lte-rr-ff-mac-scheduler", SYSTEM)
{
  NS_LOG_INFO ("creating LenaRrFfMacSchedulerTestCase");

  
  // DOWNLINK- DISTANCE 0 -> MCS 28 -> Itbs 26 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 24 PRB at Itbs 26 -> 2196 -> 2196000 bytes/sec
  // 3 users -> 8 PRB at Itbs 26 -> 749 -> 749000 bytes/sec
  // 6 users -> 4 PRB at Itbs 26 -> 373 -> 373000 bytes/sec
  // 9 user -> 2 PRB at Itbs 26 -> 185 -> 185000 bytes/sec
  // 12 users -> 2 PRB at Itbs 26 -> 185 -> 185000 bytes/sec
  // 15 users -> 2 PRB at Itbs 26 * 0.8 -> 148 -> 148000 bytes/sec
  // UPLINK- DISTANCE 0 -> MCS 28 -> Itbs 26 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 25 PRB at Itbs 26 -> 2292 -> 2292000 bytes/sec
  // 3 users -> 8 PRB at Itbs 26 -> 749 -> 749000 bytes/sec
  // 6 users -> 4 PRB at Itbs 26 -> 373 -> 373000 bytes/sec
  // 9 user -> 2 PRB at Itbs 26 -> 185 -> 185000 bytes/sec
  // 12 users -> 2 PRB at Itbs 26 -> 185 -> 185000 bytes/sec
  // 15 users -> 1 PRB at Itbs 26 -> 89 -> 89000 bytes/sec
  AddTestCase (new LenaRrFfMacSchedulerTestCase (1,0,0,2196000,2292000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (3,0,0,749000,749000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (6,0,0,373000,373000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (9,0,0,185000,185000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (12,0,0,185000,185000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (15,0,0,148000,89000));

  // DOWNLINK - DISTANCE 3000 -> MCS 22 -> Itbs 20 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 24 PRB at Itbs 20 -> 1383 -> 1383000 bytes/sec
  // 3 users -> 8 PRB at Itbs 20 -> 469 -> 469000 bytes/sec
  // 6 users -> 4 PRB at Itbs 20 -> 233 -> 233000 bytes/sec
  // 9 user -> 2 PRB at Itbs 20 -> 113 -> 113000 bytes/sec
  // 12 users -> 2 PRB at Itbs 20 -> 113 -> 113000 bytes/sec
  // 15 users -> 2 PRB at Itbs 20 * 0.8 -> 90.4 -> 90400 bytes/sec
  // UPLINK - DISTANCE 3000 -> MCS 20 -> Itbs 18 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 25 PRB at Itbs 18 -> 1239 -> 1239000 bytes/sec
  // 3 users -> 8 PRB at Itbs 18 -> 389 -> 389000 bytes/sec
  // 6 users -> 4 PRB at Itbs 18 -> 193 -> 193000 bytes/sec
  // 9 user -> 2 PRB at Itbs 18 -> 97 -> 97000 bytes/sec
  // 12 users -> 2 PRB at Itbs 18 -> 97 -> 97000 bytes/sec
  // 15 users -> 1 PRB at Itbs 18 -> 47 -> 47000 bytes/sec 
  AddTestCase (new LenaRrFfMacSchedulerTestCase (1,0,3000,1383000,1239000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (3,0,3000,469000,389000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (6,0,3000,233000,193000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (9,0,3000,113000,97000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (12,0,3000,113000,97000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (15,0,3000,90400,47000));

  // DOWNLINK - DISTANCE 6000 -> MCS 16 -> Itbs 15 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 24 PRB at Itbs 15 -> 903 -> 903000 bytes/sec
  // 3 users -> 8 PRB at Itbs 15 -> 309 -> 309000 bytes/sec
  // 6 users -> 4 PRB at Itbs 15 -> 153 -> 153000 bytes/sec
  // 9 user -> 2 PRB at Itbs 15 -> 75 -> 75000 bytes/sec
  // 12 users -> 2 PRB at Itbs 15 -> 75 -> 75000 bytes/sec
  // 15 users -> 2 PRB at Itbs 15 * 0.8 -> 60 -> 60000 bytes/sec
  // UPLINK - DISTANCE 6000 -> MCS 12 -> Itbs 11 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 25 PRB at Itbs 11 -> 621 -> 621000 bytes/sec
  // 3 users -> 8 PRB at Itbs 11 -> 201 -> 201000 bytes/sec
  // 6 users -> 4 PRB at Itbs 11 -> 97 -> 97000 bytes/sec
  // 9 user -> 2 PRB at Itbs 11 -> 47 -> 47000 bytes/sec
  // 12 users -> 2 PRB at Itbs 11 -> 47 -> 47000 bytes/sec
  // 15 users -> 1 PRB at Itbs 11 -> 22 -> 22000 bytes/sec
  AddTestCase (new LenaRrFfMacSchedulerTestCase (1,0,6000,903000,621000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (3,0,6000,309000,201000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (6,0,6000,153000,97000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (9,0,6000,75000,47000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (12,0,6000,75000,47000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (15,0,6000,60000,22000));

  // DOWNLINK - DISTANCE 9000 -> MCS 12 -> Itbs 11 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 24 PRB at Itbs 11 -> 597 -> 597000 bytes/sec
  // 3 users -> 8 PRB at Itbs 11 -> 201 -> 201000 bytes/sec
  // 6 users -> 4 PRB at Itbs 11 -> 97 -> 97000 bytes/sec
  // 9 user -> 2 PRB at Itbs 11 -> 47 -> 47000 bytes/sec
  // 12 users -> 2 PRB at Itbs 11 -> 47 -> 47000 bytes/sec
  // 15 users -> 2 PRB at Itbs 11 * 0.8 -> 37.6 -> 37600 bytes/sec 
  // UPLINK - DISTANCE 9000 -> MCS 8 -> Itbs 8 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 24 PRB at Itbs 8 -> 437 -> 437000 bytes/sec
  // 3 users -> 8 PRB at Itbs 8 -> 137 -> 137000 bytes/sec
  // 6 users -> 4 PRB at Itbs 8 -> 67 -> 67000 bytes/sec
  // 9 user -> 2 PRB at Itbs 8 -> 32 -> 32000 bytes/sec
  // 12 users -> 2 PRB at Itbs 8 -> 32 -> 32000 bytes/sec
  // 15 users -> 1 PRB at Itbs 8 -> 15 -> 15000 bytes/sec
  AddTestCase (new LenaRrFfMacSchedulerTestCase (1,0,9000,597000,437000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (3,0,9000,201000,137000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (6,0,9000,97000,67000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (9,0,9000,47000,32000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (12,0,9000,47000,32000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (15,0,9000,37600,15000));

  // DOWNLINK - DISTANCE 15000 -> MCS 6 -> Itbs 6 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 24 PRB at Itbs 6 -> 309 -> 309000 bytes/sec
  // 3 users -> 8 PRB at Itbs 6 -> 101 -> 101000 bytes/sec
  // 6 users -> 4 PRB at Itbs 6 -> 49 -> 49000 bytes/sec
  // 9 user -> 2 PRB at Itbs 6 -> 22 -> 22000 bytes/sec
  // 12 users -> 2 PRB at Itbs 6 -> 22 -> 22000 bytes/sec
  // 15 users -> 2 PRB at Itbs 6 * 0.8 -> 17.6 -> 17600 bytes/sec
  // UPLINK - DISTANCE 15000 -> MCS 6 -> Itbs 6 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 25 PRB at Itbs 6 -> 233 -> 233000 bytes/sec
  // 3 users -> 8 PRB at Itbs 6 -> 69 -> 69000 bytes/sec
  // 6 users -> 4 PRB at Itbs 6 -> 32 -> 32000 bytes/sec
  // 9 user -> 2 PRB at Itbs 6 -> 15 -> 15000 bytes/sec
  // 12 users -> 2 PRB at Itbs 6 -> 15 -> 15000 bytes/sec
  // 15 users -> 1 PRB at Itbs 6 -> 7 -> 7000 bytes/sec
  AddTestCase (new LenaRrFfMacSchedulerTestCase (1,0,15000,309000,233000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (3,0,15000,101000,69000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (6,0,15000,49000,32000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (9,0,15000,22000,15000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (12,0,15000,22000,15000));
  AddTestCase (new LenaRrFfMacSchedulerTestCase (15,0,15000,17600,7000));

}

static LenaTestRrFfMacSchedulerSuite lenaTestRrFfMacSchedulerSuite;

std::string 
LenaRrFfMacSchedulerTestCase::BuildNameString (uint16_t nUser, uint16_t dist)
{
  std::ostringstream oss;
  oss << nUser << " UEs, distance " << dist << " m";
  return oss.str ();
}

LenaRrFfMacSchedulerTestCase::LenaRrFfMacSchedulerTestCase (uint16_t nUser, uint16_t nLc, uint16_t dist, double thrRefDl, double thrRefUl)
  : TestCase (BuildNameString (nUser, dist)),              
    m_nUser (nUser),
    m_nLc (nLc),
    m_dist (dist),
    m_thrRefDl (thrRefDl),
    m_thrRefUl (thrRefUl)
{
}

LenaRrFfMacSchedulerTestCase::~LenaRrFfMacSchedulerTestCase ()
{
}

void
LenaRrFfMacSchedulerTestCase::DoRun (void)
{
  Config::SetDefault ("ns3::LteSpectrumPhy::PemEnabled", BooleanValue (false));
  Config::SetDefault ("ns3::LteAmc::AmcModel", EnumValue (LteAmc::PiroEW2010));
  Config::SetDefault ("ns3::LteAmc::Ber", DoubleValue (0.00005));
  LogComponentDisableAll (LOG_LEVEL_ALL);
//   LogComponentEnable ("LteEnbRrc", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteUeRrc", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteEnbMac", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteUeMac", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteRlc", LOG_LEVEL_ALL);
// 
//   LogComponentEnable ("LtePhy", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteEnbPhy", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteUePhy", LOG_LEVEL_ALL);

//   LogComponentEnable ("LteSpectrumPhy", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteInterference", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteSinrChunkProcessor", LOG_LEVEL_ALL);
// 
//   LogComponentEnable ("LtePropagationLossModel", LOG_LEVEL_ALL);
//   LogComponentEnable ("LossModel", LOG_LEVEL_ALL);
//   LogComponentEnable ("ShadowingLossModel", LOG_LEVEL_ALL);
//   LogComponentEnable ("PenetrationLossModel", LOG_LEVEL_ALL);
//   LogComponentEnable ("MultipathLossModel", LOG_LEVEL_ALL);
//   LogComponentEnable ("PathLossModel", LOG_LEVEL_ALL);
// 
//   LogComponentEnable ("LteNetDevice", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteUeNetDevice", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteEnbNetDevice", LOG_LEVEL_ALL);

//   LogComponentEnable ("RrFfMacScheduler", LOG_LEVEL_ALL);
  LogComponentEnable ("LenaTestRrFfMacCheduler", LOG_LEVEL_ALL);
//     LogComponentEnable ("LteMiErrorModel", LOG_LEVEL_ALL);
//   LogComponentEnable ("LenaHelper", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteSpectrumPhy", LOG_LEVEL_ALL);


  /**
   * Initialize Simulation Scenario: 1 eNB and m_nUser UEs
   */


  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  
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
  lteHelper->SetSchedulerType ("ns3::RrFfMacScheduler");
  enbDevs = lteHelper->InstallEnbDevice (enbNodes);
  ueDevs = lteHelper->InstallUeDevice (ueNodes);

  // Attach a UE to a eNB
  lteHelper->Attach (ueDevs, enbDevs.Get (0));

  // Activate an EPS bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lteHelper->ActivateEpsBearer (ueDevs, bearer, EpcTft::Default ());
  

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

  lteHelper->EnableRlcTraces ();
  double simulationTime = 0.4;
  double tolerance = 0.1;
  Simulator::Stop (Seconds (simulationTime));

  Ptr<RadioBearerStatsCalculator> rlcStats = lteHelper->GetRlcStats ();
  rlcStats->SetAttribute ("EpochDuration", TimeValue (Seconds (simulationTime)));


  Simulator::Run ();

  /**
   * Check that the assignation is done in a RR fashion
   */
  NS_LOG_INFO ("DL - Test with " << m_nUser << " user(s) at distance " << m_dist);
  std::vector <uint64_t> dlDataRxed;
  for (int i = 0; i < m_nUser; i++)
    {
      // get the imsi
      uint64_t imsi = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetImsi ();
      // get the lcId
      uint8_t lcId = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetRrc ()->GetLcIdVector ().at (0);
      dlDataRxed.push_back (rlcStats->GetDlRxData (imsi, lcId));
      NS_LOG_INFO ("\tUser " << i << " imsi " << imsi << " lcid " << (uint16_t) lcId << " bytes rxed " << (double)dlDataRxed.at (i) << "  thr " << (double)dlDataRxed.at (i) / simulationTime << " ref " << m_thrRefDl);
      NS_TEST_ASSERT_MSG_EQ_TOL ((double)dlDataRxed.at (i) / simulationTime, m_thrRefDl, m_thrRefDl * tolerance, " Unfair Throughput!");
    }

  NS_LOG_INFO ("UL - Test with " << m_nUser << " user(s) at distance " << m_dist);
  std::vector <uint64_t> ulDataRxed;
  for (int i = 0; i < m_nUser; i++)
    {
      // get the imsi
      uint64_t imsi = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetImsi ();
      // get the lcId
      uint8_t lcId = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetRrc ()->GetLcIdVector ().at (0);
      ulDataRxed.push_back (rlcStats->GetUlRxData (imsi, lcId));
      NS_LOG_INFO ("\tUser " << i << " imsi " << imsi << " bytes txed " << (double)ulDataRxed.at (i) << "  thr " << (double)ulDataRxed.at (i) / simulationTime << " ref " << m_thrRefUl << " txed " << rlcStats->GetUlTxData (imsi, lcId) / simulationTime);
      NS_TEST_ASSERT_MSG_EQ_TOL ((double)ulDataRxed.at (i) / simulationTime, m_thrRefUl, m_thrRefUl * tolerance, " Unfair Throughput!");
    }

  Simulator::Destroy ();
}


} // namespace ns3

