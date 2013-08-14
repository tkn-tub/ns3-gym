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
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"

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

NS_LOG_COMPONENT_DEFINE ("DataRates");

double rxBytessum=0;
double throughput=0;


//===========================================================================
//Set position of the nodes
//===========================================================================
static void
SetPosition (Ptr<Node> node, Vector position)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  mobility->SetPosition (position);
}

//==========================================================================
//==========================================================================

int main (int argc, char *argv[])
{
  std::cout << "DataRate" <<"  " << "Throughput" << '\n';
  bool udp = true;
  int i=2;
  for (;i <= 2; i++)
  {
    uint32_t nWifi = 1;
    CommandLine cmd;
    cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
    cmd.Parse (argc,argv);
    Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("990000"));
    // disable rts cts all the time.
    Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("99000"));
    NodeContainer wifiNodes;
    wifiNodes.Create (1);
    NodeContainer wifiApNode;
    wifiApNode.Create (1);
 
    YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
    YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
    phy.SetChannel (channel.Create ());
    if (i ==3 || i == 4)
      phy.Set ("ShortGuardEnabled",BooleanValue(true));
      //phy.Set ("GreenfieldEnabled",BooleanValue(true));

    WifiHelper wifi = WifiHelper::Default ();
    wifi.SetStandard (WIFI_PHY_STANDARD_80211n_2_4GHZ);
    HtWifiMacHelper mac = HtWifiMacHelper::Default ();
 

    Ssid ssid = Ssid ("ns380211n");
    double datarate;
    StringValue DataRate;
    if (i==0)
      {
        DataRate = StringValue("OfdmRate6_5MbpsBW20MHz");
        datarate = 6.5;
      }
    else if (i==1)
      {
        DataRate = StringValue("OfdmRate58_5MbpsBW20MHz");
        datarate = 58.5;
      }
    else if (i == 2)
      {
        DataRate = StringValue("OfdmRate65MbpsBW20MHz");
        datarate = 65;
      }
    else if (i == 3)
      {
        DataRate = StringValue("OfdmRate57_8MbpsBW20MHz");
        datarate = 57.8;
      }
    else if (i == 4)
      {
        DataRate = StringValue("OfdmRate72_2MbpsBW20MHz");
        datarate = 72.2;
      }

    wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager","DataMode", DataRate,
                                   "ControlMode", DataRate);
    mac.SetType ("ns3::StaWifiMac",
                 "Ssid", SsidValue (ssid),
                 "ActiveProbing", BooleanValue (false));
    
    NetDeviceContainer staDevices;
    staDevices = wifi.Install (phy, mac, wifiNodes);

    mac.SetType ("ns3::ApWifiMac",
                 "Ssid", SsidValue (ssid));
   
    NetDeviceContainer apDevice;
    apDevice = wifi.Install (phy, mac, wifiApNode);
   /* Ptr<WifiRemoteStationManager> apStationManager =
              DynamicCast<WifiNetDevice>(apDevice.Get (0))->GetRemoteStationManager ();
    apStationManager->AddBasicMode (WifiMode ("OfdmRate13MbpsBW20MHz"));
    apStationManager->AddBasicMode (WifiMode ("OfdmRate19_5MbpsBW20MHz"));
    apStationManager->AddBasicMode (WifiMode ("OfdmRate26MbpsBW20MHz"));
    apStationManager->AddBasicMode (WifiMode ("OfdmRate39MbpsBW20MHz"));
    Ptr<WifiRemoteStationManager> staStationManager =
              DynamicCast<WifiNetDevice> (staDevices.Get (0))->GetRemoteStationManager ();
    staStationManager->AddBasicMode (WifiMode ("OfdmRate13MbpsBW20MHz"));
    staStationManager->AddBasicMode (WifiMode ("OfdmRate19_5MbpsBW20MHz"));
    staStationManager->AddBasicMode (WifiMode ("OfdmRate26MbpsBW20MHz"));
    staStationManager->AddBasicMode (WifiMode ("OfdmRate39MbpsBW20MHz"));*/
              
   // mobility.
   MobilityHelper mobility;
   mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
   mobility.Install (wifiNodes);
   mobility.Install (wifiApNode);
    
   SetPosition (wifiNodes.Get(0), Vector (1.0,0.0,0.0));
   SetPosition (wifiApNode.Get(0), Vector (0.0,0.0,0.0));
 

   /* Internet stack*/
   InternetStackHelper stack;
   stack.Install (wifiApNode);
   stack.Install (wifiNodes);

   Ipv4AddressHelper address;

   address.SetBase ("10.1.3.0", "255.255.255.0");
   Ipv4InterfaceContainer wifiNodesInterfaces;
   Ipv4InterfaceContainer apNodeInterface;

   wifiNodesInterfaces = address.Assign (staDevices);
   apNodeInterface = address.Assign (apDevice);

   ApplicationContainer serverApps,sink1App;

   double t=10;

   /* Setting applications */
   if (udp)
     {
       UdpServerHelper myServer (9);
       serverApps = myServer.Install (wifiNodes.Get (0));
       serverApps.Start (Seconds (0.0));
       serverApps.Stop (Seconds (t));

       UdpClientHelper myClient (wifiNodesInterfaces.GetAddress (0), 9);
       myClient.SetAttribute ("MaxPackets", UintegerValue (64707202));
       myClient.SetAttribute ("Interval", TimeValue (Time ("0.00002")));
       myClient.SetAttribute ("PacketSize", UintegerValue (1500));
 
       ApplicationContainer clientApps = myClient.Install (wifiApNode.Get (0));
       clientApps.Start (Seconds (0.0));
       clientApps.Stop (Seconds (t));
     }
   else
     {

       //TCP flow
       uint16_t port = 50000;
       Address apLocalAddress (InetSocketAddress (Ipv4Address::GetAny (), port));
       PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", apLocalAddress);
       sink1App = packetSinkHelper.Install (wifiNodes.Get (0));

       sink1App.Start (Seconds (0.0));
       sink1App.Stop (Seconds (t));
            
       OnOffHelper onoff ("ns3::TcpSocketFactory",Ipv4Address::GetAny ());
       onoff.SetAttribute ("OnTime", RandomVariableValue (ConstantVariable(30)));//in seconds
       onoff.SetAttribute ("OffTime", RandomVariableValue (ConstantVariable(0)));
       onoff.SetAttribute ("PacketSize", UintegerValue (1500-30));//1024
       onoff.SetAttribute ("DataRate", DataRateValue (100000000));//51200
       ApplicationContainer apps;

       AddressValue remoteAddress (InetSocketAddress (wifiNodesInterfaces.GetAddress(0), port));
       onoff.SetAttribute ("Remote", remoteAddress);
       apps.Add (onoff.Install (wifiApNode.Get (0)));
       apps.Start (Seconds (0.0));
       apps.Stop (Seconds (t));
     }

  
   Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

   Simulator::Stop (Seconds (t));
   Simulator::Run ();
   Simulator::Destroy ();

   //UDP
   if (udp)
     {
       uint32_t totalPacketsThrough = DynamicCast<UdpServer>(serverApps.Get (0))->GetReceived ();
       throughput=totalPacketsThrough*1500*8/(t*1000000.0);
     }
   else
     {
       //TCP
       uint32_t totalPacketsThrough = DynamicCast<PacketSink>(sink1App.Get (0))->GetTotalRx ();
       throughput=totalPacketsThrough*8/((t-3)*1000000.0);
     }
  
   std::cout << datarate <<"  " << throughput << '\n';
   }
  return 0;
} 
