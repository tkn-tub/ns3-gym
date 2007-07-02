/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/packet.h"
#include "ns3/header.h"
#include "ns3/packet-printer.h"
#include "ns3/ipv4-header.h"
#include "ns3/udp-header.h"

using namespace ns3;


void DefaultPrint (void)
{
  // We create a packet with 1000 bytes of zero payload
  // and add 3 headers to this packet.
  Packet p (1000);
  Ipv4Header ipv4;
  UdpHeader udp;
  ipv4.SetSource (Ipv4Address ("192.168.0.1"));
  ipv4.SetDestination (Ipv4Address ("192.168.0.2"));
  udp.SetSource (1025);
  udp.SetDestination (80);
  p.AddHeader (udp);
  p.AddHeader (ipv4);

  std::cout << "full packet size=" << p.GetSize () << std::endl;
  p.Print (std::cout);
  std::cout << std::endl;


  // Now, we fragment our packet in 3 consecutive pieces.
  Packet p1 = p.CreateFragment (0, 2);
  Packet p2 = p.CreateFragment (2, 1000);
  Packet p3 = p.CreateFragment (1002, 26);

  std::cout << "fragment1" << std::endl;
  p1.Print (std::cout);
  std::cout << std::endl;
  std::cout << "fragment2" << std::endl;
  p2.Print (std::cout);
  std::cout << std::endl;
  std::cout << "fragment3" << std::endl;
  p3.Print (std::cout);
  std::cout << std::endl;

  // And, finally, we re-aggregate the 3 consecutive pieces.
  Packet aggregate = p1;
  aggregate.AddAtEnd (p2);
  aggregate.AddAtEnd (p3);
  std::cout << "aggregated" << std::endl;
  aggregate.Print (std::cout);
  std::cout << std::endl;
}

void 
DoPrintDefault (std::ostream &os,uint32_t packetUid, uint32_t size, 
                std::string &name, struct PacketPrinter::FragmentInformation info)
{
  os << name <<" (size " << size << " trim_start " << info.start << " trim_end " << info.end << ")";
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

void NonDefaultPrint (void)
{
  // create an adhoc packet printer.
  PacketPrinter printer;
  // print from first header to last trailer
  printer.PrintForward ();
  // set a string separator automatically inserted
  // between each call to a printing function.
  printer.SetSeparator (" - ");
  // set the default print function: invoked if no 
  // specialized function has been provided for a header
  // or trailer
  printer.AddDefaultPrinter (MakeCallback (&DoPrintDefault));
  // set the payload print function
  printer.AddPayloadPrinter (MakeCallback (&DoPrintPayload));
  // set the print function for the header type Ipv4Header.
  printer.AddHeaderPrinter (MakeCallback (&DoPrintIpv4Header),
                            MakeCallback (&DoPrintIpv4HeaderFragment));


  // We create a packet with 1000 bytes of zero payload
  Packet p (1000);
  Ipv4Header ipv4;
  UdpHeader udp;
  ipv4.SetSource (Ipv4Address ("192.168.0.1"));
  ipv4.SetDestination (Ipv4Address ("192.168.0.2"));
  udp.SetSource (1025);
  udp.SetDestination (80);
  p.AddHeader (udp);
  p.AddHeader (ipv4);

  std::cout << "full packet size=" << p.GetSize () << std::endl;
  p.Print (std::cout, printer);
  std::cout << std::endl;


  // fragment our packet in 3 pieces
  Packet p1 = p.CreateFragment (0, 2);
  Packet p2 = p.CreateFragment (2, 1000);
  Packet p3 = p.CreateFragment (1002, 26);
  std::cout << "fragment1" << std::endl;
  p1.Print (std::cout, printer);
  std::cout << std::endl;
  std::cout << "fragment2" << std::endl;
  p2.Print (std::cout, printer);
  std::cout << std::endl;
  std::cout << "fragment3" << std::endl;
  p3.Print (std::cout, printer);
  std::cout << std::endl;

  // aggregate all 3 fragments of the original packet
  // to reconstruct a copy of the original packet.
  Packet aggregate = p1;
  aggregate.AddAtEnd (p2);
  aggregate.AddAtEnd (p3);
  std::cout << "aggregated" << std::endl;
  aggregate.Print (std::cout, printer);
  std::cout << std::endl;
}



int main (int argc, char *argv[])
{
  Packet::EnableMetadata ();

  DefaultPrint ();

  NonDefaultPrint ();

  return 0;
}
