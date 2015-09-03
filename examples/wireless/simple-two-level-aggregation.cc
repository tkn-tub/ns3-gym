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

// This is a simple example in order to show how 802.11n two-level aggregation feature works.
// Two-level aggregation is the simultaneous use of MSDU and MPDU aggregation schemes (known as one-level aggregation techniques).
//
// The throughput is obtained for a given number of aggregated MSDUs and MPDUs.
// The number of aggregated MSDUs and MPDUs can be chosen by the user through the nMsdus and nMpdus attibutes, respectively.
//
// Example: ./waf --run "simple-two-level-aggregation --nMsdus=3 --nMpdus=8"
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
//
// Throughput gets higher as either nMsdus or nMpdus is increased (one-level aggregation),
// or when both nMsdus and nMpdus are increases (two-level aggregation).
// MSDU aggregation offers a better header reduction compare to MPDU aggregation,
// while MPDU aggregation is more robust against transmission errors than MSDU aggregation.
// The good setting of nMsdu and nMpdus will depends on various factors (payload size, channel conditions, ...).
// Since this example considers an ideal channel, the highest throughput is obtained with the largest values for nMsdus and nMpdus parameters.
// Users should nevertheless take care that the standard rules limit the maximum MSDU size to 7935 bytes and the MPDU size to 65535 bytes.
// Consequently, more packets should be aggregated if their payload is small with standard-compliant parameters.
// Users should also note that the maximum duration of the frame is limited by the standard.
// As a result, higher values for nMsdus and nMpdus may not always provide throughput improvements.

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SimpleTwoLevelAggregation");

int main (int argc, char *argv[])
{
  uint32_t payloadSize = 1472; //bytes
  uint64_t simulationTime = 10; //seconds
  uint32_t nMsdus = 1;
  uint32_t nMpdus = 1;
  bool enableRts = 0;

  CommandLine cmd;
  cmd.AddValue ("nMsdus", "Number of aggregated MSDUs", nMsdus); //number of aggregated MSDUs specified by the user
  cmd.AddValue ("nMpdus", "Number of aggregated MPDUs", nMpdus); //number of aggregated MPDUs specified by the user
  cmd.AddValue ("payloadSize", "Payload size in bytes", payloadSize);
  cmd.AddValue ("enableRts", "Enable RTS/CTS", enableRts);
  cmd.AddValue ("simulationTime", "Simulation time in seconds", simulationTime);
  cmd.Parse (argc, argv);

  if (!enableRts)
    {
      Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("999999"));
    }
  else
    {
      Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("0"));
    }

  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("990000"));

  NodeContainer wifiStaNode;
  wifiStaNode.Create (1);
  NodeContainer wifiApNode;
  wifiApNode.Create (1);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);
  phy.SetChannel (channel.Create ());

  WifiHelper wifi = WifiHelper::Default ();
  wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
  //We consider a constant bitrate since HT rate adaptation algorithms are not supported yet in the simulator
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode", StringValue ("HtMcs7"),
                                "ControlMode", StringValue ("HtMcs0"));
  HtWifiMacHelper mac = HtWifiMacHelper::Default ();

  Ssid ssid = Ssid ("simple-two-level-aggregation");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));


  //Enable aggregation at the station side
  if (nMpdus > 1)
    {
      mac.SetBlockAckThresholdForAc (AC_BE, 2); //enable BlockAck when A-MPDU is used
    }
  if (nMsdus > 0)
    {
      mac.SetMpduAggregatorForAc (AC_BE,"ns3::MpduStandardAggregator",
                                  "MaxAmpduSize", UintegerValue (nMpdus * (nMsdus * (payloadSize + 100)))); //Set the maximum size for A-MPDU with regards to the payload size and the number of MSDUs expected in each MPDU.
    }
  else //MPDU aggregation only (one-level aggregation)
    {
      mac.SetMpduAggregatorForAc (AC_BE,"ns3::MpduStandardAggregator",
                                  "MaxAmpduSize", UintegerValue (nMpdus * (payloadSize + 200))); //Set the maximum size for A-MPDU with regards to the payload size
    }
  mac.SetMsduAggregatorForAc (AC_BE,"ns3::MsduStandardAggregator",
                              "MaxAmsduSize", UintegerValue (nMsdus * (payloadSize + 100))); //Set the maximum size for A-MSDU with regards to the payload size


  NetDeviceContainer staDevice;
  staDevice = wifi.Install (phy, mac, wifiStaNode);

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid),
               "BeaconInterval", TimeValue (MicroSeconds (102400)),
               "BeaconGeneration", BooleanValue (true));

  //Enable aggregation at the AP side
  if (nMpdus > 1)
    {
      mac.SetBlockAckThresholdForAc (AC_BE, 2); //enable BlockAck when A-MPDU is used
    }
  if (nMsdus > 0)
    {
      mac.SetMpduAggregatorForAc (AC_BE,"ns3::MpduStandardAggregator",
                                  "MaxAmpduSize", UintegerValue (nMpdus * (nMsdus * (payloadSize + 100)))); //Set the maximum size for A-MPDU according to the payload size and the number of MSDUs expected in each MPDU.
    }
  else //MPDU aggregation only (one-level aggregation)
    {
      mac.SetMpduAggregatorForAc (AC_BE,"ns3::MpduStandardAggregator",
                                  "MaxAmpduSize", UintegerValue (nMpdus * (payloadSize + 200))); //Set the maximum size for A-MPDU with regards to the payload size
    }
  mac.SetMsduAggregatorForAc (AC_BE,"ns3::MsduStandardAggregator",
                              "MaxAmsduSize", UintegerValue (nMsdus * (payloadSize + 100))); //Set the maximum size for A-MSDU with regards to the payload size

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
  serverApp.Stop (Seconds (simulationTime + 1));

  UdpClientHelper myClient (StaInterface.GetAddress (0), 9);
  myClient.SetAttribute ("MaxPackets", UintegerValue (4294967295u));
  myClient.SetAttribute ("Interval", TimeValue (Time ("0.00002"))); //packets/s
  myClient.SetAttribute ("PacketSize", UintegerValue (payloadSize));

  ApplicationContainer clientApp = myClient.Install (wifiApNode.Get (0));
  clientApp.Start (Seconds (1.0));
  clientApp.Stop (Seconds (simulationTime + 1));

  Simulator::Stop (Seconds (simulationTime + 1));

  Simulator::Run ();
  Simulator::Destroy ();

  uint32_t totalPacketsThrough = DynamicCast<UdpServer> (serverApp.Get (0))->GetReceived ();
  double throughput = totalPacketsThrough * payloadSize * 8 / (simulationTime * 1000000.0);
  std::cout << "Throughput: " << throughput << " Mbit/s" << '\n';

  return 0;
}
