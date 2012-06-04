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

#include <iostream>
#include <sstream>
#include <string>

#include <ns3/object.h>
#include <ns3/spectrum-interference.h>
#include <ns3/spectrum-error-model.h>
#include <ns3/log.h>
#include <ns3/test.h>
#include <ns3/simulator.h>
#include <ns3/packet.h>
#include <ns3/ptr.h>
#include <iostream>
#include "ns3/radio-bearer-stats-calculator.h"
#include <ns3/buildings-mobility-model.h>
#include <ns3/buildings-propagation-loss-model.h>
#include <ns3/eps-bearer.h>
#include <ns3/node-container.h>
#include <ns3/mobility-helper.h>
#include <ns3/net-device-container.h>
#include <ns3/lte-ue-net-device.h>
#include <ns3/lte-enb-net-device.h>
#include <ns3/lte-ue-rrc.h>
#include <ns3/lte-helper.h>
#include "ns3/string.h"
#include "ns3/double.h"
#include <ns3/lte-enb-phy.h>
#include <ns3/lte-ue-phy.h>
#include <ns3/boolean.h>
#include <ns3/rr-ff-mac-scheduler.h>
#include <ns3/pf-ff-mac-scheduler.h>
#include <ns3/pointer.h>
#include <ns3/enum.h>

#include "lte-test-mimo.h"


NS_LOG_COMPONENT_DEFINE ("LenaTestMimo");

namespace ns3 {


LenaTestMimoSuite::LenaTestMimoSuite ()
  : TestSuite ("lte-mimo", SYSTEM)
{
  NS_LOG_INFO ("creating LenaMimoTestCase");

  // RR DOWNLINK- DISTANCE 300
  // [0, 0.2] sec   TxMode 0: MCS 20 -> TB size 1191
  // [0.2, 0.3] sec TxMode 1: MCS 26 -> TB size 1836
  // [0.3, 0.4] sec TxMode 2: MCS 18 -> TB size  967 (x2 layers)
  // --> 
  std::vector<uint32_t> estThrDl;
  estThrDl.push_back (119100); // TTI 1 estimated throughput for TxMode 1
  estThrDl.push_back (183600); // TTI 2 estimated throughput for TxMode 2
  estThrDl.push_back (193400); // TTI 3 estimated throughput for TxMode 3
  AddTestCase (new LenaMimoTestCase(300, estThrDl, "ns3::RrFfMacScheduler"));
  AddTestCase (new LenaMimoTestCase(300, estThrDl, "ns3::PfFfMacScheduler"));
 

}

static LenaTestMimoSuite lenaTestMimoSuite;

std::string 
LenaMimoTestCase::BuildNameString (uint16_t dist)
{
  std::ostringstream oss;
  oss << " UE distance " << dist << " m";
  return oss.str ();
}

LenaMimoTestCase::LenaMimoTestCase (uint16_t dist, std::vector<uint32_t> estThrDl, std::string schedulerType)
  : TestCase (BuildNameString (dist)),              
    m_dist (dist),
    m_estThrDl (estThrDl),
    m_schedulerType (schedulerType)
{
}

LenaMimoTestCase::~LenaMimoTestCase ()
{
}

void
LenaMimoTestCase::DoRun (void)
{
//   Config::SetDefault ("ns3::LteSpectrumPhy::PemEnabled", BooleanValue (false));
  Config::SetDefault ("ns3::LteAmc::AmcModel", EnumValue (LteAmc::PiroEW2010));
  LogComponentDisableAll (LOG_LEVEL_ALL);
//   LogComponentEnable ("LteEnbRrc", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteUeRrc", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteEnbMac", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteUeMac", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteRlc", LOG_LEVEL_ALL);
// 
//   LogComponentEnable ("LtePhy", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteEnbPhy", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteUePhy", LOG_LEVEL_ALL);

//   LogComponentEnable ("LteSpectrumPhy", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteInterference", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteSinrChunkProcessor", LOG_LEVEL_ALL);
// 
//   LogComponentEnable ("LtePropagationLossModel", LOG_LEVEL_ALL);
//   LogComponentEnable ("LossModel", LOG_LEVEL_ALL);
//   LogComponentEnable ("ShadowingLossModel", LOG_LEVEL_ALL);
//   LogComponentEnable ("PenetrationLossModel", LOG_LEVEL_ALL);
//   LogComponentEnable ("MultipathLossModel", LOG_LEVEL_ALL);
//   LogComponentEnable ("PathLossModel", LOG_LEVEL_ALL);
// 
//   LogComponentEnable ("LteNetDevice", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteUeNetDevice", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteEnbNetDevice", LOG_LEVEL_ALL);

//     LogComponentEnable ("LteMiErrorModel", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteAmc", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteSpectrumPhy", LOG_LEVEL_ALL);

  LogComponentEnable ("LenaTestMimo", LOG_LEVEL_ALL);
//   LogComponentEnable ("BuildingsPropagationLossModel", LOG_LEVEL_ALL);
//   LogComponentEnable ("RrFfMacScheduler", LOG_LEVEL_ALL);
//   LogComponentEnable ("PfFfMacScheduler", LOG_LEVEL_ALL);


  /**
   * Initialize Simulation Scenario: 1 eNB and m_nUser UEs
   */


  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  
  
  lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::HybridBuildingsPropagationLossModel"));
  lteHelper->SetPathlossModelAttribute ("ShadowSigmaOutdoor", DoubleValue (0.0));
  lteHelper->SetPathlossModelAttribute ("ShadowSigmaIndoor", DoubleValue (0.0));
  lteHelper->SetPathlossModelAttribute ("ShadowSigmaExtWalls", DoubleValue (0.0));
  
//   lteHelper->EnableLogComponents ();

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

  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
  lteHelper->SetSchedulerType (m_schedulerType);
  enbDevs = lteHelper->InstallEnbDevice (enbNodes);
  ueDevs = lteHelper->InstallUeDevice (ueNodes);
  
  // Attach a UE to a eNB
  lteHelper->Attach (ueDevs, enbDevs.Get (0));

  // Activate an EPS bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lteHelper->ActivateEpsBearer (ueDevs, bearer, EpcTft::Default ());
  

  Ptr<LteEnbNetDevice> lteEnbDev = enbDevs.Get (0)->GetObject<LteEnbNetDevice> ();
  Ptr<LteEnbPhy> enbPhy = lteEnbDev->GetPhy ();
  enbPhy->SetAttribute ("TxPower", DoubleValue (46.0));
  enbPhy->SetAttribute ("NoiseFigure", DoubleValue (5.0));
  Ptr<BuildingsMobilityModel> mmenb = enbNodes.Get (0)->GetObject<BuildingsMobilityModel> ();
  mmenb->SetPosition (Vector (0.0, 0.0, 30.0));

  // Set UE's position and power
  Ptr<BuildingsMobilityModel> mmue = ueNodes.Get (0)->GetObject<BuildingsMobilityModel> ();
  mmue->SetPosition (Vector (m_dist, 0.0, 1.0));
  Ptr<LteUeNetDevice> lteUeDev = ueDevs.Get (0)->GetObject<LteUeNetDevice> ();
  Ptr<LteUePhy> uePhy = lteUeDev->GetPhy ();
  uePhy->SetAttribute ("TxPower", DoubleValue (23.0));
  uePhy->SetAttribute ("NoiseFigure", DoubleValue (9.0));
  

  lteHelper->EnableRlcTraces ();
  lteHelper->EnableMacTraces ();
  double simulationTime = 0.401;
  double tolerance = 0.1;
  
  uint8_t rnti = ueDevs.Get (0)->GetObject<LteUeNetDevice> ()->GetRrc ()->GetRnti ();
  Ptr<LteEnbNetDevice> enbNetDev = enbDevs.Get (0)->GetObject<LteEnbNetDevice> ();
  
  PointerValue ptrval;
  enbNetDev->GetAttribute ("FfMacScheduler", ptrval);
  Ptr<PfFfMacScheduler> pfsched;
  Ptr<RrFfMacScheduler> rrsched;
  if (m_schedulerType.compare ("ns3::RrFfMacScheduler") == 0)
    {
      rrsched = ptrval.Get<RrFfMacScheduler> ();
      if (rrsched == 0)
        {
          NS_FATAL_ERROR ("No RR Scheduler available");
        }
      
      Simulator::Schedule (Seconds (0.2), &RrFfMacScheduler::TransmissionModeConfigurationUpdate, rrsched, rnti, 1);
      Simulator::Schedule (Seconds (0.3), &RrFfMacScheduler::TransmissionModeConfigurationUpdate, rrsched, rnti, 2);
    }
  else if (m_schedulerType.compare ("ns3::PfFfMacScheduler") == 0)
    {
      pfsched = ptrval.Get<PfFfMacScheduler> ();
      if (pfsched == 0)
        {
          NS_FATAL_ERROR ("No Pf Scheduler available");
        }
      
      Simulator::Schedule (Seconds (0.2), &PfFfMacScheduler::TransmissionModeConfigurationUpdate, pfsched, rnti, 1);
      Simulator::Schedule (Seconds (0.3), &PfFfMacScheduler::TransmissionModeConfigurationUpdate, pfsched, rnti, 2);
    }
  else
    {
      NS_FATAL_ERROR ("Scheduler not supported by this test");
    }
    
  
  Ptr<RadioBearerStatsCalculator> rlcStats = lteHelper->GetRlcStats ();
  rlcStats->SetAttribute ("EpochDuration", TimeValue (Seconds (0.1)));


  /**
   * Check that the assignation is done in a RR fashion
   */
  NS_LOG_INFO (m_schedulerType << " MIMO test:");
  double sampleTime = 0.2;
  for (uint8_t j = 0; j < m_estThrDl.size (); j ++)
    {
      NS_LOG_INFO ("\t test with user at distance " << m_dist << " time " << sampleTime);
      // get the imsi
      uint64_t imsi = ueDevs.Get (0)->GetObject<LteUeNetDevice> ()->GetImsi ();
      // get the lcId
      uint8_t lcId = ueDevs.Get (0)->GetObject<LteUeNetDevice> ()->GetRrc ()->GetLcIdVector ().at (0);
      Time t = Seconds (sampleTime);
      Simulator::Schedule(t, &LenaMimoTestCase::GetRlcBufferSample, this, rlcStats, imsi, lcId);
      sampleTime += 0.1;
    }
  Simulator::Stop (Seconds (simulationTime));
  Simulator::Run ();
  Simulator::Destroy ();

  NS_LOG_INFO ("Check consistency");
    for (uint8_t i = 0; i < m_estThrDl.size (); i++)
      {
        NS_LOG_INFO ("\tTTI " << i + 1 << " bytes rxed " << (double)m_dlDataRxed.at (i) << " ref " << m_estThrDl.at (i));
        NS_TEST_ASSERT_MSG_EQ_TOL ((double)m_dlDataRxed.at (i) , m_estThrDl.at (i), m_estThrDl.at (i) * tolerance, " Unfair Throughput!");
      }

}


void
LenaMimoTestCase::GetRlcBufferSample (Ptr<RadioBearerStatsCalculator> rlcStats, uint64_t imsi, uint8_t lcId)
{
  m_dlDataRxed.push_back (rlcStats->GetDlRxData (imsi, lcId));
//   NS_LOG_INFO ("\t get bytes " << m_dlDataRxed.at (m_dlDataRxed.size () - 1));
}



} // namespace ns3

