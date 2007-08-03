/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/packet.h"
#include "ns3/header.h"
#include <iostream>

using namespace ns3;

/* A sample Header implementation
 */
class MyHeader : public Header 
{
public:
  static const char *GetUid (void);

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

const char *
MyHeader::GetUid (void)
{
  // make sure the string is really unique.
  return "MyHeader.test.nsnam.org";
}

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
class MyTag 
{
public:
  static const char *GetUid (void) {return "MyTag.test.nsnam.org";}
  void Print (std::ostream &os) const {}
  uint32_t GetSerializedSize (void) const {return 0;}
  void Serialize (Buffer::Iterator i) const {}
  uint32_t Deserialize (Buffer::Iterator i) {return 0;}

  uint16_t m_streamId;
};

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
