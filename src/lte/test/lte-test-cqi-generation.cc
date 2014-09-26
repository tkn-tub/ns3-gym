/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Piotr Gawlowicz
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
 * Author: Piotr Gawlowicz <gawlowicz.p@gmail.com>
 *
 */

#include <ns3/simulator.h>
#include <ns3/log.h>
#include <ns3/callback.h>
#include <ns3/config.h>
#include <ns3/string.h>
#include <ns3/double.h>
#include <ns3/enum.h>
#include <ns3/boolean.h>
#include <ns3/pointer.h>
#include "ns3/ff-mac-scheduler.h"
#include "ns3/mobility-helper.h"
#include "ns3/lte-helper.h"

#include "lte-test-cqi-generation.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("LteCqiGenerationTest");

void
LteTestDlSchedulingCallback (LteCqiGenerationTestCase *testcase, std::string path,
                             uint32_t frameNo, uint32_t subframeNo, uint16_t rnti,
                             uint8_t mcsTb1, uint16_t sizeTb1, uint8_t mcsTb2, uint16_t sizeTb2)
{
  testcase->DlScheduling (frameNo, subframeNo, rnti, mcsTb1, sizeTb1, mcsTb2, sizeTb2);
}

void
LteTestUlSchedulingCallback (LteCqiGenerationTestCase *testcase, std::string path,
                             uint32_t frameNo, uint32_t subframeNo, uint16_t rnti,
                             uint8_t mcs, uint16_t sizeTb)
{
  testcase->UlScheduling (frameNo, subframeNo, rnti, mcs, sizeTb);
}


/**
 * TestSuite
 */

LteCqiGenerationTestSuite::LteCqiGenerationTestSuite ()
  : TestSuite ("lte-cqi-generation", SYSTEM)
{
//  LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_DEBUG);
//  LogComponentEnable ("LteCqiGenerationTest", logLevel);
  NS_LOG_INFO ("Creating LteCqiGenerationTestSuite");

  AddTestCase (new LteCqiGenerationTestCase ("UsePdcchForCqiGeneration", false, 4, 2), TestCase::QUICK);
  AddTestCase (new LteCqiGenerationTestCase ("UsePdschForCqiGeneration", true, 28, 2), TestCase::QUICK);

}

static LteCqiGenerationTestSuite lteCqiGenerationTestSuite;


LteCqiGenerationTestCase::LteCqiGenerationTestCase (std::string name, bool usePdcchForCqiGeneration,
                                                    uint16_t dlMcs, uint16_t ulMcs)
  : TestCase ("Downlink Power Control: " + name),
    m_dlMcs (dlMcs),
    m_ulMcs (ulMcs)
{
  m_usePdcchForCqiGeneration = usePdcchForCqiGeneration;
  NS_LOG_INFO ("Creating LteCqiGenerationTestCase");
}

LteCqiGenerationTestCase::~LteCqiGenerationTestCase ()
{
}

void
LteCqiGenerationTestCase::DlScheduling (uint32_t frameNo, uint32_t subframeNo, uint16_t rnti,
                                        uint8_t mcsTb1, uint16_t sizeTb1, uint8_t mcsTb2, uint16_t sizeTb2)
{
  // need to allow for RRC connection establishment + CQI feedback reception
  if (Simulator::Now () > MilliSeconds (35))
    {
//	  NS_LOG_UNCOND("DL MSC: " << (uint32_t)mcsTb1 << " expected DL MCS: " << (uint32_t)m_dlMcs);
      NS_TEST_ASSERT_MSG_EQ ((uint32_t)mcsTb1, (uint32_t)m_dlMcs, "Wrong DL MCS ");
    }
}

void
LteCqiGenerationTestCase::UlScheduling (uint32_t frameNo, uint32_t subframeNo, uint16_t rnti,
                                        uint8_t mcs, uint16_t sizeTb)
{
  // need to allow for RRC connection establishment + SRS transmission
  if (Simulator::Now () > MilliSeconds (50))
    {
//	  NS_LOG_UNCOND("UL MSC: " << (uint32_t)mcs << " expected UL MCS: " << (uint32_t)m_ulMcs);
      NS_TEST_ASSERT_MSG_EQ ((uint32_t)mcs, (uint32_t)m_ulMcs, "Wrong UL MCS");
    }
}

void
LteCqiGenerationTestCase::DoRun (void)
{
  NS_LOG_DEBUG ("LteCqiGenerationTestCase");

  Config::Reset ();
  Config::SetDefault ("ns3::LteHelper::UseIdealRrc", BooleanValue (true));
  Config::SetDefault ("ns3::LteHelper::UsePdschForCqiGeneration", BooleanValue (m_usePdcchForCqiGeneration));

  Config::SetDefault ("ns3::LteSpectrumPhy::CtrlErrorModelEnabled", BooleanValue (true));
  Config::SetDefault ("ns3::LteSpectrumPhy::DataErrorModelEnabled", BooleanValue (true));

  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();

  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes1;
  NodeContainer ueNodes2;
  enbNodes.Create (2);
  ueNodes1.Create (1);
  ueNodes2.Create (1);
  NodeContainer allNodes = NodeContainer ( enbNodes, ueNodes1, ueNodes2);

  /*
   * The topology is the following:
   *
   *  eNB1                        UE1 UE2                        eNB2
   *    |                            |                            |
   *    x -------------------------- x -------------------------- x
   *                  500 m                       500 m
   *
   */

  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));   // eNB1
  positionAlloc->Add (Vector (1000, 0.0, 0.0)); // eNB2
  positionAlloc->Add (Vector (500.0, 0.0, 0.0));  // UE1
  positionAlloc->Add (Vector (500, 0.0, 0.0));  // UE2
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (allNodes);

  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs1;
  NetDeviceContainer ueDevs2;
  lteHelper->SetSchedulerType ("ns3::PfFfMacScheduler");
  lteHelper->SetSchedulerAttribute ("UlCqiFilter", EnumValue (FfMacScheduler::PUSCH_UL_CQI));

  lteHelper->SetFfrAlgorithmType ("ns3::LteFrHardAlgorithm");

  lteHelper->SetFfrAlgorithmAttribute ("DlSubBandOffset", UintegerValue (0));
  lteHelper->SetFfrAlgorithmAttribute ("DlSubBandwidth", UintegerValue (12));
  lteHelper->SetFfrAlgorithmAttribute ("UlSubBandOffset", UintegerValue (0));
  lteHelper->SetFfrAlgorithmAttribute ("UlSubBandwidth", UintegerValue (25));
  enbDevs.Add (lteHelper->InstallEnbDevice (enbNodes.Get (0)));

  lteHelper->SetFfrAlgorithmAttribute ("DlSubBandOffset", UintegerValue (12));
  lteHelper->SetFfrAlgorithmAttribute ("DlSubBandwidth", UintegerValue (12));
  lteHelper->SetFfrAlgorithmAttribute ("UlSubBandOffset", UintegerValue (0));
  lteHelper->SetFfrAlgorithmAttribute ("UlSubBandwidth", UintegerValue (25));
  enbDevs.Add (lteHelper->InstallEnbDevice (enbNodes.Get (1)));

  ueDevs1 = lteHelper->InstallUeDevice (ueNodes1);
  ueDevs2 = lteHelper->InstallUeDevice (ueNodes2);

  // Attach a UE to a eNB
  lteHelper->Attach (ueDevs1, enbDevs.Get (0));
  lteHelper->Attach (ueDevs2, enbDevs.Get (1));

  // Activate an EPS bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lteHelper->ActivateDataRadioBearer (ueDevs1, bearer);
  lteHelper->ActivateDataRadioBearer (ueDevs2, bearer);

  Config::Connect ("/NodeList/0/DeviceList/0/LteEnbMac/DlScheduling",
                   MakeBoundCallback (&LteTestDlSchedulingCallback, this));

  Config::Connect ("/NodeList/0/DeviceList/0/LteEnbMac/UlScheduling",
                   MakeBoundCallback (&LteTestUlSchedulingCallback, this));

  Config::Connect ("/NodeList/1/DeviceList/0/LteEnbMac/DlScheduling",
                   MakeBoundCallback (&LteTestDlSchedulingCallback, this));

  Config::Connect ("/NodeList/1/DeviceList/0/LteEnbMac/UlScheduling",
                   MakeBoundCallback (&LteTestUlSchedulingCallback, this));

  Simulator::Stop (Seconds (1.100));
  Simulator::Run ();

  Simulator::Destroy ();
}
