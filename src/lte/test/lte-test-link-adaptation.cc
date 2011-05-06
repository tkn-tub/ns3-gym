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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */

#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/double.h"

#include "ns3/mobility-helper.h"
#include "ns3/lena-helper.h"

#include "ns3/lte-test-link-adaptation.h"

NS_LOG_COMPONENT_DEFINE ("LteLinkAdaptationTest");

using namespace ns3;


uint32_t LteLinkAdaptationTestCase::m_runId = 0;


/**
 * Test 1.3 Link Adaptation
 */

void
LteTestDlSchedulingCallback (std::string path,
                      uint32_t frameNo, uint32_t subframeNo, uint16_t rnti,
                      uint8_t mcsTb1, uint16_t sizeTb1, uint8_t mcsTb2, uint16_t sizeTb2)
{
  static bool firstTime = true;

  if ( firstTime )
    {
      NS_LOG_UNCOND ("frame\tsbframe\trnti\tmcsTb1\tsizeTb1\tmcsTb2\tsizeTb2");
      firstTime = false;
    }

  if ( subframeNo == 10 )
    {
      NS_LOG_UNCOND (" " << frameNo << "\t" << subframeNo << "\t" << rnti << "\t"
                        << (uint16_t)mcsTb1 << "\t" << sizeTb1 << "\t"
                        << (uint16_t)mcsTb2 << "\t" << sizeTb2);
    }
}

/**
 * TestSuite
 */

LteLinkAdaptationTestSuite::LteLinkAdaptationTestSuite ()
  : TestSuite ("lte-link-adaptation", SYSTEM)
{
//   LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);

//   LogComponentEnable ("LteTestUePhy", logLevel);
//   LogComponentEnable ("LteLinkAdaptationTest", logLevel);

  NS_LOG_INFO ("Creating LteLinkAdaptionTestSuite");

  int distanceMin = 1;
  int distanceMax = 100000;
  int distanceStep = 10000;
  bool logStep = false;

  for ( int distance = distanceMin ; distance <= distanceMax ; logStep ? ( distance *= distanceStep) : ( distance += distanceStep ) )
    {
      /**
       * Propagation Loss
       *
       *         (  4 * PI * distance * frequency  ) 2
       *  Loss = ( ------------------------------- )
       *         (            c                    ) 
       *
       *  where: c is speed of light in vacuum = 3e8 (m/s)
       *         distance in (m)
       *         frequency in (Hz)
       */
      double myLoss = ( ( 4 * M_PI * distance * 1.92e9 ) / 3e8 );
      myLoss = myLoss * myLoss;

      AddTestCase (new LteLinkAdaptationTestCase (myLoss));
    }

}

static LteLinkAdaptationTestSuite lteLinkAdaptationTestSuite;


/**
 * TestCase
 */

LteLinkAdaptationTestCase::LteLinkAdaptationTestCase (double loss)
  : TestCase ("Link Adaptation"),
    m_loss (loss)
{
  std::ostringstream sstream1, sstream2;
  sstream1 << loss;
  sstream2 << ++m_runId;

  NS_LOG_INFO ("Creating LteLinkAdaptationTestCase (" << sstream2.str () + "): LOSS = " + sstream1.str ());
}

LteLinkAdaptationTestCase::~LteLinkAdaptationTestCase ()
{
}

void
LteLinkAdaptationTestCase::DoRun (void)
{
#if 1
//   LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);

//   LogComponentEnable ("LteEnbRrc", logLevel);
//   LogComponentEnable ("LteUeRrc", logLevel);
//   LogComponentEnable ("LteEnbMac", logLevel);
//   LogComponentEnable ("LteUeMac", logLevel);
//   LogComponentEnable ("LteRlc", logLevel);
//   LogComponentEnable ("RrPacketScheduler", logLevel);

//   LogComponentEnable ("RrFfMacScheduler", logLevel);

//   LogComponentEnable ("LtePhy", logLevel);
//   LogComponentEnable ("LteEnbPhy", logLevel);
//   LogComponentEnable ("LteUePhy", logLevel);

//   LogComponentEnable ("LteSpectrumValueHelper", logLevel);
//   LogComponentEnable ("LteSpectrumPhy", logLevel);
//   LogComponentEnable ("LteInterference", logLevel);
//   LogComponentEnable ("LteSinrChunkProcessor", logLevel);

//   LogComponentEnable ("LtePropagationLossModel", logLevel);
//   LogComponentEnable ("LossModel", logLevel);
//   LogComponentEnable ("ShadowingLossModel", logLevel);
//   LogComponentEnable ("PenetrationLossModel", logLevel);
//   LogComponentEnable ("MultipathLossModel", logLevel);
//   LogComponentEnable ("PathLossModel", logLevel);
//   LogComponentEnable ("FriisSpectrumPropagationLossModel", logLevel);
//   LogComponentEnable ("ConstantSpectrumPropagationLossModel", logLevel);

//   LogComponentEnable ("LteNetDevice", logLevel);
//   LogComponentEnable ("LteUeNetDevice", logLevel);
//   LogComponentEnable ("LteEnbNetDevice", logLevel);

//   LogComponentEnable ("MobilityModel", logLevel);
//   LogComponentEnable ("ConstantPositionMobilityModel", logLevel);
//   LogComponentEnable ("MultiModelSpectrumChannel", logLevel);
//   LogComponentEnable ("SingleModelSpectrumChannel", logLevel);
#endif

  /**
    * Simulation Topology
    */

  Ptr<LenaHelper> lena = CreateObject<LenaHelper> ();
//   lena->EnableLogComponents ();
  lena->EnableMacTraces ();
  lena->EnableRlcTraces ();
  lena->SetAttribute ("PropagationModel", StringValue ("ns3::ConstantSpectrumPropagationLossModel"));
  NS_LOG_INFO ("LOSS = " << m_loss);
  lena->SetPropagationModelAttribute ("Loss", DoubleValue (m_loss));

  /**
   *  Distance (m)  PropagationLoss (XXX)
   *      0             0.0
   *    100             6.46814e+07
   *    200             2.58726e+08
   *    300             5.82133e+08
   *    400             1.0349e+09
   *    500             1.61704e+09
   *    600             2.32853e+09
   *    700             3.16939e+09
   *    800             4.13961e+09
   *    900             5.2392e+09
   *   1000             6.46814e+09
   */

//   for ( int i = 0 ; i <= 10 ; i++ )
//     {
//       double myLoss = ( ( 4 * M_PI * ( i * 100.0 ) * 1.92e9 ) / 3e8 );
//       myLoss = myLoss * myLoss;
//       NS_LOG_INFO ("i = " << i << "\tLoss = " << myLoss);
//     }

  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes;
  enbNodes.Create (1);
  ueNodes.Create (1);
  NodeContainer allNodes = NodeContainer ( enbNodes, ueNodes );

  // Position of the eNodeB and UE
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (100.0, 0.0, 0.0));

  // Install Mobility Model
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (allNodes);

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


  Config::Connect ("/NodeList/0/DeviceList/0/LteEnbMac/DlScheduling",
                   MakeCallback(&LteTestDlSchedulingCallback));

 
//   Simulator::Stop (Seconds (0.005));
  Simulator::Stop (Seconds (0.01));
  Simulator::Run ();
  Simulator::Destroy ();


  NS_LOG_INFO ("Link Adaptation Test");

  NS_TEST_ASSERT_MSG_EQ_TOL (1.0, 1.0, 0.0000001, "Wrong Test !");
}
