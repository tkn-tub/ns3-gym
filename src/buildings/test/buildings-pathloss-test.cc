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
#include "ns3/string.h"
#include "ns3/double.h"
#include <ns3/building.h>
#include <ns3/enum.h>


NS_LOG_COMPONENT_DEFINE ("BuildingsPathlossTest");

namespace ns3 {


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

  double freq = 869e6;  // E_UTRA BAND #5 see table 5.5-1 of 36.101

  AddTestCase (new BuildingsPathlossTestCase (freq, 1, 2, UrbanEnvironment, LargeCity, 137.93, "OH Urban Large city"));

  AddTestCase (new BuildingsPathlossTestCase (freq, 1, 2, UrbanEnvironment, SmallCity, 137.88, "OH Urban small city"));

  AddTestCase (new BuildingsPathlossTestCase (freq, 1, 2, SubUrbanEnvironment, LargeCity, 128.03, "loss OH SubUrban"));

  AddTestCase (new BuildingsPathlossTestCase (freq, 1, 2, OpenAreasEnvironment, LargeCity, 110.21, "loss OH OpenAreas"));

  // Test #2 COST231 Model (1500 < freq < 2000~2170 MHz) (Macro<->UE)

  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101

  AddTestCase (new BuildingsPathlossTestCase (freq, 1, 2, UrbanEnvironment, LargeCity, 148.55, "COST231 Urban Large city"));

  AddTestCase (new BuildingsPathlossTestCase (freq, 1, 2, UrbanEnvironment, SmallCity, 150.64, "COST231 Urban small city and suburban"));

  // Test #3 2.6 GHz model (Macro<->UE)

  freq = 2.620e9; // E_UTRA BAND #7 see table 5.5-1 of 36.101

  AddTestCase (new BuildingsPathlossTestCase (freq, 1, 2, UrbanEnvironment, SmallCity, 121.83, "2.6GHz model"));

  // Test #4 ITU1411 LOS model (Macro<->UE)

  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  AddTestCase (new BuildingsPathlossTestCase (freq, 1, 3, UrbanEnvironment, LargeCity, 81.00, "ITU1411 LOS"));

  // Test #5 ITU1411 NLOS model (Macro<->UE)

  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101

  AddTestCase (new BuildingsPathlossTestCase (freq, 1, 4, UrbanEnvironment, LargeCity, 143.69, "ITU1411 NLOS"));

  // Test #6 ITUP1238 (HeNB <-> UE)

  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  AddTestCase (new BuildingsPathlossTestCase (freq, 5, 6, UrbanEnvironment, LargeCity, 88.3855, "ITUP1238"));

  // Test #7 Outdoor -> Indoor OkumuraHata (Macro<->UE)

  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  // The loss is as in test #2 (large city) plus the building penetration loss
  // which for ConcreteWithWindows is equal to 7 dB -> 148.55 + 7 = 155.55
  AddTestCase (new BuildingsPathlossTestCase (freq, 1, 7, UrbanEnvironment, LargeCity, 155.55, "Okumura Hata Outdoor -> Indoor"));

  // Test #8 Outdoor -> Indoor ITU1411 (Macro<->UE)
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  // The loss is as in test #4 plus the building penetration loss
  // which for ConcreteWithWindows is equal to 7 dB -> 81.000 + 7 = 88.000
  AddTestCase (new BuildingsPathlossTestCase (freq, 1, 8, UrbanEnvironment, LargeCity, 88.000, "ITU1411 LOS Outdoor -> Indoor"));

  // Test #9 Indoor -> Outdoor LOS (HeNB <-> UE)

  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  // The loss is similar of test #4 plus the building penetration loss
  // which for ConcreteWithWindows is equal to 7 dB and the height gain
  // (2 floors x 2 dB/floor = 4) -> 81.838 + 7 - 4 = 84.838
  AddTestCase (new BuildingsPathlossTestCase (freq, 9, 10, UrbanEnvironment, LargeCity, 84.838, "ITU1411 LOS Indoor -> Outdoor"));

  // Test #10 Indoor -> Outdoor NLOS (HeNB <-> UE)

  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  // The loss is similar as in test #4 plus the building penetration loss
  // which for ConcreteWithWindows is equal to 7 dB and the height gain
  // (2 floors x 2 dB/floor = 4) -> 180.90 + 7 - 4 = 183.90
  AddTestCase (new BuildingsPathlossTestCase (freq, 9, 11, UrbanEnvironment, LargeCity, 183.90, "ITU1411 NLOS Indoor -> Outdoor"));


}



static BuildingsPathlossTestSuite buildingsPathlossTestSuite;


/**
 * TestCase
 */

BuildingsPathlossTestCase::BuildingsPathlossTestCase (double freq, uint16_t m1, uint16_t m2, EnvironmentType env, CitySize city, double refValue, std::string name)
  : TestCase ("LOSS calculation: " + name),
    m_freq (freq),
    m_mobilityModelIndex1 (m1),
    m_mobilityModelIndex2 (m2),
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
  LogComponentEnable ("HybridBuildingsPropagationLossModel", LOG_LEVEL_ALL);

  NS_LOG_FUNCTION (this);

  Ptr<MobilityModel> mma = CreateMobilityModel (m_mobilityModelIndex1);
  Ptr<MobilityModel> mmb = CreateMobilityModel (m_mobilityModelIndex2);




  Ptr<HybridBuildingsPropagationLossModel> propagationLossModel = CreateObject<HybridBuildingsPropagationLossModel> ();
  propagationLossModel->SetAttribute ("Frequency", DoubleValue (m_freq));
  propagationLossModel->SetAttribute ("Environment", EnumValue (m_env));
  propagationLossModel->SetAttribute ("CitySize", EnumValue (m_city));
  // cancel shadowing effect
  propagationLossModel->SetAttribute ("ShadowSigmaOutdoor", DoubleValue (0.0));
  propagationLossModel->SetAttribute ("ShadowSigmaIndoor", DoubleValue (0.0));
  propagationLossModel->SetAttribute ("ShadowSigmaExtWalls", DoubleValue (0.0));

  double loss = propagationLossModel->GetLoss (mma, mmb);

  NS_LOG_INFO ("Calculated loss: " << loss);
  NS_LOG_INFO ("Theoretical loss: " << m_lossRef);
 
  NS_TEST_ASSERT_MSG_EQ_TOL (loss, m_lossRef, 0.1, "Wrong loss !");
  Simulator::Destroy ();
}

Ptr<MobilityModel>
BuildingsPathlossTestCase::CreateMobilityModel (uint16_t index)
{

  /*
   * The purpose of this method is to defer the creation of the
   * MobilityModel instances to when DoRun() is called. In a previous
   * version, MobilityModel instances where created directly in the
   * constructor of the test suite, which caused subtle bugs due to
   * "static initialization order fiasco". An example of such a subtle
   * bug is that logging via NS_LOG failed for some modules.
   * 
   */


  double distance = 2000;
  double hm = 1;
  double hb = 30;

  Ptr<BuildingsMobilityModel> mm1 = CreateObject<BuildingsMobilityModel> ();
  mm1->SetPosition (Vector (0.0, 0.0, hb));

  Ptr<BuildingsMobilityModel> mm2 = CreateObject<BuildingsMobilityModel> ();
  mm2->SetPosition (Vector (distance, 0.0, hm));
 
  distance = 100;
  Ptr<BuildingsMobilityModel> mm3 = CreateObject<BuildingsMobilityModel> ();
  mm3->SetPosition (Vector (distance, 0.0, hm));

  distance = 900;
  Ptr<BuildingsMobilityModel> mm4 = CreateObject<BuildingsMobilityModel> ();
  mm4->SetPosition (Vector (distance, 0.0, hm));

  distance = 30;
  double henbHeight = 10.0;
  Ptr<BuildingsMobilityModel> mm5 = CreateObject<BuildingsMobilityModel> ();
  mm5->SetPosition (Vector (0.0, 0.0, henbHeight));

  // this needs to be static otherwise it will look like a different building every time
  static Ptr<Building> building1 = Create<Building> ();
  building1->SetBoundaries (Box (0.0, 10.0, 0.0, 10.0, 0.0, 20.0 /*, 1, 1, 1*/));
  building1->SetBuildingType (Building::Residential);
  building1->SetExtWallsType (Building::ConcreteWithWindows);
  mm5->SetIndoor (building1);
  Ptr<BuildingsMobilityModel> mm6 = CreateObject<BuildingsMobilityModel> ();
  mm6->SetPosition (Vector (distance, 0.0, hm));
  mm6->SetIndoor (building1);
  mm6->SetFloorNumber (2);
 
  distance = 2000;
  Ptr<BuildingsMobilityModel> mm7 = CreateObject<BuildingsMobilityModel> ();
  mm7->SetPosition (Vector (distance, 0.0, hm));
  mm7->SetIndoor (building1);

  distance = 100;
  Ptr<BuildingsMobilityModel> mm8 = CreateObject<BuildingsMobilityModel> ();
  mm8->SetPosition (Vector (distance, 0.0, hm));
  mm8->SetIndoor (building1);

  distance = 100;
  Ptr<BuildingsMobilityModel> mm9 = CreateObject<BuildingsMobilityModel> ();
  mm9->SetPosition (Vector (0.0, 0.0, henbHeight));
  mm9->SetIndoor (building1);
  mm9->SetFloorNumber (2);
  Ptr<BuildingsMobilityModel> mm10 = CreateObject<BuildingsMobilityModel> ();
  mm10->SetPosition (Vector (distance, 0.0, hm));

  distance = 500;
  Ptr<BuildingsMobilityModel> mm11 = CreateObject<BuildingsMobilityModel> ();
  mm11->SetPosition (Vector (distance, 0.0, hm));
 


  switch (index)
    {
    case 1:
      return mm1;
      break;

    case 2:
      return mm2;
      break;

    case 3:
      return mm3;
      break;

    case 4:
      return mm4;
      break;

    case 5:
      return mm5;
      break;

    case 6:
      return mm6;
      break;

    case 7:
      return mm7;
      break;

    case 8:
      return mm8;
      break;

    case 9:
      return mm9;
      break;

    case 10:
      return mm10;
      break;

    case 11:
      return mm11;
      break;

    default:
      return 0;
      break;
    }
  return 0;
}



} // namespace ns3
