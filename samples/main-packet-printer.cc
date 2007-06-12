/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/packet.h"
#include "ns3/header.h"
#include "ns3/packet-printer.h"

using namespace ns3;

#define MY_HEADER(name,size) \
class MyHeader##name : public Header \
{ \
public: \
  void Set (uint8_t v) \
  { \
    m_v = v; \
  } \
  uint8_t Get (void) const \
  { \
    return m_v; \
  } \
private: \
  virtual std::string DoGetName (void) const \
  { \
    return #name; \
  } \
  virtual void PrintTo (std::ostream &os) const \
  { \
    uint32_t v = m_v; \
    os << #name << ", v=" << v; \
  } \
  virtual uint32_t GetSerializedSize (void) const \
  { \
    return size; \
  } \
  virtual void SerializeTo (Buffer::Iterator start) const \
  { \
    start.WriteU8 (m_v, size); \
  } \
  virtual uint32_t DeserializeFrom (Buffer::Iterator start) \
  { \
    NS_ASSERT (size >= 1); \
    m_v = start.ReadU8 (); \
    for (uint32_t i = 2; i < size; i++) \
      { \
	NS_ASSERT (start.ReadU8 () == m_v); \
      } \
    return size; \
  } \
  uint8_t m_v; \
};

MY_HEADER(A,10);
MY_HEADER(B,100);
MY_HEADER(C,20);

void DefaultPrint (void)
{
  // We create a packet with 1000 bytes of zero payload
  // and add 3 headers to this packet.
  Packet p (1000);
  MyHeaderA a;
  MyHeaderB b;
  MyHeaderC c;
  a.Set (1);
  b.Set (3);
  c.Set (2);
  p.AddHeader (a);
  p.AddHeader (b);
  p.AddHeader (c);

  std::cout << "full packet size=" << p.GetSize () << std::endl;
  p.Print (std::cout);
  std::cout << std::endl;


  // Now, we fragment our packet in 3 consecutive pieces.
  Packet p1 = p.CreateFragment (0, 2);
  Packet p2 = p.CreateFragment (2, 1000);
  Packet p3 = p.CreateFragment (1002, 128);

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
  os << "default name="<<name<<", size=" << size << ", [" << info.start << ":" << info.end << "]";
}
void
DoPrintPayload (std::ostream & os,uint32_t packetUid,uint32_t size,
                struct PacketPrinter::FragmentInformation info)
{
  os << "payload size="<<size<< ", [" << info.start << ":" << info.end << "]";
}
void 
DoPrintMyHeaderA (std::ostream &os, uint32_t packetUid, uint32_t size, const MyHeaderA *headerA)
{
  uint32_t v = headerA->Get ();
  os << "A v=" << v;
}
void 
DoPrintMyHeaderAFragment (std::ostream &os, uint32_t packetUid, uint32_t size,
                             std::string &name, struct PacketPrinter::FragmentInformation info)
{
  os << "A fragment";
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
  // set the print function for the header type MyHeaderA.
  printer.AddHeaderPrinter (MakeCallback (&DoPrintMyHeaderA),
                            MakeCallback (&DoPrintMyHeaderAFragment));


  // We create a packet with 1000 bytes of zero payload
  // and add 3 headers to this packet.
  Packet p (1000);
  MyHeaderA a;
  MyHeaderB b;
  MyHeaderC c;
  a.Set (1);
  b.Set (3);
  c.Set (2);
  p.AddHeader (a);
  p.AddHeader (b);
  p.AddHeader (c);

  std::cout << "full packet size=" << p.GetSize () << std::endl;
  p.Print (std::cout, printer);
  std::cout << std::endl;


  // fragment our packet in 3 pieces
  Packet p1 = p.CreateFragment (0, 2);
  Packet p2 = p.CreateFragment (2, 1000);
  Packet p3 = p.CreateFragment (1002, 128);
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
