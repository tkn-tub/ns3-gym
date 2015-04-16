/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/* This program is free software; you can redistribute it and/or modify
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 *
 */

// OLSR was observed to not converge in simple 3-nodes varying topology.
// https://www.nsnam.org/bugzilla/show_bug.cgi?id=780
// tcpdump -r bug780-0-0.pcap -nn -tt icmp | wc  
// should show about 395 packets; there is a ping outage from time
// 123-127 due to the mobility.

#include <fstream>
#include <iostream>

#include "ns3/test.h"
#include "ns3/olsr-helper.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/wifi-helper.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/nqos-wifi-mac-helper.h"
#include "ns3/constant-velocity-mobility-model.h"
#include "ns3/mobility-helper.h"
#include "ns3/olsr-routing-protocol.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/log.h"
#include "ns3/config.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-interface-container.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/v4ping-helper.h"
#include "ns3/pcap-file.h"
#include "ns3/pcap-test.h"
#include "ns3/rng-seed-manager.h"

#include "bug780-test.h"

namespace ns3
{
namespace olsr
{

const char * const Bug780Test::PREFIX = "bug780";

static void
SetVelocity (Ptr<Node> node, Vector vel)
{
  Ptr<ConstantVelocityMobilityModel> mobility =
    node->GetObject<ConstantVelocityMobilityModel> ();
  mobility->SetVelocity (vel);
}


Bug780Test::Bug780Test() : 
  TestCase ("Test OLSR bug 780"),
  m_time (Seconds (200.0))
{
}

Bug780Test::~Bug780Test()
{
}

void
Bug780Test::DoRun ()
{
  RngSeedManager::SetSeed (12345);
  RngSeedManager::SetRun (12345);
  CreateNodes ();

  Simulator::Stop (m_time);
  Simulator::Run ();
  Simulator::Destroy ();

  CheckResults ();  
}

void
Bug780Test::CreateNodes (void)
{
  int nWifis = 3;
  double SimTime = 200.0;
  std::string phyMode ("DsssRate1Mbps");
  int64_t streamsUsed = 0;

  //sending one packets per sec
  // Fix non-unicast data rate to be the same as that of unicast
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold",
                      StringValue ("400"));

  NodeContainer adhocNodes;
  adhocNodes.Create (nWifis);

  WifiHelper wifi;
  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::FriisPropagationLossModel");
  Ptr<YansWifiChannel> chan = wifiChannel.Create ();
  wifiPhy.SetChannel (chan);

  // Add a non-QoS upper mac, and disable rate control
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode",StringValue (phyMode),
                                "ControlMode",StringValue (phyMode));

  //set the tx range to 300
  wifiPhy.Set ("TxPowerStart",DoubleValue (-0.1615));
  wifiPhy.Set ("TxPowerEnd", DoubleValue (-0.1615));

  // Set it to adhoc mode
  wifiMac.SetType ("ns3::AdhocWifiMac");
  NetDeviceContainer adhocDevices = wifi.Install (wifiPhy, wifiMac, adhocNodes);

  // Assign fixed stream numbers to wifi and channel random variables
  streamsUsed += wifi.AssignStreams (adhocDevices, streamsUsed);
  // Assign 6 streams per Wifi device
  NS_TEST_ASSERT_MSG_EQ (streamsUsed, (adhocDevices.GetN () * 6), "Stream assignment mismatch");
  streamsUsed += wifiChannel.AssignStreams (chan, streamsUsed);
  // Assign 0 additional streams per channel for this configuration 
  NS_TEST_ASSERT_MSG_EQ (streamsUsed, (adhocDevices.GetN () * 6), "Stream assignment mismatch");

  OlsrHelper olsr;

  InternetStackHelper internet;
  internet.SetRoutingHelper (olsr);
  internet.Install (adhocNodes);
  // Assign 3 streams per node to internet stack for this configuration
  streamsUsed += internet.AssignStreams (adhocNodes, streamsUsed);
  NS_TEST_ASSERT_MSG_EQ (streamsUsed, (adhocDevices.GetN () * 6) + (adhocNodes.GetN () * 3), "Stream assignment mismatch");
  // Olsr uses one additional stream per wifi device for this configuration
  streamsUsed += olsr.AssignStreams (adhocNodes, 0);
  NS_TEST_ASSERT_MSG_EQ (streamsUsed, ((adhocDevices.GetN () * 6) + (adhocDevices.GetN () * 3) + nWifis), "Should have assigned 3 streams");

  Ipv4AddressHelper addressAdhoc;
  addressAdhoc.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer adhocInterfaces;
  adhocInterfaces = addressAdhoc.Assign (adhocDevices);

  MobilityHelper mobilityAdhoc;

  Ptr<ListPositionAllocator> positionAlloc_Adhoc =
    CreateObject<ListPositionAllocator>();
  double distance = 0.0;
  for (uint32_t i = 0; i <= adhocNodes.GetN (); i++)
    {
      positionAlloc_Adhoc->Add (Vector (distance,0.0,0.0));
      distance += 250.0;
    }

  mobilityAdhoc.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  mobilityAdhoc.SetPositionAllocator (positionAlloc_Adhoc);
  mobilityAdhoc.Install (adhocNodes);

  //At 50 sec node 3 moves towards node 2
  Simulator::Schedule (Seconds (50.0), &SetVelocity, adhocNodes.Get (2),Vector (-5.0,0.0,0.0));
  //AT 100 sec set node 3 with zero velocity
  Simulator::Schedule (Seconds (100.0), &SetVelocity, adhocNodes.Get (2),Vector (0.0,0.0,0.0));
  //Move node2 away from node 3
  Simulator::Schedule (Seconds (100.0), &SetVelocity, adhocNodes.Get (1),Vector (5.0,0.0,0.0));
  //AT 150 sec set node 2 with zero velocity
  Simulator::Schedule (Seconds (150.0), &SetVelocity, adhocNodes.Get (1), Vector (0.0,0.0,0.0));


  // Ping 10.1.1.1 -> 10.1.1.2
  V4PingHelper ping (adhocInterfaces.GetAddress (1));
  ping.SetAttribute ("Verbose", BooleanValue (true));

  ApplicationContainer p = ping.Install (adhocNodes.Get (0));
  p.Start (Seconds (50));
  p.Stop (Seconds (SimTime) - Seconds (0.001));

  // pcap
  wifiPhy.EnablePcapAll (CreateTempDirFilename (PREFIX));
}

void
Bug780Test::CheckResults ()
{
  for (uint32_t i = 0; i < 2; ++i)
    {
      NS_PCAP_TEST_EXPECT_EQ (PREFIX << "-" << i << "-0.pcap");
    }
}

} // namespace olsr
} // namespace ns3

