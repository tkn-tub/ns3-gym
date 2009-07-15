/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 1997, 1998 Carnegie Mellon University.
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
 * Authors: The AODV code developed by the CMU/MONARCH group was optimized and
 * tuned by Samir Das and Mahesh Marina, University of Cincinnati.
 * The work was partially done in Sun Microsystems.
 *
 * Ported to ns-3 by Elena Borovkova <borovkovaes@iitp.ru>
 */
#include "ns3/test.h"
#include "aodv-packet.h"
#include "ns3/address-utils.h"
#include "ns3/packet.h"

namespace ns3 {
namespace aodv {

TypeHeader::TypeHeader(uint8_t t) : type(t), valid(true)
{
  switch (type)
  {
  case AODVTYPE_RREQ:
  case AODVTYPE_RREP:
  case AODVTYPE_RERR:
  case AODVTYPE_RREP_ACK:
    break;
  default:
    valid = false;
  }
}

TypeId
TypeHeader::GetInstanceTypeId() const
{
  return TypeId();
}

uint32_t
TypeHeader::GetSerializedSize () const
{
  return 1;
}

void
TypeHeader::Serialize (Buffer::Iterator i) const
{
  i.WriteU8(type);
}

uint32_t
TypeHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  type = i.ReadU8 ();
  valid = true;
  switch (type)
  {
  case AODVTYPE_RREQ:
  case AODVTYPE_RREP:
  case AODVTYPE_RERR:
  case AODVTYPE_RREP_ACK:
    break;
  default:
    valid = false;
  }
  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void
TypeHeader::Print (std::ostream &os) const
{
  switch(type)
  {
  case AODVTYPE_RREQ:
  {
    os << "RREQ" << "\n";
    break;
  }
  case AODVTYPE_RREP:
  {
    os << "RREP" << "\n";
    break;
  }
  case AODVTYPE_RERR:
  {
    os << "RERR" << "\n";
    break;
  }
  case AODVTYPE_RREP_ACK:
  {
    os << "RREP_ACK" << "\n";
    break;
  }
  default:
    os << "";
  }
}

bool
TypeHeader::operator==(TypeHeader const & o) const
{
  return ( type == o.type && valid == o.valid );
}

std::ostream & operator<<(std::ostream & os, TypeHeader const & h)
{
  h.Print(os);
  return os;
}


#ifdef RUN_SELF_TESTS
/// Unit test for TypeHeader
struct TypeHeaderTest : public Test
{
  TypeHeaderTest () : Test ("AODV/TypeHeader") {}
  virtual bool RunTests();
};

/// Test instance
static TypeHeaderTest g_TypeHeaderTest;

bool TypeHeaderTest::RunTests ()
{
  bool result(true);

  TypeHeader h(AODVTYPE_RREQ);
  NS_TEST_ASSERT(h.IsValid());
  TypeHeader h1(13);
  NS_TEST_ASSERT(!h1.IsValid());

  Ptr<Packet> p = Create<Packet> ();
  p->AddHeader (h);
  TypeHeader h2(AODVTYPE_RREP);
  uint32_t bytes = p->RemoveHeader(h2);
  NS_TEST_ASSERT_EQUAL (bytes, 1);
  NS_TEST_ASSERT_EQUAL (h, h2);
  return result;
}
#endif


//-----------------------------------------------------------------------------
// RREQ
//-----------------------------------------------------------------------------
RreqHeader::RreqHeader () : rq_flags(0), reserved(0), rq_hop_count(0), rq_bcast_id(0),
rq_dst_seqno(0), rq_src_seqno(0)
{
  SetGratiousRrep (false);
  SetDestinationOnly (false);
  SetUnknownSeqno (false);
}

TypeId
RreqHeader::GetInstanceTypeId() const
{
  return TypeId();
}

uint32_t 
RreqHeader::GetSerializedSize () const
{
  return 23;
}

void 
RreqHeader::Serialize (Buffer::Iterator i) const
{
  i.WriteU8 (rq_flags);
  i.WriteU8 (reserved);
  i.WriteU8 (rq_hop_count);
  i.WriteHtonU32 (rq_bcast_id);
  WriteTo (i, rq_dst);
  i.WriteHtonU32 (rq_dst_seqno);
  WriteTo (i, rq_src);
  i.WriteHtonU32 (rq_src_seqno);
}

uint32_t 
RreqHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  rq_flags = i.ReadU8 ();
  reserved = i.ReadU8 ();
  rq_hop_count = i.ReadU8 ();
  rq_bcast_id = i.ReadNtohU32 ();
  ReadFrom (i, rq_dst);
  rq_dst_seqno = i.ReadNtohU32 ();
  ReadFrom (i, rq_src);
  rq_src_seqno = i.ReadNtohU32 ();

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void 
RreqHeader::Print (std::ostream &os) const
{
  os << "RREQ ID " << rq_bcast_id << "\n"
  << "destination: ipv4 " << rq_dst << " "
  << "sequence number " << rq_dst_seqno << "\n"
  << "source: ipv4 " << rq_src << " "
  << "sequence number " << rq_src_seqno << "\n"
  << "flags:\n"
  << "Gratuitous RREP " << (*this).GetGratiousRrep() << "\n"
  << "Destination only " << (*this).GetDestinationOnly() << "\n"
  << "Unknown sequence number " << (*this).GetUnknownSeqno() << "\n";
}

std::ostream & operator<<(std::ostream & os, RreqHeader const & h)
{
  h.Print (os);
  return os;
}

void 
RreqHeader::SetGratiousRrep (bool f)
{
  if (f) rq_flags |= (1 << 5);
  else   rq_flags &= ~(1 << 5);
}

bool 
RreqHeader::GetGratiousRrep () const
{
  return (rq_flags & (1 << 5));
}

void 
RreqHeader::SetDestinationOnly (bool f)
{
  if (f) rq_flags |= (1 << 4);
  else   rq_flags &= ~(1 << 4);
}

bool 
RreqHeader::GetDestinationOnly () const
{
  return (rq_flags & (1 << 4));
}

void 
RreqHeader::SetUnknownSeqno (bool f)
{
  if (f) rq_flags |= (1 << 3);
  else   rq_flags &= ~(1 << 3);
}

bool 
RreqHeader::GetUnknownSeqno () const
{
  return (rq_flags & (1 << 3));
}

bool
RreqHeader::operator==(RreqHeader const & o) const
{
  return (rq_flags == o.rq_flags && reserved == o.reserved &&
      rq_hop_count == o.rq_hop_count && rq_bcast_id == o.rq_bcast_id &&
      rq_dst == o.rq_dst && rq_dst_seqno == o.rq_dst_seqno && 
      rq_src == o.rq_src && rq_src_seqno == o.rq_src_seqno);
}

#ifdef RUN_SELF_TESTS
/// Unit test for RREQ
struct RreqHeaderTest : public Test
{
  RreqHeaderTest () : Test ("AODV/RREQ") {}
  virtual bool RunTests();
};

/// Test instance
static RreqHeaderTest g_RreqHeaderTest;

bool RreqHeaderTest::RunTests ()
{
  bool result(true);

  RreqHeader h;
  h.SetDst (Ipv4Address("1.2.3.4"));
  h.SetDstSeqno (123);
  h.SetOrigin (Ipv4Address("4.3.2.1"));
  h.SetOriginSeqno (321);
  h.SetId (1);
  h.SetGratiousRrep (true);
  NS_TEST_ASSERT(h.GetGratiousRrep ());
  h.SetDestinationOnly (true);
  NS_TEST_ASSERT(h.GetDestinationOnly ());
  h.SetUnknownSeqno (true);
  NS_TEST_ASSERT(h.GetUnknownSeqno ());
  Ptr<Packet> p = Create<Packet> ();
  p->AddHeader (h);
  RreqHeader h2;
  uint32_t bytes = p->RemoveHeader(h2);
  NS_TEST_ASSERT_EQUAL (bytes, 23);
  NS_TEST_ASSERT_EQUAL (h, h2);
  return result;
}
#endif

//-----------------------------------------------------------------------------
// RREP
//-----------------------------------------------------------------------------

RrepHeader::RrepHeader() :rp_flags(0), prefixSize(0),  rp_hop_count(0), rp_dst_seqno(0)
{
  SetAckRequired(false);
}

TypeId
RrepHeader::GetInstanceTypeId() const
{
  return TypeId();
}

uint32_t
RrepHeader::GetSerializedSize () const
{
  return 19;
}

void
RrepHeader::Serialize (Buffer::Iterator i) const
{
  i.WriteU8(rp_flags);
  i.WriteU8(prefixSize);
  i.WriteU8 (rp_hop_count);
  WriteTo (i, rp_dst);
  i.WriteHtonU32 (rp_dst_seqno);
  WriteTo (i, rp_src);
  i.WriteHtonU32 (rp_lifetime);
}

uint32_t
RrepHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  rp_flags = i.ReadU8 ();
  prefixSize = i.ReadU8 ();
  rp_hop_count = i.ReadU8 ();
  ReadFrom (i, rp_dst);
  rp_dst_seqno = i.ReadNtohU32 ();
  ReadFrom (i, rp_src);
  rp_lifetime = i.ReadNtohU32 ();

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void
RrepHeader::Print (std::ostream &os) const
{
  os << "destination: ipv4 " << rp_dst
  << "sequence number " <<  rp_dst_seqno;
  if(prefixSize != 0)
    os << "prefix size " << prefixSize << "\n";
  else os << "\n";
  os << "source ipv4 " << rp_src << "\n"
  << "life time " << rp_lifetime << "\n"
  << "acknowledgment required flag " << (*this).GetAckRequired() << "\n";
}

void
RrepHeader::SetLifeTime (Time t)
{
  rp_lifetime = t.GetMilliSeconds();
}
Time
RrepHeader::GetLifeTime () const
{
  Time t(MilliSeconds(rp_lifetime));
  return t;
}

void
RrepHeader::SetAckRequired (bool f)
{
  if (f) rp_flags |= (1 << 6);
  else   rp_flags &= ~(1 << 6);
}

bool
RrepHeader::GetAckRequired () const
{
  return (rp_flags & (1 << 6));
}

void
RrepHeader::SetPrefixSize(uint8_t sz)
{
  prefixSize = sz;
}

uint8_t
RrepHeader::GetPrefixSize() const
{
  return prefixSize;
}

bool
RrepHeader::operator==(RrepHeader const & o) const
{
  return (rp_flags == o.rp_flags && prefixSize == o.prefixSize &&
      rp_hop_count == o.rp_hop_count && rp_dst == o.rp_dst &&
      rp_dst_seqno == o.rp_dst_seqno && rp_src == o.rp_src &&
      rp_lifetime == o.rp_lifetime);
}


void
RrepHeader::SetHello(Ipv4Address src, uint32_t srcSeqNo, Time lifetime)
{
  rp_flags = 0;
  prefixSize = 0;
  rp_hop_count = 0;
  rp_dst = src;
  rp_dst_seqno = srcSeqNo;
  rp_src = src;
  rp_lifetime = lifetime.GetMilliSeconds ();
}

std::ostream & operator<<(std::ostream & os, RrepHeader const & h)
{
  h.Print (os);
  return os;
}

#ifdef RUN_SELF_TESTS
/// Unit test for RREP
struct RrepHeaderTest : public Test
{
  RrepHeaderTest () : Test ("AODV/RREP") {}
  virtual bool RunTests();
};

/// Test instance
static RrepHeaderTest g_RrepHeaderTest;

bool RrepHeaderTest::RunTests ()
{
  bool result(true);

  RrepHeader h;
  h.SetDst (Ipv4Address("1.2.3.4"));
  h.SetDstSeqno (123);
  h.SetOrigin (Ipv4Address("4.3.2.1"));
  h.SetLifeTime(MilliSeconds(1200));
  h.SetAckRequired(true);
  NS_TEST_ASSERT(h.GetAckRequired ());
  h.SetAckRequired(false);
  NS_TEST_ASSERT(!h.GetAckRequired ());
  h.SetPrefixSize(2);
  uint8_t sz = h.GetPrefixSize();
  NS_TEST_ASSERT_EQUAL(2, sz);

  Ptr<Packet> p = Create<Packet> ();
  p->AddHeader (h);
  RrepHeader h2;
  uint32_t bytes = p->RemoveHeader(h2);
  NS_TEST_ASSERT_EQUAL (bytes, 19);
  NS_TEST_ASSERT_EQUAL (h, h2);
  return result;
}
#endif

//-----------------------------------------------------------------------------
// RREP-ACK
//-----------------------------------------------------------------------------

RrepAckHeader::RrepAckHeader () : reserved(0)
{
}

TypeId
RrepAckHeader::GetInstanceTypeId() const
{
  return TypeId();
}

uint32_t
RrepAckHeader::GetSerializedSize () const
{
  return 1;
}

void
RrepAckHeader::Serialize (Buffer::Iterator i) const
{
  i.WriteU8(reserved);
}

uint32_t
RrepAckHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  reserved = i.ReadU8 ();
  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void
RrepAckHeader::Print (std::ostream &os) const
{
}

bool
RrepAckHeader::operator==(RrepAckHeader const & o) const
{
  return reserved == o.reserved;
}

std::ostream & operator<<(std::ostream & os, RrepAckHeader const & h)
{
  h.Print(os);
  return os;
}

#ifdef RUN_SELF_TESTS
/// Unit test for RREP-ACK
struct RrepAckHeaderTest : public Test
{
  RrepAckHeaderTest () : Test ("AODV/RREP-ACK") {}
  virtual bool RunTests();
};

/// Test instance
static RrepAckHeaderTest g_RrepAckHeaderTest;

bool RrepAckHeaderTest::RunTests ()
{
  bool result(true);

  RrepAckHeader h;
  Ptr<Packet> p = Create<Packet> ();
  p->AddHeader (h);
  RrepAckHeader h2;
  uint32_t bytes = p->RemoveHeader(h2);
  NS_TEST_ASSERT_EQUAL (bytes, 1);
  NS_TEST_ASSERT_EQUAL (h, h2);
  return result;
}
#endif

//-----------------------------------------------------------------------------
// RERR
//-----------------------------------------------------------------------------
RerrHeader::RerrHeader() : er_flag(0), reserved(0)
{
}

TypeId
RerrHeader::GetInstanceTypeId() const
{
  return TypeId();
}

uint32_t
RerrHeader::GetSerializedSize () const
{
  return ( 3 + 8*GetDestCount());
}

void
RerrHeader::Serialize (Buffer::Iterator i) const
{
  i.WriteU8(er_flag);
  i.WriteU8(reserved);
  i.WriteU8(GetDestCount());
  std::map<Ipv4Address, uint32_t>::const_iterator j;
  for(j = unreachable_dst.begin(); j != unreachable_dst.end(); ++j)
  {
    WriteTo (i, (*j).first);
    i.WriteHtonU32 ((*j).second);
  }
}

uint32_t
RerrHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  er_flag = i.ReadU8 ();
  reserved = i.ReadU8 ();
  uint8_t dest = i.ReadU8 ();
  unreachable_dst.clear();
  Ipv4Address address;
  uint32_t seqNo;
  for(uint8_t k = 0; k < dest; ++k)
  {
    ReadFrom (i, address);
    seqNo = i.ReadNtohU32 ();
    unreachable_dst.insert(std::make_pair(address, seqNo));
  }

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void
RerrHeader::Print (std::ostream &os) const
{
  os << "Unreachable destination (ipv4 address, seq. number):\n";
  std::map<Ipv4Address, uint32_t>::const_iterator j;
  for(j = unreachable_dst.begin(); j != unreachable_dst.end(); ++j)
  {
    os << (*j).first << ", " << (*j).second << "\n";
  }
  os << "No delete flag " << (*this).GetNoDelete() << "\n";
}

void
RerrHeader::SetNoDelete(bool f)
{
  if (f) er_flag |= (1 << 0);
  else   er_flag &= ~(1 << 0);
}

bool
RerrHeader::GetNoDelete() const
{
  return (er_flag & (1 << 0));
}

bool
RerrHeader::AddUnDestination(Ipv4Address dst, uint32_t seqNo)
{
  if(unreachable_dst.find(dst) != unreachable_dst.end())
    return false;

  NS_ASSERT (GetDestCount() < 255); // can't support more than 255 destinations in single RERR
  unreachable_dst.insert(std::make_pair(dst, seqNo));
  return true;
}

bool
RerrHeader::operator==(RerrHeader const & o) const
{
  if (er_flag != o.er_flag || reserved != o.reserved || GetDestCount() != o.GetDestCount())
    return false;

  std::map<Ipv4Address, uint32_t>::const_iterator j = unreachable_dst.begin();
  std::map<Ipv4Address, uint32_t>::const_iterator k = o.unreachable_dst.begin();
  for(uint8_t i = 0; i < GetDestCount(); ++i)
  {
    if ((j->first != k->first ) || (j->second != k->second))
      return false;

    j++;
    k++;
  }
  return true;
}

std::ostream & operator<<(std::ostream & os, RerrHeader const & h)
{
  h.Print(os);
  return os;
}

#ifdef RUN_SELF_TESTS
/// Unit test for RERR
struct RerrHeaderTest : public Test
{
  RerrHeaderTest () : Test ("AODV/RERR") {}
  virtual bool RunTests();
};

/// Test instance
static RerrHeaderTest g_RerrHeaderTest;

bool RerrHeaderTest::RunTests ()
{
  bool result(true);

  RerrHeader h;
  h.SetNoDelete(true);
  NS_TEST_ASSERT(h.GetNoDelete());
  Ipv4Address dst = Ipv4Address("1.2.3.4");
  NS_TEST_ASSERT(h.AddUnDestination(dst, 12));
  NS_TEST_ASSERT_EQUAL(h.GetDestCount(),1);
  NS_TEST_ASSERT(!h.AddUnDestination(dst, 13));
  Ipv4Address dst2 = Ipv4Address("4.3.2.1");
  NS_TEST_ASSERT(h.AddUnDestination(dst2, 12));
  NS_TEST_ASSERT_EQUAL(h.GetDestCount(), 2);

  Ptr<Packet> p = Create<Packet> ();
  p->AddHeader (h);
  RerrHeader h2;
  uint32_t bytes = p->RemoveHeader(h2);
  NS_TEST_ASSERT_EQUAL (bytes, h.GetSerializedSize());
  NS_TEST_ASSERT_EQUAL (h, h2);
  return result;
}
#endif
}}
