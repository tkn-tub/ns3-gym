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
 * Modified by:   Pasquale Imputato <p.imputato@gmail.com>
 *
 */

#include "ns3/test.h"
#include "ns3/codel-queue-disc.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/double.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

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

/**
 * \ingroup traffic-control-test
 * \ingroup tests
 *
 * \brief Codel Queue Disc Test Item
 */
class CodelQueueDiscTestItem : public QueueDiscItem {
public:
  /**
   * Constructor
   *
   * \param p packet
   * \param addr address
   * \param protocol
   */
  CodelQueueDiscTestItem (Ptr<Packet> p, const Address & addr, uint16_t protocol);
  virtual ~CodelQueueDiscTestItem ();
  virtual void AddHeader (void);
  virtual bool Mark(void);

private:
  CodelQueueDiscTestItem ();
  /**
   * \brief Copy constructor
   * Disable default implementation to avoid misuse
   */
  CodelQueueDiscTestItem (const CodelQueueDiscTestItem &);
  /**
   * \brief Assignment operator
   * \return this object
   * Disable default implementation to avoid misuse
   */
  CodelQueueDiscTestItem &operator = (const CodelQueueDiscTestItem &);
};

CodelQueueDiscTestItem::CodelQueueDiscTestItem (Ptr<Packet> p, const Address & addr, uint16_t protocol)
  : QueueDiscItem (p, addr, protocol)
{
}

CodelQueueDiscTestItem::~CodelQueueDiscTestItem ()
{
}

void
CodelQueueDiscTestItem::AddHeader (void)
{
}

bool
CodelQueueDiscTestItem::Mark (void)
{
  return false;
}

/**
 * \ingroup traffic-control-test
 * \ingroup tests
 *
 * \brief Test 1: simple enqueue/dequeue with no drops
 */
class CoDelQueueDiscBasicEnqueueDequeue : public TestCase
{
public:
  /**
   * Constructor
   *
   * \param mode the mode
   */
  CoDelQueueDiscBasicEnqueueDequeue (std::string mode);
  virtual void DoRun (void);

  /**
   * Queue test size function
   * \param queue the queue disc
   * \param size the size
   * \param error the error string
   */
  void QueueTestSize (Ptr<CoDelQueueDisc> queue, uint32_t size, std::string error)
  {
    if (queue->GetMode () == CoDelQueueDisc::QUEUE_DISC_MODE_BYTES)
      {
        NS_TEST_EXPECT_MSG_EQ (queue->GetNBytes (), size, error);
      }
    else if (queue->GetMode () == CoDelQueueDisc::QUEUE_DISC_MODE_PACKETS)
      {
        NS_TEST_EXPECT_MSG_EQ (queue->GetNPackets (), size, error);
      }

    NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), size, error);
  }

private:
  StringValue m_mode; ///< mode
};

CoDelQueueDiscBasicEnqueueDequeue::CoDelQueueDiscBasicEnqueueDequeue (std::string mode)
  : TestCase ("Basic enqueue and dequeue operations, and attribute setting for " + mode)
{
  m_mode = StringValue (mode);
}

void
CoDelQueueDiscBasicEnqueueDequeue::DoRun (void)
{
  Ptr<CoDelQueueDisc> queue = CreateObject<CoDelQueueDisc> ();

  uint32_t pktSize = 1000;
  uint32_t modeSize = 0;
  
  Address dest;

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

  if (queue->GetMode () == CoDelQueueDisc::QUEUE_DISC_MODE_BYTES)
    {
      modeSize = pktSize;
    }
  else if (queue->GetMode () == CoDelQueueDisc::QUEUE_DISC_MODE_PACKETS)
    {
      modeSize = 1;
    }
  queue->Initialize ();

  Ptr<Packet> p1, p2, p3, p4, p5, p6;
  p1 = Create<Packet> (pktSize);
  p2 = Create<Packet> (pktSize);
  p3 = Create<Packet> (pktSize);
  p4 = Create<Packet> (pktSize);
  p5 = Create<Packet> (pktSize);
  p6 = Create<Packet> (pktSize);

  QueueTestSize (queue, 0 * modeSize, "There should be no packets in queue");
  queue->Enqueue (Create<CodelQueueDiscTestItem> (p1, dest, 0));
  QueueTestSize (queue, 1 * modeSize, "There should be one packet in queue");
  queue->Enqueue (Create<CodelQueueDiscTestItem> (p2, dest, 0));
  QueueTestSize (queue, 2 * modeSize, "There should be two packets in queue");
  queue->Enqueue (Create<CodelQueueDiscTestItem> (p3, dest, 0));
  QueueTestSize (queue, 3 * modeSize, "There should be three packets in queue");
  queue->Enqueue (Create<CodelQueueDiscTestItem> (p4, dest, 0));
  QueueTestSize (queue, 4 * modeSize, "There should be four packets in queue");
  queue->Enqueue (Create<CodelQueueDiscTestItem> (p5, dest, 0));
  QueueTestSize (queue, 5 * modeSize, "There should be five packets in queue");
  queue->Enqueue (Create<CodelQueueDiscTestItem> (p6, dest, 0));
  QueueTestSize (queue, 6 * modeSize, "There should be six packets in queue");

  NS_TEST_EXPECT_MSG_EQ (queue->GetStats ().GetNDroppedPackets (CoDelQueueDisc::OVERLIMIT_DROP),
                         0, "There should be no packets being dropped due to full queue");

  Ptr<QueueDiscItem> item;

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

  NS_TEST_EXPECT_MSG_EQ (queue->GetStats ().GetNDroppedPackets (CoDelQueueDisc::TARGET_EXCEEDED_DROP), 0,
                         "There should be no packet drops according to CoDel algorithm");
}

/**
 * \ingroup traffic-control-test
 * \ingroup tests
 *
 * \brief Test 2: enqueue with drops due to queue overflow
 */
class CoDelQueueDiscBasicOverflow : public TestCase
{
public:
  /**
   * Constructor
   *
   * \param mode the mode
   */
  CoDelQueueDiscBasicOverflow (std::string mode);
  virtual void DoRun (void);

  /**
   * Queue test size function
   * \param queue the queue disc
   * \param size the size
   * \param error the error string
   */
  void QueueTestSize (Ptr<CoDelQueueDisc> queue, uint32_t size, std::string error)
  {
    if (queue->GetMode () == CoDelQueueDisc::QUEUE_DISC_MODE_BYTES)
      {
        NS_TEST_EXPECT_MSG_EQ (queue->GetNBytes (), size, error);
      }
    else if (queue->GetMode () == CoDelQueueDisc::QUEUE_DISC_MODE_PACKETS)
      {
        NS_TEST_EXPECT_MSG_EQ (queue->GetNPackets (), size, error);
      }

    NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), size, error);
  }

private:
  /**
   * Enqueue function
   * \param queue the queue disc
   * \param size the size
   * \param nPkt the number of packets
   */
  void Enqueue (Ptr<CoDelQueueDisc> queue, uint32_t size, uint32_t nPkt);
  StringValue m_mode; ///< mode
};

CoDelQueueDiscBasicOverflow::CoDelQueueDiscBasicOverflow (std::string mode)
  : TestCase ("Basic overflow behavior for " + mode)
{
  m_mode = StringValue (mode);
}

void
CoDelQueueDiscBasicOverflow::DoRun (void)
{
  Ptr<CoDelQueueDisc> queue = CreateObject<CoDelQueueDisc> ();
  uint32_t pktSize = 1000;
  uint32_t modeSize = 0;

  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", m_mode), true,
                         "Verify that we can actually set the attribute Mode");

  Address dest;

  if (queue->GetMode () == CoDelQueueDisc::QUEUE_DISC_MODE_BYTES)
    {
      modeSize = pktSize;
    }
  else if (queue->GetMode () == CoDelQueueDisc::QUEUE_DISC_MODE_PACKETS)
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

  queue->Initialize ();

  Enqueue (queue, pktSize, 500);
  queue->Enqueue (Create<CodelQueueDiscTestItem> (p1, dest, 0));
  queue->Enqueue (Create<CodelQueueDiscTestItem> (p2, dest, 0));
  queue->Enqueue (Create<CodelQueueDiscTestItem> (p3, dest, 0));

  QueueTestSize (queue, 500 * modeSize, "There should be 500 packets in queue");
  NS_TEST_EXPECT_MSG_EQ (queue->GetStats ().GetNDroppedPackets (CoDelQueueDisc::OVERLIMIT_DROP),
                         3, "There should be three packets being dropped due to full queue");
}

void
CoDelQueueDiscBasicOverflow::Enqueue (Ptr<CoDelQueueDisc> queue, uint32_t size, uint32_t nPkt)
{
  Address dest;
  for (uint32_t i = 0; i < nPkt; i++)
    {
      queue->Enqueue (Create<CodelQueueDiscTestItem> (Create<Packet> (size), dest, 0));
    }
}

/**
 * \ingroup traffic-control-test
 * \ingroup tests
 *
 * \brief Test 3: NewtonStep unit test - test against explicit port of Linux implementation
 */
class CoDelQueueDiscNewtonStepTest : public TestCase
{
public:
  CoDelQueueDiscNewtonStepTest ();
  virtual void DoRun (void);
};

CoDelQueueDiscNewtonStepTest::CoDelQueueDiscNewtonStepTest ()
  : TestCase ("NewtonStep arithmetic unit test")
{
}

void
CoDelQueueDiscNewtonStepTest::DoRun (void)
{
  Ptr<CoDelQueueDisc> queue = CreateObject<CoDelQueueDisc> ();

  // Spot check a few points in the expected operational range of
  // CoDelQueueDisc's m_count and m_recInvSqrt variables
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

/**
 * \ingroup traffic-control-test
 * \ingroup tests
 *
 * \brief Test 4: ControlLaw unit test - test against explicit port of Linux implementation
 */
class CoDelQueueDiscControlLawTest : public TestCase
{
public:
  CoDelQueueDiscControlLawTest ();
  virtual void DoRun (void);
  /**
   * Codel control law function
   * \param queue the queue disc
   * \param t
   * \returns the codel control law
   */
  uint32_t _codel_control_law (Ptr<CoDelQueueDisc> queue, uint32_t t);
};

CoDelQueueDiscControlLawTest::CoDelQueueDiscControlLawTest ()
  : TestCase ("ControlLaw arithmetic unit test")
{
}

// The following code borrowed from Linux codel.h,
// except the addition of queue parameter
uint32_t
CoDelQueueDiscControlLawTest::_codel_control_law (Ptr<CoDelQueueDisc> queue, uint32_t t)
{
  return t + _reciprocal_scale (queue->Time2CoDel (queue->m_interval), queue->m_recInvSqrt << REC_INV_SQRT_SHIFT_ns3);
}
// End Linux borrrow

void
CoDelQueueDiscControlLawTest::DoRun (void)
{
  Ptr<CoDelQueueDisc> queue = CreateObject<CoDelQueueDisc> ();

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

/**
 * \ingroup traffic-control-test
 * \ingroup tests
 *
 * \brief Test 5: enqueue/dequeue with drops according to CoDel algorithm
 */
class CoDelQueueDiscBasicDrop : public TestCase
{
public:
  /**
   * Constructor
   *
   * \param mode the mode
   */
  CoDelQueueDiscBasicDrop (std::string mode);
  virtual void DoRun (void);

  /**
   * Queue test size function
   * \param queue the queue disc
   * \param size the size
   * \param error the error string
   */
  void QueueTestSize (Ptr<CoDelQueueDisc> queue, uint32_t size, std::string error)
  {
    if (queue->GetMode () == CoDelQueueDisc::QUEUE_DISC_MODE_BYTES)
      {
        NS_TEST_EXPECT_MSG_EQ (queue->GetNBytes (), size, error);
      }
    else if (queue->GetMode () == CoDelQueueDisc::QUEUE_DISC_MODE_PACKETS)
      {
        NS_TEST_EXPECT_MSG_EQ (queue->GetNPackets (), size, error);
      }

    NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), size, error);
  }

private:
  /**
   * Enqueue function
   * \param queue the queue disc
   * \param size the size
   * \param nPkt the number of packets
   */
  void Enqueue (Ptr<CoDelQueueDisc> queue, uint32_t size, uint32_t nPkt);
  /** Dequeue function
   * \param queue the queue disc
   * \param modeSize the mode size
   */
  void Dequeue (Ptr<CoDelQueueDisc> queue, uint32_t modeSize);
  /**
   * Drop next tracer function
   * \param oldVal the old value
   * \param newVal the new value
   */
  void DropNextTracer (uint32_t oldVal, uint32_t newVal);
  StringValue m_mode; ///< mode
  uint32_t m_dropNextCount;    ///< count the number of times m_dropNext is recalculated
};

CoDelQueueDiscBasicDrop::CoDelQueueDiscBasicDrop (std::string mode)
  : TestCase ("Basic drop operations for " + mode)
{
  m_mode = StringValue (mode);
  m_dropNextCount = 0;
}

void
CoDelQueueDiscBasicDrop::DropNextTracer (uint32_t oldVal, uint32_t newVal)
{
  m_dropNextCount++;
}

void
CoDelQueueDiscBasicDrop::DoRun (void)
{
  Ptr<CoDelQueueDisc> queue = CreateObject<CoDelQueueDisc> ();
  uint32_t pktSize = 1000;
  uint32_t modeSize = 0;

  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("Mode", m_mode), true,
                         "Verify that we can actually set the attribute Mode");
  
  if (queue->GetMode () == CoDelQueueDisc::QUEUE_DISC_MODE_BYTES)
    {
      modeSize = pktSize;
    }
  else if (queue->GetMode () == CoDelQueueDisc::QUEUE_DISC_MODE_PACKETS)
    {
      modeSize = 1;
    }

  queue->Initialize ();

  Enqueue (queue, pktSize, 20);
  NS_TEST_EXPECT_MSG_EQ (queue->GetQueueSize (), 20 * modeSize, "There should be 20 packets in queue.");

  // Although the first dequeue occurs with a sojourn time above target
  // the dequeue should be successful in this interval
  Time waitUntilFirstDequeue =  2 * queue->GetTarget ();
  Simulator::Schedule (waitUntilFirstDequeue, &CoDelQueueDiscBasicDrop::Dequeue, this, queue, modeSize);

  // This dequeue should cause a drop
  Time waitUntilSecondDequeue = waitUntilFirstDequeue + 2 * queue->GetInterval ();
  Simulator::Schedule (waitUntilSecondDequeue, &CoDelQueueDiscBasicDrop::Dequeue, this, queue, modeSize);

  // Although we are in dropping state, it's not time for next drop
  // the dequeue should not cause a drop
  Simulator::Schedule (waitUntilSecondDequeue, &CoDelQueueDiscBasicDrop::Dequeue, this, queue, modeSize);

  // In dropping time and it's time for next drop
  // the dequeue should cause additional packet drops
  Simulator::Schedule (waitUntilSecondDequeue * 2, &CoDelQueueDiscBasicDrop::Dequeue, this, queue, modeSize);

  Simulator::Run ();
  Simulator::Destroy ();
}

void
CoDelQueueDiscBasicDrop::Enqueue (Ptr<CoDelQueueDisc> queue, uint32_t size, uint32_t nPkt)
{
  Address dest;
  for (uint32_t i = 0; i < nPkt; i++)
    {
      queue->Enqueue (Create<CodelQueueDiscTestItem> (Create<Packet> (size), dest, 0));
    }
}

void
CoDelQueueDiscBasicDrop::Dequeue (Ptr<CoDelQueueDisc> queue, uint32_t modeSize)
{
  uint32_t initialDropCount = queue->GetStats ().GetNDroppedPackets (CoDelQueueDisc::TARGET_EXCEEDED_DROP);
  uint32_t initialQSize = queue->GetQueueSize ();
  uint32_t initialDropNext = queue->GetDropNext ();
  Time currentTime = Simulator::Now ();
  uint32_t currentDropCount = 0;

  if (initialDropCount > 0 && currentTime.GetMicroSeconds () >= initialDropNext)
    {
      queue->TraceConnectWithoutContext ("DropNext", MakeCallback (&CoDelQueueDiscBasicDrop::DropNextTracer, this));
    }

  if (initialQSize != 0)
    {
      Ptr<QueueDiscItem> item = queue->Dequeue ();
      if (initialDropCount == 0 && currentTime > queue->GetTarget ())
        {
          if (currentTime < queue->GetInterval ())
            {
              currentDropCount = queue->GetStats ().GetNDroppedPackets (CoDelQueueDisc::TARGET_EXCEEDED_DROP);
              NS_TEST_EXPECT_MSG_EQ (currentDropCount, 0, "We are not in dropping state."
                                     "Sojourn time has just gone above target from below."
                                     "Hence, there should be no packet drops");
              QueueTestSize (queue, initialQSize - modeSize, "There should be 1 packet dequeued.");

            }
          else if (currentTime >= queue->GetInterval ())
            {
              currentDropCount = queue->GetStats ().GetNDroppedPackets (CoDelQueueDisc::TARGET_EXCEEDED_DROP);
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
              currentDropCount = queue->GetStats ().GetNDroppedPackets (CoDelQueueDisc::TARGET_EXCEEDED_DROP);
              QueueTestSize (queue, initialQSize - modeSize, "We are in dropping state."
                                     "Sojourn is still above target."
                                     "However, it's not time for next drop."
                                     "So there should be only 1 more packet dequeued");

              NS_TEST_EXPECT_MSG_EQ (currentDropCount, 1, "There should still be only 1 packet drop from the last dequeue");
            }
          else if (currentTime.GetMicroSeconds () >= initialDropNext)
            {
              currentDropCount = queue->GetStats ().GetNDroppedPackets (CoDelQueueDisc::TARGET_EXCEEDED_DROP);
              QueueTestSize (queue, initialQSize - (m_dropNextCount + 1) * modeSize, "We are in dropping state."
                                     "It's time for next drop."
                                     "The number of packets dequeued equals to the number of times m_dropNext is updated plus initial dequeue");
              NS_TEST_EXPECT_MSG_EQ (currentDropCount, 1 + m_dropNextCount, "The number of drops equals to the number of times m_dropNext is updated plus 1 from last dequeue");
            }
        }
    }
}

/**
 * \ingroup traffic-control-test
 * \ingroup tests
 *
 * \brief CoDel Queue Disc Test Suite
 */
static class CoDelQueueDiscTestSuite : public TestSuite
{
public:
  CoDelQueueDiscTestSuite ()
    : TestSuite ("codel-queue-disc", UNIT)
  {
    // Test 1: simple enqueue/dequeue with no drops
    AddTestCase (new CoDelQueueDiscBasicEnqueueDequeue ("QUEUE_DISC_MODE_PACKETS"), TestCase::QUICK);
    AddTestCase (new CoDelQueueDiscBasicEnqueueDequeue ("QUEUE_DISC_MODE_BYTES"), TestCase::QUICK);
    // Test 2: enqueue with drops due to queue overflow
    AddTestCase (new CoDelQueueDiscBasicOverflow ("QUEUE_DISC_MODE_PACKETS"), TestCase::QUICK);
    AddTestCase (new CoDelQueueDiscBasicOverflow ("QUEUE_DISC_MODE_BYTES"), TestCase::QUICK);
    // Test 3: test NewtonStep() against explicit port of Linux implementation
    AddTestCase (new CoDelQueueDiscNewtonStepTest (), TestCase::QUICK);
    // Test 4: test ControlLaw() against explicit port of Linux implementation
    AddTestCase (new CoDelQueueDiscControlLawTest (), TestCase::QUICK);
    // Test 5: enqueue/dequeue with drops according to CoDel algorithm
    AddTestCase (new CoDelQueueDiscBasicDrop ("QUEUE_DISC_MODE_PACKETS"), TestCase::QUICK);
    AddTestCase (new CoDelQueueDiscBasicDrop ("QUEUE_DISC_MODE_BYTES"), TestCase::QUICK);
  }
} g_coDelQueueTestSuite; ///< the test suite
