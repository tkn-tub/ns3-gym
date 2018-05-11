/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018 NITK Surathkal
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
 * Author: Viyom Mittal <viyommittal@gmail.com>
 *         Vivek Jain <jain.vivek.anand@gmail.com>
 *         Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 *
 */
#ifndef TCPRECOVERYOPS_H
#define TCPRECOVERYOPS_H

#include "ns3/tcp-socket-base.h"

namespace ns3 {

/**
 * \ingroup tcp
 * \defgroup recoveryOps Recovery Algorithms.
 *
 * The various recovery algorithms used in recovery phase of TCP.
 */

/**
 * \ingroup recoveryOps
 *
 * \brief recovery abstract class
 *
 * The design is inspired by the TcpCongestionOps class in ns-3. The fast
 * recovery is splitted from the main socket code, and it is a pluggable
 * component. An interface has been defined; variables are maintained in the
 * TcpSocketState class, while subclasses of TcpRecoveryOps operate over an
 * instance of that class.
 *
 * \see DoRecovery
 */
class TcpRecoveryOps : public Object
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Constructor
   */
  TcpRecoveryOps ();

  /**
   * \brief Copy constructor.
   * \param other object to copy.
   */
  TcpRecoveryOps (const TcpRecoveryOps &other);

  /**
   * \brief Deconstructor
   */
  virtual ~TcpRecoveryOps ();

  /**
   * \brief Get the name of the recovery algorithm
   *
   * \return A string identifying the name
   */
  virtual std::string GetName () const = 0;

  /**
   * \brief Performs variable initialization at the start of recovery
   *
   * The function is called when the TcpSocketState is changed to CA_RECOVERY.
   *
   * \param tcb internal congestion state
   * \param dupAckCount duplicate acknowldgement count
   */
  virtual void EnterRecovery (Ptr<TcpSocketState> tcb, uint32_t dupAckCount)
  {
    NS_UNUSED (tcb);
    NS_UNUSED (dupAckCount);
  }

  /**
   * \brief Performs recovery based on the recovery algorithm
   *
   * The function is called on arrival of every ack when TcpSocketState
   * is set to CA_RECOVERY. It performs the necessary cwnd changes
   * as per the recovery algorithm.
   *
   * \param tcb internal congestion state
   */
  virtual void DoRecovery (Ptr<TcpSocketState> tcb)
  {
    NS_UNUSED (tcb);
  }

  /**
   * \brief Performs cwnd adjustments at the end of recovery
   *
   * The function is called when the TcpSocketState is changed from CA_RECOVERY.
   *
   * \param tcb internal congestion state
   * \param isSackEnabled
   */
  virtual void ExitRecovery (Ptr<TcpSocketState> tcb)
  {
    NS_UNUSED (tcb);
  }

  /**
   * \brief Copy the recovery algorithm across socket
   *
   * \return a pointer of the copied object
   */
  virtual Ptr<TcpRecoveryOps> Fork () = 0;
};

/**
 * \brief The Classic recovery implementation
 *
 * Classic recovery refers to the two well-established recovery algorithms,
 * namely, NewReno (RFC 6582) and SACK based recovery (RFC 6675).
 *
 * The idea of the algorithm is that when we enter recovery, we set the
 * congestion window value to the slow start threshold and maintain it
 * at such value until we are fully recovered (in other words, until
 * the highest sequence transmitted at time of detecting the loss is
 * ACKed by the receiver).
 *
 * \see DoRecovery
 */
class ClassicRecovery : public TcpRecoveryOps
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Constructor
   */
  ClassicRecovery ();

  /**
   * \brief Copy constructor.
   * \param recovery object to copy.
   */
  ClassicRecovery (const ClassicRecovery& recovery);

  /**
   * \brief Constructor
   */
  ~ClassicRecovery ();

  virtual std::string GetName () const override;

  virtual void EnterRecovery (Ptr<TcpSocketState> tcb, uint32_t dupAckCount) override;

  virtual void DoRecovery (Ptr<TcpSocketState> tcb) override;

  virtual void ExitRecovery (Ptr<TcpSocketState> tcb) override;

  virtual Ptr<TcpRecoveryOps> Fork () override;
};

} // namespace ns3

#endif // TCPRECOVERYOPS_H
