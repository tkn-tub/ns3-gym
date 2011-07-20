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
// #include "ns3/network-module.h"
#include <ns3/mobility-helper.h>
//#include "ns3/lte-module.h"
#include <ns3/lena-helper.h>
#include <ns3/single-model-spectrum-channel.h>
#include "ns3/string.h"
#include "ns3/double.h"
#include <ns3/building.h>
#include <ns3/enum.h>

NS_LOG_COMPONENT_DEFINE ("LtePathlossModelTest");

using namespace ns3;


/**
 * Test 1.1 SINR calculation in downlink
 */

/**
 * TestSuite
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
  
//   lena->SetPropagationModelAttribute ("Environment", EnumValue (BuildingsPropagationLossModel::SubUrban));

  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer henbNodes;
  NodeContainer ueNodes;
  NodeContainer hueNodes;
  enbNodes.Create (1);
  henbNodes.Create (1);
  ueNodes.Create (3);
  hueNodes.Create (1);
  
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
  
  double distance = 2000;
  double hm = 1;
  double hb = 30;
  //double hb = 15;
//   double hr = 20;

  //   double freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  double freq = 869e6; // E_UTRA BAND #5 see table 5.5-1 of 36.101
  //double freq = 2.620e9; // E_UTRA BAND #7 see table 5.5-1 of 36.101

  Ptr<BuildingsMobilityModel> mm1 = enbNodes.Get (0)->GetObject<BuildingsMobilityModel> ();
  mm1->SetPosition (Vector (0.0, 0.0, hb));
  
//   Ptr<Building> building1 = Create<Building> (0.0, 10.0, 0.0, 10.0, 0.0, 20.0/*, 1, 1, 1*/);
//   mm1->SetIndoor (building1);
  //mm1->SetOutdoor ();
  Ptr<BuildingsMobilityModel> mm2 = ueNodes.Get (0)->GetObject<BuildingsMobilityModel> ();
  mm2->SetPosition (Vector (distance, 0.0, hm));
  Ptr<Building> building = Create<Building> (0.0, 10.0, 0.0, 10.0, 0.0, 20.0/*, 1, 1, 1*/);
//   mm2->SetOutdoor ();
//   mm2->SetIndoor (building1);
//   mm2->SetFloorNumber (3);
  
  // Test Okumura Hata Model (150 < freq < 1500 MHz)
  
  freq = 869e6; // E_UTRA BAND #5 see table 5.5-1 of 36.101
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 243.61, "OH Urban Large city = ??"));
  
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Small, 243.55, "OH Urban small city = ??"));
  
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::SubUrban, BuildingsPropagationLossModel::Large, 233.70, "loss OH SubUrban"));
  
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::OpenAreas, BuildingsPropagationLossModel::Large, 215.89, "loss OH OpenAreas"));
  
  // Test COST231 Model (1500 < freq < 2000~2170 MHz)
  
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 254.22, "COST231 Urban Large city"));
  
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Small, 256.32, "COST231 Urban small city and suburban"));
  
  // Test 2.6 GHz model
  
  freq = 2.620e9; // E_UTRA BAND #7 see table 5.5-1 of 36.101
  
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Small, 121.83, "2.6GHz model"));
  
  // Test ITU1411 LOS model
  
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  distance = 100;
  Ptr<BuildingsMobilityModel> mm3 = ueNodes.Get (1)->GetObject<BuildingsMobilityModel> ();
  mm3->SetPosition (Vector (distance, 0.0, hm));
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm3, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 80.605, "ITU1411 LOS"));
  
  // Test ITU1411 NLOS model
  
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  distance = 900;
  
  Ptr<BuildingsMobilityModel> mm4 = ueNodes.Get (2)->GetObject<BuildingsMobilityModel> ();
  mm4->SetPosition (Vector (distance, 0.0, hm));
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm4, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 143.69, "ITU1411 NLOS"));
  
  // Test ITUP1238
  
  distance = 30;
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  double henbHeight = 10.0;
  Ptr<BuildingsMobilityModel> mm5 = henbNodes.Get (0)->GetObject<BuildingsMobilityModel> ();
  mm5->SetPosition (Vector (0.0, 0.0, henbHeight));
  Ptr<Building> building1 = Create<Building> (0.0, 10.0, 0.0, 10.0, 0.0, 20.0/*, 1, 1, 1*/);
  mm5->SetIndoor (building1);
  Ptr<BuildingsMobilityModel> mm6 = hueNodes.Get (0)->GetObject<BuildingsMobilityModel> ();
  mm6->SetPosition (Vector (distance, 0.0, hm));
  mm6->SetIndoor (building1);
  mm6->SetFloorNumber (3);
  AddTestCase (new LtePathlossModelTestCase (freq, mm5, mm6, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 88.3855, "ITUP1238"));
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
  m_downlinkPropagationLossModel->SetAttribute ("Frequency", DoubleValue (m_freq)); // E_UTRA BAND #1 see table 5.5-1 of 36.101
  m_downlinkPropagationLossModel->SetAttribute ("Lambda", DoubleValue (300000000.0 /m_freq)); // E_UTRA BAND #1 see table 5.5-1 of 36.101
  m_downlinkPropagationLossModel->SetAttribute ("Environment", EnumValue (m_env));
  m_downlinkPropagationLossModel->SetAttribute ("CitySize", EnumValue (m_city));
  Ptr<BuildingsPropagationLossModel>  m_uplinkPropagationLossModel = CreateObject<BuildingsPropagationLossModel> ();
  m_uplinkPropagationLossModel->SetAttribute ("Frequency", DoubleValue (m_freq)); // E_UTRA BAND #1 see table 5.5-1 of 36.101
  m_uplinkPropagationLossModel->SetAttribute ("Lambda", DoubleValue (300000000.0 /m_freq)); // E_UTRA BAND #1 see table 5.5-1 of 36.101
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
