/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Adrian Sai-wah Tam
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
 * Author: Adrian Sai-wah Tam <adrian.sw.tam@gmail.com>
 */

#define NS_LOG_APPEND_CONTEXT \
  if (m_node) { std::clog << Simulator::Now ().GetSeconds () << " [node " << m_node->GetId () << "] "; }

#include "tcp-newreno.h"
#include "ns3/log.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/simulator.h"
#include "ns3/abort.h"
#include "ns3/node.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TcpNewReno");

NS_OBJECT_ENSURE_REGISTERED (TcpNewReno);

TypeId
TcpNewReno::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpNewReno")
    .SetParent<TcpSocketBase> ()
    .SetGroupName ("Internet")
    .AddConstructor<TcpNewReno> ()
 ;
  return tid;
}

TcpNewReno::TcpNewReno (void)
  : TcpSocketBase ()
{
  NS_LOG_FUNCTION (this);
}

TcpNewReno::TcpNewReno (const TcpNewReno& sock)
  : TcpSocketBase (sock)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC ("Invoked the copy constructor");
}

TcpNewReno::~TcpNewReno (void)
{
}

Ptr<TcpSocketBase>
TcpNewReno::Fork (void)
{
  return CopyObject<TcpNewReno> (this);
}

/* New ACK (up to seqnum seq) received. Increase cwnd and call TcpSocketBase::NewAck() */
void
TcpNewReno::NewAck (const SequenceNumber32& seq)
{
  NS_LOG_FUNCTION (this << seq);

  // No cWnd management while recovering
  if (m_ackState == RECOVERY && seq < m_recover)
    {
      TcpSocketBase::NewAck (seq);
      return;
    }

  uint32_t segmentsAcked = (seq - m_txBuffer->HeadSequence ()) / m_tcb->m_segmentSize;

  NS_LOG_LOGIC ("TcpNewReno received ACK for seq " << seq <<
                " cwnd " << m_tcb->m_cWnd <<
                " ssthresh " << m_tcb->m_ssThresh <<
                " acked " << segmentsAcked);

  if (m_tcb->m_cWnd <= m_tcb->m_ssThresh)
    {
      segmentsAcked = SlowStart (segmentsAcked);
    }

  if (m_tcb->m_cWnd > m_tcb->m_ssThresh)
    {
      CongestionAvoidance (segmentsAcked);
    }

  // Complete newAck processing
  TcpSocketBase::NewAck (seq);
}

/**
 * \brief Tcp NewReno slow start algorithm
 *
 * Defined in RFC 5681 as
 *
 * > During slow start, a TCP increments cwnd by at most SMSS bytes for
 * > each ACK received that cumulatively acknowledges new data.  Slow
 * > start ends when cwnd exceeds ssthresh (or, optionally, when it
 * > reaches it, as noted above) or when congestion is observed.  While
 * > traditionally TCP implementations have increased cwnd by precisely
 * > SMSS bytes upon receipt of an ACK covering new data, we RECOMMEND
 * > that TCP implementations increase cwnd, per:
 * >
 * >    cwnd += min (N, SMSS)                      (2)
 * >
 * > where N is the number of previously unacknowledged bytes acknowledged
 * > in the incoming ACK.
 *
 * The ns-3 implementation respect the RFC definition. Linux does something
 * different:
 * \verbatim
u32 tcp_slow_start(struct tcp_sock *tp, u32 acked)
  {
    u32 cwnd = tp->snd_cwnd + acked;

    if (cwnd > tp->snd_ssthresh)
      cwnd = tp->snd_ssthresh + 1;
    acked -= cwnd - tp->snd_cwnd;
    tp->snd_cwnd = min(cwnd, tp->snd_cwnd_clamp);

    return acked;
  }
  \endverbatim
 *
 * As stated, we want to avoid the case when a cumulative ACK increases cWnd more
 * than a segment size, but we keep count of how many segments we have ignored,
 * and return them.
 *
 * \param segmentsAcked count of segments acked
 * \return the number of segments not considered for increasing the cWnd
 */
uint32_t
TcpNewReno::SlowStart (uint32_t segmentsAcked)
{
  NS_LOG_FUNCTION (this << segmentsAcked);

  if (segmentsAcked >= 1)
    {
      m_tcb->m_cWnd = m_tcb->m_cWnd.Get () + m_tcb->m_segmentSize;

      NS_LOG_INFO ("In SlowStart, updated to cwnd " << m_tcb->m_cWnd <<
                   " ssthresh " << m_tcb->m_ssThresh << " MSS " << m_tcb->m_segmentSize);

      return segmentsAcked - 1;
    }

  return 0;
}

/**
 * \brief NewReno congestion avoidance
 *
 * During congestion avoidance, cwnd is incremented by roughly 1 full-sized
 * segment per round-trip time (RTT).
 *
 * \param segmentsAcked count of segments acked
 */
void
TcpNewReno::CongestionAvoidance (uint32_t segmentsAcked)
{
  NS_LOG_FUNCTION (this << segmentsAcked);

  if (segmentsAcked > 0)
    {
      double adder = static_cast<double> (m_tcb->m_segmentSize * m_tcb->m_segmentSize) / m_tcb->m_cWnd.Get ();
      adder = std::max (1.0, adder);
      m_tcb->m_cWnd += static_cast<uint32_t> (adder);
      NS_LOG_INFO ("In CongAvoid, updated to cwnd " << m_tcb->m_cWnd <<
                   " ssthresh " << m_tcb->m_ssThresh);
    }
}

uint32_t
TcpNewReno::GetSsThresh ()
{
  return std::max (2 * m_tcb->m_segmentSize, BytesInFlight () / 2);
}

} // namespace ns3
