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

#ifndef QUEUE_DISC_H
#define QUEUE_DISC_H

#include "ns3/object.h"
#include "ns3/traced-value.h"
#include <ns3/queue.h>
#include "ns3/net-device.h"
#include <vector>
#include "packet-filter.h"

namespace ns3 {

class Packet;
class QueueDisc;

/**
 * \ingroup traffic-control
 *
 * QueueDiscItem is the abstract base class for items that are stored in a queue
 * disc. It is derived from QueueItem (which only consists of a Ptr<Packet>)
 * to additionally store the destination MAC address, the
 * L3 protocol number and the transmission queue index,
 */
class QueueDiscItem : public QueueItem {
public:
  /**
   * \brief Create a queue disc item.
   * \param p the packet included in the created item.
   * \param addr the destination MAC address
   * \param protocol the L3 protocol number
   */
  QueueDiscItem (Ptr<Packet> p, const Address & addr, uint16_t protocol);

  virtual ~QueueDiscItem ();

  /**
   * \brief Get the MAC address included in this item
   * \return the MAC address included in this item.
   */
  Address GetAddress (void) const;

  /**
   * \brief Get the L3 protocol included in this item
   * \return the L3 protocol included in this item.
   */
  uint16_t GetProtocol (void) const;

  /**
   * \brief Get the transmission queue index included in this item
   * \return the transmission queue index included in this item.
   */
  uint8_t GetTxQueueIndex (void) const;

  /**
   * \brief Set the transmission queue index to store in this item
   * \param txq the transmission queue index to store in this item.
   */
  void SetTxQueueIndex (uint8_t txq);

  /**
   * \brief Add the header to the packet
   *
   * Subclasses may keep header and payload separate to allow manipulating the header,
   * so this method allows to add the header to the packet before sending the packet
   * to the device.
   */
  virtual void AddHeader (void) = 0;

  /**
   * \brief Print the item contents.
   * \param os output stream in which the data should be printed.
   */
  virtual void Print (std::ostream &os) const;

private:
  /**
   * \brief Default constructor
   *
   * Defined and unimplemented to avoid misuse
   */
  QueueDiscItem ();
  /**
   * \brief Copy constructor
   *
   * Defined and unimplemented to avoid misuse
   */
  QueueDiscItem (const QueueDiscItem &);
  /**
   * \brief Assignment operator
   *
   * Defined and unimplemented to avoid misuse
   * \returns
   */
  QueueDiscItem &operator = (const QueueDiscItem &);

  Address m_address;      //!< MAC destination address
  uint16_t m_protocol;    //!< L3 Protocol number
  uint8_t m_txq;          //!< Transmission queue index
};


/**
 * \ingroup traffic-control
 *
 * QueueDiscClass is the base class for classes that are included in a queue
 * disc. It has a single attribute, QueueDisc, used to set the child queue disc
 * attached to the class. Classful queue discs needing to set parameters for
 * their classes can subclass QueueDiscClass and add the required parameters
 * as attributes.
 */
class QueueDiscClass : public Object {
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  QueueDiscClass ();
  virtual ~QueueDiscClass () {}

  /**
   * \brief Get the queue disc attached to this class
   * \return the queue disc attached to this class.
   */
  Ptr<QueueDisc> GetQueueDisc (void) const;

  /**
   * \brief Set the queue disc attached to this class
   */
  void SetQueueDisc (Ptr<QueueDisc> qd);

protected:
  /**
   * \brief Dispose of the object
   */
  virtual void DoDispose (void);

private:
  Ptr<QueueDisc> m_queueDisc;        //!< Queue disc attached to this class
};


/**
 * \ingroup traffic-control
 *
 * QueueDisc is an abstract base class providing the interface and implementing
 * the operations common to all the queueing disciplines. Child classes
 * need to implement the methods used to enqueue a packet (DoEnqueue),
 * dequeue a single packet (DoDequeue), get a copy of the next packet
 * to extract (DoPeek), check whether the current configuration is correct
 * (CheckConfig).
 *
 * As in Linux, a queue disc may contain distinct elements:
 * - queues, which actually store the packets waiting for transmission
 * - classes, which allow to reserve a different treatment to different packets
 * - filters, which determine the queue or class which a packet is destined to
 *
 * Notice that a child queue disc must be attached to every class and a packet
 * filter is only able to classify packets of a single protocol. Also, while in Linux
 * some queue discs (e.g., fq-codel) use an internal classifier and do not make use of
 * packet filters, in ns-3 every queue disc including multiple queues or multiple classes
 * needs an external filter to classify packets (this is to avoid having the traffic-control
 * module depend on other modules such as internet).
 *
 * Queue disc configuration vary from queue disc to queue disc. A typical taxonomy divides
 * queue discs in classful (i.e., support classes) and classless (i.e., do not support
 * classes). More recently, after the appearance of multi-queue devices (such as Wifi),
 * some multi-queue aware queue discs have been introduced. Multi-queue aware queue discs
 * handle as many queues (or queue discs -- without using classes) as the number of
 * transmission queues used by the device on which the queue disc is installed.
 * An attempt is made, also, to enqueue each packet in the "same" queue both within the
 * queue disc and within the device.
 *
 * The traffic control layer interacts with a queue disc in a simple manner: after requesting
 * to enqueue a packet, the traffic control layer requests the qdisc to "run", i.e., to
 * dequeue a set of packets, until a predefined number ("quota") of packets is dequeued
 * or the netdevice stops the queue disc. A netdevice may stop the queue disc when its
 * transmission queue(s) is/are (almost) full. Also, a netdevice may wake the
 * queue disc when its transmission queue(s) is/are (almost) empty. Waking a queue disc
 * is equivalent to make it run.
 *
 * The design and implementation of this class is heavily inspired by Linux.
 * For more details, see the traffic-control model page.
 */
class QueueDisc : public Object {
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  QueueDisc ();

  /**
   * \brief Get the number of packets stored by the queue disc
   * \return the number of packets stored by the queue disc.
   *
   * Note that the number of packets stored by the queue disc is updated as soon
   * as a packet is received by the queue disc and before actually enqueuing the
   * packet (i.e., before calling DoEnqueue). Thus, while implementing the DoEnqueue
   * method of a subclass, keep in mind that GetNPackets returns the number of
   * packets stored in the queue disc, including the packet that we are trying
   * to enqueue.
   */
  uint32_t GetNPackets (void) const;

  /**
   * \brief Get the amount of bytes stored by the queue disc
   * \return the amount of bytes stored by the queue disc.
   *
   * Note that the amount of bytes stored by the queue disc is updated as soon
   * as a packet is received by the queue disc and before actually enqueuing the
   * packet (i.e., before calling DoEnqueue). Thus, while implementing the DoEnqueue
   * method of a subclass, keep in mind that GetNBytes returns the amount of
   * bytes stored in the queue disc, including the size of the packet that we are
   * trying to enqueue.
   */
  uint32_t GetNBytes (void) const;

  /**
   * \brief Get the total number of received packets
   * \return the total number of received packets.
   */
  uint32_t GetTotalReceivedPackets (void) const;

  /**
   * \brief Get the total amount of received bytes
   * \return the total amount of received bytes.
   */
  uint32_t GetTotalReceivedBytes (void) const;

  /**
   * \brief Get the total number of dropped packets
   * \return the total number of dropped packets.
   */
  uint32_t GetTotalDroppedPackets (void) const;

  /**
   * \brief Get the total amount of dropped bytes
   * \return the total amount of dropped bytes.
   */
  uint32_t GetTotalDroppedBytes (void) const;

  /**
   * \brief Get the total number of requeued packets
   * \return the total number of requeued packets.
   */
  uint32_t GetTotalRequeuedPackets (void) const;

  /**
   * \brief Get the total amount of requeued bytes
   * \return the total amount of requeued bytes.
   */
  uint32_t GetTotalRequeuedBytes (void) const;

  /**
   * \brief Set the NetDevice on which this queue discipline is installed.
   * \param device the NetDevice on which this queue discipline is installed.
   */
  void SetNetDevice (Ptr<NetDevice> device);

  /**
   * \brief Get the NetDevice on which this queue discipline is installed
   * \return the NetDevice on which this queue discipline is installed.
   */
  Ptr<NetDevice> GetNetDevice (void) const;

  /**
   * \brief Set the maximum number of dequeue operations following a packet enqueue
   * \param quota the maximum number of dequeue operations following a packet enqueue.
   */
  virtual void SetQuota (const uint32_t quota);

  /**
   * \brief Get the maximum number of dequeue operations following a packet enqueue
   * \return the maximum number of dequeue operations following a packet enqueue.
   */
  virtual uint32_t GetQuota (void) const;

  /**
   * Pass a packet to store to the queue discipline. This function only updates
   * the statistics and calls the (private) DoEnqueue function, which must be
   * implemented by derived classes.
   * \param item item to enqueue
   * \return True if the operation was successful; false otherwise
   */
  bool Enqueue (Ptr<QueueDiscItem> item);

  /**
   * Request the queue discipline to extract a packet. This function only updates
   * the statistics and calls the (private) DoDequeue function, which must be
   * implemented by derived classes.
   * \return 0 if the operation was not successful; the item otherwise.
   */
  Ptr<QueueDiscItem> Dequeue (void);

  /**
   * Get a copy of the next packet the queue discipline will extract, without
   * actually extracting the packet. This function only calls the (private)
   * DoPeek function, which must be implemented by derived classes.
   * \return 0 if the operation was not successful; the item otherwise.
   */
  Ptr<const QueueDiscItem> Peek (void) const;

  /**
   * Modelled after the Linux function __qdisc_run (net/sched/sch_generic.c)
   * Dequeues multiple packets, until a quota is exceeded or sending a packet
   * to the device failed.
   */
  void Run (void);

  /**
   * \brief Add an internal queue to the tail of the list of queues.
   * \param queue the queue to be added
   */
  void AddInternalQueue (Ptr<Queue> queue);

  /**
   * \brief Get the i-th internal queue
   * \param i the index of the queue
   * \return the i-th internal queue.
   */
  Ptr<Queue> GetInternalQueue (uint32_t i) const;

  /**
   * \brief Get the number of internal queues
   * \return the number of internal queues.
   */
  uint32_t GetNInternalQueues (void) const;

  /**
   * \brief Add a packet filter to the tail of the list of filters used to classify packets.
   * \param filter the packet filter to be added
   */
  void AddPacketFilter (Ptr<PacketFilter> filter);

  /**
   * \brief Get the i-th packet filter
   * \param i the index of the packet filter
   * \return the i-th packet filter.
   */
  Ptr<PacketFilter> GetPacketFilter (uint32_t i) const;

  /**
   * \brief Get the number of packet filters
   * \return the number of packet filters.
   */
  uint32_t GetNPacketFilters (void) const;

  /**
   * \brief Add a queue disc class to the tail of the list of classes.
   * \param qdClass the queue disc class to be added
   */
  void AddQueueDiscClass (Ptr<QueueDiscClass> qdClass);

  /**
   * \brief Get the i-th queue disc class
   * \param i the index of the queue disc class
   * \return the i-th queue disc class.
   */
  Ptr<QueueDiscClass> GetQueueDiscClass (uint32_t i) const;

  /**
   * \brief Get the number of queue disc classes
   * \return the number of queue disc classes.
   */
  uint32_t GetNQueueDiscClasses (void) const;

  /**
   * Classify a packet by calling the packet filters, one at a time, until either
   * a filter able to classify the packet is found or all the filters have been
   * processed.
   * \param item item to classify
   * \return -1 if no filter able to classify the packet has been found, the value
   * returned by first filter found to be able to classify the packet otherwise.
   */
  int32_t Classify (Ptr<QueueDiscItem> item);

  /**
   * \enum WakeMode
   * \brief Used to determine whether the queue disc itself or its children must
   *        be activated when a netdevice wakes a transmission queue
   */
  enum WakeMode
    {
      WAKE_ROOT = 0x00,
      WAKE_CHILD = 0x01
    };

  /**
   * When setting up the wake callbacks on the netdevice queues, it is necessary to
   * determine which queue disc (the root queue disc or one of its children) should
   * be activated when the netdevice wakes one of its transmission queues. The
   * implementation of this method for the base class returns WAKE_ROOT, i.e., the
   * root queue disc is activated. Subclasses implementing queue discs adopting
   * a different strategy (e.g., multi-queue aware queue discs such as mq) have
   * to redefine this method.
   *
   * \return the wake mode adopted by this queue disc.
   */
  WakeMode GetWakeMode (void);

protected:
  /**
   * \brief Dispose of the object
   */
  virtual void DoDispose (void);

  /**
   * \brief Check whether the configuration is correct and initialize parameters
   */
  virtual void DoInitialize (void);

  /**
   *  \brief Drop a packet
   *  \param item item that was dropped
   *  This method is called by subclasses to notify parent (this class) of packet drops.
   */
  void Drop (Ptr<QueueDiscItem> item);

private:

  /**
   * This function actually enqueues a packet into the queue disc.
   * \param item item to enqueue
   * \return True if the operation was successful; false otherwise
   */
  virtual bool DoEnqueue (Ptr<QueueDiscItem> item) = 0;

  /**
   * This function actually extracts a packet from the queue disc.
   * \return 0 if the operation was not successful; the item otherwise.
   */
  virtual Ptr<QueueDiscItem> DoDequeue (void) = 0;

  /**
   * This function returns a copy of the next packet the queue disc will extract.
   * \return 0 if the operation was not successful; the packet otherwise.
   */
  virtual Ptr<const QueueDiscItem> DoPeek (void) const = 0;

  /**
   * Check whether the current configuration is correct. Default objects (such
   * as internal queues) might be created by this method to ensure the
   * configuration is correct.
   * \return true if the configuration is correct, false otherwise
   */
  virtual bool CheckConfig (void) = 0;

  /**
   * Initialize parameters (if any) before the first packet is enqueued.
   */
  virtual void InitializeParams (void) = 0;

  /**
   * Modelled after the Linux function qdisc_run_begin (include/net/sch_generic.h).
   * \return false if the qdisc is already running; otherwise, set the qdisc as running and return true.
   */
  bool RunBegin (void);

  /**
   * Modelled after the Linux function qdisc_run_end (include/net/sch_generic.h).
   * Set the qdisc as not running.
   */
  void RunEnd (void);

  /**
   * Modelled after the Linux function qdisc_restart (net/sched/sch_generic.c)
   * Dequeue a packet (by calling DequeuePacket) and send it to the device (by calling Transmit).
   * \return true if a packet is successfully sent to the device.
   */
  bool Restart (void);

  /**
   * Modelled after the Linux function dequeue_skb (net/sched/sch_generic.c)
   * \return the requeued packet, if any, or the packet dequeued by the queue disc, otherwise.
   */
  Ptr<QueueDiscItem> DequeuePacket (void);

  /**
   * Modelled after the Linux function dev_requeue_skb (net/sched/sch_generic.c)
   * Requeues a packet whose transmission failed.
   * \param p the packet to requeue
   */
  void Requeue (Ptr<QueueDiscItem> p);

  /**
   * Modelled after the Linux function sch_direct_xmit (net/sched/sch_generic.c)
   * Sends a packet to the device and requeues it in case transmission fails.
   * \param p the packet to transmit
   * \return true if the transmission succeeded and the queue is not stopped
   */
  bool Transmit (Ptr<QueueDiscItem> p);

  static const uint32_t DEFAULT_QUOTA = 64; //!< Default quota (as in /proc/sys/net/core/dev_weight)

  std::vector<Ptr<Queue> > m_queues;            //!< Internal queues
  std::vector<Ptr<PacketFilter> > m_filters;    //!< Packet filters
  std::vector<Ptr<QueueDiscClass> > m_classes;  //!< Classes

  TracedValue<uint32_t> m_nPackets; //!< Number of packets in the queue
  TracedValue<uint32_t> m_nBytes;   //!< Number of bytes in the queue

  uint32_t m_nTotalReceivedPackets; //!< Total received packets
  uint32_t m_nTotalReceivedBytes;   //!< Total received bytes
  uint32_t m_nTotalDroppedPackets;  //!< Total dropped packets
  uint32_t m_nTotalDroppedBytes;    //!< Total dropped bytes
  uint32_t m_nTotalRequeuedPackets; //!< Total requeued packets
  uint32_t m_nTotalRequeuedBytes;   //!< Total requeued bytes
  uint32_t m_quota;                 //!< Maximum number of packets dequeued in a qdisc run
  Ptr<NetDevice> m_device;          //!< The NetDevice on which this queue discipline is installed
  Ptr<NetDeviceQueueInterface> m_devQueueIface;   //!< NetDevice queue interface
  bool m_running;                   //!< The queue disc is performing multiple dequeue operations
  bool m_InitCompleted;             //!< True if the config has been verified and the parameters initialized
  Ptr<QueueDiscItem> m_requeued;    //!< The last packet that failed to be transmitted

  /// Traced callback: fired when a packet is enqueued
  TracedCallback<Ptr<const QueueItem> > m_traceEnqueue;
    /// Traced callback: fired when a packet is dequeued
  TracedCallback<Ptr<const QueueItem> > m_traceDequeue;
    /// Traced callback: fired when a packet is requeued
  TracedCallback<Ptr<const QueueItem> > m_traceRequeue;
  /// Traced callback: fired when a packet is dropped
  TracedCallback<Ptr<const QueueItem> > m_traceDrop;
};

} // namespace ns3

#endif /* QueueDisc */
