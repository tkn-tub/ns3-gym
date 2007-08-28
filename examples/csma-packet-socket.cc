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

#include "ns3/command-line.h"
#include "ns3/default-value.h"
#include "ns3/ptr.h"
#include "ns3/random-variable.h"
#include "ns3/debug.h"

#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"

#include "ns3/ascii-trace.h"
#include "ns3/pcap-trace.h"
#include "ns3/internet-node.h"
#include "ns3/csma-channel.h"
#include "ns3/csma-net-device.h"
#include "ns3/eui48-address.h"
#include "ns3/packet-socket-address.h"
#include "ns3/socket.h"
#include "ns3/onoff-application.h"
#include "ns3/queue.h"

using namespace ns3;

static Ptr<CsmaNetDevice>
CreateCsmaDevice (Ptr<Node> node, Ptr<CsmaChannel> channel)
{
  Ptr<CsmaNetDevice> device = Create<CsmaNetDevice> (node);
  device->Attach (channel);
  Ptr<Queue> queue = Queue::CreateDefault ();
  device->AddQueue (queue);
  return device;
}


int main (int argc, char *argv[])
{
  CommandLine::Parse (argc, argv);

  // Here, we will explicitly create four nodes.  In more sophisticated
  // topologies, we could configure a node factory.
  Ptr<Node> n0 = Create<Node> ();
  Ptr<Node> n1 = Create<Node> (); 
  Ptr<Node> n2 = Create<Node> (); 
  Ptr<Node> n3 = Create<Node> ();

  // create the shared medium used by all csma devices.
  Ptr<CsmaChannel> channel = Create<CsmaChannel> (DataRate(5000000), MilliSeconds(2));

  // use a helper function to connect our nodes to the shared channel.
  Ptr<NetDevice> n0If = CreateCsmaDevice (n0, channel);
  Ptr<NetDevice> n1If = CreateCsmaDevice (n1, channel);
  Ptr<NetDevice> n2If = CreateCsmaDevice (n2, channel);
  Ptr<NetDevice> n3If = CreateCsmaDevice (n3, channel);


  // create the address which identifies n1 from n0
  PacketSocketAddress n0ToN1;
  n0ToN1.SetSingleDevice (n0If->GetIfIndex ());      // set outgoing interface for outgoing packets
  n0ToN1.SetPhysicalAddress (n1If->GetAddress ()); // set destination address for outgoing packets
  n0ToN1.SetProtocol (2);            // set arbitrary protocol for outgoing packets

  // create the address which identifies n0 from n3
  PacketSocketAddress n3ToN0;
  n3ToN0.SetSingleDevice (n3If->GetIfIndex ());
  n3ToN0.SetPhysicalAddress (n0If->GetAddress ());
  n3ToN0.SetProtocol (3);
  
  // Create the OnOff application to send raw datagrams of size
  // 210 bytes at a rate of 448 Kb/s
  // from n0 to n1
  Ptr<OnOffApplication> ooff = Create<OnOffApplication> (
    n0, 
    n0ToN1,
    "Packet",
    ConstantVariable(1), 
    ConstantVariable(0));
  // Start the application
  ooff->Start(Seconds(1.0));
  ooff->Stop (Seconds(10.0));

  // Create a similar flow from n3 to n0, starting at time 1.1 seconds
  ooff = Create<OnOffApplication> (
    n3, 
    n3ToN0,
    "Packet",
    ConstantVariable(1), 
    ConstantVariable(0));
  // Start the application
  ooff->Start(Seconds(1.1));
  ooff->Stop (Seconds(10.0));
 
  // Configure tracing of all enqueue, dequeue, and NetDevice receive events
  // Trace output will be sent to the csma-packet-socket.tr file
  AsciiTrace asciitrace ("csma-packet-socket.tr");
  asciitrace.TraceAllNetDeviceRx ();
  asciitrace.TraceAllQueues ();

  Simulator::Run ();
    
  Simulator::Destroy ();
}
