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
#include "aodv-rqueue.h"
#include "ns3/simulator.h"
#include "ns3/test.h"
#include <algorithm>
#include <functional>
#include "ns3/ipv4-route.h"
#include "ns3/socket.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("AodvRequestQueue");

namespace ns3
{
namespace aodv
{

#ifdef RUN_SELF_TESTS
/// Unit test for AODV routing table entry
struct QueueEntryTest : public Test
{
  QueueEntryTest () : Test ("AODV/QueueEntry"), result(true) {}
  virtual bool RunTests();
  void Unicast (Ptr<Ipv4Route> route, Ptr<const Packet> packet, const Ipv4Header & header) {}
  void Error (Ptr<const Packet>, const Ipv4Header &, Socket::SocketErrno) {}
  void Unicast2 (Ptr<Ipv4Route> route, Ptr<const Packet> packet, const Ipv4Header & header) {}
  void Error2 (Ptr<const Packet>, const Ipv4Header &, Socket::SocketErrno) {}
  bool result;
};

/// Test instance
static QueueEntryTest g_QueueEntryTest;

bool
QueueEntryTest::RunTests ()
{
  Ptr<const Packet> packet = Create<Packet> ();
  Ipv4Header h;
  h.SetDestination (Ipv4Address("1.2.3.4"));
  h.SetSource (Ipv4Address("4.3.2.1"));
  Ipv4RoutingProtocol::UnicastForwardCallback ucb = MakeCallback (&QueueEntryTest::Unicast, this);
  Ipv4RoutingProtocol::ErrorCallback ecb = MakeCallback (&QueueEntryTest::Error, this);
  QueueEntry entry (packet, h, ucb, ecb, Seconds(1));
  NS_TEST_ASSERT_EQUAL (h.GetDestination (),  entry.GetIpv4Header ().GetDestination ());
  NS_TEST_ASSERT_EQUAL (h.GetSource (),  entry.GetIpv4Header ().GetSource ());
  NS_TEST_ASSERT_EQUAL (ucb.IsEqual(entry.GetUnicastForwardCallback()), true);
  NS_TEST_ASSERT_EQUAL (ecb.IsEqual(entry.GetErrorCallback()), true);
  NS_TEST_ASSERT_EQUAL (entry.GetExpireTime(), Seconds(1));
  NS_TEST_ASSERT_EQUAL (entry.GetPacket(), packet);
  entry.SetExpireTime(Seconds(3));
  NS_TEST_ASSERT_EQUAL (entry.GetExpireTime(), Seconds(3));
  Ipv4Header h2;
  h2.SetDestination(Ipv4Address("1.1.1.1"));
  entry.SetIpv4Header(h2);
  NS_TEST_ASSERT_EQUAL (entry.GetIpv4Header ().GetDestination (), Ipv4Address("1.1.1.1"));
  Ipv4RoutingProtocol::UnicastForwardCallback ucb2 = MakeCallback (&QueueEntryTest::Unicast2, this);
  Ipv4RoutingProtocol::ErrorCallback ecb2 = MakeCallback (&QueueEntryTest::Error2, this);
  entry.SetErrorCallback(ecb2);
  NS_TEST_ASSERT_EQUAL (ecb2.IsEqual(entry.GetErrorCallback()), true);
  entry.SetUnicastForwardCallback(ucb2);
  NS_TEST_ASSERT_EQUAL (ucb2.IsEqual(entry.GetUnicastForwardCallback()), true);

  return result;
}
#endif


uint32_t
RequestQueue::GetSize ()
{
  Purge ();
  return m_queue.size ();
}

void
RequestQueue::Enqueue (QueueEntry & entry )
{
  Purge ();
  entry.SetExpireTime (m_queueTimeout);

  if (m_queue.size () == m_maxLen)
    Drop (Pop (), "Drop the most aged packet"); // Drop the most aged packet
  m_queue.push_back (entry);
}

void
RequestQueue::DropPacketWithDst (Ipv4Address dst )
{
  NS_LOG_FUNCTION (this << dst);
  Purge ();
  const Ipv4Address addr = dst;
  std::vector<QueueEntry>::iterator i = std::remove_if (m_queue.begin (), m_queue.end (), std::bind2nd (std::ptr_fun (RequestQueue::IsEqual), dst));
  for (std::vector<QueueEntry>::iterator j = i; j != m_queue.end (); ++j)
    {
      Drop (*j, "DropPacketWithDst ");
    }
  m_queue.erase (i, m_queue.end ());
}

bool
RequestQueue::Dequeue (Ipv4Address dst, QueueEntry & entry )
{
  Purge ();
  for (std::vector<QueueEntry>::iterator i = m_queue.begin (); i != m_queue.end (); ++i)
    if (i->GetIpv4Header ().GetDestination () == dst)
      {
        entry = *i;
        m_queue.erase (i);
        return true;
      }
  return false;
}

bool
RequestQueue::Find (Ipv4Address dst )
{
  for (std::vector<QueueEntry>::const_iterator i = m_queue.begin (); i != m_queue.end (); ++i)
    if (i->GetIpv4Header ().GetDestination () == dst)
      return true;
  return false;
}

QueueEntry
RequestQueue::Pop ()
{
  NS_LOG_FUNCTION (this);
  QueueEntry entry = m_queue.front ();
  m_queue.erase (m_queue.begin ());
  return entry;
}

struct IsExpired
{
  bool
  operator() (QueueEntry const & e ) const
  {
    return (e.GetExpireTime () < Seconds (0));
  }
};

void
RequestQueue::Purge ()
{
  std::vector<QueueEntry>::iterator i = std::remove_if (m_queue.begin (), m_queue.end (), IsExpired ());
  for (std::vector<QueueEntry>::iterator j = i; j < m_queue.end (); ++j)
    {
      Drop (*j, "Drop outdated packet ");
    }
  m_queue.erase (i, m_queue.end ());
}

void
RequestQueue::Drop (QueueEntry en, std::string reason )
{
  NS_LOG_UNCOND (reason << en.GetPacket ()->GetUid () << " " << en.GetIpv4Header ().GetDestination ());
  en.GetErrorCallback() (en.GetPacket(), en.GetIpv4Header(), Socket::ERROR_NOROUTETOHOST);
  return;
}


#ifdef RUN_SELF_TESTS
/// Unit test for RequestQueue
struct AodvRqueueTest : public Test
  {
    AodvRqueueTest () : Test ("AODV/Rqueue"), q(64, Seconds(30)), result(true) {}
    virtual bool RunTests();
    void Unicast (Ptr<Ipv4Route> route, Ptr<const Packet> packet, const Ipv4Header & header) {}
    void Error (Ptr<const Packet>, const Ipv4Header &, Socket::SocketErrno) {}
    void CheckSizeLimit ();
    void CheckTimeout ();

    RequestQueue q;
    bool result;
  };

/// Test instance
static AodvRqueueTest g_AodvRqueueTest;

bool
AodvRqueueTest::RunTests ()
{
  NS_TEST_ASSERT_EQUAL (q.GetMaxQueueLen(), 64);
  q.SetMaxQueueLen(32);
  NS_TEST_ASSERT_EQUAL (q.GetMaxQueueLen(), 32);
  NS_TEST_ASSERT_EQUAL (q.GetQueueTimeout(), Seconds(30));
  q.SetQueueTimeout(Seconds(10));
  NS_TEST_ASSERT_EQUAL (q.GetQueueTimeout(), Seconds(10));

  Ptr<const Packet> packet = Create<Packet> ();
  Ipv4Header h;
  h.SetDestination (Ipv4Address ("1.2.3.4"));
  h.SetSource (Ipv4Address ("4.3.2.1"));
  Ipv4RoutingProtocol::UnicastForwardCallback ucb = MakeCallback (&AodvRqueueTest::Unicast, this);
  Ipv4RoutingProtocol::ErrorCallback ecb = MakeCallback (&AodvRqueueTest::Error, this);
  QueueEntry e1 (packet, h, ucb, ecb, Seconds (1));
  q.Enqueue (e1);
  q.Enqueue (e1);
  q.Enqueue (e1);
  NS_TEST_ASSERT_EQUAL (q.Find(Ipv4Address ("1.2.3.4")), true);
  NS_TEST_ASSERT_EQUAL (q.Find(Ipv4Address ("1.1.1.1")), false);
  NS_TEST_ASSERT_EQUAL (q.GetSize(), 3);
  q.DropPacketWithDst(Ipv4Address ("1.2.3.4"));
  NS_TEST_ASSERT_EQUAL (q.Find(Ipv4Address ("1.2.3.4")), false);
  NS_TEST_ASSERT_EQUAL (q.GetSize(), 0);

  h.SetDestination(Ipv4Address("2.2.2.2"));
  QueueEntry e2 (packet, h, ucb, ecb, Seconds (1));
  q.Enqueue (e1);
  q.Enqueue (e2);
  Ptr<Packet> packet2 = Create<Packet> ();
  QueueEntry e3 (packet2, h, ucb, ecb, Seconds (1));
  NS_TEST_ASSERT_EQUAL(q.Dequeue(Ipv4Address("3.3.3.3"), e3), false);
  NS_TEST_ASSERT_EQUAL(q.Dequeue(Ipv4Address("2.2.2.2"), e3), true);
  NS_TEST_ASSERT_EQUAL (q.Find(Ipv4Address("2.2.2.2")), false);
  q.Enqueue(e2);
  q.Enqueue(e3);
  q.Enqueue(e1);
  NS_TEST_ASSERT_EQUAL (q.GetSize(), 4);
  q.DropPacketWithDst(Ipv4Address ("1.2.3.4"));
  NS_TEST_ASSERT_EQUAL (q.GetSize(), 2);

  CheckSizeLimit ();

  Ipv4Header header2;
  Ipv4Address dst2 ("1.2.3.4");
  header2.SetDestination (dst2);

  Simulator::Schedule (q.GetQueueTimeout() + Seconds(1), &AodvRqueueTest::CheckTimeout, this);

  Simulator::Run ();
  Simulator::Destroy ();

  return result;
}

void
AodvRqueueTest::CheckSizeLimit ()
{
  Ptr<Packet> packet = Create<Packet> ();
  Ipv4Header header;
  Ipv4RoutingProtocol::UnicastForwardCallback ucb = MakeCallback (&AodvRqueueTest::Unicast, this);
  Ipv4RoutingProtocol::ErrorCallback ecb = MakeCallback (&AodvRqueueTest::Error, this);
  QueueEntry e1 (packet, header, ucb, ecb, Seconds (1));

  for (uint32_t i = 0; i < q.GetMaxQueueLen (); ++i)
    q.Enqueue (e1);
  NS_TEST_ASSERT_EQUAL (q.GetSize (), q.GetMaxQueueLen ());

  for (uint32_t i = 0; i < q.GetMaxQueueLen (); ++i)
    q.Enqueue (e1);
  NS_TEST_ASSERT_EQUAL (q.GetSize (), q.GetMaxQueueLen ());
}

void
AodvRqueueTest::CheckTimeout ()
{
  NS_TEST_ASSERT_EQUAL (q.GetSize (), 0);
}
#endif

}
}
