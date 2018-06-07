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
 */

/*
 * PORT NOTE: This code was ported from ns-2.36rc1 (queue/pie.cc).
 * Most of the comments are also ported from the same.
 */

#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/simulator.h"
#include "ns3/abort.h"
#include "pie-queue-disc.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/net-device-queue-interface.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("PieQueueDisc");

NS_OBJECT_ENSURE_REGISTERED (PieQueueDisc);

TypeId PieQueueDisc::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PieQueueDisc")
    .SetParent<QueueDisc> ()
    .SetGroupName ("TrafficControl")
    .AddConstructor<PieQueueDisc> ()
    .AddAttribute ("MeanPktSize",
                   "Average of packet size",
                   UintegerValue (1000),
                   MakeUintegerAccessor (&PieQueueDisc::m_meanPktSize),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("A",
                   "Value of alpha",
                   DoubleValue (0.125),
                   MakeDoubleAccessor (&PieQueueDisc::m_a),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("B",
                   "Value of beta",
                   DoubleValue (1.25),
                   MakeDoubleAccessor (&PieQueueDisc::m_b),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Tupdate",
                   "Time period to calculate drop probability",
                   TimeValue (Seconds (0.03)),
                   MakeTimeAccessor (&PieQueueDisc::m_tUpdate),
                   MakeTimeChecker ())
    .AddAttribute ("Supdate",
                   "Start time of the update timer",
                   TimeValue (Seconds (0)),
                   MakeTimeAccessor (&PieQueueDisc::m_sUpdate),
                   MakeTimeChecker ())
    .AddAttribute ("MaxSize",
                   "The maximum number of packets accepted by this queue disc",
                   QueueSizeValue (QueueSize ("25p")),
                   MakeQueueSizeAccessor (&QueueDisc::SetMaxSize,
                                          &QueueDisc::GetMaxSize),
                   MakeQueueSizeChecker ())
    .AddAttribute ("DequeueThreshold",
                   "Minimum queue size in bytes before dequeue rate is measured",
                   UintegerValue (10000),
                   MakeUintegerAccessor (&PieQueueDisc::m_dqThreshold),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("QueueDelayReference",
                   "Desired queue delay",
                   TimeValue (Seconds (0.02)),
                   MakeTimeAccessor (&PieQueueDisc::m_qDelayRef),
                   MakeTimeChecker ())
    .AddAttribute ("MaxBurstAllowance",
                   "Current max burst allowance in seconds before random drop",
                   TimeValue (Seconds (0.1)),
                   MakeTimeAccessor (&PieQueueDisc::m_maxBurst),
                   MakeTimeChecker ())
  ;

  return tid;
}

PieQueueDisc::PieQueueDisc ()
  : QueueDisc (QueueDiscSizePolicy::SINGLE_INTERNAL_QUEUE)
{
  NS_LOG_FUNCTION (this);
  m_uv = CreateObject<UniformRandomVariable> ();
  m_rtrsEvent = Simulator::Schedule (m_sUpdate, &PieQueueDisc::CalculateP, this);
}

PieQueueDisc::~PieQueueDisc ()
{
  NS_LOG_FUNCTION (this);
}

void
PieQueueDisc::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_uv = 0;
  Simulator::Remove (m_rtrsEvent);
  QueueDisc::DoDispose ();
}

Time
PieQueueDisc::GetQueueDelay (void)
{
  NS_LOG_FUNCTION (this);
  return m_qDelay;
}

int64_t
PieQueueDisc::AssignStreams (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);
  m_uv->SetStream (stream);
  return 1;
}

bool
PieQueueDisc::DoEnqueue (Ptr<QueueDiscItem> item)
{
  NS_LOG_FUNCTION (this << item);

  QueueSize nQueued = GetCurrentSize ();

  if (nQueued + item > GetMaxSize ())
    {
      // Drops due to queue limit: reactive
      DropBeforeEnqueue (item, FORCED_DROP);
      return false;
    }
  else if (DropEarly (item, nQueued.GetValue ()))
    {
      // Early probability drop: proactive
      DropBeforeEnqueue (item, UNFORCED_DROP);
      return false;
    }

  // No drop
  bool retval = GetInternalQueue (0)->Enqueue (item);

  // If Queue::Enqueue fails, QueueDisc::DropBeforeEnqueue is called by the
  // internal queue because QueueDisc::AddInternalQueue sets the trace callback

  NS_LOG_LOGIC ("\t bytesInQueue  " << GetInternalQueue (0)->GetNBytes ());
  NS_LOG_LOGIC ("\t packetsInQueue  " << GetInternalQueue (0)->GetNPackets ());

  return retval;
}

void
PieQueueDisc::InitializeParams (void)
{
  // Initially queue is empty so variables are initialize to zero except m_dqCount
  m_inMeasurement = false;
  m_dqCount = DQCOUNT_INVALID;
  m_dropProb = 0;
  m_avgDqRate = 0.0;
  m_dqStart = 0;
  m_burstState = NO_BURST;
  m_qDelayOld = Time (Seconds (0));
}

bool PieQueueDisc::DropEarly (Ptr<QueueDiscItem> item, uint32_t qSize)
{
  NS_LOG_FUNCTION (this << item << qSize);
  if (m_burstAllowance.GetSeconds () > 0)
    {
      // If there is still burst_allowance left, skip random early drop.
      return false;
    }

  if (m_burstState == NO_BURST)
    {
      m_burstState = IN_BURST_PROTECTING;
      m_burstAllowance = m_maxBurst;
    }

  double p = m_dropProb;

  uint32_t packetSize = item->GetSize ();

  if (GetMaxSize ().GetUnit () == QueueSizeUnit::BYTES)
    {
      p = p * packetSize / m_meanPktSize;
    }
  bool earlyDrop = true;
  double u =  m_uv->GetValue ();

  if ((m_qDelayOld.GetSeconds () < (0.5 * m_qDelayRef.GetSeconds ())) && (m_dropProb < 0.2))
    {
      return false;
    }
  else if (GetMaxSize ().GetUnit () == QueueSizeUnit::BYTES && qSize <= 2 * m_meanPktSize)
    {
      return false;
    }
  else if (GetMaxSize ().GetUnit () == QueueSizeUnit::PACKETS && qSize <= 2)
    {
      return false;
    }

  if (u > p)
    {
      earlyDrop = false;
    }
  if (!earlyDrop)
    {
      return false;
    }

  return true;
}

void PieQueueDisc::CalculateP ()
{
  NS_LOG_FUNCTION (this);
  Time qDelay;
  double p = 0.0;
  bool missingInitFlag = false;
  if (m_avgDqRate > 0)
    {
      qDelay = Time (Seconds (GetInternalQueue (0)->GetNBytes () / m_avgDqRate));
    }
  else
    {
      qDelay = Time (Seconds (0));
      missingInitFlag = true;
    }

  m_qDelay = qDelay;

  if (m_burstAllowance.GetSeconds () > 0)
    {
      m_dropProb = 0;
    }
  else
    {
      p = m_a * (qDelay.GetSeconds () - m_qDelayRef.GetSeconds ()) + m_b * (qDelay.GetSeconds () - m_qDelayOld.GetSeconds ());
      if (m_dropProb < 0.001)
        {
          p /= 32;
        }
      else if (m_dropProb < 0.01)
        {
          p /= 8;
        }
      else if (m_dropProb < 0.1)
        {
          p /= 2;
        }
      else if (m_dropProb < 1)
        {
          p /= 0.5;
        }
      else if (m_dropProb < 10)
        {
          p /= 0.125;
        }
      else
        {
          p /= 0.03125;
        }
      if ((m_dropProb >= 0.1) && (p > 0.02))
        {
          p = 0.02;
        }
    }

  p += m_dropProb;

  // For non-linear drop in prob

  if (qDelay.GetSeconds () == 0 && m_qDelayOld.GetSeconds () == 0)
    {
      p *= 0.98;
    }
  else if (qDelay.GetSeconds () > 0.2)
    {
      p += 0.02;
    }

  m_dropProb = (p > 0) ? p : 0;
  if (m_burstAllowance < m_tUpdate)
    {
      m_burstAllowance =  Time (Seconds (0));
    }
  else
    {
      m_burstAllowance -= m_tUpdate;
    }

  uint32_t burstResetLimit = static_cast<uint32_t>(BURST_RESET_TIMEOUT / m_tUpdate.GetSeconds ());
  if ( (qDelay.GetSeconds () < 0.5 * m_qDelayRef.GetSeconds ()) && (m_qDelayOld.GetSeconds () < (0.5 * m_qDelayRef.GetSeconds ())) && (m_dropProb == 0) && !missingInitFlag )
    {
      m_dqCount = DQCOUNT_INVALID;
      m_avgDqRate = 0.0;
    }
  if ( (qDelay.GetSeconds () < 0.5 * m_qDelayRef.GetSeconds ()) && (m_qDelayOld.GetSeconds () < (0.5 * m_qDelayRef.GetSeconds ())) && (m_dropProb == 0) && (m_burstAllowance.GetSeconds () == 0))
    {
      if (m_burstState == IN_BURST_PROTECTING)
        {
          m_burstState = IN_BURST;
          m_burstReset = 0;
        }
      else if (m_burstState == IN_BURST)
        {
          m_burstReset++;
          if (m_burstReset > burstResetLimit)
            {
              m_burstReset = 0;
              m_burstState = NO_BURST;
            }
        }
    }
  else if (m_burstState == IN_BURST)
    {
      m_burstReset = 0;
    }

  m_qDelayOld = qDelay;
  m_rtrsEvent = Simulator::Schedule (m_tUpdate, &PieQueueDisc::CalculateP, this);
}

Ptr<QueueDiscItem>
PieQueueDisc::DoDequeue ()
{
  NS_LOG_FUNCTION (this);

  if (GetInternalQueue (0)->IsEmpty ())
    {
      NS_LOG_LOGIC ("Queue empty");
      return 0;
    }

  Ptr<QueueDiscItem> item = GetInternalQueue (0)->Dequeue ();
  double now = Simulator::Now ().GetSeconds ();
  uint32_t pktSize = item->GetSize ();

  // if not in a measurement cycle and the queue has built up to dq_threshold,
  // start the measurement cycle

  if ( (GetInternalQueue (0)->GetNBytes () >= m_dqThreshold) && (!m_inMeasurement) )
    {
      m_dqStart = now;
      m_dqCount = 0;
      m_inMeasurement = true;
    }

  if (m_inMeasurement)
    {
      m_dqCount += pktSize;

      // done with a measurement cycle
      if (m_dqCount >= m_dqThreshold)
        {

          double tmp = now - m_dqStart;

          if (tmp > 0)
            {
              if (m_avgDqRate == 0)
                {
                  m_avgDqRate = m_dqCount / tmp;
                }
              else
                {
                  m_avgDqRate = (0.5 * m_avgDqRate) + (0.5 * (m_dqCount / tmp));
                }
            }

          // restart a measurement cycle if there is enough data
          if (GetInternalQueue (0)->GetNBytes () > m_dqThreshold)
            {
              m_dqStart = now;
              m_dqCount = 0;
              m_inMeasurement = true;
            }
          else
            {
              m_dqCount = 0;
              m_inMeasurement = false;
            }
        }
    }

  return item;
}

bool
PieQueueDisc::CheckConfig (void)
{
  NS_LOG_FUNCTION (this);
  if (GetNQueueDiscClasses () > 0)
    {
      NS_LOG_ERROR ("PieQueueDisc cannot have classes");
      return false;
    }

  if (GetNPacketFilters () > 0)
    {
      NS_LOG_ERROR ("PieQueueDisc cannot have packet filters");
      return false;
    }

  if (GetNInternalQueues () == 0)
    {
      // add  a DropTail queue
      AddInternalQueue (CreateObjectWithAttributes<DropTailQueue<QueueDiscItem> >
                          ("MaxSize", QueueSizeValue (GetMaxSize ())));
    }

  if (GetNInternalQueues () != 1)
    {
      NS_LOG_ERROR ("PieQueueDisc needs 1 internal queue");
      return false;
    }

  return true;
}

} //namespace ns3
