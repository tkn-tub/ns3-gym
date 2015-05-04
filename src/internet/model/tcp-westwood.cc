/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 ResiliNets, ITTC, University of Kansas 
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
 * Authors: Siddharth Gangadhar <siddharth@ittc.ku.edu>, Truc Anh N. Nguyen <annguyen@ittc.ku.edu>,
 * and Greeshma Umapathi
 *
 * James P.G. Sterbenz <jpgs@ittc.ku.edu>, director
 * ResiliNets Research Group  http://wiki.ittc.ku.edu/resilinets
 * Information and Telecommunication Technology Center (ITTC)
 * and Department of Electrical Engineering and Computer Science
 * The University of Kansas Lawrence, KS USA.
 *
 * Work supported in part by NSF FIND (Future Internet Design) Program
 * under grant CNS-0626918 (Postmodern Internet Architecture),
 * NSF grant CNS-1050226 (Multilayer Network Resilience Analysis and Experimentation on GENI),
 * US Department of Defense (DoD), and ITTC at The University of Kansas.
 */

#define NS_LOG_APPEND_CONTEXT \
  if (m_node) { std::clog << Simulator::Now ().GetSeconds () << " [node " << m_node->GetId () << "] "; }

#include "tcp-westwood.h"
#include "ns3/log.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/simulator.h"
#include "ns3/abort.h"
#include "ns3/node.h"
#include "ns3/sequence-number.h"
#include "rtt-estimator.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("TcpWestwood");

NS_OBJECT_ENSURE_REGISTERED(TcpWestwood);

TypeId
TcpWestwood::GetTypeId (void)
{
  static TypeId tid = TypeId("ns3::TcpWestwood")
      .SetParent<TcpSocketBase>()
      .SetGroupName ("Internet")
      .AddConstructor<TcpWestwood>()
      .AddTraceSource("CongestionWindow", "The TCP connection's congestion window",
                      MakeTraceSourceAccessor(&TcpWestwood::m_cWnd),
                      "ns3::TracedValue::Uint32Callback")
      .AddTraceSource ("SlowStartThreshold",
                       "TCP slow start threshold (bytes)",
                       MakeTraceSourceAccessor (&TcpWestwood::m_ssThresh),
                       "ns3::TracedValue::Uint32Callback")
      .AddAttribute("FilterType", "Use this to choose no filter or Tustin's approximation filter",
                    EnumValue(TcpWestwood::TUSTIN), MakeEnumAccessor(&TcpWestwood::m_fType),
                    MakeEnumChecker(TcpWestwood::NONE, "None", TcpWestwood::TUSTIN, "Tustin"))
      .AddAttribute("ProtocolType", "Use this to let the code run as Westwood or WestwoodPlus",
                    EnumValue(TcpWestwood::WESTWOOD),
                    MakeEnumAccessor(&TcpWestwood::m_pType),
                    MakeEnumChecker(TcpWestwood::WESTWOOD, "Westwood",TcpWestwood::WESTWOODPLUS, "WestwoodPlus"))
      .AddTraceSource("EstimatedBW", "The estimated bandwidth",
                    MakeTraceSourceAccessor(&TcpWestwood::m_currentBW),
                      "ns3::TracedValue::DoubleCallback");
  return tid;
}

TcpWestwood::TcpWestwood (void) :
  m_inFastRec(false),
  m_currentBW(0),
  m_lastSampleBW(0),
  m_lastBW(0),
  m_minRtt(0),
  m_lastAck(0),
  m_prevAckNo(0),
  m_accountedFor(0),
  m_ackedSegments(0),
  m_IsCount(false)
{
  NS_LOG_FUNCTION (this);
}

TcpWestwood::TcpWestwood (const TcpWestwood& sock) :
  TcpSocketBase(sock),
  m_cWnd(sock.m_cWnd),
  m_ssThresh(sock.m_ssThresh),
  m_initialCWnd(sock.m_initialCWnd),
  m_initialSsThresh (sock.m_initialSsThresh),
  m_inFastRec(false),
  m_currentBW(sock.m_currentBW),
  m_lastSampleBW(sock.m_lastSampleBW),
  m_lastBW(sock.m_lastBW),
  m_minRtt(sock.m_minRtt),
  m_lastAck(sock.m_lastAck),
  m_prevAckNo(sock.m_prevAckNo),
  m_accountedFor(sock.m_accountedFor),
  m_pType(sock.m_pType),
  m_fType(sock.m_fType),
  m_IsCount(sock.m_IsCount)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC ("Invoked the copy constructor");
  NS_LOG_INFO ("m_minRtt at copy constructor" << m_minRtt);
}

TcpWestwood::~TcpWestwood (void)
{
}

int
TcpWestwood::Listen (void)
{
  NS_LOG_FUNCTION (this);
  InitializeCwnd();
  return TcpSocketBase::Listen();
}

int
TcpWestwood::Connect (const Address & address)
{
  NS_LOG_FUNCTION (this << address);
  InitializeCwnd();
  return TcpSocketBase::Connect(address);
}

uint32_t
TcpWestwood::Window (void)
{
  NS_LOG_FUNCTION (this);
  return std::min (m_rWnd.Get (), m_cWnd.Get ());
}

Ptr<TcpSocketBase>
TcpWestwood::Fork (void)
{
  NS_LOG_FUNCTION (this);
  return CopyObject<TcpWestwood>(this);
}

void
TcpWestwood::NewAck (const SequenceNumber32& seq)
{ // Same as Reno
  NS_LOG_FUNCTION (this << seq);
  NS_LOG_LOGIC ("TcpWestwood receieved ACK for seq " << seq <<
                " cwnd " << m_cWnd <<
                " ssthresh " << m_ssThresh);

  // Check for exit condition of fast recovery
  if (m_inFastRec)
    {// First new ACK after fast recovery, reset cwnd as in Reno
      m_cWnd = m_ssThresh;
      m_inFastRec = false;
      NS_LOG_INFO ("Reset cwnd to " << m_cWnd);
    };

  // Increase of cwnd based on current phase (slow start or congestion avoidance)
  if (m_cWnd < m_ssThresh)
    { // Slow start mode, add one segSize to cWnd as in Reno
      m_cWnd += m_segmentSize;
      NS_LOG_INFO ("In SlowStart, updated to cwnd " << m_cWnd << " ssthresh " << m_ssThresh);
    }
  else
    { // Congestion avoidance mode, increase by (segSize*segSize)/cwnd as in Reno
      double adder = static_cast<double> (m_segmentSize * m_segmentSize) / m_cWnd.Get();
      adder = std::max(1.0, adder);
      m_cWnd += static_cast<uint32_t>(adder);
      NS_LOG_INFO ("In CongAvoid, updated to cwnd " << m_cWnd << " ssthresh " << m_ssThresh);
    }

  // Complete newAck processing
  TcpSocketBase::NewAck(seq);
}

void
TcpWestwood::ReceivedAck (Ptr<Packet> packet, const TcpHeader& tcpHeader)
{
  NS_LOG_FUNCTION (this);
  int acked = 0;
  if ((0 != (tcpHeader.GetFlags () & TcpHeader::ACK)) && tcpHeader.GetAckNumber() >= m_prevAckNo)
    {// It is a duplicate ACK or a new ACK. Old ACK is ignored.
      if (m_pType == TcpWestwood::WESTWOOD)
        {// For Westwood, calculate the number of ACKed segments and estimate the BW
          acked = CountAck (tcpHeader);
          EstimateBW (acked, tcpHeader, Time(0));
        }
      else if (m_pType == TcpWestwood::WESTWOODPLUS)
        {// For Weswood+, calculate the number of ACKed segments and update m_ackedSegments
          if (m_IsCount)
            {
              acked = CountAck (tcpHeader);
              UpdateAckedSegments (acked);
            }
        }
    }

  TcpSocketBase::ReceivedAck (packet, tcpHeader);
}

void
TcpWestwood::EstimateBW (int acked, const TcpHeader& tcpHeader, Time rtt)
{
  NS_LOG_FUNCTION (this);
  if (m_pType == TcpWestwood::WESTWOOD)
    {
      // Get the time when the current ACK is received
      double currentAck = static_cast<double> (Simulator::Now().GetSeconds());
      // Calculate the BW
      m_currentBW = acked * m_segmentSize / (currentAck - m_lastAck);
      // Update the last ACK time
      m_lastAck = currentAck;
    }
  else if (m_pType == TcpWestwood::WESTWOODPLUS)
    {
      // Calculate the BW
      m_currentBW = m_ackedSegments * m_segmentSize / rtt.GetSeconds();
      // Reset m_ackedSegments and m_IsCount for the next sampling
      m_ackedSegments = 0;
      m_IsCount = false;
    }

  // Filter the BW sample
  Filtering();
}

int
TcpWestwood::CountAck (const TcpHeader& tcpHeader)
{
  NS_LOG_FUNCTION (this);

  // Calculate the number of acknowledged segments based on the received ACK number
  int cumul_ack = (tcpHeader.GetAckNumber() - m_prevAckNo) / m_segmentSize;

  if (cumul_ack == 0)
    {// A DUPACK counts for 1 segment delivered successfully
      m_accountedFor++;
      cumul_ack = 1;
    }
  if (cumul_ack > 1)
    {// A delayed ACK or a cumulative ACK after a retransmission
     // Check how much new data it ACKs
      if (m_accountedFor >= cumul_ack)
        {
          m_accountedFor -= cumul_ack;
          cumul_ack = 1;
        }
      else if (m_accountedFor < cumul_ack)
        {
          cumul_ack -= m_accountedFor;
          m_accountedFor = 0;
        }
    }

  // Update the previous ACK number
  m_prevAckNo = tcpHeader.GetAckNumber();

  return cumul_ack;
}

void
TcpWestwood::UpdateAckedSegments (int acked)
{
  m_ackedSegments += acked;
}

void
TcpWestwood::DupAck (const TcpHeader& header, uint32_t count)
{
  NS_LOG_FUNCTION (this << count << m_cWnd);

  if (count == 3 && !m_inFastRec)
    {// Triple duplicate ACK triggers fast retransmit
     // Adjust cwnd and ssthresh based on the estimated BW
      m_ssThresh = uint32_t(m_currentBW * static_cast<double> (m_minRtt.GetSeconds()));
      if (m_cWnd > m_ssThresh)
        {
          m_cWnd = m_ssThresh;
        }
      m_inFastRec = true;
      NS_LOG_INFO ("Triple dupack. Enter fast recovery mode. Reset cwnd to " << m_cWnd <<", ssthresh to " << m_ssThresh);
      DoRetransmit ();
    }
  else if (m_inFastRec)
    {// Increase cwnd for every additional DUPACK as in Reno
      m_cWnd += m_segmentSize;
      NS_LOG_INFO ("Dupack in fast recovery mode. Increase cwnd to " << m_cWnd);
      if (!m_sendPendingDataEvent.IsRunning ())
        {
          SendPendingData (m_connected);
        }
    }
}

void
TcpWestwood::Retransmit (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC (this << " ReTxTimeout Expired at time " << Simulator::Now ().GetSeconds ());
  m_inFastRec = false;

  // If erroneous timeout in closed/timed-wait state, just return
  if (m_state == CLOSED || m_state == TIME_WAIT)
    return;
  // If all data are received, just return
  if (m_txBuffer->HeadSequence () >= m_nextTxSequence)
    return;

  // Upon an RTO, adjust cwnd and ssthresh based on the estimated BW
  m_ssThresh = std::max (static_cast<double> (2 * m_segmentSize), m_currentBW.Get () * static_cast<double> (m_minRtt.GetSeconds ()));
  m_cWnd = m_segmentSize;

  // Restart from highest ACK
  m_nextTxSequence = m_txBuffer->HeadSequence ();
  NS_LOG_INFO ("RTO. Reset cwnd to " << m_cWnd <<
      ", ssthresh to " << m_ssThresh << ", restart from seqnum " << m_nextTxSequence);

  // Retransmit the packet
  DoRetransmit ();
}

void
TcpWestwood::EstimateRtt (const TcpHeader& tcpHeader)
{
  NS_LOG_FUNCTION_NOARGS ();

  // Calculate m_lastRtt
  TcpSocketBase::EstimateRtt (tcpHeader);

  // Update minRtt
  if (m_minRtt == Time (0))
    {
      m_minRtt = m_lastRtt;
    }
  else
    {
      if (m_lastRtt < m_minRtt)
        {
          m_minRtt = m_lastRtt;
        }
    }

  // For Westwood+, start running a clock on the currently estimated RTT if possible
  // to trigger a new BW sampling event
  if (m_pType == TcpWestwood::WESTWOODPLUS)
   {
     if(m_lastRtt != Time (0) && m_state == ESTABLISHED && !m_IsCount)
       {
         m_IsCount = true;
         m_bwEstimateEvent.Cancel();
         m_bwEstimateEvent = Simulator::Schedule (m_lastRtt, &TcpWestwood::EstimateBW,this,m_ackedSegments,tcpHeader,m_lastRtt);
       }
   }
}

void
TcpWestwood::Filtering ()
{
  NS_LOG_FUNCTION (this);

  double alpha = 0.9;

  if (m_fType == TcpWestwood::NONE)
    {
    }
  else if (m_fType == TcpWestwood::TUSTIN)
    {
      double sample_bwe = m_currentBW;
      m_currentBW = (alpha * m_lastBW) + ((1 - alpha) * ((sample_bwe + m_lastSampleBW) / 2));
      m_lastSampleBW = sample_bwe;
      m_lastBW = m_currentBW;
    }
}

void
TcpWestwood::SetSegSize (uint32_t size)
{
  NS_ABORT_MSG_UNLESS(m_state == CLOSED, "TcpWestwood::SetSegSize() cannot change segment size after connection started.");
  m_segmentSize = size;
}

void
TcpWestwood::SetInitialSSThresh (uint32_t threshold)
{
  NS_LOG_FUNCTION (this);
  NS_ABORT_MSG_UNLESS (m_state == CLOSED, "TcpWestwood::SetSSThresh() cannot change initial ssThresh after connection started.");
  m_initialSsThresh = threshold;
}

uint32_t
TcpWestwood::GetInitialSSThresh (void) const
{
  NS_LOG_FUNCTION (this);
  return m_initialSsThresh;
}

void
TcpWestwood::SetInitialCwnd (uint32_t cwnd)
{
  NS_ABORT_MSG_UNLESS(m_state == CLOSED, "TcpWestwood::SetInitialCwnd() cannot change initial cwnd after connection started.");
  m_initialCWnd = cwnd;
}

uint32_t
TcpWestwood::GetInitialCwnd (void) const
{
  NS_LOG_FUNCTION (this);
  return m_initialCWnd;
}

void
TcpWestwood::InitializeCwnd(void)
{
  NS_LOG_FUNCTION (this);
  /*
   * Initialize congestion window, default to 1 MSS (RFC2001, sec.1) and must
   * not be larger than 2 MSS (RFC2581, sec.3.1). Both m_initiaCWnd and
   * m_segmentSize are set by the attribute system in ns3::TcpSocket.
   */
  m_cWnd = m_initialCWnd * m_segmentSize;
  m_ssThresh = m_initialSsThresh;
}

void
TcpWestwood::ScaleSsThresh (uint8_t scaleFactor)
{
  m_ssThresh <<= scaleFactor;
}


} // namespace ns3
