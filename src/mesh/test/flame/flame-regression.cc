/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 */
#include "ns3/mesh-helper.h"
#include "ns3/simulator.h"
#include "ns3/random-variable-stream.h"
#include "ns3/rng-seed-manager.h"
#include "ns3/mobility-helper.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/abort.h"
#include "ns3/pcap-test.h"
#include "ns3/udp-echo-helper.h"
#include "ns3/mobility-model.h"
#include <sstream>

#include "flame-regression.h"

using namespace ns3;

/// Unique PCAP file name prefix
const char * const PREFIX = "flame-regression-test";


FlameRegressionTest::FlameRegressionTest () : TestCase ("FLAME regression test"),
                                              m_nodes (0),
                                              m_time (Seconds (10))
{
}

FlameRegressionTest::~FlameRegressionTest ()
{
  delete m_nodes;
}

void
FlameRegressionTest::DoRun ()
{
  RngSeedManager::SetSeed (12345);
  RngSeedManager::SetRun (7);
  CreateNodes ();
  CreateDevices ();
  InstallApplications ();

  Simulator::Stop (m_time);
  Simulator::Run ();
  Simulator::Destroy ();

  CheckResults ();

  delete m_nodes, m_nodes = 0;
}

void
FlameRegressionTest::CreateNodes ()
{
  m_nodes = new NodeContainer;
  m_nodes->Create (3);
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (150),
                                 "DeltaY", DoubleValue (0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (*m_nodes);
}

void
FlameRegressionTest::CreateDevices ()
{
  int64_t streamsUsed = 0;
  // 1. setup WiFi
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  // This test suite output was originally based on YansErrorRateModel
  wifiPhy.SetErrorRateModel ("ns3::YansErrorRateModel"); 
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  Ptr<YansWifiChannel> chan = wifiChannel.Create ();
  wifiPhy.SetChannel (chan);
  // 2. setup mesh
  MeshHelper mesh = MeshHelper::Default ();
  mesh.SetStackInstaller ("ns3::FlameStack");
  mesh.SetMacType ("RandomStart", TimeValue (Seconds (0.1)));
  mesh.SetNumberOfInterfaces (1);
  NetDeviceContainer meshDevices = mesh.Install (wifiPhy, *m_nodes);
  // Three devices, seven streams per device
  streamsUsed += mesh.AssignStreams (meshDevices, streamsUsed);
  NS_TEST_ASSERT_MSG_EQ (streamsUsed, (meshDevices.GetN () * 7), "Stream assignment unexpected value");
  // No further streams used in the default wifi channel configuration
  streamsUsed += wifiChannel.AssignStreams (chan, streamsUsed);
  NS_TEST_ASSERT_MSG_EQ (streamsUsed, (meshDevices.GetN () * 7), "Stream assignment unexpected value");
  // 3. setup TCP/IP
  InternetStackHelper internetStack;
  internetStack.Install (*m_nodes);
  streamsUsed += internetStack.AssignStreams (*m_nodes, streamsUsed);
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  m_interfaces = address.Assign (meshDevices);
  // 4. write PCAP if needed
  wifiPhy.EnablePcapAll (CreateTempDirFilename (PREFIX));

}
void
FlameRegressionTest::InstallApplications ()
{
  UdpEchoServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (m_nodes->Get (0));
  serverApps.Start (Seconds (0.0));
  serverApps.Stop (m_time);
  UdpEchoClientHelper echoClient (m_interfaces.GetAddress (0), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (300));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.1)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (20));
  ApplicationContainer clientApps = echoClient.Install (m_nodes->Get (2));
  clientApps.Start (Seconds (1.0));
  clientApps.Stop (m_time);
}

void
FlameRegressionTest::CheckResults ()
{
  for (int i = 0; i < 3; ++i)
    {
      NS_PCAP_TEST_EXPECT_EQ (PREFIX << "-" << i << "-1.pcap");
    }
}

