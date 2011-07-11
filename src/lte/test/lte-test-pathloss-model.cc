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
  // LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);

  // LogComponentEnable ("LteTestUePhy", logLevel);
  // LogComponentEnable ("LteDownlinkSinrTest", logLevel);

  // NS_LOG_INFO ("Creating LteDownlinkSinrTestSuite");
  
  Ptr<LenaHelper> lena = CreateObject<LenaHelper> ();
  
  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes;
  enbNodes.Create (1);
  ueNodes.Create (1);
  
  // Install Mobility Model
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::BuildingsMobilityModel");
  mobility.Install (enbNodes);
  mobility.SetMobilityModel ("ns3::BuildingsMobilityModel");
  mobility.Install (ueNodes);
  
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
  enbDevs = lena->InstallEnbDevice (enbNodes);
  ueDevs = lena->InstallUeDevice (ueNodes);
  
  lena->Attach (ueDevs, enbDevs.Get (0));
  
  
  Ptr<BuildingsMobilityModel> mm1 = enbNodes.Get (0)->GetObject<BuildingsMobilityModel> ();
  mm1->SetPosition (Vector (0.0, 0.0, 0.0));
  mm1->SetOutdoor ();
  Ptr<BuildingsMobilityModel> mm2 = ueNodes.Get (0)->GetObject<BuildingsMobilityModel> ();
  mm2->SetPosition (Vector (10.0, 0.0, 0.0));
  mm2->SetOutdoor ();
         
  double thrLoss = 0.0;
  AddTestCase (new LtePathlossModelTestCase (mm1, mm2, thrLoss, "loss = ??"));

  /**
   * TX signal #2: Power Spectral Density (W/Hz) of the signal of interest = [-63 -61] dBm and BW = [20 22] MHz
   */
//   Ptr<SpectrumValue> rxPsd2 = Create<SpectrumValue> (sm);
//   (*rxPsd2)[0] = 2.505936168136e-17;
//   (*rxPsd2)[1] = 3.610582885110e-17;
// 
//   Ptr<SpectrumValue> theoreticalSinr2 = Create<SpectrumValue> (sm);
//   (*theoreticalSinr2)[0] = 0.0743413124381667;
//   (*theoreticalSinr2)[1] = 0.1865697965291756;
// 
//   AddTestCase (new LtePathlossModelTestCase (rxPsd2, theoreticalSinr2, "sdBm = [-63 -61]"));

}

static LtePathlossModelTestSuite ltePathlossModelTestSuite;


/**
 * TestCase
 */

LtePathlossModelTestCase::LtePathlossModelTestCase (Ptr<BuildingsMobilityModel> m1, Ptr<BuildingsMobilityModel> m2, double refValue, std::string name)
  : TestCase ("LOSS calculation: " + name),
    m_node1 (m1),
    m_node2 (m2),
    m_lossRef (refValue)
{
  NS_LOG_INFO ("Creating LtePathlossModelTestCase");
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
  LogComponentEnable ("BuildingsPropagationLossModel", LOG_LEVEL_ALL);
  
  LogComponentEnable ("LtePathlossModelTest", LOG_LEVEL_ALL);


  Ptr<SpectrumChannel> m_downlinkChannel = CreateObject<SingleModelSpectrumChannel> ();
  Ptr<SpectrumChannel> m_uplinkChannel = CreateObject<SingleModelSpectrumChannel> ();
  Ptr<BuildingsPropagationLossModel> m_downlinkPropagationLossModel = CreateObject<BuildingsPropagationLossModel> ();
  m_downlinkPropagationLossModel->SetAttribute ("Frequency", DoubleValue (2.1140e9)); // E_UTRA BAND #1 see table 5.5-1 of 36.101
  m_downlinkPropagationLossModel->SetAttribute ("Lambda", DoubleValue (300000000.0 /2.1140e9)); // E_UTRA BAND #1 see table 5.5-1 of 36.101
  Ptr<BuildingsPropagationLossModel>  m_uplinkPropagationLossModel = CreateObject<BuildingsPropagationLossModel> ();
  m_uplinkPropagationLossModel->SetAttribute ("Frequency", DoubleValue (1.950e9)); // E_UTRA BAND #1 see table 5.5-1 of 36.101
  m_downlinkPropagationLossModel->SetAttribute ("Lambda", DoubleValue (300000000.0 /1.950e9)); // E_UTRA BAND #1 see table 5.5-1 of 36.101
  m_downlinkChannel->AddPropagationLossModel (m_downlinkPropagationLossModel);
  m_uplinkChannel->AddPropagationLossModel (m_uplinkPropagationLossModel);
  

  
  Simulator::Stop (Seconds (0.1));
  Simulator::Run ();
  Simulator::Destroy ();

  double loss = m_downlinkPropagationLossModel->GetLoss (m_node1, m_node2);

  NS_LOG_INFO ("Calculated loss: " << loss);
  NS_LOG_INFO ("Theoretical loss: " << m_lossRef);
 
  NS_TEST_ASSERT_MSG_EQ_TOL(loss, m_lossRef, 0.0000001, "Wrong loss !");
}
