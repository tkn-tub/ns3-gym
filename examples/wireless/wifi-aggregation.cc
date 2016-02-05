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
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"

// This is an example that illustrates how 802.11n aggregation is configured.
// It defines 4 independant Wi-Fi networks (working on different channels).
// Each network contains one access point and one station. Each station
// continously transmits data packets to its respective AP.
//
// Network topology (numbers in parentheses are channel numbers):
//
//  Network A (36)   Network B (40)   Network C (44)   Network D (48)
//   *      *          *      *         *      *          *      *
//   |      |          |      |         |      |          |      |
//  AP A   STA A      AP B   STA B     AP C   STA C      AP D   STA D
//
// The aggregation parameters are configured differently on the 4 stations:
// - station A uses default aggregation parameter values (A-MSDU disabled, A-MPDU enabled with maximum size of 65 kB);
// - station B doesn't use aggregation (both A-MPDU and A-MSDU are disabled);
// - station C enables A-MSDU (with maximum size of 8 kB) but disables A-MPDU;
// - station C uses two-level aggregation (A-MPDU with maximum size of 32 kB and A-MSDU with maximum size of 4 kB).
//
// Packets in this simulation aren't marked with a QosTag so they
// are considered belonging to BestEffort Access Class (AC_BE).
//
// The user can select the distance between the stations and the APs and can enable/disable the RTS/CTS mechanism.
// Example: ./waf --run "wifi-aggregation --distance=10 --enableRts=0 --simulationTime=20"
//
// The output prints the throughput measured for the 4 cases/networks decribed above. When default aggregation parameters are enabled, the
// maximum A-MPDU size is 65 kB and the throughput is maximal. When aggregation is disabled, the thoughput is about the half of the
// physical bitrate as in legacy wifi networks. When only A-MSDU is enabled, the throughput is increased but is not maximal, since the maximum
// A-MSDU size is limited to 7935 bytes (whereas the maximum A-MPDU size is limited to 65535 bytes). When A-MSDU and A-MPDU are both enabled
// (= two-level aggregation), the throughput is slightly smaller than the first scenario since we set a smaller maximum A-MPDU size.
//
// When the distance is increased, the frame error rate gets higher, and the output shows how it affects the throughput for the 4 networks.
// Even through A-MSDU has less overheads than A-MPDU, A-MSDU is less robust against transmission errors than A-MPDU. When the distance is
// augmented, the throughput for the third scenario is more affected than the throughput obtained in other networks.

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SimpleMpduAggregation");

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

  WifiHelper wifi;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("HtMcs7"), "ControlMode", StringValue ("HtMcs0"));
  WifiMacHelper mac;
  
  NetDeviceContainer staDeviceA, staDeviceB, staDeviceC, staDeviceD, apDeviceA, apDeviceB, apDeviceC, apDeviceD;
  Ssid ssid;

  //Network A
  ssid = Ssid ("network-A");
  phy.Set ("ChannelNumber", UintegerValue(36));
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));
  staDeviceA = wifi.Install (phy, mac, wifiStaNode.Get(0));

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid),
               "BeaconInterval", TimeValue (MicroSeconds (102400)),
               "BeaconGeneration", BooleanValue (true));
  apDeviceA = wifi.Install (phy, mac, wifiApNode.Get(0));
  
  //Network B
  ssid = Ssid ("network-B");
  phy.Set ("ChannelNumber", UintegerValue(40));
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false),
               "BE_MaxAmpduSize", UintegerValue (0)); //Disable A-MPDU

  staDeviceB = wifi.Install (phy, mac, wifiStaNode.Get(1));

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid),
               "BeaconInterval", TimeValue (MicroSeconds (102400)),
               "BeaconGeneration", BooleanValue (true));
  apDeviceB = wifi.Install (phy, mac, wifiApNode.Get(1));
  
  //Network C
  ssid = Ssid ("network-C");
  phy.Set ("ChannelNumber", UintegerValue(44));
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false),
               "BE_MaxAmpduSize", UintegerValue (0), //Disable A-MPDU
               "BE_MaxAmsduSize", UintegerValue (7935)); //Enable A-MSDU with the highest maximum size allowed by the standard (7935 bytes)

  staDeviceC = wifi.Install (phy, mac, wifiStaNode.Get(2));

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid),
               "BeaconInterval", TimeValue (MicroSeconds (102400)),
               "BeaconGeneration", BooleanValue (true));
  apDeviceC = wifi.Install (phy, mac, wifiApNode.Get(2));
  
  //Network D
  ssid = Ssid ("network-D");
  phy.Set ("ChannelNumber", UintegerValue(48));
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false),
               "BE_MaxAmpduSize", UintegerValue (32768), //Enable A-MPDU with a smaller size than the default one
               "BE_MaxAmsduSize", UintegerValue (3839)); //Enable A-MSDU with the smallest maximum size allowed by the standard (3839 bytes)

  staDeviceD = wifi.Install (phy, mac, wifiStaNode.Get(3));

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid),
               "BeaconInterval", TimeValue (MicroSeconds (102400)),
               "BeaconGeneration", BooleanValue (true));
  apDeviceD = wifi.Install (phy, mac, wifiApNode.Get(3));

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
  UdpServerHelper myServerA (9);
  ApplicationContainer serverAppA = myServerA.Install (wifiStaNode.Get (0));
  serverAppA.Start (Seconds (0.0));
  serverAppA.Stop (Seconds (simulationTime + 1));

  UdpClientHelper myClientA (StaInterfaceA.GetAddress (0), 9);
  myClientA.SetAttribute ("MaxPackets", UintegerValue (4294967295u));
  myClientA.SetAttribute ("Interval", TimeValue (Time ("0.00002"))); //packets/s
  myClientA.SetAttribute ("PacketSize", UintegerValue (payloadSize));

  ApplicationContainer clientAppA = myClientA.Install (wifiApNode.Get (0));
  clientAppA.Start (Seconds (1.0));
  clientAppA.Stop (Seconds (simulationTime + 1));
  
  UdpServerHelper myServerB (9);
  ApplicationContainer serverAppB = myServerB.Install (wifiStaNode.Get (1));
  serverAppB.Start (Seconds (0.0));
  serverAppB.Stop (Seconds (simulationTime + 1));

  UdpClientHelper myClientB (StaInterfaceB.GetAddress (0), 9);
  myClientB.SetAttribute ("MaxPackets", UintegerValue (4294967295u));
  myClientB.SetAttribute ("Interval", TimeValue (Time ("0.00002"))); //packets/s
  myClientB.SetAttribute ("PacketSize", UintegerValue (payloadSize));

  ApplicationContainer clientAppB = myClientB.Install (wifiApNode.Get (1));
  clientAppB.Start (Seconds (1.0));
  clientAppB.Stop (Seconds (simulationTime + 1));
  
  UdpServerHelper myServerC (9);
  ApplicationContainer serverAppC = myServerC.Install (wifiStaNode.Get (2));
  serverAppC.Start (Seconds (0.0));
  serverAppC.Stop (Seconds (simulationTime + 1));

  UdpClientHelper myClientC (StaInterfaceC.GetAddress (0), 9);
  myClientC.SetAttribute ("MaxPackets", UintegerValue (4294967295u));
  myClientC.SetAttribute ("Interval", TimeValue (Time ("0.00002"))); //packets/s
  myClientC.SetAttribute ("PacketSize", UintegerValue (payloadSize));

  ApplicationContainer clientAppC = myClientC.Install (wifiApNode.Get (2));
  clientAppC.Start (Seconds (1.0));
  clientAppC.Stop (Seconds (simulationTime + 1));
  
  UdpServerHelper myServerD (9);
  ApplicationContainer serverAppD = myServerD.Install (wifiStaNode.Get (3));
  serverAppD.Start (Seconds (0.0));
  serverAppD.Stop (Seconds (simulationTime + 1));

  UdpClientHelper myClientD (StaInterfaceD.GetAddress (0), 9);
  myClientD.SetAttribute ("MaxPackets", UintegerValue (4294967295u));
  myClientD.SetAttribute ("Interval", TimeValue (Time ("0.00002"))); //packets/s
  myClientD.SetAttribute ("PacketSize", UintegerValue (payloadSize));

  ApplicationContainer clientAppD = myClientD.Install (wifiApNode.Get (3));
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
  std::cout << "Throughput with default configuration (A-MPDU aggregation enabled, 65kB): " << throughput << " Mbit/s" << '\n';
  
  totalPacketsThrough = DynamicCast<UdpServer> (serverAppB.Get (0))->GetReceived ();
  throughput = totalPacketsThrough * payloadSize * 8 / (simulationTime * 1000000.0);
  std::cout << "Throughput with aggregation disabled: " << throughput << " Mbit/s" << '\n';
  
  totalPacketsThrough = DynamicCast<UdpServer> (serverAppC.Get (0))->GetReceived ();
  throughput = totalPacketsThrough * payloadSize * 8 / (simulationTime * 1000000.0);
  std::cout << "Throughput with A-MPDU disabled and A-MSDU enabled (8kB): " << throughput << " Mbit/s" << '\n';
  
  totalPacketsThrough = DynamicCast<UdpServer> (serverAppD.Get (0))->GetReceived ();
  throughput = totalPacketsThrough * payloadSize * 8 / (simulationTime * 1000000.0);
  std::cout << "Throughput with A-MPDU enabled (32kB) and A-MSDU enabled (4kB): " << throughput << " Mbit/s" << '\n';

  return 0;
}
