/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 NITK Surathkal
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
 * Authors: Shravya Ks <shravya.ks0@gmail.com>
 *          Smriti Murali <m.smriti.95@gmail.com>
 *          Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 *
 */

#include "ns3/test.h"
#include "ns3/pie-queue-disc.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/double.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

using namespace ns3;

/**
 * \ingroup traffic-control-test
 * \ingroup tests
 *
 * \brief Pie Queue Disc Test Item
 */
class PieQueueDiscTestItem : public QueueDiscItem
{
public:
  /**
   * Constructor
   *
   * \param p the packet
   * \param addr the address
   * \param protocol the protocol
   */
  PieQueueDiscTestItem (Ptr<Packet> p, const Address & addr, uint16_t protocol);
  virtual ~PieQueueDiscTestItem ();
  virtual void AddHeader (void);
  virtual bool Mark (void);

private:
  PieQueueDiscTestItem ();
  /**
   * \brief Copy constructor
   * Disable default implementation to avoid misuse
   */
  PieQueueDiscTestItem (const PieQueueDiscTestItem &);
  /**
   * \brief Assignment operator
   * \return this object
   * Disable default implementation to avoid misuse
   */
  PieQueueDiscTestItem &operator = (const PieQueueDiscTestItem &);
};

PieQueueDiscTestItem::PieQueueDiscTestItem (Ptr<Packet> p, const Address & addr, uint16_t protocol)
  : QueueDiscItem (p, addr, protocol)
{
}

PieQueueDiscTestItem::~PieQueueDiscTestItem ()
{
}

void
PieQueueDiscTestItem::AddHeader (void)
{
}

bool
PieQueueDiscTestItem::Mark (void)
{
  return false;
}

/**
 * \ingroup traffic-control-test
 * \ingroup tests
 *
 * \brief Pie Queue Disc Test Case
 */
class PieQueueDiscTestCase : public TestCase
{
public:
  PieQueueDiscTestCase ();
  virtual void DoRun (void);
private:
  /**
   * Enqueue function
   * \param queue the queue disc
   * \param size the size
   * \param nPkt the number of packets
   */
  void Enqueue (Ptr<PieQueueDisc> queue, uint32_t size, uint32_t nPkt);
  /**
   * Enqueue with delay function
   * \param queue the queue disc
   * \param size the size
   * \param nPkt the number of packets
   */
  void EnqueueWithDelay (Ptr<PieQueueDisc> queue, uint32_t size, uint32_t nPkt);
  /**
   * Dequeue function
   * \param queue the queue disc
   * \param nPkt the number of packets
   */
  void Dequeue (Ptr<PieQueueDisc> queue, uint32_t nPkt);
  /**
   * Dequeue with delay function
   * \param queue the queue disc
   * \param delay the delay
   * \param nPkt the number of packets
   */
  void DequeueWithDelay (Ptr<PieQueueDisc> queue, double delay, uint32_t nPkt);
  /**
   * Run test function
   * \param mode the test mode
   */
  void RunPieTest (StringValue mode);
};

PieQueueDiscTestCase::PieQueueDiscTestCase ()
  : TestCase ("Sanity check on the pie queue disc implementation")
{
}

void
PieQueueDiscTestCase::RunPieTest (StringValue mode)
{
  uint32_t pktSize = 0;

  // 1 for packets; pktSize for bytes
  uint32_t modeSize = 1;

  uint32_t qSize = 300;
  Ptr<PieQueueDisc> queue = CreateObject<PieQueueDisc> ();


  // test 1: simple enqueue/dequeue with defaults, no drops
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");

  Address dest;

  if (queue->GetMode () == PieQueueDisc::QUEUE_DISC_MODE_BYTES)
    {
      // pktSize should be same as MeanPktSize to avoid performance gap between byte and packet mode
      pktSize = 1000;
      modeSize = pktSize;
      qSize = qSize * modeSize;
    }

  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");

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
  queue->Enqueue (Create<PieQueueDiscTestItem> (p1, dest, 0));
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 1 * modeSize, "There should be one packet in there");
  queue->Enqueue (Create<PieQueueDiscTestItem> (p2, dest, 0));
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 2 * modeSize, "There should be two packets in there");
  queue->Enqueue (Create<PieQueueDiscTestItem> (p3, dest, 0));
  queue->Enqueue (Create<PieQueueDiscTestItem> (p4, dest, 0));
  queue->Enqueue (Create<PieQueueDiscTestItem> (p5, dest, 0));
  queue->Enqueue (Create<PieQueueDiscTestItem> (p6, dest, 0));
  queue->Enqueue (Create<PieQueueDiscTestItem> (p7, dest, 0));
  queue->Enqueue (Create<PieQueueDiscTestItem> (p8, dest, 0));
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 8 * modeSize, "There should be eight packets in there");

  Ptr<QueueDiscItem> item;

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the first packet");
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 7 * modeSize, "There should be seven packets in there");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p1->GetUid (), "was this the first packet ?");

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the second packet");
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 6 * modeSize, "There should be six packet in there");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p2->GetUid (), "Was this the second packet ?");

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the third packet");
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 5 * modeSize, "There should be five packets in there");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p3->GetUid (), "Was this the third packet ?");

  item = queue->Dequeue ();
  item = queue->Dequeue ();
  item = queue->Dequeue ();
  item = queue->Dequeue ();
  item = queue->Dequeue ();

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item == 0), true, "There are really no packets in there");


  // test 2: more data with defaults, unforced drops but no forced drops
  queue = CreateObject<PieQueueDisc> ();
  pktSize = 1000;  // pktSize != 0 because DequeueThreshold always works in bytes
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("A", DoubleValue (0.125)), true,
                         "Verify that we can actually set the attribute A");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("B", DoubleValue (1.25)), true,
                         "Verify that we can actually set the attribute B");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Tupdate", TimeValue (Seconds (0.03))), true,
                         "Verify that we can actually set the attribute Tupdate");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Supdate", TimeValue (Seconds (0.0))), true,
                         "Verify that we can actually set the attribute Supdate");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("DequeueThreshold", UintegerValue (10000)), true,
                         "Verify that we can actually set the attribute DequeueThreshold");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueDelayReference", TimeValue (Seconds (0.02))), true,
                         "Verify that we can actually set the attribute QueueDelayReference");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxBurstAllowance", TimeValue (Seconds (0.1))), true,
                         "Verify that we can actually set the attribute MaxBurstAllowance");
  queue->Initialize ();
  EnqueueWithDelay (queue, pktSize, 400);
  DequeueWithDelay (queue, 0.012, 400);
  Simulator::Stop (Seconds (8.0));
  Simulator::Run ();
  QueueDisc::Stats st = queue->GetStats ();
  uint32_t test2 = st.GetNDroppedPackets (PieQueueDisc::UNFORCED_DROP);
  NS_TEST_EXPECT_MSG_NE (test2, 0, "There should be some unforced drops");
  NS_TEST_EXPECT_MSG_EQ (st.GetNDroppedPackets (PieQueueDisc::FORCED_DROP), 0, "There should be zero forced drops");


  // test 3: same as test 2, but with higher QueueDelayReference
  queue = CreateObject<PieQueueDisc> ();
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("A", DoubleValue (0.125)), true,
                         "Verify that we can actually set the attribute A");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("B", DoubleValue (1.25)), true,
                         "Verify that we can actually set the attribute B");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Tupdate", TimeValue (Seconds (0.03))), true,
                         "Verify that we can actually set the attribute Tupdate");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Supdate", TimeValue (Seconds (0.0))), true,
                         "Verify that we can actually set the attribute Supdate");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("DequeueThreshold", UintegerValue (10000)), true,
                         "Verify that we can actually set the attribute DequeueThreshold");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueDelayReference", TimeValue (Seconds (0.08))), true,
                         "Verify that we can actually set the attribute QueueDelayReference");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxBurstAllowance", TimeValue (Seconds (0.1))), true,
                         "Verify that we can actually set the attribute MaxBurstAllowance");
  queue->Initialize ();
  EnqueueWithDelay (queue, pktSize, 400);
  DequeueWithDelay (queue, 0.012, 400);
  Simulator::Stop (Seconds (8.0));
  Simulator::Run ();
  st = queue->GetStats ();
  uint32_t test3 = st.GetNDroppedPackets (PieQueueDisc::UNFORCED_DROP);
  NS_TEST_EXPECT_MSG_LT (test3, test2, "Test 3 should have less unforced drops than test 2");
  NS_TEST_EXPECT_MSG_EQ (st.GetNDroppedPackets (PieQueueDisc::FORCED_DROP), 0, "There should be zero forced drops");


  // test 4: same as test 2, but with reduced dequeue rate
  queue = CreateObject<PieQueueDisc> ();
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("A", DoubleValue (0.125)), true,
                         "Verify that we can actually set the attribute A");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("B", DoubleValue (1.25)), true,
                         "Verify that we can actually set the attribute B");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Tupdate", TimeValue (Seconds (0.03))), true,
                         "Verify that we can actually set the attribute Tupdate");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Supdate", TimeValue (Seconds (0.0))), true,
                         "Verify that we can actually set the attribute Supdate");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("DequeueThreshold", UintegerValue (10000)), true,
                         "Verify that we can actually set the attribute DequeueThreshold");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueDelayReference", TimeValue (Seconds (0.02))), true,
                         "Verify that we can actually set the attribute QueueDelayReference");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxBurstAllowance", TimeValue (Seconds (0.1))), true,
                         "Verify that we can actually set the attribute MaxBurstAllowance");
  queue->Initialize ();
  EnqueueWithDelay (queue, pktSize, 400);
  DequeueWithDelay (queue, 0.015, 400); // delay between two successive dequeue events is increased
  Simulator::Stop (Seconds (8.0));
  Simulator::Run ();
  st = queue->GetStats ();
  uint32_t test4 = st.GetNDroppedPackets (PieQueueDisc::UNFORCED_DROP);
  NS_TEST_EXPECT_MSG_GT (test4, test2, "Test 4 should have more unforced drops than test 2");
  NS_TEST_EXPECT_MSG_EQ (st.GetNDroppedPackets (PieQueueDisc::FORCED_DROP), 0, "There should be zero forced drops");


  // test 5: same dequeue rate as test 4, but with higher Tupdate
  queue = CreateObject<PieQueueDisc> ();
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueLimit", UintegerValue (qSize)), true,
                         "Verify that we can actually set the attribute QueueLimit");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("A", DoubleValue (0.125)), true,
                         "Verify that we can actually set the attribute A");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("B", DoubleValue (1.25)), true,
                         "Verify that we can actually set the attribute B");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Tupdate", TimeValue (Seconds (0.09))), true,
                         "Verify that we can actually set the attribute Tupdate");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Supdate", TimeValue (Seconds (0.0))), true,
                         "Verify that we can actually set the attribute Supdate");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("DequeueThreshold", UintegerValue (10000)), true,
                         "Verify that we can actually set the attribute DequeueThreshold");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueDelayReference", TimeValue (Seconds (0.02))), true,
                         "Verify that we can actually set the attribute QueueDelayReference");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxBurstAllowance", TimeValue (Seconds (0.1))), true,
                         "Verify that we can actually set the attribute MaxBurstAllowance");
  queue->Initialize ();
  EnqueueWithDelay (queue, pktSize, 400);
  DequeueWithDelay (queue, 0.015, 400);
  Simulator::Stop (Seconds (8.0));
  Simulator::Run ();
  st = queue->GetStats ();
  uint32_t test5 = st.GetNDroppedPackets (PieQueueDisc::UNFORCED_DROP);
  NS_TEST_EXPECT_MSG_LT (test5, test4, "Test 5 should have less unforced drops than test 4");
  NS_TEST_EXPECT_MSG_EQ (st.GetNDroppedPackets (PieQueueDisc::FORCED_DROP), 0, "There should be zero forced drops");
}

void
PieQueueDiscTestCase::Enqueue (Ptr<PieQueueDisc> queue, uint32_t size, uint32_t nPkt)
{
  Address dest;
  for (uint32_t i = 0; i < nPkt; i++)
    {
      queue->Enqueue (Create<PieQueueDiscTestItem> (Create<Packet> (size), dest, 0));
    }
}

void
PieQueueDiscTestCase::EnqueueWithDelay (Ptr<PieQueueDisc> queue, uint32_t size, uint32_t nPkt)
{
  Address dest;
  double delay = 0.01;  // enqueue packets with delay
  for (uint32_t i = 0; i < nPkt; i++)
    {
      Simulator::Schedule (Time (Seconds ((i + 1) * delay)), &PieQueueDiscTestCase::Enqueue, this, queue, size, 1);
    }
}

void
PieQueueDiscTestCase::Dequeue (Ptr<PieQueueDisc> queue, uint32_t nPkt)
{
  for (uint32_t i = 0; i < nPkt; i++)
    {
      Ptr<QueueDiscItem> item = queue->Dequeue ();
    }
}

void
PieQueueDiscTestCase::DequeueWithDelay (Ptr<PieQueueDisc> queue, double delay, uint32_t nPkt)
{
  for (uint32_t i = 0; i < nPkt; i++)
    {
      Simulator::Schedule (Time (Seconds ((i + 1) * delay)), &PieQueueDiscTestCase::Dequeue, this, queue, 1);
    }
}

void
PieQueueDiscTestCase::DoRun (void)
{
  RunPieTest (StringValue ("QUEUE_DISC_MODE_PACKETS"));
  RunPieTest (StringValue ("QUEUE_DISC_MODE_BYTES"));
  Simulator::Destroy ();
}

/**
 * \ingroup traffic-control-test
 * \ingroup tests
 *
 * \brief Pie Queue Disc Test Suite
 */
static class PieQueueDiscTestSuite : public TestSuite
{
public:
  PieQueueDiscTestSuite ()
    : TestSuite ("pie-queue-disc", UNIT)
  {
    AddTestCase (new PieQueueDiscTestCase (), TestCase::QUICK);
  }
} g_pieQueueTestSuite; ///< the test suite
