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

#ifndef CODEL_H
#define CODEL_H

#include "ns3/queue-disc.h"
#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include "ns3/string.h"
#include "ns3/traced-value.h"
#include "ns3/trace-source-accessor.h"

class CoDelQueueDiscNewtonStepTest;  // Forward declaration for unit test
class CoDelQueueDiscControlLawTest;  // Forward declaration for unit test

namespace ns3 {

/**
 * Number of bits discarded from the time representation.
 * The time is assumed to be in nanoseconds.
 */
static const int  CODEL_SHIFT = 10;

#define DEFAULT_CODEL_LIMIT 1000
#define REC_INV_SQRT_BITS (8 * sizeof(uint16_t))
#define REC_INV_SQRT_SHIFT (32 - REC_INV_SQRT_BITS)

class TraceContainer;

/**
 * \ingroup traffic-control
 *
 * \brief A CoDel packet queue disc
 */

class CoDelQueueDisc : public QueueDisc
{
public:
  /**
   * Get the type ID.
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * \brief CoDelQueueDisc Constructor
   *
   * Creates a CoDel queue
   */
  CoDelQueueDisc ();

  virtual ~CoDelQueueDisc ();

  /**
   * \brief Enumeration of the modes supported in the class.
   *
   */
  enum QueueDiscMode
  {
    QUEUE_DISC_MODE_PACKETS,     /**< Use number of packets for maximum queue disc size */
    QUEUE_DISC_MODE_BYTES,       /**< Use number of bytes for maximum queue disc size */
  };

  /**
   * \brief Set the operating mode of this queue disc.
   *
   * \param mode The operating mode of this queue disc.
   */
  void SetMode (QueueDiscMode mode);

  /**
   * \brief Get the operating mode of this queue disc.
   *
   * \returns The operating mode of this queue disc.
   */
  QueueDiscMode GetMode (void);

  /**
   * \brief Get the current value of the queue in bytes or packets.
   *
   * \returns The queue size in bytes or packets.
   */
  uint32_t GetQueueSize (void);

  /**
   * \brief Get the target queue delay
   *
   * \returns The target queue delay
   */
  Time GetTarget (void);

  /**
   * \brief Get the interval
   *
   * \returns The interval
   */
  Time GetInterval (void);

  /**
   * \brief Get the time for next packet drop while in the dropping state
   *
   * \returns The time for next packet drop
   */
  uint32_t GetDropNext (void);

  // Reasons for dropping packets
  static constexpr const char* TARGET_EXCEEDED_DROP = "Target exceeded drop";  //!< Sojourn time above target
  static constexpr const char* OVERLIMIT_DROP = "Overlimit drop";  //!< Overlimit dropped packet

private:
  friend class::CoDelQueueDiscNewtonStepTest;  // Test code
  friend class::CoDelQueueDiscControlLawTest;  // Test code
  /**
   * \brief Add a packet to the queue
   *
   * \param item The item to be added
   * \returns True if the packet can be added, False if the packet is dropped due to full queue
   */
  virtual bool DoEnqueue (Ptr<QueueDiscItem> item);

  /**
   * \brief Remove a packet from queue based on the current state
   * If we are in dropping state, check if we could leave the dropping state
   * or if we should perform next drop
   * If we are not currently in dropping state, check if we need to enter the state
   * and drop the first packet
   *
   * \returns The packet that is examined
   */
  virtual Ptr<QueueDiscItem> DoDequeue (void);

  virtual Ptr<const QueueDiscItem> DoPeek (void) const;
  virtual bool CheckConfig (void);

  /**
   * \brief Calculate the reciprocal square root of m_count by using Newton's method
   *  http://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Iterative_methods_for_reciprocal_square_roots
   * m_recInvSqrt (new) = (m_recInvSqrt (old) / 2) * (3 - m_count * m_recInvSqrt^2)
   */
  void NewtonStep (void);

  /**
   * \brief Determine the time for next drop
   * CoDel control law is t + m_interval/sqrt(m_count).
   * Here, we use m_recInvSqrt calculated by Newton's method in NewtonStep() to avoid
   * both sqrt() and divide operations
   *
   * \param t Current next drop time
   * \returns The new next drop time:
   */
  uint32_t ControlLaw (uint32_t t);

  /**
   * \brief Determine whether a packet is OK to be dropped. The packet
   * may not be actually dropped (depending on the drop state)
   *
   * \param item The packet that is considered
   * \param now The current time represented as 32-bit unsigned integer (us)
   * \returns True if it is OK to drop the packet (sojourn time above target for at least interval)
   */
  bool OkToDrop (Ptr<QueueDiscItem> item, uint32_t now);

  /**
   * Check if CoDel time a is successive to b
   * @param a left operand
   * @param b right operand
   * @return true if a is greater than b
   */
  bool CoDelTimeAfter (uint32_t a, uint32_t b);
  /**
   * Check if CoDel time a is successive or equal to b
   * @param a left operand
   * @param b right operand
   * @return true if a is greater than or equal to b
   */
  bool CoDelTimeAfterEq (uint32_t a, uint32_t b);
  /**
   * Check if CoDel time a is preceding b
   * @param a left operand
   * @param b right operand
   * @return true if a is less than to b
   */
  bool CoDelTimeBefore (uint32_t a, uint32_t b);
  /**
   * Check if CoDel time a is preceding or equal to b
   * @param a left operand
   * @param b right operand
   * @return true if a is less than or equal to b
   */
  bool CoDelTimeBeforeEq (uint32_t a, uint32_t b);

  /**
   * Return the unsigned 32-bit integer representation of the input Time
   * object. Units are microseconds
   * @param t the input Time Object
   * @return the unsigned 32-bit integer representation
   */
  uint32_t Time2CoDel (Time t);

  virtual void InitializeParams (void);

  uint32_t m_maxPackets;                  //!< Max # of packets accepted by the queue
  uint32_t m_maxBytes;                    //!< Max # of bytes accepted by the queue
  uint32_t m_minBytes;                    //!< Minimum bytes in queue to allow a packet drop
  Time m_interval;                        //!< 100 ms sliding minimum time window width
  Time m_target;                          //!< 5 ms target queue delay
  TracedValue<uint32_t> m_count;          //!< Number of packets dropped since entering drop state
  TracedValue<uint32_t> m_lastCount;      //!< Last number of packets dropped since entering drop state
  TracedValue<bool> m_dropping;           //!< True if in dropping state
  uint16_t m_recInvSqrt;                  //!< Reciprocal inverse square root
  uint32_t m_firstAboveTime;              //!< Time to declare sojourn time above target
  TracedValue<uint32_t> m_dropNext;       //!< Time to drop next packet
  uint32_t m_state1;                      //!< Number of times packet sojourn goes above target for interval
  uint32_t m_state2;                      //!< Number of times we perform next drop while in dropping state
  uint32_t m_state3;                      //!< Number of times we enter drop state and drop the fist packet
  uint32_t m_states;                      //!< Total number of times we are in state 1, state 2, or state 3
  QueueDiscMode m_mode;                   //!< The operating mode (Bytes or packets)
};

} // namespace ns3

#endif /* CODEL_H */
