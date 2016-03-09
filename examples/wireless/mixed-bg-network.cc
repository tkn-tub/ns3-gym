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

// This example shows how the presence of an 802.11b station in an 802.11g network does affect the performance.
//
// The example compares different scenarios depending on whether an 802.11b station is associated to the access point
// and depending on the configuration of the following parameters:
// - protection mode that is configured on the AP;
// - whether short PLCP is supported by the 802.11b station;
// - whether short slot time is supported by both the 802.11g station and the AP.
//
// The output results show that the presence of an 802.11b station strongly affects 802.11g performance.
// Protection mechanisms ensure that the NAV value of 802.11b stations is set correctly in case of 802.11g transmissions.
// In practice, those protection mechanism add a lot of overhead, resulting in reduced performance. CTS-To-Self introduces
// less overhead than Rts-Cts, but is not heard by hidden stations (and is thus generally only recommended as a protection
// mechanism for access points). Since short slot time is disabled once an 802.11b station enters the network, benefits from
// short slot time are only observed in a pure-G configuration.
//
// The user can also select the payload size and can choose either an UDP or a TCP connection.
// Example: ./waf --run "mixed-bg-network --isUdp=1"
//
// Network topology:
//
//   STA (802.11b)    AP (802.11b/g)   STA (802.11b/g)
//   *                *                *
//   |                |                |
//   n1               n2               n3

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ErpAndNonErp");

class Experiment
{
public:
  Experiment ();
  double Run (bool enableProtection, bool enableShortSlotTime, bool enableShortPlcpPreamble, bool isMixed, bool isUdp, uint32_t payloadSize, uint32_t simulationTime);
};

Experiment::Experiment ()
{
}

double
Experiment::Run (bool enableProtection, bool enableShortSlotTime, bool enableShortPlcpPreamble, bool isMixed, bool isUdp, uint32_t payloadSize, uint32_t simulationTime)
{
  double throughput = 0;
  uint32_t totalPacketsThrough = 0;
  uint32_t nWifiB = 0;
  if (isMixed)
    {
      nWifiB = 1;
    }

  NodeContainer wifiBStaNodes;
  wifiBStaNodes.Create (nWifiB);
  NodeContainer wifiGStaNodes;
  wifiGStaNodes.Create (1);
  NodeContainer wifiApNode;
  wifiApNode.Create (1);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  channel.AddPropagationLoss ("ns3::RangePropagationLossModel");

  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);
  phy.SetChannel (channel.Create ());

  WifiHelper wifi;
  wifi.SetRemoteStationManager ("ns3::IdealWifiManager");

  // 802.11b STA
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);

  WifiMacHelper mac;
  Ssid ssid = Ssid ("ns-3-ssid");

  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false),
               "ShortSlotTimeSupported", BooleanValue (enableShortSlotTime));
    
  // Configure the PLCP preamble type: long or short
  phy.Set ("ShortPlcpPreambleSupported", BooleanValue (enableShortPlcpPreamble));

  NetDeviceContainer bStaDevice;
  bStaDevice = wifi.Install (phy, mac, wifiBStaNodes);
  
  // 802.11b/g STA
  wifi.SetStandard (WIFI_PHY_STANDARD_80211g);
  NetDeviceContainer gStaDevice;
  gStaDevice = wifi.Install (phy, mac, wifiGStaNodes);
  
  // 802.11b/g AP
  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid),
               "BeaconGeneration", BooleanValue (true),
               "EnableNonErpProtection", BooleanValue (enableProtection),
               "ShortSlotTimeSupported", BooleanValue (enableShortSlotTime));
    
  NetDeviceContainer apDevice;
  apDevice = wifi.Install (phy, mac, wifiApNode);

  // Setting mobility model
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  if (isMixed)
  {
    positionAlloc->Add (Vector (5.0, 0.0, 0.0));
  }
  positionAlloc->Add (Vector (0.0, 5.0, 0.0));
  
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode);
  mobility.Install (wifiBStaNodes);
  mobility.Install (wifiGStaNodes);

  // Internet stack
  InternetStackHelper stack;
  stack.Install (wifiApNode);
  stack.Install (wifiBStaNodes);
  stack.Install (wifiGStaNodes);

  Ipv4AddressHelper address;

  address.SetBase ("192.168.1.0", "255.255.255.0");
  Ipv4InterfaceContainer bStaInterface;
  bStaInterface = address.Assign (bStaDevice);
  Ipv4InterfaceContainer gStaInterface;
  gStaInterface = address.Assign (gStaDevice);
  Ipv4InterfaceContainer ApInterface;
  ApInterface = address.Assign (apDevice);

  // Setting applications
  if (isUdp)
    {
      UdpServerHelper myServer (9);
      ApplicationContainer serverApp = myServer.Install (wifiApNode);
      serverApp.Start (Seconds (0.0));
      serverApp.Stop (Seconds (simulationTime + 1));

      UdpClientHelper myClient (ApInterface.GetAddress (0), 9);
      myClient.SetAttribute ("MaxPackets", UintegerValue (4294967295u));
      myClient.SetAttribute ("Interval", TimeValue (Time ("0.0002"))); //packets/s
      myClient.SetAttribute ("PacketSize", UintegerValue (payloadSize));

      ApplicationContainer clientApp = myClient.Install (wifiGStaNodes);
      clientApp.Start (Seconds (1.0));
      clientApp.Stop (Seconds (simulationTime + 1));
  
      Simulator::Stop (Seconds (simulationTime + 1));
      Simulator::Run ();
      Simulator::Destroy ();
  
      totalPacketsThrough = DynamicCast<UdpServer> (serverApp.Get (0))->GetReceived ();
      throughput = totalPacketsThrough * payloadSize * 8 / (simulationTime * 1000000.0);
    }
  else
    {
      uint16_t port = 50000;
      Address apLocalAddress (InetSocketAddress (Ipv4Address::GetAny (), port));
      PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", apLocalAddress);
      
      ApplicationContainer sinkApp = packetSinkHelper.Install (wifiApNode.Get (0));
      sinkApp.Start (Seconds (0.0));
      sinkApp.Stop (Seconds (simulationTime + 1));

      OnOffHelper onoff ("ns3::TcpSocketFactory",Ipv4Address::GetAny ());
      onoff.SetAttribute ("OnTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
      onoff.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
      onoff.SetAttribute ("PacketSize", UintegerValue (payloadSize));
      onoff.SetAttribute ("DataRate", DataRateValue (54000000)); //bit/s
      
      AddressValue remoteAddress (InetSocketAddress (ApInterface.GetAddress (0), port));
      onoff.SetAttribute ("Remote", remoteAddress);
      
      ApplicationContainer apps;
      apps.Add (onoff.Install (wifiGStaNodes));
      apps.Start (Seconds (1.0));
      apps.Stop (Seconds (simulationTime + 1));

      Simulator::Stop (Seconds (simulationTime + 1));
      Simulator::Run ();
      Simulator::Destroy ();
      
      totalPacketsThrough = DynamicCast<PacketSink> (sinkApp.Get (0))->GetTotalRx ();
      throughput += totalPacketsThrough * 8 / (simulationTime * 1000000.0);
    }
  
  return throughput;
}

int main (int argc, char *argv[])
{
  uint32_t payloadSize = 1472; //bytes
  uint32_t simulationTime = 10; //seconds
  bool isUdp = true;
  
  CommandLine cmd;
  cmd.AddValue ("payloadSize", "Payload size in bytes", payloadSize);
  cmd.AddValue ("simulationTime", "Simulation time in seconds", simulationTime);
  cmd.AddValue ("isUdp", "UDP if set to 1, TCP otherwise", isUdp);
  cmd.Parse (argc, argv);

  Experiment experiment;
  double throughput = 0;
  std::cout << "Protection mode" << "\t\t" << "Slot time supported" << "\t\t" << "PLCP preamble supported" << "\t\t" << "Scenario" << "\t" << "Throughput" << std::endl;
  
  throughput = experiment.Run (false, false, false, false, isUdp, payloadSize, simulationTime);
  if (throughput < 23 || throughput > 24)
    {
      NS_LOG_ERROR ("Obtained throughput is not in the expected boundaries!");
      exit (1);
    }
  std::cout << "Disabled" << "\t\t" << "Long" << "\t\t\t\t" << "Long" << "\t\t\t\t" << "G-only" << "\t\t" << throughput <<" Mbit/s" << std::endl;
  
  throughput = experiment.Run (false, true, false, false, isUdp, payloadSize, simulationTime);
  if (throughput < 29 || throughput > 30)
    {
      NS_LOG_ERROR ("Obtained throughput is not in the expected boundaries!");
      exit (1);
    }
  std::cout << "Disabled" << "\t\t" << "Short" << "\t\t\t\t" << "Long" << "\t\t\t\t" << "G-only" << "\t\t" << throughput <<" Mbit/s" << std::endl;
  
  throughput = experiment.Run (false, false, false, true, isUdp, payloadSize, simulationTime);
  if (throughput < 23 || throughput > 24)
    {
      NS_LOG_ERROR ("Obtained throughput is not in the expected boundaries!");
      exit (1);
    }
  std::cout << "Disabled" << "\t\t" << "Long" << "\t\t\t\t" << "Long" << "\t\t\t\t" << "Mixed" << "\t\t" << throughput <<" Mbit/s" << std::endl;
  
  throughput = experiment.Run (false, false, true, true, isUdp, payloadSize, simulationTime);
  if (throughput < 23 || throughput > 24)
    {
      NS_LOG_ERROR ("Obtained throughput is not in the expected boundaries!");
      exit (1);
    }
  std::cout << "Disabled" << "\t\t" << "Long" << "\t\t\t\t" << "Short" << "\t\t\t\t" << "Mixed" << "\t\t" << throughput <<" Mbit/s" << std::endl;
  
  Config::SetDefault ("ns3::WifiRemoteStationManager::ProtectionMode", StringValue ("Rts-Cts"));
  
  throughput = experiment.Run (true, false, false, true, isUdp, payloadSize, simulationTime);
  if (throughput < 11 || throughput > 12)
    {
      NS_LOG_ERROR ("Obtained throughput is not in the expected boundaries!");
      exit (1);
    }
  std::cout << "RTS/CTS" << "\t\t\t" << "Long" << "\t\t\t\t" << "Long" << "\t\t\t\t" << "Mixed" << "\t\t" << throughput <<" Mbit/s" << std::endl;
  
  throughput = experiment.Run (true, false, true, true, isUdp, payloadSize, simulationTime);
  if (throughput < 13 || throughput > 14)
    {
      NS_LOG_ERROR ("Obtained throughput is not in the expected boundaries!");
      exit (1);
    }
  std::cout << "RTS/CTS" << "\t\t\t" << "Long" << "\t\t\t\t" << "Short" << "\t\t\t\t" << "Mixed" << "\t\t" << throughput <<" Mbit/s" << std::endl;
  
  Config::SetDefault ("ns3::WifiRemoteStationManager::ProtectionMode", StringValue ("Cts-To-Self"));
  
  throughput = experiment.Run (true, false, false, true, isUdp, payloadSize, simulationTime);
  if (throughput < 15 || throughput > 16)
    {
      NS_LOG_ERROR ("Obtained throughput is not in the expected boundaries!");
      exit (1);
    }
  std::cout << "CTS-TO-SELF" << "\t\t" << "Long" << "\t\t\t\t" << "Long" << "\t\t\t\t" << "Mixed" << "\t\t" << throughput <<" Mbit/s" << std::endl;
  
  throughput = experiment.Run (true, false, true, true, isUdp, payloadSize, simulationTime);
  if (throughput < 17 || throughput > 18)
    {
      NS_LOG_ERROR ("Obtained throughput is not in the expected boundaries!");
      exit (1);
    }
  std::cout << "CTS-TO-SELF" << "\t\t" << "Long" << "\t\t\t\t" << "Short" << "\t\t\t\t" << "Mixed" << "\t\t" << throughput <<" Mbit/s" << std::endl;
  
  return 0;
}
