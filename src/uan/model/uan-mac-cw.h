/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 * Author: Leonard Tracy <lentracy@gmail.com>
 */

#ifndef UAN_MAC_CW_H
#define UAN_MAC_CW_H

#include "ns3/uan-mac.h"
#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include "ns3/uan-phy.h"
#include "ns3/uan-tx-mode.h"
#include "ns3/uan-address.h"


namespace ns3 {

/**
 * \class UanMacCw
 * \brief CW-MAC A MAC protocol similar in idea to the 802.11 DCF with constant backoff window
 *
 * For more information on this MAC protocol, see:
 * Parrish, N.; Tracy, L.; Roy, S.; Arabshahi, P.; Fox, W.,
 * "System Design Considerations for Undersea Networks: Link and Multiple Access Protocols,"
 * Selected Areas in Communications, IEEE Journal on , vol.26, no.9, pp.1720-1730, December 2008
 */
class UanMacCw : public UanMac,
                 public UanPhyListener
{
public:
  UanMacCw ();
  virtual ~UanMacCw ();
  static TypeId GetTypeId (void);

  /**
   * \param cw Contention window size
   */
  virtual void SetCw (uint32_t cw);
  /**
   * \param duration Slot time duration
   */
  virtual void SetSlotTime (Time duration);
  /**
   * \returns Contention window size
   */
  virtual uint32_t GetCw (void);
  /**
   * \returns slot time duration
   */
  virtual Time GetSlotTime (void);

  // Inherited methods
  virtual Address GetAddress ();
  virtual void SetAddress (UanAddress addr);
  virtual bool Enqueue (Ptr<Packet> pkt, const Address &dest, uint16_t protocolNumber);
  virtual void SetForwardUpCb (Callback<void, Ptr<Packet>, const UanAddress&> cb);
  virtual void AttachPhy (Ptr<UanPhy> phy);
  virtual Address GetBroadcast (void) const;
  virtual void Clear (void);

  // PHY listeners
  /// Function called by UanPhy object to notify of packet reception
  virtual void NotifyRxStart (void);
  /// Function called by UanPhy object to notify of packet received successfully
  virtual void NotifyRxEndOk (void);
  /// Function called by UanPhy object to notify of packet received in error
  virtual void NotifyRxEndError (void);
  /// Function called by UanPhy object to notify of channel sensed busy
  virtual void NotifyCcaStart (void);
  /// Function called by UanPhy object to notify of channel no longer sensed busy
  virtual void NotifyCcaEnd (void);
  /// Function called by UanPhy object to notify of outgoing transmission start
  virtual void NotifyTxStart (Time duration);
private:
  typedef enum {
    IDLE, CCABUSY, RUNNING, TX
  } State;

  Callback <void, Ptr<Packet>, const UanAddress& > m_forwardUpCb;
  UanAddress m_address;
  Ptr<UanPhy> m_phy;
  TracedCallback<Ptr<const Packet>, UanTxMode > m_rxLogger;
  TracedCallback<Ptr<const Packet>, uint16_t  > m_enqueueLogger;
  TracedCallback<Ptr<const Packet>, uint16_t  > m_dequeueLogger;

  // Mac parameters
  uint32_t m_cw;
  Time m_slotTime;

  // State variables
  Time m_sendTime;
  Time m_savedDelayS;
  Ptr<Packet> m_pktTx;
  uint16_t m_pktTxProt;
  EventId m_sendEvent;
  EventId m_txEndEvent;
  State m_state;

  bool m_cleared;

  void PhyRxPacketGood (Ptr<Packet> packet, double sinr, UanTxMode mode);
  void PhyRxPacketError (Ptr<Packet> packet, double sinr);
  void SaveTimer (void);
  void StartTimer (void);
  void SendPacket (void);
  void EndTx (void);
protected:
  virtual void DoDispose ();
};

}

#endif /* UAN_MAC_CW_H */
