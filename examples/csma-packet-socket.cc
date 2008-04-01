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
#include "ns3/helper-module.h"

#include "ns3/ascii-trace.h"
#include "ns3/pcap-trace.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CsmaPacketSocketExample");

int
main (int argc, char *argv[])
{
#if 0 
  LogComponentEnable ("CsmaPacketSocketExample", LOG_LEVEL_INFO);
#endif

  //
  // Make the random number generators generate reproducible results.
  //
  RandomVariable::UseGlobalSeed (1, 1, 2, 3, 5, 8);

  CommandLine cmd;
  cmd.Parse (argc, argv);

  // Here, we will explicitly create four nodes.
  NS_LOG_INFO ("Create nodes.");
  NodeContainer c;
  c.Create (4);

  PacketSocketHelper packetSocket;
  packetSocket.Build (c);

  // create the shared medium used by all csma devices.
  NS_LOG_INFO ("Create channels.");
  Ptr<CsmaChannel> channel = CreateObject<CsmaChannel> ("BitRate", DataRate(5000000), 
                                                        "Delay", MilliSeconds(2));

  // use a helper function to connect our nodes to the shared channel.
  NS_LOG_INFO ("Build Topology.");
  CsmaHelper csma;
  csma.SetDeviceParameter ("EncapsulationMode", String ("Llc"));
  NetDeviceContainer devs = csma.Build (c, channel);

  NS_LOG_INFO ("Create Applications.");
  // Create the OnOff application to send raw datagrams
  OnOffHelper onoff;
  onoff.SetAppAttribute ("OnTime", ConstantVariable (1.0));
  onoff.SetAppAttribute ("OffTime", ConstantVariable (0.0));
  onoff.SetPacketRemote (devs.Get (0), devs.Get (1)->GetAddress (), 2);
  ApplicationContainer apps = onoff.Build (c.Get (0));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));
  

  onoff.SetPacketRemote (devs.Get (3), devs.Get (0)->GetAddress (), 3);
  apps = onoff.Build (c.Get (3));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));
 
  // Configure tracing of all enqueue, dequeue, and NetDevice receive events
  // Trace output will be sent to the csma-packet-socket.tr file
  NS_LOG_INFO ("Configure Tracing.");
  AsciiTrace asciitrace ("csma-packet-socket.tr");
  asciitrace.TraceAllNetDeviceRx ();
  asciitrace.TraceAllQueues ();

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}
