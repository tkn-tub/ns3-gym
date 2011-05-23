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

#include "bug-772.h"

#include "ns3/mesh-helper.h"
#include "ns3/simulator.h"
#include "ns3/random-variable.h"
#include "ns3/mobility-helper.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/abort.h"
#include "ns3/udp-echo-helper.h"
#include "ns3/mobility-model.h"
#include "ns3/pcap-file.h"
#include "ns3/aodv-helper.h"
#include "ns3/v4ping-helper.h"
#include "ns3/nqos-wifi-mac-helper.h"
#include "ns3/config.h"
#include "ns3/on-off-helper.h"
#include "ns3/inet-socket-address.h"
#include "ns3/data-rate.h"
#include "ns3/packet-sink-helper.h"
#include <sstream>

/// Set to true to rewrite reference traces, leave false to run regression tests
const bool WRITE_VECTORS = false;

namespace ns3 {
namespace aodv {

//-----------------------------------------------------------------------------
// UdpChainTest
//-----------------------------------------------------------------------------
Bug772ChainTest::Bug772ChainTest (const char * const prefix, const char * const proto, Time t, uint32_t size) : 
  TestCase ("Bug 772 UDP/TCP chain regression test"),
  m_nodes (0),
  m_prefix (prefix),
  m_proto (proto),
  m_time (t),
  m_size (size),
  m_step (120)
{
}

Bug772ChainTest::~Bug772ChainTest ()
{
  delete m_nodes;
}

void
Bug772ChainTest::DoRun ()
{
  SeedManager::SetSeed (12345);

  CreateNodes ();
  CreateDevices ();

  Simulator::Stop (m_time);
  Simulator::Run ();
  Simulator::Destroy ();

  if (!WRITE_VECTORS) CheckResults ();

  delete m_nodes, m_nodes = 0;
}

void
Bug772ChainTest::CreateNodes ()
{
  m_nodes = new NodeContainer;
  m_nodes->Create (m_size);
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (m_step),
                                 "DeltaY", DoubleValue (0),
                                 "GridWidth", UintegerValue (m_size),
                                 "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (*m_nodes);
}

void
Bug772ChainTest::CreateDevices ()
{
  // 1. Setup WiFi
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifiMac.SetType ("ns3::AdhocWifiMac");
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  // This test suite output was originally based on YansErrorRateModel
  wifiPhy.SetErrorRateModel ("ns3::YansErrorRateModel");
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());
  WifiHelper wifi = WifiHelper::Default ();
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("OfdmRate6Mbps"), "RtsCtsThreshold", StringValue ("2200"));
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, *m_nodes); 

  // 2. Setup TCP/IP & AODV
  AodvHelper aodv; // Use default parameters here
  InternetStackHelper internetStack;
  internetStack.SetRoutingHelper (aodv);
  internetStack.Install (*m_nodes);
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  // 3. Setup UDP source and sink
  uint16_t port = 9; // Discard port (RFC 863)
  OnOffHelper onoff (m_proto, Address (InetSocketAddress (interfaces.GetAddress (m_size-1), port)));
  onoff.SetAttribute ("OnTime", RandomVariableValue (ConstantVariable (1)));
  onoff.SetAttribute ("OffTime", RandomVariableValue (ConstantVariable (0)));
  onoff.SetAttribute ("DataRate", DataRateValue (DataRate (64000)));
  onoff.SetAttribute ("PacketSize", UintegerValue (1200));
  ApplicationContainer app = onoff.Install (m_nodes->Get (0));
  app.Start (Seconds (1.0));
  app.Stop (m_time);
  PacketSinkHelper sink (m_proto, Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
  app = sink.Install (m_nodes->Get (m_size - 1));
  app.Start (Seconds (0.0));

  // 4. write PCAP on the first and last nodes only
  std::string prefix = (WRITE_VECTORS ? NS_TEST_SOURCEDIR : GetTempDir ()) + m_prefix;
  wifiPhy.EnablePcap (prefix, devices.Get (0));
  wifiPhy.EnablePcap (prefix, devices.Get (m_size-1));
}

void
Bug772ChainTest::CheckResults ()
{
  for (uint32_t i = 0; i < m_size; i += (m_size - 1) /*first and last nodes only*/)
    {
      std::ostringstream os1, os2;
      // File naming conventions are hard-coded here.
      os1 << NS_TEST_SOURCEDIR << m_prefix << "-" << i << "-0.pcap";
      os2 << GetTempDir () << m_prefix << "-" << i << "-0.pcap";

      uint32_t sec (0), usec (0);
      bool diff = PcapFile::Diff (os1.str (), os2.str (), sec, usec);
      NS_TEST_EXPECT_MSG_EQ (diff, false, "PCAP traces " << os1.str () << " and " << os2.str ()
                                                         << " differ starting from " << sec << " s " << usec << " us");
    }
}

}
}
