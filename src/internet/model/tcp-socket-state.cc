/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018 Natale Patriciello <natale.patriciello@gmail.com>
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
#include "tcp-socket-state.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (TcpSocketState);

TypeId
TcpSocketState::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpSocketState")
    .SetParent<Object> ()
    .SetGroupName ("Internet")
    .AddConstructor <TcpSocketState> ()
    .AddAttribute ("EnablePacing", "Enable Pacing",
                   BooleanValue (false),
                   MakeBooleanAccessor (&TcpSocketState::m_pacing),
                   MakeBooleanChecker ())
    .AddAttribute ("MaxPacingRate", "Set Max Pacing Rate",
                   DataRateValue (DataRate ("4Gb/s")),
                   MakeDataRateAccessor (&TcpSocketState::m_maxPacingRate),
                   MakeDataRateChecker ())
    .AddTraceSource ("CongestionWindow",
                     "The TCP connection's congestion window",
                     MakeTraceSourceAccessor (&TcpSocketState::m_cWnd),
                     "ns3::TracedValue::Uint32Callback")
    .AddTraceSource ("CongestionWindowInflated",
                     "The TCP connection's inflated congestion window",
                     MakeTraceSourceAccessor (&TcpSocketState::m_cWndInfl),
                     "ns3::TracedValue::Uint32Callback")
    .AddTraceSource ("SlowStartThreshold",
                     "TCP slow start threshold (bytes)",
                     MakeTraceSourceAccessor (&TcpSocketState::m_ssThresh),
                     "ns3::TracedValue::Uint32Callback")
    .AddTraceSource ("CongState",
                     "TCP Congestion machine state",
                     MakeTraceSourceAccessor (&TcpSocketState::m_congState),
                     "ns3::TracedValue::TcpCongStatesTracedValueCallback")
    .AddTraceSource ("HighestSequence",
                     "Highest sequence number received from peer",
                     MakeTraceSourceAccessor (&TcpSocketState::m_highTxMark),
                     "ns3::SequenceNumber32TracedValueCallback")
    .AddTraceSource ("NextTxSequence",
                     "Next sequence number to send (SND.NXT)",
                     MakeTraceSourceAccessor (&TcpSocketState::m_nextTxSequence),
                     "ns3::SequenceNumber32TracedValueCallback")
    .AddTraceSource ("BytesInFlight",
                     "The TCP connection's congestion window",
                     MakeTraceSourceAccessor (&TcpSocketState::m_bytesInFlight),
                     "ns3::TracedValue::Uint32Callback")
    .AddTraceSource ("RTT",
                     "Last RTT sample",
                     MakeTraceSourceAccessor (&TcpSocketState::m_lastRtt),
                     "ns3::TracedValue::TimeCallback")
  ;
  return tid;
}

TcpSocketState::TcpSocketState (const TcpSocketState &other)
  : Object (other),
    m_cWnd (other.m_cWnd),
    m_ssThresh (other.m_ssThresh),
    m_initialCWnd (other.m_initialCWnd),
    m_initialSsThresh (other.m_initialSsThresh),
    m_segmentSize (other.m_segmentSize),
    m_lastAckedSeq (other.m_lastAckedSeq),
    m_congState (other.m_congState),
    m_highTxMark (other.m_highTxMark),
    m_nextTxSequence (other.m_nextTxSequence),
    m_rcvTimestampValue (other.m_rcvTimestampValue),
    m_rcvTimestampEchoReply (other.m_rcvTimestampEchoReply),
    m_pacing (other.m_pacing),
    m_maxPacingRate (other.m_maxPacingRate),
    m_currentPacingRate (other.m_currentPacingRate),
    m_minRtt (other.m_minRtt),
    m_bytesInFlight (other.m_bytesInFlight),
    m_lastRtt (other.m_lastRtt)
{
}

const char* const
TcpSocketState::TcpCongStateName[TcpSocketState::CA_LAST_STATE] =
{
  "CA_OPEN", "CA_DISORDER", "CA_CWR", "CA_RECOVERY", "CA_LOSS"
};

} //namespace ns3
