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
#include "ns3/test.h"
#include <string>
#include <stdarg.h>

namespace ns3 {

//-----------------------------------------------------------------------------
// Unit tests
//-----------------------------------------------------------------------------
namespace {

class ATestTagBase : public Tag
{
public:
  ATestTagBase () : m_error (false) {}
  bool m_error;
};

template <int N>
class ATestTag : public ATestTagBase
{
public:
  static TypeId GetTypeId (void) {
    std::ostringstream oss;
    oss << "anon::ATestTag<" << N << ">";
    static TypeId tid = TypeId (oss.str ().c_str ())
      .SetParent<Tag> ()
      .AddConstructor<ATestTag<N> > ()
      .HideFromDocumentation ()
    ;
    return tid;
  }
  virtual TypeId GetInstanceTypeId (void) const {
    return GetTypeId ();
  }
  virtual uint32_t GetSerializedSize (void) const {
    return N;
  }
  virtual void Serialize (TagBuffer buf) const {
    for (uint32_t i = 0; i < N; ++i)
      {
        buf.WriteU8 (N);
      }
  }
  virtual void Deserialize (TagBuffer buf) {
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
    os << N;
  }
  ATestTag ()
    : ATestTagBase () {}
};

class ATestHeaderBase : public Header
{
public:
  ATestHeaderBase () : Header (), m_error (false) {}
  bool m_error;
};

template <int N>
class ATestHeader : public ATestHeaderBase
{
public:
  static TypeId GetTypeId (void) {
    std::ostringstream oss;
    oss << "anon::ATestHeader<" << N << ">";
    static TypeId tid = TypeId (oss.str ().c_str ())
      .SetParent<Header> ()
      .AddConstructor<ATestHeader<N> > ()
      .HideFromDocumentation ()
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
  bool m_error;
};

template <int N>
class ATestTrailer : public ATestTrailerBase
{
public:
  static TypeId GetTypeId (void) {
    std::ostringstream oss;
    oss << "anon::ATestTrailer<" << N << ">";
    static TypeId tid = TypeId (oss.str ().c_str ())
      .SetParent<Header> ()
      .AddConstructor<ATestTrailer<N> > ()
      .HideFromDocumentation ()
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
    // bug 572
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
  AddTestCase (new PacketTest);
}

static PacketTestSuite g_packetTestSuite;


} // namespace ns3
