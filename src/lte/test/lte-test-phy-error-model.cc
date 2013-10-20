/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2013 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 *         Nicola Baldo <nbaldo@cttc.es>
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
  // 1 interfering eNB SINR -2.0 BLER 0.007 TB size 217
  AddTestCase (new LenaDlCtrlPhyErrorModelTestCase (2, 1078, 217, 0.007), TestCase::QUICK);
  // 2 interfering eNBs SINR -4.0 BLER 0.037 TB size 217
  AddTestCase (new LenaDlCtrlPhyErrorModelTestCase (3, 1040, 217, 0.045), TestCase::EXTENSIVE);
  // 3 interfering eNBs SINR -6.0 BLER 0.21 TB size 133
  AddTestCase (new LenaDlCtrlPhyErrorModelTestCase (4, 1250, 133, 0.206), TestCase::EXTENSIVE);
  // 4 interfering eNBs SINR -7.0 BLER 0.34 TB size 133
  AddTestCase (new LenaDlCtrlPhyErrorModelTestCase (5, 1260, 81, 0.343), TestCase::EXTENSIVE);

  // Tests on DL Data channels (PDSCH)
  // the tolerance is calculated with the following octave code:
  //
  // n =  1000;
  // for p=1-[0.33 0.11 0.2 0.3 0.55 0.14]
  //    tol = n*p - binoinv(0.005, n, p)
  // endfor

  // MCS 2 TB size of 256 bits BLER 0.33 SINR -5.51
  AddTestCase (new LenaDataPhyErrorModelTestCase (4, 1800, 32, 0.33, 39), TestCase::QUICK);
// MCS 2 TB size of 528 bits BLER 0.11 SINR -5.51
  AddTestCase (new LenaDataPhyErrorModelTestCase (2, 1800, 66, 0.11, 26), TestCase::EXTENSIVE);
// MCS 2 TB size of 1088 bits BLER 0.02 SINR -5.51
  AddTestCase (new LenaDataPhyErrorModelTestCase (1, 1800, 136, 0.02, 33), TestCase::EXTENSIVE);
  // MCS 12 TB size of 4800 bits  BLER 0.3  SINR 4.43
  AddTestCase (new LenaDataPhyErrorModelTestCase (1, 600, 600, 0.3, 38), TestCase::EXTENSIVE);
// MCS 12 TB size of 1632 bits  BLER 0.55  SINR 4.43
  AddTestCase (new LenaDataPhyErrorModelTestCase (3, 600, 204, 0.55, 40), TestCase::EXTENSIVE);
// MCS 16 TB size of 7272 bits (3648 x 3584) BLER 0.14 SINR 8.48
// BLER 0.14 = 1 - ((1-0.075)*(1-0.075))
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

LenaDataPhyErrorModelTestCase::LenaDataPhyErrorModelTestCase (uint16_t nUser, uint16_t dist, uint16_t tbSize, double blerRef, uint16_t toleranceRxPackets)
  : TestCase (BuildNameString (nUser, dist)),              
    m_nUser (nUser),
    m_dist (dist),
    m_tbSize (tbSize),
    m_blerRef (blerRef),
    m_toleranceRxPackets (toleranceRxPackets)
{
  NS_UNUSED (m_tbSize);
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
  Time simulationTime = Seconds (1.000);

  Simulator::Stop (simulationTime);

  Ptr<RadioBearerStatsCalculator> rlcStats = lena->GetRlcStats ();
  rlcStats->SetAttribute ("EpochDuration", TimeValue (simulationTime));


  Simulator::Run ();

  NS_LOG_INFO ("\tTest downlink data shared channels (PDSCH)");
  NS_LOG_INFO ("Test with " << m_nUser << " user(s) at distance " << m_dist << " expected BLER " << m_blerRef);
  for (int i = 0; i < m_nUser; i++)
    {
      // get the imsi
      uint64_t imsi = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetImsi ();
      uint8_t lcId = 3;

      double dlRxPackets = rlcStats->GetDlRxPackets (imsi, lcId);
      double dlTxPackets = rlcStats->GetDlTxPackets (imsi, lcId);
      double dlBler =  1.0 - (dlRxPackets/dlTxPackets);
      double expectedDlRxPackets = dlTxPackets -dlTxPackets*m_blerRef;
      NS_LOG_INFO ("\tUser " << i << " imsi " << imsi << " DOWNLINK"
                   << " pkts rx " << dlRxPackets << " tx " << dlTxPackets
                   << " BLER " << dlBler << " Err " << std::fabs (m_blerRef - dlBler) 
                   << " expected rx " << expectedDlRxPackets 
                   << " difference " << std::abs (expectedDlRxPackets - dlRxPackets) 
                   << " tolerance " << m_toleranceRxPackets);
      NS_UNUSED (dlBler);

      // sanity check for whether the tx packets reported by the stats are correct 
      // we expect one packet per TTI, excluding the initial RRC connection establishment period
      double expectedDlMinTxPackets = simulationTime.GetMilliSeconds () - 50;
      NS_TEST_ASSERT_MSG_GT (dlTxPackets, expectedDlMinTxPackets, " too few DL TX packets reported");

      // the quantiles are evaluated offline according to a Bernoulli 
      // ditribution with n equal to the number of packet sent and p equal 
      // to the BLER (see /reference/bernuolliDistribution.m for details)
      NS_TEST_ASSERT_MSG_EQ_TOL (dlRxPackets, expectedDlRxPackets, m_toleranceRxPackets, " Unexpected DL BLER distribution!");


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


LenaDlCtrlPhyErrorModelTestCase::LenaDlCtrlPhyErrorModelTestCase (uint16_t nEnb, uint16_t dist, uint16_t tbSize, double blerRef)
: TestCase (BuildNameString (nEnb, dist)),              
m_nEnb (nEnb),
m_dist (dist),
m_tbSize (tbSize),
m_blerRef (blerRef)
{
  NS_UNUSED (m_tbSize);
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
  Time simulationTime = Seconds (1.000);
  
  Simulator::Stop (simulationTime);
  
  Ptr<RadioBearerStatsCalculator> rlcStats = lena->GetRlcStats ();
  rlcStats->SetAttribute ("EpochDuration", TimeValue (simulationTime));
  
  
  Simulator::Run ();
  
  NS_LOG_INFO ("\tTest downlink control channels (PCFICH+PDCCH)");
  NS_LOG_INFO ("Test with " << m_nEnb << " eNB(s) at distance " << m_dist << " expected BLER " << m_blerRef);
  int nUser = 1;
  for (int i = 0; i < nUser; i++)
    {
      // get the imsi
      uint64_t imsi = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetImsi ();
      uint8_t lcId = 3;
      double dlRxPackets = rlcStats->GetDlRxPackets (imsi, lcId);
      double dlTxPackets = rlcStats->GetDlTxPackets (imsi, lcId);
      double bler = 1.0 - (dlRxPackets/dlTxPackets);
      NS_LOG_INFO ("\tUser " << i << " imsi " << imsi 
                   << " pkts rx " << dlRxPackets << " tx " << dlTxPackets
                   << " BLER " << bler << " Err " << fabs (m_blerRef - bler));
      NS_UNUSED (bler);

      // sanity check for whether the tx packets reported by the stats are correct 
      // we expect one packet per TTI, excluding the initial RRC connection establishment period
      double expectedMinDlTxPackets = simulationTime.GetMilliSeconds () - 50;
      NS_TEST_ASSERT_MSG_GT (dlTxPackets, expectedMinDlTxPackets, " too few TX packets reported");

      NS_TEST_ASSERT_MSG_EQ_TOL (bler, m_blerRef, 0.1, " Unexpected BLER distribution!");
    }  
  
  Simulator::Destroy ();
}


} // namespace
