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
  NS_LOG_LOGIC ("TcpNewReno received ACK for seq " << seq <<
                " cwnd " << m_tcb->m_cWnd <<
                " ssthresh " << m_tcb->m_ssThresh);

  // No cWnd management while recovering
  if (m_ackState == RECOVERY && seq < m_recover)
    {
      TcpSocketBase::NewAck (seq);
      return;
    }

  // Increase of cwnd based on current phase (slow start or congestion avoidance)
  if (m_tcb->m_cWnd < m_tcb->m_ssThresh)
    { // Slow start mode, add one segSize to cWnd. Default m_tcb->m_ssThresh is 65535. (RFC2001, sec.1)
      m_tcb->m_cWnd += m_tcb->m_segmentSize;
      NS_LOG_INFO ("In SlowStart, ACK of seq " << seq << "; update cwnd to " << m_tcb->m_cWnd << "; ssthresh " << m_tcb->m_ssThresh);
    }
  else
    { // Congestion avoidance mode, increase by (segSize*segSize)/cwnd. (RFC2581, sec.3.1)
      // To increase cwnd for one segSize per RTT, it should be (ackBytes*segSize)/cwnd
      double adder = static_cast<double> (m_tcb->m_segmentSize * m_tcb->m_segmentSize) / m_tcb->m_cWnd.Get ();
      adder = std::max (1.0, adder);
      m_tcb->m_cWnd += static_cast<uint32_t> (adder);
      NS_LOG_INFO ("In CongAvoid, updated to cwnd " << m_tcb->m_cWnd << " ssthresh " << m_tcb->m_ssThresh);
    }

  // Complete newAck processing
  TcpSocketBase::NewAck (seq);
}

uint32_t
TcpNewReno::GetSsThresh ()
{
  return std::max (2 * m_tcb->m_segmentSize, BytesInFlight () / 2);
}

} // namespace ns3
