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
 * Authors: Kirill Andreev  <andreev@iitp.ru>
 */

#include "ns3/mesh-helper.h"
#include "ns3/simulator.h"
#include "ns3/random-variable.h"
#include "ns3/mobility-helper.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/abort.h"
#include "ns3/udp-echo-helper.h"
#include "ns3/mobility-model.h"
#include <sstream>
#include "hwmp-simplest-regression.h"

using namespace ns3;

/// Set to true to rewrite reference traces, leave false to run regression test
const bool WRITE_VECTORS = false;
/// Unique PCAP file name prefix
const char * const PREFIX = "hwmp-simplest-regression-test";


HwmpSimplestRegressionTest::HwmpSimplestRegressionTest () : TestCase ("Simplest HWMP regression test"),
                                                            m_nodes (0),
                                                            m_time (Seconds (15))
{
}

HwmpSimplestRegressionTest::~HwmpSimplestRegressionTest ()
{
  delete m_nodes;
}
void
HwmpSimplestRegressionTest::DoRun ()
{
  SeedManager::SetSeed (12345);
  CreateNodes ();
  CreateDevices ();
  InstallApplications ();

  Simulator::Stop (m_time);
  Simulator::Run ();
  Simulator::Destroy ();

  if (!WRITE_VECTORS) CheckResults ();

  delete m_nodes, m_nodes = 0;
}
void
HwmpSimplestRegressionTest::CreateNodes ()
{
  m_nodes = new NodeContainer;
  m_nodes->Create (2);
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (1 /*meter*/),
                                 "DeltaY", DoubleValue (0),
                                 "GridWidth", UintegerValue (2),
                                 "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (*m_nodes);
  Simulator::Schedule (Seconds (10.0), &HwmpSimplestRegressionTest::ResetPosition, this);
}
void
HwmpSimplestRegressionTest::ResetPosition ()
{
  Ptr<Object> object = m_nodes->Get (1);
  Ptr<MobilityModel> model = object->GetObject<MobilityModel> ();
  if (model == 0)
    {
      return;
    }
  model->SetPosition (Vector (9000, 0, 0));
}
void
HwmpSimplestRegressionTest::InstallApplications ()
{
  UdpEchoServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (m_nodes->Get (0));
  serverApps.Start (Seconds (0.0));
  serverApps.Stop (m_time);
  UdpEchoClientHelper echoClient (m_interfaces.GetAddress (0), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (300));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.05)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (100));
  ApplicationContainer clientApps = echoClient.Install (m_nodes->Get (1));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (m_time);
}
void
HwmpSimplestRegressionTest::CreateDevices ()
{
  // 1. setup WiFi
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());
  // 2. setup mesh
  MeshHelper mesh = MeshHelper::Default ();
  mesh.SetStackInstaller ("ns3::Dot11sStack");
  mesh.SetMacType ("RandomStart", TimeValue (Seconds (0.1)));
  mesh.SetNumberOfInterfaces (1);
  NetDeviceContainer meshDevices = mesh.Install (wifiPhy, *m_nodes);
  // 3. setup TCP/IP
  InternetStackHelper internetStack;
  internetStack.Install (*m_nodes);
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  m_interfaces = address.Assign (meshDevices);
  // 4. write PCAP if needed
  std::string prefix = (WRITE_VECTORS ? NS_TEST_SOURCEDIR : std::string (GetTempDir ())) + PREFIX;
  wifiPhy.EnablePcapAll (prefix);
}

void
HwmpSimplestRegressionTest::CheckResults ()
{
  for (int i = 0; i < 2; ++i)
    {
      std::ostringstream os1, os2;
      // File naming conventions are hard-coded here.
      os1 << NS_TEST_SOURCEDIR << PREFIX << "-" << i << "-1.pcap";
      os2 << GetTempDir () << PREFIX << "-" << i << "-1.pcap";

      uint32_t sec (0), usec (0);
      bool diff = PcapFile::Diff (os1.str (), os2.str (), sec, usec); // TODO support default PcapWriter snap length here
      NS_TEST_EXPECT_MSG_EQ (diff, false, "PCAP traces " << os1.str () << " and " << os2.str ()
                                                         << " differ starting from " << sec << " s " << usec << " us");
    }
}

