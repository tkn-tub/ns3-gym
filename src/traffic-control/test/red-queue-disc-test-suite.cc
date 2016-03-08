/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright © 2011 Marcos Talau
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
 * Author: Marcos Talau (talau@users.sourceforge.net)
 * Modified by:   Pasquale Imputato <p.imputato@gmail.com>
 *
 */

#include "ns3/test.h"
#include "ns3/red-queue-disc.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/double.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/ipv4-queue-disc-item.h"

using namespace ns3;

class RedQueueDiscTestCase : public TestCase
{
public:
  RedQueueDiscTestCase ();
  virtual void DoRun (void);
private:
  void Enqueue (Ptr<RedQueueDisc> queue, uint32_t size, uint32_t nPkt);
  void RunRedTest (StringValue mode);
};

RedQueueDiscTestCase::RedQueueDiscTestCase ()
  : TestCase ("Sanity check on the red queue implementation")
{
}

void
RedQueueDiscTestCase::RunRedTest (StringValue mode)
{
  uint32_t pktSize = 0;
  // 1 for packets; pktSize for bytes
  uint32_t modeSize = 1;
  double minTh = 2;
  double maxTh = 5;
  uint32_t qSize = 8;
  Ptr<RedQueueDisc> queue = CreateObject<RedQueueDisc> ();

  // test 1: simple enqueue/dequeue with no drops
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MinTh", DoubleValue (minTh)), true,
                         "Verify that we can actually set the attribute MinTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxTh", DoubleValue (maxTh)), true,
                         "Verify that we can actually set the attribute MaxTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QW", DoubleValue (0.002)), true,
                         "Verify that we can actually set the attribute QW");

  Ipv4Header ipHeader;
  Address dest;
  
  if (queue->GetMode () == Queue::QUEUE_MODE_BYTES)
    {
      pktSize = 1000;
      modeSize = pktSize + ipHeader.GetSerializedSize ();
      queue->SetTh (minTh * modeSize, maxTh * modeSize);
      queue->SetQueueLimit (qSize * modeSize);
    }

  Ptr<Packet> p1, p2, p3, p4, p5, p6, p7, p8;
  p1 = Create<Packet> (pktSize);
  p2 = Create<Packet> (pktSize);
  p3 = Create<Packet> (pktSize);
  p4 = Create<Packet> (pktSize);
  p5 = Create<Packet> (pktSize);
  p6 = Create<Packet> (pktSize);
  p7 = Create<Packet> (pktSize);
  p8 = Create<Packet> (pktSize);

  queue->Initialize ();
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 0 * modeSize, "There should be no packets in there");
  queue->Enqueue (Create<Ipv4QueueDiscItem> (p1, dest, 0, ipHeader));
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 1 * modeSize, "There should be one packet in there");
  queue->Enqueue (Create<Ipv4QueueDiscItem> (p2, dest, 0, ipHeader));
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 2 * modeSize, "There should be two packets in there");
  queue->Enqueue (Create<Ipv4QueueDiscItem> (p3, dest, 0, ipHeader));
  queue->Enqueue (Create<Ipv4QueueDiscItem> (p4, dest, 0, ipHeader));
  queue->Enqueue (Create<Ipv4QueueDiscItem> (p5, dest, 0, ipHeader));
  queue->Enqueue (Create<Ipv4QueueDiscItem> (p6, dest, 0, ipHeader));
  queue->Enqueue (Create<Ipv4QueueDiscItem> (p7, dest, 0, ipHeader));
  queue->Enqueue (Create<Ipv4QueueDiscItem> (p8, dest, 0, ipHeader));
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 8 * modeSize, "There should be eight packets in there");

  Ptr<Ipv4QueueDiscItem> item;

  item = StaticCast<Ipv4QueueDiscItem> (queue->Dequeue ());
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the first packet");
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 7 * modeSize, "There should be seven packets in there");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p1->GetUid (), "was this the first packet ?");

  item = StaticCast<Ipv4QueueDiscItem> (queue->Dequeue ());
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the second packet");
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 6 * modeSize, "There should be six packet in there");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p2->GetUid (), "Was this the second packet ?");

  item = StaticCast<Ipv4QueueDiscItem> (queue->Dequeue ());
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the third packet");
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 5 * modeSize, "There should be five packets in there");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p3->GetUid (), "Was this the third packet ?");

  item = StaticCast<Ipv4QueueDiscItem> (queue->Dequeue ());
  item = StaticCast<Ipv4QueueDiscItem> (queue->Dequeue ());
  item = StaticCast<Ipv4QueueDiscItem> (queue->Dequeue ());
  item = StaticCast<Ipv4QueueDiscItem> (queue->Dequeue ());
  item = StaticCast<Ipv4QueueDiscItem> (queue->Dequeue ());

  item = StaticCast<Ipv4QueueDiscItem> (queue->Dequeue ());
  NS_TEST_EXPECT_MSG_EQ ((item == 0), true, "There are really no packets in there");


  // test 2: more data, but with no drops
  queue = CreateObject<RedQueueDisc> ();
  minTh = 70 * modeSize;
  maxTh = 150 * modeSize;
  qSize = 300 * modeSize;
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MinTh", DoubleValue (minTh)), true,
                         "Verify that we can actually set the attribute MinTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxTh", DoubleValue (maxTh)), true,
                         "Verify that we can actually set the attribute MaxTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  queue->Initialize ();
  Enqueue (queue, pktSize, 300);
  RedQueueDisc::Stats st = StaticCast<RedQueueDisc> (queue)->GetStats ();
  NS_TEST_EXPECT_MSG_EQ (st.unforcedDrop, 0, "There should zero dropped packets due probability mark");
  NS_TEST_EXPECT_MSG_EQ (st.forcedDrop, 0, "There should zero dropped packets due hardmark mark");
  NS_TEST_EXPECT_MSG_EQ (st.qLimDrop, 0, "There should zero dropped packets due queue full");

  // save number of drops from tests
  struct d {
    uint32_t test3;
    uint32_t test4;
    uint32_t test5;
    uint32_t test6;
    uint32_t test7;
  } drop;


  // test 3: more data, now drops due QW change
  queue = CreateObject<RedQueueDisc> ();
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MinTh", DoubleValue (minTh)), true,
                         "Verify that we can actually set the attribute MinTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxTh", DoubleValue (maxTh)), true,
                         "Verify that we can actually set the attribute MaxTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QW", DoubleValue (0.020)), true,
                         "Verify that we can actually set the attribute QW");
  queue->Initialize ();
  Enqueue (queue, pktSize, 300);
  st = StaticCast<RedQueueDisc> (queue)->GetStats ();
  drop.test3 = st.unforcedDrop + st.forcedDrop + st.qLimDrop;
  NS_TEST_EXPECT_MSG_NE (drop.test3, 0, "There should be some dropped packets");


  // test 4: reduced maxTh, this causes more drops
  maxTh = 100 * modeSize;
  queue = CreateObject<RedQueueDisc> ();
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MinTh", DoubleValue (minTh)), true,
                         "Verify that we can actually set the attribute MinTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxTh", DoubleValue (maxTh)), true,
                         "Verify that we can actually set the attribute MaxTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QW", DoubleValue (0.020)), true,
                         "Verify that we can actually set the attribute QW");
  queue->Initialize ();
  Enqueue (queue, pktSize, 300);
  st = StaticCast<RedQueueDisc> (queue)->GetStats ();
  drop.test4 = st.unforcedDrop + st.forcedDrop + st.qLimDrop;
  NS_TEST_EXPECT_MSG_GT (drop.test4, drop.test3, "Test 4 should have more drops than test 3");


  // test 5: change drop probability to a high value (LInterm)
  maxTh = 150 * modeSize;
  queue = CreateObject<RedQueueDisc> ();
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MinTh", DoubleValue (minTh)), true,
                         "Verify that we can actually set the attribute MinTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxTh", DoubleValue (maxTh)), true,
                         "Verify that we can actually set the attribute MaxTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QW", DoubleValue (0.020)), true,
                         "Verify that we can actually set the attribute QW");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("LInterm", DoubleValue (5)), true,
                         "Verify that we can actually set the attribute LInterm");
  queue->Initialize ();
  Enqueue (queue, pktSize, 300);
  st = StaticCast<RedQueueDisc> (queue)->GetStats ();
  drop.test5 = st.unforcedDrop + st.forcedDrop + st.qLimDrop;
  NS_TEST_EXPECT_MSG_GT (drop.test5, drop.test3, "Test 5 should have more drops than test 3");


  // test 6: disable Gentle param
  queue = CreateObject<RedQueueDisc> ();
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MinTh", DoubleValue (minTh)), true,
                         "Verify that we can actually set the attribute MinTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxTh", DoubleValue (maxTh)), true,
                         "Verify that we can actually set the attribute MaxTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QW", DoubleValue (0.020)), true,
                         "Verify that we can actually set the attribute QW");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Gentle", BooleanValue (false)), true,
                         "Verify that we can actually set the attribute Gentle");
  queue->Initialize ();
  Enqueue (queue, pktSize, 300);
  st = StaticCast<RedQueueDisc> (queue)->GetStats ();
  drop.test6 = st.unforcedDrop + st.forcedDrop + st.qLimDrop;
  NS_TEST_EXPECT_MSG_GT (drop.test6, drop.test3, "Test 6 should have more drops than test 3");


  // test 7: disable Wait param
  queue = CreateObject<RedQueueDisc> ();
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MinTh", DoubleValue (minTh)), true,
                         "Verify that we can actually set the attribute MinTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxTh", DoubleValue (maxTh)), true,
                         "Verify that we can actually set the attribute MaxTh");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QW", DoubleValue (0.020)), true,
                         "Verify that we can actually set the attribute QW");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Wait", BooleanValue (false)), true,
                         "Verify that we can actually set the attribute Wait");
  queue->Initialize ();
  Enqueue (queue, pktSize, 300);
  st = StaticCast<RedQueueDisc> (queue)->GetStats ();
  drop.test7 = st.unforcedDrop + st.forcedDrop + st.qLimDrop;
  NS_TEST_EXPECT_MSG_GT (drop.test7, drop.test3, "Test 7 should have more drops than test 3");
}

void 
RedQueueDiscTestCase::Enqueue (Ptr<RedQueueDisc> queue, uint32_t size, uint32_t nPkt)
{
  Ipv4Header ipHeader;
  Address dest;
  for (uint32_t i = 0; i < nPkt; i++)
    {
      queue->Enqueue (Create<Ipv4QueueDiscItem> (Create<Packet> (size), dest, 0, ipHeader));
    }
}

void
RedQueueDiscTestCase::DoRun (void)
{
  RunRedTest (StringValue ("QUEUE_MODE_PACKETS"));
  RunRedTest (StringValue ("QUEUE_MODE_BYTES"));
  Simulator::Destroy ();

}

static class RedQueueDiscTestSuite : public TestSuite
{
public:
  RedQueueDiscTestSuite ()
    : TestSuite ("red-queue-disc", UNIT)
  {
    AddTestCase (new RedQueueDiscTestCase (), TestCase::QUICK);
  }
} g_redQueueTestSuite;
