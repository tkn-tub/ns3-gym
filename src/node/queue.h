/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
 * All rights reserved.
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
#include "ns3/packet.h"
#include "ns3/callback-tracer.h"
#include "ns3/trace-container.h"

namespace ns3 {

class Queue
{
public:
  Queue (std::string const &name);
  virtual ~Queue ();

  bool Enque (const Packet& p);
  bool Deque (Packet &p);

  void DequeAll (void);
  uint32_t GetNPackets (void);
  uint32_t GetNBytes (void);

  bool IsEmpty (void);

  uint32_t GetTotalReceivedBytes (void);
  uint32_t GetTotalReceivedPackets (void);
  uint32_t GetTotalDroppedBytes (void);
  uint32_t GetTotalDroppedPackets (void);

  void ResetStatistics (void);

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
  virtual bool DoEnque (const Packet& p) = 0;
  virtual bool DoDeque (Packet &p) = 0;

protected:
  // called by subclasses to notify parent of packet drops.
  void Drop (const Packet& p);
  void QueueRegisterTraces (TraceContainer &container);

private:
  CallbackTracer<std::string const &, const Packet &> m_traceEnque;
  CallbackTracer<std::string const &, const Packet &> m_traceDeque;
  CallbackTracer<std::string const &, const Packet &> m_traceDrop;

  uint32_t m_nBytes;
  uint32_t m_nTotalReceivedBytes;
  uint32_t m_nPackets;
  uint32_t m_nTotalReceivedPackets;
  uint32_t m_nTotalDroppedBytes;
  uint32_t m_nTotalDroppedPackets;

  std::string m_name;


#if 0
  // Static methods to manage queue default
  // Set desired queue default
  static void    Default(const Queue& q);
  // Return reference to the current default queue type
  static Queue&  Default();
  // Static variable pointing to current default queue
  static Queue* defaultQueue;
#endif
};

}; // namespace ns3

#endif /* QUEUE_H */
