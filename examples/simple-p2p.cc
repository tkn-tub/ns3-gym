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
 *
 * ns-2 simple.tcl script (ported from ns-2)
 * Originally authored by Steve McCanne, 12/19/1996
 */

// Port of ns-2/tcl/ex/simple.tcl to ns-3
//
// Network topology
//
//  n0
//     \ 5 Mb/s, 2ms
//      \          1.5Mb/s, 10ms
//       n2 -------------------------n3
//      /
//     / 5 Mb/s, 2ms
//   n1
//
// - all links are p2p links with indicated one-way BW/delay
// - CBR/UDP flows from n0 to n3, and from n3 to n1
// - FTP/TCP flow from n0 to n3, starting at time 1.2 to time 1.35 sec.
// - UDP packet size of 210 bytes, with per-packet interval 0.00375 sec.
//   (i.e., DataRate of 448,000 bps)
// - DropTail queues 
// - Tracing of queues and packet receptions to file "simple-p2p.tr"

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/debug.h"
#include "ns3/command-line.h"
#include "ns3/default-value.h"

#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"

#include "ns3/ascii-trace.h"
#include "ns3/pcap-trace.h"
#include "ns3/internet-node.h"
#include "ns3/p2p-channel.h"
#include "ns3/p2p-net-device.h"
#include "ns3/mac-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/i-ipv4.h"
#include "ns3/socket.h"
#include "ns3/ipv4-route.h"
#include "ns3/drop-tail.h"
#include "ns3/node-list.h"
#include "ns3/trace-root.h"
#include "ns3/p2p-topology.h"
#include "ns3/onoff-application.h"
#include "ns3/application-list.h"
#include "ns3/random-variable.h"

using namespace ns3;

int main (int argc, char *argv[])
{

  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
#if 0 
  DebugComponentEnable("Object");
  DebugComponentEnable("Queue");
  DebugComponentEnable("DropTailQueue");
  DebugComponentEnable("Channel");
  DebugComponentEnable("PointToPointChannel");
  DebugComponentEnable("PointToPointNetDevice");
#endif

  // Set up some default values for the simulation.  Use the Bind()
  // technique to tell the system what subclass of Queue to use,
  // and what the queue limit is

  // The below Bind command tells the queue factory which class to
  // instantiate, when the queue factory is invoked in the topology code
  Bind ("Queue", "DropTailQueue");   

  //Bind ("DropTailQueue::m_maxPackets", 30);   

  // Allow the user to override any of the defaults and the above
  // Bind()s at run-time, via command-line arguments
  CommandLine::Parse (argc, argv);

  // Here, we will explicitly create four nodes.  In more sophisticated
  // topologies, we could configure a node factory.
  Node* n0 = new InternetNode ();
  Node* n1 = new InternetNode (); 
  Node* n2 = new InternetNode (); 
  Node* n3 = new InternetNode ();

  // We create the channels first without any IP addressing information
  PointToPointChannel *channel0 = 
    PointToPointTopology::AddPointToPointLink (
      n0, n2, DataRate(5000000), MilliSeconds(2));

  PointToPointChannel *channel1 = 
    PointToPointTopology::AddPointToPointLink (
      n1, n2, DataRate(5000000), MilliSeconds(2));
  
  PointToPointChannel *channel2 = 
    PointToPointTopology::AddPointToPointLink (
      n2, n3, DataRate(1500000), MilliSeconds(10));
  
  // Later, we add IP addresses.  
  PointToPointTopology::AddIpv4Addresses (
      channel0, n0, Ipv4Address("10.1.1.1"),
      n2, Ipv4Address("10.1.1.2"));
  
  PointToPointTopology::AddIpv4Addresses (
      channel1, n1, Ipv4Address("10.1.2.1"),
      n2, Ipv4Address("10.1.2.2"));
  
  PointToPointTopology::AddIpv4Addresses (
      channel2, n2, Ipv4Address("10.1.3.1"),
      n3, Ipv4Address("10.1.3.2"));

  // Finally, we add static routes.  These three steps (Channel and
  // NetDevice creation, IP Address assignment, and routing) are 
  // separated because there may be a need to postpone IP Address
  // assignment (emulation) or modify to use dynamic routing
  PointToPointTopology::AddIpv4Routes(n0, n2, channel0);
  PointToPointTopology::AddIpv4Routes(n1, n2, channel1);
  PointToPointTopology::AddIpv4Routes(n2, n3, channel2);

  channel0->Unref ();
  channel1->Unref ();
  channel2->Unref ();

  // Create the OnOff application to send UDP datagrams of size
  // 210 bytes at a rate of 448 Kb/s
  OnOffApplication* ooff0 = new OnOffApplication(
    n0, 
    Ipv4Address("10.1.3.2"), 
    80, 
    ConstantVariable(1), 
    ConstantVariable(0), 
    DataRate(448000), 
    210);
  // Add to Node's ApplicationList (takes ownership of pointer)
  ApplicationList *apl0 = n0->QueryInterface<ApplicationList> 
    (ApplicationList::iid);
  apl0->Add(ooff0);
  apl0->Unref ();

  // Start the application
  ooff0->Start(Seconds(1.0));
  ooff0->Stop (Seconds(10.0));
  ooff0->Unref ();

  // Create a similar flow from n3 to n1, starting at time 1.1 seconds
  OnOffApplication* ooff1 = new OnOffApplication(
    n3, 
    Ipv4Address("10.1.2.1"), 
    80, 
    ConstantVariable(1), 
    ConstantVariable(0), 
    DataRate(448000), 
    210);
  // Add to Node's ApplicationList (takes ownership of pointer)
  ApplicationList *apl3 = n3->QueryInterface<ApplicationList> (ApplicationList::iid);
  apl3->Add(ooff1);
  apl3->Unref ();
  // Start the application
  ooff1->Start(Seconds(1.1));
  ooff1->Stop (Seconds(10.0));
  ooff1->Unref ();

  // Here, finish off packet routing configuration
  // This will likely set by some global StaticRouting object in the future
  IIpv4 *ipv4;
  ipv4 = n0->QueryInterface<IIpv4> (IIpv4::iid);
  ipv4->SetDefaultRoute (Ipv4Address ("10.1.1.2"), 1);
  ipv4->Unref ();
  ipv4 = n3->QueryInterface<IIpv4> (IIpv4::iid);
  ipv4->SetDefaultRoute (Ipv4Address ("10.1.3.1"), 1);
  ipv4->Unref ();

  n0->Unref ();
  n1->Unref ();
  n2->Unref ();
  n3->Unref ();

  // Configure tracing of all enqueue, dequeue, and NetDevice receive events
  // Trace output will be sent to the simple-p2p.tr file
  AsciiTrace asciitrace ("simple-p2p.tr");
  asciitrace.TraceAllQueues ();
  asciitrace.TraceAllNetDeviceRx ();

  // Also configure some tcpdump traces; each interface will be traced
  // The output files will be named simple-p2p.pcap-<nodeId>-<interfaceId>
  // and can be read by the "tcpdump -r" command (use "-tt" option to
  // display timestamps correctly)
  PcapTrace pcaptrace ("simple-p2p.pcap");
  pcaptrace.TraceAllIp ();

  Simulator::Run ();
    
  Simulator::Destroy ();
}
