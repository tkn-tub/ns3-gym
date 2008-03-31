/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006,2007 INRIA
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
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "ns3/packet.h"
#include "ns3/header.h"
#include "ns3/packet-printer.h"
#include "ns3/ipv4-header.h"
#include "ns3/udp-header.h"

using namespace ns3;

// This sample file shows how to use the Packet metadata facility
//
// Packets are stored as ``packed'' data structures, to facilitate
// fragmentation and network emulation.  However, when debugging a program,
// or for certain tracing applications, it may be convenient to dump out
// the contents of a packet header in a human-friendly form.
//
// To do this, a few things are needed:
// i) enable the metadata facility (disabled by default, because it causes
//    a small performance hit 
// ii) decide on whether you want to use a default or customized (you
//     provide your own) routine to dump a particular header
//
// This sample steps through two routines; one to use the default
// printing of IPv4 and UDP headers, and one to show a non-default case.
// There is a lot of emphasis in this sample of how this facility
// interacts with packet fragmentation.

void DefaultPrint (void)
{
  // We create a packet with 1000 bytes of zero payload
  // and add 3 headers to this packet.
  Ptr<Packet> p = Create<Packet> (1000);
  Ipv4Header ipv4;
  UdpHeader udp;
  ipv4.SetSource (Ipv4Address ("192.168.0.1"));
  ipv4.SetDestination (Ipv4Address ("192.168.0.2"));
  udp.SetSource (1025);
  udp.SetDestination (80);
  udp.SetPayloadSize (1000);
  p->AddHeader (udp);
  p->AddHeader (ipv4);

  std::cout << "full packet size=" << p->GetSize () << std::endl;
  // Here, invoke the default Print routine, directed to std out
  p->Print (std::cout);
  std::cout << std::endl;


  // Now, we fragment our packet in 3 consecutive pieces.
  Ptr<Packet> p1 = p->CreateFragment (0, 2);
  Ptr<Packet> p2 = p->CreateFragment (2, 1000);
  Ptr<Packet> p3 = p->CreateFragment (1002, 26);

  std::cout << "fragment1" << std::endl;
  p1->Print (std::cout);
  std::cout << std::endl;
  std::cout << "fragment2" << std::endl;
  p2->Print (std::cout);
  std::cout << std::endl;
  std::cout << "fragment3" << std::endl;
  p3->Print (std::cout);
  std::cout << std::endl;

  // And, finally, we re-aggregate the 3 consecutive pieces.
  Ptr<Packet> aggregate = p1->Copy ();
  aggregate->AddAtEnd (p2);
  aggregate->AddAtEnd (p3);
  std::cout << "aggregated" << std::endl;
  aggregate->Print (std::cout);
  std::cout << std::endl;
}

void
DoPrintPayload (std::ostream & os,uint32_t packetUid,uint32_t size,
                struct PacketPrinter::FragmentInformation info)
{
  os << "PAYLOAD (size " << size << " trim_start " << info.start << " trim_end " << info.end << ")";
}
void 
DoPrintIpv4Header (std::ostream &os, uint32_t packetUid, uint32_t size, const Ipv4Header *ipv4)
{
  os << "IPV4 " << ipv4->GetSource () << " > " << ipv4->GetDestination ();
}
void 
DoPrintIpv4HeaderFragment (std::ostream &os, uint32_t packetUid, uint32_t size,
                          std::string &name, struct PacketPrinter::FragmentInformation info)
{
  os << "IPV4 fragment";
}

// This function walks through a non-default case.  A few features of
// the API (defined in common/packet-printer.h) are shown.
//
void NonDefaultPrint (void)
{
  // create an adhoc packet printer.
  PacketPrinter printer;
  // print from first header to last trailer
  printer.PrintForward ();
  // set a string separator automatically inserted
  // between each call to a printing function.
  printer.SetSeparator (" - ");
  // set the payload print function
  printer.SetPayloadPrinter (MakeCallback (&DoPrintPayload));
  // set the print function for the header type Ipv4Header.
  printer.SetHeaderPrinter (MakeCallback (&DoPrintIpv4Header),
                            MakeCallback (&DoPrintIpv4HeaderFragment));


  // We create a packet with 1000 bytes of zero payload
  Ptr<Packet> p = Create<Packet> (1000);
  Ipv4Header ipv4;
  UdpHeader udp;
  ipv4.SetSource (Ipv4Address ("192.168.0.1"));
  ipv4.SetDestination (Ipv4Address ("192.168.0.2"));
  udp.SetSource (1025);
  udp.SetDestination (80);
  udp.SetPayloadSize (1000);
  p->AddHeader (udp);
  p->AddHeader (ipv4);

  std::cout << "full packet size=" << p->GetSize () << std::endl;
  p->Print (std::cout, printer);
  std::cout << std::endl;


  // fragment our packet in 3 pieces
  Ptr<Packet> p1 = p->CreateFragment (0, 2);
  Ptr<Packet> p2 = p->CreateFragment (2, 1000);
  Ptr<Packet> p3 = p->CreateFragment (1002, 26);
  std::cout << "fragment1" << std::endl;
  p1->Print (std::cout, printer);
  std::cout << std::endl;
  std::cout << "fragment2" << std::endl;
  p2->Print (std::cout, printer);
  std::cout << std::endl;
  std::cout << "fragment3" << std::endl;
  p3->Print (std::cout, printer);
  std::cout << std::endl;

  // aggregate all 3 fragments of the original packet
  // to reconstruct a copy of the original packet.
  Ptr<Packet> aggregate = p1->Copy ();
  aggregate->AddAtEnd (p2);
  aggregate->AddAtEnd (p3);
  std::cout << "aggregated" << std::endl;
  aggregate->Print (std::cout, printer);
  std::cout << std::endl;
}



int main (int argc, char *argv[])
{
  Packet::EnableMetadata ();

  std::cout << "DefaultPrint()" << std::endl;
  DefaultPrint ();

  std::cout << std::endl << "NonDefaultPrint()" << std::endl;
  NonDefaultPrint ();

  return 0;
}
