/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 ResiliNets, ITTC, University of Kansas
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
 * Author: Truc Anh N Nguyen <trucanh524@gmail.com>
 *
 */

#include "ns3/test.h"
#include "ns3/codel-queue.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/double.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/network-module.h"
#include "ns3/core-module.h"

using namespace ns3;

// The following code borrowed from Linux codel.h, for unit testing
#define REC_INV_SQRT_BITS_ns3 (8 * sizeof(uint16_t))
/* or sizeof_in_bits(rec_inv_sqrt) */
/* needed shift to get a Q0.32 number from rec_inv_sqrt */
#define REC_INV_SQRT_SHIFT_ns3 (32 - REC_INV_SQRT_BITS_ns3)

static uint16_t _codel_Newton_step (uint32_t count, uint16_t rec_inv_sqrt)
{
  uint32_t invsqrt = ((uint32_t)rec_inv_sqrt) << REC_INV_SQRT_SHIFT_ns3;
  uint32_t invsqrt2 = ((uint64_t)invsqrt * invsqrt) >> 32;
  uint64_t val = (3LL << 32) - ((uint64_t)count * invsqrt2);

  val >>= 2; /* avoid overflow in following multiply */
  val = (val * invsqrt) >> (32 - 2 + 1);
  return (val >> REC_INV_SQRT_SHIFT_ns3);
}

static uint32_t _reciprocal_scale (uint32_t val, uint32_t ep_ro)
{
  return (uint32_t)(((uint64_t)val * ep_ro) >> 32);
}
// End Linux borrow


// Test 1: simple enqueue/dequeue with no drops
class CoDelQueueBasicEnqueueDequeue : public TestCase
{
public:
  CoDelQueueBasicEnqueueDequeue (std::string mode);
  virtual void DoRun (void);

  void QueueTestSize (Ptr<CoDelQueue> queue, uint32_t size, std::string error)
  {
    if (queue->GetMode () == CoDelQueue::QUEUE_MODE_BYTES)
      {
        NS_TEST_EXPECT_MSG_EQ (queue->GetNBytes (), size, error);
      }
    else if (queue->GetMode () == CoDelQueue::QUEUE_MODE_PACKETS)
      {
        NS_TEST_EXPECT_MSG_EQ (queue->GetNPackets (), size, error);
      }

    NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), size, error);
  }

private:
  StringValue m_mode;
};

CoDelQueueBasicEnqueueDequeue::CoDelQueueBasicEnqueueDequeue (std::string mode)
  : TestCase ("Basic enqueue and dequeue operations, and attribute setting for " + mode)
{
  m_mode = StringValue (mode);
}

void
CoDelQueueBasicEnqueueDequeue::DoRun (void)
{
  Ptr<CoDelQueue> queue = CreateObject<CoDelQueue> ();

  uint32_t pktSize = 1000;
  uint32_t modeSize = 0;

  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", m_mode), true,
                         "Verify that we can actually set the attribute Mode");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxPackets", UintegerValue (1500)), true,
                         "Verify that we can actually set the attribute MaxPackets");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxBytes", UintegerValue (pktSize * 1500)), true,
                         "Verify that we can actually set the attribute MaxBytes");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MinBytes", UintegerValue (pktSize)), true,
                         "Verify that we can actually set the attribute MinBytes");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Interval", StringValue ("50ms")), true,
                         "Verify that we can actually set the attribute Interval");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Target", StringValue ("4ms")), true,
                         "Verify that we can actually set the attribute Target");

  if (queue->GetMode () == CoDelQueue::QUEUE_MODE_BYTES)
    {
      modeSize = pktSize;
    }
  else if (queue->GetMode () == CoDelQueue::QUEUE_MODE_PACKETS)
    {
      modeSize = 1;
    }

  Ptr<Packet> p1, p2, p3, p4, p5, p6;
  p1 = Create<Packet> (pktSize);
  p2 = Create<Packet> (pktSize);
  p3 = Create<Packet> (pktSize);
  p4 = Create<Packet> (pktSize);
  p5 = Create<Packet> (pktSize);
  p6 = Create<Packet> (pktSize);

  QueueTestSize (queue, 0 * modeSize, "There should be no packets in queue");
  queue->Enqueue (Create<QueueItem> (p1));
  QueueTestSize (queue, 1 * modeSize, "There should be one packet in queue");
  queue->Enqueue (Create<QueueItem> (p2));
  QueueTestSize (queue, 2 * modeSize, "There should be two packets in queue");
  queue->Enqueue (Create<QueueItem> (p3));
  QueueTestSize (queue, 3 * modeSize, "There should be three packets in queue");
  queue->Enqueue (Create<QueueItem> (p4));
  QueueTestSize (queue, 4 * modeSize, "There should be four packets in queue");
  queue->Enqueue (Create<QueueItem> (p5));
  QueueTestSize (queue, 5 * modeSize, "There should be five packets in queue");
  queue->Enqueue (Create<QueueItem> (p6));
  QueueTestSize (queue, 6 * modeSize, "There should be six packets in queue");

  NS_TEST_EXPECT_MSG_EQ (queue->GetDropOverLimit (), 0, "There should be no packets being dropped due to full queue");

  Ptr<QueueItem> item;

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the first packet");
  QueueTestSize (queue, 5 * modeSize, "There should be five packets in queue");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p1->GetUid (), "was this the first packet ?");

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the second packet");
  QueueTestSize (queue, 4 * modeSize, "There should be four packets in queue");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p2->GetUid (), "Was this the second packet ?");

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the third packet");
  QueueTestSize (queue, 3 * modeSize, "There should be three packets in queue");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p3->GetUid (), "Was this the third packet ?");

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the forth packet");
  QueueTestSize (queue, 2 * modeSize, "There should be two packets in queue");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p4->GetUid (), "Was this the fourth packet ?");

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the fifth packet");
  QueueTestSize (queue, 1 * modeSize, "There should be one packet in queue");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p5->GetUid (), "Was this the fifth packet ?");

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the last packet");
  QueueTestSize (queue, 0 * modeSize, "There should be zero packet in queue");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p6->GetUid (), "Was this the sixth packet ?");

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item == 0), true, "There are really no packets in queue");

  NS_TEST_EXPECT_MSG_EQ (queue->GetDropCount (), 0, "There should be no packet drops according to CoDel algorithm");
}

// Test 2: enqueue with drops due to queue overflow
class CoDelQueueBasicOverflow : public TestCase
{
public:
  CoDelQueueBasicOverflow (std::string mode);
  virtual void DoRun (void);

  void QueueTestSize (Ptr<CoDelQueue> queue, uint32_t size, std::string error)
  {
    if (queue->GetMode () == CoDelQueue::QUEUE_MODE_BYTES)
      {
        NS_TEST_EXPECT_MSG_EQ (queue->GetNBytes (), size, error);
      }
    else if (queue->GetMode () == CoDelQueue::QUEUE_MODE_PACKETS)
      {
        NS_TEST_EXPECT_MSG_EQ (queue->GetNPackets (), size, error);
      }

    NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), size, error);
  }

private:
  void Enqueue (Ptr<CoDelQueue> queue, uint32_t size, uint32_t nPkt);
  StringValue m_mode;
};

CoDelQueueBasicOverflow::CoDelQueueBasicOverflow (std::string mode)
  : TestCase ("Basic overflow behavior for " + mode)
{
  m_mode = StringValue (mode);
}

void
CoDelQueueBasicOverflow::DoRun (void)
{
  Ptr<CoDelQueue> queue = CreateObject<CoDelQueue> ();
  uint32_t pktSize = 1000;
  uint32_t modeSize = 0;

  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", m_mode), true,
                         "Verify that we can actually set the attribute Mode");

  if (queue->GetMode () == CoDelQueue::QUEUE_MODE_BYTES)
    {
      modeSize = pktSize;
    }
  else if (queue->GetMode () == CoDelQueue::QUEUE_MODE_PACKETS)
    {
      modeSize = 1;
    }

  Ptr<Packet> p1, p2, p3;
  p1 = Create<Packet> (pktSize);
  p2 = Create<Packet> (pktSize);
  p3 = Create<Packet> (pktSize);

  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxPackets", UintegerValue (500)), true,
                         "Verify that we can actually set the attribute MaxPackets");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxBytes", UintegerValue (pktSize * 500)), true,
                         "Verify that we can actually set the attribute MaxBytes");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MinBytes", UintegerValue (pktSize)), true,
                         "Verify that we can actually set the attribute MinBytes");

  Enqueue (queue, pktSize, 500);
  queue->Enqueue (Create<QueueItem> (p1));
  queue->Enqueue (Create<QueueItem> (p2));
  queue->Enqueue (Create<QueueItem> (p3));

  QueueTestSize (queue, 500 * modeSize, "There should be 500 packets in queue");
  NS_TEST_EXPECT_MSG_EQ (queue->GetDropOverLimit (), 3, "There should be three packets being dropped due to full queue");
}

void
CoDelQueueBasicOverflow::Enqueue (Ptr<CoDelQueue> queue, uint32_t size, uint32_t nPkt)
{
  for (uint32_t i = 0; i < nPkt; i++)
    {
      queue->Enqueue (Create<QueueItem> (Create<Packet> (size)));
    }
}

// Test 3: NewtonStep unit test
// test against explicit port of Linux implementation
class CoDelQueueNewtonStepTest : public TestCase
{
public:
  CoDelQueueNewtonStepTest ();
  virtual void DoRun (void);
};

CoDelQueueNewtonStepTest::CoDelQueueNewtonStepTest ()
  : TestCase ("NewtonStep arithmetic unit test")
{
}

void
CoDelQueueNewtonStepTest::DoRun (void)
{
  Ptr<CoDelQueue> queue = CreateObject<CoDelQueue> ();

  // Spot check a few points in the expected operational range of
  // CoDelQueue's m_count and m_recInvSqrt variables
  uint32_t count = 2;
  uint16_t recInvSqrt = 65535;
  queue->m_count = count;
  queue->m_recInvSqrt = recInvSqrt;
  queue->NewtonStep ();
  // Test that ns-3 value is exactly the same as the Linux value
  NS_TEST_ASSERT_MSG_EQ (_codel_Newton_step (count, recInvSqrt), queue->m_recInvSqrt,
                         "ns-3 NewtonStep() fails to match Linux equivalent");

  count = 4;
  recInvSqrt = 36864;
  queue->m_count = count;
  queue->m_recInvSqrt = recInvSqrt;
  queue->NewtonStep ();
  // Test that ns-3 value is exactly the same as the Linux value
  NS_TEST_ASSERT_MSG_EQ (_codel_Newton_step (count, recInvSqrt), queue->m_recInvSqrt,
                         "ns-3 NewtonStep() fails to match Linux equivalent");
}

// Test 4: ControlLaw unit test
// test against explicit port of Linux implementation
class CoDelQueueControlLawTest : public TestCase
{
public:
  CoDelQueueControlLawTest ();
  virtual void DoRun (void);
  uint32_t _codel_control_law (Ptr<CoDelQueue> queue, uint32_t t);
};

CoDelQueueControlLawTest::CoDelQueueControlLawTest ()
  : TestCase ("ControlLaw arithmetic unit test")
{
}

// The following code borrowed from Linux codel.h,
// except the addition of queue parameter
uint32_t
CoDelQueueControlLawTest::_codel_control_law (Ptr<CoDelQueue> queue, uint32_t t)
{
  return t + _reciprocal_scale (queue->Time2CoDel (queue->m_interval), queue->m_recInvSqrt << REC_INV_SQRT_SHIFT_ns3);
}
// End Linux borrrow

void
CoDelQueueControlLawTest::DoRun (void)
{
  Ptr<CoDelQueue> queue = CreateObject<CoDelQueue> ();

  /* Spot check a few points of m_dropNext
   The integer approximations in Linux should be within
   2% of the true floating point value obtained in ns-3
   */
  uint32_t dropNextTestVals [4] = {292299, 341128, 9804717, 55885007};

  for (int i = 0; i < 4; ++i)
    {
      uint32_t ns3Result = queue->ControlLaw (dropNextTestVals[i]);
      uint32_t upperBound = ns3Result + 0.02 * ns3Result;
      uint32_t lowerBound = ns3Result - 0.02 * ns3Result;
      uint32_t linuxResult = _codel_control_law (queue, dropNextTestVals[i]);
      NS_TEST_EXPECT_MSG_EQ ((lowerBound < linuxResult || linuxResult < upperBound), true,
                             "Linux result should stay within 2% of ns-3 result");
    }
}

// Test 5: enqueue/dequeue with drops according to CoDel algorithm
class CoDelQueueBasicDrop : public TestCase
{
public:
  CoDelQueueBasicDrop (std::string mode);
  virtual void DoRun (void);

  void QueueTestSize (Ptr<CoDelQueue> queue, uint32_t size, std::string error)
  {
    if (queue->GetMode () == CoDelQueue::QUEUE_MODE_BYTES)
      {
        NS_TEST_EXPECT_MSG_EQ (queue->GetNBytes (), size, error);
      }
    else if (queue->GetMode () == CoDelQueue::QUEUE_MODE_PACKETS)
      {
        NS_TEST_EXPECT_MSG_EQ (queue->GetNPackets (), size, error);
      }

    NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), size, error);
  }

private:
  void Enqueue (Ptr<CoDelQueue> queue, uint32_t size, uint32_t nPkt);
  void Dequeue (Ptr<CoDelQueue> queue, uint32_t modeSize);
  void DropNextTracer (uint32_t oldVal, uint32_t newVal);
  StringValue m_mode;
  uint32_t m_dropNextCount;    //count the number of times m_dropNext is recalculated
};

CoDelQueueBasicDrop::CoDelQueueBasicDrop (std::string mode)
  : TestCase ("Basic drop operations for " + mode)
{
  m_mode = StringValue (mode);
  m_dropNextCount = 0;
}

void
CoDelQueueBasicDrop::DropNextTracer (uint32_t oldVal, uint32_t newVal)
{
  m_dropNextCount++;
}

void
CoDelQueueBasicDrop::DoRun (void)
{
  Ptr<CoDelQueue> queue = CreateObject<CoDelQueue> ();
  uint32_t pktSize = 1000;
  uint32_t modeSize = 0;

  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", m_mode), true,
                         "Verify that we can actually set the attribute Mode");

  if (queue->GetMode () == CoDelQueue::QUEUE_MODE_BYTES)
    {
      modeSize = pktSize;
    }
  else if (queue->GetMode () == CoDelQueue::QUEUE_MODE_PACKETS)
    {
      modeSize = 1;
    }

  Enqueue (queue, pktSize, 20);
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 20 * modeSize, "There should be 20 packets in queue.");

  // Although the first dequeue occurs with a sojourn time above target
  // the dequeue should be successful in this interval
  Time waitUntilFirstDequeue =  2 * queue->GetTarget ();
  Simulator::Schedule (waitUntilFirstDequeue, &CoDelQueueBasicDrop::Dequeue, this, queue, modeSize);

  // This dequeue should cause a drop
  Time waitUntilSecondDequeue = waitUntilFirstDequeue + 2 * queue->GetInterval ();
  Simulator::Schedule (waitUntilSecondDequeue, &CoDelQueueBasicDrop::Dequeue, this, queue, modeSize);

  // Although we are in dropping state, it's not time for next drop
  // the dequeue should not cause a drop
  Simulator::Schedule (waitUntilSecondDequeue, &CoDelQueueBasicDrop::Dequeue, this, queue, modeSize);

  // In dropping time and it's time for next drop
  // the dequeue should cause additional packet drops
  Simulator::Schedule (waitUntilSecondDequeue * 2, &CoDelQueueBasicDrop::Dequeue, this, queue, modeSize);

  Simulator::Run ();
  Simulator::Destroy ();
}

void
CoDelQueueBasicDrop::Enqueue (Ptr<CoDelQueue> queue, uint32_t size, uint32_t nPkt)
{
  for (uint32_t i = 0; i < nPkt; i++)
    {
      queue->Enqueue (Create<QueueItem> (Create<Packet> (size)));
    }
}

void
CoDelQueueBasicDrop::Dequeue (Ptr<CoDelQueue> queue, uint32_t modeSize)
{
  uint32_t initialDropCount = queue->GetDropCount ();
  uint32_t initialQSize = queue->GetQueueSize ();
  uint32_t initialDropNext = queue->GetDropNext ();
  Time currentTime = Simulator::Now ();
  uint32_t currentDropCount = 0;

  if (initialDropCount > 0 && currentTime.GetMicroSeconds () >= initialDropNext)
    {
      queue->TraceConnectWithoutContext ("DropNext", MakeCallback (&CoDelQueueBasicDrop::DropNextTracer, this));
    }

  if (initialQSize != 0)
    {
      Ptr<QueueItem> item = queue->Dequeue ();
      if (initialDropCount == 0 && currentTime > queue->GetTarget ())
        {
          if (currentTime < queue->GetInterval ())
            {
              currentDropCount = queue->GetDropCount ();
              NS_TEST_EXPECT_MSG_EQ (currentDropCount, 0, "We are not in dropping state."
                                     "Sojourn time has just gone above target from below."
                                     "Hence, there should be no packet drops");
              QueueTestSize (queue, initialQSize - modeSize, "There should be 1 packet dequeued.");

            }
          else if (currentTime >= queue->GetInterval ())
            {
              currentDropCount = queue->GetDropCount ();
              QueueTestSize (queue, initialQSize - 2 * modeSize, "Sojourn time has been above target for at least interval."
                                     "We enter the dropping state, perform initial packet drop, and dequeue the next."
                                     "So there should be 2 more packets dequeued.");
              NS_TEST_EXPECT_MSG_EQ (currentDropCount, 1, "There should be 1 packet drop");
            }
        }
      else if (initialDropCount > 0)
        { // In dropping state
          if (currentTime.GetMicroSeconds () < initialDropNext)
            {
              currentDropCount = queue->GetDropCount ();
              QueueTestSize (queue, initialQSize - modeSize, "We are in dropping state."
                                     "Sojourn is still above target."
                                     "However, it's not time for next drop."
                                     "So there should be only 1 more packet dequeued");

              NS_TEST_EXPECT_MSG_EQ (currentDropCount, 1, "There should still be only 1 packet drop from the last dequeue");
            }
          else if (currentTime.GetMicroSeconds () >= initialDropNext)
            {
              currentDropCount = queue->GetDropCount ();
              QueueTestSize (queue, initialQSize - (m_dropNextCount + 1) * modeSize, "We are in dropping state."
                                     "It's time for next drop."
                                     "The number of packets dequeued equals to the number of times m_dropNext is updated plus initial dequeue");
              NS_TEST_EXPECT_MSG_EQ (currentDropCount, 1 + m_dropNextCount, "The number of drops equals to the number of times m_dropNext is updated plus 1 from last dequeue");
            }
        }
    }
}

static class CoDelQueueTestSuite : public TestSuite
{
public:
  CoDelQueueTestSuite ()
    : TestSuite ("codel-queue", UNIT)
  {
    // Test 1: simple enqueue/dequeue with no drops
    AddTestCase (new CoDelQueueBasicEnqueueDequeue ("QUEUE_MODE_PACKETS"), TestCase::QUICK);
    AddTestCase (new CoDelQueueBasicEnqueueDequeue ("QUEUE_MODE_BYTES"), TestCase::QUICK);
    // Test 2: enqueue with drops due to queue overflow
    AddTestCase (new CoDelQueueBasicOverflow ("QUEUE_MODE_PACKETS"), TestCase::QUICK);
    AddTestCase (new CoDelQueueBasicOverflow ("QUEUE_MODE_BYTES"), TestCase::QUICK);
    // Test 3: test NewtonStep() against explicit port of Linux implementation
    AddTestCase (new CoDelQueueNewtonStepTest (), TestCase::QUICK);
    // Test 4: test ControlLaw() against explicit port of Linux implementation
    AddTestCase (new CoDelQueueControlLawTest (), TestCase::QUICK);
    // Test 5: enqueue/dequeue with drops according to CoDel algorithm
    AddTestCase (new CoDelQueueBasicDrop ("QUEUE_MODE_PACKETS"), TestCase::QUICK);
    AddTestCase (new CoDelQueueBasicDrop ("QUEUE_MODE_PACKETS"), TestCase::QUICK);
  }
} g_coDelQueueTestSuite;
