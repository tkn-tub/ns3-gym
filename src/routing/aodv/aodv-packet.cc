/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
 * Based on 
 *      NS-2 AODV model developed by the CMU/MONARCH group and optimized and
 *      tuned by Samir Das and Mahesh Marina, University of Cincinnati;
 * 
 *      AODV-UU implementation by Erik Nordström of Uppsala University
 *      http://core.it.uu.se/core/index.php/AODV-UU
 *
 * Authors: Elena Borovkova <borovkovaes@iitp.ru>
 *          Pavel Boyko <boyko@iitp.ru>
 */
#include "ns3/test.h"
#include "aodv-packet.h"
#include "ns3/address-utils.h"
#include "ns3/packet.h"

namespace ns3
{
namespace aodv
{

TypeHeader::TypeHeader (uint8_t t ) :
  m_type (t), m_valid (true)
{
  switch (m_type)
    {
    case AODVTYPE_RREQ:
    case AODVTYPE_RREP:
    case AODVTYPE_RERR:
    case AODVTYPE_RREP_ACK:
      break;
    default:
      m_valid = false;
    }
}

TypeId
TypeHeader::GetInstanceTypeId () const
{
  return TypeId ();
}

uint32_t
TypeHeader::GetSerializedSize () const
{
  return 1;
}

void
TypeHeader::Serialize (Buffer::Iterator i ) const
{
  i.WriteU8 (m_type);
}

uint32_t
TypeHeader::Deserialize (Buffer::Iterator start )
{
  Buffer::Iterator i = start;
  m_type = i.ReadU8 ();
  m_valid = true;
  switch (m_type)
    {
    case AODVTYPE_RREQ:
    case AODVTYPE_RREP:
    case AODVTYPE_RERR:
    case AODVTYPE_RREP_ACK:
      break;
    default:
      m_valid = false;
    }
  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void
TypeHeader::Print (std::ostream &os ) const
{
  switch (m_type)
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
      os << "UNKNOWN_TYPE";
    }
}

bool
TypeHeader::operator== (TypeHeader const & o ) const
{
  return (m_type == o.m_type && m_valid == o.m_valid);
}

std::ostream &
operator<< (std::ostream & os, TypeHeader const & h )
{
  h.Print (os);
  return os;
}

#ifdef RUN_SELF_TESTS
/// Unit test for TypeHeader
struct TypeHeaderTest : public Test
  {
    TypeHeaderTest () : Test ("AODV/TypeHeader")
      {}
    virtual bool RunTests();
  };

/// Test instance
static TypeHeaderTest g_TypeHeaderTest;

bool TypeHeaderTest::RunTests ()
  {
    bool result(true);

    TypeHeader badType(12);
    NS_TEST_ASSERT(!badType.IsValid ());

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
RreqHeader::RreqHeader () :
  m_flags (0), m_reserved (0), m_hopCount (0), m_broadcastID (0), m_dstSeqNo (0), m_srcSeqNo (0)
{
}

TypeId
RreqHeader::GetInstanceTypeId () const
{
  return TypeId ();
}

uint32_t
RreqHeader::GetSerializedSize () const
{
  return 23;
}

void
RreqHeader::Serialize (Buffer::Iterator i ) const
{
  i.WriteU8 (m_flags);
  i.WriteU8 (m_reserved);
  i.WriteU8 (m_hopCount);
  i.WriteHtonU32 (m_broadcastID);
  WriteTo (i, m_dst);
  i.WriteHtonU32 (m_dstSeqNo);
  WriteTo (i, m_origin);
  i.WriteHtonU32 (m_srcSeqNo);
}

uint32_t
RreqHeader::Deserialize (Buffer::Iterator start )
{
  Buffer::Iterator i = start;
  m_flags = i.ReadU8 ();
  m_reserved = i.ReadU8 ();
  m_hopCount = i.ReadU8 ();
  m_broadcastID = i.ReadNtohU32 ();
  ReadFrom (i, m_dst);
  m_dstSeqNo = i.ReadNtohU32 ();
  ReadFrom (i, m_origin);
  m_srcSeqNo = i.ReadNtohU32 ();

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void
RreqHeader::Print (std::ostream &os ) const
{
  os << "RREQ ID " << m_broadcastID << "\n" << "destination: ipv4 " << m_dst << " " << "sequence number " << m_dstSeqNo << "\n" << "source: ipv4 "
      << m_origin << " " << "sequence number " << m_srcSeqNo << "\n" << "flags:\n" << "Gratuitous RREP " << (*this).GetGratiousRrep () << "\n"
      << "Destination only " << (*this).GetDestinationOnly () << "\n" << "Unknown sequence number " << (*this).GetUnknownSeqno () << "\n";
}

std::ostream &
operator<< (std::ostream & os, RreqHeader const & h )
{
  h.Print (os);
  return os;
}

void
RreqHeader::SetGratiousRrep (bool f )
{
  if (f)
    m_flags |= (1 << 5);
  else
    m_flags &= ~(1 << 5);
}

bool
RreqHeader::GetGratiousRrep () const
{
  return (m_flags & (1 << 5));
}

void
RreqHeader::SetDestinationOnly (bool f )
{
  if (f)
    m_flags |= (1 << 4);
  else
    m_flags &= ~(1 << 4);
}

bool
RreqHeader::GetDestinationOnly () const
{
  return (m_flags & (1 << 4));
}

void
RreqHeader::SetUnknownSeqno (bool f )
{
  if (f)
    m_flags |= (1 << 3);
  else
    m_flags &= ~(1 << 3);
}

bool
RreqHeader::GetUnknownSeqno () const
{
  return (m_flags & (1 << 3));
}

bool
RreqHeader::operator== (RreqHeader const & o ) const
{
  return (m_flags == o.m_flags && m_reserved == o.m_reserved && m_hopCount == o.m_hopCount && m_broadcastID == o.m_broadcastID && m_dst == o.m_dst
      && m_dstSeqNo == o.m_dstSeqNo && m_origin == o.m_origin && m_srcSeqNo == o.m_srcSeqNo);
}

#ifdef RUN_SELF_TESTS
/// Unit test for RREQ
struct RreqHeaderTest : public Test
  {
    RreqHeaderTest () : Test ("AODV/RREQ")
      {}
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
    h.SetGratiousRrep (false);
    NS_TEST_ASSERT_EQUAL(h.GetGratiousRrep (), 0);
    h.SetDestinationOnly (true);
    NS_TEST_ASSERT(h.GetDestinationOnly ());
    h.SetDestinationOnly (false);
    NS_TEST_ASSERT_EQUAL(h.GetDestinationOnly (), 0);
    h.SetUnknownSeqno (true);
    NS_TEST_ASSERT(h.GetUnknownSeqno ());
    h.SetUnknownSeqno (false);
    NS_TEST_ASSERT_EQUAL(h.GetUnknownSeqno (), 0);

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

RrepHeader::RrepHeader (uint8_t prefixSize, uint8_t hopCount, Ipv4Address dst, uint32_t dstSeqNo, Ipv4Address origin, Time lifeTime ) :
  m_flags (0), m_prefixSize (prefixSize), m_hopCount (hopCount), m_dst (dst), m_dstSeqNo (dstSeqNo), m_origin (origin)
{
  m_lifeTime = uint32_t (lifeTime.GetMilliSeconds ());
}

TypeId
RrepHeader::GetInstanceTypeId () const
{
  return TypeId ();
}

uint32_t
RrepHeader::GetSerializedSize () const
{
  return 19;
}

void
RrepHeader::Serialize (Buffer::Iterator i ) const
{
  i.WriteU8 (m_flags);
  i.WriteU8 (m_prefixSize);
  i.WriteU8 (m_hopCount);
  WriteTo (i, m_dst);
  i.WriteHtonU32 (m_dstSeqNo);
  WriteTo (i, m_origin);
  i.WriteHtonU32 (m_lifeTime);
}

uint32_t
RrepHeader::Deserialize (Buffer::Iterator start )
{
  Buffer::Iterator i = start;

  m_flags = i.ReadU8 ();
  m_prefixSize = i.ReadU8 ();
  m_hopCount = i.ReadU8 ();
  ReadFrom (i, m_dst);
  m_dstSeqNo = i.ReadNtohU32 ();
  ReadFrom (i, m_origin);
  m_lifeTime = i.ReadNtohU32 ();

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void
RrepHeader::Print (std::ostream &os ) const
{
  os << "destination: ipv4 " << m_dst << "sequence number " << m_dstSeqNo;
  if (m_prefixSize != 0)
    os << "prefix size " << m_prefixSize << "\n";
  else
    os << "\n";
  os << "source ipv4 " << m_origin << "\n" << "life time " << m_lifeTime << "\n" << "acknowledgment required flag " << (*this).GetAckRequired ()
      << "\n";
}

void
RrepHeader::SetLifeTime (Time t )
{
  m_lifeTime = t.GetMilliSeconds ();
}
Time
RrepHeader::GetLifeTime () const
{
  Time t (MilliSeconds (m_lifeTime));
  return t;
}

void
RrepHeader::SetAckRequired (bool f )
{
  if (f)
    m_flags |= (1 << 6);
  else
    m_flags &= ~(1 << 6);
}

bool
RrepHeader::GetAckRequired () const
{
  return (m_flags & (1 << 6));
}

void
RrepHeader::SetPrefixSize (uint8_t sz )
{
  m_prefixSize = sz;
}

uint8_t
RrepHeader::GetPrefixSize () const
{
  return m_prefixSize;
}

bool
RrepHeader::operator== (RrepHeader const & o ) const
{
  return (m_flags == o.m_flags && m_prefixSize == o.m_prefixSize && m_hopCount == o.m_hopCount && m_dst == o.m_dst && m_dstSeqNo == o.m_dstSeqNo
      && m_origin == o.m_origin && m_lifeTime == o.m_lifeTime);
}

void
RrepHeader::SetHello (Ipv4Address origin, uint32_t srcSeqNo, Time lifetime )
{
  m_flags = 0;
  m_prefixSize = 0;
  m_hopCount = 0;
  m_dst = origin;
  m_dstSeqNo = srcSeqNo;
  m_origin = origin;
  m_lifeTime = lifetime.GetMilliSeconds ();
}

std::ostream &
operator<< (std::ostream & os, RrepHeader const & h )
{
  h.Print (os);
  return os;
}

#ifdef RUN_SELF_TESTS
/// Unit test for RREP
struct RrepHeaderTest : public Test
  {
    RrepHeaderTest () : Test ("AODV/RREP")
      {}
    virtual bool RunTests();
  };

/// Test instance
static RrepHeaderTest g_RrepHeaderTest;

bool RrepHeaderTest::RunTests ()
  {
    bool result(true);

    RrepHeader h (/*prefixSize*/0, /*hopCount*/12, /*dst*/Ipv4Address("1.2.3.4"), /*dstSeqNo*/2,
        /*origin*/Ipv4Address("4.3.2.1"), /*lifetime*/Seconds(3));
    NS_TEST_ASSERT_EQUAL (h.GetPrefixSize (), 0);
    NS_TEST_ASSERT_EQUAL (h.GetHopCount (), 12);
    NS_TEST_ASSERT_EQUAL (h.GetDst (), Ipv4Address("1.2.3.4"));
    NS_TEST_ASSERT_EQUAL (h.GetDstSeqno (), 2);
    NS_TEST_ASSERT_EQUAL (h.GetOrigin (), Ipv4Address("4.3.2.1"));
    NS_TEST_ASSERT_EQUAL (h.GetLifeTime(), Seconds(3));
    h.SetDst (Ipv4Address("1.1.1.1"));
    NS_TEST_ASSERT_EQUAL (h.GetDst (), Ipv4Address("1.1.1.1"));
    h.SetDstSeqno (123);
    NS_TEST_ASSERT_EQUAL (h.GetDstSeqno (), 123);
    h.SetOrigin (Ipv4Address("4.4.4.4"));
    NS_TEST_ASSERT_EQUAL (h.GetOrigin (), Ipv4Address("4.4.4.4"));
    h.SetLifeTime(MilliSeconds(1200));
    NS_TEST_ASSERT_EQUAL (h.GetLifeTime(), MilliSeconds(1200));
    h.SetAckRequired(true);
    NS_TEST_ASSERT(h.GetAckRequired ());
    h.SetAckRequired(false);
    NS_TEST_ASSERT(!h.GetAckRequired ());
    h.SetPrefixSize(2);
    NS_TEST_ASSERT_EQUAL (h.GetPrefixSize (), 2);

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

RrepAckHeader::RrepAckHeader () :
  m_reserved (0)
{
}

TypeId
RrepAckHeader::GetInstanceTypeId () const
{
  return TypeId ();
}

uint32_t
RrepAckHeader::GetSerializedSize () const
{
  return 1;
}

void
RrepAckHeader::Serialize (Buffer::Iterator i ) const
{
  i.WriteU8 (m_reserved);
}

uint32_t
RrepAckHeader::Deserialize (Buffer::Iterator start )
{
  Buffer::Iterator i = start;
  m_reserved = i.ReadU8 ();
  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void
RrepAckHeader::Print (std::ostream &os ) const
{
}

bool
RrepAckHeader::operator== (RrepAckHeader const & o ) const
{
  return m_reserved == o.m_reserved;
}

std::ostream &
operator<< (std::ostream & os, RrepAckHeader const & h )
{
  h.Print (os);
  return os;
}

#ifdef RUN_SELF_TESTS
/// Unit test for RREP-ACK
struct RrepAckHeaderTest : public Test
  {
    RrepAckHeaderTest () : Test ("AODV/RREP-ACK")
      {}
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
RerrHeader::RerrHeader () :
  m_flag (0), m_reserved (0)
{
}

TypeId
RerrHeader::GetInstanceTypeId () const
{
  return TypeId ();
}

uint32_t
RerrHeader::GetSerializedSize () const
{
  return (3 + 8 * GetDestCount ());
}

void
RerrHeader::Serialize (Buffer::Iterator i ) const
{
  i.WriteU8 (m_flag);
  i.WriteU8 (m_reserved);
  i.WriteU8 (GetDestCount ());
  std::map<Ipv4Address, uint32_t>::const_iterator j;
  for (j = m_unreachableDstSeqNo.begin (); j != m_unreachableDstSeqNo.end (); ++j)
    {
      WriteTo (i, (*j).first);
      i.WriteHtonU32 ((*j).second);
    }
}

uint32_t
RerrHeader::Deserialize (Buffer::Iterator start )
{
  Buffer::Iterator i = start;
  m_flag = i.ReadU8 ();
  m_reserved = i.ReadU8 ();
  uint8_t dest = i.ReadU8 ();
  m_unreachableDstSeqNo.clear ();
  Ipv4Address address;
  uint32_t seqNo;
  for (uint8_t k = 0; k < dest; ++k)
    {
      ReadFrom (i, address);
      seqNo = i.ReadNtohU32 ();
      m_unreachableDstSeqNo.insert (std::make_pair (address, seqNo));
    }

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void
RerrHeader::Print (std::ostream &os ) const
{
  os << "Unreachable destination (ipv4 address, seq. number):\n";
  std::map<Ipv4Address, uint32_t>::const_iterator j;
  for (j = m_unreachableDstSeqNo.begin (); j != m_unreachableDstSeqNo.end (); ++j)
    {
      os << (*j).first << ", " << (*j).second << "\n";
    }
  os << "No delete flag " << (*this).GetNoDelete () << "\n";
}

void
RerrHeader::SetNoDelete (bool f )
{
  if (f)
    m_flag |= (1 << 0);
  else
    m_flag &= ~(1 << 0);
}

bool
RerrHeader::GetNoDelete () const
{
  return (m_flag & (1 << 0));
}

bool
RerrHeader::AddUnDestination (Ipv4Address dst, uint32_t seqNo )
{
  if (m_unreachableDstSeqNo.find (dst) != m_unreachableDstSeqNo.end ())
    return false;

  NS_ASSERT (GetDestCount() < 255); // can't support more than 255 destinations in single RERR
  m_unreachableDstSeqNo.insert (std::make_pair (dst, seqNo));
  return true;
}

bool
RerrHeader::RemoveUnDestination (std::pair<Ipv4Address, uint32_t> & un )
{
  if (GetDestCount () == 0)
    return false;
  std::map<Ipv4Address, uint32_t>::iterator i = m_unreachableDstSeqNo.end ();
  un = *i;
  m_unreachableDstSeqNo.erase (i);
  return true;
}

void
RerrHeader::Clear ()
{
  m_unreachableDstSeqNo.clear ();
  m_flag = 0;
  m_reserved = 0;
}

bool
RerrHeader::operator== (RerrHeader const & o ) const
{
  if (m_flag != o.m_flag || m_reserved != o.m_reserved || GetDestCount () != o.GetDestCount ())
    return false;

  std::map<Ipv4Address, uint32_t>::const_iterator j = m_unreachableDstSeqNo.begin ();
  std::map<Ipv4Address, uint32_t>::const_iterator k = o.m_unreachableDstSeqNo.begin ();
  for (uint8_t i = 0; i < GetDestCount (); ++i)
    {
      if ((j->first != k->first) || (j->second != k->second))
        return false;

      j++;
      k++;
    }
  return true;
}

std::ostream &
operator<< (std::ostream & os, RerrHeader const & h )
{
  h.Print (os);
  return os;
}

#ifdef RUN_SELF_TESTS
/// Unit test for RERR
struct RerrHeaderTest : public Test
  {
    RerrHeaderTest () : Test ("AODV/RERR")
      {}
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
}
}
