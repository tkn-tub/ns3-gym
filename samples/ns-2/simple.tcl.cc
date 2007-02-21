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
// Corresponding ns-2 otcl commands are commented out above the 
// replacement ns-3 code, to highlight the change in design

#include <iostream>
#include <string>

#include "ns3/debug.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/internet-node.h"
#include "ns3/serial-channel.h"
#include "ns3/mac-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/arp-ipv4-interface.h"
#include "ns3/ipv4.h"
#include "ns3/udp-socket.h"
#include "ns3/ipv4-route.h"
#include "ns3/drop-tail.h"
#include "ns3/trace-writer.h"

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

  void Log (const char *s, const Packet &p)
  {
    m_filestr << s << &p << std::endl;
  }

protected:
  TraceWriter m_tracer;
};


static void
GenerateTraffic (UdpSocket *socket, uint32_t size)
{
  std::cout << "Node: " << socket->GetNode()->GetId () << " at=" << Simulator::Now ().GetSeconds () << "s, tx bytes=" << size << std::endl;
  socket->SendDummy (size);
  if (size > 50)
    {
      Simulator::Schedule (Seconds (0.5), &GenerateTraffic, socket, size - 50);
    }
}

static void
UdpSocketPrinter (UdpSocket *socket, uint32_t size, Ipv4Address from, uint16_t fromPort)
{
  std::cout << "Node: " << socket->GetNode()->GetId () << " at=" << Simulator::Now ().GetSeconds () << "s, rx bytes=" << size << std::endl;
}

static void
PrintTraffic (UdpSocket *socket)
{
  socket->SetDummyRxCallback (MakeCallback (&UdpSocketPrinter));
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
  const MacAddress& macaddra, 
  InternetNode* b, 
  const Ipv4Address& addrb,
  const MacAddress& macaddrb, 
  const Ipv4Mask& netmask,
  // const Rate& rate, 
  // const Time& delay,
  TraceContainer &traceContainer,
  std::string &name) 
{
    std::string qName;
    SerialChannel* channel = new SerialChannel();

    qName = name + "::Queue A";
    DropTailQueue* dtqa = new DropTailQueue(qName, traceContainer);

    SerialNetDevice* neta = new SerialNetDevice(a, macaddra);
    neta->AddQueue(dtqa);
    Ipv4Interface *interfA = new ArpIpv4Interface (a, neta);
    uint32_t indexA = a->GetIpv4 ()->AddInterface (interfA);
    channel->Attach (neta);
    neta->Attach (channel);

    interfA->SetAddress (addra);
    interfA->SetNetworkMask (netmask);
    interfA->SetUp ();

    qName = name + "::Queue B";
    DropTailQueue* dtqb = new DropTailQueue(qName, traceContainer);

    SerialNetDevice* netb = new SerialNetDevice(b, macaddrb);
    netb->AddQueue(dtqb);
    Ipv4Interface *interfB = new ArpIpv4Interface (b, netb);
    uint32_t indexB = b->GetIpv4 ()->AddInterface (interfB);
    channel->Attach (netb);
    netb->Attach (channel);

    interfB->SetAddress (addrb);
    interfB->SetNetworkMask (netmask);
    interfB->SetUp ();

    a->GetIpv4 ()->AddHostRouteTo (addrb, indexA);
    b->GetIpv4 ()->AddHostRouteTo (addra, indexB);

    NS_DEBUG_UNCOND("Adding interface " << indexA << " to node " << a->GetId())
    NS_DEBUG_UNCOND("Adding interface " << indexB << " to node " << b->GetId())

    //PrintRoutingTable (a, "a");
    //PrintRoutingTable (b, "b");

    return channel;
}

int main (int argc, char *argv[])
{
    // set ns [new Simulator]
    // ** not needed, Simulator is static object 

    // set n0 [$ns node]
    // set n1 [$ns node]
    // set n2 [$ns node]
    // set n3 [$ns node]

    // ** Here, some kind of factory or topology object will instantiates 
    // ** four identical nodes; for now, we just explicitly create them
    InternetNode *n0 = new InternetNode();
    InternetNode *n1 = new InternetNode();
    InternetNode *n2 = new InternetNode();
    InternetNode *n3 = new InternetNode();

    TraceContainer traceContainer;

    n0->SetName(std::string("Node 0"));
    n1->SetName(std::string("Node 1"));
    n2->SetName(std::string("Node 2"));
    n3->SetName(std::string("Node 3"));
 
    // set f [open out.tr w]
    // $ns trace-all $f 
    // set nf [open out.nam w]
    // $ns namtrace-all $nf
    // ** tracing configuration occurs later **
    
    // $ns duplex-link $n0 $n2 5Mb 2ms DropTail
    // $ns duplex-link $n1 $n2 5Mb 2ms DropTail
    // $ns duplex-link $n2 $n3 1.5Mb 10ms DropTail
    // ** part of topology creation object? **
    std::string channelName;

    channelName = "Channel 1";
    SerialChannel* ch1 = AddDuplexLink (
      n0, Ipv4Address("10.1.1.1"), MacAddress("00:00:00:00:00:01"), 
      n2, Ipv4Address("10.1.1.2"), MacAddress("00:00:00:00:00:02"), 
      Ipv4Mask("255.255.255.0"), 
      traceContainer, channelName);

    channelName = "Channel 2";
    SerialChannel* ch2 = AddDuplexLink (
      n1, Ipv4Address("10.1.2.1"), MacAddress("00:00:00:00:00:03"), 
      n2, Ipv4Address("10.1.2.2"), MacAddress("00:00:00:00:00:04"), 
      Ipv4Mask("255.255.255.0"), 
      traceContainer, channelName);

    channelName = "Channel 3";
    SerialChannel* ch3 = AddDuplexLink (
      n2, Ipv4Address("10.1.3.1"), MacAddress("00:00:00:00:00:05"), 
      n3, Ipv4Address("10.1.3.2"), MacAddress("00:00:00:00:00:06"), 
      Ipv4Mask("255.255.255.0"), 
      traceContainer, channelName);

    // $ns duplex-link-op $n0 $n2 orient right-up
    // $ns duplex-link-op $n1 $n2 orient right-down
    // $ns duplex-link-op $n2 $n3 orient right
    // $ns duplex-link-op $n2 $n3 queuePos 0.5
    // ** nam commands-- not supported **

    // set udp0 [new Agent/UDP]
    // $ns attach-agent $n0 $udp0
    UdpSocket *source0 = new UdpSocket (n0);

    // set cbr0 [new Application/Traffic/CBR]
    // $cbr0 attach-agent $udp0

    // set udp1 [new Agent/UDP]
    // $ns attach-agent $n3 $udp1
    UdpSocket *source3 = new UdpSocket (n3);

    // $udp1 set class_ 1
    // ** class_ is for nam **

    // set cbr1 [new Application/Traffic/CBR]
    // $cbr1 attach-agent $udp1
    // 
    // set null0 [new Agent/Null]
    // $ns attach-agent $n3 $null0
    UdpSocket *sink3 = new UdpSocket(n3);
    sink3->Bind (80);
   
    // set null1 [new Agent/Null]
    // $ns attach-agent $n1 $null1
    UdpSocket *sink1 = new UdpSocket(n1);
    sink1->Bind (80);

    // $ns connect $udp0 $null0
    source0->SetDefaultDestination (Ipv4Address ("10.1.3.2"), 80);
    // $ns connect $udp1 $null1
    source3->SetDefaultDestination (Ipv4Address ("10.1.2.1"), 80);

    // Here, finish off packet routing configuration
    n0->GetIpv4()->SetDefaultRoute (Ipv4Address ("10.1.1.2"), 1);
    n3->GetIpv4()->SetDefaultRoute (Ipv4Address ("10.1.3.1"), 1);

    //PrintRoutingTable (n0, "n0");
    //PrintRoutingTable (n1, "n1");
    //PrintRoutingTable (n2, "n2");
    //PrintRoutingTable (n3, "n3");

    // $ns at 1.0 "$cbr0 start"
    // $ns at 1.1 "$cbr1 start"

    Tracer tracer("serial-net-test.log");

    char buffer[80];

    for (int i = 1; i <= 3; ++i) {
      for (int j = 0; j < 2; ++j) {
        sprintf(buffer, "Channel %d::Queue %c::Queue::Enque", i, 'A' + j);
        
        NS_DEBUG_UNCOND("tracing event " << buffer)

        traceContainer.SetCallback (buffer,
          MakeCallback (&Tracer::Log, &tracer));

        sprintf(buffer, "Channel %d::Queue %c::Queue::Deque", i, 'A' + j);

        NS_DEBUG_UNCOND("tracing event " << buffer)

        traceContainer.SetCallback (buffer,
          MakeCallback (&Tracer::Log, &tracer));

        sprintf(buffer, "Channel %d::Queue %c::Queue::Drop", i, 'A' + j);

        NS_DEBUG_UNCOND("tracing event " << buffer)

        traceContainer.SetCallback (buffer,
          MakeCallback (&Tracer::Log, &tracer));
      }
    }

    GenerateTraffic (source0, 100);
    PrintTraffic (sink3);

    GenerateTraffic (source3, 100);
    PrintTraffic (sink1);
    // 
    // set tcp [new Agent/TCP]
    // $tcp set class_ 2
    // set sink [new Agent/TCPSink]
    // $ns attach-agent $n0 $tcp
    // $ns attach-agent $n3 $sink
    // $ns connect $tcp $sink
    // set ftp [new Application/FTP]
    // $ftp attach-agent $tcp
    // $ns at 1.2 "$ftp start"
    // 
    // $ns at 1.35 "$ns detach-agent $n0 $tcp ; $ns detach-agent $n3 $sink"
    // ** TCP may not be in this snapshot **
    // 
    // puts [$cbr0 set packetSize_]
    // puts [$cbr0 set interval_]
    // 
    // $ns at 3.0 "finish"
    Simulator::StopAt (Seconds(3.0));
    // 
    // proc finish {} {
    //         global ns f nf
    //         $ns flush-trace
    //         close $f
    //         close $nf
    //         
    //         puts "running nam..."
    //         exec nam out.nam &
    //         exit 0
    // }
    // ** this finish proc not needed? **
    // 
    // $ns run
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
