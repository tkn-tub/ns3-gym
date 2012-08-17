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
 * Authors: Pavel Boyko <boyko@iitp.ru>
 */

#include "tc-regression-test.h"
#include "ns3/simulator.h"
#include "ns3/random-variable-stream.h"
#include "ns3/rng-seed-manager.h"
#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/pcap-test.h"
#include "ns3/pcap-file.h"
#include "ns3/olsr-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/abort.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/mobility-helper.h"
#include "ns3/nqos-wifi-mac-helper.h"

namespace ns3
{
namespace olsr
{

const char * const TcRegressionTest::PREFIX = "olsr-tc-regression-test";

TcRegressionTest::TcRegressionTest() : 
  TestCase ("Test OLSR Topology Control message generation"),
  m_time (Seconds (20)),
  m_step (120)
{
}

TcRegressionTest::~TcRegressionTest()
{
}

void
TcRegressionTest::DoRun ()
{
  RngSeedManager::SetSeed (12345);
  RngSeedManager::SetRun (7);
  CreateNodes ();

  Simulator::Stop (m_time);
  Simulator::Run ();
  Simulator::Destroy ();

  CheckResults ();
}

void
TcRegressionTest::CreateNodes ()
{
  // create 3 nodes
  NodeContainer c;
  c.Create (3);

  // place nodes in the chain
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (m_step),
                                 "DeltaY", DoubleValue (0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (c);

  // install TCP/IP & OLSR
  OlsrHelper olsr;
  InternetStackHelper internet;
  internet.SetRoutingHelper (olsr);
  internet.Install (c);
  int64_t streamsUsed = olsr.AssignStreams (c, 0);
  NS_TEST_EXPECT_MSG_EQ (streamsUsed, 3, "Should have assigned 2 streams");

  // create wifi channel & devices
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifiMac.SetType ("ns3::AdhocWifiMac");
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());
  // This test suite output was originally based on YansErrorRateModel 
  wifiPhy.SetErrorRateModel ("ns3::YansErrorRateModel");
  WifiHelper wifi = WifiHelper::Default ();
  wifi.SetStandard (WIFI_PHY_STANDARD_80211a);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("OfdmRate6Mbps"), "RtsCtsThreshold", StringValue ("2200"));
  NetDeviceContainer nd = wifi.Install (wifiPhy, wifiMac, c); 

  // setup IP addresses
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  ipv4.Assign (nd);

  // setup PCAP traces
  wifiPhy.EnablePcapAll (CreateTempDirFilename(PREFIX));
}

void
TcRegressionTest::CheckResults ()
{
  for (uint32_t i = 0; i < 3; ++i)
    {
      NS_PCAP_TEST_EXPECT_EQ (PREFIX << "-" << i << "-1.pcap");
    }
}

}
}
