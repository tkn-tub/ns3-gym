/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 The Boeing Company
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
 */

#include "ns3/log.h"
#include "ns3/abort.h"
#include "ns3/test.h"
#include "ns3/pcap-file.h"
#include "ns3/config.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/data-rate.h"
#include "ns3/inet-socket-address.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/tcp-socket-factory.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/yans-wifi-phy.h"
#include "ns3/wifi-net-device.h"
#include "ns3/mobility-helper.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/nqos-wifi-mac-helper.h"
#include "ns3/simulator.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Ns3WifiPropagationLossModelsTest");

// ===========================================================================
// This is a simple test to validate propagation loss models of ns-3 wifi.
//
// The basic configuration is,
//
//                        node 0           node 1
//                    +------------+   +------------+
//                    |  ns-3 UDP  |   |  ns-3 UDP  |
//                    +------------+   +------------+
//                    |  10.1.1.1  |   |  10.1.1.2  |
//                    +------------+   +------------+
//                    |    wifi    |   |    wifi    |
//                    +------------+   +------------+
//                           |                |
//                        (((*)))          (((*)))
//                 
//                           |<-- distance -->|
//
//
// We vary the propagation loss model and the distance between the nodes, 
// looking at the received power and SNR for a packet sent between them.  We
// compare the found values with values found in an "authoritative source."
// ===========================================================================
//
class Ns3FriisPropagationLossModelTestCase : public TestCase
{
public:
  Ns3FriisPropagationLossModelTestCase ();
  virtual ~Ns3FriisPropagationLossModelTestCase ();

private:
  virtual bool DoRun (void);

  void SendPacket (uint32_t i, Ptr<Socket> socket, uint32_t size);
  void Receive (Ptr<Packet> p, double snr, WifiMode mode, enum WifiPreamble preamble);

  uint32_t m_gotCallbacks;

  Ptr<Node> m_receiver;
  uint32_t m_vectorIndex;

  typedef struct {
    Vector m_position;
    double m_snr;
    double m_tolerance;
  } TestVector;

  TestVectors<TestVector> m_testVectors;
};

Ns3FriisPropagationLossModelTestCase::Ns3FriisPropagationLossModelTestCase ()
  : TestCase ("Check to see that the ns-3 Friis propagation loss model provides correct SNR values"),
    m_gotCallbacks (false), m_receiver (0), m_vectorIndex (0), m_testVectors ()
{
}

Ns3FriisPropagationLossModelTestCase::~Ns3FriisPropagationLossModelTestCase ()
{
}

void 
Ns3FriisPropagationLossModelTestCase::Receive (Ptr<Packet> p, double snr, WifiMode mode, enum WifiPreamble preamble)
{
  TestVector testVector = m_testVectors.Get (m_vectorIndex++);
  if (GetErrorStatus () == false)
    {
      NS_TEST_EXPECT_MSG_EQ_TOL (snr, testVector.m_snr, testVector.m_tolerance, "Got unexpected SNR value");
    }
  ++m_gotCallbacks;
}

void 
Ns3FriisPropagationLossModelTestCase::SendPacket (uint32_t i, Ptr<Socket> socket, uint32_t size)
{
  TestVector testVector = m_testVectors.Get (i);
  m_receiver->GetObject<ConstantPositionMobilityModel> ()->SetPosition (testVector.m_position);
  socket->Send (Create<Packet> (size));
}

bool
Ns3FriisPropagationLossModelTestCase::DoRun (void)
{
  //
  // We want to test the propagation loss model calculations at a few chosen 
  // distances and compare the results to those we have manually calculated
  // according to the model documentation.  The following "TestVector" objects
  // will drive the test.
  //
  // For example, the first test vector provides a position to which the 
  // receiver node will be moved prior to transmitting a packet.  It also
  // specifies that when the packet is received, the SNR shold be found
  // to be 1129.93 +- 0.005 in the ReceiveOkCallback.
  //
  TestVector testVector;

  testVector.m_position = Vector (100, 0, 0);
  testVector.m_snr = 1128.93;
  testVector.m_tolerance = 0.005;
  m_testVectors.Add (testVector);

  testVector.m_position = Vector (500, 0, 0);
  testVector.m_snr = 45.1571;
  testVector.m_tolerance = 0.00005;
  m_testVectors.Add (testVector);

  testVector.m_position = Vector (1000, 0, 0);
  testVector.m_snr = 11.2893;
  testVector.m_tolerance = 0.00005;
  m_testVectors.Add (testVector);

  testVector.m_position = Vector (2000, 0, 0);
  testVector.m_snr = 2.82232;
  testVector.m_tolerance = 0.000005;
  m_testVectors.Add (testVector);

  //
  // Disable fragmentation for frames shorter than 2200 bytes.
  //
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));

  //
  // Turn off RTS/CTS for frames shorter than 2200 bytes.
  //
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
  
  //
  // Create the two nodes in the system.  Data will be sent from node zero to
  // node one.
  //
  NodeContainer nodes;
  nodes.Create (2);

  //
  // Save a Ptr<Node> to the receiver node so we can get at its mobility model
  // and change its position (distance) later.
  //
  m_receiver = nodes.Get (1);
	
  //
  // Use the regular WifiHelper to orchestrate hooking the various pieces of 
  // the wifi system together.  Tell it that we want to use an 802.11b phy.
  //
  WifiHelper wifi;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);

  //
  // Create a physical layer helper and tell it we don't want any receiver
  // gain.
  //
  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  wifiPhy.Set ("RxGain", DoubleValue (0) ); 

  //
  // Create the channel helper and tell it that signals will be moving at the
  // speed of light.
  //
  YansWifiChannelHelper wifiChannel ;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");

  //
  // The propagation loss model is one of our independent variables in the 
  // test.
  //
  wifiChannel.AddPropagationLoss ("ns3::FriisPropagationLossModel", 
                                  "Lambda", DoubleValue (0.125), 
                                  "SystemLoss", DoubleValue (1.));

  //
  // Create a yans wifi channel and tell the phy helper to use it.
  //
  wifiPhy.SetChannel (wifiChannel.Create ());
	
  //
  // Create a non-quality-of-service mac layer and set it to ad-hoc mode.
  //
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode", StringValue ("wifib-1mbs"),
                                "ControlMode",StringValue ("wifib-1mbs"));
  wifiMac.SetType ("ns3::AdhocWifiMac");

  //
  // Create the wifi devices.
  //
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, nodes);

  //
  // We need to reach down into the receiving wifi device's phy layer and hook
  // the appropriate trace event to get the snr.  This isn't one of the usual
  // events so it takes some poking around to get there from here.
  //
  Ptr<YansWifiPhy> phy = devices.Get (1)->GetObject<WifiNetDevice> ()->GetPhy ()->GetObject<YansWifiPhy> ();
  phy->SetReceiveOkCallback (MakeCallback (&Ns3FriisPropagationLossModelTestCase::Receive, this));

  //
  // Add mobility models to both nodes.  This is used to place the two nodes a 
  // fixed distance apart.  Node zero (the sender) is always at the origin and
  // Node one (the receiver) is moved along the x-axis to a given distance from
  // the origin.  This distance is the second independent variable in our test.
  //
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0., 0., 0.));
  positionAlloc->Add (Vector (0, 0., 0.));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);
	
  //
  // In order to use UDP sockets, we need to install the ns-3 internet stack
  // on our nodes.
  //
  InternetStackHelper internet;
  internet.Install (nodes);
	
  //
  // Assign IP addresses to our nodes.  The source node is going to end up
  // as 10.1.1.1 and the destination will be 10.1.1.2
  //
  Ipv4AddressHelper addresses;
  addresses.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = addresses.Assign (devices);

  //
  // The destination is the wifi device on node one.
  //
  InetSocketAddress destaddr = InetSocketAddress (interfaces.GetAddress (1), 80);
	
  //
  // We just want to send packets from the source to the destination node, so
  // the simplest thing is to cook something up manually.
  //
  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  Ptr<Socket> dest = Socket::CreateSocket (nodes.Get (1), tid);
  dest->Bind (destaddr);
	
  Ptr<Socket> source = Socket::CreateSocket (nodes.Get (0), tid);
  source->Connect (destaddr);

  //
  // Schedule the packet sends, one packet per simulated second.
  //
  for (uint32_t i = 0; i < m_testVectors.GetN (); ++i)
    {
      Time t = Seconds (1. * i);
      Simulator::Schedule (t, &Ns3FriisPropagationLossModelTestCase::SendPacket, this,  i, source, 1000);
    }
	
  Simulator::Stop (Seconds(1. * m_testVectors.GetN ()));
  Simulator::Run ();

  source->Close ();
  source = 0;

  dest->Close ();
  dest = 0;

  m_receiver = 0;

  Simulator::Destroy ();
  
  //
  // If we've already reported an error, just leave it at that.
  //
  if (GetErrorStatus () == false)
    {
      NS_TEST_ASSERT_MSG_EQ (m_gotCallbacks, m_testVectors.GetN (), "Did not get expected number of ReceiveOkCallbacks");
    }

  return GetErrorStatus ();
}

class Ns3LogDistancePropagationLossModelTestCase : public TestCase
{
public:
  Ns3LogDistancePropagationLossModelTestCase ();
  virtual ~Ns3LogDistancePropagationLossModelTestCase ();

private:
  virtual bool DoRun (void);

  void SendPacket (uint32_t i, Ptr<Socket> socket, uint32_t size);
  void Receive (Ptr<Packet> p, double snr, WifiMode mode, enum WifiPreamble preamble);

  uint32_t m_gotCallbacks;

  Ptr<Node> m_receiver;
  uint32_t m_vectorIndex;

  typedef struct {
    Vector m_position;
    double m_snr;
    double m_tolerance;
  } TestVector;

  TestVectors<TestVector> m_testVectors;
};

Ns3LogDistancePropagationLossModelTestCase::Ns3LogDistancePropagationLossModelTestCase ()
  : TestCase ("Check to see that the ns-3 Log Distance propagation loss model provides correct SNR values"),
    m_gotCallbacks (false), m_receiver (0), m_vectorIndex (0), m_testVectors ()
{
}

Ns3LogDistancePropagationLossModelTestCase::~Ns3LogDistancePropagationLossModelTestCase ()
{
}

void 
Ns3LogDistancePropagationLossModelTestCase::Receive (Ptr<Packet> p, double snr, WifiMode mode, enum WifiPreamble preamble)
{
  TestVector testVector = m_testVectors.Get (m_vectorIndex++);
  if (GetErrorStatus () == false)
    {
      NS_TEST_EXPECT_MSG_EQ_TOL (snr, testVector.m_snr, testVector.m_tolerance, "Got unexpected SNR value");
    }
  ++m_gotCallbacks;
}

void 
Ns3LogDistancePropagationLossModelTestCase::SendPacket (uint32_t i, Ptr<Socket> socket, uint32_t size)
{
  TestVector testVector = m_testVectors.Get (i);
  m_receiver->GetObject<ConstantPositionMobilityModel> ()->SetPosition (testVector.m_position);
  socket->Send (Create<Packet> (size));
}

bool
Ns3LogDistancePropagationLossModelTestCase::DoRun (void)
{
  //
  // We want to test the propagation loss model calculations at a few chosen 
  // distances and compare the results to those we have manually calculated
  // according to the model documentation.  The following "TestVector" objects
  // will drive the test.
  //
  // For example, the first test vector provides a position to which the 
  // receiver node will be moved prior to transmitting a packet.  It also
  // specifies that when the packet is received, the SNR shold be found
  // to be 1129.93 +- 0.005 in the ReceiveOkCallback.
  //
  TestVector testVector;

  testVector.m_position = Vector (10, 0, 0);
  testVector.m_snr = 11289.3;
  testVector.m_tolerance = 0.05;
  m_testVectors.Add (testVector);

  testVector.m_position = Vector (20, 0, 0);
  testVector.m_snr = 1411.16;
  testVector.m_tolerance = 0.005;
  m_testVectors.Add (testVector);

  testVector.m_position = Vector (40, 0, 0);
  testVector.m_snr = 176.407;
  testVector.m_tolerance = 0.0005;
  m_testVectors.Add (testVector);

  testVector.m_position = Vector (80, 0, 0);
  testVector.m_snr = 22.0494;
  testVector.m_tolerance = 0.00005;
  m_testVectors.Add (testVector);

  //
  // Disable fragmentation for frames shorter than 2200 bytes.
  //
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));

  //
  // Turn off RTS/CTS for frames shorter than 2200 bytes.
  //
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
  
  //
  // Create the two nodes in the system.  Data will be sent from node zero to
  // node one.
  //
  NodeContainer nodes;
  nodes.Create (2);

  //
  // Save a Ptr<Node> to the receiver node so we can get at its mobility model
  // and change its position (distance) later.
  //
  m_receiver = nodes.Get (1);
	
  //
  // Use the regular WifiHelper to orchestrate hooking the various pieces of 
  // the wifi system together.  Tell it that we want to use an 802.11b phy.
  //
  WifiHelper wifi;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);

  //
  // Create a physical layer helper and tell it we don't want any receiver
  // gain.
  //
  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  wifiPhy.Set ("RxGain", DoubleValue (0) ); 

  //
  // Create the channel helper and tell it that signals will be moving at the
  // speed of light.
  //
  YansWifiChannelHelper wifiChannel ;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");

  //
  // The propagation loss model is one of our independent variables in the 
  // test.
  //
  wifiChannel.AddPropagationLoss ("ns3::LogDistancePropagationLossModel", 
                                  "Exponent", DoubleValue(3), 
                                  "ReferenceLoss", DoubleValue(40.045997));

  //
  // Create a yans wifi channel and tell the phy helper to use it.
  //
  wifiPhy.SetChannel (wifiChannel.Create ());
	
  //
  // Create a non-quality-of-service mac layer and set it to ad-hoc mode.
  //
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode", StringValue ("wifib-1mbs"),
                                "ControlMode",StringValue ("wifib-1mbs"));
  wifiMac.SetType ("ns3::AdhocWifiMac");

  //
  // Create the wifi devices.
  //
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, nodes);

  //
  // We need to reach down into the receiving wifi device's phy layer and hook
  // the appropriate trace event to get the snr.  This isn't one of the usual
  // events so it takes some poking around to get there from here.
  //
  Ptr<YansWifiPhy> phy = devices.Get (1)->GetObject<WifiNetDevice> ()->GetPhy ()->GetObject<YansWifiPhy> ();
  phy->SetReceiveOkCallback (MakeCallback (&Ns3LogDistancePropagationLossModelTestCase::Receive, this));

  //
  // Add mobility models to both nodes.  This is used to place the two nodes a 
  // fixed distance apart.  Node zero (the sender) is always at the origin and
  // Node one (the receiver) is moved along the x-axis to a given distance from
  // the origin.  This distance is the second independent variable in our test.
  //
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0., 0., 0.));
  positionAlloc->Add (Vector (0, 0., 0.));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);
	
  //
  // In order to use UDP sockets, we need to install the ns-3 internet stack
  // on our nodes.
  //
  InternetStackHelper internet;
  internet.Install (nodes);
	
  //
  // Assign IP addresses to our nodes.  The source node is going to end up
  // as 10.1.1.1 and the destination will be 10.1.1.2
  //
  Ipv4AddressHelper addresses;
  addresses.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = addresses.Assign (devices);

  //
  // The destination is the wifi device on node one.
  //
  InetSocketAddress destaddr = InetSocketAddress (interfaces.GetAddress (1), 80);
	
  //
  // We just want to send packets from the source to the destination node, so
  // the simplest thing is to cook something up manually.
  //
  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  Ptr<Socket> dest = Socket::CreateSocket (nodes.Get (1), tid);
  dest->Bind (destaddr);
	
  Ptr<Socket> source = Socket::CreateSocket (nodes.Get (0), tid);
  source->Connect (destaddr);

  //
  // Schedule the packet sends, one packet per simulated second.
  //
  for (uint32_t i = 0; i < m_testVectors.GetN (); ++i)
    {
      Time t = Seconds (1. * i);
      Simulator::Schedule (t, &Ns3LogDistancePropagationLossModelTestCase::SendPacket, this,  i, source, 1000);
    }
	
  Simulator::Stop (Seconds(1. * m_testVectors.GetN ()));
  Simulator::Run ();

  source->Close ();
  source = 0;

  dest->Close ();
  dest = 0;

  m_receiver = 0;

  Simulator::Destroy ();

  //
  // If we've already reported an error, just leave it at that.
  //
  if (GetErrorStatus () == false)
    {
      NS_TEST_ASSERT_MSG_EQ (m_gotCallbacks, m_testVectors.GetN (), "Did not get expected number of ReceiveOkCallbacks");
    }

  return GetErrorStatus ();
}

class Ns3WifiPropagationLossModelsTestSuite : public TestSuite
{
public:
  Ns3WifiPropagationLossModelsTestSuite ();
};

Ns3WifiPropagationLossModelsTestSuite::Ns3WifiPropagationLossModelsTestSuite ()
  : TestSuite ("ns3-wifi-propagation-loss-models", SYSTEM)
{
  AddTestCase (new Ns3FriisPropagationLossModelTestCase);
  AddTestCase (new Ns3LogDistancePropagationLossModelTestCase);
}

Ns3WifiPropagationLossModelsTestSuite ns3WifiPropagationLossModelsTestSuite;
