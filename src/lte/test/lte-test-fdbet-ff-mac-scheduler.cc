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
 *         Nicola Baldo <nbaldo@cttc.es>
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

#include "lte-test-fdbet-ff-mac-scheduler.h"

NS_LOG_COMPONENT_DEFINE ("LenaTestFdBetFfMacCheduler");

namespace ns3 {

LenaTestFdBetFfMacSchedulerSuite::LenaTestFdBetFfMacSchedulerSuite ()
  : TestSuite ("lte-fdbet-ff-mac-scheduler", SYSTEM)
{
  NS_LOG_INFO ("creating LenaTestFdBetFfMacSchedulerSuite");

  //Test Case 1: AMC works in FDBET

  // DOWNLINK- DISTANCE 0 -> MCS 28 -> Itbs 26 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 24 PRB at Itbs 26 -> 2196 -> 2196000 bytes/sec
  // 3 users -> 8 PRB at Itbs 26 -> 749 -> 749000 bytes/sec
  // 6 users -> 4 PRB at Itbs 26 -> 373 -> 373000 bytes/sec
  // 12 users -> 2 PRB at Itbs 26 -> 185 -> 185000 bytes/sec
  // UPLINK- DISTANCE 0 -> MCS 28 -> Itbs 26 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 25 PRB at Itbs 26 -> 2292 -> 2292000 bytes/sec
  // 3 users -> 8 PRB at Itbs 26 -> 749 -> 749000 bytes/sec
  // 6 users -> 4 PRB at Itbs 26 -> 373 -> 373000 bytes/sec
  // 12 users -> 2 PRB at Itbs 26 -> 185 -> 185000 bytes/sec
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase1 (1,0,0,2196000,2292000));
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase1 (3,0,0,749000,749000));
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase1 (6,0,0,373000,373000));
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase1 (12,0,0,185000,185000));

  // DOWNLINK - DISTANCE 3000 -> MCS 22 -> Itbs 20 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 24 PRB at Itbs 20 -> 1383 -> 1383000 bytes/sec
  // 3 users -> 8 PRB at Itbs 20 -> 469 -> 469000 bytes/sec
  // 6 users -> 4 PRB at Itbs 20 -> 233 -> 233000 bytes/sec
  // 12 users -> 2 PRB at Itbs 20 -> 113 -> 113000 bytes/sec
  // UPLINK - DISTANCE 3000 -> MCS 20 -> Itbs 18 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 25 PRB at Itbs 18 -> 1239 -> 1239000 bytes/sec
  // 3 users -> 8 PRB at Itbs 18 -> 389 -> 389000 bytes/sec
  // 6 users -> 4 PRB at Itbs 18 -> 193 -> 193000 bytes/sec
  // 12 users -> 2 PRB at Itbs 18 -> 97 -> 97000 bytes/sec
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase1 (1,0,3000,1383000,1239000));
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase1 (3,0,3000,469000,389000));
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase1 (6,0,3000,233500,193000));
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase1 (12,0,3000,113000,97000));

  // DOWNLINK - DISTANCE 6000 -> MCS 16 -> Itbs 15 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 24 PRB at Itbs 15 -> 903 -> 903000 bytes/sec
  // 3 users -> 8 PRB at Itbs 15 -> 309 -> 309000 bytes/sec
  // 6 users -> 4 PRB at Itbs 15 -> 153 -> 153000 bytes/sec
  // 12 users -> 2 PRB at Itbs 15 -> 75 -> 75000 bytes/sec
  // UPLINK - DISTANCE 6000 -> MCS 12 -> Itbs 11 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 25 PRB at Itbs 11 -> 621 -> 621000 bytes/sec
  // 3 users -> 8 PRB at Itbs 11 -> 201 -> 201000 bytes/sec
  // 6 users -> 4 PRB at Itbs 11 -> 97 -> 97000 bytes/sec
  // 12 users -> 2 PRB at Itbs 11 -> 47 -> 47000 bytes/sec
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase1 (1,0,6000,903000,621000));
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase1 (3,0,6000,309000,201000));
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase1 (6,0,6000,153000,97000));
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase1 (12,0,6000,75000,47000));

  // DOWNLINK - DISTANCE 9000 -> MCS 12 -> Itbs 11 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 24 PRB at Itbs 11 -> 597 -> 597000 bytes/sec
  // 3 users -> 8 PRB at Itbs 11 -> 201 -> 201000 bytes/sec
  // 6 users -> 4 PRB at Itbs 11 -> 97 -> 97000 bytes/sec
  // 12 users -> 2 PRB at Itbs 11 -> 47 -> 47000 bytes/sec
  // UPLINK - DISTANCE 9000 -> MCS 8 -> Itbs 8 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 24 PRB at Itbs 8 -> 437 -> 437000 bytes/sec
  // 3 users -> 8 PRB at Itbs 8 -> 137 -> 137000 bytes/sec
  // 6 users -> 4 PRB at Itbs 8 -> 67 -> 67000 bytes/sec
  // 12 users -> 2 PRB at Itbs 8 -> 32 -> 32000 bytes/sec
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase1 (1,0,9000,597000,437000));
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase1 (3,0,9000,201000,137000));
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase1 (6,0,9000,97000,67000));
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase1 (12,0,9000,47000,32000));

  // DOWNLINK - DISTANCE 15000 -> MCS 6 -> Itbs 6 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 24 PRB at Itbs 6 -> 309 -> 309000 bytes/sec
  // 3 users -> 8 PRB at Itbs 6 -> 101 -> 101000 bytes/sec
  // 6 users -> 4 PRB at Itbs 6 -> 49 -> 49000 bytes/sec
  // 12 users -> 2 PRB at Itbs 6 -> 22 -> 22000 bytes/sec
  // UPLINK - DISTANCE 15000 -> MCS 6 -> Itbs 6 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 25 PRB at Itbs 6 -> 233 -> 233000 bytes/sec
  // 3 users -> 8 PRB at Itbs 6 -> 69 -> 69000 bytes/sec
  // 6 users -> 4 PRB at Itbs 6 -> 32 -> 32000 bytes/sec
  // 12 users -> 2 PRB at Itbs 6 -> 15 -> 15000 bytes/sec
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase1 (1,0,15000,309000,233000));
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase1 (3,0,15000,101000,69000));
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase1 (6,0,15000,49000,32000));
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase1 (12,0,15000,22000,15000));

  // Test Case 2: fairness check

  std::vector<uint16_t> dist;
  dist.push_back (0);       // User 0 distance --> MCS 28
  dist.push_back (3000);    // User 1 distance --> MCS 24
  dist.push_back (6000);    // User 2 distance --> MCS 16
  dist.push_back (9000);    // User 3 distance --> MCS 12
  std::vector<uint32_t> estAchievableRateDl;
  estAchievableRateDl.push_back (2196000);
  estAchievableRateDl.push_back (1383000);
  estAchievableRateDl.push_back (903000);
  estAchievableRateDl.push_back (597000);
  std::vector<uint32_t> estThrFdBetUl;
  estThrFdBetUl.push_back (549000); // User 0 estimated TTI throughput from FDBET
  estThrFdBetUl.push_back (293000); // User 1 estimated TTI throughput from FDBET
  estThrFdBetUl.push_back (149000); // User 2 estimated TTI throughput from FDBET
  estThrFdBetUl.push_back (101000);  // User 3 estimated TTI throughput from FDBET
  AddTestCase (new LenaFdBetFfMacSchedulerTestCase2 (dist, estAchievableRateDl, estThrFdBetUl));


}

static LenaTestFdBetFfMacSchedulerSuite lenaTestFdBetFfMacSchedulerSuite;


// --------------- T E S T - C A S E   # 1 ------------------------------


std::string 
LenaFdBetFfMacSchedulerTestCase1::BuildNameString (uint16_t nUser, uint16_t dist)
{
  std::ostringstream oss;
  oss << nUser << " UEs, distance " << dist << " m";
  return oss.str ();
}

LenaFdBetFfMacSchedulerTestCase1::LenaFdBetFfMacSchedulerTestCase1 (uint16_t nUser, uint16_t nLc, uint16_t dist, double thrRefDl, double thrRefUl)
  : TestCase (BuildNameString (nUser, dist)),
    m_nUser (nUser),
    m_nLc (nLc),
    m_dist (dist),
    m_thrRefDl (thrRefDl),
    m_thrRefUl (thrRefUl)
{
}

LenaFdBetFfMacSchedulerTestCase1::~LenaFdBetFfMacSchedulerTestCase1 ()
{
}

void
LenaFdBetFfMacSchedulerTestCase1::DoRun (void)
{
  Config::SetDefault ("ns3::LteAmc::AmcModel", EnumValue (LteAmc::PiroEW2010));
  Config::SetDefault ("ns3::LteAmc::Ber", DoubleValue (0.00005));
  Config::SetDefault ("ns3::LteSpectrumPhy::CtrlErrorModelEnabled", BooleanValue (false));
  Config::SetDefault ("ns3::LteSpectrumPhy::DataErrorModelEnabled", BooleanValue (false));

  LogComponentDisableAll (LOG_LEVEL_ALL);
  //LogComponentEnable ("LenaTestFdBetFfMacCheduler", LOG_LEVEL_ALL);

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
  lteHelper->SetSchedulerType ("ns3::FdBetFfMacScheduler");
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

  double simulationTime = 1.0;
  double tolerance = 0.1;
  Simulator::Stop (Seconds (simulationTime));

  Ptr<RadioBearerStatsCalculator> rlcStats = lteHelper->GetRlcStats ();
  rlcStats->SetAttribute ("EpochDuration", TimeValue (Seconds (simulationTime)));

  Simulator::Run ();

  /**
   * Check that the downlink assignation is done in a "FD blind equal throughput" manner
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
      NS_LOG_INFO ("\tUser " << i << " imsi " << imsi << " bytes rxed " << (double)dlDataRxed.at (i) << "  thr " << (double)dlDataRxed.at (i) / simulationTime << " ref " << m_thrRefDl);
    }
  /**
  * Check that the assignation is done in a "FD blind equal throughput" manner among users
  * with equal SINRs: the bandwidth should be distributed according to the 
  * ratio of the estimated throughput per TTI of each user; therefore equally 
  * partitioning the whole bandwidth achievable from a single users in a TTI
  */
  for (int i = 0; i < m_nUser; i++)
    {
      NS_TEST_ASSERT_MSG_EQ_TOL ((double)dlDataRxed.at (i) / simulationTime, m_thrRefDl, m_thrRefDl * tolerance, " Unfair Throughput!");
    }

  /**
  * Check that the uplink assignation is done in a "FD blind equal throughput" manner
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
  /**
  * Check that the assignation is done in a "FD blind equal throughput" manner among users
  * with equal SINRs: the bandwidht should be distributed according to the 
  * ratio of the estimated throughput per TTI of each user; therefore equally 
  * partitioning the whole bandwidth achievable from a single users in a TTI
  */
  for (int i = 0; i < m_nUser; i++)
    {
      NS_TEST_ASSERT_MSG_EQ_TOL ((double)ulDataRxed.at (i) / simulationTime, m_thrRefUl, m_thrRefUl * tolerance, " Unfair Throughput!");
    }
  Simulator::Destroy ();

}



// --------------- T E S T - C A S E   # 2 ------------------------------


std::string 
LenaFdBetFfMacSchedulerTestCase2::BuildNameString (uint16_t nUser, std::vector<uint16_t> dist)
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


LenaFdBetFfMacSchedulerTestCase2::LenaFdBetFfMacSchedulerTestCase2 (std::vector<uint16_t> dist, std::vector<uint32_t> estAchievableRateDl, std::vector<uint32_t> estThrFdBetUl)
  : TestCase (BuildNameString (dist.size (), dist)),
    m_nUser (dist.size ()),
    m_dist (dist),
    m_achievableRateDl (estAchievableRateDl),
    m_estThrFdBetUl (estThrFdBetUl)
{
}

LenaFdBetFfMacSchedulerTestCase2::~LenaFdBetFfMacSchedulerTestCase2 ()
{
}

void
LenaFdBetFfMacSchedulerTestCase2::DoRun (void)
{
  LogComponentDisableAll (LOG_LEVEL_ALL);
  //LogComponentEnable ("LenaTestFdBetFfMacCheduler", LOG_LEVEL_ALL);

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
  lteHelper->SetSchedulerType ("ns3::FdBetFfMacScheduler");
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
      mm->SetPosition (Vector (m_dist.at (i), 0.0, 0.0));
      Ptr<LteUeNetDevice> lteUeDev = ueDevs.Get (i)->GetObject<LteUeNetDevice> ();
      Ptr<LteUePhy> uePhy = lteUeDev->GetPhy ();
      uePhy->SetAttribute ("TxPower", DoubleValue (23.0));
      uePhy->SetAttribute ("NoiseFigure", DoubleValue (9.0));
    }

  lteHelper->EnableRlcTraces ();

  double simulationTime = 1;
  double tolerance = 0.1;
  Simulator::Stop (Seconds (simulationTime));

  Ptr<RadioBearerStatsCalculator> rlcStats = lteHelper->GetRlcStats ();
  rlcStats->SetAttribute ("EpochDuration", TimeValue (Seconds (simulationTime)));

  Simulator::Run ();

  NS_LOG_INFO ("DL - Test with " << m_nUser << " user(s)");
  std::vector <uint64_t> dlDataRxed;
  double totalData = 0;
  double estTotalThr = 0;
  double estUeThr = 0;
  for (int i = 0; i < m_nUser; i++)
    {
      // get the imsi
      uint64_t imsi = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetImsi ();
      // get the lcId
      uint8_t lcId = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetRrc ()->GetLcIdVector ().at (0);
      dlDataRxed.push_back (rlcStats->GetDlRxData (imsi, lcId));
      totalData += (double)dlDataRxed.at (i);
      estTotalThr += 1 / m_achievableRateDl.at (i);
      //NS_LOG_INFO ("\tUser " << i << " dist " << m_dist.at (i) << " imsi " << imsi << " bytes rxed " << (double)dlDataRxed.at (i) << "  thr " << (double)dlDataRxed.at (i) / simulationTime << " ref " << m_nUser);
    }
    
  estTotalThr = m_nUser * (1 / estTotalThr);
  estUeThr = estTotalThr / m_nUser;
  /**
  * Check that the assignation is done in a "FD blind equal throughput" manner among users
  * with different SINRs: the bandwidth should be distributed equally in long term
  */
  for (int i = 0; i < m_nUser; i++)
    {
      double thrRatio = (double) 1 / m_nUser;
      double estThrRatio = (double)dlDataRxed.at (i) / totalData;
      NS_LOG_INFO ("\tUser " << i << " thrRatio " << thrRatio << " estThrRatio " << estThrRatio);
      NS_TEST_ASSERT_MSG_EQ_TOL (estThrRatio, thrRatio, tolerance, " Unfair Throughput!");
      NS_TEST_ASSERT_MSG_EQ_TOL ((double)dlDataRxed.at (i) / simulationTime, estUeThr, estUeThr * tolerance, " Unfair Throughput!");

    }

  /**
  * Check that the assignation in uplink is done in a round robin manner.
  */

  NS_LOG_INFO ("UL - Test with " << m_nUser);
  std::vector <uint64_t> ulDataRxed;
  for (int i = 0; i < m_nUser; i++)
    {
      // get the imsi
      uint64_t imsi = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetImsi ();
      // get the lcId
      uint8_t lcId = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetRrc ()->GetLcIdVector ().at (0);
      ulDataRxed.push_back (rlcStats->GetUlRxData (imsi, lcId));
      NS_LOG_INFO ("\tUser " << i << " dist " << m_dist.at (i) << " bytes rxed " << (double)ulDataRxed.at (i) << "  thr " << (double)ulDataRxed.at (i) / simulationTime << " ref " << (double)m_estThrFdBetUl.at (i));
      NS_TEST_ASSERT_MSG_EQ_TOL ((double)ulDataRxed.at (i) / simulationTime, (double)m_estThrFdBetUl.at (i), (double)m_estThrFdBetUl.at (i) * tolerance, " Unfair Throughput!");
    }
  Simulator::Destroy ();

}


} // namespace ns3




