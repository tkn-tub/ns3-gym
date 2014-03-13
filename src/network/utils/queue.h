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

// The queue base class does not have any limit based on the number
// of packets or number of bytes. It is, conceptually, infinite 
// by default. Only subclasses define limitations.
// The base class implements tracing and basic statistics calculations.

#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include <list>
#include "ns3/packet.h"
#include "ns3/object.h"
#include "ns3/traced-callback.h"

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
   * Place a packet into the rear of the Queue
   * \param p packet to enqueue
   * \return True if the operation was successful; false otherwise
   */
  bool Enqueue (Ptr<Packet> p);
  /**
   * Remove a packet from the front of the Queue
   * \return 0 if the operation was not successful; the packet otherwise.
   */
  Ptr<Packet> Dequeue (void);
  /**
   * Get a copy of the item at the front of the queue without removing it
   * \return 0 if the operation was not successful; the packet otherwise.
   */
  Ptr<const Packet> Peek (void) const;

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

private:

  /**
   * Push a packet in the queue
   * \param p the packet to enqueue
   * \return true if success, false if the packet has been dropped.
   */
  virtual bool DoEnqueue (Ptr<Packet> p) = 0;
  /**
   * Pull a packet from the queue
   * \return the packet.
   */
  virtual Ptr<Packet> DoDequeue (void) = 0;
  /**
   * Peek the front packet in the queue
   * \return the packet.
   */
  virtual Ptr<const Packet> DoPeek (void) const = 0;

protected:
  /**
   *  \brief Drop a packet 
   *  \param packet packet that was dropped
   *  This method is called by subclasses to notify parent (this class) of packet drops.
   */
  void Drop (Ptr<Packet> packet);

private:
  /// Traced callback: fired when a packet is enqueued
  TracedCallback<Ptr<const Packet> > m_traceEnqueue;
  /// Traced callback: fired when a packet is dequeued
  TracedCallback<Ptr<const Packet> > m_traceDequeue;
  /// Traced callback: fired when a packet is dropped
  TracedCallback<Ptr<const Packet> > m_traceDrop;

  uint32_t m_nBytes;                //!< Number of bytes in the queue
  uint32_t m_nTotalReceivedBytes;   //!< Total received bytes
  uint32_t m_nPackets;              //!< Number of packets in the queue
  uint32_t m_nTotalReceivedPackets; //!< Total received packets
  uint32_t m_nTotalDroppedBytes;    //!< Total dropped bytes
  uint32_t m_nTotalDroppedPackets;  //!< Total dropped packets
};

} // namespace ns3

#endif /* QUEUE_H */
