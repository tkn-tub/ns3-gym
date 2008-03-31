/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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
 */

#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "drop-tail-queue.h"

NS_LOG_COMPONENT_DEFINE ("DropTailQueue");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (DropTailQueue);

TypeId DropTailQueue::GetTypeId (void) 
{
  static TypeId tid = TypeId ("ns3::DropTailQueue")
    .SetParent<Queue> ()
    .AddConstructor<DropTailQueue> ()
    .AddAttribute ("MaxPackets", "The maximum number of packets accepted by this DropTailQueue.",
                   Uinteger (100),
                   MakeUintegerAccessor (&DropTailQueue::m_maxPackets),
                   MakeUintegerChecker<uint32_t> ())
    ;
  
  return tid;
}

DropTailQueue::DropTailQueue () :
  Queue (),
  m_packets ()
{
  NS_LOG_FUNCTION;
}

DropTailQueue::~DropTailQueue ()
{
  NS_LOG_FUNCTION;
}

bool 
DropTailQueue::DoEnqueue (Ptr<Packet> p)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAMS (this << p);

  if (m_packets.size () >= m_maxPackets)
    {
      NS_LOG_LOGIC ("Queue full -- droppping pkt");
      Drop (p);
      return false;
    }

  m_packets.push(p);
  return true;
}

Ptr<Packet>
DropTailQueue::DoDequeue (void)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAMS (this);

  if (m_packets.empty()) 
    {
      NS_LOG_LOGIC ("Queue empty");
      return false;
    }

  Ptr<Packet> p = m_packets.front ();
  m_packets.pop ();

  NS_LOG_LOGIC ("Popped " << p);

  return p;
}

Ptr<Packet>
DropTailQueue::DoPeek (void) const
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAMS (this);

  if (m_packets.empty()) 
    {
      NS_LOG_LOGIC ("Queue empty");
      return false;
    }

  Ptr<Packet> p = m_packets.front ();

  return p;
}

} // namespace ns3


#ifdef RUN_SELF_TESTS

#include "ns3/test.h"

namespace ns3 {

class DropTailQueueTest: public Test {
public:
  virtual bool RunTests (void);
  DropTailQueueTest ();
};


DropTailQueueTest::DropTailQueueTest ()
  : Test ("DropTailQueue") {}


bool
DropTailQueueTest::RunTests (void)
{
  bool result = true;

  Ptr<DropTailQueue> queue = CreateObject<DropTailQueue> ();
  NS_TEST_ASSERT (queue->SetAttributeFailSafe ("MaxPackets", Uinteger (3)));
  
  Ptr<Packet> p1, p2, p3, p4;
  p1 = Create<Packet> ();
  p2 = Create<Packet> ();
  p3 = Create<Packet> ();
  p4 = Create<Packet> ();

  NS_TEST_ASSERT_EQUAL (queue->GetNPackets (), 0);
  queue->Enqueue (p1);
  NS_TEST_ASSERT_EQUAL (queue->GetNPackets (), 1);
  queue->Enqueue (p2);
  NS_TEST_ASSERT_EQUAL (queue->GetNPackets (), 2);
  queue->Enqueue (p3);
  NS_TEST_ASSERT_EQUAL (queue->GetNPackets (), 3);
  queue->Enqueue (p4); // will be dropped
  NS_TEST_ASSERT_EQUAL (queue->GetNPackets (), 3);

  Ptr<Packet> p;

  p = queue->Dequeue ();
  NS_TEST_ASSERT (p != 0);
  NS_TEST_ASSERT_EQUAL (queue->GetNPackets (), 2);
  NS_TEST_ASSERT_EQUAL (p->GetUid (), p1->GetUid ());

  p = queue->Dequeue ();
  NS_TEST_ASSERT (p != 0);
  NS_TEST_ASSERT_EQUAL (queue->GetNPackets (), 1);
  NS_TEST_ASSERT_EQUAL (p->GetUid (), p2->GetUid ());

  p = queue->Dequeue ();
  NS_TEST_ASSERT (p != 0);
  NS_TEST_ASSERT_EQUAL (queue->GetNPackets (), 0);
  NS_TEST_ASSERT_EQUAL (p->GetUid (), p3->GetUid ());

  p = queue->Dequeue ();
  NS_TEST_ASSERT (p == 0);

  return result;
}


static DropTailQueueTest gDropTailQueueTest;

}; // namespace ns3

#endif /* RUN_SELF_TESTS */

