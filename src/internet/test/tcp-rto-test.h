/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
#ifndef TCPRTOTEST_H
#define TCPRTOTEST_H

#include "tcp-fast-retr-test.h"

namespace ns3 {

/**
 * \brief Testing the moments after an RTO expiration
 *
 * The scope of this test is to be sure that, after an RTO expiration,
 * the TCP implementation set the correct state in the ACK state machine,
 * and marks the lost segment as lost; then, after the retransmission, the
 * state is fully recovered. This is the base check, where only one segment
 * (the first) is lost and retransmitted.
 *
 */
class TcpRtoTest : public TcpGeneralTest
{
public:
  TcpRtoTest (TypeId &congControl, const std::string &msg);

protected:

  virtual Ptr<TcpSocketMsgBase> CreateSenderSocket (Ptr<Node> node);
  virtual void RTOExpired (const Ptr<const TcpSocketState> tcb, SocketWho who);
  virtual void RcvAck      (const Ptr<const TcpSocketState> tcb,
                            const TcpHeader& h, SocketWho who);
  virtual void ProcessedAck (const Ptr<const TcpSocketState> tcb,
                             const TcpHeader& h, SocketWho who);
  virtual void FinalChecks ();

private:
  bool m_rtoExpired;
  bool m_segmentReceived;
};

/**
 * \brief Testing the timing of RTO
 *
 * Checking if RTO is doubled ONLY after a retransmission.
 */
class TcpTimeRtoTest : public TcpGeneralTest
{
public:
  TcpTimeRtoTest (TypeId &congControl, const std::string &msg);

protected:
  virtual Ptr<TcpSocketMsgBase> CreateSenderSocket (Ptr<Node> node);
  virtual Ptr<ErrorModel> CreateReceiverErrorModel ();
  virtual void ErrorClose  (SocketWho who);
  virtual void RTOExpired (const Ptr<const TcpSocketState> tcb, SocketWho who);
  virtual void Tx (const Ptr<const Packet> p, const TcpHeader&h, SocketWho who);
  virtual void FinalChecks ();

  void PktDropped (const Ipv4Header &ipH, const TcpHeader& tcpH, Ptr<const Packet> p);

private:
  uint32_t m_senderSentSegments;
  Time m_previousRTO;
  bool m_closed;
};

} // namespace ns3

#endif // TCPRTOTEST_H
