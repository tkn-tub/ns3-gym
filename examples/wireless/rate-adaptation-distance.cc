/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Universidad de la República - Uruguay
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
 * Author: Matías Richart <mrichart@fing.edu.uy>
 */

/**
 * This example program is designed to illustrate the behavior of
 * rate-adaptive WiFi rate controls such as Minstrel.  Power-adaptive
 * rate controls can be illustrated also, but separate examples exist for
 * highlighting the power adaptation.
 *
 * This simulation consist of 2 nodes, one AP and one STA.
 * The AP generates UDP traffic with a CBR of 54 Mbps to the STA.
 * The AP can use any power and rate control mechanism and the STA uses
 * only Minstrel rate control.
 * The STA can be configured to move away from (or towards to) the AP.
 * By default, the AP is at coordinate (0,0,0) and the STA starts at
 * coordinate (5,0,0) (meters) and moves away on the x axis by 1 meter every
 * second.
 *
 * The output consists of:
 * - A plot of average throughput vs. distance.
 * - (if logging is enabled) the changes of rate to standard output.
 *
 * Example usage:
 * ./waf --run "rate-adaptation-distance --manager=ns3::MinstrelWifiManager --outputFileName=minstrel"
 *
 * Another example (moving towards the AP):
 * ./waf --run "rate-adaptation-distance --manager=ns3::MinstrelWifiManager --outputFileName=minstrel --stepsSize=1 --STA1_x=-200"
 *
 * Example for HT rates with SGI and channel width of 40MHz:
 * ./waf --run "rate-adaptation-distance --manager=ns3::MinstrelHtWifiManager --outputFileName=minstrelHt --shortGuardInterval=true --channelWidth=40"
 *
 * To enable the log of rate changes:
 * export NS_LOG=RateAdaptationDistance=level_info
 */

#include <sstream>
#include <fstream>
#include <math.h>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/applications-module.h"
#include "ns3/stats-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("RateAdaptationDistance");

class NodeStatistics
{
public:
  NodeStatistics (NetDeviceContainer aps, NetDeviceContainer stas);

  void CheckStatistics (double time);

  void RxCallback (std::string path, Ptr<const Packet> packet, const Address &from);
  void SetPosition (Ptr<Node> node, Vector position);
  void AdvancePosition (Ptr<Node> node, int stepsSize, int stepsTime);
  Vector GetPosition (Ptr<Node> node);

  Gnuplot2dDataset GetDatafile ();

private:
  uint32_t m_bytesTotal;
  Gnuplot2dDataset m_output;
};

NodeStatistics::NodeStatistics (NetDeviceContainer aps, NetDeviceContainer stas)
{
  m_bytesTotal = 0;
}

void
NodeStatistics::RxCallback (std::string path, Ptr<const Packet> packet, const Address &from)
{
  m_bytesTotal += packet->GetSize ();
}

void
NodeStatistics::CheckStatistics (double time)
{

}

void
NodeStatistics::SetPosition (Ptr<Node> node, Vector position)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  mobility->SetPosition (position);
}

Vector
NodeStatistics::GetPosition (Ptr<Node> node)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  return mobility->GetPosition ();
}

void
NodeStatistics::AdvancePosition (Ptr<Node> node, int stepsSize, int stepsTime)
{
  Vector pos = GetPosition (node);
  double mbs = ((m_bytesTotal * 8.0) / (1000000 * stepsTime));
  m_bytesTotal = 0;
  m_output.Add (pos.x, mbs);
  pos.x += stepsSize;
  SetPosition (node, pos);
  Simulator::Schedule (Seconds (stepsTime), &NodeStatistics::AdvancePosition, this, node, stepsSize, stepsTime);
}

Gnuplot2dDataset
NodeStatistics::GetDatafile ()
{
  return m_output;
}


void RateCallback (std::string path, uint64_t rate, Mac48Address dest)
{
  NS_LOG_INFO ((Simulator::Now ()).GetSeconds () << " " << dest << " Rate " <<  rate / 1000000.0);
}

int main (int argc, char *argv[])
{
  uint32_t rtsThreshold = 65535;
  std::string staManager = "ns3::MinstrelHtWifiManager";
  std::string apManager = "ns3::MinstrelHtWifiManager";
  std::string standard = "802.11n-5GHz";
  std::string outputFileName = "minstrelHT";
  uint32_t BE_MaxAmpduSize = 65535;
  bool shortGuardInterval = false;
  uint32_t chWidth = 20;
  int ap1_x = 0;
  int ap1_y = 0;
  int sta1_x = 5;
  int sta1_y = 0;
  int steps = 100;
  int stepsSize = 1;
  int stepsTime = 1;

  CommandLine cmd;
  cmd.AddValue ("staManager", "PRC Manager of the STA", staManager);
  cmd.AddValue ("apManager", "PRC Manager of the AP", apManager);
  cmd.AddValue ("standard", "Wifi Phy Standard", standard);
  cmd.AddValue ("shortGuardInterval", "Enable Short Guard Interval in all stations", shortGuardInterval);
  cmd.AddValue ("channelWidth", "Channel width of all the stations", chWidth);
  cmd.AddValue ("rtsThreshold", "RTS threshold", rtsThreshold);
  cmd.AddValue ("BE_MaxAmpduSize", "BE Mac A-MPDU size", BE_MaxAmpduSize);
  cmd.AddValue ("outputFileName", "Output filename", outputFileName);
  cmd.AddValue ("steps", "How many different distances to try", steps);
  cmd.AddValue ("stepsTime", "Time on each step", stepsTime);
  cmd.AddValue ("stepsSize", "Distance between steps", stepsSize);
  cmd.AddValue ("AP1_x", "Position of AP1 in x coordinate", ap1_x);
  cmd.AddValue ("AP1_y", "Position of AP1 in y coordinate", ap1_y);
  cmd.AddValue ("STA1_x", "Position of STA1 in x coordinate", sta1_x);
  cmd.AddValue ("STA1_y", "Position of STA1 in y coordinate", sta1_y);
  cmd.Parse (argc, argv);

  int simuTime = steps * stepsTime;

  // Define the APs
  NodeContainer wifiApNodes;
  wifiApNodes.Create (1);

  //Define the STAs
  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (1);


  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());

  wifiPhy.Set("ShortGuardEnabled", BooleanValue(shortGuardInterval));

  NetDeviceContainer wifiApDevices;
  NetDeviceContainer wifiStaDevices;
  NetDeviceContainer wifiDevices;

  WifiHelper wifi;
  if (standard == "802.11a" || standard == "802.11b" || standard == "802.11g")
    {
      if (standard == "802.11a")
        {
          wifi.SetStandard (WIFI_PHY_STANDARD_80211a);
        }
      else if (standard == "802.11b")
        {
          wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
        }
      else if (standard == "802.11g")
        {
          wifi.SetStandard (WIFI_PHY_STANDARD_80211g);
        }
      NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();

      //Configure the STA node
      wifi.SetRemoteStationManager (staManager, "RtsCtsThreshold", UintegerValue (rtsThreshold));

      Ssid ssid = Ssid ("AP");
      wifiMac.SetType ("ns3::StaWifiMac",
                       "Ssid", SsidValue (ssid));
      wifiStaDevices.Add (wifi.Install (wifiPhy, wifiMac, wifiStaNodes.Get (0)));

      //Configure the AP node
      wifi.SetRemoteStationManager (apManager, "RtsCtsThreshold", UintegerValue (rtsThreshold));

      ssid = Ssid ("AP");
      wifiMac.SetType ("ns3::ApWifiMac",
                       "Ssid", SsidValue (ssid));
      wifiApDevices.Add (wifi.Install (wifiPhy, wifiMac, wifiApNodes.Get (0)));
    }
  else if (standard == "802.11n-2.4GHz" || standard == "802.11n-5GHz")
    {
      if (standard == "802.11n-2.4GHz")
        {
          wifi.SetStandard (WIFI_PHY_STANDARD_80211n_2_4GHZ);
        }
      else if (standard == "802.11n-5GHz")
        {
          wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
        }

      HtWifiMacHelper wifiMac = HtWifiMacHelper::Default ();

      //Configure the STA node
      wifi.SetRemoteStationManager (staManager, "RtsCtsThreshold", UintegerValue (rtsThreshold));

      Ssid ssid = Ssid ("AP");
      wifiMac.SetType ("ns3::StaWifiMac",
                       "Ssid", SsidValue (ssid),
                       "BE_MaxAmpduSize", UintegerValue (BE_MaxAmpduSize));
      wifiStaDevices.Add (wifi.Install (wifiPhy, wifiMac, wifiStaNodes.Get (0)));

      //Configure the AP node
      wifi.SetRemoteStationManager (apManager, "RtsCtsThreshold", UintegerValue (rtsThreshold));

      ssid = Ssid ("AP");
      wifiMac.SetType ("ns3::ApWifiMac",
                       "Ssid", SsidValue (ssid),
                       "BE_MaxAmpduSize", UintegerValue (BE_MaxAmpduSize));
      wifiApDevices.Add (wifi.Install (wifiPhy, wifiMac, wifiApNodes.Get (0)));
    }
  else if (standard == "802.11ac")
    {
      wifi.SetStandard (WIFI_PHY_STANDARD_80211ac);
      VhtWifiMacHelper wifiMac = VhtWifiMacHelper::Default ();

      //Configure the STA node
      wifi.SetRemoteStationManager (staManager, "RtsCtsThreshold", UintegerValue (rtsThreshold));

      Ssid ssid = Ssid ("AP");
      wifiMac.SetType ("ns3::StaWifiMac",
                       "Ssid", SsidValue (ssid),
                       "BE_MaxAmpduSize", UintegerValue (BE_MaxAmpduSize));
      wifiStaDevices.Add (wifi.Install (wifiPhy, wifiMac, wifiStaNodes.Get (0)));

      //Configure the AP node
      wifi.SetRemoteStationManager (apManager, "RtsCtsThreshold", UintegerValue (rtsThreshold));

      ssid = Ssid ("AP");
      wifiMac.SetType ("ns3::ApWifiMac",
                       "Ssid", SsidValue (ssid),
                       "BE_MaxAmpduSize", UintegerValue (BE_MaxAmpduSize));
      wifiApDevices.Add (wifi.Install (wifiPhy, wifiMac, wifiApNodes.Get (0)));
    }

  wifiDevices.Add (wifiStaDevices);
  wifiDevices.Add (wifiApDevices);

  // Set channel width
  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/ChannelWidth", UintegerValue (chWidth));

  // Configure the mobility.
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  //Initial position of AP and STA
  positionAlloc->Add (Vector (ap1_x, ap1_y, 0.0));
  positionAlloc->Add (Vector (sta1_x, sta1_y, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNodes.Get (0));
  mobility.Install (wifiStaNodes.Get (0));

  //Statistics counter
  NodeStatistics atpCounter = NodeStatistics (wifiApDevices, wifiStaDevices);

  //Move the STA by stepsSize meters every stepsTime seconds
  Simulator::Schedule (Seconds (0.5 + stepsTime), &NodeStatistics::AdvancePosition, &atpCounter, wifiStaNodes.Get (0), stepsSize, stepsTime);

  //Configure the IP stack
  InternetStackHelper stack;
  stack.Install (wifiApNodes);
  stack.Install (wifiStaNodes);
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = address.Assign (wifiDevices);
  Ipv4Address sinkAddress = i.GetAddress (0);
  uint16_t port = 9;

  //Configure the CBR generator
  PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress (sinkAddress, port));
  ApplicationContainer apps_sink = sink.Install (wifiStaNodes.Get (0));

  OnOffHelper onoff ("ns3::UdpSocketFactory", InetSocketAddress (sinkAddress, port));
  onoff.SetConstantRate (DataRate ("200Mb/s"), 1420);
  onoff.SetAttribute ("StartTime", TimeValue (Seconds (0.5)));
  onoff.SetAttribute ("StopTime", TimeValue (Seconds (simuTime)));
  ApplicationContainer apps_source = onoff.Install (wifiApNodes.Get (0));

  apps_sink.Start (Seconds (0.5));
  apps_sink.Stop (Seconds (simuTime));

  //------------------------------------------------------------
  //-- Setup stats and data collection
  //--------------------------------------------

  //Register packet receptions to calculate throughput
  Config::Connect ("/NodeList/1/ApplicationList/*/$ns3::PacketSink/Rx",
                   MakeCallback (&NodeStatistics::RxCallback, &atpCounter));

  //Callbacks to print every change of rate
  Config::Connect ("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/RemoteStationManager/$" + apManager + "/RateChange",
                   MakeCallback (RateCallback));

  Simulator::Stop (Seconds (simuTime));
  Simulator::Run ();

  std::ofstream outfile (("throughput-" + outputFileName + ".plt").c_str ());
  Gnuplot gnuplot = Gnuplot (("throughput-" + outputFileName + ".eps").c_str (), "Throughput");
  gnuplot.SetTerminal ("post eps color enhanced");
  gnuplot.SetLegend ("Time (seconds)", "Throughput (Mb/s)");
  gnuplot.SetTitle ("Throughput (AP to STA) vs time");
  gnuplot.AddDataset (atpCounter.GetDatafile ());
  gnuplot.GenerateOutput (outfile);

  Simulator::Destroy ();

  return 0;
}
