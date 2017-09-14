/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Andrew McGregor
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
 * Codel, the COntrolled DELay Queueing discipline
 * Based on ns2 simulation code presented by Kathie Nichols
 *
 * This port based on linux kernel code by
 * Authors:	Dave Täht <d@taht.net>
 *		Eric Dumazet <edumazet@google.com>
 *
 * Ported to ns-3 by: Andrew McGregor <andrewmcgr@gmail.com>
*/

#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "ns3/abort.h"
#include "codel-queue-disc.h"
#include "ns3/object-factory.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/net-device-queue-interface.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("CoDelQueueDisc");

/**
 * Performs a reciprocal divide, similar to the
 * Linux kernel reciprocal_divide function
 * \param A numerator
 * \param R reciprocal of the denominator B
 * \return the value of A/B
 */
/* borrowed from the linux kernel */
static inline uint32_t ReciprocalDivide (uint32_t A, uint32_t R)
{
  return (uint32_t)(((uint64_t)A * R) >> 32);
}

/* end kernel borrowings */

/**
 * Returns the current time translated in CoDel time representation
 * \return the current time
 */
static uint32_t CoDelGetTime (void)
{
  Time time = Simulator::Now ();
  uint64_t ns = time.GetNanoSeconds ();

  return ns >> CODEL_SHIFT;
}


NS_OBJECT_ENSURE_REGISTERED (CoDelQueueDisc);

TypeId CoDelQueueDisc::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CoDelQueueDisc")
    .SetParent<QueueDisc> ()
    .SetGroupName ("TrafficControl")
    .AddConstructor<CoDelQueueDisc> ()
    .AddAttribute ("Mode",
                   "Whether to use Bytes (see MaxBytes) or Packets (see MaxPackets) as the maximum queue size metric.",
                   EnumValue (QUEUE_DISC_MODE_BYTES),
                   MakeEnumAccessor (&CoDelQueueDisc::SetMode),
                   MakeEnumChecker (QUEUE_DISC_MODE_BYTES, "QUEUE_DISC_MODE_BYTES",
                                    QUEUE_DISC_MODE_PACKETS, "QUEUE_DISC_MODE_PACKETS"))
    .AddAttribute ("MaxPackets",
                   "The maximum number of packets accepted by this CoDelQueueDisc.",
                   UintegerValue (DEFAULT_CODEL_LIMIT),
                   MakeUintegerAccessor (&CoDelQueueDisc::m_maxPackets),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MaxBytes",
                   "The maximum number of bytes accepted by this CoDelQueueDisc.",
                   UintegerValue (1500 * DEFAULT_CODEL_LIMIT),
                   MakeUintegerAccessor (&CoDelQueueDisc::m_maxBytes),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MinBytes",
                   "The CoDel algorithm minbytes parameter.",
                   UintegerValue (1500),
                   MakeUintegerAccessor (&CoDelQueueDisc::m_minBytes),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Interval",
                   "The CoDel algorithm interval",
                   StringValue ("100ms"),
                   MakeTimeAccessor (&CoDelQueueDisc::m_interval),
                   MakeTimeChecker ())
    .AddAttribute ("Target",
                   "The CoDel algorithm target queue delay",
                   StringValue ("5ms"),
                   MakeTimeAccessor (&CoDelQueueDisc::m_target),
                   MakeTimeChecker ())
    .AddTraceSource ("Count",
                     "CoDel count",
                     MakeTraceSourceAccessor (&CoDelQueueDisc::m_count),
                     "ns3::TracedValueCallback::Uint32")
    .AddTraceSource ("LastCount",
                     "CoDel lastcount",
                     MakeTraceSourceAccessor (&CoDelQueueDisc::m_lastCount),
                     "ns3::TracedValueCallback::Uint32")
    .AddTraceSource ("DropState",
                     "Dropping state",
                     MakeTraceSourceAccessor (&CoDelQueueDisc::m_dropping),
                     "ns3::TracedValueCallback::Bool")
    .AddTraceSource ("DropNext",
                     "Time until next packet drop",
                     MakeTraceSourceAccessor (&CoDelQueueDisc::m_dropNext),
                     "ns3::TracedValueCallback::Uint32")
  ;

  return tid;
}

CoDelQueueDisc::CoDelQueueDisc ()
  : QueueDisc (),
    m_maxBytes (),
    m_count (0),
    m_lastCount (0),
    m_dropping (false),
    m_recInvSqrt (~0U >> REC_INV_SQRT_SHIFT),
    m_firstAboveTime (0),
    m_dropNext (0),
    m_state1 (0),
    m_state2 (0),
    m_state3 (0),
    m_states (0)
{
  NS_LOG_FUNCTION (this);
}

CoDelQueueDisc::~CoDelQueueDisc ()
{
  NS_LOG_FUNCTION (this);
}

void
CoDelQueueDisc::NewtonStep (void)
{
  NS_LOG_FUNCTION (this);
  uint32_t invsqrt = ((uint32_t) m_recInvSqrt) << REC_INV_SQRT_SHIFT;
  uint32_t invsqrt2 = ((uint64_t) invsqrt * invsqrt) >> 32;
  uint64_t val = (3ll << 32) - ((uint64_t) m_count * invsqrt2);

  val >>= 2; /* avoid overflow */
  val = (val * invsqrt) >> (32 - 2 + 1);
  m_recInvSqrt = val >> REC_INV_SQRT_SHIFT;
}

uint32_t
CoDelQueueDisc::ControlLaw (uint32_t t)
{
  NS_LOG_FUNCTION (this);
  return t + ReciprocalDivide (Time2CoDel (m_interval), m_recInvSqrt << REC_INV_SQRT_SHIFT);
}

void
CoDelQueueDisc::SetMode (QueueDiscMode mode)
{
  NS_LOG_FUNCTION (mode);
  m_mode = mode;
}

CoDelQueueDisc::QueueDiscMode
CoDelQueueDisc::GetMode (void)
{
  NS_LOG_FUNCTION (this);
  return m_mode;
}

bool
CoDelQueueDisc::DoEnqueue (Ptr<QueueDiscItem> item)
{
  NS_LOG_FUNCTION (this << item);

  if (m_mode == QUEUE_DISC_MODE_PACKETS && (GetInternalQueue (0)->GetNPackets () + 1 > m_maxPackets))
    {
      NS_LOG_LOGIC ("Queue full (at max packets) -- dropping pkt");
      DropBeforeEnqueue (item, OVERLIMIT_DROP);
      return false;
    }

  if (m_mode == QUEUE_DISC_MODE_BYTES && (GetInternalQueue (0)->GetNBytes () + item->GetSize () > m_maxBytes))
    {
      NS_LOG_LOGIC ("Queue full (packet would exceed max bytes) -- dropping pkt");
      DropBeforeEnqueue (item, OVERLIMIT_DROP);
      return false;
    }

  bool retval = GetInternalQueue (0)->Enqueue (item);

  // If Queue::Enqueue fails, QueueDisc::DropBeforeEnqueue is called by the
  // internal queue because QueueDisc::AddInternalQueue sets the trace callback

  NS_LOG_LOGIC ("Number packets " << GetInternalQueue (0)->GetNPackets ());
  NS_LOG_LOGIC ("Number bytes " << GetInternalQueue (0)->GetNBytes ());

  return retval;
}

bool
CoDelQueueDisc::OkToDrop (Ptr<QueueDiscItem> item, uint32_t now)
{
  NS_LOG_FUNCTION (this);
  bool okToDrop;

  if (!item)
    {
      m_firstAboveTime = 0;
      return false;
    }

  Time delta = Simulator::Now () - item->GetTimeStamp ();
  NS_LOG_INFO ("Sojourn time " << delta.ToDouble (Time::MS) << "ms");
  uint32_t sojournTime = Time2CoDel (delta);

  if (CoDelTimeBefore (sojournTime, Time2CoDel (m_target))
      || GetInternalQueue (0)->GetNBytes () < m_minBytes)
    {
      // went below so we'll stay below for at least q->interval
      NS_LOG_LOGIC ("Sojourn time is below target or number of bytes in queue is less than minBytes; packet should not be dropped");
      m_firstAboveTime = 0;
      return false;
    }
  okToDrop = false;
  if (m_firstAboveTime == 0)
    {
      /* just went above from below. If we stay above
       * for at least q->interval we'll say it's ok to drop
       */
      NS_LOG_LOGIC ("Sojourn time has just gone above target from below, need to stay above for at least q->interval before packet can be dropped. ");
      m_firstAboveTime = now + Time2CoDel (m_interval);
    }
  else if (CoDelTimeAfter (now, m_firstAboveTime))
    {
      NS_LOG_LOGIC ("Sojourn time has been above target for at least q->interval; it's OK to (possibly) drop packet.");
      okToDrop = true;
      ++m_state1;
    }
  return okToDrop;
}

Ptr<QueueDiscItem>
CoDelQueueDisc::DoDequeue (void)
{
  NS_LOG_FUNCTION (this);

  Ptr<QueueDiscItem> item = GetInternalQueue (0)->Dequeue ();
  if (!item)
    {
      // Leave dropping state when queue is empty
      m_dropping = false;
      NS_LOG_LOGIC ("Queue empty");
      return 0;
    }
  uint32_t now = CoDelGetTime ();

  NS_LOG_LOGIC ("Popped " << item);
  NS_LOG_LOGIC ("Number packets remaining " << GetInternalQueue (0)->GetNPackets ());
  NS_LOG_LOGIC ("Number bytes remaining " << GetInternalQueue (0)->GetNBytes ());

  // Determine if item should be dropped
  bool okToDrop = OkToDrop (item, now);

  if (m_dropping)
    { // In the dropping state (sojourn time has gone above target and hasn't come down yet)
      // Check if we can leave the dropping state or next drop should occur
      NS_LOG_LOGIC ("In dropping state, check if it's OK to leave or next drop should occur");
      if (!okToDrop)
        {
          /* sojourn time fell below target - leave dropping state */
          NS_LOG_LOGIC ("Sojourn time goes below target, it's OK to leave dropping state.");
          m_dropping = false;
        }
      else if (CoDelTimeAfterEq (now, m_dropNext))
        {
          m_state2++;
          while (m_dropping && CoDelTimeAfterEq (now, m_dropNext))
            {
              // It's time for the next drop. Drop the current packet and
              // dequeue the next. The dequeue might take us out of dropping
              // state. If not, schedule the next drop.
              // A large amount of packets in queue might result in drop
              // rates so high that the next drop should happen now,
              // hence the while loop.
              NS_LOG_LOGIC ("Sojourn time is still above target and it's time for next drop; dropping " << item);
              DropAfterDequeue (item, TARGET_EXCEEDED_DROP);

              ++m_count;
              NewtonStep ();
              item = GetInternalQueue (0)->Dequeue ();

              if (item)
                {
                  NS_LOG_LOGIC ("Popped " << item);
                  NS_LOG_LOGIC ("Number packets remaining " << GetInternalQueue (0)->GetNPackets ());
                  NS_LOG_LOGIC ("Number bytes remaining " << GetInternalQueue (0)->GetNBytes ());
                }

              if (!OkToDrop (item, now))
                {
                  /* leave dropping state */
                  NS_LOG_LOGIC ("Leaving dropping state");
                  m_dropping = false;
                }
              else
                {
                  /* schedule the next drop */
                  NS_LOG_LOGIC ("Running ControlLaw for input m_dropNext: " << (double)m_dropNext / 1000000);
                  m_dropNext = ControlLaw (m_dropNext);
                  NS_LOG_LOGIC ("Scheduled next drop at " << (double)m_dropNext / 1000000);
                }
            }
        }
    }
  else
    {
      // Not in the dropping state
      // Decide if we have to enter the dropping state and drop the first packet
      NS_LOG_LOGIC ("Not in dropping state; decide if we have to enter the state and drop the first packet");
      if (okToDrop)
        {
          // Drop the first packet and enter dropping state unless the queue is empty
          NS_LOG_LOGIC ("Sojourn time goes above target, dropping the first packet " << item << " and entering the dropping state");
          DropAfterDequeue (item, TARGET_EXCEEDED_DROP);

          item = GetInternalQueue (0)->Dequeue ();

          if (item)
            {
              NS_LOG_LOGIC ("Popped " << item);
              NS_LOG_LOGIC ("Number packets remaining " << GetInternalQueue (0)->GetNPackets ());
              NS_LOG_LOGIC ("Number bytes remaining " << GetInternalQueue (0)->GetNBytes ());
            }

          OkToDrop (item, now);
          m_dropping = true;
          ++m_state3;
          /*
           * if min went above target close to when we last went below it
           * assume that the drop rate that controlled the queue on the
           * last cycle is a good starting point to control it now.
           */
          int delta = m_count - m_lastCount;
          if (delta > 1 && CoDelTimeBefore (now - m_dropNext, 16 * Time2CoDel (m_interval)))
            {
              m_count = delta;
              NewtonStep ();
            }
          else
            {
              m_count = 1;
              m_recInvSqrt = ~0U >> REC_INV_SQRT_SHIFT;
            }
          m_lastCount = m_count;
          NS_LOG_LOGIC ("Running ControlLaw for input now: " << (double)now);
          m_dropNext = ControlLaw (now);
          NS_LOG_LOGIC ("Scheduled next drop at " << (double)m_dropNext / 1000000 << " now " << (double)now / 1000000);
        }
    }
  ++m_states;
  return item;
}

uint32_t
CoDelQueueDisc::GetQueueSize (void)
{
  NS_LOG_FUNCTION (this);
  if (GetMode () == QUEUE_DISC_MODE_BYTES)
    {
      return GetInternalQueue (0)->GetNBytes ();
    }
  else if (GetMode () == QUEUE_DISC_MODE_PACKETS)
    {
      return GetInternalQueue (0)->GetNPackets ();
    }
  else
    {
      NS_ABORT_MSG ("Unknown mode.");
    }
}

Time
CoDelQueueDisc::GetTarget (void)
{
  return m_target;
}

Time
CoDelQueueDisc::GetInterval (void)
{
  return m_interval;
}

uint32_t
CoDelQueueDisc::GetDropNext (void)
{
  return m_dropNext;
}

Ptr<const QueueDiscItem>
CoDelQueueDisc::DoPeek (void) const
{
  NS_LOG_FUNCTION (this);

  if (GetInternalQueue (0)->IsEmpty ())
    {
      NS_LOG_LOGIC ("Queue empty");
      return 0;
    }

  Ptr<const QueueDiscItem> item = GetInternalQueue (0)->Peek ();

  NS_LOG_LOGIC ("Number packets " << GetInternalQueue (0)->GetNPackets ());
  NS_LOG_LOGIC ("Number bytes " << GetInternalQueue (0)->GetNBytes ());

  return item;
}

bool
CoDelQueueDisc::CoDelTimeAfter (uint32_t a, uint32_t b)
{
  return  ((int)(a) - (int)(b) > 0);
}

bool
CoDelQueueDisc::CoDelTimeAfterEq (uint32_t a, uint32_t b)
{
  return ((int)(a) - (int)(b) >= 0);
}

bool
CoDelQueueDisc::CoDelTimeBefore (uint32_t a, uint32_t b)
{
  return  ((int)(a) - (int)(b) < 0);
}

bool
CoDelQueueDisc::CoDelTimeBeforeEq (uint32_t a, uint32_t b)
{
  return ((int)(a) - (int)(b) <= 0);
}

uint32_t
CoDelQueueDisc::Time2CoDel (Time t)
{
  return (t.GetNanoSeconds () >> CODEL_SHIFT);
}

bool
CoDelQueueDisc::CheckConfig (void)
{
  NS_LOG_FUNCTION (this);
  if (GetNQueueDiscClasses () > 0)
    {
      NS_LOG_ERROR ("CoDelQueueDisc cannot have classes");
      return false;
    }

  if (GetNPacketFilters () > 0)
    {
      NS_LOG_ERROR ("CoDelQueueDisc cannot have packet filters");
      return false;
    }

  if (GetNInternalQueues () == 0)
    {
      // create a DropTail queue
      Ptr<InternalQueue> queue = CreateObjectWithAttributes<DropTailQueue<QueueDiscItem> > ("Mode", EnumValue (m_mode));
      if (m_mode == QUEUE_DISC_MODE_PACKETS)
        {
          queue->SetMaxPackets (m_maxPackets);
        }
      else
        {
          queue->SetMaxBytes (m_maxBytes);
        }
      AddInternalQueue (queue);
    }

  if (GetNInternalQueues () != 1)
    {
      NS_LOG_ERROR ("CoDelQueueDisc needs 1 internal queue");
      return false;
    }

  if ((GetInternalQueue (0)->GetMode () == QueueBase::QUEUE_MODE_PACKETS && m_mode == QUEUE_DISC_MODE_BYTES) ||
      (GetInternalQueue (0)->GetMode () == QueueBase::QUEUE_MODE_BYTES && m_mode == QUEUE_DISC_MODE_PACKETS))
    {
      NS_LOG_ERROR ("The mode of the provided queue does not match the mode set on the CoDelQueueDisc");
      return false;
    }

  if ((m_mode ==  QUEUE_DISC_MODE_PACKETS && GetInternalQueue (0)->GetMaxPackets () != m_maxPackets) ||
      (m_mode ==  QUEUE_DISC_MODE_BYTES && GetInternalQueue (0)->GetMaxBytes () != m_maxBytes))
    {
      NS_LOG_ERROR ("The size of the internal queue differs from the queue disc limit");
      return false;
    }

  return true;
}

void
CoDelQueueDisc::InitializeParams (void)
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3

