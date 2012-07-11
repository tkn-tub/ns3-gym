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

#include <ns3/object.h>
#include <ns3/spectrum-interference.h>
#include <ns3/spectrum-error-model.h>
#include <ns3/log.h>
#include <ns3/test.h>
#include <ns3/simulator.h>
#include <ns3/packet.h>
#include <ns3/ptr.h>
#include <iostream>
#include <ns3/radio-bearer-stats-calculator.h>
#include <ns3/buildings-mobility-model.h>
#include <ns3/hybrid-buildings-propagation-loss-model.h>
#include <ns3/eps-bearer.h>
#include <ns3/node-container.h>
#include <ns3/mobility-helper.h>
#include <ns3/net-device-container.h>
#include <ns3/lte-ue-net-device.h>
#include <ns3/lte-enb-net-device.h>
#include <ns3/lte-ue-rrc.h>
#include <ns3/lte-helper.h>
#include <ns3/string.h>
#include <ns3/double.h>
#include <ns3/lte-enb-phy.h>
#include <ns3/lte-ue-phy.h>
#include <ns3/config.h>
#include <ns3/boolean.h>
#include <ns3/enum.h>
#include <ns3/unused.h>

#include "lte-test-phy-error-model.h"

NS_LOG_COMPONENT_DEFINE ("LenaTestPhyErrorModel");

namespace ns3 {


LenaTestPhyErrorModelrSuite::LenaTestPhyErrorModelrSuite ()
  : TestSuite ("lte-phy-error-model", SYSTEM)
{
  NS_LOG_INFO ("creating LenaTestPhyErrorModelTestCase");

  // MCS 2 TB size of 256 bits BER 0.33 SINR -5.51
   AddTestCase (new LenaPhyErrorModelTestCase (4, 1800, 32, 0.35, 50));
// MCS 2 TB size of 528 bits BER 0.11 SINR -5.51
   AddTestCase (new LenaPhyErrorModelTestCase (2, 1800, 66, 0.11, 34));
// MCS 2 TB size of 1088 bits BER 0.02 SINR -5.51
  AddTestCase (new LenaPhyErrorModelTestCase (1, 1800, 136, 0.02, 16));
  // MCS 12 TB size of 4800 bits  BER 0.3  SINR 4.43
   AddTestCase (new LenaPhyErrorModelTestCase (1, 600, 600, 0.3, 48));
// MCS 12 TB size of 1632 bits  BER 0.55  SINR 4.43
  AddTestCase (new LenaPhyErrorModelTestCase (3, 600, 204, 0.55, 52));
// MCS 16 TB size of 7272 bits (3648 x 3584) BER 0.14 SINR 8.48
// BER 0.14 = 1 - ((1-0.075)*(1-0.075))
   AddTestCase (new LenaPhyErrorModelTestCase (1, 470, 781, 0.14, 29));

 

}

static LenaTestPhyErrorModelrSuite lenaTestPhyErrorModelrSuite;

std::string 
LenaPhyErrorModelTestCase::BuildNameString (uint16_t nUser, uint16_t dist)
{
  std::ostringstream oss;
  oss << nUser << " UEs, distance " << dist << " m";
  return oss.str ();
}

LenaPhyErrorModelTestCase::LenaPhyErrorModelTestCase (uint16_t nUser, uint16_t dist, uint16_t tbSize, double berRef, uint16_t bernQuantile)
  : TestCase (BuildNameString (nUser, dist)),              
    m_nUser (nUser),
    m_dist (dist),
    m_tbSize (tbSize),
    m_berRef (berRef),
    m_bernQuantile (bernQuantile)
{
}

LenaPhyErrorModelTestCase::~LenaPhyErrorModelTestCase ()
{
}

void
LenaPhyErrorModelTestCase::DoRun (void)
{
  
   double ber = 0.03;
  Config::SetDefault ("ns3::LteAmc::Ber", DoubleValue (ber));
  Config::SetDefault ("ns3::LteAmc::AmcModel", EnumValue (LteAmc::PiroEW2010));
  Config::SetDefault ("ns3::LteSpectrumPhy::PemEnabled", BooleanValue (true));
  
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

//   LogComponentEnable ("RrFfMacScheduler", LOG_LEVEL_ALL);
//   LogComponentEnable ("LenaHelper", LOG_LEVEL_ALL);
//   LogComponentEnable ("RlcStatsCalculator", LOG_LEVEL_ALL);


//   LogComponentEnable ("LteSpectrumPhy", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteEnbMac", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteEnbPhy", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteUePhy", LOG_LEVEL_ALL);
//   LogComponentEnable ("RrFfMacScheduler", LOG_LEVEL_ALL);
//   LogComponentEnable ("LenaHelper", LOG_LEVEL_ALL);
//   LogComponentEnable ("BuildingsPropagationLossModel", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteMiErrorModel", LOG_LEVEL_ALL);
//   LogComponentEnable ("LteAmc", LOG_LEVEL_ALL);
//   
  LogComponentDisableAll (LOG_LEVEL_ALL);
  
  LogComponentEnable ("LenaTestPhyErrorModel", LOG_LEVEL_ALL);


  /**
   * Initialize Simulation Scenario: 1 eNB and m_nUser UEs
   */

  Ptr<LteHelper> lena = CreateObject<LteHelper> ();
  
  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes;
  enbNodes.Create (1);
  ueNodes.Create (m_nUser);

  // Install Mobility Model
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::BuildingsMobilityModel");
  mobility.Install (enbNodes);
  mobility.SetMobilityModel ("ns3::BuildingsMobilityModel");
  mobility.Install (ueNodes);
  
  // remove random shadowing component
  lena->SetAttribute ("PathlossModel", StringValue ("ns3::HybridBuildingsPropagationLossModel"));
  lena->SetPathlossModelAttribute ("ShadowSigmaOutdoor", DoubleValue (0.0));
  lena->SetPathlossModelAttribute ("ShadowSigmaIndoor", DoubleValue (0.0));
  lena->SetPathlossModelAttribute ("ShadowSigmaExtWalls", DoubleValue (0.0));

  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
  lena->SetSchedulerType ("ns3::RrFfMacScheduler");
  
  enbDevs = lena->InstallEnbDevice (enbNodes);
  ueDevs = lena->InstallUeDevice (ueNodes);

  // Attach a UE to a eNB
  lena->Attach (ueDevs, enbDevs.Get (0));

  // Activate an EPS bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lena->ActivateEpsBearer (ueDevs, bearer, EpcTft::Default ());
  

  Ptr<LteEnbNetDevice> lteEnbDev = enbDevs.Get (0)->GetObject<LteEnbNetDevice> ();
  Ptr<LteEnbPhy> enbPhy = lteEnbDev->GetPhy ();
  enbPhy->SetAttribute ("TxPower", DoubleValue (43.0));
  enbPhy->SetAttribute ("NoiseFigure", DoubleValue (5.0));
  // place the HeNB over the default rooftop level (20 mt.)
  Ptr<BuildingsMobilityModel> mm = enbNodes.Get (0)->GetObject<BuildingsMobilityModel> ();
  mm->SetPosition (Vector (0.0, 0.0, 30.0));

  // Set UEs' position and power
  for (int i = 0; i < m_nUser; i++)
    {
      Ptr<BuildingsMobilityModel> mm = ueNodes.Get (i)->GetObject<BuildingsMobilityModel> ();
      mm->SetPosition (Vector (m_dist, 0.0, 1.0));
      Ptr<LteUeNetDevice> lteUeDev = ueDevs.Get (i)->GetObject<LteUeNetDevice> ();
      Ptr<LteUePhy> uePhy = lteUeDev->GetPhy ();
      uePhy->SetAttribute ("TxPower", DoubleValue (23.0));
      uePhy->SetAttribute ("NoiseFigure", DoubleValue (9.0));
    }
    
  lena->EnableRlcTraces ();
  double simulationTime = 1.000;
  Simulator::Stop (Seconds (simulationTime));

  Ptr<RadioBearerStatsCalculator> rlcStats = lena->GetRlcStats ();
  rlcStats->SetAttribute ("EpochDuration", TimeValue (Seconds (simulationTime)));


  Simulator::Run ();

  /**
   * Check that the assignation is done in a RR fashion
   */
  NS_LOG_INFO ("Test with " << m_nUser << " user(s) at distance " << m_dist << " expected BER " << m_berRef);
  std::vector <uint64_t> dlDataRxed;
  for (int i = 0; i < m_nUser; i++)
    {
      // get the imsi
      uint64_t imsi = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetImsi ();
      // get the lcId
      uint8_t lcId = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetRrc ()->GetLcIdVector ().at (0);
      dlDataRxed.push_back (rlcStats->GetDlRxData (imsi, lcId));
      double txed = rlcStats->GetDlTxData (imsi, lcId);
      int n = txed / m_tbSize;
      int lambda = (double)dlDataRxed.at (i) / m_tbSize;
      double ber = 1.0 - ((double)dlDataRxed.at (i)/txed);
      double np = n-n*m_berRef;
      NS_LOG_INFO ("\tUser " << i << " imsi " << imsi << " bytes rxed " << (double)dlDataRxed.at (i) << " txed " << txed 
        << " BER " << ber << " Err " << fabs (m_berRef - ber) << " lambda " << lambda 
        << " np " << np << " difference " << abs(lambda - np) << " quantile " << m_bernQuantile);
      NS_UNUSED (ber);
      // the quantiles are evaluated offline according to a Bernoulli 
      // ditribution with n equal to the number of packet sent and p equal 
      // to the BER (see /reference/bernuolliDistribution.m for details)
      NS_TEST_ASSERT_MSG_EQ_TOL (lambda, np, m_bernQuantile, " Unexpected BER distribution!");
    }


  Simulator::Destroy ();
}

} // namespace ns3


