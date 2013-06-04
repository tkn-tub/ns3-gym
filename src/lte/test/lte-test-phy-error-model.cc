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
#include <ns3/mobility-building-info.h>
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
#include <ns3/ff-mac-scheduler.h>
#include <ns3/buildings-helper.h>

#include "lte-test-phy-error-model.h"

NS_LOG_COMPONENT_DEFINE ("LenaTestPhyErrorModel");

namespace ns3 {


LenaTestPhyErrorModelrSuite::LenaTestPhyErrorModelrSuite ()
  : TestSuite ("lte-phy-error-model", SYSTEM)
{
  NS_LOG_INFO ("creating LenaTestPhyErrorModelTestCase");
  
  
  // Tests on DL Control Channels (PCFICH+PDDCH)
  // 1 interfering eNB SINR -2.0 BER 0.007 TB size 217
  AddTestCase (new LenaDlCtrlPhyErrorModelTestCase (2, 1078, 217, 0.007), TestCase::QUICK);
  // 2 interfering eNBs SINR -4.0 BER 0.037 TB size 217
  AddTestCase (new LenaDlCtrlPhyErrorModelTestCase (3, 1040, 217, 0.045), TestCase::EXTENSIVE);
  // 3 interfering eNBs SINR -6.0 BER 0.21 TB size 133
  AddTestCase (new LenaDlCtrlPhyErrorModelTestCase (4, 1250, 133, 0.206), TestCase::EXTENSIVE);
  // 4 interfering eNBs SINR -7.0 BER 0.34 TB size 133
  AddTestCase (new LenaDlCtrlPhyErrorModelTestCase (5, 1260, 81, 0.343), TestCase::EXTENSIVE);

  // Tests on DL/UL Data channels (PDSCH, PUSCH)
  // MCS 2 TB size of 256 bits BER 0.33 SINR -5.51
  AddTestCase (new LenaDataPhyErrorModelTestCase (4, 1800, 32, 0.33, 50), TestCase::QUICK);
// MCS 2 TB size of 528 bits BER 0.11 SINR -5.51
  AddTestCase (new LenaDataPhyErrorModelTestCase (2, 1800, 66, 0.11, 34), TestCase::EXTENSIVE);
// MCS 2 TB size of 1088 bits BER 0.02 SINR -5.51
  AddTestCase (new LenaDataPhyErrorModelTestCase (1, 1800, 136, 0.02, 16), TestCase::EXTENSIVE);
  // MCS 12 TB size of 4800 bits  BER 0.3  SINR 4.43
  AddTestCase (new LenaDataPhyErrorModelTestCase (1, 600, 600, 0.3, 48), TestCase::EXTENSIVE);
// MCS 12 TB size of 1632 bits  BER 0.55  SINR 4.43
  AddTestCase (new LenaDataPhyErrorModelTestCase (3, 600, 204, 0.55, 52), TestCase::EXTENSIVE);
// MCS 16 TB size of 7272 bits (3648 x 3584) BER 0.14 SINR 8.48
// BER 0.14 = 1 - ((1-0.075)*(1-0.075))
  AddTestCase (new LenaDataPhyErrorModelTestCase (1, 470, 781, 0.14, 29), TestCase::EXTENSIVE);

 

}

static LenaTestPhyErrorModelrSuite lenaTestPhyErrorModelrSuite;

std::string 
LenaDataPhyErrorModelTestCase::BuildNameString (uint16_t nUser, uint16_t dist)
{
  std::ostringstream oss;
  oss << nUser << " UEs, distance " << dist << " m";
  return oss.str ();
}

LenaDataPhyErrorModelTestCase::LenaDataPhyErrorModelTestCase (uint16_t nUser, uint16_t dist, uint16_t tbSize, double berRef, uint16_t bernQuantile)
  : TestCase (BuildNameString (nUser, dist)),              
    m_nUser (nUser),
    m_dist (dist),
    m_tbSize (tbSize),
    m_berRef (berRef),
    m_bernQuantile (bernQuantile)
{
}

LenaDataPhyErrorModelTestCase::~LenaDataPhyErrorModelTestCase ()
{
}

void
LenaDataPhyErrorModelTestCase::DoRun (void)
{
  
   double ber = 0.03;
  Config::SetDefault ("ns3::LteAmc::Ber", DoubleValue (ber));
  Config::SetDefault ("ns3::LteAmc::AmcModel", EnumValue (LteAmc::PiroEW2010));
  Config::SetDefault ("ns3::LteSpectrumPhy::CtrlErrorModelEnabled", BooleanValue (false));
  Config::SetDefault ("ns3::LteSpectrumPhy::DataErrorModelEnabled", BooleanValue (true));
  Config::SetDefault ("ns3::RrFfMacScheduler::HarqEnabled", BooleanValue (false));
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
//   LogComponentDisableAll (LOG_LEVEL_ALL);

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
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (enbNodes);
  BuildingsHelper::Install (enbNodes);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (ueNodes);
  BuildingsHelper::Install (ueNodes);
  
  // remove random shadowing component
  lena->SetAttribute ("PathlossModel", StringValue ("ns3::HybridBuildingsPropagationLossModel"));
  lena->SetPathlossModelAttribute ("ShadowSigmaOutdoor", DoubleValue (0.0));
  lena->SetPathlossModelAttribute ("ShadowSigmaIndoor", DoubleValue (0.0));
  lena->SetPathlossModelAttribute ("ShadowSigmaExtWalls", DoubleValue (0.0));

  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
  lena->SetSchedulerType ("ns3::RrFfMacScheduler");
  lena->SetSchedulerAttribute ("UlCqiFilter", EnumValue (FfMacScheduler::PUSCH_UL_CQI));
  
  enbDevs = lena->InstallEnbDevice (enbNodes);
  ueDevs = lena->InstallUeDevice (ueNodes);

  // Attach a UE to a eNB
  lena->Attach (ueDevs, enbDevs.Get (0));

  // Activate an EPS bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lena->ActivateDataRadioBearer (ueDevs, bearer);
  

  Ptr<LteEnbNetDevice> lteEnbDev = enbDevs.Get (0)->GetObject<LteEnbNetDevice> ();
  Ptr<LteEnbPhy> enbPhy = lteEnbDev->GetPhy ();
  enbPhy->SetAttribute ("TxPower", DoubleValue (43.0));
  enbPhy->SetAttribute ("NoiseFigure", DoubleValue (5.0));
  // place the HeNB over the default rooftop level (20 mt.)
  Ptr<MobilityModel> mm = enbNodes.Get (0)->GetObject<MobilityModel> ();
  mm->SetPosition (Vector (0.0, 0.0, 30.0));

  // Set UEs' position and power
  for (int i = 0; i < m_nUser; i++)
    {
      Ptr<MobilityModel> mm = ueNodes.Get (i)->GetObject<MobilityModel> ();
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
  NS_LOG_INFO ("\tTest downlink/uplink data shared channels (PDSCH and PUSCH)");
  NS_LOG_INFO ("Test with " << m_nUser << " user(s) at distance " << m_dist << " expected BER " << m_berRef);
  std::vector <uint64_t> dlDataRxed;
  for (int i = 0; i < m_nUser; i++)
    {
      // get the imsi
      uint64_t imsi = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetImsi ();
      // get the lcId
      uint8_t lcId = 1;
      dlDataRxed.push_back (rlcStats->GetDlRxData (imsi, lcId));
      double txed = rlcStats->GetDlTxData (imsi, lcId);
      int n = txed / m_tbSize;
      int lambda = (double)dlDataRxed.at (i) / m_tbSize;
      double ber = 1.0 - ((double)dlDataRxed.at (i)/txed);
      double np = n-n*m_berRef;
      NS_LOG_INFO ("\tUser " << i << " imsi " << imsi << " bytes rxed " << (double)dlDataRxed.at (i) << " txed " << txed 
                   << " BER " << ber << " Err " << std::fabs (m_berRef - ber) << " lambda " << lambda 
                   << " np " << np << " difference " << std::abs (lambda - np) << " quantile " << m_bernQuantile);
      NS_UNUSED (ber);
      // the quantiles are evaluated offline according to a Bernoulli 
      // ditribution with n equal to the number of packet sent and p equal 
      // to the BER (see /reference/bernuolliDistribution.m for details)
      NS_TEST_ASSERT_MSG_EQ_TOL (lambda, np, m_bernQuantile, " Unexpected BER distribution!");
    }


  Simulator::Destroy ();
}




std::string 
LenaDlCtrlPhyErrorModelTestCase::BuildNameString (uint16_t nEnb, uint16_t dist)
{
  std::ostringstream oss;
  oss << nEnb << " eNBs, distance " << dist << " m";
  return oss.str ();
}


LenaDlCtrlPhyErrorModelTestCase::LenaDlCtrlPhyErrorModelTestCase (uint16_t nEnb, uint16_t dist, uint16_t tbSize, double berRef)
: TestCase (BuildNameString (nEnb, dist)),              
m_nEnb (nEnb),
m_dist (dist),
m_tbSize (tbSize),
m_berRef (berRef)
{
}

LenaDlCtrlPhyErrorModelTestCase::~LenaDlCtrlPhyErrorModelTestCase ()
{
}

void
LenaDlCtrlPhyErrorModelTestCase::DoRun (void)
{
  
  double ber = 0.03;
  Config::SetDefault ("ns3::LteAmc::Ber", DoubleValue (ber));
  Config::SetDefault ("ns3::LteAmc::AmcModel", EnumValue (LteAmc::PiroEW2010));
  Config::SetDefault ("ns3::LteSpectrumPhy::CtrlErrorModelEnabled", BooleanValue (true));
  Config::SetDefault ("ns3::LteSpectrumPhy::DataErrorModelEnabled", BooleanValue (false));
  Config::SetDefault ("ns3::RrFfMacScheduler::HarqEnabled", BooleanValue (false));
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
  
//     LogComponentEnable ("RrFfMacScheduler", LOG_LEVEL_ALL);
  //   LogComponentEnable ("LenaHelper", LOG_LEVEL_ALL);
  //   LogComponentEnable ("RlcStatsCalculator", LOG_LEVEL_ALL);
  
  
  //   LogComponentEnable ("LteSpectrumPhy", LOG_LEVEL_ALL);
  //   LogComponentEnable ("LteEnbMac", LOG_LEVEL_ALL);
  //   LogComponentEnable ("LteEnbPhy", LOG_LEVEL_ALL);
  //   LogComponentEnable ("LteUePhy", LOG_LEVEL_ALL);
//     LogComponentEnable ("RrFfMacScheduler", LOG_LEVEL_ALL);
  //   LogComponentEnable ("LenaHelper", LOG_LEVEL_ALL);
  //   LogComponentEnable ("BuildingsPropagationLossModel", LOG_LEVEL_ALL);
//     LogComponentEnable ("LteMiErrorModel", LOG_LEVEL_ALL);
  //   LogComponentEnable ("LteAmc", LOG_LEVEL_ALL);
  //   
//   LogComponentDisableAll (LOG_LEVEL_ALL);
  
  LogComponentEnable ("LenaTestPhyErrorModel", LOG_LEVEL_ALL);
  
  
  /**
  * Initialize Simulation Scenario: 1 eNB and m_nUser UEs
  */
  
  Ptr<LteHelper> lena = CreateObject<LteHelper> ();
  
  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes;
  enbNodes.Create (m_nEnb);
  ueNodes.Create (1);
  
  // Install Mobility Model
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (enbNodes);
  BuildingsHelper::Install (enbNodes);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (ueNodes);
  BuildingsHelper::Install (ueNodes);
  
  // remove random shadowing component
  lena->SetAttribute ("PathlossModel", StringValue ("ns3::HybridBuildingsPropagationLossModel"));
  lena->SetPathlossModelAttribute ("ShadowSigmaOutdoor", DoubleValue (0.0));
  lena->SetPathlossModelAttribute ("ShadowSigmaIndoor", DoubleValue (0.0));
  lena->SetPathlossModelAttribute ("ShadowSigmaExtWalls", DoubleValue (0.0));
  
  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
  lena->SetSchedulerType ("ns3::RrFfMacScheduler");
  lena->SetSchedulerAttribute ("UlCqiFilter", EnumValue (FfMacScheduler::PUSCH_UL_CQI));
  
  enbDevs = lena->InstallEnbDevice (enbNodes);
  ueDevs = lena->InstallUeDevice (ueNodes);
  
  // Attach a UE to one eNB (the others are interfering ones)
  lena->Attach (ueDevs, enbDevs.Get (0));
  
  // Activate an EPS bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lena->ActivateDataRadioBearer (ueDevs, bearer);
  
  // Set UEs' position and power
  for (int i = 0; i < m_nEnb; i++)
    {
      // place the HeNB over the default rooftop level (20 mt.)
      Ptr<MobilityModel> mm = enbNodes.Get (i)->GetObject<MobilityModel> ();
      mm->SetPosition (Vector (0.0, 0.0, 30.0));
      Ptr<LteEnbNetDevice> lteEnbDev = enbDevs.Get (i)->GetObject<LteEnbNetDevice> ();
      Ptr<LteEnbPhy> enbPhy = lteEnbDev->GetPhy ();
      enbPhy->SetAttribute ("TxPower", DoubleValue (43.0));
      enbPhy->SetAttribute ("NoiseFigure", DoubleValue (5.0));
    }
  
  // Set UEs' position and power
  Ptr<MobilityModel> mm = ueNodes.Get (0)->GetObject<MobilityModel> ();
  mm->SetPosition (Vector (m_dist, 0.0, 1.0));
  Ptr<LteUeNetDevice> lteUeDev = ueDevs.Get (0)->GetObject<LteUeNetDevice> ();
  Ptr<LteUePhy> uePhy = lteUeDev->GetPhy ();
  uePhy->SetAttribute ("TxPower", DoubleValue (23.0));
  uePhy->SetAttribute ("NoiseFigure", DoubleValue (9.0));
  
  
  lena->EnableRlcTraces ();
  double simulationTime = 1.000;
  
  Simulator::Stop (Seconds (simulationTime));
  
  Ptr<RadioBearerStatsCalculator> rlcStats = lena->GetRlcStats ();
  rlcStats->SetAttribute ("EpochDuration", TimeValue (Seconds (simulationTime)));
  
  
  Simulator::Run ();
  
  /**
  * Check that the assignation is done in a RR fashion
  */
  NS_LOG_INFO ("\tTest downlink control channels (PCFICH+PDCCH)");
  NS_LOG_INFO ("Test with " << m_nEnb << " eNB(s) at distance " << m_dist << " expected BER " << m_berRef);
  std::vector <uint64_t> dlDataRxed;
  int nUser = 1;
  for (int i = 0; i < nUser; i++)
  {
    // get the imsi
    uint64_t imsi = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetImsi ();
    // get the lcId
    uint8_t lcId = 1;
    dlDataRxed.push_back (rlcStats->GetDlRxData (imsi, lcId));
    double txed = rlcStats->GetDlTxData (imsi, lcId);
    double ber = 1.0 - ((double)dlDataRxed.at (i)/txed);
    NS_LOG_INFO ("\tUser " << i << " imsi " << imsi << " bytes rxed " << (double)dlDataRxed.at (i) << " txed " << txed 
    << " BER " << ber << " Err " << fabs (m_berRef - ber));
    NS_UNUSED (ber);
    NS_TEST_ASSERT_MSG_EQ_TOL (ber, m_berRef, 0.1, " Unexpected BER distribution!");
  }
  
  
  Simulator::Destroy ();
}


} // namespace
