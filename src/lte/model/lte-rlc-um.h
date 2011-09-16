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

#ifndef LTE_RLC_UM_H
#define LTE_RLC_UM_H

#include "ns3/lte-rlc.h"

#include <map>

namespace ns3 {

/**
 * LTE RLC Unacknowledged Mode (UM), see 3GPP TS 36.322
 */
class LteRlcUm : public LteRlc
{
public:
  LteRlcUm ();
  virtual ~LteRlcUm ();
  static TypeId GetTypeId (void);

  /**
   * RLC SAP
   */
  virtual void DoTransmitPdcpPdu (Ptr<Packet> p);

  /**
   * MAC SAP
   */
  virtual void DoNotifyTxOpportunity (uint32_t bytes);
  virtual void DoNotifyHarqDeliveryFailure ();
  virtual void DoReceivePdu (Ptr<Packet> p);

  void Start ();

private:
  /**
   * This method will schedule a timeout at WaitReplyTimeout interval
   * in the future, unless a timer is already running for the cache,
   * in which case this method does nothing.
   */
  void StartReorderingTimer (void);
  void ExpireReorderingTimer (void);

  bool IsInsideReorderingWindow (uint16_t seqNumber);

  void ReassembleOutsideWindow (void);
  void ReassembleSnLessThan (uint16_t seqNumber);

  void ReassembleAndDeliver (Ptr<Packet> packet);

private:
  uint32_t m_txBufferSize;
  std::vector < Ptr<Packet> > m_txBuffer;       // Transmission buffer
  std::map <uint16_t, Ptr<Packet> > m_rxBuffer; // Reception buffer
  std::vector < Ptr<Packet> > m_reasBuffer;     // Reassembling buffer

  std::list < Ptr<Packet> > m_sdusBuffer;       // List of SDUs in a packet

  /**
   * State variables. See section 7.1 in TS 36.322
   */
  uint16_t m_sequenceNumber;      // VT(US)

  uint16_t m_vrUr;                // VR(UR)
  uint16_t m_vrUx;                // VR(UX)
  uint16_t m_vrUh;                // VR(UH)

  /**
   * Constants. See section 7.2 in TS 36.322
   */
  uint16_t m_windowSize;

  /**
   * Timers. See section 7.3 in TS 36.322
   */
  EventId m_reorderingTimer;

  /**
   * Reassembling state
   */
  typedef enum { NONE            = 0,
                 WAITING_S0_FULL = 1,
                 WAITING_SI_SF   = 2 } ReassemblingState_t;
  ReassemblingState_t m_reassemblingState;
  Ptr<Packet> m_keepS0;

  /**
   * Expected Sequence Number
   */
  uint16_t m_expectedSeqNumber;

};


} // namespace ns3

#endif // LTE_RLC_UM_H
