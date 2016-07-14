/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 Sébastien Deronne
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
 * Author: Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"

// This is an example that illustrates 802.11 QoS for different Access Categories.
// It defines 4 independant Wi-Fi networks (working on different logical channels
// on the same "ns3::YansWifiPhy" channel object).
// Each network contains one access point and one station. Each station continuously
// transmits data packets to its respective AP.
//
// Network topology (numbers in parentheses are channel numbers):
//
//  BSS A (36)        BSS B (40)       BSS C (44)        BSS D (48)
//   *      *          *      *         *      *          *      *
//   |      |          |      |         |      |          |      |
//  AP A   STA A      AP B   STA B     AP C   STA C      AP D   STA D
//
// The configuration is the following on the 4 networks:
// - STA A sends AC_BE traffic to AP A with default AC_BE TXOP value of 0 (1 MSDU);
// - STA B sends AC_BE traffic to AP B with non-default AC_BE TXOP of 3.008 ms;
// - STA C sends AC_VI traffic to AP C with default AC_VI TXOP of 3.008 ms;
// - STA D sends AC_VI traffic to AP D with non-default AC_VI TXOP value of 0 (1 MSDU);
//
// The user can select the distance between the stations and the APs, can enable/disable the RTS/CTS mechanism
// and can choose the payload size and the simulation duration.
// Example: ./waf --run "80211e-txop --distance=10 --enableRts=0 --simulationTime=20 --payloadSize=1000"
//
// The output prints the throughput measured for the 4 cases/networks decribed above. When TXOP is enabled, results show
// increased throughput since the channel is granted for a longer duration. TXOP is enabled by default for AC_VI and AC_VO,
// so that they can use the channel for a longer duration than AC_BE and AC_BK.

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("80211eTxop");

int main (int argc, char *argv[])
{
  uint32_t payloadSize = 1472; //bytes
  uint64_t simulationTime = 10; //seconds
  double distance = 5; //meters
  bool enablePcap = 0;

  CommandLine cmd;
  cmd.AddValue ("payloadSize", "Payload size in bytes", payloadSize);
  cmd.AddValue ("simulationTime", "Simulation time in seconds", simulationTime);
  cmd.AddValue ("distance", "Distance in meters between the station and the access point", distance);
  cmd.AddValue ("enablePcap", "Enable/disable pcap file generation", enablePcap);
  cmd.Parse (argc, argv);

  NodeContainer wifiStaNode;
  wifiStaNode.Create (4);
  NodeContainer wifiApNode;
  wifiApNode.Create (4);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);
  phy.SetChannel (channel.Create ());

  WifiHelper wifi; //the default standard of 802.11a will be selected by this helper since the program doesn't specify another one
  wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
  WifiMacHelper mac;
  
  NetDeviceContainer staDeviceA, staDeviceB, staDeviceC, staDeviceD, apDeviceA, apDeviceB, apDeviceC, apDeviceD;
  Ssid ssid;

  //Network A
  ssid = Ssid ("network-A");
  phy.Set ("ChannelNumber", UintegerValue(36));
  mac.SetType ("ns3::StaWifiMac",
               "QosSupported", BooleanValue (true),
               "Ssid", SsidValue (ssid));
  staDeviceA = wifi.Install (phy, mac, wifiStaNode.Get(0));

  mac.SetType ("ns3::ApWifiMac",
               "QosSupported", BooleanValue (true),
               "Ssid", SsidValue (ssid),
               "BeaconGeneration", BooleanValue (true));
  apDeviceA = wifi.Install (phy, mac, wifiApNode.Get(0));
  
  //Network B
  ssid = Ssid ("network-B");
  phy.Set ("ChannelNumber", UintegerValue(40));
  mac.SetType ("ns3::StaWifiMac",
               "QosSupported", BooleanValue (true),
               "Ssid", SsidValue (ssid));

  staDeviceB = wifi.Install (phy, mac, wifiStaNode.Get(1));

  mac.SetType ("ns3::ApWifiMac",
               "QosSupported", BooleanValue (true),
               "Ssid", SsidValue (ssid),
               "BeaconGeneration", BooleanValue (true));
  apDeviceB = wifi.Install (phy, mac, wifiApNode.Get(1));
  
  //Modify EDCA configuration (TXOP limit) for AC_BE
  Ptr<NetDevice> dev = wifiApNode.Get(1)->GetDevice(0);
  Ptr<WifiNetDevice> wifi_dev = DynamicCast<WifiNetDevice>(dev);
  Ptr<WifiMac> wifi_mac = wifi_dev->GetMac();
  PointerValue ptr;
  Ptr<EdcaTxopN> edca;
  wifi_mac->GetAttribute("BE_EdcaTxopN", ptr);
  edca = ptr.Get<EdcaTxopN>();
  edca->SetTxopLimit (MicroSeconds (3008));
  
  //Network C
  ssid = Ssid ("network-C");
  phy.Set ("ChannelNumber", UintegerValue(44));
  mac.SetType ("ns3::StaWifiMac",
               "QosSupported", BooleanValue (true),
               "Ssid", SsidValue (ssid));

  staDeviceC = wifi.Install (phy, mac, wifiStaNode.Get(2));

  mac.SetType ("ns3::ApWifiMac",
               "QosSupported", BooleanValue (true),
               "Ssid", SsidValue (ssid),
               "BeaconGeneration", BooleanValue (true));
  apDeviceC = wifi.Install (phy, mac, wifiApNode.Get(2));
  
  //Network D
  ssid = Ssid ("network-D");
  phy.Set ("ChannelNumber", UintegerValue(48));
  mac.SetType ("ns3::StaWifiMac",
               "QosSupported", BooleanValue (true),
               "Ssid", SsidValue (ssid));

  staDeviceD = wifi.Install (phy, mac, wifiStaNode.Get(3));

  mac.SetType ("ns3::ApWifiMac",
               "QosSupported", BooleanValue (true),
               "Ssid", SsidValue (ssid),
               "BeaconGeneration", BooleanValue (true));
  apDeviceD = wifi.Install (phy, mac, wifiApNode.Get(3));
  
  //Modify EDCA configuration (TXOP limit) for AC_VO
  dev = wifiApNode.Get(3)->GetDevice(0);
  wifi_dev = DynamicCast<WifiNetDevice>(dev);
  wifi_mac = wifi_dev->GetMac();
  wifi_mac->GetAttribute("VI_EdcaTxopN", ptr);
  edca = ptr.Get<EdcaTxopN>();
  edca->SetTxopLimit (MicroSeconds (0));

  /* Setting mobility model */
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  //Set position for APs
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (10.0, 0.0, 0.0));
  positionAlloc->Add (Vector (20.0, 0.0, 0.0));
  positionAlloc->Add (Vector (30.0, 0.0, 0.0));
  //Set position for STAs
  positionAlloc->Add (Vector (distance, 0.0, 0.0));
  positionAlloc->Add (Vector (10 + distance, 0.0, 0.0));
  positionAlloc->Add (Vector (20 + distance, 0.0, 0.0));
  positionAlloc->Add (Vector (30 + distance, 0.0, 0.0));
  //Remark: while we set these positions 10 meters apart, the networks do not interact
  //and the only variable that affects transmission performance is the distance.
  
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (wifiApNode);
  mobility.Install (wifiStaNode);

  /* Internet stack */
  InternetStackHelper stack;
  stack.Install (wifiApNode);
  stack.Install (wifiStaNode);

  Ipv4AddressHelper address;

  address.SetBase ("192.168.1.0", "255.255.255.0");
  Ipv4InterfaceContainer StaInterfaceA;
  StaInterfaceA = address.Assign (staDeviceA);
  Ipv4InterfaceContainer ApInterfaceA;
  ApInterfaceA = address.Assign (apDeviceA);
  
  address.SetBase ("192.168.2.0", "255.255.255.0");
  Ipv4InterfaceContainer StaInterfaceB;
  StaInterfaceB = address.Assign (staDeviceB);
  Ipv4InterfaceContainer ApInterfaceB;
  ApInterfaceB = address.Assign (apDeviceB);
  
  address.SetBase ("192.168.3.0", "255.255.255.0");
  Ipv4InterfaceContainer StaInterfaceC;
  StaInterfaceC = address.Assign (staDeviceC);
  Ipv4InterfaceContainer ApInterfaceC;
  ApInterfaceC = address.Assign (apDeviceC);
  
  address.SetBase ("192.168.4.0", "255.255.255.0");
  Ipv4InterfaceContainer StaInterfaceD;
  StaInterfaceD = address.Assign (staDeviceD);
  Ipv4InterfaceContainer ApInterfaceD;
  ApInterfaceD = address.Assign (apDeviceD);

  /* Setting applications */
  UdpServerHelper myServerA (5001);
  ApplicationContainer serverAppA = myServerA.Install (wifiApNode.Get (0));
  serverAppA.Start (Seconds (0.0));
  serverAppA.Stop (Seconds (simulationTime + 1));
  
  InetSocketAddress destA (ApInterfaceA.GetAddress (0), 5001);
  destA.SetTos (0x70); //AC_BE
  
  OnOffHelper myClientA ("ns3::UdpSocketFactory", destA);
  myClientA.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  myClientA.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
  myClientA.SetAttribute ("DataRate", StringValue("100000kb/s"));
  myClientA.SetAttribute ("PacketSize", UintegerValue(payloadSize));

  ApplicationContainer clientAppA = myClientA.Install (wifiStaNode.Get (0));
  clientAppA.Start (Seconds (1.0));
  clientAppA.Stop (Seconds (simulationTime + 1));
  
  UdpServerHelper myServerB (5001);
  ApplicationContainer serverAppB = myServerB.Install (wifiApNode.Get (1));
  serverAppB.Start (Seconds (0.0));
  serverAppB.Stop (Seconds (simulationTime + 1));

  InetSocketAddress destB (ApInterfaceB.GetAddress (0), 5001);
  destB.SetTos (0x70); //AC_BE

  OnOffHelper myClientB ("ns3::UdpSocketFactory", destB);
  myClientB.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  myClientB.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
  myClientB.SetAttribute ("DataRate", StringValue("100000kb/s"));
  myClientB.SetAttribute ("PacketSize", UintegerValue(payloadSize));

  ApplicationContainer clientAppB = myClientB.Install (wifiStaNode.Get (1));
  clientAppB.Start (Seconds (1.0));
  clientAppB.Stop (Seconds (simulationTime + 1));
  
  UdpServerHelper myServerC (5001);
  ApplicationContainer serverAppC = myServerC.Install (wifiApNode.Get (2));
  serverAppC.Start (Seconds (0.0));
  serverAppC.Stop (Seconds (simulationTime + 1));

  InetSocketAddress destC (ApInterfaceC.GetAddress (0), 5001);
  destC.SetTos (0xb8); //AC_VI

  OnOffHelper myClientC ("ns3::UdpSocketFactory", destC);
  myClientC.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  myClientC.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
  myClientC.SetAttribute ("DataRate", StringValue("100000kb/s"));
  myClientC.SetAttribute ("PacketSize", UintegerValue(payloadSize));

  ApplicationContainer clientAppC = myClientC.Install (wifiStaNode.Get (2));
  clientAppC.Start (Seconds (1.0));
  clientAppC.Stop (Seconds (simulationTime + 1));
  
  UdpServerHelper myServerD (5001);
  ApplicationContainer serverAppD = myServerD.Install (wifiApNode.Get (3));
  serverAppD.Start (Seconds (0.0));
  serverAppD.Stop (Seconds (simulationTime + 1));

  InetSocketAddress destD (ApInterfaceD.GetAddress (0), 5001);
  destD.SetTos (0xb8); //AC_VI

  OnOffHelper myClientD ("ns3::UdpSocketFactory", destD);
  myClientD.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  myClientD.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
  myClientD.SetAttribute ("DataRate", StringValue("100000kb/s"));
  myClientD.SetAttribute ("PacketSize", UintegerValue(payloadSize));

  ApplicationContainer clientAppD = myClientD.Install (wifiStaNode.Get (3));
  clientAppD.Start (Seconds (1.0));
  clientAppD.Stop (Seconds (simulationTime + 1));
  
  if (enablePcap)
    {
      phy.EnablePcap ("AP_A", apDeviceA.Get (0));
      phy.EnablePcap ("STA_A", staDeviceA.Get (0));
      phy.EnablePcap ("AP_B", apDeviceB.Get (0));
      phy.EnablePcap ("STA_B", staDeviceB.Get (0));
      phy.EnablePcap ("AP_C", apDeviceC.Get (0));
      phy.EnablePcap ("STA_C", staDeviceC.Get (0));
      phy.EnablePcap ("AP_D", apDeviceD.Get (0));
      phy.EnablePcap ("STA_D", staDeviceD.Get (0));
    }

  Simulator::Stop (Seconds (simulationTime + 1));
  Simulator::Run ();
  Simulator::Destroy ();

  /* Show results */
  uint32_t totalPacketsThrough = DynamicCast<UdpServer> (serverAppA.Get (0))->GetReceived ();
  double throughput = totalPacketsThrough * payloadSize * 8 / (simulationTime * 1000000.0);
  std::cout << "Throughput for AC_BE with default TXOP limit (0ms): " << throughput << " Mbit/s" << '\n';
  
  totalPacketsThrough = DynamicCast<UdpServer> (serverAppB.Get (0))->GetReceived ();
  throughput = totalPacketsThrough * payloadSize * 8 / (simulationTime * 1000000.0);
  std::cout << "Throughput for AC_BE with non-default TXOP limit (3.008ms): " << throughput << " Mbit/s" << '\n';
  
  totalPacketsThrough = DynamicCast<UdpServer> (serverAppC.Get (0))->GetReceived ();
  throughput = totalPacketsThrough * payloadSize * 8 / (simulationTime * 1000000.0);
  std::cout << "Throughput for AC_VI with default TXOP limit (3.008ms): " << throughput << " Mbit/s" << '\n';
  
  totalPacketsThrough = DynamicCast<UdpServer> (serverAppD.Get (0))->GetReceived ();
  throughput = totalPacketsThrough * payloadSize * 8 / (simulationTime * 1000000.0);
  std::cout << "Throughput for AC_VI with non-default TXOP limit (0ms): " << throughput << " Mbit/s" << '\n';

  return 0;
}
