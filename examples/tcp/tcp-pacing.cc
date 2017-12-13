/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */

// Network topology
//
//       n0 ----------- n1
//            40 Gbps
//            0.01 ms

// This programs illustrates how TCP pacing can be used and how user can set
// pacing rate. The program gives information about each flow like transmitted
// and received bytes (packets) and throughput of that flow. Currently, it is 
// using TCP NewReno but in future after having congestion control algorithms 
// which can change pacing rate can be used.

#include <string>
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TcpPacingExample");

int
main (int argc, char *argv[])
{

  bool tracing = false;
  uint32_t maxBytes = 0;
  uint32_t TCPFlows = 1;
  bool isPacingEnabled = true;
  std::string pacingRate = "4Gbps";
  bool isSack = false;
  uint32_t maxPackets = 0;

  CommandLine cmd;
  cmd.AddValue ("tracing", "Flag to enable/disable tracing", tracing);
  cmd.AddValue ("maxBytes",
                "Total number of bytes for application to send", maxBytes);
  cmd.AddValue ("maxPackets",
                "Total number of bytes for application to send", maxPackets);
  cmd.AddValue ("TCPFlows", "Number of application flows between sender and receiver", TCPFlows);
  cmd.AddValue ("Pacing", "Flag to enable/disable pacing in TCP", isPacingEnabled);
  cmd.AddValue ("Sack", "Flag to enable/disable sack in TCP", isSack);
  cmd.AddValue ("PacingRate", "Max Pacing Rate in bps", pacingRate);
  cmd.Parse (argc, argv);

  if (maxPackets != 0 )
    {
      maxBytes = 500 * maxPackets;
    }

  Config::SetDefault ("ns3::TcpSocketState::MaxPacingRate", StringValue (pacingRate));
  Config::SetDefault ("ns3::TcpSocketState::EnablePacing", BooleanValue (isPacingEnabled));
  Config::SetDefault ("ns3::TcpSocketBase::Sack", BooleanValue (isSack));

  NS_LOG_INFO ("Create nodes.");
  NodeContainer nodes;
  nodes.Create (2);

  NS_LOG_INFO ("Create channels.");
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("40Gbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("0.01ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  InternetStackHelper internet;
  internet.Install (nodes);

  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Assign (devices);

  NS_LOG_INFO ("Create Applications.");

  ApplicationContainer sourceApps;
  ApplicationContainer sinkApps;
  for (uint32_t iterator = 0; iterator < TCPFlows; iterator++)
    {
      uint16_t port = 10000 + iterator;

      BulkSendHelper source  ("ns3::TcpSocketFactory",
                              InetSocketAddress (i.GetAddress (1), port));
      // Set the amount of data to send in bytes.  Zero is unlimited.
      source.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
      sourceApps.Add (source.Install (nodes.Get (0)));

      PacketSinkHelper sink ("ns3::TcpSocketFactory",
                             InetSocketAddress (Ipv4Address::GetAny (), port));
      sinkApps.Add (sink.Install (nodes.Get (1)));
    }

  sinkApps.Start (Seconds (0.0));
  sinkApps.Stop (Seconds (5));
  sourceApps.Start (Seconds (1));
  sourceApps.Stop (Seconds (5));

  if (tracing)
    {
      AsciiTraceHelper ascii;
      pointToPoint.EnableAsciiAll (ascii.CreateFileStream ("tcp-pacing.tr"));
      pointToPoint.EnablePcapAll ("tcp-pacing", false);
    }

  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll ();

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop (Seconds (5));
  Simulator::Run ();

  monitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
  FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats ();
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
      Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
      if (t.sourceAddress == "10.1.1.2")
        {
          continue;
        }
      std::cout << "Flow " << i->first  << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
      std::cout << "  Tx Packets: " << i->second.txPackets << "\n";
      std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
      std::cout << "  TxOffered:  " << i->second.txBytes * 8.0 / 9.0 / 1000 / 1000  << " Mbps\n";
      std::cout << "  Rx Packets: " << i->second.rxPackets << "\n";
      std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
      std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / 9.0 / 1000 / 1000  << " Mbps\n";
    }

  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}
