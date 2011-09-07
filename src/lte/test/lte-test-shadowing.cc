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


void
LteTestShadowingDlSchedCallback (LteShadowingSystemTestCase *testcase, std::string path, uint32_t frameNo, uint32_t subframeNo, uint16_t rnti,                                uint8_t mcsTb1, uint16_t sizeTb1, uint8_t mcsTb2, uint16_t sizeTb2)
{
  testcase->DlScheduling (frameNo, subframeNo, rnti, mcsTb1, sizeTb1, mcsTb2, sizeTb2);
}



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
  
  
  //------------------- SYSTEM TEST ------------------------------
  
/*//   LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);
  // 
  //   LogComponentEnable ("LenaHelper", logLevel);
  LogComponentEnable ("LteShadowingTest", LOG_LEVEL_ALL);
  //   LogComponentEnable ("BuildingsPropagationLossModel", logLevel);
  //   LogComponentEnable ("LteInterference", logLevel);
  //   LogComponentEnable ("LteSpectrumValueHelper", logLevel);
  
  
  struct SnrEfficiencyMcs
  {
    double  snrDb;
    double  efficiency;
    int  mcsIndex;
  };
  
 
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
    AddTestCase (new LteShadowingSystemTestCase (name.str (),  sinrDb, dist[i], snrEfficiencyMcs[i].mcsIndex));
  }
  
  */
  
  
  
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
  /*m_downlinkPropagationLossModel->SetAttribute ("Frequency", DoubleValue (m_freq));
  m_downlinkPropagationLossModel->SetAttribute ("Lambda", DoubleValue (300000000.0 /m_freq));
  m_downlinkPropagationLossModel->SetAttribute ("Environment", EnumValue (m_env));
  m_downlinkPropagationLossModel->SetAttribute ("CitySize", EnumValue (m_city));
  */Ptr<BuildingsPropagationLossModel>  m_uplinkPropagationLossModel = CreateObject<BuildingsPropagationLossModel> ();
//   m_uplinkPropagationLossModel->SetAttribute ("Frequency", DoubleValue (m_freq));
//   m_uplinkPropagationLossModel->SetAttribute ("Lambda", DoubleValue (300000000.0 /m_freq));
  m_downlinkChannel->AddPropagationLossModel (m_downlinkPropagationLossModel);
  m_uplinkChannel->AddPropagationLossModel (m_uplinkPropagationLossModel);
  loss.push_back (m_downlinkPropagationLossModel->GetLoss (m_node1, m_node2) - m_lossRef);
//   NS_LOG_INFO (loss.at (loss.size()-1));
  sum += loss.at (loss.size()-1);
  sumSquared += (loss.at (loss.size()-1)*loss.at (loss.size()-1));
  }
  double mean = sum/samples;
  double sigma = sumSquared/samples - (mean*mean);
  NS_LOG_INFO ("Sigma from simulation " << sigma << ", reference value " << m_sigmaRef);
  
  NS_TEST_ASSERT_MSG_EQ_TOL(mean, 0.0, 0.1, "Wrong mean !");
  NS_TEST_ASSERT_MSG_EQ_TOL(sigma, m_sigmaRef, 0.2, "Wrong sigma !");
}


//-------------------- SYSTEM TEST ---------------------------------


LteShadowingSystemTestCase::LteShadowingSystemTestCase (std::string name, double snrDb, double dist, uint16_t mcsIndex)
: TestCase (name),
m_snrDb (snrDb),
m_distance (dist),
m_mcsIndex (mcsIndex)
{
  std::ostringstream sstream1, sstream2;
  sstream1 << " snr=" << snrDb 
  << " mcs=" << mcsIndex << " distance=" << dist;
  
  NS_LOG_INFO ("Creating LteShadowingSystemTestCase: " + sstream1.str ());
}

LteShadowingSystemTestCase::~LteShadowingSystemTestCase ()
{
}

void
LteShadowingSystemTestCase::DoRun (void)
{

//     LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);
//     LogComponentEnable ("LteAmc", LOG_LEVEL_ALL);
//     LogComponentEnable ("LtePhy", LOG_LEVEL_ALL);
//     LogComponentEnable ("LteEnbPhy", LOG_LEVEL_ALL);
//     LogComponentEnable ("LteUePhy", LOG_LEVEL_ALL);
//     LogComponentEnable ("SingleModelSpectrumChannel", LOG_LEVEL_ALL);
//     LogComponentEnable ("BuildingsPropagationLossModel", logLevel);
  
  LogComponentEnable ("BuildingsPropagationLossModel", LOG_LEVEL_ALL);
    
  Ptr<LenaHelper> lena = CreateObject<LenaHelper> ();
    lena->EnableLogComponents ();
  lena->EnableMacTraces ();
  lena->EnableRlcTraces ();
  lena->SetAttribute ("PropagationModel", StringValue ("ns3::BuildingsPropagationLossModel"));
  
  //Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes;
  enbNodes.Create (1);
  ueNodes.Create (1);
  NodeContainer allNodes = NodeContainer ( enbNodes, ueNodes );
  
//   Install Mobility Model
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::BuildingsMobilityModel");
  mobility.Install (allNodes);
  
//   Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
  lena->SetSchedulerType ("ns3::RrFfMacScheduler");
  enbDevs = lena->InstallEnbDevice (enbNodes);
  ueDevs = lena->InstallUeDevice (ueNodes);
  
  Ptr<BuildingsMobilityModel> mm_enb = enbNodes.Get (0)->GetObject<BuildingsMobilityModel> ();
  mm_enb->SetPosition (Vector (0.0, 0.0, 30.0));
  Ptr<BuildingsMobilityModel> mm_ue = ueNodes.Get (0)->GetObject<BuildingsMobilityModel> ();
  mm_ue->SetPosition (Vector (m_distance, 0.0, 1.0));
  
  NS_LOG_INFO (" DISTANCE " << mm_ue->GetDistanceFrom (mm_enb));
  
  Ptr<LteEnbNetDevice> lteEnbDev = enbDevs.Get (0)->GetObject<LteEnbNetDevice> ();
  Ptr<LteEnbPhy> enbPhy = lteEnbDev->GetPhy ();
  enbPhy->SetAttribute ("TxPower", DoubleValue (30.0));
  enbPhy->SetAttribute ("NoiseFigure", DoubleValue (5.0));
  
  Ptr<LteUeNetDevice> lteUeDev = ueDevs.Get (0)->GetObject<LteUeNetDevice> ();
  Ptr<LteUePhy> uePhy = lteUeDev->GetPhy ();
  uePhy->SetAttribute ("TxPower", DoubleValue (23.0));
  uePhy->SetAttribute ("NoiseFigure", DoubleValue (9.0));
  
//   Attach a UE to a eNB
  lena->Attach (ueDevs, enbDevs.Get (0));
  
//   Activate an EPS bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lena->ActivateEpsBearer (ueDevs, bearer);
  
//   Use testing chunk processor in the PHY layer
//   It will be used to test that the SNR is as intended
//   Ptr<LtePhy> uePhy = ueDevs.Get (0)->GetObject<LteUeNetDevice> ()->GetPhy ()->GetObject<LtePhy> ();
  Ptr<LteTestSinrChunkProcessor> testSinr = Create<LteTestSinrChunkProcessor> (uePhy);
  uePhy->GetDownlinkSpectrumPhy ()->AddSinrChunkProcessor (testSinr);
  
//     Config::Connect ("/NodeList/0/DeviceList/0/LteEnbMac/DlScheduling",
//                      MakeBoundCallback (&LteTestShadowingDlSchedCallback, this));
  
  Simulator::Stop (Seconds (0.005));
  Simulator::Run ();
  
  double calculatedSinrDb = 10.0 * log10 (testSinr->GetSinr ()[0]);
  NS_LOG_INFO ("Distance " << m_distance << " Calculated SINR " << calculatedSinrDb << " ref " << m_snrDb);
  Simulator::Destroy ();
  NS_TEST_ASSERT_MSG_EQ_TOL (calculatedSinrDb, m_snrDb, 0.001, "Wrong SINR !");
}


void
LteShadowingSystemTestCase::DlScheduling (uint32_t frameNo, uint32_t subframeNo, uint16_t rnti,
                                              uint8_t mcsTb1, uint16_t sizeTb1, uint8_t mcsTb2, uint16_t sizeTb2) 
{
  static bool firstTime = true;

  if ( firstTime )
    {
      firstTime = false;
      NS_LOG_INFO ("SNR\tRef_MCS\tCalc_MCS");
    }


  if ( (frameNo > 1) || (subframeNo > 4) )
    {
      NS_LOG_INFO (m_snrDb << "\t" << m_mcsIndex << "\t" << (uint16_t)mcsTb1);
      
      NS_TEST_ASSERT_MSG_EQ ((uint16_t)mcsTb1, m_mcsIndex, "Wrong MCS index");
    }

}

