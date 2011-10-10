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

#include <ns3/simulator.h>

#include <ns3/log.h>

#include <ns3/buildings-pathloss-test.h>
#include <ns3/spectrum-model.h>
#include <ns3/single-model-spectrum-channel.h>
#include "ns3/string.h"
#include "ns3/double.h"
#include <ns3/building.h>
#include <ns3/enum.h>


NS_LOG_COMPONENT_DEFINE ("BuildingsPathlossTest");

using namespace ns3;


/**
 * Test 1.1 BuildingsPathlossModel Pathloss compound test
 */

/**
 * This TestSuite tests the BuildingPathlossModel by reproducing
 * several communication scenarios 
 */


BuildingsPathlossTestSuite::BuildingsPathlossTestSuite ()
  : TestSuite ("buildings-pathloss-test", SYSTEM)
{

  LogComponentEnable ("BuildingsPathlossTest", LOG_LEVEL_ALL);
  
  double distance = 2000;
  double hm = 1;
  double hb = 30;
  double freq = 869e6; // E_UTRA BAND #5 see table 5.5-1 of 36.101
  Ptr<BuildingsMobilityModel> mm1 = CreateObject<BuildingsMobilityModel> ();
  mm1->SetPosition (Vector (0.0, 0.0, hb));
  
  Ptr<BuildingsMobilityModel> mm2 = CreateObject<BuildingsMobilityModel> ();
  mm2->SetPosition (Vector (distance, 0.0, hm));
  
  AddTestCase (new BuildingsPathlossTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 137.93, "OH Urban Large city"));
  
  AddTestCase (new BuildingsPathlossTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Small, 137.88, "OH Urban small city"));
  
  AddTestCase (new BuildingsPathlossTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::SubUrban, BuildingsPropagationLossModel::Large, 128.03, "loss OH SubUrban"));
  
  AddTestCase (new BuildingsPathlossTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::OpenAreas, BuildingsPropagationLossModel::Large, 110.21, "loss OH OpenAreas"));
  
  // Test #2 COST231 Model (1500 < freq < 2000~2170 MHz) (Macro<->UE)
  
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  
  AddTestCase (new BuildingsPathlossTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 148.55, "COST231 Urban Large city"));
  
  AddTestCase (new BuildingsPathlossTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Small, 150.64, "COST231 Urban small city and suburban"));
  
  // Test #3 2.6 GHz model (Macro<->UE)
  
  freq = 2.620e9; // E_UTRA BAND #7 see table 5.5-1 of 36.101
  
  AddTestCase (new BuildingsPathlossTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Small, 121.83, "2.6GHz model"));
  
  // Test #4 ITU1411 LOS model (Macro<->UE)
  
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  distance = 100;
  Ptr<BuildingsMobilityModel> mm3 = CreateObject<BuildingsMobilityModel> ();
  mm3->SetPosition (Vector (distance, 0.0, hm));
  AddTestCase (new BuildingsPathlossTestCase (freq, mm1, mm3, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 81.00, "ITU1411 LOS"));
  
  // Test #5 ITU1411 NLOS model (Macro<->UE)
  
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  distance = 900;
  
  Ptr<BuildingsMobilityModel> mm4 = CreateObject<BuildingsMobilityModel> ();
  mm4->SetPosition (Vector (distance, 0.0, hm));
  AddTestCase (new BuildingsPathlossTestCase (freq, mm1, mm4, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 143.69, "ITU1411 NLOS"));
  
  // Test #6 ITUP1238 (HeNB <-> UE)
  
  distance = 30;
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  double henbHeight = 10.0;
  Ptr<BuildingsMobilityModel> mm5 = CreateObject<BuildingsMobilityModel> ();
  mm5->SetPosition (Vector (0.0, 0.0, henbHeight));
  Ptr<Building> building1 = Create<Building> (0.0, 10.0, 0.0, 10.0, 0.0, 20.0/*, 1, 1, 1*/);
  building1->SetBuildingType (Building::Residential);
  building1->SetExtWallsType (Building::ConcreteWithWindows);
  mm5->SetIndoor (building1);
  Ptr<BuildingsMobilityModel> mm6 = CreateObject<BuildingsMobilityModel> ();
  mm6->SetPosition (Vector (distance, 0.0, hm));
  mm6->SetIndoor (building1);
  mm6->SetFloorNumber (2);
  AddTestCase (new BuildingsPathlossTestCase (freq, mm5, mm6, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 88.3855, "ITUP1238"));
  
  // Test #7 Outdoor -> Indoor OkumuraHata (Macro<->UE)
  
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  distance = 2000;
  // The loss is as in test #2 (large city) plus the building penetration loss
  // which for ConcreteWithWindows is equal to 7 dB -> 148.55 + 7 = 155.55
  Ptr<BuildingsMobilityModel> mm7 = CreateObject<BuildingsMobilityModel> ();
  mm7->SetPosition (Vector (distance, 0.0, hm));
  mm7->SetIndoor (building1);
  AddTestCase (new BuildingsPathlossTestCase (freq, mm1, mm7, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 155.55, "Okumura Hata Outdoor -> Indoor"));
  
  // Test #8 Outdoor -> Indoor ITU1411 (Macro<->UE)
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  distance = 100;
  Ptr<BuildingsMobilityModel> mm8 = CreateObject<BuildingsMobilityModel> ();
  mm8->SetPosition (Vector (distance, 0.0, hm));
  mm8->SetIndoor (building1);
  // The loss is as in test #4 plus the building penetration loss
  // which for ConcreteWithWindows is equal to 7 dB -> 81.000 + 7 = 88.000
  AddTestCase (new BuildingsPathlossTestCase (freq, mm1, mm8, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 88.000, "ITU1411 LOS Outdoor -> Indoor"));
  
  // Test #9 Indoor -> Outdoor LOS (HeNB <-> UE)
  
  distance = 100;
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  Ptr<BuildingsMobilityModel> mm9 = CreateObject<BuildingsMobilityModel> ();
  mm9->SetPosition (Vector (0.0, 0.0, henbHeight));
  mm9->SetIndoor (building1);
  mm9->SetFloorNumber (2);
  Ptr<BuildingsMobilityModel> mm10 = CreateObject<BuildingsMobilityModel> ();
  mm10->SetPosition (Vector (distance, 0.0, hm));
  // The loss is similar of test #4 plus the building penetration loss
  // which for ConcreteWithWindows is equal to 7 dB and the height gain
  // (2 floors x 2 dB/floor = 4) -> 81.838 + 7 - 4 = 84.838
  AddTestCase (new BuildingsPathlossTestCase (freq, mm9, mm10, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 84.838, "ITU1411 LOS Indoor -> Outdoor"));
  
  // Test #10 Indoor -> Outdoor NLOS (HeNB <-> UE)
  
  distance = 500;
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  Ptr<BuildingsMobilityModel> mm11 = CreateObject<BuildingsMobilityModel> ();
  mm11->SetPosition (Vector (distance, 0.0, hm));
  // The loss is similar as in test #4 plus the building penetration loss
  // which for ConcreteWithWindows is equal to 7 dB and the height gain
  // (2 floors x 2 dB/floor = 4) -> 180.90 + 7 - 4 = 183.90
  AddTestCase (new BuildingsPathlossTestCase (freq, mm9, mm11, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 183.90, "ITU1411 NLOS Indoor -> Outdoor"));
  

}



static BuildingsPathlossTestSuite buildingsPathlossTestSuite;


/**
 * TestCase
 */

BuildingsPathlossTestCase::BuildingsPathlossTestCase (double freq, Ptr<BuildingsMobilityModel> m1, Ptr<BuildingsMobilityModel> m2, BuildingsPropagationLossModel::Environment env, BuildingsPropagationLossModel::CitySize city, double refValue, std::string name)
  : TestCase ("LOSS calculation: " + name),
    m_freq (freq),
    m_node1 (m1),
    m_node2 (m2),
    m_env (env),
    m_city (city),
    m_lossRef (refValue)
{
}

BuildingsPathlossTestCase::~BuildingsPathlossTestCase ()
{
}

void
BuildingsPathlossTestCase::DoRun (void)
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
  NS_LOG_INFO ("Testing ");

  Ptr<SpectrumChannel> m_downlinkChannel = CreateObject<SingleModelSpectrumChannel> ();
  Ptr<SpectrumChannel> m_uplinkChannel = CreateObject<SingleModelSpectrumChannel> ();
  Ptr<BuildingsPropagationLossModel> m_downlinkPropagationLossModel = CreateObject<BuildingsPropagationLossModel> ();
  m_downlinkPropagationLossModel->SetAttribute ("Frequency", DoubleValue (m_freq));
  m_downlinkPropagationLossModel->SetAttribute ("Lambda", DoubleValue (300000000.0 /m_freq));
  m_downlinkPropagationLossModel->SetAttribute ("Environment", EnumValue (m_env));
  m_downlinkPropagationLossModel->SetAttribute ("CitySize", EnumValue (m_city));
  // cancel shadowing effect
  m_downlinkPropagationLossModel->SetAttribute ("ShadowSigmaOutdoor", DoubleValue (0.0));
  m_downlinkPropagationLossModel->SetAttribute ("ShadowSigmaIndoor", DoubleValue (0.0));
  m_downlinkPropagationLossModel->SetAttribute ("ShadowSigmaExtWalls", DoubleValue (0.0));
  Ptr<BuildingsPropagationLossModel>  m_uplinkPropagationLossModel = CreateObject<BuildingsPropagationLossModel> ();
  m_uplinkPropagationLossModel->SetAttribute ("Frequency", DoubleValue (m_freq));
  m_uplinkPropagationLossModel->SetAttribute ("Lambda", DoubleValue (300000000.0 /m_freq));
  // cancel shadowing effect
  m_uplinkPropagationLossModel->SetAttribute ("ShadowSigmaOutdoor", DoubleValue (0.0));
  m_uplinkPropagationLossModel->SetAttribute ("ShadowSigmaIndoor", DoubleValue (0.0));
  m_uplinkPropagationLossModel->SetAttribute ("ShadowSigmaExtWalls", DoubleValue (0.0));
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

