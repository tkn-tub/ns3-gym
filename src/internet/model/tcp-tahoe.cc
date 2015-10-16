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

#include "tcp-tahoe.h"
#include "ns3/log.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/simulator.h"
#include "ns3/abort.h"
#include "ns3/node.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TcpTahoe");

NS_OBJECT_ENSURE_REGISTERED (TcpTahoe);

TypeId
TcpTahoe::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpTahoe")
    .SetParent<TcpSocketBase> ()
    .SetGroupName ("Internet")
    .AddConstructor<TcpTahoe> ()
  ;
  return tid;
}

TcpTahoe::TcpTahoe (void)
{
  NS_LOG_FUNCTION (this);
}

TcpTahoe::TcpTahoe (const TcpTahoe& sock)
  : TcpSocketBase (sock)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC ("Invoked the copy constructor");
}

TcpTahoe::~TcpTahoe (void)
{
}

Ptr<TcpSocketBase>
TcpTahoe::Fork (void)
{
  return CopyObject<TcpTahoe> (this);
}

/* New ACK (up to seqnum seq) received. Increase cwnd and call TcpSocketBase::NewAck() */
void
TcpTahoe::NewAck (SequenceNumber32 const& seq)
{
  NS_LOG_FUNCTION (this << seq);
  NS_LOG_LOGIC ("TcpTahoe received ACK for seq " << seq <<
                " cwnd " << m_cWnd <<
                " ssthresh " << m_ssThresh);
  if (m_cWnd < m_ssThresh)
    { // Slow start mode, add one segSize to cWnd. Default m_ssThresh is 65535. (RFC2001, sec.1)
      m_cWnd += m_segmentSize;
      NS_LOG_INFO ("In SlowStart, updated to cwnd " << m_cWnd << " ssthresh " << m_ssThresh);
    }
  else
    { // Congestion avoidance mode, increase by (segSize*segSize)/cwnd. (RFC2581, sec.3.1)
      // To increase cwnd for one segSize per RTT, it should be (ackBytes*segSize)/cwnd
      double adder = static_cast<double> (m_segmentSize * m_segmentSize) / m_cWnd.Get ();
      adder = std::max (1.0, adder);
      m_cWnd += static_cast<uint32_t> (adder);
      NS_LOG_INFO ("In CongAvoid, updated to cwnd " << m_cWnd << " ssthresh " << m_ssThresh);
    }
  TcpSocketBase::NewAck (seq);           // Complete newAck processing
}

uint32_t
TcpTahoe::GetSsThresh ()
{
  /* Common Tahoe implementations detect congestion only from an RTO.
   * Since this function is called in two part (Retransmit and when a DupAck is
   * received, depending on the value of m_inFastRec we know if an RTO is expired
   * or a triple dupack received */
  if (m_inFastRec)
    {
      return 0;
    }
  else
    {
      return std::max (2 * m_segmentSize, BytesInFlight () / 2);
    }
}

} // namespace ns3
