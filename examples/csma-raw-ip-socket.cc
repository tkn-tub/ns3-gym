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

// Port of ns-2/tcl/ex/simple.tcl to ns-3
//
// Network topology
//
//       n0    n1   n2   n3
//       |     |    |    |
//     =====================
//
// - CBR/UDP flows from n0 to n1, and from n3 to n0
// - UDP packet size of 210 bytes, with per-packet interval 0.00375 sec.
//   (i.e., DataRate of 448,000 bps)
// - DropTail queues 
// - Tracing of queues and packet receptions to file "csma-one-subnet.tr"

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CsmaPacketSocketExample");

static void SinkRx (Ptr<const Packet> p, const Address &ad)
{
  //std::cout << *p << std::endl;
}

int
main (int argc, char *argv[])
{
#if 0 
  LogComponentEnable ("CsmaPacketSocketExample", LOG_LEVEL_INFO);
#endif

  CommandLine cmd;
  cmd.Parse (argc, argv);

  // Here, we will explicitly create four nodes.
  NS_LOG_INFO ("Create nodes.");
  NodeContainer c;
  c.Create (4);

  // connect all our nodes to a shared channel.
  NS_LOG_INFO ("Build Topology.");
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", DataRateValue (DataRate (5000000)));
  csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
  csma.SetDeviceAttribute ("EncapsulationMode", StringValue ("Llc"));
  NetDeviceContainer devs = csma.Install (c);

  // add an ip stack to all nodes.
  NS_LOG_INFO ("Add ip stack.");
  InternetStackHelper ipStack;
  ipStack.Install (c);

  // assign ip addresses
  NS_LOG_INFO ("Assign ip addresses.");
  Ipv4AddressHelper ip;
  ip.SetBase ("192.168.1.0", "255.255.255.0");
  Ipv4InterfaceContainer addresses = ip.Assign (devs);

  NS_LOG_INFO ("Create Source");
  Config::SetDefault ("ns3::Ipv4RawSocketImpl::Protocol", StringValue ("2"));
  InetSocketAddress dst = InetSocketAddress (addresses.GetAddress (3));
  OnOffHelper onoff = OnOffHelper ("ns3::Ipv4RawSocketFactory", dst);
  onoff.SetAttribute ("OnTime", RandomVariableValue (ConstantVariable (1.0)));
  onoff.SetAttribute ("OffTime", RandomVariableValue (ConstantVariable (0.0)));
  onoff.SetAttribute ("DataRate", DataRateValue (DataRate (6000)));
  onoff.SetAttribute ("PacketSize", UintegerValue (1200));

  ApplicationContainer apps = onoff.Install (c.Get (0));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  NS_LOG_INFO ("Create Sink.");
  PacketSinkHelper sink = PacketSinkHelper ("ns3::Ipv4RawSocketFactory", dst);
  apps = sink.Install (c.Get (3));
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (11.0));

  NS_LOG_INFO ("Configure Tracing.");
  // first, pcap tracing.
  csma.EnablePcapAll ("csma-raw-ip-socket");
  // then, print what the packet sink receives.
  Config::ConnectWithoutContext ("/NodeList/3/ApplicationList/0/$ns3::PacketSink/Rx", 
                                 MakeCallback (&SinkRx));

  Packet::EnablePrinting ();
  

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}
