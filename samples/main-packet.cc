/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/packet.h"
#include "ns3/header.h"
#include <iostream>

using namespace ns3;

/* A sample Header implementation
 */
class MyHeader : public Header {
public:
  MyHeader ();
  virtual ~MyHeader ();

  void SetData (uint16_t data);
  uint16_t GetData (void) const;
private:
  virtual std::string DoGetName (void) const;
  virtual void PrintTo (std::ostream &os) const;
  virtual void SerializeTo (Buffer::Iterator start) const;
  virtual uint32_t DeserializeFrom (Buffer::Iterator start);
  virtual uint32_t GetSerializedSize (void) const;

  uint16_t m_data;
};

MyHeader::MyHeader ()
{}
MyHeader::~MyHeader ()
{}
std::string 
MyHeader::DoGetName (void) const
{
  return "MyHeader";
}
void 
MyHeader::PrintTo (std::ostream &os) const
{
  os << "MyHeader data=" << m_data << std::endl;
}
uint32_t
MyHeader::GetSerializedSize (void) const
{
  return 2;
}
void
MyHeader::SerializeTo (Buffer::Iterator start) const
{
  // serialize in head of buffer
  start.WriteHtonU16 (m_data);
}
uint32_t
MyHeader::DeserializeFrom (Buffer::Iterator start)
{
  // deserialize from head of buffer
  m_data = start.ReadNtohU16 ();
  return GetSerializedSize ();
}

void 
MyHeader::SetData (uint16_t data)
{
  m_data = data;
}
uint16_t 
MyHeader::GetData (void) const
{
  return m_data;
}

/* A sample Tag implementation
 */
struct MyTag {
  uint16_t m_streamId;
};

static TagRegistration<struct MyTag> g_MyTagRegistration ("ns3::MyTag", 0);


static void
Receive (Packet p)
{
  MyHeader my;
  p.RemoveHeader (my);
  std::cout << "received data=" << my.GetData () << std::endl;
  struct MyTag myTag;
  p.PeekTag (myTag);
}


int main (int argc, char *argv[])
{
  Packet p;
  MyHeader my;
  my.SetData (2);
  std::cout << "send data=2" << std::endl;
  p.AddHeader (my);
  struct MyTag myTag;
  myTag.m_streamId = 5;
  p.AddTag (myTag);
  Receive (p);
  return 0;
}
