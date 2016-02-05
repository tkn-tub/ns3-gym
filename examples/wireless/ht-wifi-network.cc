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
 * Authors: Mirko Banchi <mk.banchi@gmail.com>
 *          Sebastien Deronne <sebastien.deronne@gmail.com>
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"

// This is a simple example in order to show how to configure an IEEE 802.11n Wi-Fi network.
//
// It ouputs the UDP or TCP goodput for every VHT bitrate value, which depends on the MCS value (0 to 7), the
// channel width (20 or 40 MHz) and the guard interval (long or short). The PHY bitrate is constant over all
// the simulation run. The user can also specify the distance between the access point and the station: the
// larger the distance the smaller the goodput.
//
// The simulation assumes a single station in an infrastructure network:
//
//  STA     AP
//    *     *
//    |     |
//   n1     n2
//
//Packets in this simulation aren't marked with a QosTag so they are considered
//belonging to BestEffort Access Class (AC_BE).

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ht-wifi-network");

int main (int argc, char *argv[])
{
  bool udp = true;
  double simulationTime = 10; //seconds
  double distance = 1.0; //meters
  double frequency = 5.0; //whether 2.4 or 5.0 GHz
  
  CommandLine cmd;
  cmd.AddValue ("frequency", "Whether working in the 2.4 or 5.0 GHz band (other values gets rejected)", frequency);
  cmd.AddValue ("distance", "Distance in meters between the station and the access point", distance);
  cmd.AddValue ("simulationTime", "Simulation time in seconds", simulationTime);
  cmd.AddValue ("udp", "UDP if set to 1, TCP otherwise", udp);
  cmd.Parse (argc,argv);

  std::cout << "MCS value" << "\t\t" << "Channel width" << "\t\t" << "short GI" << "\t\t" << "Throughput" << '\n';
  for (int i = 0; i <= 7; i++) //MCS
    {
      for (int j = 20; j <= 40; ) //channel width
        {
          for (int k = 0; k < 2; k++) //GI: 0 and 1
            {
              uint32_t payloadSize; //1500 byte IP packet
              if (udp)
                {
                  payloadSize = 1472; //bytes
                }
              else
                {
                  payloadSize = 1448; //bytes
                  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (payloadSize));
                }

              NodeContainer wifiStaNode;
              wifiStaNode.Create (1);
              NodeContainer wifiApNode;
              wifiApNode.Create (1);

              YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
              YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
              phy.SetChannel (channel.Create ());

              // Set guard interval
              phy.Set ("ShortGuardEnabled", BooleanValue (k));

              WifiMacHelper mac;
              WifiHelper wifi;
              if (frequency == 5.0)
                {
                  wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
                }
              else if (frequency == 2.4)
                {
                  wifi.SetStandard (WIFI_PHY_STANDARD_80211n_2_4GHZ);
                  Config::SetDefault ("ns3::LogDistancePropagationLossModel::ReferenceLoss", DoubleValue (40.046));
                }
              else
                {
                  std::cout<<"Wrong frequency value!"<<std::endl;
                  return 0;
                }

              std::ostringstream oss;
              oss << "HtMcs" << i;
              wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager","DataMode", StringValue (oss.str ()),
                                            "ControlMode", StringValue (oss.str ()));
                
              Ssid ssid = Ssid ("ns3-80211n");

              mac.SetType ("ns3::StaWifiMac",
                           "Ssid", SsidValue (ssid),
                           "ActiveProbing", BooleanValue (false));

              NetDeviceContainer staDevice;
              staDevice = wifi.Install (phy, mac, wifiStaNode);

              mac.SetType ("ns3::ApWifiMac",
                           "Ssid", SsidValue (ssid));

              NetDeviceContainer apDevice;
              apDevice = wifi.Install (phy, mac, wifiApNode);

              // Set channel width
              Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/ChannelWidth", UintegerValue (j));

              // mobility.
              MobilityHelper mobility;
              Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();

              positionAlloc->Add (Vector (0.0, 0.0, 0.0));
              positionAlloc->Add (Vector (distance, 0.0, 0.0));
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
              Ipv4InterfaceContainer staNodeInterface;
              Ipv4InterfaceContainer apNodeInterface;

              staNodeInterface = address.Assign (staDevice);
              apNodeInterface = address.Assign (apDevice);

              /* Setting applications */
              ApplicationContainer serverApp, sinkApp;
              if (udp)
                {
                  //UDP flow
                  UdpServerHelper myServer (9);
                  serverApp = myServer.Install (wifiStaNode.Get (0));
                  serverApp.Start (Seconds (0.0));
                  serverApp.Stop (Seconds (simulationTime + 1));

                  UdpClientHelper myClient (staNodeInterface.GetAddress (0), 9);
                  myClient.SetAttribute ("MaxPackets", UintegerValue (4294967295u));
                  myClient.SetAttribute ("Interval", TimeValue (Time ("0.00001"))); //packets/s
                  myClient.SetAttribute ("PacketSize", UintegerValue (payloadSize));

                  ApplicationContainer clientApp = myClient.Install (wifiApNode.Get (0));
                  clientApp.Start (Seconds (1.0));
                  clientApp.Stop (Seconds (simulationTime + 1));
                }
              else
                {
                  //TCP flow
                  uint16_t port = 50000;
                  Address apLocalAddress (InetSocketAddress (Ipv4Address::GetAny (), port));
                  PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", apLocalAddress);
                  sinkApp = packetSinkHelper.Install (wifiStaNode.Get (0));

                  sinkApp.Start (Seconds (0.0));
                  sinkApp.Stop (Seconds (simulationTime + 1));

                  OnOffHelper onoff ("ns3::TcpSocketFactory",Ipv4Address::GetAny ());
                  onoff.SetAttribute ("OnTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
                  onoff.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
                  onoff.SetAttribute ("PacketSize", UintegerValue (payloadSize));
                  onoff.SetAttribute ("DataRate", DataRateValue (1000000000)); //bit/s
                  ApplicationContainer apps;

                  AddressValue remoteAddress (InetSocketAddress (staNodeInterface.GetAddress (0), port));
                  onoff.SetAttribute ("Remote", remoteAddress);
                  apps.Add (onoff.Install (wifiApNode.Get (0)));
                  apps.Start (Seconds (1.0));
                  apps.Stop (Seconds (simulationTime + 1));
                }

              Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

              Simulator::Stop (Seconds (simulationTime + 1));
              Simulator::Run ();
              Simulator::Destroy ();

              double throughput = 0;
              if (udp)
                {
                  //UDP
                  uint32_t totalPacketsThrough = DynamicCast<UdpServer> (serverApp.Get (0))->GetReceived ();
                  throughput = totalPacketsThrough * payloadSize * 8 / (simulationTime * 1000000.0); //Mbit/s
                }
              else
                {
                  //TCP
                  uint32_t totalPacketsThrough = DynamicCast<PacketSink> (sinkApp.Get (0))->GetTotalRx ();
                  throughput = totalPacketsThrough * 8 / (simulationTime * 1000000.0); //Mbit/s
                }
              std::cout << i << "\t\t\t" << j << " MHz\t\t\t" << k << "\t\t\t" << throughput << " Mbit/s" << std::endl;
            }
          j *= 2;
        }
    }
  return 0;
}
