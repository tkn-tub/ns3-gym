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
#include <ns3/net-device-container.h>
#include <ns3/lte-ue-net-device.h>
#include <ns3/lte-enb-net-device.h>
#include <ns3/lte-ue-rrc.h>
#include <ns3/lena-helper.h>
#include <ns3/lte-enb-phy.h>
#include <ns3/lte-ue-phy.h>
#include "lte-test-sinr-chunk-processor.h"

NS_LOG_COMPONENT_DEFINE ("LtePathlossModelTest");

using namespace ns3;


/**
 * Test 1.1 Pathloss compound test
 */

/**
 * This TestSuite tests the BuildingPathlossModel by reproducing
 * several communication scenarios 
 */


void
LteTestPathlossDlSchedCallback (LtePathlossModelSystemTestCase *testcase, std::string path,
                             uint32_t frameNo, uint32_t subframeNo, uint16_t rnti,
                             uint8_t mcsTb1, uint16_t sizeTb1, uint8_t mcsTb2, uint16_t sizeTb2)
{
  testcase->DlScheduling (frameNo, subframeNo, rnti, mcsTb1, sizeTb1, mcsTb2, sizeTb2);
}



LtePathlossModelTestSuite::LtePathlossModelTestSuite ()
  : TestSuite ("lte-pathloss-model", SYSTEM)
{
 
  
// -------------- COMPOUND TESTS ----------------------------------

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
  
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 137.93, "OH Urban Large city = ??"));
  
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Small, 137.88, "OH Urban small city = ??"));
  
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::SubUrban, BuildingsPropagationLossModel::Large, 128.03, "loss OH SubUrban"));
  
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::OpenAreas, BuildingsPropagationLossModel::Large, 110.21, "loss OH OpenAreas"));
  
  // Test #2 COST231 Model (1500 < freq < 2000~2170 MHz) (Macro<->UE)
  
  freq = 2.1140e9; // E_UTRA BAND #1 see table 5.5-1 of 36.101
  
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 148.55, "COST231 Urban Large city"));
  
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm2, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Small, 150.64, "COST231 Urban small city and suburban"));
  
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
  // which for ConcreteWithWindows is equal to 7 dB -> 148.55 + 7 = 155.55
  Ptr<BuildingsMobilityModel> mm7 = ueNodes.Get (3)->GetObject<BuildingsMobilityModel> ();
  mm7->SetPosition (Vector (distance, 0.0, hm));
  mm7->SetIndoor (building1);
  AddTestCase (new LtePathlossModelTestCase (freq, mm1, mm7, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 155.55, "Okumura Hata Outdoor -> Indoor"));
  
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
  // The loss is similar as in test #4 plus the building penetration loss
  // which for ConcreteWithWindows is equal to 7 dB and the height gain
  // (2 floors x 2 dB/floor = 4) -> 180.90 + 7 - 4 = 183.90
  AddTestCase (new LtePathlossModelTestCase (freq, mm9, mm11, BuildingsPropagationLossModel::Urban, BuildingsPropagationLossModel::Large, 183.90, "ITU1411 NLOS Indoor -> Outdoor"));
  
  
//------------------- SYSTEM TEST ------------------------------

LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);
// 
//   LogComponentEnable ("LenaHelper", logLevel);
LogComponentEnable ("LtePathlossModelTest", logLevel);
//   LogComponentEnable ("BuildingsPropagationLossModel", logLevel);
//   LogComponentEnable ("LteInterference", logLevel);
//   LogComponentEnable ("LteSpectrumValueHelper", logLevel);


struct SnrEfficiencyMcs
{
  double  snrDb;
  double  efficiency;
  int  mcsIndex;
};

/**
* Test vectors: SNRDB, Spectral Efficiency, MCS index
* From XXX
*/
SnrEfficiencyMcs snrEfficiencyMcs[] = {
  { -5.00000,  0.08024,        -1},
  { -4.00000,  0.10030,        -1},
  { -3.00000,  0.12518,        -1},
  { -2.00000,  0.15589,        0},
  { -1.00000,  0.19365,        0},
  { 0.00000,   0.23983,        2},
  { 1.00000,   0.29593,        2},
  { 2.00000,   0.36360,        2},
  { 3.00000,   0.44451,        4},
  { 4.00000,   0.54031,        4},
  { 5.00000,   0.65251,        6},
  { 6.00000,   0.78240,        6},
  { 7.00000,   0.93086,        8},
  { 8.00000,   1.09835,        8},
  { 9.00000,   1.28485,        10},
  { 10.00000,  1.48981,        12},
  { 11.00000,  1.71229,        12},
  { 12.00000,  1.95096,        14},
  { 13.00000,  2.20429,        14},
  { 14.00000,  2.47062,        16},
  { 15.00000,  2.74826,        18},
  { 16.00000,  3.03560,        18},
  { 17.00000,  3.33115,        20},
  { 18.00000,  3.63355,        20},
  { 19.00000,  3.94163,        22},
  { 20.00000,  4.25439,        22},
  { 21.00000,  4.57095,        24},
  { 22.00000,  4.89060,        24},
  { 23.00000,  5.21276,        26},
  { 24.00000,  5.53693,        26},
  { 25.00000,  5.86271,        28},
  { 26.00000,  6.18980,        28},
  { 27.00000,  6.51792,        28},
  { 28.00000,  6.84687,        28},
  { 29.00000,  7.17649,        28},
  { 30.00000,  7.50663,        28},
};


double txPowerDbm = 30; // default eNB TX power over whole bandwdith
double txPowerLin = pow (10, (txPowerDbm - 30)/10);
double ktDbm = -174;    // reference LTE noise PSD
double noisePowerDbm = ktDbm + 10 * log10 (25 * 180000); // corresponds to kT*bandwidth in linear units
double receiverNoiseFigureDb = 9.0; // default UE noise figure
double noiseLin = pow (10, (noisePowerDbm-30+receiverNoiseFigureDb)/10);
double loss[] = {81.0000, 134.0125, 144.1489};
double dist[] = {100.0, 500.0, 1500};

int numOfTests = sizeof (loss) / sizeof (double);
for ( int i = 0 ; i < numOfTests; i++ )
{
  //     double lossDb = txPowerDbm - snrEfficiencyMcs[i].snrDb - noisePowerDbm - receiverNoiseFigureDb;
  double sinrLin = (txPowerLin*(pow(10, loss[i]/10))) / noiseLin;
  //     double sinrDb = txPowerDbm- noisePowerDbm - receiverNoiseFigureDb - loss[i];
  double sinrDb = 10*log10(sinrLin);
  NS_LOG_INFO (" Ptx " << txPowerDbm << " Pn " << noisePowerDbm << " Fn " << receiverNoiseFigureDb << " Pl " << loss[i] << " dist " << dist[i]);
  std::ostringstream name;
  name << " snr= " << sinrDb << " dB, "
  << " mcs= " << snrEfficiencyMcs[i].mcsIndex;
  AddTestCase (new LtePathlossModelSystemTestCase (name.str (),  sinrDb, dist[i], snrEfficiencyMcs[i].mcsIndex));
}




  
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


//-------------------- SYSTEM TEST ---------------------------------


LtePathlossModelSystemTestCase::LtePathlossModelSystemTestCase (std::string name, double snrDb, double dist, uint16_t mcsIndex)
: TestCase (name),
m_snrDb (snrDb),
m_distance (dist),
m_mcsIndex (mcsIndex)
{
  std::ostringstream sstream1, sstream2;
  sstream1 << " snr=" << snrDb 
  << " mcs=" << mcsIndex << " distance=" << dist;
  
  NS_LOG_INFO ("Creating LtePathlossModelSystemTestCase: " + sstream1.str ());
}

LtePathlossModelSystemTestCase::~LtePathlossModelSystemTestCase ()
{
}

void
LtePathlossModelSystemTestCase::DoRun (void)
{
  /**
  * Simulation Topology
  */
//   LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);
//   LogComponentEnable ("LteAmc", LOG_LEVEL_ALL);
//   LogComponentEnable ("LtePhy", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteEnbPhy", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteUePhy", LOG_LEVEL_ALL);
//   LogComponentEnable ("SingleModelSpectrumChannel", LOG_LEVEL_ALL);
  LogComponentEnable ("BuildingsPropagationLossModel", LOG_LEVEL_ALL);
  LogComponentEnable ("LenaHelper", LOG_LEVEL_ALL);
//   LogComponentDisable ("BuildingsPropagationLossModel", LOG_LEVEL_ALL);
//   
  Ptr<LenaHelper> lena = CreateObject<LenaHelper> ();
  //   lena->EnableLogComponents ();
  lena->EnableMacTraces ();
  lena->EnableRlcTraces ();
  lena->SetAttribute ("PropagationModel", StringValue ("ns3::BuildingsPropagationLossModel"));
  // remove shadowing component
  lena->SetPropagationModelAttribute ("ShadowSigmaOutdoor", DoubleValue (0.0));
  lena->SetPropagationModelAttribute ("ShadowSigmaIndoor", DoubleValue (0.0));
  lena->SetPropagationModelAttribute ("ShadowSigmaExtWalls", DoubleValue (0.0));
  
  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes;
  enbNodes.Create (1);
  ueNodes.Create (1);
  NodeContainer allNodes = NodeContainer ( enbNodes, ueNodes );
  
  // Install Mobility Model
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::BuildingsMobilityModel");
  mobility.Install (allNodes);
  
  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
  lena->SetSchedulerType ("ns3::RrFfMacScheduler");
  enbDevs = lena->InstallEnbDevice (enbNodes);
  ueDevs = lena->InstallUeDevice (ueNodes);
  
  Ptr<BuildingsMobilityModel> mm_enb = enbNodes.Get (0)->GetObject<BuildingsMobilityModel> ();
  mm_enb->SetPosition (Vector (0.0, 0.0, 30.0));
  Ptr<BuildingsMobilityModel> mm_ue = ueNodes.Get (0)->GetObject<BuildingsMobilityModel> ();
  mm_ue->SetPosition (Vector (m_distance, 0.0, 1.0));
  
  Ptr<LteEnbNetDevice> lteEnbDev = enbDevs.Get (0)->GetObject<LteEnbNetDevice> ();
  Ptr<LteEnbPhy> enbPhy = lteEnbDev->GetPhy ();
  enbPhy->SetAttribute ("TxPower", DoubleValue (30.0));
  enbPhy->SetAttribute ("NoiseFigure", DoubleValue (5.0));
  
  Ptr<LteUeNetDevice> lteUeDev = ueDevs.Get (0)->GetObject<LteUeNetDevice> ();
  Ptr<LteUePhy> uePhy = lteUeDev->GetPhy ();
  uePhy->SetAttribute ("TxPower", DoubleValue (23.0));
  uePhy->SetAttribute ("NoiseFigure", DoubleValue (9.0));
  
  
  // Attach a UE to a eNB
  lena->Attach (ueDevs, enbDevs.Get (0));
  
  // Activate an EPS bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lena->ActivateEpsBearer (ueDevs, bearer);
  
  // Use testing chunk processor in the PHY layer
  // It will be used to test that the SNR is as intended
  //Ptr<LtePhy> uePhy = ueDevs.Get (0)->GetObject<LteUeNetDevice> ()->GetPhy ()->GetObject<LtePhy> ();
  Ptr<LteTestSinrChunkProcessor> testSinr = Create<LteTestSinrChunkProcessor> (uePhy);
  uePhy->GetDownlinkSpectrumPhy ()->AddSinrChunkProcessor (testSinr);
   
//   Config::Connect ("/NodeList/0/DeviceList/0/LteEnbMac/DlScheduling",
//                    MakeBoundCallback (&LteTestPathlossDlSchedCallback, this));
                   
  Simulator::Stop (Seconds (0.005));
  Simulator::Run ();
  
  double calculatedSinrDb = 10.0 * log10 (testSinr->GetSinr ()[0]);
  NS_LOG_INFO ("Distance " << m_distance << " Calculated SINR " << calculatedSinrDb << " ref " << m_snrDb);
  Simulator::Destroy ();
  NS_TEST_ASSERT_MSG_EQ_TOL (calculatedSinrDb, m_snrDb, 0.001, "Wrong SINR !");
}


void
LtePathlossModelSystemTestCase::DlScheduling (uint32_t frameNo, uint32_t subframeNo, uint16_t rnti,
                                         uint8_t mcsTb1, uint16_t sizeTb1, uint8_t mcsTb2, uint16_t sizeTb2) 
{
  static bool firstTime = true;
  
  if ( firstTime )
  {
    firstTime = false;
    NS_LOG_INFO ("SNR\tRef_MCS\tCalc_MCS");
  }
  
  /**
  * Note:
  *    For first 4 subframeNo in the first frameNo, the MCS cannot be properly evaluated,
  *    because CQI feedback is still not available at the eNB.
  */
  if ( (frameNo > 1) || (subframeNo > 4) )
  {
    NS_LOG_INFO (m_snrDb << "\t" << m_mcsIndex << "\t" << (uint16_t)mcsTb1);
    
    NS_TEST_ASSERT_MSG_EQ ((uint16_t)mcsTb1, m_mcsIndex, "Wrong MCS index");
  }
}
                                         