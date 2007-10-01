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
#include "ns3/tag.h"
#include "ns3/packet.h"
#include <iostream>

using namespace ns3;

// define this class in a public header
class MyTag : public Tag
{
public:
  // we have to define a public constructor
  MyTag ();
  // we have to define a public copy constructor
  MyTag (const MyTag &other);
  // we have to define a public destructor
  ~MyTag ();
  // we have to define a public static GetUid method
  static uint32_t GetUid (void);
  // we have to define a public Print method
  void Print (std::ostream &os) const;
  // we have to define a public GetSerializedSize method
  uint32_t GetSerializedSize (void) const;
  // we have to define a public Serialize method
  void Serialize (Buffer::Iterator i) const;
  // we have to define a public Deserialize method
  uint32_t Deserialize (Buffer::Iterator i);
  
  // these are our accessors to our tag structure
  void SetSimpleValue (uint8_t value);
  uint8_t GetSimpleValue (void) const;
private:
  uint8_t m_simpleValue;
};

MyTag::MyTag ()
{}
MyTag::MyTag (const MyTag &other)
  : m_simpleValue (other.m_simpleValue)
{}
MyTag::~MyTag ()
{}
uint32_t 
MyTag::GetUid (void)
{
  // we input a unique string to AllocateUid
  // to avoid name collisions.
  static uint32_t uid = AllocateUid<MyTag> ("MyTag.tests.nsnam.org");
  return uid;
}
void 
MyTag::Print (std::ostream &os) const
{
  // print the content of this tag for Packet::PrintTags
  os << "MyTag=0x" << std::hex << (uint32_t)m_simpleValue << std::dec;
}
uint32_t 
MyTag::GetSerializedSize (void) const
{
  // we do not want to deal with parallel simulations
  // so we return 0.
  return 0;
}
void 
MyTag::Serialize (Buffer::Iterator i) const
{
  // we will never be invoked because we are not doing
  // parallel simulations so, we assert.
  NS_ASSERT (false);
}
uint32_t
MyTag::Deserialize (Buffer::Iterator i)
{
  // we will never be invoked because we are not doing
  // parallel simulations so, we assert.
  NS_ASSERT (false);
  // theoretically, return the number of bytes read
  return 0;
}


void 
MyTag::SetSimpleValue (uint8_t value)
{
  m_simpleValue = value;
}
uint8_t 
MyTag::GetSimpleValue (void) const
{
  return m_simpleValue;
}


int main (int argc, char *argv[])
{
  // create a tag.
  MyTag tag;
  tag.SetSimpleValue (0x56);

  // store the tag in a packet.
  Ptr<Packet> p = Create<Packet> ();
  p->AddTag (tag);

  // create a copy of the packet
  Ptr<Packet> aCopy = p->Copy ();

  // read the tag from the packet copy
  MyTag tagCopy;
  p->PeekTag (tagCopy);

  // the copy and the original are the same !
  NS_ASSERT (tagCopy.GetSimpleValue () == tag.GetSimpleValue ());

  aCopy->PrintTags (std::cout);
  std::cout << std::endl;

  return 0;
}
