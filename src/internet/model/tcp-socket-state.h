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
#pragma once

#include "ns3/object.h"
#include "ns3/data-rate.h"
#include "ns3/traced-value.h"
#include "ns3/sequence-number.h"

namespace ns3 {

/**
 * \brief Data structure that records the congestion state of a connection
 *
 * In this data structure, basic informations that should be passed between
 * socket and the congestion control algorithm are saved. Through the code,
 * it will be referred as Transmission Control Block (TCB), but there are some
 * differencies. In the RFCs, the TCB contains all the variables that defines
 * a connection, while we preferred to maintain in this class only the values
 * that should be exchanged between socket and other parts, like congestion
 * control algorithms.
 *
 */
class TcpSocketState : public Object
{
public:
  /**
   * Get the type ID.
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * \brief TcpSocketState Constructor
   */
  TcpSocketState () : Object () { }

  /**
   * \brief Copy constructor.
   * \param other object to copy.
   */
  TcpSocketState (const TcpSocketState &other);

  /**
   * \brief Definition of the Congestion state machine
   *
   * The design of this state machine is taken from Linux v4.0, but it has been
   * maintained in the Linux mainline from ages. It basically avoids to maintain
   * a lot of boolean variables, and it allows to check the transitions from
   * different algorithm in a cleaner way.
   *
   * These states represent the situation from a congestion control point of view:
   * in fact, apart the CA_OPEN state, the other states represent a situation in
   * which there is a congestion, and different actions should be taken,
   * depending on the case.
   *
   */
  typedef enum
  {
    CA_OPEN,      /**< Normal state, no dubious events */
    CA_DISORDER,  /**< In all the respects it is "Open",
                    *  but requires a bit more attention. It is entered when
                    *  we see some SACKs or dupacks. It is split of "Open" */
    CA_CWR,       /**< cWnd was reduced due to some Congestion Notification event.
                    *  It can be ECN, ICMP source quench, local device congestion.
                    *  Not used in NS-3 right now. */
    CA_RECOVERY,  /**< CWND was reduced, we are fast-retransmitting. */
    CA_LOSS,      /**< CWND was reduced due to RTO timeout or SACK reneging. */
    CA_LAST_STATE /**< Used only in debug messages */
  } TcpCongState_t;

  // Note: "not triggered" events are currently not triggered by the code.
  typedef enum
  {
    CA_EVENT_TX_START,     /**< first transmit when no packets in flight */
    CA_EVENT_CWND_RESTART, /**< congestion window restart. Not triggered */
    CA_EVENT_COMPLETE_CWR, /**< end of congestion recovery */
    CA_EVENT_LOSS,         /**< loss timeout */
    CA_EVENT_ECN_NO_CE,    /**< ECT set, but not CE marked. Not triggered */
    CA_EVENT_ECN_IS_CE,    /**< received CE marked IP packet. Not triggered */
    CA_EVENT_DELAYED_ACK,  /**< Delayed ack is sent */
    CA_EVENT_NON_DELAYED_ACK, /**< Non-delayed ack is sent */
  } TcpCAEvent_t;

  /**
   * \ingroup tcp
   * TracedValue Callback signature for TcpCongState_t
   *
   * \param [in] oldValue original value of the traced variable
   * \param [in] newValue new value of the traced variable
   */
  typedef void (* TcpCongStatesTracedValueCallback)(const TcpCongState_t oldValue,
                                                    const TcpCongState_t newValue);

  /**
   * \brief Literal names of TCP states for use in log messages
   */
  static const char* const TcpCongStateName[TcpSocketState::CA_LAST_STATE];

  // Congestion control
  TracedValue<uint32_t>  m_cWnd             {0}; //!< Congestion window
  TracedValue<uint32_t>  m_cWndInfl         {0}; //!< Inflated congestion window trace (used only for backward compatibility purpose)
  TracedValue<uint32_t>  m_ssThresh         {0}; //!< Slow start threshold
  uint32_t               m_initialCWnd      {0}; //!< Initial cWnd value
  uint32_t               m_initialSsThresh  {0}; //!< Initial Slow Start Threshold value

  // Segment
  uint32_t               m_segmentSize   {0}; //!< Segment size
  SequenceNumber32       m_lastAckedSeq  {0}; //!< Last sequence ACKed

  TracedValue<TcpCongState_t> m_congState {CA_OPEN}; //!< State in the Congestion state machine

  TracedValue<SequenceNumber32> m_highTxMark     {0}; //!< Highest seqno ever sent, regardless of ReTx
  TracedValue<SequenceNumber32> m_nextTxSequence {0}; //!< Next seqnum to be sent (SND.NXT), ReTx pushes it back

  uint32_t               m_rcvTimestampValue     {0}; //!< Receiver Timestamp value
  uint32_t               m_rcvTimestampEchoReply {0}; //!< Sender Timestamp echoed by the receiver

  // Pacing related variables
  bool                   m_pacing            {false}; //!< Pacing status
  DataRate               m_maxPacingRate     {0};    //!< Max Pacing rate
  DataRate               m_currentPacingRate {0};    //!< Current Pacing rate

  Time                   m_minRtt  {Time::Max ()};   //!< Minimum RTT observed throughout the connection

  TracedValue<uint32_t>  m_bytesInFlight {0};        //!< Bytes in flight
  TracedValue<Time>      m_lastRtt {Seconds (0.0)};  //!< Last RTT sample collected

  /**
   * \brief Get cwnd in segments rather than bytes
   *
   * \return Congestion window in segments
   */
  uint32_t GetCwndInSegments () const
  {
    return m_cWnd / m_segmentSize;
  }

  /**
   * \brief Get slow start thresh in segments rather than bytes
   *
   * \return Slow start threshold in segments
   */
  uint32_t GetSsThreshInSegments () const
  {
    return m_ssThresh / m_segmentSize;
  }
};

} //namespace ns3
