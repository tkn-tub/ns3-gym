/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017 Universita' degli Studi di Napoli Federico II
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
 * Author: Stefano Avallone <stefano.avallone@.unina.it>
 */
#ifndef NET_DEVICE_QUEUE_INTERFACE_H
#define NET_DEVICE_QUEUE_INTERFACE_H

#include <vector>
#include <map>
#include "ns3/callback.h"
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/queue-item.h"
#include "ns3/queue.h"
#include "ns3/net-device.h"

namespace ns3 {

class QueueLimits;
class NetDeviceQueueInterface;

// This header file is included by all the queue discs and all the netdevices
// using a Queue object. The following explicit template instantiation
// declarations enables them to suppress implicit template instantiations
extern template class Queue<Packet>;
extern template class Queue<QueueDiscItem>;


/**
 * \ingroup network
 * \defgroup netdevice Network Device
 */

/**
 * \ingroup netdevice
 *
 * \brief Network device transmission queue
 *
 * This class stores information about a single transmission queue
 * of a network device that is exposed to queue discs. Such information
 * includes the state of the transmission queue (whether it has been
 * stopped or not) and data used by techniques such as Byte Queue Limits.
 *
 * This class roughly models the struct netdev_queue of Linux.
 */
class NetDeviceQueue : public SimpleRefCount<NetDeviceQueue>
{
public:
  NetDeviceQueue ();
  virtual ~NetDeviceQueue();

  /**
   * Called by the device to start this device transmission queue.
   * This is the analogous to the netif_tx_start_queue function of the Linux kernel.
   */
  virtual void Start (void);

  /**
   * Called by the device to stop this device transmission queue.
   * This is the analogous to the netif_tx_stop_queue function of the Linux kernel.
   */
  virtual void Stop (void);

  /**
   * Called by the device to wake the queue disc associated with this
   * device transmission queue. This is done by invoking the wake callback.
   * This is the analogous to the netif_tx_wake_queue function of the Linux kernel.
   */
  virtual void Wake (void);

  /**
   * \brief Get the status of the device transmission queue.
   * \return true if the device transmission queue is stopped.
   *
   * Called by queue discs to enquire about the status of a given transmission queue.
   * This is the analogous to the netif_xmit_stopped function of the Linux kernel.
   */
  bool IsStopped (void) const;

  /// Callback invoked by netdevices to wake upper layers
  typedef Callback< void > WakeCallback;

  /**
   * \brief Set the wake callback
   * \param cb the callback to set
   *
   * Called by the traffic control layer to set the wake callback. The wake callback
   * is invoked by the device whenever it is needed to "wake" the upper layers (i.e.,
   * solicitate the queue disc associated with this transmission queue (in case of
   * multi-queue aware queue discs) or to the network device (otherwise) to send
   * packets down to the device).
   */
  virtual void SetWakeCallback (WakeCallback cb);

  /**
   * \brief Called by the netdevice to report the number of bytes queued to the device queue
   * \param bytes number of bytes queued to the device queue
   */
  void NotifyQueuedBytes (uint32_t bytes);

  /**
   * \brief Called by the netdevice to report the number of bytes it is going to transmit
   * \param bytes number of bytes the device is going to transmit
   */
  void NotifyTransmittedBytes (uint32_t bytes);

  /**
   * \brief Reset queue limits state
   */
  void ResetQueueLimits ();

  /**
   * \brief Set queue limits to this queue
   * \param ql the queue limits associated to this queue
   */
  void SetQueueLimits (Ptr<QueueLimits> ql);

  /**
   * \brief Get queue limits to this queue
   * \return the queue limits associated to this queue
   */
  Ptr<QueueLimits> GetQueueLimits ();

  /**
   * \brief Perform the actions required by flow control and dynamic queue
   *        limits when a packet is enqueued in the queue of a netdevice
   *
   * \param queue the device queue
   * \param ndqi the NetDeviceQueueInterface object aggregated to the device
   * \param txq the index of the transmission queue associated with the device queue
   * \param item the enqueued packet
   *
   * This method must be connected to the "Enqueue" traced callback of a Queue
   * object (through a bound callback) in order for a netdevice to support
   * flow control and dynamic queue limits.
   */
  template <typename Item>
  static void PacketEnqueued (Ptr<Queue<Item> > queue,
                              Ptr<NetDeviceQueueInterface> ndqi,
                              uint8_t txq, Ptr<const Item> item);

  /**
   * \brief Perform the actions required by flow control and dynamic queue
   *        limits when a packet is dequeued (or dropped after dequeue) from
   *        the queue of a netdevice
   *
   * \param queue the device queue
   * \param ndqi the NetDeviceQueueInterface object aggregated to the device
   * \param txq the index of the transmission queue associated with the device queue
   * \param item the dequeued (or dropped after dequeue) packet
   *
   * This method must be connected to the "Dequeue" and "DropAfterDequeue"
   * traced callbacks of a Queue object (through a bound callback) in order for
   * a netdevice to support flow control and dynamic queue limits.
   */
  template <typename Item>
  static void PacketDequeued (Ptr<Queue<Item> > queue,
                              Ptr<NetDeviceQueueInterface> ndqi,
                              uint8_t txq, Ptr<const Item> item);

  /**
   * \brief Perform the actions required by flow control and dynamic queue
   *        limits when a packet is dropped before being enqueued in the queue
   *        of a netdevice (which likely indicates that the queue is full)
   *
   * \param queue the device queue
   * \param ndqi the NetDeviceQueueInterface object aggregated to the device
   * \param txq the index of the transmission queue associated with the device queue
   * \param item the dropped packet
   *
   * This method must be connected to the "DropBeforeEnqueue" traced callback
   * of a Queue object (through a bound callback) in order for a netdevice to
   * support flow control and dynamic queue limits.
   */
  template <typename Item>
  static void PacketDiscarded (Ptr<Queue<Item> > queue,
                               Ptr<NetDeviceQueueInterface> ndqi,
                               uint8_t txq, Ptr<const Item> item);

private:
  bool m_stoppedByDevice;         //!< True if the queue has been stopped by the device
  bool m_stoppedByQueueLimits;    //!< True if the queue has been stopped by a queue limits object
  Ptr<QueueLimits> m_queueLimits; //!< Queue limits object
  WakeCallback m_wakeCallback;    //!< Wake callback
};


/**
 * \ingroup netdevice
 *
 * \brief Network device transmission queue interface
 *
 * This interface is used by the traffic control layer and by the aggregated
 * device to access the transmission queues of the device. Additionally, through
 * this interface, traffic control aware netdevices can:
 * - set the number of transmission queues
 * - set the method used (by upper layers) to determine the transmission queue
 *   in which the netdevice would enqueue a given packet
 * This interface is created and aggregated to a device by the traffic control
 * layer when an Ipv{4,6}Interface is added to the device or a queue disc is
 * installed on the device.
 */
class NetDeviceQueueInterface : public Object
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Constructor
   *
   * Creates one NetDeviceQueue by default
   */
  NetDeviceQueueInterface ();
  virtual ~NetDeviceQueueInterface ();

  /**
   * \brief Get the i-th transmission queue of the device.
   *
   * \param i the index of the requested queue.
   * \return the i-th transmission queue of the device.
   *
   * The index of the first transmission queue is zero.
   */
  Ptr<NetDeviceQueue> GetTxQueue (uint8_t i) const;

  /**
   * \brief Get the number of device transmission queues.
   * \return the number of device transmission queues.
   */
  uint8_t GetNTxQueues (void) const;

  /**
   * \brief Set the number of device transmission queues to create.
   * \param numTxQueues number of device transmission queues to create.
   *
   * A multi-queue netdevice must call this method from within its
   * NotifyNewAggregate method to set the number of device transmission queues
   * to create.
   */
  void SetTxQueuesN (uint8_t numTxQueues);

  /**
   * \brief Create the device transmission queues.
   *
   * Called by the traffic control layer just after aggregating this netdevice
   * queue interface to the netdevice.
   */
  void CreateTxQueues (void);

  /**
   * \brief Get the value of the late TX queues creation flag.
   * \return the value of the late TX queues creation flag.
   */
  bool GetLateTxQueuesCreation (void) const;

  /**
   * \brief Set the late TX queues creation flag.
   * \param value the boolean value
   *
   * By default, the late TX queues creation flag is false, which leads the
   * traffic control layer to create the TX queues right after the netdevice
   * queue interface is aggregated to the device. Netdevices that want to
   * explicitly create TX queues at a later stage need to set this flag to
   * true in the NotifyNewAggregate method.
   */
  void SetLateTxQueuesCreation (bool value);

  /// Callback invoked to determine the tx queue selected for a given packet
  typedef Callback< uint8_t, Ptr<QueueItem> > SelectQueueCallback;

  /**
   * \brief Set the select queue callback.
   * \param cb the callback to set.
   *
   * A multi-queue netdevice must call this method from within its
   * NotifyNewAggregate method to set the select queue callback, i.e., the
   * method used to select a device transmission queue for a given packet.
   */
  void SetSelectQueueCallback (SelectQueueCallback cb);

  /**
   * \brief Get the select queue callback.
   * \return the select queue callback.
   *
   * Called by the traffic control layer to get the select queue callback set
   * by a multi-queue device.
   */
  SelectQueueCallback GetSelectQueueCallback (void) const;

  /**
   * \brief Connect the traced callbacks of a queue to the static methods of the
   *        NetDeviceQueue class to support flow control and dynamic queue limits
   * \param queue the queue
   * \param txq the index of the tx queue
   */
  template <typename Item>
  void ConnectQueueTraces (Ptr<Queue<Item> > queue, uint8_t txq);

protected:
  /**
   * \brief Dispose of the object
   */
  virtual void DoDispose (void);

private:
  std::vector< Ptr<NetDeviceQueue> > m_txQueuesVector;   //!< Device transmission queues
  SelectQueueCallback m_selectQueueCallback;   //!< Select queue callback
  uint8_t m_numTxQueues;   //!< Number of transmission queues to create
  bool m_lateTxQueuesCreation;   //!< True if a device wants to create the TX queues by itself
  std::map<Ptr<QueueBase>, std::vector<CallbackBase> > m_traceMap;   //!< Map storing all the connected traces
};


/**
 * Implementation of the templates declared above.
 */

template <typename Item>
void
NetDeviceQueueInterface::ConnectQueueTraces (Ptr<Queue<Item> > queue, uint8_t txq)
{
  NS_ASSERT (queue != 0);
  NS_ASSERT (txq < GetNTxQueues ());

  m_traceMap.emplace (queue, std::initializer_list<CallbackBase> {
                               MakeBoundCallback (&NetDeviceQueue::PacketEnqueued<Item>, queue, this, txq),
                               MakeBoundCallback (&NetDeviceQueue::PacketDequeued<Item>, queue, this, txq),
                               MakeBoundCallback (&NetDeviceQueue::PacketDiscarded<Item>, queue, this, txq) });

  queue->TraceConnectWithoutContext ("Enqueue", m_traceMap[queue][0]);
  queue->TraceConnectWithoutContext ("Dequeue", m_traceMap[queue][1]);
  queue->TraceConnectWithoutContext ("DropAfterDequeue", m_traceMap[queue][1]);
  queue->TraceConnectWithoutContext ("DropBeforeEnqueue", m_traceMap[queue][2]);
}

template <typename Item>
void
NetDeviceQueue::PacketEnqueued (Ptr<Queue<Item> > queue,
                                Ptr<NetDeviceQueueInterface> ndqi,
                                uint8_t txq, Ptr<const Item> item)
{
  NS_LOG_STATIC_TEMPLATE_DEFINE ("NetDeviceQueueInterface");

  NS_LOG_FUNCTION (queue << ndqi << txq << item);

  // Inform BQL
  ndqi->GetTxQueue (txq)->NotifyQueuedBytes (item->GetSize ());

  uint16_t mtu = ndqi->GetObject<NetDevice> ()->GetMtu ();

  // After enqueuing a packet, we need to check whether the queue is able to
  // store another packet. If not, we stop the queue

  if ((queue->GetMode () == QueueBase::QUEUE_MODE_PACKETS &&
       queue->GetNPackets () >= queue->GetMaxPackets ()) ||
      (queue->GetMode () == QueueBase::QUEUE_MODE_BYTES &&
       queue->GetNBytes () + mtu > queue->GetMaxBytes ()))
    {
      NS_LOG_DEBUG ("The device queue is being stopped (" << queue->GetNPackets ()
                    << " packets and " << queue->GetNBytes () << " bytes inside)");
      ndqi->GetTxQueue (txq)->Stop ();
    }
}

template <typename Item>
void
NetDeviceQueue::PacketDequeued (Ptr<Queue<Item> > queue,
                                Ptr<NetDeviceQueueInterface> ndqi,
                                uint8_t txq, Ptr<const Item> item)
{
  NS_LOG_STATIC_TEMPLATE_DEFINE ("NetDeviceQueueInterface");

  NS_LOG_FUNCTION (queue << ndqi << txq << item);

  // Inform BQL
  ndqi->GetTxQueue (txq)->NotifyTransmittedBytes (item->GetSize ());

  uint16_t mtu = ndqi->GetObject<NetDevice> ()->GetMtu ();

  // After dequeuing a packet, if there is room for another packet we
  // call Wake () that ensures that the queue is not stopped and restarts
  // the queue disc if the queue was stopped

  if ((queue->GetMode () == QueueBase::QUEUE_MODE_PACKETS &&
       queue->GetNPackets () < queue->GetMaxPackets ()) ||
      (queue->GetMode () == QueueBase::QUEUE_MODE_BYTES &&
       queue->GetNBytes () + mtu <= queue->GetMaxBytes ()))
    {
      ndqi->GetTxQueue (txq)->Wake ();
    }
}

template <typename Item>
void
NetDeviceQueue::PacketDiscarded (Ptr<Queue<Item> > queue,
                                 Ptr<NetDeviceQueueInterface> ndqi,
                                 uint8_t txq, Ptr<const Item> item)
{
  NS_LOG_STATIC_TEMPLATE_DEFINE ("NetDeviceQueueInterface");

  NS_LOG_FUNCTION (queue << ndqi << txq << item);

  // This method is called when a packet is discarded before being enqueued in the
  // device queue, likely because the queue is full. This should not happen if the
  // device correctly stops the queue. Anyway, stop the tx queue, so that the upper
  // layers do not send packets until there is room in the queue again.

  NS_LOG_ERROR ("BUG! No room in the device queue for the received packet! ("
                << queue->GetNPackets () << " packets and " << queue->GetNBytes () << " bytes inside)");

  ndqi->GetTxQueue (txq)->Stop ();
}

} // namespace ns3

#endif /* NET_DEVICE_QUEUE_INTERFACE_H */
