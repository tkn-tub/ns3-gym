/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Natale Patriciello <natale.patriciello@gmail.com>
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
 */
#ifndef TCPCONGESTIONOPS_H
#define TCPCONGESTIONOPS_H

#include "ns3/object.h"
#include "ns3/timer.h"
#include "ns3/tcp-socket-base.h"

namespace ns3 {

/**
 * \brief Congestion control abstract class
 *
 * The design is inspired on what Linux v4.0 does (but it has been
 * in place since years). The congestion control is splitted from the main
 * socket code, and it is a pluggable component. An interface has been defined;
 * variables are maintained in the TcpSocketState class, while subclasses of
 * TcpCongestionOps operate over an instance of that class.
 *
 * Only three methods has been utilized right now; however, Linux has many others,
 * which can be added later in ns-3.
 *
 * \see IncreaseWindow
 * \see PktsAcked
 */
class TcpCongestionOps : public Object
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  TcpCongestionOps ();

  /**
   * \brief Copy constructor.
   * \param other object to copy.
   */
  TcpCongestionOps (const TcpCongestionOps &other);

  virtual ~TcpCongestionOps ();

  /**
   * \brief Get the name of the congestion control algorithm
   *
   * \return A string identifying the name
   */
  virtual std::string GetName () const = 0;

  /**
   * \brief Get the slow start threshold after a loss event
   *
   * Is guaranteed that the congestion control state (TcpAckState_t) is
   * changed BEFORE the invocation of this method.
   * The implementator should return the slow start threshold (and not change
   * it directly) because, in the future, the TCP implementation may require to
   * instantly recover from a loss event (e.g. when there is a network with an high
   * reordering factor).
   *
   * \param tcb internal congestion state
   * \param bytesInFlight total bytes in flight
   * \return Slow start threshold
   */
  virtual uint32_t GetSsThresh (Ptr<const TcpSocketState> tcb,
                                uint32_t bytesInFlight) = 0;

  /**
   * \brief Congestion avoidance algorithm implementation
   *
   * Mimic the function cong_avoid in Linux. New segments have been ACKed,
   * and the congestion control duty is to set
   *
   * The function is allowed to change directly cWnd and/or ssThresh.
   *
   * \param tcb internal congestion state
   * \param segmentsAcked count of segments acked
   */
  virtual void IncreaseWindow (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked) = 0;

  /**
   * \brief Timing information on received ACK
   *
   * The function is called every time an ACK is received (only one time
   * also for cumulative ACKs) and contains timing information. It is
   * optional (congestion controls can not implement it) and the default
   * implementation does nothing.
   *
   * \param tcb internal congestion state
   * \param segmentsAcked count of segments acked
   * \param rtt last rtt
   */
  virtual void PktsAcked (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked,
                          const Time& rtt)
  {
  }

  /**
   * \brief Trigger events/calculations specific to a congestion state
   *
   * This function mimics the function set_state in Linux.
   * The function is called before changing congestion state.
   *
   * \param tcb internal congestion state
   * \param newState new congestion state to which the TCP is going to switch
   */
  virtual void CongestionStateSet (Ptr<TcpSocketState> tcb,
                                   const TcpSocketState::TcpCongState_t newstate)
  {
  }

  // Present in Linux but not in ns-3 yet:
  /* call when cwnd event occurs (optional) */
  // void (*cwnd_event)(struct sock *sk, enum tcp_ca_event ev);
  /* call when ack arrives (optional) */
  // void (*in_ack_event)(struct sock *sk, u32 flags);
  /* new value of cwnd after loss (optional) */
  // u32  (*undo_cwnd)(struct sock *sk);
  /* hook for packet ack accounting (optional) */
  // void (*pkts_acked)(struct sock *sk, u32 num_acked, s32 rtt_us);

  /**
   * \brief Copy the congestion control algorithm across socket
   *
   * \return a pointer of the copied object
   */
  virtual Ptr<TcpCongestionOps> Fork () = 0;
};

/**
 * \brief The NewReno implementation
 *
 * New Reno introduces partial ACKs inside the well-established Reno algorithm.
 * This and other modifications are described in RFC 6582.
 *
 * \see IncreaseWindow
 */
class TcpNewReno : public TcpCongestionOps
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  TcpNewReno ();

  /**
   * \brief Copy constructor.
   * \param sock object to copy.
   */
  TcpNewReno (const TcpNewReno& sock);

  ~TcpNewReno ();

  std::string GetName () const;

  virtual void IncreaseWindow (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked);
  virtual uint32_t GetSsThresh (Ptr<const TcpSocketState> tcb,
                                uint32_t bytesInFlight);

  virtual Ptr<TcpCongestionOps> Fork ();

protected:
  virtual uint32_t SlowStart (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked);
  virtual void CongestionAvoidance (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked);
};

} // namespace ns3

#endif // TCPCONGESTIONOPS_H
