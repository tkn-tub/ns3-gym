/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 MIRKO BANCHI
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
 * Author: Mirko Banchi <mk.banchi@gmail.com>
 */
#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"

//This is a simple example in order to show how 802.11n frame aggregation feature (A-MSDU) works.
//
//Network topology:
// 
//  Wifi 192.168.1.0
// 
//             AP
//   *    *    *
//   |    |    |
//   n1   n2   n3 
//
//Packets in this simulation aren't marked with a QosTag so they are considered
//belonging to BestEffort Access Class (AC_BE).

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SimpleWifiFrameAggregation");

int main (int argc, char *argv[])
{
  //LogComponentEnable ("EdcaTxopN", LOG_LEVEL_DEBUG);
  LogComponentEnable ("MsduAggregator", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  
  uint32_t nWifi = 1;
  CommandLine cmd;
  cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
  cmd.Parse (argc,argv);

  NodeContainer wifiNodes;
  wifiNodes.Create (2);
  NodeContainer wifiApNode;
  wifiApNode.Create (1);
 
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

  WifiHelper wifi = WifiHelper::Default ();
  QosWifiMacHelper mac = QosWifiMacHelper::Default ();
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager", "FragmentationThreshold", UintegerValue (2500));

  Ssid ssid = Ssid ("ns-3-802.11n");
  mac.SetType ("ns3::QstaWifiMac", 
    "Ssid", SsidValue (ssid),
    "ActiveProbing", BooleanValue (false));
  mac.SetMsduAggregatorForAc (AC_BE, "ns3::MsduStandardAggregator", 
                                     "MaxAmsduSize", UintegerValue (3839));
  
  NetDeviceContainer staDevices;
  staDevices = wifi.Install (phy, mac, wifiNodes);
  
  mac.SetType ("ns3::QapWifiMac", 
    "Ssid", SsidValue (ssid),
    "BeaconGeneration", BooleanValue (true),
    "BeaconInterval", TimeValue (Seconds (2.5)));
  mac.SetMsduAggregatorForAc (AC_BE, "ns3::MsduStandardAggregator", 
                                     "MaxAmsduSize", UintegerValue (7935));

  NetDeviceContainer apDevice;
  apDevice = wifi.Install (phy, mac, wifiApNode);
 
  /* Setting mobility model */
  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
    "MinX", DoubleValue (0.0),
    "MinY", DoubleValue (0.0),
    "DeltaX", DoubleValue (5.0),
    "DeltaY", DoubleValue (10.0),
    "GridWidth", UintegerValue (3),
    "LayoutType", StringValue ("RowFirst"));

  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
    "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
  mobility.Install (wifiNodes);

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode);
  
  /* Internet stack*/
  InternetStackHelper stack;
  stack.Install (wifiApNode);
  stack.Install (wifiNodes);

  Ipv4AddressHelper address;

  address.SetBase ("192.168.1.0", "255.255.255.0");
  Ipv4InterfaceContainer wifiNodesInterfaces;
  Ipv4InterfaceContainer apNodeInterface;
  
  wifiNodesInterfaces = address.Assign (staDevices);
  apNodeInterface = address.Assign (apDevice);
  
  /* Setting applications */
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (wifiNodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (wifiNodesInterfaces.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (3));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.000001)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1500));

  ApplicationContainer clientApps = 
    echoClient.Install (wifiNodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Stop (Seconds (10.0));
  
  phy.EnablePcap ("test-802.11n", 
    wifiNodes.Get (nWifi - 1)->GetId (), 0);

  Simulator::Run ();
  Simulator::Destroy ();
  
  return 0;
}
