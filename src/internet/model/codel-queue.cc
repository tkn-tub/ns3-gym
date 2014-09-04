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
#include "codel-queue.h"

NS_LOG_COMPONENT_DEFINE ("CoDelQueue");

namespace ns3 {

#define BITS_PER_LONG (8 * sizeof (unsigned long))

/* borrowed from the linux kernel */
#define do_div(n,base)						\
({								\
	int __res;						\
	__res = ((unsigned long)n) % (unsigned int)base;	\
	n = ((unsigned long)n) / (unsigned int)base;		\
	__res;							\
})

static inline uint32_t reciprocal_divide(uint32_t A, uint32_t R)
{
	return (uint32_t)(((uint64_t)A * R) >> 32);
}

/* end kernel borrowings */

static codel_time_t codel_get_time(void)
{
  Time time = Simulator::Now ();
  uint64_t ns = time.GetNanoSeconds ();

  return ns >> CODEL_SHIFT;
}

#define codel_time_after(a, b)	 ((int)(a) - (int)(b) > 0)
#define codel_time_after_eq(a, b) ((int)(a) - (int)(b) >= 0)
#define codel_time_before(a, b)	 ((int)(a) - (int)(b) < 0)
#define codel_time_before_eq(a, b) ((int)(a) - (int)(b) <= 0)

#define NSEC_PER_MSEC 1000000
#define NSEC_PER_USEC 1000
#define MS2TIME(a) ((a * NSEC_PER_MSEC) >> CODEL_SHIFT)
#define US2TIME(a) ((a * NSEC_PER_USEC) >> CODEL_SHIFT)
#define NS2TIME(a) ((a) >> CODEL_SHIFT)
#define TIME2CODEL(a) NS2TIME(a.GetNanoSeconds())

#define DEFAULT_CODEL_LIMIT 1000


class CoDelTimestampTag : public Tag
{
public:
  CoDelTimestampTag ();
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual void Print (std::ostream &os) const;

  Time GetTxTime (void) const;
private:
  uint64_t m_creationTime;
};

CoDelTimestampTag::CoDelTimestampTag ()
  : m_creationTime (Simulator::Now ().GetTimeStep ())
{
}

TypeId
CoDelTimestampTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("anon::CoDelTimestampTag")
    .SetParent<Tag> ()
    .AddConstructor<CoDelTimestampTag> ()
    .AddAttribute ("CreationTime",
                   "The time at which the timestamp was created",
                   StringValue ("0.0s"),
                   MakeTimeAccessor (&CoDelTimestampTag::GetTxTime),
                   MakeTimeChecker ())
  ;
  return tid;
}
TypeId
CoDelTimestampTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
CoDelTimestampTag::GetSerializedSize (void) const
{
  return 8;
}
void
CoDelTimestampTag::Serialize (TagBuffer i) const
{
  i.WriteU64 (m_creationTime);
}
void
CoDelTimestampTag::Deserialize (TagBuffer i)
{
  m_creationTime = i.ReadU64 ();
}
void
CoDelTimestampTag::Print (std::ostream &os) const
{
  os << "CreationTime=" << m_creationTime;
}
Time
CoDelTimestampTag::GetTxTime (void) const
{
  return TimeStep (m_creationTime);
}

NS_OBJECT_ENSURE_REGISTERED (CoDelQueue);

TypeId CoDelQueue::GetTypeId (void) 
{
  static TypeId tid = TypeId ("ns3::CoDelQueue")
    .SetParent<Queue> ()
    .AddConstructor<CoDelQueue> ()
    .AddAttribute ("Mode", 
                   "Whether to use Bytes (see MaxBytes) or Packets (see MaxPackets) as the maximum queue size metric.",
                   EnumValue (BYTES),
                   MakeEnumAccessor (&CoDelQueue::SetMode),
                   MakeEnumChecker (BYTES, "Bytes",
                                    PACKETS, "Packets"))
    .AddAttribute ("MaxPackets", 
                   "The maximum number of packets accepted by this CoDelQueue.",
                   UintegerValue (DEFAULT_CODEL_LIMIT),
                   MakeUintegerAccessor (&CoDelQueue::m_maxPackets),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MaxBytes", 
                   "The maximum number of bytes accepted by this CoDelQueue.",
                   UintegerValue (1500*DEFAULT_CODEL_LIMIT),
                   MakeUintegerAccessor (&CoDelQueue::m_maxBytes),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MinBytes", 
                   "The CoDel algorithm minbytes parameter.",
                   UintegerValue (1500),
                   MakeUintegerAccessor (&CoDelQueue::m_minbytes),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Interval",
                   "The CoDel algorithm interval",
                   StringValue ("100ms"),
                   MakeTimeAccessor (&CoDelQueue::m_Interval),
                   MakeTimeChecker ())
    .AddAttribute ("Target",
                   "The CoDel algorithm target queue delay",
                   StringValue ("5ms"),
                   MakeTimeAccessor (&CoDelQueue::m_Target),
                   MakeTimeChecker ())
    .AddTraceSource("count",
                    "CoDel count",
                    MakeTraceSourceAccessor(&CoDelQueue::m_count))
    .AddTraceSource("drop_count",
                    "CoDel drop count",
                    MakeTraceSourceAccessor(&CoDelQueue::m_drop_count))
    .AddTraceSource("last_count",
                    "CoDel lastcount",
                    MakeTraceSourceAccessor(&CoDelQueue::m_lastcount))
    // .AddTraceSource("bytesInQueue",
    //                 "Number of bytes in the queue",
    //                 MakeTraceSourceAccessor(&CoDelQueue::m_bytesInQueue))
  ;

  return tid;
}

CoDelQueue::CoDelQueue () :
  Queue (),
  m_packets (),
  m_maxBytes(),
  m_bytesInQueue(0),
  backlog(&m_bytesInQueue),
  m_count(0),
  m_drop_count(0),
  m_lastcount(0),
  m_dropping(false),
  m_rec_inv_sqrt(~0U >> REC_INV_SQRT_SHIFT),
  m_first_above_time(0),
  m_drop_next(0),
  m_state1(0),
  m_state2(0),
  m_state3(0),
  m_states(0),
  m_drop_overlimit(0)  
{
  NS_LOG_FUNCTION_NOARGS ();
}

CoDelQueue::~CoDelQueue ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
CoDelQueue::NewtonStep(void)
{
  uint32_t invsqrt = ((uint32_t) m_rec_inv_sqrt) << REC_INV_SQRT_SHIFT;
  uint32_t invsqrt2 = ((uint64_t) invsqrt*invsqrt) >> 32;
  uint64_t val = (3ll<<32) - ((uint64_t) m_count * invsqrt2);

  val >>= 2; /* avoid overflow */
  val = (val * invsqrt) >> (32-2+1);
  m_rec_inv_sqrt = val >> REC_INV_SQRT_SHIFT;
}

codel_time_t 
CoDelQueue::ControlLaw(codel_time_t t)
{
  return t + reciprocal_divide(TIME2CODEL(m_Interval), m_rec_inv_sqrt << REC_INV_SQRT_SHIFT);
}

void
CoDelQueue::SetMode (enum Mode mode)
{
  NS_LOG_FUNCTION (mode);
  m_mode = mode;
}

CoDelQueue::Mode
CoDelQueue::GetMode (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_mode;
}

bool 
CoDelQueue::DoEnqueue (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);

  if (m_mode == PACKETS && (m_packets.size () >= m_maxPackets))
    {
      NS_LOG_LOGIC ("Queue full (at max packets) -- droppping pkt");
      Drop (p);
      ++m_drop_overlimit;
      return false;
    }

  if (m_mode == BYTES && (m_bytesInQueue + p->GetSize () >= m_maxBytes))
    {
      NS_LOG_LOGIC ("Queue full (packet would exceed max bytes) -- droppping pkt");
      Drop (p);
      ++m_drop_overlimit;
      return false;
    }

  CoDelTimestampTag tag;
  p->AddByteTag (tag);
  m_bytesInQueue += p->GetSize ();
  m_packets.push (p);

  NS_LOG_LOGIC ("Number packets " << m_packets.size ());
  NS_LOG_LOGIC ("Number bytes " << m_bytesInQueue);

  return true;
}

bool
CoDelQueue::ShouldDrop(Ptr<Packet> p, codel_time_t now)
{
  CoDelTimestampTag tag;
  bool drop;
  p->FindFirstMatchingByteTag (tag);
  Time delta = Simulator::Now () - tag.GetTxTime ();
  NS_LOG_INFO ("Sojourn time "<<delta.GetSeconds ());
  codel_time_t sojourn_time = TIME2CODEL(delta);
  
  if (codel_time_before(sojourn_time, TIME2CODEL(m_Target)) || 
      *backlog < m_minbytes)
    {
      /* went below so we'll stay below for at least q->interval */
      m_first_above_time = 0;
      return false;
    }
  drop = false;
  if (m_first_above_time == 0) 
    {
      /* just went above from below. If we stay above
       * for at least q->interval we'll say it's ok to drop
       */
      m_first_above_time = now + TIME2CODEL(m_Interval);
    } 
  else 
    if (codel_time_after(now, m_first_above_time)) 
      {
        drop = true;
        ++m_state1;
      }
  if (!drop)
    Drop (p);
  return drop;
}

Ptr<Packet>
CoDelQueue::DoDequeue (void)
{
  NS_LOG_FUNCTION (this);

  if (m_packets.empty ())
    {
      m_dropping = false;
      m_first_above_time = 0;
      NS_LOG_LOGIC ("Queue empty");
      return 0;
    }
  codel_time_t now = codel_get_time();
  Ptr<Packet> p = m_packets.front ();
  m_packets.pop ();
  m_bytesInQueue -= p->GetSize ();

  NS_LOG_LOGIC ("Popped " << p);
  NS_LOG_LOGIC ("Number packets " << m_packets.size ());
  NS_LOG_LOGIC ("Number bytes " << m_bytesInQueue);

  bool drop = ShouldDrop(p, now);
  if (m_dropping)
    {
      if (!drop)
        {
          /* sojourn time below target - leave dropping state */    
          m_dropping = false;
        }
      else
        if (codel_time_after_eq(now, m_drop_next))
          {
            m_state2++;
            /* It's time for the next drop. Drop the current
             * packet and dequeue the next. The dequeue might 
             * take us out of dropping state. 
             * If not, schedule the next drop.
             * A large backlog might result in drop rates so high
             * that the next drop should happen now, 
             * hence the while loop.
             */  
            while (m_dropping && 
                   codel_time_after_eq(now, m_drop_next)) {
              Drop(p);
              ++m_drop_count;
              ++m_count;
              NewtonStep();
              if (m_packets.empty ())
                {
                  m_dropping = false;
                  NS_LOG_LOGIC ("Queue empty");
                  ++m_states;
                  return 0;
                }
              p = m_packets.front ();
              m_packets.pop ();
              m_bytesInQueue -= p->GetSize ();

              NS_LOG_LOGIC ("Popped " << p);
              NS_LOG_LOGIC ("Number packets " << m_packets.size ());
              NS_LOG_LOGIC ("Number bytes " << m_bytesInQueue);

              if (!ShouldDrop(p, now)) 
                {
                  /* leave dropping state */
                  m_dropping = false;
                }
              else 
                {
                  /* and schedule the next drop */
                  m_drop_next = ControlLaw(m_drop_next);
                }
            }
          }
    }
  else 
    if (drop &&
        (codel_time_before(now - m_drop_next,
                           TIME2CODEL(m_Interval)) ||
         codel_time_after_eq(now - m_first_above_time,
                             TIME2CODEL(m_Interval)))) 
      {
        ++m_drop_count;

        NS_LOG_LOGIC ("Popped " << p);
        NS_LOG_LOGIC ("Number packets " << m_packets.size ());
        NS_LOG_LOGIC ("Number bytes " << m_bytesInQueue);

        drop = ShouldDrop(p, now);
        m_dropping = true;
        ++m_state3;
        /* 
         * if min went above target close to when we last went below it
         * assume that the drop rate that controlled the queue on the
         * last cycle is a good starting point to control it now.
         */
        int delta = m_count - m_lastcount;
        if (delta > 1 && codel_time_after(now - m_drop_next, TIME2CODEL(m_Interval))) 
          {
            m_count = delta;
            NewtonStep();
          } 
        else
          {
            m_count = 1;
            m_rec_inv_sqrt = ~0U >> REC_INV_SQRT_SHIFT;
          }
        m_lastcount = m_count;
        m_drop_next = ControlLaw(now);
        p = NULL;
      }
  ++m_states;
  return p;
}

uint32_t
CoDelQueue::GetQueueSize (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (GetMode () == BYTES)
    {
      return m_bytesInQueue;
    }
  else if (GetMode () == PACKETS)
    {
      return m_packets.size ();
    }
  else
    {
      NS_ABORT_MSG ("Unknown mode.");
    }
}

Ptr<const Packet>
CoDelQueue::DoPeek (void) const
{
  NS_LOG_FUNCTION (this);

  if (m_packets.empty ())
    {
      NS_LOG_LOGIC ("Queue empty");
      return 0;
    }

  Ptr<Packet> p = m_packets.front ();

  NS_LOG_LOGIC ("Number packets " << m_packets.size ());
  NS_LOG_LOGIC ("Number bytes " << m_bytesInQueue);

  return p;
}

} // namespace ns3

