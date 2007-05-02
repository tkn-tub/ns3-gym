/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
 * All rights reserved.
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
 */

#include <fstream>

#include "ns3/debug.h"
#include "ns3/internet-node.h"
#include "ns3/packet.h"
#include "ns3/ipv4-address.h"
#include "ns3/p2p-channel.h"
#include "ns3/p2p-net-device.h"
#include "ns3/drop-tail.h"
#include "ns3/ipv4.h"
#include "ns3/udp.h"
#include "ns3/trace-context.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/node-list.h"
#include "ns3/trace-root.h"


using namespace ns3;

class Logger {
public:

  Logger (std::string const &filename) 
  {
    m_filestr.open (filename.c_str ());
    NS_DEBUG_UNCOND("**** Logger(string const &)");
  }

  ~Logger () {}

  void Log (TraceContext const &context, const Packet &p)
  {
    NodeList::NodeIndex nodeIndex;
    context.Get (nodeIndex);
    m_filestr << "node=" << NodeList::PeekNode (nodeIndex)->GetId () << " ";
    Ipv4::InterfaceIndex interfaceIndex;
    context.Get (interfaceIndex);
    m_filestr << "interface=" << interfaceIndex << " ";
    enum Queue::TraceType type;
    context.Get (type);
    switch (type) 
      {
      case Queue::ENQUEUE:
        m_filestr << "enqueue";
        break;
      case Queue::DEQUEUE:
        m_filestr << "dequeue";
        break;
      case Queue::DROP:
        m_filestr << "drop";
        break;
      }
    m_filestr << " bytes=" << p.GetSize () << std::endl;
  }

protected:
  std::ofstream m_filestr;;
};

static void
GenerateTraffic (Socket *socket, uint32_t size)
{
  std::cout << "Node: " << socket->PeekNode()->GetId () 
            << " at=" << Simulator::Now ().GetSeconds () << "s,"
            << " tx bytes=" << size << std::endl;
  socket->Send (0, size);
  if (size > 50)
    {
      Simulator::Schedule (Seconds (0.5), &GenerateTraffic, socket, size - 50);
    }
}

static void
SocketPrinter (Socket *socket, uint32_t size, const Ipv4Address &from, uint16_t fromPort)
{
  std::cout << "Node: " << socket->PeekNode()->GetId () 
            << " at=" << Simulator::Now ().GetSeconds () << "s,"
            << " rx bytes=" << size << std::endl;
}

static void
PrintTraffic (Socket *socket)
{
  socket->RecvDummy (MakeCallback (&SocketPrinter));
}


int main (int argc, char *argv[])
{
  NS_DEBUG_UNCOND("PointToPoint Net Device Test");

  // create two nodes and a simple PointToPointChannel
  InternetNode a;
  InternetNode b;
  PointToPointChannel ch = PointToPointChannel ("Test Channel", 1000, 
    Seconds (0.1));

  NodeList::Add (&a);
  NodeList::Add (&b);

  // create two NetDevices and assign one to each node
  // Note:  this would normally be done also in conjunction with
  //        creating a Channel
   
  PointToPointNetDevice neta(&a);
  a.AddDevice (&neta);

  neta.AddQueue(new DropTailQueue () );
  neta.SetName("a.eth0"); 

  PointToPointNetDevice netb(&b);
  b.AddDevice (&netb);

  neta.AddQueue(new DropTailQueue () );
  netb.SetName("b.eth0"); 

  // bind the two NetDevices together by using a simple Channel
  // this method changed to do a bidirectional binding
  neta.Attach (&ch);
  netb.Attach (&ch);

  // Some simple prints to see whether it is working
  NS_DEBUG_UNCOND("neta.GetMtu() <= " << neta.GetMtu());
  NS_DEBUG_UNCOND("netb.GetMtu() <= " << netb.GetMtu());
  NS_DEBUG_UNCOND("neta.GetAddress() <= " << neta.GetAddress());
  NS_DEBUG_UNCOND("netb.GetAddress() <= " << netb.GetAddress());

  // Note:  InternetNode constructor instantiates multiple Layer-3
  // protocols and registers them with the L3Demux object.
  // This takes care of Layer-2 -> Layer-3 bindings.
  //  XXX TODO:  will need to create a dummy IPv4 object for insertion
  //             into the Demux

  // We now need to bind the InternetNode to the various interfaces.
  // to get the Layer-3 -> Layer-2 bindings.

  // We do this by calling an "AddArpIpv4Interface(neta)" function on node a.
  // This should:  
  // i) create an Ipv4ArpInterface object (subclass of pure virtual
  //    Ipv4Interface object)
  // ii) add the Ipv4ArpInterface object to the InternetNode's internal
  //     vector of Ipv4Interfaces (keyed off of ifIndex)

  NS_DEBUG_UNCOND("Adding ARP Interface to InternetNode a");
  Ipv4 *ipa = (&a)->GetIpv4 ();
  uint32_t indexA = ipa->AddInterface (&neta);
  NS_DEBUG_UNCOND("Adding Interface " << indexA);


  // iii) give the interface an IP address

  NS_DEBUG_UNCOND("Giving IP address to ARP Interface");
  ipa->SetAddress(indexA, Ipv4Address("10.1.1.1"));
  ipa->SetNetworkMask(indexA, Ipv4Mask("255.255.255.0"));

  // iv) set the interface's state to "UP"

  NS_DEBUG_UNCOND("Setting ARP interface to UP");
  ipa->SetUp(indexA);

  ipa->SetDefaultRoute (Ipv4Address ("10.1.1.2"), 1);


  NS_DEBUG_UNCOND("Adding ARP Interface to InternetNode b");
  Ipv4 *ipb = (&b)->GetIpv4 ();
  uint32_t indexB = ipb->AddInterface (&netb);
  NS_DEBUG_UNCOND("Adding Interface " << indexB);


  // iii) give the interface an IP address

  NS_DEBUG_UNCOND("Giving IP address to ARP Interface");
  ipb->SetAddress(indexB, Ipv4Address("10.1.1.2"));
  ipb->SetNetworkMask(indexB, Ipv4Mask("255.255.255.0"));

  // iv) set the interface's state to "UP"

  NS_DEBUG_UNCOND("Setting ARP interface to UP");
  ipb->SetUp(indexB);

  ipb->SetDefaultRoute (Ipv4Address ("10.1.1.1"), 1);


  Socket *source = a.GetUdp ()->CreateSocket ();
  Socket *sink = b.GetUdp ()->CreateSocket ();
  sink->Bind (80);
  source->Connect (Ipv4Address ("10.1.1.2"), 80);

  Logger logger("p2p-net-test.log");

  TraceRoot::Connect ("/nodes/*/ipv4/interfaces/*/netdevice/queue/*", 
                      MakeCallback (&Logger::Log, &logger));

  PrintTraffic (sink);
  GenerateTraffic (source, 100);

  Simulator::Run ();

  Simulator::Destroy ();

  delete source;
  delete sink;

  return 0;
}
