/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011, 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 *         Nicola Baldo <nbaldo@cttc.es>
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
#include <ns3/config-store-module.h>

#include "lte-test-rr-ff-mac-scheduler.h"

NS_LOG_COMPONENT_DEFINE ("LenaTestRrFfMacScheduler");

namespace ns3 {


LenaTestRrFfMacSchedulerSuite::LenaTestRrFfMacSchedulerSuite ()
  : TestSuite ("lte-rr-ff-mac-scheduler", SYSTEM)
{
  NS_LOG_INFO ("creating LenaRrFfMacSchedulerTestCase");

  bool errorModel = true;

  
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
  // after the patch enforcing min 3 PRBs per UE:
  // 9 users -> 3 PRB at Itbs 26 -> 277 bytes * 8/9 UE/TTI ->  246220 bytes/sec
  // 12 users -> 3 PRB at Itbs 26 -> 277 bytes * 8/12 UE/TTI -> 184670 bytes/sec
  // 15 users -> 3 PRB at Itbs 26 -> 277 bytes * 8/15 UE/TTI -> 147730 bytes/sec
  AddTestCase (new LenaRrFfMacSchedulerTestCase (1,0,0,2196000,2292000,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (3,0,0,749000,749000,errorModel), TestCase::QUICK);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (6,0,0,373000,373000,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (9,0,0,185000,246220,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (12,0,0,185000,184670,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (15,0,0,148000,147730,errorModel), TestCase::EXTENSIVE);

  // DOWNLINK - DISTANCE 4800 -> MCS 22 -> Itbs 20 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 24 PRB at Itbs 20 -> 1383 -> 1383000 bytes/sec
  // 3 users -> 8 PRB at Itbs 20 -> 469 -> 469000 bytes/sec
  // 6 users -> 4 PRB at Itbs 20 -> 233 -> 233000 bytes/sec
  // 9 user -> 2 PRB at Itbs 20 -> 113 -> 113000 bytes/sec
  // 12 users -> 2 PRB at Itbs 20 -> 113 -> 113000 bytes/sec
  // 15 users -> 2 PRB at Itbs 20 * 0.8 -> 90.4 -> 90400 bytes/sec
  // UPLINK - DISTANCE 4800 -> MCS 14 -> Itbs 13 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 25 PRB at Itbs 13 -> 807 -> 807000 bytes/sec
  // 3 users -> 8 PRB at Itbs 13 -> 253 -> 253000 bytes/sec
  // 6 users -> 4 PRB at Itbs 13 -> 125 -> 125000 bytes/sec
  // after the patch enforcing min 3 PRBs per UE:
  // 9 users -> 3 PRB at Itbs 13 -> 93  bytes * 8/9 UE/TTI  -> 82667 bytes/sec
  // 12 users -> 3 PRB at Itbs 13 -> 93  bytes * 8/12 UE/TTI  -> 62000 bytes/sec
  // 15 users -> 3 PRB at Itbs 13 -> 93  bytes * 8/15 UE/TTI  -> 49600 bytes/sec
  AddTestCase (new LenaRrFfMacSchedulerTestCase (1,0,4800,1383000,807000,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (3,0,4800,469000,253000,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (6,0,4800,233000,125000,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (9,0,4800,113000,82667,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (12,0,4800,113000,62000,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (15,0,4800,90400,49600,errorModel), TestCase::EXTENSIVE);

  // DOWNLINK - DISTANCE 6000 -> MCS 14 -> Itbs 13 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 24 PRB at Itbs 15 -> 775 -> 775000 bytes/sec
  // 3 users -> 8 PRB at Itbs 15 -> 253 -> 253000 bytes/sec
  // 6 users -> 4 PRB at Itbs 15 -> 125 -> 125000 bytes/sec
  // 9 user -> 2 PRB at Itbs 15 -> 61 -> 61000 bytes/sec
  // 12 users -> 2 PRB at Itbs 15 -> 61 -> 61000 bytes/sec
  // 15 users -> 2 PRB at Itbs 15 * 0.8 -> 48.8 -> 48800 bytes/sec
  // UPLINK - DISTANCE 6000 -> MCS 12 -> Itbs 11 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 25 PRB at Itbs 11 -> 621 -> 621000 bytes/sec
  // 3 users -> 8 PRB at Itbs 11 -> 201 -> 201000 bytes/sec
  // 6 users -> 4 PRB at Itbs 11 -> 97 -> 97000 bytes/sec
  // 9 users -> 3 PRB at Itbs 11 -> 73 bytes * 8/9 UE/TTI -> 64889 bytes/sec
  // 12 users -> 3 PRB at Itbs 11 -> 73 bytes * 8/12 UE/TTI -> 48667 bytes/sec
  // 15 users -> 3 PRB at Itbs 11 -> 73 bytes * 8/15 UE/TTI -> 38993 bytes/sec
  AddTestCase (new LenaRrFfMacSchedulerTestCase (1,0,6000,775000,621000,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (3,0,6000,253000,201000,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (6,0,6000,125000,97000,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (9,0,6000,61000,64889,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (12,0,6000,61000,48667,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (15,0,6000,48800,38993,errorModel), TestCase::EXTENSIVE);

  // DOWNLINK - DISTANCE 10000 -> MCS 8 -> Itbs 8 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 24 PRB at Itbs 8 -> 437 -> 437000 bytes/sec
  // 3 users -> 8 PRB at Itbs 8 -> 137 -> 137000 bytes/sec
  // 6 users -> 4 PRB at Itbs 8 -> 67 -> 67000 bytes/sec
  // 9 user -> 2 PRB at Itbs 8 -> 32 -> 32000 bytes/sec
  // 12 users -> 2 PRB at Itbs 8 -> 32 -> 32000 bytes/sec
  // 15 users -> 2 PRB at Itbs 8 * 0.8 -> 25.6 -> 25600 bytes/sec
  // UPLINK - DISTANCE 9000 -> MCS 8 -> Itbs 8 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 24 PRB at Itbs 8 -> 437 -> 437000 bytes/sec
  // 3 users -> 8 PRB at Itbs 8 -> 137 -> 137000 bytes/sec
  // 6 users -> 4 PRB at Itbs 8 -> 67 -> 67000 bytes/sec
  // after the patch enforcing min 3 PRBs per UE:
  // 9 users -> 3 PRB at Itbs 8 -> 49 bytes * 8/9 UE/TTI -> 43556 bytes/sec
  // 12 users -> 3 PRB at Itbs 8 -> 49 bytes * 8/12 UE/TTI -> 32667 bytes/sec
  // 15 users -> 3 PRB at Itbs 8 -> 49 bytes * 8/15 UE/TTI -> 26133 bytes/sec
  AddTestCase (new LenaRrFfMacSchedulerTestCase (1,0,10000,437000,437000,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (3,0,10000,137000,137000,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (6,0,10000,67000,67000,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (9,0,10000,32000,43556,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (12,0,10000,32000,32667,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (15,0,10000,25600,26133,errorModel), TestCase::EXTENSIVE);

  // DOWNLINK - DISTANCE 20000 -> MCS 8 -> Itbs 8 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 24 PRB at Itbs 8 -> 421 -> 421000 bytes/sec
  // 3 users -> 8 PRB at Itbs 8 -> 137 -> 137000 bytes/sec
  // 6 users -> 4 PRB at Itbs 8 -> 67 -> 67000 bytes/sec
  // 9 user -> 2 PRB at Itbs 8 -> 32 -> 32000 bytes/sec
  // 12 users -> 2 PRB at Itbs 8 -> 32 -> 32000 bytes/sec
  // 15 users -> 2 PRB at Itbs 8 * 0.8 -> 25.6 -> 25600 bytes/sec
  // UPLINK - DISTANCE 20000 -> MCS 2 -> Itbs 2 (from table 7.1.7.2.1-1 of 36.213)
  // 1 user -> 25 PRB at Itbs 2 -> 137 -> 137000 bytes/sec
  // 3 users -> 8 PRB at Itbs 2 -> 41 -> 41000 bytes/sec
  // 6 users -> 4 PRB at Itbs 2 -> 22 -> 22000 bytes/sec
  // after the patch enforcing min 3 PRBs per UE:
  // 9 users -> 3 PRB at Itbs 2 -> 18 bytes * 8/9 UE/TTI -> 16000 bytes/sec
  // 12 users -> 3 PRB at Itbs 2 -> 18 bytes * 8/12 UE/TTI -> 12000 bytes/sec
  // 15 users -> 3 PRB at Itbs 2 -> 18 bytes * 8/15 UE/TTI -> 9600 bytes/sec
  AddTestCase (new LenaRrFfMacSchedulerTestCase (1,0,20000,421000,137000,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (3,0,20000,137000,41000,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (6,0,20000,67000,22000,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (9,0,20000,32000,16000,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (12,0,20000,32000,12000,errorModel), TestCase::EXTENSIVE);
  AddTestCase (new LenaRrFfMacSchedulerTestCase (15,0,20000,25600,9600,errorModel), TestCase::EXTENSIVE);

}

static LenaTestRrFfMacSchedulerSuite lenaTestRrFfMacSchedulerSuite;

std::string 
LenaRrFfMacSchedulerTestCase::BuildNameString (uint16_t nUser, uint16_t dist)
{
  std::ostringstream oss;
  oss << nUser << " UEs, distance " << dist << " m";
  return oss.str ();
}

LenaRrFfMacSchedulerTestCase::LenaRrFfMacSchedulerTestCase (uint16_t nUser, uint16_t nLc, uint16_t dist, double thrRefDl, double thrRefUl, bool errorModelEnabled)
  : TestCase (BuildNameString (nUser, dist)),              
    m_nUser (nUser),
    m_nLc (nLc),
    m_dist (dist),
    m_thrRefDl (thrRefDl),
    m_thrRefUl (thrRefUl),
    m_errorModelEnabled (errorModelEnabled)
{
}

LenaRrFfMacSchedulerTestCase::~LenaRrFfMacSchedulerTestCase ()
{
}

void
LenaRrFfMacSchedulerTestCase::DoRun (void)
{
  NS_LOG_FUNCTION (this << m_nUser << m_nLc << m_dist);
  if (!m_errorModelEnabled)
    {
      Config::SetDefault ("ns3::LteSpectrumPhy::CtrlErrorModelEnabled", BooleanValue (false));
      Config::SetDefault ("ns3::LteSpectrumPhy::DataErrorModelEnabled", BooleanValue (false));
    }
  Config::SetDefault ("ns3::LteHelper::UseIdealRrc", BooleanValue (false));
  LogComponentDisableAll (LOG_LEVEL_ALL);
  //LogComponentEnable ("LenaTestRrFfMacCheduler", LOG_LEVEL_ALL);

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
  lteHelper->ActivateDataRadioBearer (ueDevs, bearer);
  
 
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


  double statsStartTime = 0.300; // need to allow for RRC connection establishment + SRS
  double statsDuration = 0.4;
  double tolerance = 0.1;
  Simulator::Stop (Seconds (statsStartTime + statsDuration - 0.0001));

  lteHelper->EnableRlcTraces ();
  Ptr<RadioBearerStatsCalculator> rlcStats = lteHelper->GetRlcStats ();
  rlcStats->SetAttribute ("StartTime", TimeValue (Seconds (statsStartTime)));
  rlcStats->SetAttribute ("EpochDuration", TimeValue (Seconds (statsDuration)));

  Simulator::Run ();

  /**
   * Check that the assignation is done in a RR fashion
   */
  NS_LOG_INFO ("DL - Test with " << m_nUser << " user(s) at distance " << m_dist);
  std::vector <uint64_t> dlDataRxed;
  if (m_errorModelEnabled)
    {
      m_thrRefDl *= 0.95; // for couting the Vienna AMC behavior: BLER between 0% and 10%
    }
  for (int i = 0; i < m_nUser; i++)
    {
      // get the imsi
      uint64_t imsi = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetImsi ();
      uint8_t lcId = 3;
      dlDataRxed.push_back (rlcStats->GetDlRxData (imsi, lcId));
      NS_LOG_INFO ("\tUser " << i << " imsi " << imsi << " lcid " << (uint16_t) lcId << " bytes rxed " << (double)dlDataRxed.at (i) << "  thr " << (double)dlDataRxed.at (i) / statsDuration << " ref " << m_thrRefDl);
      NS_TEST_ASSERT_MSG_EQ_TOL ((double)dlDataRxed.at (i) / statsDuration, m_thrRefDl, m_thrRefDl * tolerance, " Unfair Throughput!");
    }

  NS_LOG_INFO ("UL - Test with " << m_nUser << " user(s) at distance " << m_dist);
  std::vector <uint64_t> ulDataRxed;
  if (m_errorModelEnabled)
    {
      m_thrRefUl *= 0.95; // for couting the Vienna AMC behavior: BLER between 0% and 10%
    }
  for (int i = 0; i < m_nUser; i++)
    {
      // get the imsi
      uint64_t imsi = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetImsi ();
      // get the lcId
      uint8_t lcId = 3;
      ulDataRxed.push_back (rlcStats->GetUlRxData (imsi, lcId));
      NS_LOG_INFO ("\tUser " << i << " imsi " << imsi << " lcid " << (uint16_t) lcId << " bytes rxed " << (double)ulDataRxed.at (i) << "  thr " << (double)ulDataRxed.at (i) / statsDuration << " ref " << m_thrRefUl << " txed " << rlcStats->GetUlTxData (imsi, lcId) / statsDuration);
      NS_TEST_ASSERT_MSG_EQ_TOL ((double)ulDataRxed.at (i) / statsDuration, m_thrRefUl, m_thrRefUl * tolerance, " Unfair Throughput!");
    }

  Simulator::Destroy ();
}


} // namespace ns3

