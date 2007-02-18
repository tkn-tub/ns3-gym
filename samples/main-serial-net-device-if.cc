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

#if 0
#include <list>
#include <cassert>
#endif

#include "ns3/debug.h"
#include "ns3/internet-node.h"
#include "ns3/mac-address.h"
#include "ns3/packet.h"
#include "ns3/arp-ipv4-interface.h"
#include "ns3/ipv4-address.h"
#include "ns3/serial-channel.h"
#include "ns3/serial-net-device.h"
#include "ns3/trace-writer.h"
#include "ns3/trace-container.h"
#include "ns3/drop-tail.h"
#include "ns3/arp-ipv4-interface.h"
#include "ns3/ipv4.h"

#include "ns3/pcap-writer.h"

using namespace ns3;

class Logger : public TraceWriter{
public:
  Logger ()
  {
    NS_DEBUG_UNCOND("**** Logger()")
  };

  Logger (std::string const &filename) 
  {
    NS_DEBUG_UNCOND("**** Logger(string const &)")
    Open(filename);
  };

  Logger (char const *filename) : m_tracer(filename)
  {
    NS_DEBUG_UNCOND("**** Logger(char const *)")
    Open(filename);
  };

  ~Logger () {};

  void Log (const char *s, const Packet &p)
  {
    NS_DEBUG_UNCOND("**** LogEnque ("<< s << &p << ")")
    m_filestr << s << &p << std::endl;
  }

protected:
  TraceWriter m_tracer;
};

int main (int argc, char *argv[])
{
  NS_DEBUG_UNCOND("Serial Net Device Test")

  TraceContainer traceContainerA;
  TraceContainer traceContainerB;
  
  // create two nodes and a simple SerialChannel
  InternetNode a;
  InternetNode b;
  SerialChannel ch;

  // create two NetDevices and assign one to each node
  // Note:  this would normally be done also in conjunction with
  //        creating a Channel
  //        Here, we do not care about the Device Address (point-to-point)
  //        but more generally, we would use a subclass such as MacAddress
  //        as follows:    MacAddress addra("00:00:00:00:00:01");
  //        so we'll pretend and give them simple MacAddresses here
   
  MacAddress addra("00:00:00:00:00:01");
  SerialNetDevice neta(&a, addra);

  DropTailQueue dtqa (traceContainerA);

  neta.AddQueue(&dtqa);
  neta.SetName("a.eth0"); 

  MacAddress addrb("00:00:00:00:00:02");
  SerialNetDevice netb(&b, addrb);

  DropTailQueue dtqb (traceContainerB);

  netb.AddQueue(&dtqb);
  netb.SetName("b.eth0"); 

  // bind the two NetDevices together by using a simple Channel
  // this method changed to do a bidirectional binding
  ch.Attach(&neta);
  ch.Attach(&netb);

  // Some simple prints to see whether it is working
  NS_DEBUG_UNCOND("neta.GetMtu() <= " << neta.GetMtu())
  NS_DEBUG_UNCOND("netb.GetMtu() <= " << netb.GetMtu())
  NS_DEBUG_UNCOND("neta.GetAddress() <= " << neta.GetAddress())
  NS_DEBUG_UNCOND("netb.GetAddress() <= " << netb.GetAddress())

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

  NS_DEBUG_UNCOND("Adding ARP Interface to InternetNode a")
  ArpIpv4Interface* arpipv4interfacep = new ArpIpv4Interface(&a, &neta);
  uint32_t indexA = (&a)->GetIpv4 ()->AddInterface (arpipv4interfacep);
  NS_DEBUG_UNCOND("Adding Interface " << indexA);


  // iii) give the interface an IP address

  NS_DEBUG_UNCOND("Giving IP address to ARP Interface")
  arpipv4interfacep->SetAddress(Ipv4Address("10.1.1.1"));
  arpipv4interfacep->SetNetworkMask(Ipv4Mask("255.255.255.0"));

  // iv) set the interface's state to "UP"

  NS_DEBUG_UNCOND("Setting ARP interface to UP")
  arpipv4interfacep->SetUp();

  NS_DEBUG_UNCOND("Adding ARP Interface to InternetNode b")
  ArpIpv4Interface* arpipv4interfacepb = new ArpIpv4Interface(&b, &netb);
  uint32_t indexB = (&b)->GetIpv4 ()->AddInterface (arpipv4interfacepb);
  NS_DEBUG_UNCOND("Adding Interface " << indexB);


  // iii) give the interface an IP address

  NS_DEBUG_UNCOND("Giving IP address to ARP Interface")
  arpipv4interfacepb->SetAddress(Ipv4Address("10.1.1.2"));
  arpipv4interfacepb->SetNetworkMask(Ipv4Mask("255.255.255.0"));

  // iv) set the interface's state to "UP"

  NS_DEBUG_UNCOND("Setting ARP interface to UP")
  arpipv4interfacepb->SetUp();

  Logger logger("serial-net-test.log");

  traceContainerA.SetCallback ("Queue::Enque", 
    MakeCallback (&Logger::Log, &logger));

  // create a packet on one node and send it through, reading it
  // on the other node
  Packet p;

  NS_DEBUG_UNCOND("Sending Packet " << &p)
  arpipv4interfacep->Send(p, Ipv4Address("10.1.1.2"));

  //neta.Send(p, MacAddress());  // Test that all-zero's MacAddress used
  //netb.Send(p, "00:01:02:03:04:05");  // Dummy function call

  return 0;
}
