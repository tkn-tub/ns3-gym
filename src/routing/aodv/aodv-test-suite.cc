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
 * Authors: Pavel Boyko <boyko@iitp.ru>
 */
#include "ns3/test.h"
#include "aodv-neighbor.h"
#include "aodv-packet.h"
#include "aodv-rqueue.h"
#include "ns3/ipv4-route.h"


namespace ns3
{
namespace aodv
{

/// Unit test for neighbors
struct NeighborTest : public TestCase
{
  NeighborTest () : TestCase ("Neighbor"), neighbor (0) { }
  virtual bool DoRun ();
  void Handler (Ipv4Address addr);
  void CheckTimeout1 ();
  void CheckTimeout2 ();
  void CheckTimeout3 ();
  Neighbors * neighbor;
};

void
NeighborTest::Handler (Ipv4Address addr)
{
}

void
NeighborTest::CheckTimeout1 ()
{
  NS_TEST_EXPECT_MSG_EQ (neighbor->IsNeighbor (Ipv4Address ("1.2.3.4")), true, "Neighbor exists");
  NS_TEST_EXPECT_MSG_EQ (neighbor->IsNeighbor (Ipv4Address ("1.1.1.1")), true, "Neighbor exists");
  NS_TEST_EXPECT_MSG_EQ (neighbor->IsNeighbor (Ipv4Address ("2.2.2.2")), true, "Neighbor exists");
  NS_TEST_EXPECT_MSG_EQ (neighbor->IsNeighbor (Ipv4Address ("3.3.3.3")), true, "Neighbor exists");
}
void
NeighborTest::CheckTimeout2 ()
{
  NS_TEST_EXPECT_MSG_EQ (neighbor->IsNeighbor (Ipv4Address ("1.2.3.4")), false, "Neighbor doesn't exist");
  NS_TEST_EXPECT_MSG_EQ (neighbor->IsNeighbor (Ipv4Address ("1.1.1.1")), false, "Neighbor doesn't exist");
  NS_TEST_EXPECT_MSG_EQ (neighbor->IsNeighbor (Ipv4Address ("2.2.2.2")), false, "Neighbor doesn't exist");
  NS_TEST_EXPECT_MSG_EQ (neighbor->IsNeighbor (Ipv4Address ("3.3.3.3")), true, "Neighbor exists");
}
void
NeighborTest::CheckTimeout3 ()
{
  NS_TEST_EXPECT_MSG_EQ (neighbor->IsNeighbor (Ipv4Address ("1.2.3.4")), false, "Neighbor doesn't exist");
  NS_TEST_EXPECT_MSG_EQ (neighbor->IsNeighbor (Ipv4Address ("1.1.1.1")), false, "Neighbor doesn't exist");
  NS_TEST_EXPECT_MSG_EQ (neighbor->IsNeighbor (Ipv4Address ("2.2.2.2")), false, "Neighbor doesn't exist");
  NS_TEST_EXPECT_MSG_EQ (neighbor->IsNeighbor (Ipv4Address ("3.3.3.3")), false, "Neighbor doesn't exist");
}

bool
NeighborTest::DoRun ()
{
  Neighbors nb (Seconds (1));
  neighbor = &nb;
  neighbor->SetCallback (MakeCallback (&NeighborTest::Handler, this));
  neighbor->Update (Ipv4Address ("1.2.3.4"), Seconds (1));
  NS_TEST_EXPECT_MSG_EQ (neighbor->IsNeighbor (Ipv4Address ("1.2.3.4")), true, "Neighbor exists");
  NS_TEST_EXPECT_MSG_EQ (neighbor->IsNeighbor (Ipv4Address ("4.3.2.1")), false, "Neighbor doesn't exist");
  neighbor->Update (Ipv4Address ("1.2.3.4"), Seconds (10));
  NS_TEST_EXPECT_MSG_EQ (neighbor->IsNeighbor (Ipv4Address ("1.2.3.4")), true, "Neighbor exists");
  NS_TEST_EXPECT_MSG_EQ (neighbor->GetExpireTime (Ipv4Address ("1.2.3.4")), Seconds (10), "Known expire time");
  NS_TEST_EXPECT_MSG_EQ (neighbor->GetExpireTime (Ipv4Address ("4.3.2.1")), Seconds (0), "Known expire time");
  neighbor->Update (Ipv4Address ("1.1.1.1"), Seconds (5));
  neighbor->Update (Ipv4Address ("2.2.2.2"), Seconds (10));
  neighbor->Update (Ipv4Address ("3.3.3.3"), Seconds (20));

  Simulator::Schedule (Seconds (2), &NeighborTest::CheckTimeout1, this);
  Simulator::Schedule (Seconds (15), &NeighborTest::CheckTimeout2, this);
  Simulator::Schedule (Seconds (30), &NeighborTest::CheckTimeout3, this);
  Simulator::Run ();
  Simulator::Destroy ();
  
  return GetErrorStatus ();
}
//-----------------------------------------------------------------------------
struct TypeHeaderTest : public TestCase
{
  TypeHeaderTest () : TestCase ("AODV TypeHeader") 
  {
  }
  virtual bool DoRun ()
  {
    TypeHeader h (AODVTYPE_RREQ);
    NS_TEST_EXPECT_MSG_EQ (h.IsValid (), true, "Default header is valid");
    NS_TEST_EXPECT_MSG_EQ (h.Get (), AODVTYPE_RREQ, "Default header is RREQ");

    Ptr<Packet> p = Create<Packet> ();
    p->AddHeader (h);
    TypeHeader h2 (AODVTYPE_RREP);
    uint32_t bytes = p->RemoveHeader (h2);
    NS_TEST_EXPECT_MSG_EQ (bytes, 1, "Type header is 1 byte long");
    NS_TEST_EXPECT_MSG_EQ (h, h2, "Round trip serialization works");
    return GetErrorStatus ();
  }
};
//-----------------------------------------------------------------------------
/// Unit test for RREQ
struct RreqHeaderTest : public TestCase
{
  RreqHeaderTest () : TestCase ("AODV RREQ") 
  {
  }
  virtual bool DoRun ()
  {
    RreqHeader h (/*flags*/0, /*reserved*/0, /*hopCount*/6, /*requestID*/1, /*dst*/Ipv4Address ("1.2.3.4"),
                  /*dstSeqNo*/40, /*origin*/Ipv4Address ("4.3.2.1"), /*originSeqNo*/10);
    NS_TEST_EXPECT_MSG_EQ (h.GetGratiousRrep (), false, "trivial");
    NS_TEST_EXPECT_MSG_EQ (h.GetDestinationOnly (), false, "trivial");
    NS_TEST_EXPECT_MSG_EQ (h.GetHopCount (), 6, "trivial");
    NS_TEST_EXPECT_MSG_EQ (h.GetId (), 1, "trivial");
    NS_TEST_EXPECT_MSG_EQ (h.GetDst (), Ipv4Address ("1.2.3.4"), "trivial");
    NS_TEST_EXPECT_MSG_EQ (h.GetDstSeqno (), 40, "trivial");
    NS_TEST_EXPECT_MSG_EQ (h.GetOrigin (), Ipv4Address ("4.3.2.1"), "trivial");
    NS_TEST_EXPECT_MSG_EQ (h.GetOriginSeqno (), 10, "trivial");

    h.SetGratiousRrep (true);
    NS_TEST_EXPECT_MSG_EQ (h.GetGratiousRrep (), true, "trivial");
    h.SetDestinationOnly (true);
    NS_TEST_EXPECT_MSG_EQ (h.GetDestinationOnly (), true, "trivial");
    h.SetUnknownSeqno (true);
    NS_TEST_EXPECT_MSG_EQ (h.GetUnknownSeqno (), true, "trivial");
    h.SetDst (Ipv4Address ("1.1.1.1"));
    NS_TEST_EXPECT_MSG_EQ (h.GetDst (), Ipv4Address ("1.1.1.1"), "trivial");
    h.SetDstSeqno (5);
    NS_TEST_EXPECT_MSG_EQ (h.GetDstSeqno (), 5, "trivial");
    h.SetHopCount (7);
    NS_TEST_EXPECT_MSG_EQ (h.GetHopCount (), 7, "trivial");
    h.SetId (55);
    NS_TEST_EXPECT_MSG_EQ (h.GetId (), 55, "trivial");
    h.SetOrigin (Ipv4Address ("4.4.4.4"));
    NS_TEST_EXPECT_MSG_EQ (h.GetOrigin (), Ipv4Address ("4.4.4.4"), "trivial");
    h.SetOriginSeqno (23);
    NS_TEST_EXPECT_MSG_EQ (h.GetOriginSeqno (), 23, "trivial");

    Ptr<Packet> p = Create<Packet> ();
    p->AddHeader (h);
    RreqHeader h2;
    uint32_t bytes = p->RemoveHeader (h2);
    NS_TEST_EXPECT_MSG_EQ (bytes, 23, "RREP is 23 bytes long");
    NS_TEST_EXPECT_MSG_EQ (h, h2, "Round trip serialization works");
    
    return GetErrorStatus ();
  }
};
//-----------------------------------------------------------------------------
/// Unit test for RREP
struct RrepHeaderTest : public TestCase
{
  RrepHeaderTest () : TestCase ("AODV RREP") {}
  virtual bool DoRun ()
  {
    RrepHeader h (/*prefixSize*/0, /*hopCount*/12, /*dst*/Ipv4Address ("1.2.3.4"), /*dstSeqNo*/2,
                  /*origin*/Ipv4Address ("4.3.2.1"), /*lifetime*/Seconds (3));
    NS_TEST_EXPECT_MSG_EQ (h.GetPrefixSize (), 0, "trivial");
    NS_TEST_EXPECT_MSG_EQ (h.GetHopCount (), 12, "trivial");
    NS_TEST_EXPECT_MSG_EQ (h.GetDst (), Ipv4Address ("1.2.3.4"), "trivial");
    NS_TEST_EXPECT_MSG_EQ (h.GetDstSeqno (), 2, "trivial");
    NS_TEST_EXPECT_MSG_EQ (h.GetOrigin (), Ipv4Address ("4.3.2.1"), "trivial");
    NS_TEST_EXPECT_MSG_EQ (h.GetLifeTime (), Seconds (3), "trivial");
    h.SetDst (Ipv4Address ("1.1.1.1"));
    NS_TEST_EXPECT_MSG_EQ (h.GetDst (), Ipv4Address ("1.1.1.1"), "trivial");
    h.SetDstSeqno (123);
    NS_TEST_EXPECT_MSG_EQ (h.GetDstSeqno (), 123, "trivial");
    h.SetOrigin (Ipv4Address ("4.4.4.4"));
    NS_TEST_EXPECT_MSG_EQ (h.GetOrigin (), Ipv4Address ("4.4.4.4"), "trivial");
    h.SetLifeTime (MilliSeconds (1200));
    NS_TEST_EXPECT_MSG_EQ (h.GetLifeTime (), MilliSeconds (1200), "trivial");
    h.SetAckRequired (true);
    NS_TEST_EXPECT_MSG_EQ (h.GetAckRequired (), true, "trivial");
    h.SetAckRequired (false);
    NS_TEST_EXPECT_MSG_EQ (h.GetAckRequired (), false, "trivial");
    h.SetPrefixSize (2);
    NS_TEST_EXPECT_MSG_EQ (h.GetPrefixSize (), 2, "trivial");
    h.SetHopCount (15);
    NS_TEST_EXPECT_MSG_EQ (h.GetHopCount (), 15, "trivial");

    h.SetHello (Ipv4Address ("10.0.0.2"), 9, Seconds (15));
    NS_TEST_EXPECT_MSG_EQ (h.GetDst (), h.GetOrigin (), "trivial");
    NS_TEST_EXPECT_MSG_EQ (h.GetDstSeqno (), 9, "trivial");
    NS_TEST_EXPECT_MSG_EQ (h.GetLifeTime (), Seconds (15), "trivial");

    Ptr<Packet> p = Create<Packet> ();
    p->AddHeader (h);
    RrepHeader h2;
    uint32_t bytes = p->RemoveHeader (h2);
    NS_TEST_EXPECT_MSG_EQ (bytes, 19, "RREP is 19 bytes long");
    NS_TEST_EXPECT_MSG_EQ (h, h2, "Round trip serialization works");
    return GetErrorStatus ();
  }
};
//-----------------------------------------------------------------------------
/// Unit test for RREP-ACK
struct RrepAckHeaderTest : public TestCase
{
  RrepAckHeaderTest () : TestCase ("AODV RREP-ACK")
  {
  }
  virtual bool DoRun()
  {
    RrepAckHeader h;
    Ptr<Packet> p = Create<Packet> ();
    p->AddHeader (h);
    RrepAckHeader h2;
    uint32_t bytes = p->RemoveHeader(h2);
    NS_TEST_EXPECT_MSG_EQ (bytes, 1, "ACK is 1 byte long");
    NS_TEST_EXPECT_MSG_EQ (h, h2, "Round trip serialization works");
    return GetErrorStatus ();
  }
};
//-----------------------------------------------------------------------------
/// Unit test for RERR
struct RerrHeaderTest : public TestCase
{
  RerrHeaderTest () : TestCase ("AODV RERR")
  {
  }
  virtual bool DoRun()
  {
    RerrHeader h;
    h.SetNoDelete(true);
    NS_TEST_EXPECT_MSG_EQ(h.GetNoDelete(), true, "trivial");
    Ipv4Address dst = Ipv4Address("1.2.3.4");
    NS_TEST_EXPECT_MSG_EQ(h.AddUnDestination(dst, 12), true, "trivial");
    NS_TEST_EXPECT_MSG_EQ(h.GetDestCount(), 1, "trivial");
    NS_TEST_EXPECT_MSG_EQ(h.AddUnDestination(dst, 13), true, "trivial");
    Ipv4Address dst2 = Ipv4Address("4.3.2.1");
    NS_TEST_EXPECT_MSG_EQ(h.AddUnDestination(dst2, 12), true, "trivial");
    NS_TEST_EXPECT_MSG_EQ(h.GetDestCount(), 2, "trivial");

    Ptr<Packet> p = Create<Packet> ();
    p->AddHeader (h);
    RerrHeader h2;
    uint32_t bytes = p->RemoveHeader(h2);
    NS_TEST_EXPECT_MSG_EQ (bytes, h.GetSerializedSize(), "(De)Serialized size match");
    NS_TEST_EXPECT_MSG_EQ (h, h2, "Round trip serialization works");
    return GetErrorStatus ();
  }
};
//-----------------------------------------------------------------------------
/// Unit test for AODV routing table entry
struct QueueEntryTest : public TestCase
{
  QueueEntryTest () : TestCase ("QueueEntry") {}
  void Unicast (Ptr<Ipv4Route> route, Ptr<const Packet> packet, const Ipv4Header & header) {}
  void Error (Ptr<const Packet>, const Ipv4Header &, Socket::SocketErrno) {}
  void Unicast2 (Ptr<Ipv4Route> route, Ptr<const Packet> packet, const Ipv4Header & header) {}
  void Error2 (Ptr<const Packet>, const Ipv4Header &, Socket::SocketErrno) {}
  virtual bool DoRun()
  {
    Ptr<const Packet> packet = Create<Packet> ();
    Ipv4Header h;
    h.SetDestination (Ipv4Address ("1.2.3.4"));
    h.SetSource (Ipv4Address ("4.3.2.1"));
    Ipv4RoutingProtocol::UnicastForwardCallback ucb = MakeCallback (&QueueEntryTest::Unicast, this);
    Ipv4RoutingProtocol::ErrorCallback ecb = MakeCallback (&QueueEntryTest::Error, this);
    QueueEntry entry (packet, h, ucb, ecb, Seconds (1));
    NS_TEST_EXPECT_MSG_EQ (h.GetDestination (),  entry.GetIpv4Header ().GetDestination (), "trivial");
    NS_TEST_EXPECT_MSG_EQ (h.GetSource (),  entry.GetIpv4Header ().GetSource (), "trivial");
    NS_TEST_EXPECT_MSG_EQ (ucb.IsEqual(entry.GetUnicastForwardCallback ()), true, "trivial");
    NS_TEST_EXPECT_MSG_EQ (ecb.IsEqual(entry.GetErrorCallback ()), true, "trivial");
    NS_TEST_EXPECT_MSG_EQ (entry.GetExpireTime (), Seconds (1), "trivial");
    NS_TEST_EXPECT_MSG_EQ (entry.GetPacket (), packet, "trivial");
    entry.SetExpireTime (Seconds (3));
    NS_TEST_EXPECT_MSG_EQ (entry.GetExpireTime (), Seconds (3), "trivial");
    Ipv4Header h2;
    h2.SetDestination(Ipv4Address ("1.1.1.1"));
    entry.SetIpv4Header (h2);
    NS_TEST_EXPECT_MSG_EQ (entry.GetIpv4Header ().GetDestination (), Ipv4Address ("1.1.1.1"), "trivial");
    Ipv4RoutingProtocol::UnicastForwardCallback ucb2 = MakeCallback (&QueueEntryTest::Unicast2, this);
    Ipv4RoutingProtocol::ErrorCallback ecb2 = MakeCallback (&QueueEntryTest::Error2, this);
    entry.SetErrorCallback (ecb2);
    NS_TEST_EXPECT_MSG_EQ (ecb2.IsEqual (entry.GetErrorCallback ()), true, "trivial");
    entry.SetUnicastForwardCallback (ucb2);
    NS_TEST_EXPECT_MSG_EQ (ucb2.IsEqual (entry.GetUnicastForwardCallback ()), true, "trivial");

    return GetErrorStatus();
  }
};
//-----------------------------------------------------------------------------
class AodvTestSuite : public TestSuite
{
public:
  AodvTestSuite () : TestSuite ("routing-aodv", UNIT)
  {
    AddTestCase (new NeighborTest);
    AddTestCase (new TypeHeaderTest);
    AddTestCase (new RreqHeaderTest);
    AddTestCase (new RrepHeaderTest);
    AddTestCase (new RrepAckHeaderTest);
    AddTestCase (new RerrHeaderTest);
    AddTestCase (new QueueEntryTest);
  }
} g_aodvTestSuite;

}}
