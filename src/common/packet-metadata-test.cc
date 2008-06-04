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
#ifdef RUN_SELF_TESTS

#include <stdarg.h>
#include <iostream>
#include <sstream>
#include "ns3/test.h"
#include "header.h"
#include "trailer.h"
#include "packet.h"
#include "packet-metadata.h"

using namespace ns3;

namespace {

class HistoryHeaderBase : public Header
{
public:
  static TypeId GetTypeId (void);
  HistoryHeaderBase ();
  bool IsOk (void) const;
protected:
  void ReportError (void);
private:
  bool m_ok;
};

TypeId 
HistoryHeaderBase::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::HistoryHeaderBase")
    .SetParent<Header> ()
    ;
  return tid;
}

HistoryHeaderBase::HistoryHeaderBase ()
  : m_ok (true)
{}

bool 
HistoryHeaderBase::IsOk (void) const
{
  return m_ok;
}
void 
HistoryHeaderBase::ReportError (void)
{
  m_ok = false;
}


template <int N>
class HistoryHeader : public HistoryHeaderBase
{
public:
  HistoryHeader ();
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
};

template <int N>
HistoryHeader<N>::HistoryHeader ()
  : HistoryHeaderBase ()
{}

template <int N>
TypeId
HistoryHeader<N>::GetTypeId (void)
{
  std::ostringstream oss;
  oss << "ns3::HistoryHeader<"<<N<<">";
  static TypeId tid = TypeId (oss.str ().c_str ())
    .SetParent<HistoryHeaderBase> ()
    .AddConstructor<HistoryHeader<N> > ()
    ;
  return tid;
}

template <int N>
TypeId 
HistoryHeader<N>::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
template <int N>
void 
HistoryHeader<N>::Print (std::ostream &os) const
{
  NS_ASSERT (false);
}
template <int N>
uint32_t 
HistoryHeader<N>::GetSerializedSize (void) const
{
  return N;
}
template <int N>
void 
HistoryHeader<N>::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (N, N);
}
template <int N>
uint32_t
HistoryHeader<N>::Deserialize (Buffer::Iterator start)
{
  for (int i = 0; i < N; i++)
    {
      if (start.ReadU8 () != N)
        {
          ReportError ();
        }
    }
  return N;
}

class HistoryTrailerBase : public Trailer
{
public:
  static TypeId GetTypeId (void);
  HistoryTrailerBase ();
  bool IsOk (void) const;
protected:
  void ReportError (void);
private:
  bool m_ok;
};

TypeId 
HistoryTrailerBase::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::HistoryTrailerBase")
    .SetParent<Trailer> ()
    ;
  return tid;
}
HistoryTrailerBase::HistoryTrailerBase ()
  : m_ok (true)
{}
bool 
HistoryTrailerBase::IsOk (void) const
{
  return m_ok;
}
void 
HistoryTrailerBase::ReportError (void)
{
  m_ok = false;
}


template <int N>
class HistoryTrailer : public HistoryTrailerBase
{
public:
  HistoryTrailer ();
  bool IsOk (void) const;

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
private:
  bool m_ok;
};

template <int N>
HistoryTrailer<N>::HistoryTrailer ()
{}

template <int N>
TypeId
HistoryTrailer<N>::GetTypeId (void)
{
  std::ostringstream oss;
  oss << "ns3::HistoryTrailer<"<<N<<">";
  static TypeId tid = TypeId (oss.str ().c_str ())
    .SetParent<HistoryTrailerBase> ()
    .AddConstructor<HistoryTrailer<N> > ()
    ;
  return tid;
}

template <int N>
TypeId 
HistoryTrailer<N>::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
template <int N>
void 
HistoryTrailer<N>::Print (std::ostream &os) const
{
  NS_ASSERT (false);
}
template <int N>
uint32_t 
HistoryTrailer<N>::GetSerializedSize (void) const
{
  return N;
}
template <int N>
void 
HistoryTrailer<N>::Serialize (Buffer::Iterator start) const
{
  start.Prev (N);
  start.WriteU8 (N, N);
}
template <int N>
uint32_t
HistoryTrailer<N>::Deserialize (Buffer::Iterator start)
{
  start.Prev (N);
  for (int i = 0; i < N; i++)
    {
      if (start.ReadU8 () != N)
        {
          ReportError ();
        }
    }
  return N;
}

}

namespace ns3 {



class PacketMetadataTest : public Test {
public:
  PacketMetadataTest ();
  virtual ~PacketMetadataTest ();
  bool CheckHistory (Ptr<Packet> p, const char *file, int line, uint32_t n, ...);
  virtual bool RunTests (void);
private:
  Ptr<Packet> DoAddHeader (Ptr<Packet> p);
};

PacketMetadataTest::PacketMetadataTest ()
  : Test ("PacketMetadata")
{}

PacketMetadataTest::~PacketMetadataTest ()
{}

bool 
PacketMetadataTest::CheckHistory (Ptr<Packet> p, const char *file, int line, uint32_t n, ...)
{
  std::list<int> expected;
  va_list ap;
  va_start (ap, n);
  for (uint32_t j = 0; j < n; j++)
    {
      int v = va_arg (ap, int);
      expected.push_back (v);
    }
  va_end (ap);

  PacketMetadata::ItemIterator k = p->BeginItem ();
  std::list<int> got;
  while (k.HasNext ())
    {
      struct PacketMetadata::Item item = k.Next ();
      if (item.isFragment || item.type == PacketMetadata::Item::PAYLOAD)
        {
          got.push_back (item.currentSize);
          continue;
        }
      if (item.type == PacketMetadata::Item::HEADER)
        {
          Callback<ObjectBase *> constructor = item.tid.GetConstructor ();
          HistoryHeaderBase *header = dynamic_cast<HistoryHeaderBase *> (constructor ());
          if (header == 0)
            {
              goto error;
            }
          header->Deserialize (item.current);
          if (!header->IsOk ())
            {
              delete header;
              goto error;
            }
          delete header;
        }
      else if (item.type == PacketMetadata::Item::TRAILER)
        {
          Callback<ObjectBase *> constructor = item.tid.GetConstructor ();
          HistoryTrailerBase *trailer = dynamic_cast<HistoryTrailerBase *> (constructor ());
          if (trailer == 0)
            {
              goto error;
            }
          trailer->Deserialize (item.current);
          if (!trailer->IsOk ())
            {
              delete trailer;
              goto error;
            }
          delete trailer;
        }
      got.push_back (item.currentSize);
    }

  for (std::list<int>::iterator i = got.begin (),
         j = expected.begin (); 
       i != got.end (); i++, j++)
    {
      NS_ASSERT (j != expected.end ());
      if (*j != *i)
        {
          goto error;
        }
    }
  return true;
 error:
  Failure () << "PacketMetadata error. file="<< file 
            << ", line=" << line << ", got:\"";
  for (std::list<int>::iterator i = got.begin (); 
       i != got.end (); i++)
    {
      Failure () << *i << ", ";
    }
  Failure () << "\", expected: \"";
  for (std::list<int>::iterator j = expected.begin ();
       j != expected.end (); j++)
    {
      Failure () << *j << ", ";
    }
  Failure () << "\"" << std::endl;
  return false;
}

#define ADD_HEADER(p, n)                        \
  {                                             \
    HistoryHeader<n> header;                    \
    p->AddHeader (header);                      \
  }
#define ADD_TRAILER(p, n)                       \
  {                                             \
    HistoryTrailer<n> trailer;                  \
    p->AddTrailer (trailer);                    \
  }
#define REM_HEADER(p, n)                        \
  {                                             \
    HistoryHeader<n> header;                    \
    p->RemoveHeader (header);                   \
  }
#define REM_TRAILER(p, n)                       \
  {                                             \
    HistoryTrailer<n> trailer;                  \
    p->RemoveTrailer (trailer);                 \
  }
#define CHECK_HISTORY(p, ...)                   \
  {                                             \
    if (!CheckHistory (p, __FILE__,             \
                      __LINE__, __VA_ARGS__))   \
      {                                         \
        result = false;                         \
      }                                         \
    Buffer buffer;                              \
    buffer = p->Serialize ();                   \
    Ptr<Packet> otherPacket = Create<Packet> ();\
    otherPacket->Deserialize  (buffer);         \
    if (!CheckHistory (otherPacket, __FILE__,   \
                      __LINE__, __VA_ARGS__))   \
      {                                         \
        result = false;                         \
      }                                         \
  }


Ptr<Packet>
PacketMetadataTest::DoAddHeader (Ptr<Packet> p)
{
  ADD_HEADER (p, 10);
  return p;
}

bool
PacketMetadataTest::RunTests (void)
{
  bool result = true;

  PacketMetadata::Enable ();

  Ptr<Packet> p = Create<Packet> (0);
  Ptr<Packet> p1 = Create<Packet> (0);

  p = Create<Packet> (10);
  ADD_TRAILER (p, 100);
  CHECK_HISTORY (p, 2, 10, 100);

  p = Create<Packet> (10);
  ADD_HEADER (p, 1);
  ADD_HEADER (p, 2);
  ADD_HEADER (p, 3);
  CHECK_HISTORY (p, 4, 
                 3, 2, 1, 10);
  ADD_HEADER (p, 5);
  CHECK_HISTORY (p, 5, 
                 5, 3, 2, 1, 10);
  ADD_HEADER (p, 6);
  CHECK_HISTORY (p, 6, 
                 6, 5, 3, 2, 1, 10);

  p = Create<Packet> (10);
  ADD_HEADER (p, 1);
  ADD_HEADER (p, 2);
  ADD_HEADER (p, 3);
  REM_HEADER (p, 3);
  CHECK_HISTORY (p, 3, 
                 2, 1, 10);

  p = Create<Packet> (10);
  ADD_HEADER (p, 1);
  ADD_HEADER (p, 2);
  ADD_HEADER (p, 3);
  REM_HEADER (p, 3);
  REM_HEADER (p, 2);
  CHECK_HISTORY (p, 2, 
                 1, 10);

  p = Create<Packet> (10);
  ADD_HEADER (p, 1);
  ADD_HEADER (p, 2);
  ADD_HEADER (p, 3);
  REM_HEADER (p, 3);
  REM_HEADER (p, 2);
  REM_HEADER (p, 1);
  CHECK_HISTORY (p, 1, 10);

  p = Create<Packet> (10);
  ADD_HEADER (p, 1);
  ADD_HEADER (p, 2);
  ADD_HEADER (p, 3);
  p1 = p->Copy ();
  REM_HEADER (p1, 3);
  REM_HEADER (p1, 2);
  REM_HEADER (p1, 1);
  CHECK_HISTORY (p1, 1, 10);
  CHECK_HISTORY (p, 4, 
                 3, 2, 1, 10);
  ADD_HEADER (p1, 1);
  ADD_HEADER (p1, 2);
  CHECK_HISTORY (p1, 3, 
                 2, 1, 10);
  CHECK_HISTORY (p, 4, 
                 3, 2, 1, 10);
  ADD_HEADER (p, 3);
  CHECK_HISTORY (p, 5, 
                 3, 3, 2, 1, 10);
  ADD_TRAILER (p, 4);
  CHECK_HISTORY (p, 6, 
                 3, 3, 2, 1, 10, 4);
  ADD_TRAILER (p, 5);
  CHECK_HISTORY (p, 7, 
                 3, 3, 2, 1, 10, 4, 5);
  REM_HEADER (p, 3);
  CHECK_HISTORY (p, 6, 
                 3, 2, 1, 10, 4, 5);
  REM_TRAILER (p, 5);
  CHECK_HISTORY (p, 5, 
                 3, 2, 1, 10, 4);
  p1 = p->Copy ();
  REM_TRAILER (p, 4);
  CHECK_HISTORY (p, 4, 
                 3, 2, 1, 10);
  CHECK_HISTORY (p1, 5, 
                 3, 2, 1, 10, 4);
  p1->RemoveAtStart (3);
  CHECK_HISTORY (p1, 4, 
                 2, 1, 10, 4);
  p1->RemoveAtStart (1);
  CHECK_HISTORY (p1, 4, 
                 1, 1, 10, 4);
  p1->RemoveAtStart (1);
  CHECK_HISTORY (p1, 3, 
                 1, 10, 4);
  p1->RemoveAtEnd (4);
  CHECK_HISTORY (p1, 2, 
                 1, 10);
  p1->RemoveAtStart (1);
  CHECK_HISTORY (p1, 1, 10);

  p = Create<Packet> (10);
  ADD_HEADER (p, 8);
  ADD_TRAILER (p, 8);
  ADD_TRAILER (p, 8);
  p->RemoveAtStart (8+10+8);
  CHECK_HISTORY (p, 1, 8);

  p = Create<Packet> (10);
  ADD_HEADER (p, 10);
  ADD_HEADER (p, 8);
  ADD_TRAILER (p, 6);
  ADD_TRAILER (p, 7);
  ADD_TRAILER (p, 9);
  p->RemoveAtStart (5);
  p->RemoveAtEnd (12);
  CHECK_HISTORY (p, 5, 3, 10, 10, 6, 4);

  p = Create<Packet> (10);
  ADD_HEADER (p, 10);
  ADD_TRAILER (p, 6);
  p->RemoveAtEnd (18);
  ADD_TRAILER (p, 5);
  ADD_HEADER (p, 3);
  CHECK_HISTORY (p, 3, 3, 8, 5);
  p->RemoveAtStart (12);
  CHECK_HISTORY (p, 1, 4);
  p->RemoveAtEnd (2);
  CHECK_HISTORY (p, 1, 2);
  ADD_HEADER (p, 10);
  CHECK_HISTORY (p, 2, 10, 2);
  p->RemoveAtEnd (5);
  CHECK_HISTORY (p, 1, 7);

  Ptr<Packet> p2 = Create<Packet> (0);
  Ptr<Packet> p3 = Create<Packet> (0);

  p = Create<Packet> (40);
  ADD_HEADER (p, 5);
  ADD_HEADER (p, 8);
  CHECK_HISTORY (p, 3, 8, 5, 40);
  p1 = p->CreateFragment (0, 5);
  p2 = p->CreateFragment (5, 5);
  p3 = p->CreateFragment (10, 43);
  CHECK_HISTORY (p1, 1, 5);
  CHECK_HISTORY (p2, 2, 3, 2);
  CHECK_HISTORY (p3, 2, 3, 40);
  p1->AddAtEnd (p2);
  CHECK_HISTORY (p1, 2, 8, 2);
  CHECK_HISTORY (p2, 2, 3, 2);
  p1->AddAtEnd (p3);
  CHECK_HISTORY (p1, 3, 8, 5, 40);
  CHECK_HISTORY (p2, 2, 3, 2);
  CHECK_HISTORY (p3, 2, 3, 40);
  p1 = p->CreateFragment (0, 5);
  CHECK_HISTORY (p1, 1, 5);

  p3 = Create<Packet> (50);
  ADD_HEADER (p3, 8);
  CHECK_HISTORY (p3, 2, 8, 50);
  CHECK_HISTORY (p1, 1, 5);
  p1->AddAtEnd (p3);
  CHECK_HISTORY (p1, 3, 5, 8, 50);
  ADD_HEADER (p1, 5);
  CHECK_HISTORY (p1, 4, 5, 5, 8, 50);
  ADD_TRAILER (p1, 2);
  CHECK_HISTORY (p1, 5, 5, 5, 8, 50, 2);
  REM_HEADER (p1, 5);
  CHECK_HISTORY (p1, 4, 5, 8, 50, 2);
  p1->RemoveAtEnd (60);
  CHECK_HISTORY (p1, 1, 5);
  p1->AddAtEnd (p2);
  CHECK_HISTORY (p1, 2, 8, 2);
  CHECK_HISTORY (p2, 2, 3, 2);

  p3 = Create<Packet> (40);
  ADD_HEADER (p3, 5);
  ADD_HEADER (p3, 5);
  CHECK_HISTORY (p3, 3, 5, 5, 40);
  p1 = p3->CreateFragment (0, 5);
  p2 = p3->CreateFragment (5, 5);
  CHECK_HISTORY (p1, 1, 5);
  CHECK_HISTORY (p2, 1, 5);
  p1->AddAtEnd (p2);
  CHECK_HISTORY (p1, 2, 5, 5);

  p = Create<Packet> (0);
  CHECK_HISTORY (p, 0);

  p3 = Create<Packet> (0);
  ADD_HEADER (p3, 5);
  ADD_HEADER (p3, 5);
  CHECK_HISTORY (p3, 2, 5, 5);
  p1 = p3->CreateFragment (0, 4);
  p2 = p3->CreateFragment (9, 1);
  CHECK_HISTORY (p1, 1, 4);
  CHECK_HISTORY (p2, 1, 1);
  p1->AddAtEnd (p2);
  CHECK_HISTORY (p1, 2, 4, 1);


  p = Create<Packet> (2000);
  CHECK_HISTORY (p, 1, 2000);
  
  p = Create<Packet> ();
  ADD_TRAILER (p, 10);
  ADD_HEADER (p, 5);
  p1 = p->CreateFragment (0, 8);
  p2 = p->CreateFragment (8, 7);
  p1->AddAtEnd (p2);
  CHECK_HISTORY (p, 2, 5, 10);

  p = Create<Packet> ();
  ADD_TRAILER (p, 10);
  REM_TRAILER (p, 10);
  ADD_TRAILER (p, 10);
  CHECK_HISTORY (p, 1, 10);

  p = Create<Packet> ();
  ADD_HEADER (p, 10);
  REM_HEADER (p, 10);
  ADD_HEADER (p, 10);
  CHECK_HISTORY (p, 1, 10);

  p = Create<Packet> ();
  ADD_HEADER (p, 10);
  p = DoAddHeader (p);
  CHECK_HISTORY (p, 2, 10, 10);

  p = Create<Packet> (10);
  ADD_HEADER (p, 8);
  ADD_TRAILER (p, 8);
  ADD_TRAILER (p, 8);
  p->RemoveAtStart (8+10+8);
  CHECK_HISTORY (p, 1, 8);

  p = Create<Packet> (0);
  ADD_HEADER (p, 8);
  REM_HEADER (p, 8);
  CHECK_HISTORY (p, 0);

  p = Create<Packet> (0);
  ADD_TRAILER (p, 8);
  REM_TRAILER (p, 8);
  CHECK_HISTORY (p, 0);

  p = Create<Packet> (0);
  ADD_HEADER (p, 8);
  p->RemoveAtStart (8);
  CHECK_HISTORY (p, 0);

  p = Create<Packet> (0);
  ADD_HEADER (p, 8);
  ADD_TRAILER (p, 8);
  REM_TRAILER (p, 8);
  REM_HEADER (p, 8);
  CHECK_HISTORY (p, 0);

  p = Create<Packet> (0);
  ADD_HEADER (p, 8);
  ADD_TRAILER (p, 8);
  REM_HEADER (p, 8);
  REM_TRAILER (p, 8);
  CHECK_HISTORY (p, 0);

  p = Create<Packet> (0);
  ADD_HEADER (p, 8);
  ADD_TRAILER (p, 8);
  REM_TRAILER (p, 8);
  p->RemoveAtStart (8);
  CHECK_HISTORY (p, 0);

  p = Create<Packet> (0);
  ADD_HEADER (p, 8);
  ADD_TRAILER (p, 8);
  REM_HEADER (p, 8);
  p->RemoveAtEnd (8);
  CHECK_HISTORY (p, 0);

  p = Create<Packet> (0);
  ADD_HEADER (p, 8);
  ADD_TRAILER (p, 8);
  REM_TRAILER (p, 8);
  p->RemoveAtEnd (8);
  CHECK_HISTORY (p, 0);

  p = Create<Packet> (0);
  ADD_HEADER (p, 8);
  ADD_TRAILER (p, 8);
  REM_HEADER (p, 8);
  p->RemoveAtStart (8);
  CHECK_HISTORY (p, 0);

  p = Create<Packet> (16383);
  p = Create<Packet> (16384);


  // bug 179.
  p = Create<Packet> (40);
  p2 = p->CreateFragment (5, 5);
  p3 = p->CreateFragment (10, 30);
  ADD_HEADER (p2, 8);
  ADD_HEADER (p3, 8);
  REM_HEADER (p2, 8);
  REM_HEADER (p3, 8);
  p2->AddAtEnd (p3);


  p = Create<Packet> (1000);
  ADD_HEADER (p, 10);
  ADD_TRAILER (p, 5);
  p1 = p->Copy ();
  ADD_HEADER (p1, 20);
  REM_HEADER (p1, 20);
  REM_TRAILER (p1, 5);
  NS_TEST_ASSERT_EQUAL (p->GetSize (), 1015);

  
  p = Create<Packet> (1510);
  ADD_HEADER (p, 8);
  ADD_HEADER (p, 25);
  REM_HEADER (p, 25);
  ADD_HEADER (p, 1);
  p1 = p->CreateFragment (0, 1500);
  p2 = p1->Copy ();
  ADD_HEADER (p2, 24);
  NS_TEST_ASSERT_EQUAL (p->GetSize (), 1519);

  p = Create<Packet> (1000);
  ADD_HEADER (p, 2);
  ADD_TRAILER (p, 3);
  p1 = p->Copy ();
  CHECK_HISTORY (p1, 3, 2, 1000, 3);
  REM_HEADER (p, 2);
  ADD_HEADER (p, 1);
  CHECK_HISTORY (p, 3, 1, 1000, 3);
  CHECK_HISTORY (p1, 3, 2, 1000, 3);

  p = Create<Packet> (200);
  ADD_HEADER (p, 24);
  p1 = p->CreateFragment(0, 100);
  p2 = p->CreateFragment(100, 100);	
  p1->AddAtEnd (p2);

  return result;
}

static PacketMetadataTest g_packetHistoryTest;

}//namespace ns3

#endif /* RUN_SELF_TESTS */
