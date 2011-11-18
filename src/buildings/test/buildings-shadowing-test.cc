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
#include "ns3/buildings-shadowing-test.h"
#include <ns3/buildings-propagation-loss-model.h>
#include "ns3/string.h"
#include "ns3/double.h"
#include <ns3/building.h>
#include <ns3/enum.h>

NS_LOG_COMPONENT_DEFINE ("BuildingsShadowingTest");

using namespace ns3;


/**
* Test 1.1 Shadowing compound test
*/

/**
* This TestSuite tests the shadowing model of BuildingPathlossModel 
* by reproducing several communication scenarios 
*/


BuildingsShadowingTestSuite::BuildingsShadowingTestSuite ()
  : TestSuite ("buildings-shadowing-test", SYSTEM)
{

  LogComponentEnable ("BuildingsShadowingTest", LOG_LEVEL_ALL);

  // Test #1 Outdoor Model
  AddTestCase (new BuildingsShadowingTestCase (1, 2, 148.86, 7.0, "Outdoor Shadowing"));

  // Test #2 Indoor model
  AddTestCase (new BuildingsShadowingTestCase (5, 6, 88.5724, 8.0, "Indoor Shadowing"));

  // Test #3 Indoor -> Outdoor
  AddTestCase (new BuildingsShadowingTestCase (9, 10, 85.0012, 8.6, "Indoor -> Outdoor Shadowing"));

}

static BuildingsShadowingTestSuite buildingsShadowingTestSuite;


/**
* TestCase
*/

BuildingsShadowingTestCase::BuildingsShadowingTestCase ( uint16_t m1, uint16_t m2, double refValue, double sigmaRef, std::string name)
  : TestCase ("SHADOWING calculation: " + name),
    m_mobilityModelIndex1 (m1),
    m_mobilityModelIndex2 (m2),
    m_lossRef (refValue),
    m_sigmaRef (sigmaRef)
{
}

BuildingsShadowingTestCase::~BuildingsShadowingTestCase ()
{
}

void
BuildingsShadowingTestCase::DoRun (void)
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


  Ptr<MobilityModel> mma = CreateMobilityModel (m_mobilityModelIndex1);
  Ptr<MobilityModel> mmb = CreateMobilityModel (m_mobilityModelIndex2);

  std::vector<double> loss;
  double sum = 0.0;
  double sumSquared = 0.0;
  int samples = 10000;
  for (int i = 0; i < samples; i++)
    {
      Ptr<BuildingsPropagationLossModel> propagationLossModel = CreateObject<BuildingsPropagationLossModel> ();
      loss.push_back (propagationLossModel->GetLoss (mma, mmb) - m_lossRef);
      sum += loss.at (loss.size () - 1);
      sumSquared += (loss.at (loss.size () - 1) * loss.at (loss.size () - 1));
    }
  double mean = sum / samples;
  double sigma = sqrt (sumSquared / samples - (mean * mean));
  // test whether the distribution falls in the 99% confidence interval, as expected with a nornal distribution
  double ci = (2.575829303549 * sigma) / sqrt (samples);

  NS_LOG_INFO ("Mean from simulation " << mean << ", sigma " << sigma << ", reference value " << m_sigmaRef << ", CI(99%) " << ci);

  NS_TEST_ASSERT_MSG_EQ_TOL (fabs (mean), 0.0, ci, "Wrong shadowing distribution !");
}



Ptr<MobilityModel>
BuildingsShadowingTestCase::CreateMobilityModel (uint16_t index)
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


  distance = 30;
  double henbHeight = 10.0;
  Ptr<BuildingsMobilityModel> mm5 = CreateObject<BuildingsMobilityModel> ();
  mm5->SetPosition (Vector (0.0, 0.0, henbHeight));
  static Ptr<Building> building1 = Create<Building> (0.0, 10.0, 0.0, 10.0, 0.0, 20.0 /*, 1, 1, 1*/);
  building1->SetBuildingType (Building::Residential);
  building1->SetExtWallsType (Building::ConcreteWithWindows);
  mm5->SetIndoor (building1);
  Ptr<BuildingsMobilityModel> mm6 = CreateObject<BuildingsMobilityModel> ();
  mm6->SetPosition (Vector (distance, 0.0, hm));
  mm6->SetIndoor (building1);
  mm6->SetFloorNumber (2);


  distance = 100;
  Ptr<BuildingsMobilityModel> mm9 = CreateObject<BuildingsMobilityModel> ();
  mm9->SetPosition (Vector (0.0, 0.0, henbHeight));
  mm9->SetIndoor (building1);
  mm9->SetFloorNumber (2);
  Ptr<BuildingsMobilityModel> mm10 = CreateObject<BuildingsMobilityModel> ();
  mm10->SetPosition (Vector (distance, 0.0, hm));

  switch (index)
    {
    case 1:
      return mm1;
      break;

    case 2:
      return mm2;
      break;

    case 5:
      return mm5;
      break;

    case 6:
      return mm6;
      break;

    case 9:
      return mm9;
      break;

    case 10:
      return mm10;
      break;

    default:
      return 0;
      break;
    }
  return 0;

}
