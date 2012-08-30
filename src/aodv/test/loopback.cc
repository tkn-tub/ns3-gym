/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 IITP RAS
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

#include "loopback.h"
#include "ns3/simulator.h"
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
#include "ns3/v4ping.h"
#include "ns3/nqos-wifi-mac-helper.h"
#include "ns3/config.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/names.h"
#include <sstream>

namespace ns3
{
namespace aodv
{

static uint32_t g_count (0);

static void 
PingRtt (std::string context, Time rtt)
{
  g_count++;
}

void
LoopbackTestCase::DoRun ()
{
  NodeContainer nodes;
  nodes.Create (1);
  Ptr<MobilityModel> m = CreateObject<ConstantPositionMobilityModel> ();
  m->SetPosition (Vector (0, 0, 0));
  nodes.Get (0)->AggregateObject (m);
  // Setup WiFi
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifiMac.SetType ("ns3::AdhocWifiMac");
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());
  WifiHelper wifi = WifiHelper::Default ();
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("OfdmRate6Mbps"), "RtsCtsThreshold", StringValue ("2200"));
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, nodes); 

  // Setup TCP/IP & AODV
  AodvHelper aodv; // Use default parameters here
  InternetStackHelper internetStack;
  internetStack.SetRoutingHelper (aodv);
  internetStack.Install (nodes);
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  // Setup ping
  Ptr<V4Ping> ping = CreateObject<V4Ping> ();
  ping->SetAttribute ("Remote", Ipv4AddressValue (Ipv4Address::GetLoopback ()));
  nodes.Get (0)->AddApplication (ping);
  ping->SetStartTime (Seconds (0));
  ping->SetStopTime (Seconds (4));
  Names::Add ("ping", ping);
  Config::Connect ("/Names/ping/Rtt", MakeCallback (&PingRtt));

  // Run 
  Simulator::Stop (Seconds (5));
  Simulator::Run ();
  Simulator::Destroy ();

  // Check that 4 packets delivered
  NS_TEST_ASSERT_MSG_EQ (g_count, 4, "Exactly 4 ping replies must be delivered.");
}

}
}
