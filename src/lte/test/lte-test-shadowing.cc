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

#include "ns3/simulator.h"

#include "ns3/log.h"

#include "ns3/spectrum-test.h"

#include "ns3/lte-phy-tag.h"
#include "ns3/lte-test-ue-phy.h"
#include "ns3/lte-sinr-chunk-processor.h"

#include "ns3/lte-test-shadowing.h"
#include <ns3/buildings-propagation-loss-model.h>
#include <ns3/node-container.h>
#include <ns3/mobility-helper.h>
#include <ns3/lena-helper.h>
#include <ns3/single-model-spectrum-channel.h>
#include "ns3/string.h"
#include "ns3/double.h"
#include <ns3/building.h>
#include <ns3/enum.h>
#include <ns3/net-device-container.h>
#include <ns3/lte-ue-net-device.h>
#include <ns3/lte-enb-net-device.h>
#include <ns3/lte-ue-rrc.h>
#include <ns3/lena-helper.h>
#include <ns3/lte-enb-phy.h>
#include <ns3/lte-ue-phy.h>
#include "lte-test-sinr-chunk-processor.h"

NS_LOG_COMPONENT_DEFINE ("LteShadowingTest");

using namespace ns3;


/**
* Test 1.1 Shadowing compound test
*/

/**
* This TestSuite tests the shadowing model of BuildingPathlossModel 
* by reproducing several communication scenarios 
*/


LteShadowingTestSuite::LteShadowingTestSuite ()
: TestSuite ("lte-shadowing-model", SYSTEM)
{
  
  
  // -------------- UNIT TESTS ----------------------------------
  
  LogComponentEnable ("LteShadowingTest", LOG_LEVEL_ALL);
  
  // NS_LOG_INFO ("Creating LteDownlinkSinrTestSuite");
  
  Ptr<LenaHelper> lena = CreateObject<LenaHelper> ();
  
  lena->SetAttribute ("PropagationModel", StringValue ("ns3::BuildingsPropagationLossModel"));
  
  // Create Nodes: eNodeB, home eNB, UE and home UE (UE attached to HeNB)
  NodeContainer enbNodes;
  NodeContainer henbNodes;
  NodeContainer ueNodes;
  NodeContainer hueNodes;
  enbNodes.Create (1);
  henbNodes.Create (2);
  ueNodes.Create (5);
  hueNodes.Create (3);
  
  // Install Mobility Model
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::BuildingsMobilityModel");
  mobility.Install (enbNodes);
  mobility.Install (henbNodes);
  mobility.Install (ueNodes);
  mobility.Install (hueNodes);
  
  NetDeviceContainer enbDevs;
  NetDeviceContainer henbDevs;
  NetDeviceContainer ueDevs;
  NetDeviceContainer hueDevs;
  enbDevs = lena->InstallEnbDevice (enbNodes);
  ueDevs = lena->InstallUeDevice (ueNodes);
  henbDevs = lena->InstallEnbDevice (henbNodes);
  hueDevs = lena->InstallUeDevice (hueNodes);
  
  
  
  lena->Attach (ueDevs, enbDevs.Get (0));
  lena->Attach (hueDevs, henbDevs.Get (0));
  
  // Test #1 Outdoor Model
  
  double distance = 2000;
  double hm = 1;
  double hb = 30;
  double freq = 869e6; // E_UTRA BAND #5 see table 5.5-1 of 36.101
  Ptr<BuildingsMobilityModel> mm1 = enbNodes.Get (0)->GetObject<BuildingsMobilityModel> ();
  mm1->SetPosition (Vector (0.0, 0.0, hb));
  
  Ptr<BuildingsMobilityModel> mm2 = ueNodes.Get (0)->GetObject<BuildingsMobilityModel> ();
  mm2->SetPosition (Vector (distance, 0.0, hm));
  
  AddTestCase (new LteShadowingTestCase (mm1, mm2, 148.86, 7.0, "Outdoor Shadowing"));
  
  // Test #2 Indoor model
  
  distance = 30;
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  double henbHeight = 10.0;
  Ptr<BuildingsMobilityModel> mm5 = henbNodes.Get (0)->GetObject<BuildingsMobilityModel> ();
  mm5->SetPosition (Vector (0.0, 0.0, henbHeight));
  Ptr<Building> building1 = Create<Building> (0.0, 10.0, 0.0, 10.0, 0.0, 20.0/*, 1, 1, 1*/);
  building1->SetBuildingType (Building::Residential);
  building1->SetExtWallsType (Building::ConcreteWithWindows);
  mm5->SetIndoor (building1);
  Ptr<BuildingsMobilityModel> mm6 = hueNodes.Get (0)->GetObject<BuildingsMobilityModel> ();
  mm6->SetPosition (Vector (distance, 0.0, hm));
  mm6->SetIndoor (building1);
  mm6->SetFloorNumber (2);
  AddTestCase (new LteShadowingTestCase (mm5, mm6, 88.5724, 8.0, "Indoor Shadowing"));
  
  // Test #3 Indoor -> Outdoor
  
  distance = 100;
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  Ptr<BuildingsMobilityModel> mm9 = henbNodes.Get (1)->GetObject<BuildingsMobilityModel> ();
  mm9->SetPosition (Vector (0.0, 0.0, henbHeight));
  mm9->SetIndoor (building1);
  mm9->SetFloorNumber (2);
  Ptr<BuildingsMobilityModel> mm10 = hueNodes.Get (1)->GetObject<BuildingsMobilityModel> ();
  mm10->SetPosition (Vector (distance, 0.0, hm));
  // The loss is similar of test #4 plus the building penetration loss
  // which for ConcreteWithWindows is equal to 7 dB and the height gain
  // (2 floors x 2 dB/floor = 4) -> 81.838 + 7 - 4 = 84.838
  AddTestCase (new LteShadowingTestCase (mm9, mm10, 85.0012, 8.6, "Indoor -> Outdoor Shadowing"));

}

static LteShadowingTestSuite lteShadowingTestSuite;


/**
* TestCase
*/

LteShadowingTestCase::LteShadowingTestCase (Ptr<BuildingsMobilityModel> m1, Ptr<BuildingsMobilityModel> m2, double refValue, double sigmaRef, std::string name)
: TestCase ("SHADOWING calculation: " + name),
m_node1 (m1),
m_node2 (m2),
m_lossRef (refValue),
m_sigmaRef (sigmaRef)
{
}

LteShadowingTestCase::~LteShadowingTestCase ()
{
}

void
LteShadowingTestCase::DoRun (void)
{
//     LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);
  
  //   LogComponentEnable ("LteEnbRrc", logLevel);
  //   LogComponentEnable ("LteUeRrc", logLevel);
  //   LogComponentEnable ("LteEnbMac", logLevel);
  //   LogComponentEnable ("LteUeMac", logLevel);
  //   LogComponentEnable ("LteRlc", logLevel);
  //   LogComponentEnable ("RrPacketScheduler", logLevel);
  // 
  //   LogComponentEnable ("LtePhy", logLevel);
  //   LogComponentEnable ("LteEnbPhy", logLevel);
  //   LogComponentEnable ("LteUePhy", logLevel);
  // 
  //   LogComponentEnable ("LteSpectrumPhy", logLevel);
  //   LogComponentEnable ("LteInterference", logLevel);
  //   LogComponentEnable ("LteSinrChunkProcessor", logLevel);
  // 
  //   LogComponentEnable ("LtePropagationLossModel", logLevel);
  //   LogComponentEnable ("LossModel", logLevel);
//     LogComponentEnable ("ShadowingLossModel", logLevel);
  //   LogComponentEnable ("PenetrationLossModel", logLevel);
  //   LogComponentEnable ("MultipathLossModel", logLevel);
  //   LogComponentEnable ("PathLossModel", logLevel);
  // 
  //   LogComponentEnable ("LteNetDevice", logLevel);
  //   LogComponentEnable ("LteUeNetDevice", logLevel);
  //   LogComponentEnable ("LteEnbNetDevice", logLevel);
  
//   LogComponentEnable ("BuildingsPropagationLossModel", LOG_LEVEL_ALL);
  NS_LOG_INFO ("Testing " << GetName());
  std::vector<double> loss;
  double sum = 0.0;
  double sumSquared = 0.0;
  int samples = 10000;
  for (int i = 0; i < samples; i++)
  {
  Ptr<SpectrumChannel> m_downlinkChannel = CreateObject<SingleModelSpectrumChannel> ();
  Ptr<SpectrumChannel> m_uplinkChannel = CreateObject<SingleModelSpectrumChannel> ();
  Ptr<BuildingsPropagationLossModel> m_downlinkPropagationLossModel = CreateObject<BuildingsPropagationLossModel> ();
  Ptr<BuildingsPropagationLossModel>  m_uplinkPropagationLossModel = CreateObject<BuildingsPropagationLossModel> ();
  m_downlinkChannel->AddPropagationLossModel (m_downlinkPropagationLossModel);
  m_uplinkChannel->AddPropagationLossModel (m_uplinkPropagationLossModel);
  loss.push_back (m_downlinkPropagationLossModel->GetLoss (m_node1, m_node2) - m_lossRef);
  sum += loss.at (loss.size()-1);
  sumSquared += (loss.at (loss.size()-1)*loss.at (loss.size()-1));
  }
  double mean = sum/samples;
  double sigma = sqrt(sumSquared/samples - (mean*mean));
  // test whether the distribution falls in the 99% confidence interval, as expected with a nornal distribution
  double ci = (2.575829303549 * sigma) / sqrt (samples);
  
  NS_LOG_INFO ("Mean from simulation " << mean << ", sigma " << sigma << ", reference value " << m_sigmaRef << ", CI(99%) " << ci);
  
  NS_TEST_ASSERT_MSG_EQ_TOL(fabs(mean), 0.0, ci, "Wrong shadowing distribution !");
}
