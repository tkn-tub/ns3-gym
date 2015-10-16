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
#ifndef TCPFASTRETRTEST_H
#define TCPFASTRETRTEST_H

#include "tcp-general-test.h"
#include "ns3/simple-channel.h"
#include "tcp-error-model.h"

namespace ns3 {

/**
 * \brief Test the fast retransmission
 *
 * Checking what is happening is not so easy, so there are a lot of variables
 * which helps to keep track on what is happening.
 * The idea is following sequence and ack numbers which are exchanged,
 * testing if they are the same as the implementation transmits.
 */
class TcpFastRetrTest : public TcpGeneralTest
{
public:
  TcpFastRetrTest (TypeId congControl, uint32_t seqToKill, const std::string &msg);

  virtual Ptr<ErrorModel> CreateSenderErrorModel ();
  virtual Ptr<ErrorModel> CreateReceiverErrorModel ();

  virtual Ptr<TcpSocketMsgBase> CreateSenderSocket (Ptr<Node> node);

protected:
  virtual void RcvAck      (const Ptr<const TcpSocketState> tcb,
                            const TcpHeader& h, SocketWho who);
  virtual void ProcessedAck (const Ptr<const TcpSocketState> tcb,
                             const TcpHeader& h, SocketWho who);

  virtual void CongStateTrace (const TcpSocketState::TcpCongState_t oldValue,
                               const TcpSocketState::TcpCongState_t newValue);

  virtual void Tx (const Ptr<const Packet> p, const TcpHeader&h, SocketWho who);
  virtual void Rx (const Ptr<const Packet> p, const TcpHeader&h, SocketWho who);

  virtual void RTOExpired (const Ptr<const TcpSocketState> tcb, SocketWho who);

  void PktDropped (const Ipv4Header &ipH, const TcpHeader& tcpH);
  void FinalChecks ();

  bool m_pktDropped;
  bool m_pktWasDropped;
  uint32_t m_seqToKill;
  uint32_t m_dupAckReceived;

  SequenceNumber32 m_previousAck;
  SequenceNumber32 m_sndNextExpSeq;
  SequenceNumber32 m_rcvNextExpAck;

  uint32_t m_countRetr;

  uint32_t m_bytesRcvButNotAcked;

  Ptr<TcpSeqErrorModel> m_errorModel;
};

} // namespace ns3

#endif // TCPFASTRETRTEST_H
