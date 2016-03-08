/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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

// The queue base class has a limit on its size, in terms of number of
// packets or number of bytes depending on the operating mode.
// The base class implements tracing and basic statistics calculations.

#ifndef QUEUE_H
#define QUEUE_H

#include "ns3/packet.h"
#include "ns3/object.h"
#include "ns3/traced-callback.h"
#include "ns3/net-device.h"
#include "ns3/traced-value.h"

namespace ns3 {

/**
 * \ingroup network
 * \defgroup queue Queue
 */
/**
 * \ingroup queue
 * \brief Abstract base class for packet Queues
 * 
 * This class defines the base APIs for packet queues in the ns-3 system
 */
class Queue : public Object
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  Queue ();
  virtual ~Queue ();

  /**
   * \return true if the queue is empty; false otherwise
   */
  bool IsEmpty (void) const;
  /**
   * Place a queue item into the rear of the Queue
   * \param item item to enqueue
   * \return True if the operation was successful; false otherwise
   */
  bool Enqueue (Ptr<QueueItem> item);
  /**
   * Remove an item from the front of the Queue
   * \return 0 if the operation was not successful; the item otherwise.
   */
  Ptr<QueueItem> Dequeue (void);
  /**
   * Get a copy of the item at the front of the queue without removing it
   * \return 0 if the operation was not successful; the item otherwise.
   */
  Ptr<const QueueItem> Peek (void) const;

  /**
   * Flush the queue.
   */
  void DequeueAll (void);
  /**
   * \return The number of packets currently stored in the Queue
   */
  uint32_t GetNPackets (void) const;
  /**
   * \return The number of bytes currently occupied by the packets in the Queue
   */
  uint32_t GetNBytes (void) const;

  /**
   * \return The total number of bytes received by this Queue since the
   * simulation began, or since ResetStatistics was called, according to 
   * whichever happened more recently
   * 
   */
  uint32_t GetTotalReceivedBytes (void) const;
  /**
   * \return The total number of packets received by this Queue since the
   * simulation began, or since ResetStatistics was called, according to 
   * whichever happened more recently
   */
  uint32_t GetTotalReceivedPackets (void) const;
  /**
   * \return The total number of bytes dropped by this Queue since the
   * simulation began, or since ResetStatistics was called, according to 
   * whichever happened more recently
   */
  uint32_t GetTotalDroppedBytes (void) const;
  /**
   * \return The total number of bytes dropped by this Queue since the
   * simulation began, or since ResetStatistics was called, according to 
   * whichever happened more recently
   */
  uint32_t GetTotalDroppedPackets (void) const;
  /**
   * Resets the counts for dropped packets, dropped bytes, received packets, and
   * received bytes.
   */
  void ResetStatistics (void);

  /**
   * \brief Enumeration of the modes supported in the class.
   *
   */
  enum QueueMode
  {
    QUEUE_MODE_PACKETS,     /**< Use number of packets for maximum queue size */
    QUEUE_MODE_BYTES,       /**< Use number of bytes for maximum queue size */
  };

  /**
   * Set the operating mode of this device.
   *
   * \param mode The operating mode of this device.
   */
  void SetMode (Queue::QueueMode mode);

  /**
   * Get the encapsulation mode of this device.
   *
   * \returns The encapsulation mode of this device.
   */
  Queue::QueueMode GetMode (void) const;

  /**
   * \brief Set the maximum amount of packets that can be stored in this queue
   *
   * \param maxPackets amount of packets
   */
  void SetMaxPackets (uint32_t maxPackets);

  /**
   * \return the maximum amount of packets that can be stored in this queue
   */
  uint32_t GetMaxPackets (void) const;

  /**
   * \brief Set the maximum amount of bytes that can be stored in this queue
   *
   * \param maxBytes amount of bytes
   */
  void SetMaxBytes (uint32_t maxBytes);

  /**
   * \return the maximum amount of bytes that can be stored in this queue
   */
  uint32_t GetMaxBytes (void) const;

#if 0
  // average calculation requires keeping around
  // a buffer with the date of arrival of past received packets
  // which are within the average window
  // so, it is quite costly to do it all the time.
  // Hence, it is disabled by default and must be explicitely
  // enabled with this method which specifies the size
  // of the average window in time units.
  void EnableRunningAverage (Time averageWindow);
  void DisableRunningAverage (void);
  // average
  double GetQueueSizeAverage (void);
  double GetReceivedBytesPerSecondAverage (void);
  double GetReceivedPacketsPerSecondAverage (void);
  double GetDroppedBytesPerSecondAverage (void);
  double GetDroppedPacketsPerSecondAverage (void);
  // variance
  double GetQueueSizeVariance (void);
  double GetReceivedBytesPerSecondVariance (void);
  double GetReceivedPacketsPerSecondVariance (void);
  double GetDroppedBytesPerSecondVariance (void);
  double GetDroppedPacketsPerSecondVariance (void);
#endif

protected:
  /**
   * \brief Drop a packet
   * \param p packet that was dropped
   *
   * This method is called by the base class when a packet is dropped because
   * the queue is full and by the subclasses to notify parent (this class) that
   * a packet has been dropped for other reasons.
   */
  void Drop (Ptr<Packet> p);

private:
  /**
   * Push an item in the queue
   * \param item the item to enqueue
   * \return true if success, false if the packet has been dropped.
   */
  virtual bool DoEnqueue (Ptr<QueueItem> item) = 0;
  /**
   * Pull an item from the queue
   * \return the item.
   */
  virtual Ptr<QueueItem> DoDequeue (void) = 0;
  /**
   * Peek the front item in the queue
   * \return the item.
   */
  virtual Ptr<const QueueItem> DoPeek (void) const = 0;

  /// Traced callback: fired when a packet is enqueued
  TracedCallback<Ptr<const Packet> > m_traceEnqueue;
  /// Traced callback: fired when a packet is dequeued
  TracedCallback<Ptr<const Packet> > m_traceDequeue;
  /// Traced callback: fired when a packet is dropped
  TracedCallback<Ptr<const Packet> > m_traceDrop;

  TracedValue<uint32_t> m_nBytes;   //!< Number of bytes in the queue
  uint32_t m_nTotalReceivedBytes;   //!< Total received bytes
  TracedValue<uint32_t> m_nPackets; //!< Number of packets in the queue
  uint32_t m_nTotalReceivedPackets; //!< Total received packets
  uint32_t m_nTotalDroppedBytes;    //!< Total dropped bytes
  uint32_t m_nTotalDroppedPackets;  //!< Total dropped packets

  uint32_t m_maxPackets;              //!< max packets in the queue
  uint32_t m_maxBytes;                //!< max bytes in the queue
  QueueMode m_mode;                   //!< queue mode (packets or bytes limited)
};

} // namespace ns3

#endif /* QUEUE_H */
