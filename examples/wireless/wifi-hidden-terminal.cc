/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 IITP RAS
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
 * Authors: Pavel Boyko <boyko@iitp.ru>
 */

/*
 * Classical hidden terminal problem and its RTS/CTS solution.
 *
 * Topology: [node 0] <-- -50 dB --> [node 1] <-- -50 dB --> [node 2]
 * 
 * This example illustrates the use of 
 *  - Wifi in ad-hoc mode
 *  - Matrix propagation loss model
 *  - Use of OnOffApplication to generate CBR stream 
 *  - IP flow monitor
 */
#include "ns3/core-module.h"
#include "ns3/common-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"
#include "ns3/mobility-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;

/// Run single 10 seconds experiment with enabled or disabled RTS/CTS mechanism
void experiment (bool enableCtsRts)
{
  // 0. Enable or disable CTS/RTS
  UintegerValue ctsThr = (enableCtsRts ? UintegerValue (100) : UintegerValue (2200));
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", ctsThr);
  
  // 1. Create 3 nodes 
  NodeContainer nodes;
  nodes.Create (3);
  
  // 2. Place nodes somehow, this is required by every wireless simulation
  for (size_t i = 0; i < 3; ++i)
    {
      nodes.Get(i)->AggregateObject (CreateObject<ConstantPositionMobilityModel> ());
    }
  
  // 3. Create propagation loss matrix
  Ptr<MatrixPropagationLossModel> lossModel = CreateObject<MatrixPropagationLossModel> ();
  lossModel->SetDefaultLoss (200); // set default loss to 200 dB (no link)
  lossModel->SetLoss (nodes.Get (0), nodes.Get (1), 50); // set symmetric loss 0 <-> 1 to 50 dB
  lossModel->SetLoss (nodes.Get (2), nodes.Get (1), 50); // set symmetric loss 2 <-> 1 to 50 dB

  // 4. Create & setup wifi channel
  Ptr<YansWifiChannel> wifiChannel = CreateObject <YansWifiChannel> ();
  wifiChannel->SetPropagationLossModel (lossModel);
  wifiChannel->SetPropagationDelayModel (CreateObject <ConstantSpeedPropagationDelayModel> ());
   
  // 5. Install wireless devices
  WifiHelper wifi;
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", 
                                "DataMode",StringValue ("DsssRate2Mbps"), 
                                "ControlMode",StringValue ("DsssRate1Mbps"));
  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  wifiPhy.SetChannel (wifiChannel);
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifiMac.SetType ("ns3::AdhocWifiMac"); // use ad-hoc MAC
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, nodes);
  
  // 6. Install TCP/IP stack & assign IP addresses
  InternetStackHelper internet;
  internet.Install (nodes);
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.0.0.0", "255.0.0.0");
  ipv4.Assign (devices);
  
  // 7. Install applications: two dense CBR streams  node 0 -> node 1 and node 2 -> node 1 
  ApplicationContainer apps;
  OnOffHelper onOffHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address ("10.0.0.2"), 9));
  onOffHelper.SetAttribute ("DataRate", StringValue ("10Mbps"));
  onOffHelper.SetAttribute ("PacketSize", UintegerValue (200));
  onOffHelper.SetAttribute ("OnTime",  RandomVariableValue (ConstantVariable (1)));
  onOffHelper.SetAttribute ("OffTime", RandomVariableValue (ConstantVariable (0)));
  for (size_t i = 0; i < 3; i += 2)
    {
      apps.Add (onOffHelper.Install (nodes.Get (i)));
    }
  
  // 8. Install FlowMonitor on all nodes
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll();
  
  // 9. Run simulation for 10 seconds
  Simulator::Stop (Seconds (10));
  Simulator::Run ();
  
  // 10. Print per flow statistics
  monitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
  std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
      Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
      std::cout << "Flow " << i->first << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
      std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
      std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
      std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / 10.0 / 1024 / 1024  << " Mbps\n";
    }
  
  // 11. Cleanup
  Simulator::Destroy ();
}

int main (int argc, char **argv)
{
  std::cout << "Hidden station experiment with RTS/CTS disabled:\n" << std::flush;
  experiment (false);
  std::cout << "------------------------------------------------\n";
  std::cout << "Hidden station experiment with RTS/CTS enabled:\n";
  experiment (true);
  
  return 0;
}
