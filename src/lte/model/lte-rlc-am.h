/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */

#ifndef LTE_RLC_AM_H
#define LTE_RLC_AM_H

#include <ns3/event-id.h>
#include <ns3/lte-rlc-sequence-number.h>
#include <ns3/lte-rlc.h>

#include <vector>
#include <map>

namespace ns3 {

/**
 * LTE RLC Acknowledged Mode (AM), see 3GPP TS 36.322
 */
class LteRlcAm : public LteRlc
{
public:
  LteRlcAm ();
  virtual ~LteRlcAm ();
  static TypeId GetTypeId (void);
  virtual void DoDispose ();

  /**
   * RLC SAP
   */
  virtual void DoTransmitPdcpPdu (Ptr<Packet> p);

  /**
   * MAC SAP
   */
  virtual void DoNotifyTxOpportunity (uint32_t bytes, uint8_t layer, uint8_t harqId);
  virtual void DoNotifyHarqDeliveryFailure ();
  virtual void DoReceivePdu (Ptr<Packet> p);

private:
  /**
   * This method will schedule a timeout at WaitReplyTimeout interval
   * in the future, unless a timer is already running for the cache,
   * in which case this method does nothing.
   */
  void ExpireReorderingTimer (void);
  void ExpirePollRetransmitTimer (void);

  bool IsInsideReceivingWindow (SequenceNumber10 seqNumber);
// 
//   void ReassembleOutsideWindow (void);
//   void ReassembleSnLessThan (uint16_t seqNumber);
// 
  void ReassembleAndDeliver (Ptr<Packet> packet);

  void DoReportBufferStatus ();

private:
    std::vector < Ptr<Packet> > m_txonBuffer;       // Transmission buffer
    std::vector < Ptr<Packet> > m_txedBuffer;       // Transmitted packets buffer

    struct RetxBuffer
    {
      Ptr<Packet> m_pdu;
      uint16_t    m_retxCount;
    };

    std::vector < RetxBuffer > m_retxBuffer;       // Retransmission buffer

    uint32_t m_txonBufferSize;
    uint32_t m_retxBufferSize;
    uint32_t m_txedBufferSize;

    bool     m_statusPduRequested;
    uint32_t m_statusPduBufferSize;

    struct PduBuffer
    {
      SequenceNumber10  m_seqNumber;
      std::list < Ptr<Packet> >  m_byteSegments;

      bool      m_pduComplete;
      uint16_t  m_totalSize;
      uint16_t  m_currSize;
    };

    std::map <uint16_t, PduBuffer > m_rxonBuffer; // Reception buffer

    Ptr<Packet> m_controlPduBuffer;               // Control PDU buffer (just one PDU)

    // SDU reassembly
//   std::vector < Ptr<Packet> > m_reasBuffer;     // Reassembling buffer
// 
    std::list < Ptr<Packet> > m_sdusBuffer;       // List of SDUs in a packet (PDU)

  /**
   * State variables. See section 7.1 in TS 36.322
   */
  // Transmitting side
  SequenceNumber10 m_vtA;                   // VT(A)
  SequenceNumber10 m_vtMs;                  // VT(MS)
  SequenceNumber10 m_vtS;                   // VT(S)
  SequenceNumber10 m_pollSn;                // POLL_SN

  // Receiving side
  SequenceNumber10 m_vrR;                   // VR(R)
  SequenceNumber10 m_vrMr;                  // VR(MR)
  SequenceNumber10 m_vrX;                   // VR(X)
  SequenceNumber10 m_vrMs;                  // VR(MS)
  SequenceNumber10 m_vrH;                   // VR(H)

  /**
   * Counters. See section 7.1 in TS 36.322
   */
  uint32_t m_pduWithoutPoll;
  uint32_t m_byteWithoutPoll;

  /**
   * Constants. See section 7.2 in TS 36.322
   */
  uint16_t m_windowSize;

  /**
   * Timers. See section 7.3 in TS 36.322
   */
  EventId m_pollRetransmitTimer;
  Time    m_pollRetransmitTimerValue;
  EventId m_reorderingTimer;
  EventId m_statusProhibitTimer;

  /**
   * Configurable parameters. See section 7.4 in TS 36.322
   */
  uint16_t m_maxRetxThreshold;  /// \todo How these parameters are configured???
  uint16_t m_pollPdu;
  uint16_t m_pollByte;
  
  bool m_txOpportunityForRetxAlwaysBigEnough;

  /**
   * SDU Reassembling state
   */
  typedef enum { NONE            = 0,
                 WAITING_S0_FULL = 1,
                 WAITING_SI_SF   = 2 } ReassemblingState_t;
  ReassemblingState_t m_reassemblingState;
  Ptr<Packet> m_keepS0;

  /**
   * Expected Sequence Number
   */
  SequenceNumber10 m_expectedSeqNumber;

};


} // namespace ns3

#endif // LTE_RLC_AM_H
