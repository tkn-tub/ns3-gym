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
#include "ns3/mtag.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include <iostream>

using namespace ns3;

// define this class in a public header
class MyTag : public Mtag
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (MtagBuffer i) const;
  virtual void Deserialize (MtagBuffer i);
  
  // these are our accessors to our tag structure
  void SetSimpleValue (uint8_t value);
  uint8_t GetSimpleValue (void) const;
private:
  uint8_t m_simpleValue;
};

TypeId 
MyTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MyTag")
    .SetParent<Mtag> ()
    .AddConstructor<MyTag> ()
    .AddAttribute ("SimpleValue",
                   "A simple value",
                   EmptyAttributeValue (),
                   MakeUintegerAccessor (&MyTag::GetSimpleValue),
                   MakeUintegerChecker<uint8_t> ())
    ;
  return tid;
}
TypeId 
MyTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t 
MyTag::GetSerializedSize (void) const
{
  return 1;
}
void 
MyTag::Serialize (MtagBuffer i) const
{
  i.WriteU8 (m_simpleValue);
}
void 
MyTag::Deserialize (MtagBuffer i)
{
  m_simpleValue = i.ReadU8 ();
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
  p->AddMtag (tag);

  // create a copy of the packet
  Ptr<Packet> aCopy = p->Copy ();

  // read the tag from the packet copy
  MyTag tagCopy;
  p->FindFirstMatchingTag (tagCopy);

  // the copy and the original are the same !
  NS_ASSERT (tagCopy.GetSimpleValue () == tag.GetSimpleValue ());

  aCopy->PrintTags (std::cout);
  std::cout << std::endl;

  return 0;
}
