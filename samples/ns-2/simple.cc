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
// - all links are serial links with indicated one-way BW/delay
// - CBR/UDP flows from n0 to n3, and from n3 to n1
// - FTP/TCP flow from n0 to n3, starting at time 1.2 to time 1.35 sec.
// - UDP packet size of 210 bytes, with per-packet interval 0.00375 sec.
// - DropTail queues 
// - Tracing of queues and packet receptions to file "out.tr"

#include <iostream>
#include <string>
#include <cassert>

#include "ns3/debug.h"

#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/trace-writer.h"

#include "ns3/internet-node.h"
#include "ns3/serial-channel.h"
#include "ns3/serial-net-device.h"
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

using namespace ns3;

class Tracer : public TraceWriter{
public:
  Tracer ()
  {
  };

  Tracer (std::string const &filename) 
  {
    Open(filename);
  };

  Tracer (char const *filename) : m_tracer(filename)
  {
    Open(filename);
  };

  ~Tracer () {};

  void LogNodeInterface (TraceContext const &context)
  {
    NodeList::NodeIndex nodeIndex;
    context.Get (nodeIndex);
    m_filestr << "node=" << NodeList::GetNode (nodeIndex)->GetId () << " ";
    Ipv4::InterfaceIndex interfaceIndex;
    context.Get (interfaceIndex);
    m_filestr << "interface=" << interfaceIndex << " ";
  }


  void LogEnqueue (TraceContext const &context, const Packet &p)
  {
    m_filestr << "+ " << Simulator::Now().GetSeconds() << " ";
    LogNodeInterface (context);
    m_filestr << "pkt-uid=" << p.GetUid () << " ";
    //PrintLlcPacket (p, m_filestr);
    m_filestr << std::endl;
  }

  void LogDequeue (TraceContext const &context, const Packet &p)
  {
    m_filestr << "- " << Simulator::Now().GetSeconds() << " ";
    LogNodeInterface (context);
    m_filestr << "pkt-uid=" << p.GetUid () << " ";
    //PrintLlcPacket (p, m_filestr);
    m_filestr << std::endl;
  }
  void LogDrop (TraceContext const &context, const Packet &p)
  {
    m_filestr << "d " << Simulator::Now().GetSeconds() << " ";
    LogNodeInterface (context);
    m_filestr << "pkt-uid=" << p.GetUid () << " ";
    //PrintLlcPacket (p, m_filestr);
    m_filestr << std::endl;
  }

  void PrintLlcPacket (Packet p, std::ostream &os)
  {
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
            os << "udp payload=" << p.GetSize () 
               << " from="<< ipv4.GetSource () << ":" << udp.GetSource ()
               << " to="<< ipv4.GetDestination () << ":" << udp.GetDestination ();
          }
      } break;
      case 0x0806: {
        ArpHeader arp;
        p.Peek (arp);
        p.Remove (arp);
        os << "arp ";
        if (arp.IsRequest ())
          {
            os << "request from=" << arp.GetSourceIpv4Address ()
               << ", for=" << arp.GetDestinationIpv4Address ();
          }
        else
          {
            os << "reply from=" << arp.GetSourceIpv4Address ()
               << ", for=" << arp.GetDestinationIpv4Address ();
          }
      } break;
      }
  }

protected:
  TraceWriter m_tracer;
};


static void
GenerateTraffic (DatagramSocket *socket, uint32_t size)
{
  std::cout << "Node: " << socket->GetNode()->GetId () 
            << " at=" << Simulator::Now ().GetSeconds () << "s,"
            << " tx bytes=" << size << std::endl;
  socket->SendDummy (size);
  if (size > 50)
    {
      Simulator::Schedule (Seconds (0.5), &GenerateTraffic, socket, size - 50);
    }
}

static void
DatagramSocketPrinter (DatagramSocket *socket, uint32_t size, Ipv4Address from, uint16_t fromPort)
{
  std::cout << "Node: " << socket->GetNode()->GetId () 
            << " at=" << Simulator::Now ().GetSeconds () << "s,"
            << " rx bytes=" << size << std::endl;
}

static void
PrintTraffic (DatagramSocket *socket)
{
  socket->SetDummyRxCallback (MakeCallback (&DatagramSocketPrinter));
}

#if 0
static void
PrintRoutingTable (InternetNode *a, std::string name)
{
  Ipv4 *ipv4 = a->GetIpv4 ();
  std::cout << "interfaces node="<<name<<std::endl;
  for (uint32_t i = 0; i < ipv4->GetNInterfaces (); i++)
    {
      Ipv4Interface *interface = ipv4->GetInterface (i);
      std::cout << "interface addr="<<interface->GetAddress () 
                << ", netmask="<<interface->GetNetworkMask ()
                << std::endl;
    }

  std::cout << "routing table:" << std::endl;
  for (uint32_t i = 0; i < ipv4->GetNRoutes (); i++)
    {
      Ipv4Route *route = ipv4->GetRoute (i);
      std::cout << (*route) << std::endl;
    }
  std::cout << "node end" << std::endl;
}
#endif

static SerialChannel * 
AddDuplexLink(
  InternetNode* a, 
  const Ipv4Address& addra,
  InternetNode* b, 
  const Ipv4Address& addrb,
  uint64_t bps,
  const Time& delay)
{
  SerialChannel* channel = new SerialChannel(bps, delay);

  // Duplex link is assumed to be subnetted as a /30
  // May run this unnumbered in the future?
  Ipv4Mask netmask("255.255.255.252");
  assert(netmask.IsMatch(addra,addrb));

  DropTailQueue* dtqa = new DropTailQueue();

  SerialNetDevice* neta = new SerialNetDevice(a);
  neta->AddQueue(dtqa);
  Ipv4Interface *interfA = new ArpIpv4Interface (a, neta);
  uint32_t indexA = a->GetIpv4 ()->AddInterface (interfA);
  neta->Attach (channel);
  
  interfA->SetAddress (addra);
  interfA->SetNetworkMask (netmask);
  interfA->SetUp ();

  DropTailQueue* dtqb = new DropTailQueue();

  SerialNetDevice* netb = new SerialNetDevice(b);
  netb->AddQueue(dtqb);
  Ipv4Interface *interfB = new ArpIpv4Interface (b, netb);
  uint32_t indexB = b->GetIpv4 ()->AddInterface (interfB);
  netb->Attach (channel);

  interfB->SetAddress (addrb);
  interfB->SetNetworkMask (netmask);
  interfB->SetUp ();

  a->GetIpv4 ()->AddHostRouteTo (addrb, indexA);
  b->GetIpv4 ()->AddHostRouteTo (addra, indexB);

  NS_DEBUG_UNCOND("Adding interface " << indexA << " to node " << a->GetId());
  NS_DEBUG_UNCOND("Adding interface " << indexB << " to node " << b->GetId());

  //PrintRoutingTable (a, "a");
  //PrintRoutingTable (b, "b");

  return channel;
}


int main (int argc, char *argv[])
{
#if 0
  DebugComponentEnable("Queue");
  DebugComponentEnable("DropTailQueue");
  DebugComponentEnable("Channel");
  DebugComponentEnable("SerialChannel");
  DebugComponentEnable("SerialNetDevice");
  DebugComponentEnable("SerialPhy");
#endif

  // ** Here, some kind of factory or topology object will instantiates 
  // ** four identical nodes; for now, we just explicitly create them
  InternetNode *n0 = new InternetNode();
  InternetNode *n1 = new InternetNode();
  InternetNode *n2 = new InternetNode();
  InternetNode *n3 = new InternetNode();

  NodeList::Add (n0);
  NodeList::Add (n1);
  NodeList::Add (n2);
  NodeList::Add (n3);

  n0->SetName(std::string("Node 0"));
  n1->SetName(std::string("Node 1"));
  n2->SetName(std::string("Node 2"));
  n3->SetName(std::string("Node 3"));
  
  SerialChannel* ch1 = AddDuplexLink (n0, Ipv4Address("10.1.1.1"), 
      n2, Ipv4Address("10.1.1.2"), 5000000, MilliSeconds(2));
  
  SerialChannel* ch2 = AddDuplexLink (n1, Ipv4Address("10.1.2.1"), 
      n2, Ipv4Address("10.1.2.2"), 5000000, MilliSeconds(2));

  SerialChannel* ch3 = AddDuplexLink (n2, Ipv4Address("10.1.3.1"), 
      n3, Ipv4Address("10.1.3.2"), 1500000, MilliSeconds(10));
  
  DatagramSocket *source0 = new DatagramSocket (n0);
  DatagramSocket *source3 = new DatagramSocket (n3);
  DatagramSocket *sink3 = new DatagramSocket(n3);
  sink3->Bind (80);
  DatagramSocket *sink1 = new DatagramSocket(n1);
  sink1->Bind (80);

  source3->SetDefaultDestination (Ipv4Address ("10.1.2.1"), 80);
  source0->SetDefaultDestination (Ipv4Address ("10.1.3.2"), 80);

  // Here, finish off packet routing configuration
  n0->GetIpv4()->SetDefaultRoute (Ipv4Address ("10.1.1.2"), 1);
  n3->GetIpv4()->SetDefaultRoute (Ipv4Address ("10.1.3.1"), 1);

  Tracer tracer("out.tr");
  TraceRoot::Connect ("/nodes/*/ipv4/interfaces/*/netdevice/queue/enqueue",
                      MakeCallback (&Tracer::LogEnqueue, &tracer));
  TraceRoot::Connect ("/nodes/*/ipv4/interfaces/*/netdevice/queue/dequeue",
                      MakeCallback (&Tracer::LogDequeue, &tracer));
  TraceRoot::Connect ("/nodes/*/ipv4/interfaces/*/netdevice/queue/drop",
                      MakeCallback (&Tracer::LogDrop, &tracer));
#if 0
  TraceRoot::Connect ("/nodes/*/ipv4/interfaces/*/netdevice/queue/receive",
                      MakeCallback (&Tracer::LogReceive, &tracer));
#endif


  PrintTraffic (sink3);
  GenerateTraffic (source0, 100);

  PrintTraffic (sink1);
  GenerateTraffic (source3, 100);

  Simulator::StopAt (Seconds(3.0));

  Simulator::Run ();
    
  // The below deletes will be managed by future topology objects
  // or containers or smart pointers
  delete n0;
  delete n1;
  delete n2;
  delete n3;
  delete ch1;
  delete ch2;
  delete ch3;
  delete source3;
  delete source0;
  delete sink3;
  delete sink1;

  Simulator::Destroy ();
}
