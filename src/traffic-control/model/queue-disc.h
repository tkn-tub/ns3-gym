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
#include "ns3/net-device.h"
#include "ns3/queue-item.h"
#include <vector>
#include <map>
#include <functional>
#include <string>
#include "packet-filter.h"

namespace ns3 {

class QueueDisc;
template <typename Item> class Queue;
class NetDeviceQueueInterface;

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
  virtual ~QueueDiscClass ();

  /**
   * \brief Get the queue disc attached to this class
   * \return the queue disc attached to this class.
   */
  Ptr<QueueDisc> GetQueueDisc (void) const;

  /**
   * \brief Set the queue disc attached to this class
   * \param qd The queue disc to attach to this class
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
 * The traffic control layer interacts with a queue disc in a simple manner: after
 * requesting to enqueue a packet, the traffic control layer requests the qdisc to
 * "run", i.e., to dequeue a set of packets, until a predefined number ("quota")
 * of packets is dequeued or the netdevice stops the queue disc. A netdevice shall
 * stop the queue disc when its transmission queue does not have room for another
 * packet. Also, a netdevice shall wake the queue disc when it detects that there
 * is room for another packet in its transmission queue, but the transmission queue
 * is stopped. Waking a queue disc is equivalent to make it run.
 *
 * Every queue disc collects statistics about the total number of packets/bytes
 * received from the upper layers (in case of root queue disc) or from the parent
 * queue disc (in case of child queue disc), enqueued, dequeued, requeued, dropped,
 * dropped before enqueue, dropped after dequeue, queued in the queue disc and
 * sent to the netdevice or to the parent queue disc. Note that packets that are
 * dequeued may be requeued, i.e., retained by the traffic control infrastructure,
 * if the netdevice is not ready to receive them. Requeued packets are not part
 * of the queue disc. The following identities hold:
 * - dropped = dropped before enqueue + dropped after dequeue
 * - received = dropped before enqueue + enqueued
 * - queued = enqueued - dequeued
 * - sent = dequeued - dropped after dequeue (- 1 if there is a requeued packet)
 *
 * Separate counters are also kept for each possible reason to drop a packet.
 * When a packet is dropped by an internal queue, e.g., because the queue is full,
 * the reason is "Dropped by internal queue". When a packet is dropped by a child
 * queue disc, the reason is "(Dropped by child queue disc) " followed by the
 * reason why the child queue disc dropped the packet.
 *
 * The QueueDisc base class provides the SojournTime trace source, which provides
 * the sojourn time of every packet dequeued from a queue disc, including packets
 * that are dropped or requeued after being dequeued. The sojourn time is taken
 * when the packet is dequeued from the queue disc, hence it does not account for
 * the additional time the packet is retained within the traffic control
 * infrastructure in case it is requeued.
 *
 * The design and implementation of this class is heavily inspired by Linux.
 * For more details, see the traffic-control model page.
 */
class QueueDisc : public Object {
public:

  /// \brief Structure that keeps the queue disc statistics
  struct Stats
  {
    /// Total received packets
    uint32_t nTotalReceivedPackets;
    /// Total received bytes
    uint64_t nTotalReceivedBytes;
    /// Total sent packets -- this value is not kept up to date, call GetStats first
    uint32_t nTotalSentPackets;
    /// Total sent bytes -- this value is not kept up to date, call GetStats first
    uint64_t nTotalSentBytes;
    /// Total enqueued packets
    uint32_t nTotalEnqueuedPackets;
    /// Total enqueued bytes
    uint64_t nTotalEnqueuedBytes;
    /// Total dequeued packets
    uint32_t nTotalDequeuedPackets;
    /// Total dequeued bytes
    uint64_t nTotalDequeuedBytes;
    /// Total dropped packets
    uint32_t nTotalDroppedPackets;
    /// Total packets dropped before enqueue
    uint32_t nTotalDroppedPacketsBeforeEnqueue;
    /// Packets dropped before enqueue, for each reason
    std::map<std::string, uint32_t> nDroppedPacketsBeforeEnqueue;
    /// Total packets dropped after dequeue
    uint32_t nTotalDroppedPacketsAfterDequeue;
    /// Packets dropped after dequeue, for each reason
    std::map<std::string, uint32_t> nDroppedPacketsAfterDequeue;
    /// Total dropped bytes
    uint64_t nTotalDroppedBytes;
    /// Total bytes dropped before enqueue
    uint64_t nTotalDroppedBytesBeforeEnqueue;
    /// Bytes dropped before enqueue, for each reason
    std::map<std::string, uint64_t> nDroppedBytesBeforeEnqueue;
    /// Total bytes dropped after dequeue
    uint64_t nTotalDroppedBytesAfterDequeue;
    /// Bytes dropped after dequeue, for each reason
    std::map<std::string, uint64_t> nDroppedBytesAfterDequeue;
    /// Total requeued packets
    uint32_t nTotalRequeuedPackets;
    /// Total requeued bytes
    uint64_t nTotalRequeuedBytes;
    /// Total marked packets
    uint32_t nTotalMarkedPackets;
    /// Marked packets, for each reason
    std::map<std::string, uint32_t> nMarkedPackets;
    /// Total marked bytes
    uint32_t nTotalMarkedBytes;
    /// Marked bytes, for each reason
    std::map<std::string, uint64_t> nMarkedBytes;

    /// constructor
    Stats ();

    /**
     * \brief Get the number of packets dropped for the given reason
     * \param reason the reason why packets were dropped
     * \return the number of packets dropped for the given reason
     */
    uint32_t GetNDroppedPackets (std::string reason) const;
    /**
     * \brief Get the amount of bytes dropped for the given reason
     * \param reason the reason why packets were dropped
     * \return the amount of bytes dropped for the given reason
     */
    uint64_t GetNDroppedBytes (std::string reason) const;
    /**
     * \brief Get the number of packets marked for the given reason
     * \param reason the reason why packets were marked
     * \return the number of packets marked for the given reason
     */
    uint32_t GetNMarkedPackets (std::string reason) const;
    /**
     * \brief Get the amount of bytes marked for the given reason
     * \param reason the reason why packets were marked
     * \return the amount of bytes marked for the given reason
     */
    uint64_t GetNMarkedBytes (std::string reason) const;
    /**
     * \brief Print the statistics.
     * \param os output stream in which the data should be printed.
     */
    void Print (std::ostream &os) const;
  };

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  QueueDisc ();
  virtual ~QueueDisc ();

  /**
   * \brief Get the number of packets stored by the queue disc
   * \return the number of packets stored by the queue disc.
   *
   * The requeued packet, if any, is counted.
   */
  uint32_t GetNPackets (void) const;

  /**
   * \brief Get the amount of bytes stored by the queue disc
   * \return the amount of bytes stored by the queue disc.
   *
   * The requeued packet, if any, is counted.
   */
  uint32_t GetNBytes (void) const;

  /**
   * \brief Retrieve all the collected statistics.
   * \return the collected statistics.
   */
  const Stats& GetStats (void);

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

  /// Internal queues store QueueDiscItem objects
  typedef Queue<QueueDiscItem> InternalQueue;

  /**
   * \brief Add an internal queue to the tail of the list of queues.
   * \param queue the queue to be added
   */
  void AddInternalQueue (Ptr<InternalQueue> queue);

  /**
   * \brief Get the i-th internal queue
   * \param i the index of the queue
   * \return the i-th internal queue.
   */
  Ptr<InternalQueue> GetInternalQueue (uint32_t i) const;

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
  virtual WakeMode GetWakeMode (void) const;

  // Reasons for dropping packets
  static constexpr const char* INTERNAL_QUEUE_DROP = "Dropped by internal queue";    //!< Packet dropped by an internal queue
  static constexpr const char* CHILD_QUEUE_DISC_DROP = "(Dropped by child queue disc) "; //!< Packet dropped by a child queue disc

protected:
  /**
   * \brief Dispose of the object
   */
  virtual void DoDispose (void);

  /**
   * \brief Check whether the configuration is correct and initialize parameters
   *
   * This method is not virtual to prevent subclasses from redefining it.
   * Subclasses must instead provide the implementation of the CheckConfig
   * and InitializeParams methods (which are called by this method).
   */
  void DoInitialize (void);

  /**
   *  \brief Perform the actions required when the queue disc is notified of
   *         a packet dropped before enqueue
   *  \param item item that was dropped
   *  \param reason the reason why the item was dropped
   *  This method must be called by subclasses to record that a packet was
   *  dropped before enqueue for the specified reason
   */
  void DropBeforeEnqueue (Ptr<const QueueDiscItem> item, const char* reason);

  /**
   *  \brief Perform the actions required when the queue disc is notified of
   *         a packet dropped after dequeue
   *  \param item item that was dropped
   *  \param reason the reason why the item was dropped
   *  This method must be called by subclasses to record that a packet was
   *  dropped after dequeue for the specified reason
   */
  void DropAfterDequeue (Ptr<const QueueDiscItem> item, const char* reason);

  /**
   *  \brief Marks the given packet and, if successful, updates the counters
   *         associated with the given reason
   *  \param item item that has to be marked
   *  \param reason the reason why the item has to be marked
   *  \return true if the item was successfully marked, false otherwise
   */
  bool Mark (Ptr<QueueDiscItem> item, const char* reason);

private:
  /**
   * \brief Copy constructor
   * \param o object to copy
   *
   * Defined and unimplemented to avoid misuse
   */
  QueueDisc (const QueueDisc &o);

  /**
   * \brief Assignment operator
   * \param o object to copy
   * \returns the copied object
   *
   * Defined and unimplemented to avoid misuse
   */
  QueueDisc &operator = (const QueueDisc &o);

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
   * \param item the packet to requeue
   */
  void Requeue (Ptr<QueueDiscItem> item);

  /**
   * Modelled after the Linux function sch_direct_xmit (net/sched/sch_generic.c)
   * Sends a packet to the device if the device queue is not stopped, and requeues
   * it otherwise.
   * \param item the packet to transmit
   * \return true if the device queue is not stopped and the queue disc is not empty
   */
  bool Transmit (Ptr<QueueDiscItem> item);

  /**
   *  \brief Perform the actions required when the queue disc is notified of
   *         a packet enqueue
   *  \param item item that was enqueued
   */
  void PacketEnqueued (Ptr<const QueueDiscItem> item);

  /**
   *  \brief Perform the actions required when the queue disc is notified of
   *         a packet dequeue
   *  \param item item that was dequeued
   */
  void PacketDequeued (Ptr<const QueueDiscItem> item);

  static const uint32_t DEFAULT_QUOTA = 64; //!< Default quota (as in /proc/sys/net/core/dev_weight)

  std::vector<Ptr<InternalQueue> > m_queues;    //!< Internal queues
  std::vector<Ptr<PacketFilter> > m_filters;    //!< Packet filters
  std::vector<Ptr<QueueDiscClass> > m_classes;  //!< Classes

  TracedValue<uint32_t> m_nPackets; //!< Number of packets in the queue
  TracedValue<uint32_t> m_nBytes;   //!< Number of bytes in the queue
  TracedValue<Time> m_sojourn;      //!< Sojourn time of the latest dequeued packet

  Stats m_stats;                    //!< The collected statistics
  uint32_t m_quota;                 //!< Maximum number of packets dequeued in a qdisc run
  Ptr<NetDevice> m_device;          //!< The NetDevice on which this queue discipline is installed
  Ptr<NetDeviceQueueInterface> m_devQueueIface;   //!< NetDevice queue interface
  bool m_running;                   //!< The queue disc is performing multiple dequeue operations
  Ptr<QueueDiscItem> m_requeued;    //!< The last packet that failed to be transmitted
  std::string m_childQueueDiscDropMsg;  //!< Reason why a packet was dropped by a child queue disc

  /// Traced callback: fired when a packet is enqueued
  TracedCallback<Ptr<const QueueDiscItem> > m_traceEnqueue;
  /// Traced callback: fired when a packet is dequeued
  TracedCallback<Ptr<const QueueDiscItem> > m_traceDequeue;
  /// Traced callback: fired when a packet is requeued
  TracedCallback<Ptr<const QueueDiscItem> > m_traceRequeue;
  /// Traced callback: fired when a packet is dropped
  TracedCallback<Ptr<const QueueDiscItem> > m_traceDrop;
  /// Traced callback: fired when a packet is dropped before enqueue
  TracedCallback<Ptr<const QueueDiscItem>, const char* > m_traceDropBeforeEnqueue;
  /// Traced callback: fired when a packet is dropped after dequeue
  TracedCallback<Ptr<const QueueDiscItem>, const char* > m_traceDropAfterDequeue;
  /// Traced callback: fired when a packet is marked
  TracedCallback<Ptr<const QueueDiscItem>, const char* > m_traceMark;

  /// Type for the function objects notifying that a packet has been dropped by an internal queue
  typedef std::function<void (Ptr<const QueueDiscItem>)> InternalQueueDropFunctor;
  /// Type for the function objects notifying that a packet has been dropped by a child queue disc
  typedef std::function<void (Ptr<const QueueDiscItem>, const char*)> ChildQueueDiscDropFunctor;

  /// Function object called when an internal queue dropped a packet before enqueue
  InternalQueueDropFunctor m_internalQueueDbeFunctor;
  /// Function object called when an internal queue dropped a packet after dequeue
  InternalQueueDropFunctor m_internalQueueDadFunctor;
  /// Function object called when a child queue disc dropped a packet before enqueue
  ChildQueueDiscDropFunctor m_childQueueDiscDbeFunctor;
  /// Function object called when a child queue disc dropped a packet after dequeue
  ChildQueueDiscDropFunctor m_childQueueDiscDadFunctor;
};

/**
 * \brief Stream insertion operator.
 *
 * \param os the stream
 * \param stats the queue disc statistics
 * \returns a reference to the stream
 */
std::ostream& operator<< (std::ostream& os, const QueueDisc::Stats &stats);

} // namespace ns3

#endif /* QueueDisc */
