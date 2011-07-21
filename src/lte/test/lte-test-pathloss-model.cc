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

#include "ns3/lte-test-pathloss-model.h"
#include <ns3/buildings-propagation-loss-model.h>
#include <ns3/node-container.h>
#include <ns3/mobility-helper.h>
#include <ns3/lena-helper.h>
#include <ns3/single-model-spectrum-channel.h>
#include "ns3/string.h"
#include "ns3/double.h"
#include <ns3/building.h>
#include <ns3/enum.h>

NS_LOG_COMPONENT_DEFINE ("LtePathlossModelTest");

using namespace ns3;


/**
 * Test 1.1 Pathloss compound test
 */

/**
 * This TestSuite tests the BuildingPathlossModel by reproducing
 * several communication scenarios 
 */

LtePathlossModelTestSuite::LtePathlossModelTestSuite ()
  : TestSuite ("lte-pathloss-model", SYSTEM)
{
//    LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);
// 
//   LogComponentEnable ("LenaHelper", logLevel);
//   LogComponentEnable ("LtePathlossModelTest", logLevel);
//   LogComponentEnable ("BuildingsPropagationLossModel", logLevel);

  LogComponentEnable ("LtePathlossModelTest", LOG_LEVEL_ALL);

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
  
// Test #1 Okumura Hata Model (150 < freq < 1500 MHz) (Macro<->UE)
  
  double distance = 2000;
  double hm = 1;
  double hb = 30;
  double freq = 869e6; // E_UTRA BAND #5 see table 5.5-1 of 36.101
  Ptr<BuildingsMobilityModel> mm1 = enbNodes.Get (0)->GetObject<BuildingsMobilityModel> ();
  mm1->SetPosition (Vector (0.0, 0.0, hb));
  
  Ptr<BuildingsMobilityModel> mm2 = ueNodes.Get (0)->GetObject<BuildingsMobilityModel> ();
  mm2->SetPosition (Vector (distance, 0.0, hm));
  
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 243.61, "OH Urban Large city = ??"));
  
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Small, 243.55, "OH Urban small city = ??"));
  
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::SubUrban, BuildingsPropagationLossModel::Large, 233.70, "loss OH SubUrban"));
  
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::OpenAreas, BuildingsPropagationLossModel::Large, 215.89, "loss OH OpenAreas"));
  
  // Test #2 COST231 Model (1500 < freq < 2000~2170 MHz) (Macro<->UE)
  
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 254.22, "COST231 Urban Large city"));
  
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Small, 256.32, "COST231 Urban small city and suburban"));
  
  // Test #3 2.6 GHz model (Macro<->UE)
  
  freq = 2.620e9; // E_UTRA BAND #7 see table 5.5-1 of 36.101
  
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Small, 121.83, "2.6GHz model"));
  
  // Test #4 ITU1411 LOS model (Macro<->UE)
  
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  distance = 100;
  Ptr<BuildingsMobilityModel> mm3 = ueNodes.Get (1)->GetObject<BuildingsMobilityModel> ();
  mm3->SetPosition (Vector (distance, 0.0, hm));
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm3, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 81.00, "ITU1411 LOS"));
  
  // Test #5 ITU1411 NLOS model (Macro<->UE)
  
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  distance = 900;
  
  Ptr<BuildingsMobilityModel> mm4 = ueNodes.Get (2)->GetObject<BuildingsMobilityModel> ();
  mm4->SetPosition (Vector (distance, 0.0, hm));
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm4, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 143.69, "ITU1411 NLOS"));
  
  // Test #6 ITUP1238 (HeNB <-> UE)
  
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
  AddTestCase (new LtePathlossModelTestCase (freq, mm5, mm6, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 88.3855, "ITUP1238"));
  
  // Test #7 Outdoor -> Indoor OkumuraHata (Macro<->UE)
  
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  distance = 2000;
  // The loss is as in test #2 (large city) plus the building penetration loss
  // which for ConcreteWithWindows is equal to 7 dB -> 254.22 + 7 = 261.22
  Ptr<BuildingsMobilityModel> mm7 = ueNodes.Get (3)->GetObject<BuildingsMobilityModel> ();
  mm7->SetPosition (Vector (distance, 0.0, hm));
  mm7->SetIndoor (building1);
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm7, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 261.22, "Okumura Hata Outdoor -> Indoor"));
  
  // Test #8 Outdoor -> Indoor ITU1411 (Macro<->UE)
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  distance = 100;
  Ptr<BuildingsMobilityModel> mm8 = ueNodes.Get (4)->GetObject<BuildingsMobilityModel> ();
  mm8->SetPosition (Vector (distance, 0.0, hm));
  mm8->SetIndoor (building1);
  // The loss is as in test #4 plus the building penetration loss
  // which for ConcreteWithWindows is equal to 7 dB -> 81.000 + 7 = 88.000
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm8, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 88.000, "ITU1411 LOS Outdoor -> Indoor"));
  
  // Test #9 Indoor -> Outdoor LOS (HeNB <-> UE)
  
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
  AddTestCase (new LtePathlossModelTestCase (freq, mm9, mm10, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 84.838, "ITU1411 LOS Indoor -> Outdoor"));
  
  // Test #10 Indoor -> Outdoor NLOS (HeNB <-> UE)
  
  distance = 500;
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  Ptr<BuildingsMobilityModel> mm11 = hueNodes.Get (2)->GetObject<BuildingsMobilityModel> ();
  mm11->SetPosition (Vector (distance, 0.0, hm));
  // The loss is as in test #4 plus the building penetration loss
  // which for ConcreteWithWindows is equal to 7 dB and the height gain
  // (2 floors x 2 dB/floor = 4) -> 180.90 + 7 - 4 = 183.90
  AddTestCase (new LtePathlossModelTestCase (freq, mm9, mm11, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 183.90, "ITU1411 NLOS Indoor -> Outdoor"));
  
}

static LtePathlossModelTestSuite ltePathlossModelTestSuite;


/**
 * TestCase
 */

LtePathlossModelTestCase::LtePathlossModelTestCase (double freq, Ptr<BuildingsMobilityModel> m1, Ptr<BuildingsMobilityModel> m2, BuildingsPropagationLossModel::Environment env, BuildingsPropagationLossModel::CitySize city, double refValue, std::string name)
  : TestCase ("LOSS calculation: " + name),
    m_freq (freq),
    m_node1 (m1),
    m_node2 (m2),
    m_env (env),
    m_city (city),
    m_lossRef (refValue)
{
}

LtePathlossModelTestCase::~LtePathlossModelTestCase ()
{
}

void
LtePathlossModelTestCase::DoRun (void)
{
//   LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);

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
//   LogComponentEnable ("ShadowingLossModel", logLevel);
//   LogComponentEnable ("PenetrationLossModel", logLevel);
//   LogComponentEnable ("MultipathLossModel", logLevel);
//   LogComponentEnable ("PathLossModel", logLevel);
// 
//   LogComponentEnable ("LteNetDevice", logLevel);
//   LogComponentEnable ("LteUeNetDevice", logLevel);
//   LogComponentEnable ("LteEnbNetDevice", logLevel);

  LogComponentEnable ("BuildingsPropagationLossModel", LOG_LEVEL_ALL);
  NS_LOG_INFO ("Testing " << GetName());

  Ptr<SpectrumChannel> m_downlinkChannel = CreateObject<SingleModelSpectrumChannel> ();
  Ptr<SpectrumChannel> m_uplinkChannel = CreateObject<SingleModelSpectrumChannel> ();
  Ptr<BuildingsPropagationLossModel> m_downlinkPropagationLossModel = CreateObject<BuildingsPropagationLossModel> ();
  m_downlinkPropagationLossModel->SetAttribute ("Frequency", DoubleValue (m_freq));
  m_downlinkPropagationLossModel->SetAttribute ("Lambda", DoubleValue (300000000.0 /m_freq));
  m_downlinkPropagationLossModel->SetAttribute ("Environment", EnumValue (m_env));
  m_downlinkPropagationLossModel->SetAttribute ("CitySize", EnumValue (m_city));
  Ptr<BuildingsPropagationLossModel>  m_uplinkPropagationLossModel = CreateObject<BuildingsPropagationLossModel> ();
  m_uplinkPropagationLossModel->SetAttribute ("Frequency", DoubleValue (m_freq));
  m_uplinkPropagationLossModel->SetAttribute ("Lambda", DoubleValue (300000000.0 /m_freq));
  m_downlinkChannel->AddPropagationLossModel (m_downlinkPropagationLossModel);
  m_uplinkChannel->AddPropagationLossModel (m_uplinkPropagationLossModel);
  

  
  Simulator::Stop (Seconds (0.1));
  Simulator::Run ();
  Simulator::Destroy ();
  double loss = m_downlinkPropagationLossModel->GetLoss (m_node1, m_node2);

  NS_LOG_INFO ("Calculated loss: " << loss);
  NS_LOG_INFO ("Theoretical loss: " << m_lossRef);
 
  NS_TEST_ASSERT_MSG_EQ_TOL(loss, m_lossRef, 0.1, "Wrong loss !");
}
