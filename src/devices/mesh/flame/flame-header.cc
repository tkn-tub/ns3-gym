/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */
#include "ns3/assert.h"
#include "ns3/address-utils.h"
#include "ns3/packet.h"
#include "ns3/test.h"

#include "flame-header.h"

namespace ns3 {
namespace flame {
FlameHeader::FlameHeader () :
  m_cost (0),
  m_seqno (0),
  m_origDst (Mac48Address ()),
  m_origSrc (Mac48Address ())
{}
FlameHeader::~FlameHeader ()
{}
TypeId
FlameHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FlameHeader")
    .SetParent<Header> ()
    .AddConstructor<FlameHeader> ()
    ;
  return tid;
}
TypeId
FlameHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
void
FlameHeader::Print (std::ostream &os) const
{
  os << "Cost             = " << (uint16_t)m_cost <<
      "\nSequence number  = " << m_seqno <<
      "\nOrig Destination = " << m_origDst <<
      "\nOrig Source      = " << m_origSrc << "\n";
}
uint32_t
FlameHeader::GetSerializedSize (void) const
{
  return 1  // Reserved
    + 1     // Cost
    + 2     // Seqno
    + 6     // Orig Dst
    + 6     // Orig Src
    + 2     // Flame Port
    ;
}
void
FlameHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteU8 (0);  //Reserved
  i.WriteU8 (m_cost); //Cost
  i.WriteHtonU16 (m_seqno); //Seqno
  WriteTo (i, m_origDst);
  WriteTo (i, m_origSrc);
  i.WriteHtonU16 (FlamePort);
}
uint32_t
FlameHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  i.Next (1);
  m_cost = i.ReadU8 ();
  m_seqno = i.ReadNtohU16 ();
  ReadFrom (i, m_origDst);
  ReadFrom (i, m_origSrc);
  uint16_t port = i.ReadNtohU16 ();
  NS_ASSERT(port == FlamePort);
  return i.GetDistanceFrom (start);
}
void
FlameHeader::AddCost (uint8_t cost)
{
  m_cost = (((uint16_t)cost+ (uint16_t)m_cost ) > 255) ? 255 : cost + m_cost;
}
uint8_t
FlameHeader::GetCost ()
{
  return m_cost;
}
void
FlameHeader::SetSeqno (uint16_t seqno)
{
  m_seqno = seqno;
}
uint16_t
FlameHeader::GetSeqno ()
{
  return m_seqno;
}
void
FlameHeader::SetOrigDst (Mac48Address dst)
{
  m_origDst = dst;
}
Mac48Address
FlameHeader::GetOrigDst ()
{
  return m_origDst;
}
void
FlameHeader::SetOrigSrc (Mac48Address src)
{
  m_origSrc = src;
}
Mac48Address
FlameHeader::GetOrigSrc ()
{
  return m_origSrc;
}
bool operator== (const FlameHeader & a, const FlameHeader & b)
{
  return (
      (a.m_cost == b.m_cost) &&
      (a.m_seqno == b.m_seqno) &&
      (a.m_origDst == b.m_origDst) &&
      (a.m_origSrc == b.m_origSrc)
      );
}

#ifdef RUN_SELF_TESTS

/// Built-in self test for FlameHeader
struct FlameHeaderBist : public Test 
{
  FlameHeaderBist () : Test ("mesh/flame/FlameHeader") {}
  virtual bool RunTests(); 
};

/// Test instance
static FlameHeaderBist g_FlameHeaderBist;

bool FlameHeaderBist::RunTests ()
{
  bool result (true);
    FlameHeader a;
    a.AddCost (123);
    a.SetSeqno (456);
    a.SetOrigDst (Mac48Address ("11:22:33:44:55:66"));
    a.SetOrigSrc (Mac48Address ("00:11:22:33:44:55"));
    Ptr<Packet> packet = Create<Packet> ();
    packet->AddHeader (a);
    FlameHeader b;
    packet->RemoveHeader (b);
    NS_TEST_ASSERT_EQUAL (b, a);
  return result;
}
#endif

} //namespace flame
} // namespace ns3
