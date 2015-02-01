/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 SEBASTIEN DERONNE
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
 * Authors: Sebastien Deronne <sebastien.deronne@gmail.com>
 */
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"

// This example shows how to set Wi-Fi timing parameters through WifiMac attributes.
//
// Example: set slot time to 20 microseconds, while keeping other values as defined in the simulation script:
//
//          ./waf --run "wifi-timing-attributes --slot=20"
//
// Network topology:
// 
//  Wifi 192.168.1.0
// 
//       AP
//  *    *
//  |    |
//  n1   n2
//

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("wifi-timing-attributes");

int main (int argc, char *argv[])
{
  uint32_t slot = 9; //slot time in microseconds
  uint32_t sifs = 10; //SIFS duration in microseconds
  uint32_t ackTimeout = 88; //ACK timeout duration in microseconds
  uint32_t ctsTimeout = 88; //CTS timeout duration in microseconds
  uint32_t rifs = 2; //RIFS duration in microseconds
  uint32_t basicBlockAckTimeout = 286; //Basic Block ACK timeout duration in microseconds
  uint32_t compressedBlockAckTimeout = 112; //Compressed Block ACK timeout duration in microseconds
  double simulationTime = 10; //simulation time in seconds

  CommandLine cmd;
  cmd.AddValue ("slot", "Slot time in microseconds", slot);
  cmd.AddValue ("sifs", "SIFS duration in microseconds", sifs);
  cmd.AddValue ("ackTimeout", "ACK timeout duration in microseconds", ackTimeout);
  cmd.AddValue ("ctsTimeout", "CTS timeout duration in microseconds", ctsTimeout);
  cmd.AddValue ("rifs", "RIFS duration in microseconds", rifs);
  cmd.AddValue ("basicBlockAckTimeoutTimeout", "Basic Block ACK timeout duration in microseconds", basicBlockAckTimeout);
  cmd.AddValue ("compressedBlockAckTimeoutTimeout", "Compressed Block ACK timeout duration in microseconds", compressedBlockAckTimeout);
  cmd.AddValue ("simulationTime", "Simulation time in seconds", simulationTime);
  cmd.Parse (argc,argv);
                
  //Since default reference loss is defined for 5 GHz, it needs to be changed when operating at 2.4 GHz
  Config::SetDefault ("ns3::LogDistancePropagationLossModel::ReferenceLoss", DoubleValue (40.046));
 
  //Create nodes
  NodeContainer wifiStaNode;
  wifiStaNode.Create (1);
  NodeContainer wifiApNode;
  wifiApNode.Create (1);
                
  //Create wireless channel
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

  //Default IEEE 802.11n (2.4 GHz)
  WifiHelper wifi = WifiHelper::Default ();
  wifi.SetStandard (WIFI_PHY_STANDARD_80211n_2_4GHZ);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode", StringValue("OfdmRate65MbpsBW20MHz"),
                                "ControlMode", StringValue("OfdmRate6_5MbpsBW20MHz"));
  HtWifiMacHelper mac = HtWifiMacHelper::Default ();

  //Install PHY and MAC
  Ssid ssid = Ssid ("ns3-wifi");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));

  NetDeviceContainer staDevice;
  staDevice = wifi.Install (phy, mac, wifiStaNode);

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));

  NetDeviceContainer apDevice;
  apDevice = wifi.Install (phy, mac, wifiApNode);

  //Once install is done, we overwrite the standard timing values
  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/Slot", TimeValue (MicroSeconds (slot)));
  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/Sifs", TimeValue (MicroSeconds (sifs)));
  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/AckTimeout", TimeValue (MicroSeconds (ackTimeout)));
  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/CtsTimeout", TimeValue (MicroSeconds (ctsTimeout)));
  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/Rifs", TimeValue (MicroSeconds (rifs)));
  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/BasicBlockAckTimeout", TimeValue (MicroSeconds (basicBlockAckTimeout)));
  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/CompressedBlockAckTimeout", TimeValue (MicroSeconds (compressedBlockAckTimeout)));
              
  //Mobility
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
      
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (1.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
      
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
      
  mobility.Install (wifiApNode);
  mobility.Install (wifiStaNode);

  //Internet stack
  InternetStackHelper stack;
  stack.Install (wifiApNode);
  stack.Install (wifiStaNode);

  Ipv4AddressHelper address;

  address.SetBase ("192.168.1.0", "255.255.255.0");
  Ipv4InterfaceContainer staNodeInterface;
  Ipv4InterfaceContainer apNodeInterface;

  staNodeInterface = address.Assign (staDevice);
  apNodeInterface = address.Assign (apDevice);
      
  //Setting applications
  UdpServerHelper myServer (9);
  ApplicationContainer serverApp = myServer.Install (wifiStaNode.Get (0));
  serverApp.Start (Seconds (0.0));
  serverApp.Stop (Seconds (simulationTime));

  UdpClientHelper client (staNodeInterface.GetAddress (0), 9);
  client.SetAttribute ("MaxPackets", UintegerValue (4294967295));
  client.SetAttribute ("Interval", TimeValue (Time ("0.00002"))); //packets/s
  client.SetAttribute ("PacketSize", UintegerValue (1472)); //bytes
 
  ApplicationContainer clientApp = client.Install (wifiApNode.Get (0));
  clientApp.Start (Seconds (1.0));
  clientApp.Stop (Seconds (simulationTime));
                
  //Populate routing table
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  //Set simulation time and launch simulation
  Simulator::Stop (Seconds (simulationTime));
  Simulator::Run ();
  Simulator::Destroy ();

  //Get and print results
  uint32_t totalPacketsThrough = DynamicCast<UdpServer>(serverApp.Get (0))->GetReceived ();
  double throughput = totalPacketsThrough * 1472 * 8/((simulationTime-1) * 1000000.0); //Mbit/s
  std::cout << "Throughput: " << throughput << " Mbit/s" << std::endl;
    
  return 0;
} 
