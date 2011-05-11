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
#include <ns3/lena-helper.h>
#include "ns3/string.h"
#include "ns3/double.h"


NS_LOG_COMPONENT_DEFINE ("LenaTestPfFfMacCheduler");

using namespace ns3;

LenaTestPfFfMacSchedulerSuite::LenaTestPfFfMacSchedulerSuite ()
: TestSuite ("lte-test-pf-ff-mac-scheduler", SYSTEM)
{
  SetVerbose (true);
  NS_LOG_INFO ("creating LenaPfFfMacSchedulerTestCase");
  
  AddTestCase (new LenaPfFfMacSchedulerTestCase (3,0,8000,2196000));
  
//   AddTestCase (new LenaPfFfMacSchedulerTestCase (1,0,0,2196000));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase (3,0,0,749000));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase (6,0,0,373000));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase (12,0,0,185000));
//   AddTestCase (new LenaPfFfMacSchedulerTestCase (15,0,0,148000));
}

static LenaTestPfFfMacSchedulerSuite lenaTestPfFfMacSchedulerSuite;

LenaPfFfMacSchedulerTestCase::LenaPfFfMacSchedulerTestCase (uint16_t nUser, uint16_t nLc, uint16_t dist, double thrRef)
: TestCase ("Proportional Fair (PF) Mac Scheduler Test Case"),
    m_nUser (nUser),
    m_nLc (nLc),
    m_dist (dist),
    m_thrRef (thrRef)
{
}

LenaPfFfMacSchedulerTestCase::~LenaPfFfMacSchedulerTestCase ()
{
}

void
LenaPfFfMacSchedulerTestCase::DoRun (void)
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
  //LogComponentEnable ("LteAmc", LOG_LEVEL_ALL);
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
  for (int i = 1; i < m_nUser; i++)
    {
      Ptr<ConstantPositionMobilityModel> mm = ueNodes.Get (i)->GetObject<ConstantPositionMobilityModel> ();
      mm->SetPosition (Vector (m_dist, 0.0, 0.0));
    }
    
  lena->EnableDlRlcTraces();
  
  double simulationTime = 0.50;
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
      dlDataRxed.push_back (rlcStats->GetDlRxData (imsi));
      NS_LOG_INFO ("\tUser " << i << " imsi " << imsi << " bytes rxed " << (double)dlDataRxed.at (i) << "  thr " << (double)dlDataRxed.at (i) / simulationTime << " ref " << m_thrRef);
      //NS_TEST_ASSERT_MSG_EQ_TOL ((double)dlDataRxed.at (i) / simulationTime, m_thrRef, m_thrRef * tolerance, " Unfair Throughput!");      
    }
  
  for (int i = 0; i < m_nUser; i++)
    {
      NS_TEST_ASSERT_MSG_EQ_TOL ((double)dlDataRxed.at (i) / simulationTime, m_thrRef, m_thrRef * tolerance, " Unfair Throughput!");      
    }
  
  Simulator::Destroy ();

}


