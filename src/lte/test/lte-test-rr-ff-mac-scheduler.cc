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

#include "ns3/lte-test-rr-ff-mac-scheduler.h"
#include <ns3/eps-bearer.h>
#include <ns3/node-container.h>
#include <ns3/mobility-helper.h>
#include <ns3/net-device-container.h>
#include <ns3/lena-helper.h>

NS_LOG_COMPONENT_DEFINE ("LenaTestRrFfMacCheduler");

using namespace ns3;


LenaTestRrFfMacSchedulerSuite::LenaTestRrFfMacSchedulerSuite ()
: TestSuite ("lteTestRrFfMacScheduler", SYSTEM)
{
  SetVerbose (true);
  NS_LOG_INFO ("creating LenaRrFfMacSchedulerTestCase");
  AddTestCase (new LenaRrFfMacSchedulerTestCase);
}

static LenaTestRrFfMacSchedulerSuite lenaTestRrFfMacSchedulerSuite;

LenaRrFfMacSchedulerTestCase::LenaRrFfMacSchedulerTestCase ()
  : TestCase ("Round Robin (RR) Mac Scheduler Test Case")
{
}

LenaRrFfMacSchedulerTestCase::~LenaRrFfMacSchedulerTestCase ()
{
}

void
LenaRrFfMacSchedulerTestCase::DoRun (void)
{
  LogComponentEnable ("LteEnbRrc", LOG_LEVEL_ALL);
  LogComponentEnable ("LteUeRrc", LOG_LEVEL_ALL);
  LogComponentEnable ("LteEnbMac", LOG_LEVEL_ALL);
  LogComponentEnable ("LteUeMac", LOG_LEVEL_ALL);
  LogComponentEnable ("LteRlc", LOG_LEVEL_ALL);
  LogComponentEnable ("RrPacketScheduler", LOG_LEVEL_ALL);

  //LogComponentEnable ("LtePhy", LOG_LEVEL_ALL);
  //LogComponentEnable ("LteEnbPhy", LOG_LEVEL_ALL);
  //LogComponentEnable ("LteUePhy", LOG_LEVEL_ALL);

  //LogComponentEnable ("LteSpectrumPhy", LOG_LEVEL_ALL);
  //LogComponentEnable ("LteInterference", LOG_LEVEL_ALL);
  //LogComponentEnable ("LteSinrChunkProcessor", LOG_LEVEL_ALL);

//   LogComponentEnable ("LtePropagationLossModel", LOG_LEVEL_ALL);
//   LogComponentEnable ("LossModel", LOG_LEVEL_ALL);
//   LogComponentEnable ("ShadowingLossModel", LOG_LEVEL_ALL);
//   LogComponentEnable ("PenetrationLossModel", LOG_LEVEL_ALL);
//   LogComponentEnable ("MultipathLossModel", LOG_LEVEL_ALL);
//   LogComponentEnable ("PathLossModel", LOG_LEVEL_ALL);

  LogComponentEnable ("LteNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("LteUeNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("LteEnbNetDevice", LOG_LEVEL_ALL);

  LogComponentEnable ("LenaTestRrFfMacCheduler", LOG_LEVEL_ALL);
  LogComponentEnable ("RlcStatsCalculator", LOG_LEVEL_ALL);
  

  /**
   * Initialize Simulation Scenario: 1 eNB and 2 UEs
   */
  
  SetVerbose (true);
  
  Ptr<LenaHelper> lena = CreateObject<LenaHelper> ();
  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes;
  enbNodes.Create (1);
  ueNodes.Create (1);
  
  // Install Mobility Model
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (enbNodes);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (ueNodes);
  
  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
  lena->SetSchedulerType ("ns3::RrFfMacScheduler");
  enbDevs = lena->InstallEnbDevice (enbNodes);
  ueDevs = lena->InstallUeDevice (ueNodes);
  
  // Attach a UE to a eNB
  lena->Attach (ueDevs, enbDevs.Get (3));
  
  // Activate an EPS bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lena->ActivateEpsBearer (ueDevs, bearer);
  
  lena->EnableRlcTraces ();

  Simulator::Stop (Seconds (1.0));
  
  Ptr<RlcStatsCalculator> rlcStats = lena->GetRlcStats ();
  
  Simulator::Run ();
  
  Simulator::Destroy ();

  /**
   * Check that the assignation is done in a RR fashion
   */
  //NS_TEST_ASSERT_MSG_EQ (*rxPsd, saveRxPsd, "Data signal corrupted !");
  //NS_TEST_ASSERT_MSG_EQ (*noisePsd, saveNoisePsd, "Noise signal corrupted !");

  //SpectrumValue theoreticalSinr = (*rxPsd) / ( ( 2 * (*rxPsd) ) + (*noisePsd) );
  //SpectrumValue calculatedSinr = p->GetSinr ();
  double uData1 = (double)rlcStats->GetDlRxData (1) / 1.0;
  double uData2 = (double)rlcStats->GetDlRxData (2) / 1.0;
  double uData3 = (double)rlcStats->GetDlRxData (3) / 1.0;
  NS_LOG_INFO ("User 1 Rx Data: " << uData1);
  NS_LOG_INFO ("User 2 Rx Data: " << uData2);
  NS_LOG_INFO ("User 3 Rx Data: " << uData3);
  NS_TEST_ASSERT_MSG_EQ_TOL (rlcStats->GetDlRxData (1), rlcStats->GetDlRxData (2), 0.0000001, " Unfair Throughput!");

  //NS_TEST_ASSERT_MSG_EQ_TOL (calculatedSinr, theoreticalSinr, 0.000001, "Wrong SINR !");
}

