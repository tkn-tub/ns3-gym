/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Sébastien Deronne
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

// This is a simple example in order to show how 802.11n MPDU aggregation feature works.
// The throughput is obtained for a given number of aggregated MPDUs.
//
// The number of aggregated MPDUs can be chosen by the user through the nMpdus attibute.
// A value of 1 means that no MPDU aggregation is performed.
//
// Example: ./waf --run "simple-mpdu-aggregation --nMpdus=64"
//
// Network topology:
//
//   Wifi 192.168.1.0
//
//        AP
//   *    *
//   |    |
//   n1   n2
//
// Packets in this simulation aren't marked with a QosTag so they are considered
// belonging to BestEffort Access Class (AC_BE).

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SimpleMpduAggregation");

int main (int argc, char *argv[])
{
  uint32_t payloadSize = 1472; //bytes
  uint64_t simulationTime = 10; //seconds
  uint32_t nMpdus = 1;
  bool enableRts = 0;
    
  CommandLine cmd;
  cmd.AddValue("nMpdus", "Number of aggregated MPDUs", nMpdus); //number of aggregated MPDUs specified by the user
  cmd.AddValue("payloadSize", "Payload size in bytes", payloadSize);
  cmd.AddValue("enableRts", "Enable RTS/CTS", enableRts);
  cmd.AddValue("simulationTime", "Simulation time in seconds", simulationTime);
  cmd.Parse (argc, argv);
    
  if(!enableRts)
    Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("999999"));
  else
    Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("0"));
     
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("990000"));

  NodeContainer wifiStaNode;
  wifiStaNode.Create (1);
  NodeContainer wifiApNode;
  wifiApNode.Create(1);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);
  phy.SetChannel (channel.Create());

  WifiHelper wifi = WifiHelper::Default ();
  wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue("OfdmRate65MbpsBW20MHz"), "ControlMode", StringValue("OfdmRate6_5MbpsBW20MHz"));
  HtWifiMacHelper mac = HtWifiMacHelper::Default ();

  Ssid ssid = Ssid ("simple-mpdu-aggregation");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));

  if (nMpdus > 1) mac.SetBlockAckThresholdForAc (AC_BE, 2); //enable Block ACK when A-MPDU is enabled (i.e. nMpdus > 1)

  mac.SetMpduAggregatorForAc (AC_BE,"ns3::MpduStandardAggregator",
                              "MaxAmpduSize", UintegerValue (nMpdus*(payloadSize+100))); //enable MPDU aggregation for AC_BE with a maximum aggregated size of nMpdus*(payloadSize+100) bytes, i.e. nMpdus aggregated packets in an A-MPDU
  
  NetDeviceContainer staDevice;
  staDevice = wifi.Install (phy, mac, wifiStaNode);

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid),
               "BeaconInterval", TimeValue (MicroSeconds(102400)),
               "BeaconGeneration", BooleanValue (true));

  if (nMpdus > 1) mac.SetBlockAckThresholdForAc (AC_BE, 2); //enable Block ACK when A-MPDU is enabled (i.e. nMpdus > 1)
    
  mac.SetMpduAggregatorForAc (AC_BE,"ns3::MpduStandardAggregator",
                              "MaxAmpduSize", UintegerValue (nMpdus*(payloadSize+100))); //enable MPDU aggregation for AC_BE with a maximum aggregated size of nMpdus*(payloadSize+100) bytes, i.e. nMpdus aggregated packets in an A-MPDU

  NetDeviceContainer apDevice;
  apDevice = wifi.Install (phy, mac, wifiApNode);

  /* Setting mobility model */
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();

  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (1.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  mobility.Install (wifiApNode);
  mobility.Install (wifiStaNode);

  /* Internet stack*/
  InternetStackHelper stack;
  stack.Install (wifiApNode);
  stack.Install (wifiStaNode);

  Ipv4AddressHelper address;

  address.SetBase ("192.168.1.0", "255.255.255.0");
  Ipv4InterfaceContainer StaInterface;
  StaInterface = address.Assign (staDevice);
  Ipv4InterfaceContainer ApInterface;
  ApInterface = address.Assign (apDevice);
 
  /* Setting applications */
  UdpServerHelper myServer (9);
  ApplicationContainer serverApp = myServer.Install (wifiStaNode.Get (0));
  serverApp.Start (Seconds (0.0));
  serverApp.Stop (Seconds (simulationTime+1));
      
  UdpClientHelper myClient (StaInterface.GetAddress (0), 9);
  myClient.SetAttribute ("MaxPackets", UintegerValue (4294967295u));
  myClient.SetAttribute ("Interval", TimeValue (Time ("0.00002"))); //packets/s
  myClient.SetAttribute ("PacketSize", UintegerValue (payloadSize));
              
  ApplicationContainer clientApp = myClient.Install (wifiApNode.Get (0));
  clientApp.Start (Seconds (1.0));
  clientApp.Stop (Seconds (simulationTime+1));
      
  Simulator::Stop (Seconds (simulationTime+1));

  Simulator::Run ();
  Simulator::Destroy ();
      
  uint32_t totalPacketsThrough = DynamicCast<UdpServer>(serverApp.Get (0))->GetReceived ();
  double throughput = totalPacketsThrough*payloadSize*8/(simulationTime*1000000.0);
  std::cout << "Throughput: " << throughput << " Mbit/s" << '\n';
    
  return 0;
}
