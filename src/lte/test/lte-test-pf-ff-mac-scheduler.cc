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
#include "ns3/rlc-stats-calculator.h"
#include <ns3/constant-position-mobility-model.h>
#include "ns3/lte-test-pf-ff-mac-scheduler.h"
#include <ns3/eps-bearer.h>
#include <ns3/node-container.h>
#include <ns3/mobility-helper.h>
#include <ns3/net-device-container.h>
#include <ns3/lte-ue-net-device.h>
#include <ns3/lte-ue-rrc.h>
#include <ns3/lena-helper.h>
#include "ns3/string.h"
#include "ns3/double.h"


NS_LOG_COMPONENT_DEFINE ("LenaTestPfFfMacCheduler");

using namespace ns3;

LenaTestPfFfMacSchedulerSuite::LenaTestPfFfMacSchedulerSuite ()
: TestSuite ("lte-test-pf-ff-mac-scheduler", SYSTEM)
{
  SetVerbose (true);
  NS_LOG_INFO ("creating LenaTestPfFfMacSchedulerSuite");
  
  std::vector<uint16_t> dist;
  dist.push_back (0);    // User 0 distance
  std::vector<uint16_t> estThrPf;
  estThrPf.push_back (0); // User 0 estimated TTI throughput from PF
  
  AddTestCase (new LenaPfFfMacSchedulerTestCase2 (1,dist,estThrPf));

//   // DISTANCE 0 -> MCS 28 -> Itbs 26 (from table 7.1.7.2.1-1 of 36.213)
//   // 1 user -> 24 PRB at Itbs 26 -> 2196 -> 2196000 bytes/sec
//   // 3 users -> 2196000 among 3 users -> 732000 bytes/sec
//   // 6 users -> 2196000 among 6 users -> 366000 bytes/sec
//   // 12 users -> 2196000 among 12 users -> 183000 bytes/sec
//   // 15 users -> 2196000 among 15 users -> 146400 bytes/sec
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (1,0,0,2196000));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (3,0,0,732000));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (6,0,0,366000));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (12,0,0,183000));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (15,0,0,146400));
// 
//   // DISTANCE 6000 -> MCS 24 -> Itbs 22 (from table 7.1.7.2.1-1 of 36.213)
//   // 1 user -> 24 PRB at Itbs 22 -> 1572 -> 1572000 bytes/sec
//   // 3 users -> 1572000 among 3 users -> 524000 bytes/sec
//   // 6 users -> 1572000 among 6 users -> 262000 bytes/sec
//   // 12 users -> 1572000 among 12 users -> 131000 bytes/sec
//   // 15 users -> 1572000 among 15 users -> 104800 bytes/sec
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (1,0,6000,1572000));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (3,0,6000,524000));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (6,0,6000,262000));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (12,0,6000,131000));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (15,0,6000,104800));
// 
//   // DISTANCE 9000 -> MCS 10 -> Itbs 9 (from table 7.1.7.2.1-1 of 36.213)
//   // 1 user -> 24 PRB at Itbs 9 -> 469 -> 469000 bytes/sec
//   // 3 users -> 469000 among 3 users -> 156333 bytes/sec
//   // 6 users -> 469000 among 6 users -> 78166 bytes/sec
//   // 12 users -> 469000 among 12 users -> 39083 bytes/sec
//   // 15 users -> 469000 among 15 users -> 31266 bytes/sec
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (1,0,9000,469000));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (3,0,9000,156333));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (6,0,9000,78166));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (12,0,9000,39083));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (15,0,9000,31266));
// 
//   // DISTANCE 15000 -> MCS 4 -> Itbs 4 (from table 7.1.7.2.1-1 of 36.213)
//   // 1 user -> 24 PRB at Itbs 4 -> 217 -> 217000 bytes/sec
//   // 3 users -> 217000 among 3 users -> 72333 bytes/sec
//   // 6 users -> 217000 among 6 users -> 36166 bytes/sec
//   // 12 users -> 217000 among 12 users -> 18083 bytes/sec
//   // 15 users -> 217000 among 15 users -> 14466 bytes/sec 
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (1,0,15000,217000));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (3,0,15000,72333));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (6,0,15000,36166));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (12,0,15000,18083));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (15,0,15000,14466));
// 
//   // DISTANCE 20000 -> MCS 2 -> Itbs 2 (from table 7.1.7.2.1-1 of 36.213)
//   // 1 user -> 24 PRB at Itbs 2 -> 133 -> 133000 bytes/sec
//   // 3 users -> 217000 among 3 users -> 44333 bytes/sec
//   // 6 users -> 217000 among 6 users -> 22166 bytes/sec
//   // 12 users -> 217000 among 12 users -> 11083 bytes/sec
//   // 15 users -> 217000 among 15 users -> 8866 bytes/sec  
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (1,0,20000,133000));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (3,0,20000,44333));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (6,0,20000,22166));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (12,0,20000,11083));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase1 (15,0,20000,8866));




}

static LenaTestPfFfMacSchedulerSuite lenaTestPfFfMacSchedulerSuite;


// --------------- T E S T - C A S E   # 1 ------------------------------

LenaPfFfMacSchedulerTestCase1::LenaPfFfMacSchedulerTestCase1 (uint16_t nUser, uint16_t nLc, uint16_t dist, double thrRef)
: TestCase ("Proportional Fair (PF) Mac Scheduler Test Case"),
    m_nUser (nUser),
    m_nLc (nLc),
    m_dist (dist),
    m_thrRef (thrRef)
{
  
  // 0 mt -> mcs 28
  // 6000 mt -> mcs 24
  // 12000 mt -> mcs 6
}

LenaPfFfMacSchedulerTestCase1::~LenaPfFfMacSchedulerTestCase1 ()
{
}

void
LenaPfFfMacSchedulerTestCase1::DoRun (void)
{
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
  
    LogComponentEnable ("PfFfMacScheduler", LOG_LEVEL_ALL);
  LogComponentEnable ("LenaTestPfFfMacCheduler", LOG_LEVEL_ALL);
  //   LogComponentEnable ("LteAmc", LOG_LEVEL_ALL);
  //   LogComponentEnable ("RlcStatsCalculator", LOG_LEVEL_ALL);

  /**
   * Initialize Simulation Scenario: 1 eNB and m_nUser UEs
   */
  
  SetVerbose (true);
  Ptr<LenaHelper> lena = CreateObject<LenaHelper> ();

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
  lena->SetSchedulerType ("ns3::PfFfMacScheduler");
  enbDevs = lena->InstallEnbDevice (enbNodes);
  ueDevs = lena->InstallUeDevice (ueNodes);
  
  // Attach a UE to a eNB
  lena->Attach (ueDevs, enbDevs.Get (0));
  
  // Activate an EPS bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lena->ActivateEpsBearer (ueDevs, bearer);
  
  // position nodes
  for (int i = 0; i < m_nUser; i++)
    {
      Ptr<ConstantPositionMobilityModel> mm = ueNodes.Get (i)->GetObject<ConstantPositionMobilityModel> ();
      mm->SetPosition (Vector (m_dist, 0.0, 0.0));
    }
    
  lena->EnableDlRlcTraces();
  
  double simulationTime = 2.0;
  double tolerance = 0.1;
  Simulator::Stop (Seconds (simulationTime));
  
  Ptr<RlcStatsCalculator> rlcStats = lena->GetRlcStats ();
  rlcStats->SetAttribute("EpochDuration", TimeValue(Seconds(simulationTime)));
  
  Simulator::Run ();

  /**
   * Check that the assignation is done in a "proportional fair" manner
   */
  NS_LOG_INFO("Test with " << m_nUser << " user(s) at distance " << m_dist);
  std::vector <uint64_t> dlDataRxed;
  for (int i = 0; i < m_nUser; i++)
    {
      // get the imsi
      uint64_t imsi = ueDevs.Get (i)-> GetObject<LteUeNetDevice> ()->GetImsi ();
      // get the lcId
      uint8_t lcId = ueDevs.Get (i)-> GetObject<LteUeNetDevice> ()->GetRrc ()->GetLcIdVector ().at(0);
      dlDataRxed.push_back (rlcStats->GetDlRxData (imsi, lcId));
      NS_LOG_INFO ("\tUser " << i << " imsi " << imsi << " bytes rxed " << (double)dlDataRxed.at (i) << "  thr " << (double)dlDataRxed.at (i) / simulationTime << " ref " << m_thrRef);
    }
  /**
  * Check that the assignation is done in a "proportional fair" manner among users
  * with equal SINRs: the bandwidht should be distributed according to the 
  * ratio of the estimated throughput per TTI of each user; therefore equally 
  * partitioning the whole bandwidth achievable from a single users in a TTI
  */
  for (int i = 0; i < m_nUser; i++)
    {
      NS_TEST_ASSERT_MSG_EQ_TOL ((double)dlDataRxed.at (i) / simulationTime, m_thrRef, m_thrRef * tolerance, " Unfair Throughput!");      
    }
  
  Simulator::Destroy ();

}



// --------------- T E S T - C A S E   # 2 ------------------------------


LenaPfFfMacSchedulerTestCase2::LenaPfFfMacSchedulerTestCase2 (uint16_t nUser, std::vector<uint16_t> dist, std::vector<uint16_t> estThrPf)
  : TestCase ("Proportional Fair (PF) Mac Scheduler Test Case"),
    m_nUser (nUser),
    m_dist (dist),
    m_estThrPf (estThrPf)


{
}

LenaPfFfMacSchedulerTestCase2::~LenaPfFfMacSchedulerTestCase2 ()
{
}

void
LenaPfFfMacSchedulerTestCase2::DoRun (void)
{
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
  
//     LogComponentEnable ("PfFfMacScheduler", LOG_LEVEL_ALL);
  LogComponentEnable ("LenaTestPfFfMacCheduler", LOG_LEVEL_ALL);
  //   LogComponentEnable ("LteAmc", LOG_LEVEL_ALL);
  //   LogComponentEnable ("RlcStatsCalculator", LOG_LEVEL_ALL);
  
  /**
  * Initialize Simulation Scenario: 1 eNB and m_nUser UEs
  */
  
  SetVerbose (true);
  Ptr<LenaHelper> lena = CreateObject<LenaHelper> ();
  
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
  lena->SetSchedulerType ("ns3::PfFfMacScheduler");
  enbDevs = lena->InstallEnbDevice (enbNodes);
  ueDevs = lena->InstallUeDevice (ueNodes);
  
  // Attach a UE to a eNB
  lena->Attach (ueDevs, enbDevs.Get (0));
  
  // Activate an EPS bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lena->ActivateEpsBearer (ueDevs, bearer);
  
  // position nodes
  for (int i = 0; i < m_nUser; i++)
  {
    Ptr<ConstantPositionMobilityModel> mm = ueNodes.Get (i)->GetObject<ConstantPositionMobilityModel> ();
    mm->SetPosition (Vector (m_dist.at (i), 0.0, 0.0));
  }
  
  lena->EnableDlRlcTraces();
  
  double simulationTime = 2.0;
  double tolerance = 0.1;
  Simulator::Stop (Seconds (simulationTime));
  
  Ptr<RlcStatsCalculator> rlcStats = lena->GetRlcStats ();
  rlcStats->SetAttribute("EpochDuration", TimeValue(Seconds(simulationTime)));
  
  Simulator::Run ();
  
  NS_LOG_INFO("Test with " << m_nUser << " user(s)");
  std::vector <uint64_t> dlDataRxed;
  double totalData = 0;
  double totalEstThrPf = 0;
  for (int i = 0; i < m_nUser; i++)
  {
    // get the imsi
    uint64_t imsi = ueDevs.Get (i)-> GetObject<LteUeNetDevice> ()->GetImsi ();
    // get the lcId
    uint8_t lcId = ueDevs.Get (i)-> GetObject<LteUeNetDevice> ()->GetRrc ()->GetLcIdVector ().at(0);
    dlDataRxed.push_back (rlcStats->GetDlRxData (imsi, lcId));
    totalData += (double)dlDataRxed.at (i);
    NS_LOG_INFO ("\tUser " << i << "dist" << m_dist.at (i) << " imsi " << imsi << " bytes rxed " << (double)dlDataRxed.at (i) << "  thr " << (double)dlDataRxed.at (i) / simulationTime << " ref " << m_nUser);
    totalEstThrPf += m_estThrPf.at (i);
  }
  
  /**
  * Check that the assignation is done in a "proportional fair" manner among users
  * with different SINRs: the bandwidht should be distributed according to the 
  * ratio of the estimated throughput per TTI of each user.
  */
  for (int i = 0; i < m_nUser; i++)
  {
    double thrRatio = (double)dlDataRxed.at (i) / totalData;
    double estThrRatio = (double)m_estThrPf.at (i) / totalEstThrPf;
    NS_TEST_ASSERT_MSG_EQ_TOL (estThrRatio, thrRatio, tolerance, " Unfair Throughput!");      
  }
  
  Simulator::Destroy ();
  
}




