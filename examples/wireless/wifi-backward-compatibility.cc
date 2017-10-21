/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017
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
 * Author: Sebastien Deronne <sebastien.deronne@gmail.com>
 */

#include "ns3/core-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"

// This is an example to show how to configure an IEEE 802.11 Wi-Fi
// network where the AP and the station use different 802.11 standards.
//
// It outputs the throughput for a given configuration: user can specify
// the 802.11 versions for the AT and the station as well as their rate
// adaptation algorithms. It also allows to decide whether the station,
// the AP or both has/have traffic to send.
//
// Example for an IEEE 802.11ac station sending traffic to an 802.11a AP using Ideal rate adaptation algorithm:
// ./waf --run "wifi-backward-compatibility --apVersion=80211a --staVersion=80211ac --staRaa=Ideal"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("wifi-backward-compatibility");

WifiPhyStandard ConvertStringToStandard (std::string version)
{
  WifiPhyStandard standard;
  if (version == "80211a")
    {
      standard = WIFI_PHY_STANDARD_80211a;
    }
  else if (version == "80211b")
    {
      standard = WIFI_PHY_STANDARD_80211b;
    }
  else if (version == "80211g")
    {
      standard = WIFI_PHY_STANDARD_80211g;
    }
  else if (version == "80211_10MHZ")
    {
      standard = WIFI_PHY_STANDARD_80211_10MHZ;
    }
  else if (version == "80211_5MHZ")
    {
      standard = WIFI_PHY_STANDARD_80211_5MHZ;
    }
  else if (version == "holland")
    {
      standard = WIFI_PHY_STANDARD_holland;
    }
  else if (version == "80211n_2_4GHZ")
    {
      standard = WIFI_PHY_STANDARD_80211n_2_4GHZ;
    }
  else if (version == "80211n_5GHZ")
    {
      standard = WIFI_PHY_STANDARD_80211n_5GHZ;
    }
  else if (version == "80211ac")
    {
      standard = WIFI_PHY_STANDARD_80211ac;
    }
  else
    {
      standard = WIFI_PHY_STANDARD_UNSPECIFIED;
    }
  return standard;
}

int main (int argc, char *argv[])
{
  uint32_t payloadSize = 1472; //bytes
  double simulationTime = 10; //seconds
  std::string apVersion = "80211a";
  std::string staVersion = "80211n_5GHZ";
  std::string apRaa = "Minstrel";
  std::string staRaa = "MinstrelHt";
  bool apHasTraffic = false;
  bool staHasTraffic = true;

  CommandLine cmd;
  cmd.AddValue ("simulationTime", "Simulation time in seconds", simulationTime);
  cmd.AddValue ("apVersion", "The standard version used by the AP: 80211a, 80211b, 80211g, 80211_10MHZ, 80211_5MHZ, holland, 80211n_2_4GHZ, 80211n_5GHZ or 80211ac", apVersion);
  cmd.AddValue ("staVersion", "The standard version used by the station: 80211a, 80211b, 80211g, 80211_10MHZ, 80211_5MHZ, holland, 80211n_2_4GHZ, 80211n_5GHZ or 80211ac", staVersion);
  cmd.AddValue ("apRaa", "Rate adaptation algorithm used by the AP", apRaa);
  cmd.AddValue ("staRaa", "Rate adaptation algorithm used by the station", staRaa);
  cmd.AddValue ("apHasTraffic", "Enable/disable traffic on the AP", apHasTraffic);
  cmd.AddValue ("staHasTraffic", "Enable/disable traffic on the station", staHasTraffic);
  cmd.Parse (argc,argv);

  NodeContainer wifiStaNode;
  wifiStaNode.Create (1);
  NodeContainer wifiApNode;
  wifiApNode.Create (1);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

  WifiMacHelper mac;
  WifiHelper wifi;
  Ssid ssid = Ssid ("ns3");

  wifi.SetStandard (ConvertStringToStandard (staVersion));
  wifi.SetRemoteStationManager ("ns3::" + staRaa + "WifiManager");

  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid));

  NetDeviceContainer staDevice;
  staDevice = wifi.Install (phy, mac, wifiStaNode);

  wifi.SetStandard (ConvertStringToStandard (apVersion));
  wifi.SetRemoteStationManager ("ns3::" + apRaa + "WifiManager");

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));

  NetDeviceContainer apDevice;
  apDevice = wifi.Install (phy, mac, wifiApNode);

  //Workaround needed as long as we do not fully support channel bonding
  if (staVersion == "80211ac")
    {
      Config::Set ("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/Phy/ChannelWidth", UintegerValue (20));
      Config::Set ("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/Phy/Frequency", UintegerValue (5180));
    }
  if (apVersion == "80211ac")
    {
      Config::Set ("/NodeList/1/DeviceList/*/$ns3::WifiNetDevice/Phy/ChannelWidth", UintegerValue (20));
      Config::Set ("/NodeList/1/DeviceList/*/$ns3::WifiNetDevice/Phy/Frequency", UintegerValue (5180));
    }

  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (5.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode);
  mobility.Install (wifiStaNode);

  InternetStackHelper stack;
  stack.Install (wifiApNode);
  stack.Install (wifiStaNode);

  Ipv4AddressHelper address;
  address.SetBase ("192.168.1.0", "255.255.255.0");
  Ipv4InterfaceContainer staNodeInterface;
  Ipv4InterfaceContainer apNodeInterface;

  staNodeInterface = address.Assign (staDevice);
  apNodeInterface = address.Assign (apDevice);

  UdpServerHelper apServer (9);
  ApplicationContainer apServerApp = apServer.Install (wifiApNode.Get (0));
  apServerApp.Start (Seconds (0.0));
  apServerApp.Stop (Seconds (simulationTime + 1));

  UdpServerHelper staServer (5001);
  ApplicationContainer staServerApp = staServer.Install (wifiStaNode.Get (0));
  staServerApp.Start (Seconds (0.0));
  staServerApp.Stop (Seconds (simulationTime + 1));

  if (apHasTraffic)
    {
      UdpClientHelper apClient (staNodeInterface.GetAddress (0), 5001);
      apClient.SetAttribute ("MaxPackets", UintegerValue (4294967295u));
      apClient.SetAttribute ("Interval", TimeValue (Time ("0.00001"))); //packets/s
      apClient.SetAttribute ("PacketSize", UintegerValue (payloadSize)); //bytes
      ApplicationContainer apClientApp = apClient.Install (wifiApNode.Get (0));
      apClientApp.Start (Seconds (1.0));
      apClientApp.Stop (Seconds (simulationTime + 1));
    }

  if (staHasTraffic)
    {
      UdpClientHelper staClient (apNodeInterface.GetAddress (0), 9);
      staClient.SetAttribute ("MaxPackets", UintegerValue (4294967295u));
      staClient.SetAttribute ("Interval", TimeValue (Time ("0.00001"))); //packets/s
      staClient.SetAttribute ("PacketSize", UintegerValue (payloadSize)); //bytes
      ApplicationContainer staClientApp = staClient.Install (wifiStaNode.Get (0));
      staClientApp.Start (Seconds (1.0));
      staClientApp.Stop (Seconds (simulationTime + 1));
    }

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Stop (Seconds (simulationTime + 1));
  Simulator::Run ();
  Simulator::Destroy ();

  uint64_t rxBytes;
  double throughput;
  if (apHasTraffic)
    {
      rxBytes = payloadSize * DynamicCast<UdpServer> (staServerApp.Get (0))->GetReceived ();
      throughput = (rxBytes * 8) / (simulationTime * 1000000.0); //Mbit/s
      std::cout << "AP Throughput: " << throughput << " Mbit/s" << std::endl;
      if (throughput == 0)
        {
          NS_LOG_ERROR ("No traffic received!");
          exit (1);
        }
    }
  if (staHasTraffic)
    {
      rxBytes = payloadSize * DynamicCast<UdpServer> (apServerApp.Get (0))->GetReceived ();
      throughput = (rxBytes * 8) / (simulationTime * 1000000.0); //Mbit/s
      std::cout << "STA Throughput: " << throughput << " Mbit/s" << std::endl;
      if (throughput == 0)
        {
          NS_LOG_ERROR ("No traffic received!");
          exit (1);
        }
    }
  return 0;
}
