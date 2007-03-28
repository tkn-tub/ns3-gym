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

#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"
#include "ns3/trace-writer.h"

#include "ns3/internet-node.h"
#include "ns3/p2p-channel.h"
#include "ns3/p2p-net-device.h"
#include "ns3/mac-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/arp-ipv4-interface.h"
#include "ns3/ipv4.h"
#include "ns3/datagram-socket.h"
#include "ns3/ipv4-route.h"
#include "ns3/drop-tail.h"
#include "ns3/trace-writer.h"
#include "ns3/llc-snap-header.h"
#include "ns3/arp-header.h"
#include "ns3/ipv4-header.h"
#include "ns3/udp-header.h"
#include "ns3/node-list.h"
#include "ns3/trace-root.h"
#include "ns3/p2p-topology.h"
#include "ns3/onoff-application.h"
#include "ns3/application-list.h"
#include "ns3/random-variable.h"

using namespace ns3;

class AsciiTrace 
{
public:
  AsciiTrace (std::string filename);
  ~AsciiTrace ();
  void TraceAllQueues (void);
  void TraceAllNetDeviceRx (void);
private:
  void PrintType (Packet const &p);
  void LogDevQueue (TraceContext const &context, const Packet &p);
  void LogDevRx (TraceContext const &context, Packet &p);
  std::ofstream m_os;
};

AsciiTrace::AsciiTrace (std::string filename)
{
  m_os.open (filename.c_str ());
}
AsciiTrace::~AsciiTrace ()
{
  m_os.close ();
}
void
AsciiTrace::TraceAllQueues (void)
{
  TraceRoot::Connect ("/nodes/*/ipv4/interfaces/*/netdevice/queue/*",
                      MakeCallback (&AsciiTrace::LogDevQueue, this));
}
void
AsciiTrace::TraceAllNetDeviceRx (void)
{
  TraceRoot::Connect ("/nodes/*/ipv4/interfaces/*/netdevice/rx",
                      MakeCallback (&AsciiTrace::LogDevRx, this));
}

void
AsciiTrace::PrintType (Packet const &packet)
{
  Packet p = packet;
  LlcSnapHeader llc;
  p.Peek (llc);
  p.Remove (llc);
  switch (llc.GetType ())
    {
    case 0x0800: {
      Ipv4Header ipv4;
      p.Peek (ipv4);
      p.Remove (ipv4);
      if (ipv4.GetProtocol () == 17)
        {
          UdpHeader udp;
          p.Peek (udp);
          p.Remove (udp);
          m_os << "udp size=" << p.GetSize ();
        }
    } break;
    case 0x0806: {
      ArpHeader arp;
      p.Peek (arp);
      p.Remove (arp);
      m_os << "arp ";
      if (arp.IsRequest ())
        {
          m_os << "request";
        }
      else
        {
          m_os << "reply ";
        }
    } break;
    }
} 

void 
AsciiTrace::LogDevQueue (TraceContext const &context, Packet const &packet)
{
  enum Queue::TraceType type;
  context.Get (type);
  switch (type) 
    {
    case Queue::ENQUEUE:
      m_os << "+ ";
      break;
    case Queue::DEQUEUE:
      m_os << "- ";
      break;
    case Queue::DROP:
      m_os << "d ";
      break;
    }
  m_os << Simulator::Now ().GetSeconds () << " ";
  NodeList::NodeIndex nodeIndex;
  context.Get (nodeIndex);
  m_os << "node=" << NodeList::GetNode (nodeIndex)->GetId () << " ";
  Ipv4::InterfaceIndex interfaceIndex;
  context.Get (interfaceIndex);
  m_os << "interface=" << interfaceIndex << " ";
  m_os << "pkt-uid=" << packet.GetUid () << " ";
  PrintType (packet);
  m_os << std::endl;
}
void 
AsciiTrace::LogDevRx (TraceContext const &context, Packet &p)
{
  m_os << "r " << Simulator::Now ().GetSeconds () << " ";
  NodeList::NodeIndex nodeIndex;
  context.Get (nodeIndex);
  m_os << "node=" << NodeList::GetNode (nodeIndex)->GetId () << " ";
  Ipv4::InterfaceIndex interfaceIndex;
  context.Get (interfaceIndex);
  m_os << "interface=" << interfaceIndex << " ";
  m_os << "pkt-uid=" << p.GetUid () << " ";
  PrintType (p);
  m_os << std::endl;  
}

int main (int argc, char *argv[])
{
#if 0
  DebugComponentEnable("Object");
  DebugComponentEnable("Queue");
  DebugComponentEnable("DropTailQueue");
  DebugComponentEnable("Channel");
  DebugComponentEnable("PointToPointChannel");
  DebugComponentEnable("PointToPointNetDevice");
#endif

  // Optionally, specify some default values for Queue objects.
  // For this example, we specify that we want each queue to
  // be a DropTail queue, with a limit of 30 packets.
  // Specify DropTail for default queue type (note. this is actually
  // the default, but included here as an example).
  Queue::Default(DropTailQueue());
  // Specify limit of 30 in units of packets (not implemented).
  //  Queue::Default().SetLimitPackets(30);

  // The node factory is designed to allow user specification
  // of the "type" of node desired for each node creation.  This
  // is done by creating a node object (the inNode below), configuring
  // the object with the desired capabilities, and pushing the node
  // object on the prototype stack.  In this simple example, the
  // default behavior of an InternetNode is adequate, so we don't
  // do any configuration in this simple example.
  InternetNode inNode;
  Node::PushNodePrototype(inNode);
  // Next create the physical node topology using the node factory
  Node* n0 = Node::Create(); 
  Node* n1 = Node::Create(); 
  Node* n2 = Node::Create(); 
  Node* n3 = Node::Create(); 

  PointToPointTopology::AddPointToPointLink (
      n0, Ipv4Address("10.1.1.1"), 
      n2, Ipv4Address("10.1.1.2"), 
      DataRate(5000000), MilliSeconds(2));
  
  PointToPointTopology::AddPointToPointLink (
      n1, Ipv4Address("10.1.2.1"), 
      n2, Ipv4Address("10.1.2.2"), 
      DataRate(5000000), MilliSeconds(2));

  PointToPointTopology::AddPointToPointLink (
      n2, Ipv4Address("10.1.3.1"), 
      n3, Ipv4Address("10.1.3.2"), 
      DataRate(1500000), MilliSeconds(10));
  
  // Create the OnOff application to send UDP datagrams of size
  // 210 bytes at a rate of 448 Kb/s
  OnOffApplication* ooff0 = new OnOffApplication(
    *n0, 
    Ipv4Address("10.1.3.2"), 
    80, 
    ConstantVariable(1), 
    ConstantVariable(0), 
    DataRate(448000), 
    210);
  // Add to Node's ApplicationList (takes ownership of pointer)
  n0->GetApplicationList()->Add(ooff0);
  // Start the application
  ooff0->Start(Seconds(1.0));

  // Create a similar flow from n3 to n1, starting at time 1.1 seconds
  OnOffApplication* ooff1 = new OnOffApplication(
    *n3, 
    Ipv4Address("10.1.2.1"), 
    80, 
    ConstantVariable(1), 
    ConstantVariable(0), 
    DataRate(448000), 
    210);
  // Add to Node's ApplicationList (takes ownership of pointer)
  n3->GetApplicationList()->Add(ooff1);
  // Start the application
  ooff1->Start(Seconds(1.1));

  // Here, finish off packet routing configuration
  // This will likely set by some global StaticRouting object in the future
  n0->GetIpv4()->SetDefaultRoute (Ipv4Address ("10.1.1.2"), 1);
  n3->GetIpv4()->SetDefaultRoute (Ipv4Address ("10.1.3.1"), 1);

  // Configure tracing of all enqueue, dequeue, and NetDevice receive events
  // Trace output will be sent to the simple-p2p.tr file
  AsciiTrace trace ("simple-p2p.tr");
  trace.TraceAllQueues ();
  trace.TraceAllNetDeviceRx ();

  Simulator::StopAt (Seconds(10.0));

  Simulator::Run ();
    
  Simulator::Destroy ();
}
