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
#include "ns3/rlc-stats-calculator.h"
#include <ns3/buildings-mobility-model.h>
#include <ns3/buildings-propagation-loss-model.h>
#include "ns3/lte-test-phy-error-model.h"
#include <ns3/eps-bearer.h>
#include <ns3/node-container.h>
#include <ns3/mobility-helper.h>
#include <ns3/net-device-container.h>
#include <ns3/lte-ue-net-device.h>
#include <ns3/lte-enb-net-device.h>
#include <ns3/lte-ue-rrc.h>
#include <ns3/lena-helper.h>
#include <ns3/string.h>
#include <ns3/double.h>
#include <ns3/lte-enb-phy.h>
#include <ns3/lte-ue-phy.h>
#include <ns3/config.h>
#include <ns3/boolean.h>



NS_LOG_COMPONENT_DEFINE ("LenaTestPhyErrorModel");

using namespace ns3;


LenaTestPhyErrorModelrSuite::LenaTestPhyErrorModelrSuite ()
  : TestSuite ("lte-phy-error-model", SYSTEM)
{
  NS_LOG_INFO ("creating LenaTestPhyErrorModelTestCase");

  // MCS 2 TB size of 256 bits BER 0.19 SINR -2.21
   AddTestCase (new LenaPhyErrorModelTestCase (4, 898, 32, 0.19, 25));
// MCS 2 TB size of 328 bits BER 0.09 SINR -2.25
   AddTestCase (new LenaPhyErrorModelTestCase (3, 900, 41, 0.09, 18));
// MCS 2 TB size of 520 bits BER 0.123 SINR -2.61
   AddTestCase (new LenaPhyErrorModelTestCase (2, 920, 65, 0.123, 21));
// MCS 2 TB size of 1080 bits BER 0.097 SINR -2.79
   AddTestCase (new LenaPhyErrorModelTestCase (1, 930, 135, 0.097, 19));
  // MCS 12 TB size of 4776 bits  BER 0.017  SINR 6.22
   AddTestCase (new LenaPhyErrorModelTestCase (1, 538, 597, 0.017, 8));
// MCS 12 TB size of 1608 bits  BER 0.23  SINR 6.22
  AddTestCase (new LenaPhyErrorModelTestCase (3, 538, 201, 0.23, 26));
  // MCS 12 TB size of 376 bits  BER 0.72  SINR 6.22
   AddTestCase (new LenaPhyErrorModelTestCase (7,538, 47, 0.72, 28));
// MCS 14 TB size of 6248 bits (3136 x 2) BER 0.18 (0.096 x 2) SINR 5.53
   AddTestCase (new LenaPhyErrorModelTestCase (1, 500, 781, 0.18, 24));

 

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
  
   double ber = 0.01;
  Config::SetDefault ("ns3::LteAmc::Ber", DoubleValue (ber));
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

  Ptr<LenaHelper> lena = CreateObject<LenaHelper> ();
  
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
  lena->ActivateEpsBearer (ueDevs, bearer);
  

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

  Ptr<RlcStatsCalculator> rlcStats = lena->GetRlcStats ();
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
      double ber = 2.0 - ((double)dlDataRxed.at (i)/txed);
      double np = n-n*m_berRef;
      NS_LOG_INFO ("\tUser " << i << " imsi " << imsi << " bytes rxed " << (double)dlDataRxed.at (i) << " txed " << txed << " BER " << ber << " Err " << fabs (m_berRef - ber) << " lambda " << lambda << " np " << np << " difference " << abs(lambda - np) << " quantile " << m_bernQuantile);
      // the quantiles are evaluated offline according to a Bernoulli 
      // ditribution with n equal to the number of packet sent and p equal 
      // to the BER (see /reference/bernuolliDistribution.m for details)
      NS_TEST_ASSERT_MSG_EQ_TOL (lambda, np, m_bernQuantile, " Unexpected BER distribution!");
    }


  Simulator::Destroy ();
}

