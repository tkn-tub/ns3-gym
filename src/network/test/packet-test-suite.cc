/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
#include "ns3/packet-tag-list.h"
#include "ns3/test.h"
#include "ns3/unused.h"
#include <limits>     // std:numeric_limits
#include <string>
#include <cstdarg>
#include <iostream>
#include <iomanip>
#include <ctime>

using namespace ns3;

//-----------------------------------------------------------------------------
// Unit tests
//-----------------------------------------------------------------------------
namespace {

class ATestTagBase : public Tag
{
public:
  ATestTagBase () : m_error (false), m_data (0) {}
  ATestTagBase (uint8_t data) : m_error (false), m_data (data) {}
  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void)
  {
    static TypeId tid = TypeId ("ATestTagBase")
      .SetParent<Tag> ()
      .SetGroupName ("Network")
      .HideFromDocumentation ()
    // No AddConstructor because this is an abstract class.
      ;
    return tid;
  }
  int GetData () const {
    int result = (int)m_data;
    return result;
  }
  bool m_error;
  uint8_t m_data;
};

template <int N>
class ATestTag : public ATestTagBase
{
public:
  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void) {
    std::ostringstream oss;
    oss << "anon::ATestTag<" << N << ">";
    static TypeId tid = TypeId (oss.str ().c_str ())
      .SetParent<ATestTagBase> ()
      .SetGroupName ("Network")
      .HideFromDocumentation ()
      .AddConstructor<ATestTag<N> > ()
    ;
    return tid;
  }
  virtual TypeId GetInstanceTypeId (void) const {
    return GetTypeId ();
  }
  virtual uint32_t GetSerializedSize (void) const {
    return N + sizeof(m_data);
  }
  virtual void Serialize (TagBuffer buf) const {
    buf.WriteU8 (m_data);
    for (uint32_t i = 0; i < N; ++i)
      {
        buf.WriteU8 (N);
      }
  }
  virtual void Deserialize (TagBuffer buf) {
    m_data = buf.ReadU8 ();
    for (uint32_t i = 0; i < N; ++i)
      {
        uint8_t v = buf.ReadU8 ();
        if (v != N)
          {
            m_error = true;
          }
      }
  }
  virtual void Print (std::ostream &os) const {
    os << N << "(" << m_data << ")";
  }
  ATestTag ()
    : ATestTagBase () {}
  ATestTag (uint8_t data)
    : ATestTagBase (data) {}
};

class ATestHeaderBase : public Header
{
public:
  ATestHeaderBase () : Header (), m_error (false) {}
  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void)
  {
    static TypeId tid = TypeId ("ATestHeaderBase")
      .SetParent<Header> ()
      .SetGroupName ("Network")
      .HideFromDocumentation ()
      // No AddConstructor because this is an abstract class.
      ;
    return tid;
  }
  bool m_error;
};

template <int N>
class ATestHeader : public ATestHeaderBase
{
public:
  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void) {
    std::ostringstream oss;
    oss << "anon::ATestHeader<" << N << ">";
    static TypeId tid = TypeId (oss.str ().c_str ())
      .SetParent<ATestHeaderBase> ()
      .SetGroupName ("Network")
      .HideFromDocumentation ()
      .AddConstructor<ATestHeader<N> > ()
    ;
    return tid;
  }
  virtual TypeId GetInstanceTypeId (void) const {
    return GetTypeId ();
  }
  virtual uint32_t GetSerializedSize (void) const {
    return N;
  }
  virtual void Serialize (Buffer::Iterator iter) const {
    for (uint32_t i = 0; i < N; ++i)
      {
        iter.WriteU8 (N);
      }
  }
  virtual uint32_t Deserialize (Buffer::Iterator iter) {
    for (uint32_t i = 0; i < N; ++i)
      {
        uint8_t v = iter.ReadU8 ();
        if (v != N)
          {
            m_error = true;
          }
      }
    return N;
  }
  virtual void Print (std::ostream &os) const {
  }
  ATestHeader ()
    : ATestHeaderBase () {}

};

class ATestTrailerBase : public Trailer
{
public:
  ATestTrailerBase () : Trailer (), m_error (false) {}
  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void)
  {
    static TypeId tid = TypeId ("ATestTrailerBase")
      .SetParent<Trailer> ()
      .SetGroupName ("Network")
      .HideFromDocumentation ()
    // No AddConstructor because this is an abstract class.
      ;
    return tid;
  }
  bool m_error;
};

template <int N>
class ATestTrailer : public ATestTrailerBase
{
public:
  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void) {
    std::ostringstream oss;
    oss << "anon::ATestTrailer<" << N << ">";
    static TypeId tid = TypeId (oss.str ().c_str ())
      .SetParent<ATestTrailerBase> ()
      .SetGroupName ("Network")
      .HideFromDocumentation ()
      .AddConstructor<ATestTrailer<N> > ()
    ;
    return tid;
  }
  virtual TypeId GetInstanceTypeId (void) const {
    return GetTypeId ();
  }
  virtual uint32_t GetSerializedSize (void) const {
    return N;
  }
  virtual void Serialize (Buffer::Iterator iter) const {
    iter.Prev (N);
    for (uint32_t i = 0; i < N; ++i)
      {
        iter.WriteU8 (N);
      }
  }
  virtual uint32_t Deserialize (Buffer::Iterator iter) {
    iter.Prev (N);
    for (uint32_t i = 0; i < N; ++i)
      {
        uint8_t v = iter.ReadU8 ();
        if (v != N)
          {
            m_error = true;
          }
      }
    return N;
  }
  virtual void Print (std::ostream &os) const {
  }
  ATestTrailer ()
    : ATestTrailerBase () {}

};


struct Expected
{
  Expected (uint32_t n_, uint32_t start_, uint32_t end_)
    : n (n_), start (start_), end (end_) {}

  uint32_t n;
  uint32_t start;
  uint32_t end;
};

}

// tag name, start, end
#define E(a,b,c) a,b,c

#define CHECK(p, n, ...)                                \
  DoCheck (p, __FILE__, __LINE__, n, __VA_ARGS__)

class PacketTest : public TestCase
{
public:
  PacketTest ();
  virtual void DoRun (void);
private:
  void DoCheck (Ptr<const Packet> p, const char *file, int line, uint32_t n, ...);
};


PacketTest::PacketTest ()
  : TestCase ("Packet") {
}

void
PacketTest::DoCheck (Ptr<const Packet> p, const char *file, int line, uint32_t n, ...)
{
  std::vector<struct Expected> expected;
  va_list ap;
  va_start (ap, n);
  for (uint32_t k = 0; k < n; ++k)
    {
      uint32_t N = va_arg (ap, uint32_t);
      uint32_t start = va_arg (ap, uint32_t);
      uint32_t end = va_arg (ap, uint32_t);
      expected.push_back (Expected (N, start, end));
    }
  va_end (ap);

  ByteTagIterator i = p->GetByteTagIterator ();
  uint32_t j = 0;
  while (i.HasNext () && j < expected.size ())
    {
      ByteTagIterator::Item item = i.Next ();
      struct Expected e = expected[j];
      std::ostringstream oss;
      oss << "anon::ATestTag<" << e.n << ">";
      NS_TEST_EXPECT_MSG_EQ_INTERNAL (item.GetTypeId ().GetName (), oss.str (), "trivial", file, line);
      NS_TEST_EXPECT_MSG_EQ_INTERNAL (item.GetStart (), e.start, "trivial", file, line);
      NS_TEST_EXPECT_MSG_EQ_INTERNAL (item.GetEnd (), e.end, "trivial", file, line);
      ATestTagBase *tag = dynamic_cast<ATestTagBase *> (item.GetTypeId ().GetConstructor () ());
      NS_TEST_EXPECT_MSG_NE (tag, 0, "trivial");
      item.GetTag (*tag);
      NS_TEST_EXPECT_MSG_EQ (tag->m_error, false, "trivial");
      delete tag;
      j++;
    }
  NS_TEST_EXPECT_MSG_EQ (i.HasNext (), false, "Nothing left");
  NS_TEST_EXPECT_MSG_EQ (j, expected.size (), "Size match");
}

void
PacketTest::DoRun (void)
{
  Ptr<Packet> pkt1 = Create<Packet> (reinterpret_cast<const uint8_t*> ("hello"), 5);
  Ptr<Packet> pkt2 = Create<Packet> (reinterpret_cast<const uint8_t*> (" world"), 6);
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddAtEnd (pkt1);
  packet->AddAtEnd (pkt2);

  NS_TEST_EXPECT_MSG_EQ (packet->GetSize (), 11, "trivial");

  uint8_t *buf = new uint8_t[packet->GetSize ()];
  packet->CopyData (buf, packet->GetSize ());

  std::string msg = std::string (reinterpret_cast<const char *>(buf),
                                 packet->GetSize ());
  delete [] buf;

  NS_TEST_EXPECT_MSG_EQ (msg, "hello world", "trivial");


  Ptr<const Packet> p = Create<Packet> (1000);

  p->AddByteTag (ATestTag<1> ());
  CHECK (p, 1, E (1, 0, 1000));
  Ptr<const Packet> copy = p->Copy ();
  CHECK (copy, 1, E (1, 0, 1000));

  p->AddByteTag (ATestTag<2> ());
  CHECK (p, 2, E (1, 0, 1000), E (2, 0, 1000));
  CHECK (copy, 1, E (1, 0, 1000));

  {
    Packet c0 = *copy;
    Packet c1 = *copy;
    c0 = c1;
    CHECK (&c0, 1, E (1, 0, 1000));
    CHECK (&c1, 1, E (1, 0, 1000));
    CHECK (copy, 1, E (1, 0, 1000));
    c0.AddByteTag (ATestTag<10> ());
    CHECK (&c0, 2, E (1, 0, 1000), E (10, 0, 1000));
    CHECK (&c1, 1, E (1, 0, 1000));
    CHECK (copy, 1, E (1, 0, 1000));
  }

  Ptr<Packet> frag0 = p->CreateFragment (0, 10);
  Ptr<Packet> frag1 = p->CreateFragment (10, 90);
  Ptr<const Packet> frag2 = p->CreateFragment (100, 900);
  frag0->AddByteTag (ATestTag<3> ());
  CHECK (frag0, 3, E (1, 0, 10), E (2, 0, 10), E (3, 0, 10));
  frag1->AddByteTag (ATestTag<4> ());
  CHECK (frag1, 3, E (1, 0, 90), E (2, 0, 90), E (4, 0, 90));
  frag2->AddByteTag (ATestTag<5> ());
  CHECK (frag2, 3, E (1, 0, 900), E (2, 0, 900), E (5, 0, 900));

  frag1->AddAtEnd (frag2);
  CHECK (frag1, 6, E (1, 0, 90), E (2, 0, 90), E (4, 0, 90), E (1, 90, 990), E (2, 90, 990), E (5, 90, 990));

  CHECK (frag0, 3, E (1, 0, 10), E (2, 0, 10), E (3, 0, 10));
  frag0->AddAtEnd (frag1);
  CHECK (frag0, 9, 
         E (1, 0, 10), E (2, 0, 10), E (3, 0, 10),
         E (1, 10, 100), E (2, 10, 100), E (4, 10, 100),
         E (1, 100, 1000), E (2, 100, 1000), E (5, 100, 1000));


  // force caching a buffer of the right size.
  frag0 = Create<Packet> (1000);
  frag0->AddHeader (ATestHeader<10> ());
  frag0 = 0;

  p = Create<Packet> (1000);
  p->AddByteTag (ATestTag<20> ());
  CHECK (p, 1, E (20, 0, 1000));
  frag0 = p->CreateFragment (10, 90);
  CHECK (p, 1, E (20, 0, 1000));
  CHECK (frag0, 1, E (20, 0, 90));
  p = 0;
  frag0->AddHeader (ATestHeader<10> ());
  CHECK (frag0, 1, E (20, 10, 100));

  {
    Ptr<Packet> tmp = Create<Packet> (100);
    tmp->AddByteTag (ATestTag<20> ());
    CHECK (tmp, 1, E (20, 0, 100));
    tmp->AddHeader (ATestHeader<10> ());
    CHECK (tmp, 1, E (20, 10, 110));
    ATestHeader<10> h;
    tmp->RemoveHeader (h);
    CHECK (tmp, 1, E (20, 0, 100));
    tmp->AddHeader (ATestHeader<10> ());
    CHECK (tmp, 1, E (20, 10, 110));

    tmp = Create<Packet> (100);
    tmp->AddByteTag (ATestTag<20> ());
    CHECK (tmp, 1, E (20, 0, 100));
    tmp->AddTrailer (ATestTrailer<10> ());
    CHECK (tmp, 1, E (20, 0, 100));
    ATestTrailer<10> t;
    tmp->RemoveTrailer (t);
    CHECK (tmp, 1, E (20, 0, 100));
    tmp->AddTrailer (ATestTrailer<10> ());
    CHECK (tmp, 1, E (20, 0, 100));

  }

  {
    Ptr<Packet> tmp = Create<Packet> (0);
    tmp->AddHeader (ATestHeader<156> ());
    tmp->AddByteTag (ATestTag<20> ());
    CHECK (tmp, 1, E (20, 0, 156));
    tmp->RemoveAtStart (120);
    CHECK (tmp, 1, E (20, 0, 36));
    Ptr<Packet> a = Create<Packet> (0);
    a->AddAtEnd (tmp);
    CHECK (a, 1, E (20, 0, 36));
  }

  {
    Ptr<Packet> tmp = Create<Packet> (0);
    tmp->AddByteTag (ATestTag<20> ());
    CHECK (tmp, 0, E (20, 0, 0));
  }
  {
    Ptr<Packet> tmp = Create<Packet> (1000);
    tmp->AddByteTag (ATestTag<20> ());
    CHECK (tmp, 1, E (20, 0, 1000));
    tmp->RemoveAtStart (1000);
    CHECK (tmp, 0,  E (0,0,0));
    Ptr<Packet> a = Create<Packet> (10);
    a->AddByteTag (ATestTag<10> ());
    CHECK (a, 1, E (10, 0, 10));
    tmp->AddAtEnd (a);
    CHECK (tmp, 1, E (10, 0, 10));
  }

  {
    Packet p;
    ATestTag<10> a;
    p.AddPacketTag (a);
    NS_TEST_EXPECT_MSG_EQ (p.PeekPacketTag (a), true, "trivial");
    ATestTag<11> b;
    p.AddPacketTag (b);
    NS_TEST_EXPECT_MSG_EQ (p.PeekPacketTag (b), true, "trivial");
    NS_TEST_EXPECT_MSG_EQ (p.PeekPacketTag (a), true, "trivial");
    Packet copy = p;
    NS_TEST_EXPECT_MSG_EQ (copy.PeekPacketTag (b), true, "trivial");
    NS_TEST_EXPECT_MSG_EQ (copy.PeekPacketTag (a), true, "trivial");
    ATestTag<12> c;
    NS_TEST_EXPECT_MSG_EQ (copy.PeekPacketTag (c), false, "trivial");
    copy.AddPacketTag (c);
    NS_TEST_EXPECT_MSG_EQ (copy.PeekPacketTag (c), true, "trivial");
    NS_TEST_EXPECT_MSG_EQ (copy.PeekPacketTag (b), true, "trivial");
    NS_TEST_EXPECT_MSG_EQ (copy.PeekPacketTag (a), true, "trivial");
    NS_TEST_EXPECT_MSG_EQ (p.PeekPacketTag (c), false, "trivial");
    copy.RemovePacketTag (b);
    NS_TEST_EXPECT_MSG_EQ (copy.PeekPacketTag (b), false, "trivial");
    NS_TEST_EXPECT_MSG_EQ (p.PeekPacketTag (b), true, "trivial");
    p.RemovePacketTag (a);
    NS_TEST_EXPECT_MSG_EQ (p.PeekPacketTag (a), false, "trivial");
    NS_TEST_EXPECT_MSG_EQ (copy.PeekPacketTag (a), true, "trivial");
    NS_TEST_EXPECT_MSG_EQ (p.PeekPacketTag (c), false, "trivial");
    NS_TEST_EXPECT_MSG_EQ (copy.PeekPacketTag (c), true, "trivial");
    p.RemoveAllPacketTags ();
    NS_TEST_EXPECT_MSG_EQ (p.PeekPacketTag (b), false, "trivial");
  }

  {
    /// \internal
    /// See \bugid{572}
    Ptr<Packet> tmp = Create<Packet> (1000);
    tmp->AddByteTag (ATestTag<20> ());
    CHECK (tmp, 1, E (20, 0, 1000));
    tmp->AddHeader (ATestHeader<2> ());
    CHECK (tmp, 1, E (20, 2, 1002));
    tmp->RemoveAtStart (1);
    CHECK (tmp, 1, E (20, 1, 1001));
#if 0
    tmp->PeekData ();
    CHECK (tmp, 1, E (20, 1, 1001));
#endif
  }

  /* Test reducing tagged packet size and increasing it back. */
  {
    Ptr<Packet> tmp = Create<Packet> (0);
    tmp->AddHeader (ATestHeader<100> ());
    tmp->AddByteTag (ATestTag<25> ());
    CHECK (tmp, 1, E (25, 0, 100));
    tmp->RemoveAtStart (50);
    CHECK (tmp, 1, E (25, 0, 50));
    tmp->AddHeader (ATestHeader<50> ());
    CHECK (tmp, 1, E (25, 50, 100));
  }

  /* Similar test case, but using trailer instead of header. */
  {
    Ptr<Packet> tmp = Create<Packet> (0);
    tmp->AddTrailer (ATestTrailer<100> ());
    tmp->AddByteTag (ATestTag<25> ());
    CHECK (tmp, 1, E (25, 0, 100));
    tmp->RemoveAtEnd (50);
    CHECK (tmp, 1, E (25, 0, 50));
    tmp->AddTrailer (ATestTrailer<50> ());
    CHECK (tmp, 1, E (25, 0, 50));
  }

  /* Test reducing tagged packet size and increasing it by half. */
  {
    Ptr<Packet> tmp = Create<Packet> (0);
    tmp->AddHeader (ATestHeader<100> ());
    tmp->AddByteTag (ATestTag<25> ());
    CHECK (tmp, 1, E (25, 0, 100));
    tmp->RemoveAtStart (50);
    CHECK (tmp, 1, E (25, 0, 50));
    tmp->AddHeader (ATestHeader<25> ());
    CHECK (tmp, 1, E (25, 25, 75));
  }

  /* Similar test case, but using trailer instead of header. */
  {
    Ptr<Packet> tmp = Create<Packet> (0);
    tmp->AddTrailer (ATestTrailer<100> ());
    tmp->AddByteTag (ATestTag<25> ());
    CHECK (tmp, 1, E (25, 0, 100));
    tmp->RemoveAtEnd (50);
    CHECK (tmp, 1, E (25, 0, 50));
    tmp->AddTrailer (ATestTrailer<25> ());
    CHECK (tmp, 1, E (25, 0, 50));
  }

  /* Test AddPaddingAtEnd. */
  {
    Ptr<Packet> tmp = Create<Packet> (0);
    tmp->AddTrailer (ATestTrailer<100> ());
    tmp->AddByteTag (ATestTag<25> ());
    CHECK (tmp, 1, E (25, 0, 100));
    tmp->RemoveAtEnd (50);
    CHECK (tmp, 1, E (25, 0, 50));
    tmp->AddPaddingAtEnd (50);
    CHECK (tmp, 1, E (25, 0, 50));
  }
}
//--------------------------------------
class PacketTagListTest : public TestCase
{
public:
  PacketTagListTest ();
  virtual ~PacketTagListTest ();
private:
  void DoRun (void);
  void CheckRef (const PacketTagList & ref,
                 ATestTagBase & t,
                 const char * msg,
                 bool miss = false);
  void CheckRefList (const PacketTagList & ref,
                     const char * msg,
                     int miss = 0);
  int RemoveTime (const PacketTagList & ref,
                  ATestTagBase & t,
                  const char * msg = 0);
  int AddRemoveTime (const bool verbose = false);
};

PacketTagListTest::PacketTagListTest ()
  : TestCase ("PacketTagListTest: ")
{
}

PacketTagListTest::~PacketTagListTest ()
{
}

void
PacketTagListTest::CheckRef (const PacketTagList & ref,
                             ATestTagBase & t,
                             const char * msg,
                             bool miss)
{
  int expect = t.GetData ();  // the value we should find
  bool found = ref.Peek (t); // rewrites t with actual value
  NS_TEST_EXPECT_MSG_EQ (found, !miss,
                         msg << ": ref contains "
                         << t.GetTypeId ().GetName ());
  if (found) {
    NS_TEST_EXPECT_MSG_EQ (t.GetData (), expect,
                           msg << ": ref " << t.GetTypeId ().GetName ()
                           << " = " << expect);
  }
}

  // A set of tags with data value 1, to check COW
#define MAKE_TEST_TAGS \
  ATestTag<1> t1 (1); \
  ATestTag<2> t2 (1); \
  ATestTag<3> t3 (1); \
  ATestTag<4> t4 (1); \
  ATestTag<5> t5 (1); \
  ATestTag<6> t6 (1); \
  ATestTag<7> t7 (1); \
  const int tagLast = 7;  /* length of ref PacketTagList */ \
  NS_UNUSED (tagLast) /* silence warnings */
 
  
  
void
PacketTagListTest::CheckRefList (const PacketTagList & ptl,
                                 const char * msg,
                                 int miss /* = 0 */)
{
  MAKE_TEST_TAGS ;
  CheckRef (ptl, t1, msg, miss == 1);
  CheckRef (ptl, t2, msg, miss == 2);
  CheckRef (ptl, t3, msg, miss == 3);
  CheckRef (ptl, t4, msg, miss == 4);
  CheckRef (ptl, t5, msg, miss == 5);
  CheckRef (ptl, t6, msg, miss == 6);
  CheckRef (ptl, t7, msg, miss == 7);
}
  
int
PacketTagListTest::RemoveTime (const PacketTagList & ref,
                               ATestTagBase & t,
                               const char * msg /* = 0 */)
{
  const int reps = 10000;
  std::vector< PacketTagList > ptv(reps, ref);
  int start = clock ();
  for (int i = 0; i < reps; ++i) {
    ptv[i].Remove (t);
  }
  int stop = clock ();
  int delta = stop - start;
  if (msg) {
    std::cout << GetName () << "remove time: " << msg << ": " << std::setw (8)
              << delta      << " ticks to remove "
              << reps       << " times"
            << std::endl;
  }
  return delta;
}

int
PacketTagListTest::AddRemoveTime (const bool verbose /* = false */)
{
  const int reps = 100000;
  PacketTagList ptl;
  ATestTag <2> t(2);
  int start = clock ();
  for (int i = 0; i < reps; ++i) {
    ptl.Add (t);
    ptl.Remove (t);
  }
  int stop = clock ();
  int delta = stop - start;
  if (verbose) {
    std::cout << GetName () << "add/remove time: " << std::setw (8)
              << delta      << " ticks to add+remove "
              << reps       << " times"
            << std::endl;
  }
  return delta;
}

void
PacketTagListTest::DoRun (void)
{
  std::cout << GetName () << "begin" << std::endl;

  MAKE_TEST_TAGS ;
  
  PacketTagList ref;  // empty list
  ref.Add (t1);       // last
  ref.Add (t2);       // post merge
  ref.Add (t3);       // merge successor
  ref.Add (t4);       // merge
  ref.Add (t5);       // merge precursor
  ref.Add (t6);       // pre-merge
  ref.Add (t7);       // first
  
  { // Peek
    std::cout << GetName () << "check Peek (missing tag) returns false"
              << std::endl;;
    ATestTag<10> t10;
    NS_TEST_EXPECT_MSG_EQ (ref.Peek (t10), false, "missing tag");
  }

  { // Copy ctor, assignment
    std::cout << GetName () << "check copy and assignment" << std::endl;
    { PacketTagList ptl (ref);
      CheckRefList (ref, "copy ctor orig");
      CheckRefList (ptl, "copy ctor copy");
    }
    { PacketTagList ptl = ref;
      CheckRefList (ref, "assignment orig");
      CheckRefList (ptl, "assignment copy");
    }
  }
  
  { // Removal
#   define RemoveCheck(n)                               \
    { PacketTagList p ## n = ref;			\
      p ## n .Remove ( t ## n );			\
      CheckRefList (ref,     "remove " #n " orig");	\
      CheckRefList (p ## n, "remove " #n " copy", n);   \
    }
    
    { // Remove single tags from list
      std::cout << GetName () << "check removal of each tag" << std::endl;
      RemoveCheck (1);
      RemoveCheck (2);
      RemoveCheck (3);
      RemoveCheck (4);
      RemoveCheck (5);
      RemoveCheck (6);
      RemoveCheck (7);
    }
    
    { // Remove in the presence of a merge
      std::cout << GetName () << "check removal doesn't disturb merge "
                << std::endl;
      PacketTagList ptl = ref;
      ptl.Remove (t7);
      ptl.Remove (t6);
      ptl.Remove (t5);
      
      PacketTagList mrg = ptl;  // merged list
      ATestTag<8> m5 (1);
      mrg.Add (m5);             // ptl and mrg differ
      ptl.Add (t5);
      ptl.Add (t6);
      ptl.Add (t7);

      CheckRefList (ref, "post merge, orig");
      CheckRefList (ptl, "post merge, long chain");
      const char * msg = "post merge, short chain";
      CheckRef (mrg, t1, msg, false);
      CheckRef (mrg, t2, msg, false);
      CheckRef (mrg, t3, msg, false);
      CheckRef (mrg, t4, msg, false);
      CheckRef (mrg, m5, msg, false);
    }
#   undef RemoveCheck
  }  // Removal

  { // Replace

    std::cout << GetName () << "check replacing each tag" << std::endl;
      
#   define ReplaceCheck(n)					\
    t ## n .m_data = 2;						\
    { PacketTagList p ## n = ref;				\
      p ## n .Replace ( t ## n );				\
      CheckRefList (ref,     "replace " #n " orig");		\
      CheckRef     (p ## n, t ## n, "replace " #n " copy");	\
    }
    
    ReplaceCheck (1);
    ReplaceCheck (2);
    ReplaceCheck (3);
    ReplaceCheck (4);
    ReplaceCheck (5);
    ReplaceCheck (6);
    ReplaceCheck (7);
  }
  
  { // Timing
    std::cout << GetName () << "add+remove timing" << std::endl;
    int flm = std::numeric_limits<int>::max ();
    const int nIterations = 100;
    for (int i = 0; i < nIterations; ++i) {
      int now = AddRemoveTime ();
      if (now < flm) flm = now;
    }
    std::cout << GetName () << "min add+remove time: "
              << std::setw (8) << flm        << " ticks"
              << std::endl;
    
    std::cout << GetName () << "remove timing" << std::endl;
    // tags numbered from 1, so add one for (unused) entry at 0
    std::vector <int> rmn (tagLast + 1, std::numeric_limits<int>::max ());
    for (int i = 0; i < nIterations; ++i) {
      for (int j = 1; j <= tagLast; ++j) {
	int now = 0;
	switch (j) {
        case 7:  now = RemoveTime (ref, t7);  break;
        case 6:  now = RemoveTime (ref, t6);  break;
        case 5:  now = RemoveTime (ref, t5);  break;
        case 4:  now = RemoveTime (ref, t4);  break;
        case 3:  now = RemoveTime (ref, t3);  break;
        case 2:  now = RemoveTime (ref, t2);  break;
        case 1:  now = RemoveTime (ref, t1);  break;
	}  // switch
	
	if (now < rmn[j]) rmn[j] = now;
      } // for tag j
    } // for iteration i
    for (int j = tagLast; j > 0; --j) {
      std::cout << GetName () << "min remove time: t"
		<< j          << ": "
		<< std::setw (8) << rmn[j]     << " ticks"
                << std::endl;
    }
  }  // Timing
    
}

//-----------------------------------------------------------------------------
class PacketTestSuite : public TestSuite
{
public:
  PacketTestSuite ();
};

PacketTestSuite::PacketTestSuite ()
  : TestSuite ("packet", UNIT)
{
  AddTestCase (new PacketTest, TestCase::QUICK);
  AddTestCase (new PacketTagListTest, TestCase::QUICK);
}

static PacketTestSuite g_packetTestSuite;
