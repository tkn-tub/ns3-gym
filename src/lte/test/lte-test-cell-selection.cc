/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 University of Jyvaskyla
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
 * Author: Budiarto Herman <buherman@student.jyu.fi>
 *
 */

#include "lte-test-cell-selection.h"

#include <ns3/simulator.h>
#include <ns3/log.h>
#include <ns3/boolean.h>
#include <ns3/double.h>

#include <ns3/mobility-helper.h>
#include <ns3/lte-helper.h>
#include <ns3/epc-helper.h>
#include <ns3/internet-stack-helper.h>
#include <ns3/point-to-point-helper.h>
#include <ns3/ipv4-address-helper.h>
#include <ns3/ipv4-static-routing-helper.h>

#include <ns3/node-container.h>
#include <ns3/net-device-container.h>
#include <ns3/ipv4-interface-container.h>

#include <ns3/lte-ue-net-device.h>
#include <ns3/lte-ue-rrc.h>
#include <ns3/lte-enb-net-device.h>
#include <ns3/lte-enb-phy.h>

NS_LOG_COMPONENT_DEFINE ("LteCellSelectionTest");

namespace ns3 {


/*
 * Test Suite
 */


LteCellSelectionTestSuite::LteCellSelectionTestSuite ()
  : TestSuite ("lte-cell-selection", SYSTEM)
{
  //LogComponentEnable ("LteCellSelectionTest", LOG_PREFIX_ALL);
  //LogComponentEnable ("LteCellSelectionTest", LOG_FUNCTION);
  //LogComponentEnable ("LteUePhy", LOG_INFO);
  //LogComponentEnable ("LteUePhy", LOG_WARN);
  //LogComponentEnable ("LteUePhy", LOG_DEBUG);
  //LogComponentEnable ("LteUePhy", LOG_FUNCTION);
  //LogComponentEnable ("LteSpectrumValueHelper", LOG_FUNCTION);
  //LogComponentEnable ("LteSpectrumPhy", LOG_LOGIC);
  //LogComponentEnable ("LteSpectrumPhy", LOG_FUNCTION);
  //LogComponentEnable ("LteEnbPhy", LOG_FUNCTION);
  //LogComponentEnable ("LteUeRrc", LOG_PREFIX_ALL);
  //LogComponentEnable ("LteUeRrc", LOG_LOGIC);
  //LogComponentEnable ("LteUeRrc", LOG_WARN);

  std::vector<LteCellSelectionTestCase::UeSetup_t> x;

  /*
   *           <--           <--
   *    ue1---CELL3   ue2---CELL4   ue3
   *         /                     /
   *        /                     /
   *       /   -->           --> /
   *    ue4   CELL1---ue5   CELL2---ue6
   */
  x.clear ();                                    // position  x   y  z, csg expectedCellId
  x.push_back (LteCellSelectionTestCase::UeSetup_t (Vector (-80, 20, 0), 0, 3));
  x.push_back (LteCellSelectionTestCase::UeSetup_t (Vector ( 80, 20, 0), 0, 4));
  x.push_back (LteCellSelectionTestCase::UeSetup_t (Vector (240, 20, 0), 0, 2));
  x.push_back (LteCellSelectionTestCase::UeSetup_t (Vector (-80, 10, 0), 0, 3));
  x.push_back (LteCellSelectionTestCase::UeSetup_t (Vector ( 80, 10, 0), 0, 1));
  x.push_back (LteCellSelectionTestCase::UeSetup_t (Vector (240, 10, 0), 0, 2));
  AddTestCase (new LteCellSelectionTestCase ("[EPC] Open access",
                                             true, false, x, MilliSeconds (261)),
               TestCase::QUICK);

  /*
   *           <--           <--
   *    ue1---CELL3   ue2---CELL4   ue3
   *    (1)    (1)    (1)  / (0)   /(1)
   *        /-------------/       /
   *       /   -->       /   --> /
   *    ue4   CELL1   ue5   CELL2---ue6
   *    (0)    (1)    (0)    (0)    (0)
   */
  x.clear ();                                    // position  x   y  z, csg expectedCellId
  x.push_back (LteCellSelectionTestCase::UeSetup_t (Vector (-80, 20, 0), 1, 3));
  x.push_back (LteCellSelectionTestCase::UeSetup_t (Vector ( 80, 20, 0), 1, 4));
  x.push_back (LteCellSelectionTestCase::UeSetup_t (Vector (240, 20, 0), 1, 2));
  x.push_back (LteCellSelectionTestCase::UeSetup_t (Vector (-80, 10, 0), 0, 4));
  x.push_back (LteCellSelectionTestCase::UeSetup_t (Vector ( 80, 10, 0), 0, 4));
  x.push_back (LteCellSelectionTestCase::UeSetup_t (Vector (240, 10, 0), 0, 2));
  AddTestCase (new LteCellSelectionTestCase ("[EPC] Some CSG 1",
                                             true, true, x, MilliSeconds (361)),
               TestCase::QUICK);

  /*
   *        /-------------\
   *       /   <--         \ <--
   *    ue1   CELL3   ue2---CELL4   ue3
   *    (0)  / (1)    (0)    (0)   /(0)
   *        /                     /
   *       /   -->           --> /
   *    ue4   CELL1---ue5   CELL2---ue6
   *    (1)    (1)    (1)    (0)    (1)
   */
  x.clear ();                                    // position  x   y  z, csg expectedCellId
  x.push_back (LteCellSelectionTestCase::UeSetup_t (Vector (-80, 20, 0), 0, 4));
  x.push_back (LteCellSelectionTestCase::UeSetup_t (Vector ( 80, 20, 0), 0, 4));
  x.push_back (LteCellSelectionTestCase::UeSetup_t (Vector (240, 20, 0), 0, 2));
  x.push_back (LteCellSelectionTestCase::UeSetup_t (Vector (-80, 10, 0), 1, 3));
  x.push_back (LteCellSelectionTestCase::UeSetup_t (Vector ( 80, 10, 0), 1, 1));
  x.push_back (LteCellSelectionTestCase::UeSetup_t (Vector (240, 10, 0), 1, 2));
  AddTestCase (new LteCellSelectionTestCase ("[EPC] Some CSG 2",
                                             true, true, x, MilliSeconds (361)),
               TestCase::EXTENSIVE);

} // end of LteCellSelectionTestSuite::LteCellSelectionTestSuite ()


static LteCellSelectionTestSuite g_lteCellSelectionTestSuite;



/*
 * Test Case
 */


LteCellSelectionTestCase::UeSetup_t::UeSetup_t (Vector position,
                                                uint32_t csgIdentity,
                                                uint16_t expectedCellId)
  : position (position), csgIdentity (csgIdentity),
    expectedCellId (expectedCellId)
{
}


LteCellSelectionTestCase::LteCellSelectionTestCase (
  std::string name, bool isEpcMode, bool hasCsgDiversity,
  std::vector<UeSetup_t> ueSetupList, Time duration)
  : TestCase (name), m_isEpcMode (isEpcMode),
    m_hasCsgDiversity (hasCsgDiversity), m_ueSetupList (ueSetupList),
    m_duration (duration), m_lastState (LteUeRrc::NUM_STATES)
{
  NS_LOG_FUNCTION (this << GetName ());
}


LteCellSelectionTestCase::~LteCellSelectionTestCase ()
{
  NS_LOG_FUNCTION (this << GetName ());
}


void
LteCellSelectionTestCase::DoRun ()
{
  NS_LOG_FUNCTION (this << GetName ());

  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  lteHelper->SetAttribute ("PathlossModel",
                           StringValue ("ns3::FriisSpectrumPropagationLossModel"));
  lteHelper->SetAttribute ("UseIdealRrc", BooleanValue (true));

  Ptr<EpcHelper> epcHelper;

  if (m_isEpcMode)
    {
      epcHelper = CreateObject<EpcHelper> ();
      lteHelper->SetEpcHelper (epcHelper);
    }

  /*
   * The topology is the following:
   *
   *         +   <--x                <--x      - Parabolic antenna model is used
   *         |    Cell3               Cell4    - eNodeB 1 at (  0, 10, 0), facing east
   *    10 m |                                 - eNodeB 2 at (160, 10, 0), facing east
   *         |                                 - eNodeB 3 at (  0, 20, 0), facing west
   *         +      x-->                x-->   - eNodeB 4 at (160, 20, 0), facing west
   *              Cell1               Cell2    - UEs are placed according to input
   *                                           - UEs do not move during simulation
   *                +------ 160 m ------+
   */

  // Create Nodes
  NodeContainer enbNodes;
  enbNodes.Create (4);
  NodeContainer ueNodes;
  uint16_t nUe = m_ueSetupList.size ();
  ueNodes.Create (nUe);

  // Assign nodes to position
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (  0.0, 10.0, 0.0));
  positionAlloc->Add (Vector (160.0, 10.0, 0.0));
  positionAlloc->Add (Vector (  0.0, 20.0, 0.0));
  positionAlloc->Add (Vector (160.0, 20.0, 0.0));

  std::vector<UeSetup_t>::const_iterator itSetup;
  for (itSetup = m_ueSetupList.begin (); itSetup != m_ueSetupList.end (); itSetup++)
    {
      positionAlloc->Add (itSetup->position);
    }

  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (enbNodes);
  mobility.Install (ueNodes);

  // Create Devices and install them in the Nodes (eNB and UE)
  lteHelper->SetEnbAntennaModelType ("ns3::ParabolicAntennaModel");
  lteHelper->SetEnbAntennaModelAttribute ("Beamwidth", DoubleValue (70));
  lteHelper->SetEnbAntennaModelAttribute ("MaxAttenuation", DoubleValue (20.0));
  NetDeviceContainer enbDevs;
  lteHelper->SetEnbAntennaModelAttribute ("Orientation", DoubleValue (0));
  enbDevs.Add (lteHelper->InstallEnbDevice (enbNodes.Get (0)));
  enbDevs.Add (lteHelper->InstallEnbDevice (enbNodes.Get (1)));
  lteHelper->SetEnbAntennaModelAttribute ("Orientation", DoubleValue (180));
  enbDevs.Add (lteHelper->InstallEnbDevice (enbNodes.Get (2)));
  enbDevs.Add (lteHelper->InstallEnbDevice (enbNodes.Get (3)));

  NetDeviceContainer ueDevs;
  ueDevs = lteHelper->InstallUeDevice (ueNodes);

  // Set the PLMN and CSG ID
  if (m_hasCsgDiversity)
    {
      lteHelper->SetEnbCsgId (enbDevs.Get (0), 1, true);
      lteHelper->SetEnbCsgId (enbDevs.Get (2), 1, true);
    }

  NS_ASSERT (m_ueSetupList.size () == ueDevs.GetN ());
  NetDeviceContainer::Iterator itDev;
  for (itSetup = m_ueSetupList.begin (), itDev = ueDevs.Begin ();
       itSetup != m_ueSetupList.end () || itDev != ueDevs.End ();
       itSetup++, itDev++)
    {
      lteHelper->SetUeCsgId (*itDev, itSetup->csgIdentity);
    }

  if (m_isEpcMode)
    {
      // Create P-GW node
      Ptr<Node> pgw = epcHelper->GetPgwNode ();

      // Create a single RemoteHost
      NodeContainer remoteHostContainer;
      remoteHostContainer.Create (1);
      Ptr<Node> remoteHost = remoteHostContainer.Get (0);
      InternetStackHelper internet;
      internet.Install (remoteHostContainer);

      // Create the Internet
      PointToPointHelper p2ph;
      p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Gb/s")));
      p2ph.SetDeviceAttribute ("Mtu", UintegerValue (1500));
      p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.010)));
      NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);
      Ipv4AddressHelper ipv4h;
      ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
      Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);

      // Routing of the Internet Host (towards the LTE network)
      Ipv4StaticRoutingHelper ipv4RoutingHelper;
      Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
      remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);

      // Install the IP stack on the UEs
      internet.Install (ueNodes);
      Ipv4InterfaceContainer ueIpIfaces;
      ueIpIfaces = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueDevs));

      // Assign IP address to UEs
      for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
        {
          Ptr<Node> ueNode = ueNodes.Get (u);
          // Set the default gateway for the UE
          Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
          ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
        }

      // Enable Idle mode cell selection
      lteHelper->Attach (ueDevs);

    } // end of if (m_isEpcMode)
  else
    {
      NS_FATAL_ERROR ("No support yet for LTE_only simulations");
    }

  // Connect to trace sources in UEs
  Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/MibReceived",
                   MakeCallback (&LteCellSelectionTestCase::MibReceivedCallback, this));
  Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/Sib1Received",
                   MakeCallback (&LteCellSelectionTestCase::Sib1ReceivedCallback, this));
  Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/Sib2Received",
                   MakeCallback (&LteCellSelectionTestCase::Sib2ReceivedCallback, this));
  Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/StateTransition",
                   MakeCallback (&LteCellSelectionTestCase::StateTransitionCallback, this));
  Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/InitialCellSelectionEndOk",
                   MakeCallback (&LteCellSelectionTestCase::InitialCellSelectionEndOkCallback, this));
  Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/InitialCellSelectionEndError",
                   MakeCallback (&LteCellSelectionTestCase::InitialCellSelectionEndErrorCallback, this));

  // Run simulation
  Simulator::Stop (m_duration);
  Simulator::Run ();

  // Tests
  NS_ASSERT (m_ueSetupList.size () == ueDevs.GetN ());
  for (itSetup = m_ueSetupList.begin (), itDev = ueDevs.Begin ();
       itSetup != m_ueSetupList.end () || itDev != ueDevs.End ();
       itSetup++, itDev++)
    {
      Ptr<LteUeNetDevice> ueDev = (*itDev)->GetObject<LteUeNetDevice> ();
      NS_ASSERT (ueDev != 0);
      uint16_t actualCellId = ueDev->GetRrc ()->GetCellId ();
      uint16_t expectedCellId = itSetup->expectedCellId;
      NS_TEST_ASSERT_MSG_EQ (actualCellId, expectedCellId,
                             "IMSI " << ueDev->GetImsi ()
                                     << " has attached to an unexpected cell");
    }

  NS_TEST_ASSERT_MSG_EQ (m_lastState, LteUeRrc::CONNECTED_NORMALLY,
                         "UE is not at CONNECTED_NORMALLY state");

  Simulator::Destroy ();

} // end of void LteCellSelectionTestCase::DoRun ()


void
LteCellSelectionTestCase::MibReceivedCallback (
  std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti,
  uint16_t sourceCellId)
{
  NS_LOG_FUNCTION (this << context << imsi << cellId << rnti << sourceCellId);
}


void
LteCellSelectionTestCase::Sib1ReceivedCallback (
  std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti,
  uint16_t sourceCellId)
{
  NS_LOG_FUNCTION (this << context << imsi << cellId << rnti << sourceCellId);
}


void
LteCellSelectionTestCase::Sib2ReceivedCallback (
  std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti)
{
  NS_LOG_FUNCTION (this << context << imsi << cellId << rnti);
}


void
LteCellSelectionTestCase::StateTransitionCallback (std::string context,
                                                   uint64_t imsi,
                                                   uint16_t cellId,
                                                   uint16_t rnti,
                                                   LteUeRrc::State oldState,
                                                   LteUeRrc::State newState)
{
  NS_LOG_FUNCTION (this << context << imsi << cellId << rnti
                        << oldState << newState);
  m_lastState = newState;
}


void
LteCellSelectionTestCase::InitialCellSelectionEndOkCallback (
  std::string context, uint64_t imsi, uint16_t cellId)
{
  NS_LOG_FUNCTION (this << context << imsi << cellId);
}


void
LteCellSelectionTestCase::InitialCellSelectionEndErrorCallback (
  std::string context, uint64_t imsi, uint16_t cellId)
{
  NS_LOG_FUNCTION (this << context << imsi << cellId);
}



} // end of namespace ns3
