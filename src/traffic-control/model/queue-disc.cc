/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007, 2014 University of Washington
 *               2015 Universita' degli Studi di Napoli Federico II
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
#include "ns3/abort.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"
#include "ns3/object-vector.h"
#include "ns3/packet.h"
#include "ns3/unused.h"
#include "queue-disc.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("QueueDisc");

QueueDiscItem::QueueDiscItem (Ptr<Packet> p, const Address& addr, uint16_t protocol)
  : QueueItem (p),
    m_address (addr),
    m_protocol (protocol),
    m_txq (0)
{
}

QueueDiscItem::~QueueDiscItem()
{
  NS_LOG_FUNCTION (this);
}

Address
QueueDiscItem::GetAddress (void) const
{
  return m_address;
}

uint16_t
QueueDiscItem::GetProtocol (void) const
{
  return m_protocol;
}

uint8_t
QueueDiscItem::GetTxQueueIndex (void) const
{
  return m_txq;
}

void
QueueDiscItem::SetTxQueueIndex (uint8_t txq)
{
  m_txq = txq;
}

void
QueueDiscItem::Print (std::ostream& os) const
{
  os << GetPacket () << " "
     << "Dst addr " << m_address << " "
     << "proto " << (uint16_t) m_protocol << " "
     << "txq " << (uint8_t) m_txq
  ;
}


NS_OBJECT_ENSURE_REGISTERED (QueueDiscClass);

TypeId QueueDiscClass::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::QueueDiscClass")
    .SetParent<Object> ()
    .SetGroupName ("TrafficControl")
    .AddConstructor<QueueDiscClass> ()
    .AddAttribute ("QueueDisc", "The queue disc attached to the class",
                   PointerValue (),
                   MakePointerAccessor (&QueueDiscClass::m_queueDisc),
                   MakePointerChecker<QueueDisc> ())
  ;
  return tid;
}

QueueDiscClass::QueueDiscClass()
{
  NS_LOG_FUNCTION (this);
}

void
QueueDiscClass::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_queueDisc = 0;
  Object::DoDispose ();
}

Ptr<QueueDisc>
QueueDiscClass::GetQueueDisc (void) const
{
  NS_LOG_FUNCTION (this);
  return m_queueDisc;
}

void
QueueDiscClass::SetQueueDisc (Ptr<QueueDisc> qd)
{
  NS_LOG_FUNCTION (this);
  NS_ABORT_MSG_IF (m_queueDisc, "Cannot set the queue disc on a class already having an attached queue disc");
  m_queueDisc = qd;
}


NS_OBJECT_ENSURE_REGISTERED (QueueDisc);

TypeId QueueDisc::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::QueueDisc")
    .SetParent<Object> ()
    .SetGroupName ("TrafficControl")
    .AddAttribute ("Quota", "The maximum number of packets dequeued in a qdisc run",
                   UintegerValue (DEFAULT_QUOTA),
                   MakeUintegerAccessor (&QueueDisc::SetQuota,
                                         &QueueDisc::GetQuota),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("InternalQueueList", "The list of internal queues.",
                   ObjectVectorValue (),
                   MakeObjectVectorAccessor (&QueueDisc::m_queues),
                   MakeObjectVectorChecker<Queue> ())
    .AddAttribute ("PacketFilterList", "The list of packet filters.",
                   ObjectVectorValue (),
                   MakeObjectVectorAccessor (&QueueDisc::m_filters),
                   MakeObjectVectorChecker<PacketFilter> ())
    .AddAttribute ("QueueDiscClassList", "The list of queue disc classes.",
                   ObjectVectorValue (),
                   MakeObjectVectorAccessor (&QueueDisc::m_classes),
                   MakeObjectVectorChecker<QueueDiscClass> ())
    .AddTraceSource ("Enqueue", "Enqueue a packet in the queue disc",
                     MakeTraceSourceAccessor (&QueueDisc::m_traceEnqueue),
                     "ns3::QueueItem::TracedCallback")
    .AddTraceSource ("Dequeue", "Dequeue a packet from the queue disc",
                     MakeTraceSourceAccessor (&QueueDisc::m_traceDequeue),
                     "ns3::QueueItem::TracedCallback")
    .AddTraceSource ("Requeue", "Requeue a packet in the queue disc",
                     MakeTraceSourceAccessor (&QueueDisc::m_traceRequeue),
                     "ns3::QueueItem::TracedCallback")
    .AddTraceSource ("Drop", "Drop a packet stored in the queue disc",
                     MakeTraceSourceAccessor (&QueueDisc::m_traceDrop),
                     "ns3::QueueItem::TracedCallback")
    .AddTraceSource ("PacketsInQueue",
                     "Number of packets currently stored in the queue disc",
                     MakeTraceSourceAccessor (&QueueDisc::m_nPackets),
                     "ns3::TracedValueCallback::Uint32")
    .AddTraceSource ("BytesInQueue",
                     "Number of bytes currently stored in the queue disc",
                     MakeTraceSourceAccessor (&QueueDisc::m_nBytes),
                     "ns3::TracedValueCallback::Uint32")
  ;
  return tid;
}

QueueDisc::QueueDisc ()
  :  m_nPackets (0),
     m_nBytes (0),
     m_nTotalReceivedPackets (0),
     m_nTotalReceivedBytes (0),
     m_nTotalDroppedPackets (0),
     m_nTotalDroppedBytes (0),
     m_nTotalRequeuedPackets (0),
     m_nTotalRequeuedBytes (0),
     m_running (false)
{
  NS_LOG_FUNCTION (this);
}

void
QueueDisc::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_queues.clear ();
  m_filters.clear ();
  m_classes.clear ();
  m_device = 0;
  m_devQueueIface = 0;
  m_requeued = 0;
  Object::DoDispose ();
}

void
QueueDisc::DoInitialize (void)
{
  NS_LOG_FUNCTION (this);
  // When adding a new interface, the traffic control aggregates
  // a NetDeviceQueueInterface object to the netdevice
  if (m_device)
    {
      m_devQueueIface = m_device->GetObject<NetDeviceQueueInterface> ();
    }

  // Check the configuration and initialize the parameters of this queue disc
  bool ok = CheckConfig ();
  NS_ASSERT_MSG (ok, "The queue disc configuration is not correct");
  NS_UNUSED (ok); // suppress compiler warning
  InitializeParams ();

  // Check the configuration and initialize the parameters of the child queue discs
  for (std::vector<Ptr<QueueDiscClass> >::iterator cl = m_classes.begin ();
       cl != m_classes.end (); cl++)
    {
      (*cl)->GetQueueDisc ()->Initialize ();
    }

  Object::DoInitialize ();
}

uint32_t
QueueDisc::GetNPackets () const
{
  NS_LOG_FUNCTION (this);
  return m_nPackets;
}

uint32_t
QueueDisc::GetNBytes (void) const
{
  NS_LOG_FUNCTION (this);
  return m_nBytes;
}

uint32_t
QueueDisc::GetTotalReceivedPackets (void) const
{
  NS_LOG_FUNCTION (this);
  return m_nTotalReceivedPackets;
}

uint32_t
QueueDisc::GetTotalReceivedBytes (void) const
{
  NS_LOG_FUNCTION (this);
  return m_nTotalReceivedBytes;
}

uint32_t
QueueDisc::GetTotalDroppedPackets (void) const
{
  NS_LOG_FUNCTION (this);
  return m_nTotalDroppedPackets;
}

uint32_t
QueueDisc:: GetTotalDroppedBytes (void) const
{
  NS_LOG_FUNCTION (this);
  return m_nTotalDroppedBytes;
}

uint32_t
QueueDisc::GetTotalRequeuedPackets (void) const
{
  NS_LOG_FUNCTION (this);
  return m_nTotalRequeuedPackets;
}

uint32_t
QueueDisc:: GetTotalRequeuedBytes (void) const
{
  NS_LOG_FUNCTION (this);
  return m_nTotalRequeuedBytes;
}

void
QueueDisc::SetNetDevice (Ptr<NetDevice> device)
{
  NS_LOG_FUNCTION (this << device);
  m_device = device;
}

Ptr<NetDevice>
QueueDisc::GetNetDevice (void) const
{
  NS_LOG_FUNCTION (this);
  return m_device;
}

void
QueueDisc::SetQuota (const uint32_t quota)
{
  NS_LOG_FUNCTION (this << quota);
  m_quota = quota;
}

uint32_t
QueueDisc::GetQuota (void) const
{
  NS_LOG_FUNCTION (this);
  return m_quota;
}

void
QueueDisc::AddInternalQueue (Ptr<Queue> queue)
{
  NS_LOG_FUNCTION (this);
  m_queues.push_back (queue);
}

Ptr<Queue>
QueueDisc::GetInternalQueue (uint32_t i) const
{
  NS_ASSERT (i < m_queues.size ());
  return m_queues[i];
}

uint32_t
QueueDisc::GetNInternalQueues (void) const
{
  return m_queues.size ();
}

void
QueueDisc::AddPacketFilter (Ptr<PacketFilter> filter)
{
  NS_LOG_FUNCTION (this);
  m_filters.push_back (filter);
}

Ptr<PacketFilter>
QueueDisc::GetPacketFilter (uint32_t i) const
{
  NS_ASSERT (i < m_filters.size ());
  return m_filters[i];
}

uint32_t
QueueDisc::GetNPacketFilters (void) const
{
  return m_filters.size ();
}

void
QueueDisc::AddQueueDiscClass (Ptr<QueueDiscClass> qdClass)
{
  NS_LOG_FUNCTION (this);
  NS_ABORT_MSG_IF (qdClass->GetQueueDisc () == 0, "Cannot add a class with no attached queue disc");
  // the child queue disc cannot be one with wake mode equal to WAKE_CHILD because
  // such queue discs do not implement the enqueue/dequeue methods
  NS_ABORT_MSG_IF (qdClass->GetQueueDisc ()->GetWakeMode () == WAKE_CHILD,
                   "A queue disc with WAKE_CHILD as wake mode can only be a root queue disc");
  // set the parent drop callback on the child queue disc, so that it can notify
  // packet drops to the parent queue disc
  qdClass->GetQueueDisc ()->SetParentDropCallback (MakeCallback (&QueueDisc::Drop, this));
  m_classes.push_back (qdClass);
}

Ptr<QueueDiscClass>
QueueDisc::GetQueueDiscClass (uint32_t i) const
{
  NS_ASSERT (i < m_classes.size ());
  return m_classes[i];
}

uint32_t
QueueDisc::GetNQueueDiscClasses (void) const
{
  return m_classes.size ();
}

int32_t
QueueDisc::Classify (Ptr<QueueDiscItem> item)
{
  NS_LOG_FUNCTION (this << item);

  int32_t ret = PacketFilter::PF_NO_MATCH;
  for (std::vector<Ptr<PacketFilter> >::iterator f = m_filters.begin ();
       f != m_filters.end () && ret == PacketFilter::PF_NO_MATCH; f++)
    {
      ret = (*f)->Classify (item);
    }
  return ret;
}

QueueDisc::WakeMode
QueueDisc::GetWakeMode (void)
{
  return WAKE_ROOT;
}

void
QueueDisc::SetParentDropCallback (ParentDropCallback cb)
{
  m_parentDropCallback = cb;
}

void
QueueDisc::Drop (Ptr<QueueItem> item)
{
  NS_LOG_FUNCTION (this << item);

  // if the wake mode of this queue disc is WAKE_CHILD, packets are directly
  // enqueued/dequeued from the child queue discs, thus this queue disc does not
  // keep valid packets/bytes counters and no actions need to be performed.
  if (this->GetWakeMode () == WAKE_CHILD)
    {
      return;
    }

  NS_ASSERT_MSG (m_nPackets >= 1u, "No packet in the queue disc, cannot drop");
  NS_ASSERT_MSG (m_nBytes >= item->GetPacketSize (), "The size of the packet that"
                 << " is reported to be dropped is greater than the amount of bytes"
                 << "stored in the queue disc");

  m_nPackets--;
  m_nBytes -= item->GetPacketSize ();
  m_nTotalDroppedPackets++;
  m_nTotalDroppedBytes += item->GetPacketSize ();

  NS_LOG_LOGIC ("m_traceDrop (p)");
  m_traceDrop (item);

  NotifyParentDrop (item);
}

void
QueueDisc::NotifyParentDrop (Ptr<QueueItem> item)
{
  NS_LOG_FUNCTION (this << item);
  // the parent drop callback is clearly null on root queue discs
  if (!m_parentDropCallback.IsNull ())
    {
      m_parentDropCallback (item);
    }
}

bool
QueueDisc::Enqueue (Ptr<QueueDiscItem> item)
{
  NS_LOG_FUNCTION (this << item);

  m_nPackets++;
  m_nBytes += item->GetPacketSize ();
  m_nTotalReceivedPackets++;
  m_nTotalReceivedBytes += item->GetPacketSize ();

  NS_LOG_LOGIC ("m_traceEnqueue (p)");
  m_traceEnqueue (item);

  return DoEnqueue (item);
}

Ptr<QueueDiscItem>
QueueDisc::Dequeue (void)
{
  NS_LOG_FUNCTION (this);

  Ptr<QueueDiscItem> item;
  item = DoDequeue ();

  if (item != 0)
    {
      m_nPackets--;
      m_nBytes -= item->GetPacketSize ();

      NS_LOG_LOGIC ("m_traceDequeue (p)");
      m_traceDequeue (item);
    }

  return item;
}

Ptr<const QueueDiscItem>
QueueDisc::Peek (void) const
{
  NS_LOG_FUNCTION (this);
  return DoPeek ();
}

void
QueueDisc::Run (void)
{
  NS_LOG_FUNCTION (this);

  if (RunBegin ())
    {
      uint32_t quota = m_quota;
      while (Restart ())
        {
          quota -= 1;
          if (quota <= 0)
            {
              /// \todo netif_schedule (q);
              break;
            }
        }
      RunEnd ();
    }
}

bool
QueueDisc::RunBegin (void)
{
  NS_LOG_FUNCTION (this);
  if (m_running)
    {
      return false;
    }

  m_running = true;
  return true;
}

void
QueueDisc::RunEnd (void)
{
  NS_LOG_FUNCTION (this);
  m_running = false;
}

bool
QueueDisc::Restart (void)
{
  NS_LOG_FUNCTION (this);
  Ptr<QueueDiscItem> item = DequeuePacket();
  if (item == 0)
    {
      NS_LOG_LOGIC ("No packet to send");
      return false;
    }

  return Transmit (item);
}

Ptr<QueueDiscItem>
QueueDisc::DequeuePacket ()
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (m_devQueueIface);
  Ptr<QueueDiscItem> item;

  // First check if there is a requeued packet
  if (m_requeued != 0)
    {
        // If the queue where the requeued packet is destined to is not stopped, return
        // the requeued packet; otherwise, return an empty packet.
        // If the device does not support flow control, the device queue is never stopped
        if (!m_devQueueIface->GetTxQueue (m_requeued->GetTxQueueIndex ())->IsStopped ())
          {
            item = m_requeued;
            m_requeued = 0;

            m_nPackets--;
            m_nBytes -= item->GetPacketSize ();

            NS_LOG_LOGIC ("m_traceDequeue (p)");
            m_traceDequeue (item);
          }
    }
  else
    {
      // If the device is multi-queue (actually, Linux checks if the queue disc has
      // multiple queues), ask the queue disc to dequeue a packet (a multi-queue aware
      // queue disc should try not to dequeue a packet destined to a stopped queue).
      // Otherwise, ask the queue disc to dequeue a packet only if the (unique) queue
      // is not stopped.
      if (m_devQueueIface->GetTxQueuesN ()>1 || !m_devQueueIface->GetTxQueue (0)->IsStopped ())
        {
          item = Dequeue ();
          // If the item is not null, add the header to the packet.
          if (item != 0)
            {
              item->AddHeader ();
            }
          // Here, Linux tries bulk dequeues
        }
    }
  return item;
}

void
QueueDisc::Requeue (Ptr<QueueDiscItem> item)
{
  NS_LOG_FUNCTION (this << item);
  m_requeued = item;
  /// \todo netif_schedule (q);

  m_nPackets++;       // it's still part of the queue
  m_nBytes += item->GetPacketSize ();
  m_nTotalRequeuedPackets++;
  m_nTotalRequeuedBytes += item->GetPacketSize ();

  NS_LOG_LOGIC ("m_traceRequeue (p)");
  m_traceRequeue (item);
}

bool
QueueDisc::Transmit (Ptr<QueueDiscItem> item)
{
  NS_LOG_FUNCTION (this << item);
  NS_ASSERT (m_devQueueIface);
  bool ret = false;

  if (!m_devQueueIface->GetTxQueue (item->GetTxQueueIndex ())->IsStopped ())
    {
      // send a copy of the packet because the device might add the
      // MAC header even if the transmission is unsuccessful (see BUG 2284)
      Ptr<Packet> copy = item->GetPacket ()->Copy ();
      ret = m_device->Send (copy, item->GetAddress (), item->GetProtocol ());
    }

  // If the transmission did not happen or failed, requeue the item
  if (!ret)
    {
      Requeue (item);
    }

  // If the transmission succeeded but now the queue is stopped, return false
  if (ret && m_devQueueIface->GetTxQueue (item->GetTxQueueIndex ())->IsStopped ())
    {
      ret = false;
    }

  return ret;
}

} // namespace ns3
