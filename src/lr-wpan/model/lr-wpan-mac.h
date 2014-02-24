/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 The Boeing Company
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
 * Authors:
 *  Gary Pei <guangyu.pei@boeing.com>
 *  kwong yin <kwong-sang.yin@boeing.com>
 *  Tom Henderson <thomas.r.henderson@boeing.com>
 *  Sascha Alexander Jopen <jopen@cs.uni-bonn.de>
 */
#ifndef LR_WPAN_MAC_H
#define LR_WPAN_MAC_H

#include <ns3/object.h>
#include <ns3/traced-callback.h>
#include <ns3/mac16-address.h>
#include <ns3/mac64-address.h>
#include <ns3/sequence-number.h>
#include <ns3/lr-wpan-phy.h>
#include <ns3/event-id.h>
#include <deque>


namespace ns3 {

class Packet;
class LrWpanCsmaCa;

/**
 * \defgroup lr-wpan LR-WPAN models
 *
 * This section documents the API of the IEEE 802.15.4-related models.  For a generic functional description, please refer to the ns-3 manual.
 */
typedef enum
{
  TX_OPTION_ACK = 1,
  TX_OPTION_GTS = 2,
  TX_OPTION_INDIRECT = 4
} LrWpanTxOption;

typedef enum
{
  MAC_IDLE,
  MAC_CSMA,
  MAC_SENDING,
  MAC_ACK_PENDING,
  CHANNEL_ACCESS_FAILURE,
  CHANNEL_IDLE,
  SET_PHY_TX_ON
} LrWpanMacState;

/**
 * table 80 of 802.15.4
 */
typedef enum
{
  NO_PANID_ADDR = 0,
  ADDR_MODE_RESERVED = 1,
  SHORT_ADDR = 2,
  EXT_ADDR = 3
} LrWpanAddressMode;

/**
 * table 83 of 802.15.4
 */
typedef enum
{
  ASSOCIATED = 0,
  PAN_AT_CAPACITY = 1,
  PAN_ACCESS_DENIED = 2,
  ASSOCIATED_WITHOUT_ADDRESS = 0xfe,
  DISASSOCIATED = 0xff
} LrWpanAssociationStatus;

/*
 * Table 42 of 802.15.4-2006
 */
typedef enum
{
  IEEE_802_15_4_SUCCESS                = 0,
  IEEE_802_15_4_TRANSACTION_OVERFLOW   = 1,
  IEEE_802_15_4_TRANSACTION_EXPIRED    = 2,
  IEEE_802_15_4_CHANNEL_ACCESS_FAILURE = 3,
  IEEE_802_15_4_INVALID_ADDRESS        = 4,
  IEEE_802_15_4_INVALID_GTS            = 5,
  IEEE_802_15_4_NO_ACK                 = 6,
  IEEE_802_15_4_COUNTER_ERROR          = 7,
  IEEE_802_15_4_FRAME_TOO_LONG         = 8,
  IEEE_802_15_4_UNAVAILABLE_KEY        = 9,
  IEEE_802_15_4_UNSUPPORTED_SECURITY   = 10,
  IEEE_802_15_4_INVALID_PARAMETER      = 11
} LrWpanMcpsDataConfirmStatus;

struct McpsDataRequestParams
{
  McpsDataRequestParams () :
    m_srcAddrMode (SHORT_ADDR), m_dstAddrMode (SHORT_ADDR), m_dstPanId (0),
    m_dstAddr (), m_msduHandle (0), m_txOptions (0)
  { };
  LrWpanAddressMode m_srcAddrMode;
  LrWpanAddressMode m_dstAddrMode;
  uint16_t m_dstPanId;
  Mac16Address m_dstAddr;
  uint8_t m_msduHandle;
  uint8_t m_txOptions;  // bitmap
};

struct McpsDataConfirmParams
{
  uint8_t m_msduHandle;
  LrWpanMcpsDataConfirmStatus m_status;
};

struct McpsDataIndicationParams
{
  uint8_t m_srcAddrMode;
  uint16_t m_srcPanId;
  Mac16Address m_srcAddr;
  uint8_t m_dstAddrMode;
  uint16_t m_dstPanId;
  Mac16Address m_dstAddr;
  uint8_t m_mpduLinkQuality;
  uint8_t m_dsn;
};

// This callback is called after a McpsDataRequest has been called from
// the higher layer.  It returns a status of the outcome of the
// transmission request
typedef Callback<void, McpsDataConfirmParams> McpsDataConfirmCallback;

// This callback is called after a Mcps has successfully received a
// frame and wants to deliver it to the higher layer.
//
// XXX for now, we do not deliver all of the parameters in section
// 7.1.1.3.1 but just send up the packet.
typedef Callback<void, McpsDataIndicationParams, Ptr<Packet> > McpsDataIndicationCallback;


/**
 * \ingroup lr-wpan
 *
 * Class that implements the LR-WPAN Mac state machine
 */
class LrWpanMac : public Object
{
  friend class LrWpanCsmaCa;
public:
  static TypeId GetTypeId (void);

  static const uint32_t aMinMPDUOverhead; // Table 85

  LrWpanMac ();
  virtual ~LrWpanMac ();

  bool GetRxOnWhenIdle ();
  void SetRxOnWhenIdle (bool rxOnWhenIdle);

  // XXX these setters will become obsolete if we use the attribute system
  void SetShortAddress (Mac16Address address);
  Mac16Address GetShortAddress (void) const;
  void SetExtendedAddress (Mac64Address address);
  Mac64Address GetExtendedAddress (void) const;
  void SetPanId (uint16_t panId);
  uint16_t GetPanId (void) const;

  // interface between SSCS and MAC
  /**
   * specified in Section 7.1.1.1
   *
   * XXX for now, we do not send down all of the parameters detailed in
   * section 7.1.1.1.1 but just send down the packet.
   * @param p the packet to send
   */
  void McpsDataRequest (McpsDataRequestParams params, Ptr<Packet> p);
  /* for debug we send down with delay*/

  void SetCsmaCa (Ptr<LrWpanCsmaCa> csmaCa);
  void SetPhy (Ptr<LrWpanPhy> phy);
  Ptr<LrWpanPhy> GetPhy (void);

  void SetMcpsDataIndicationCallback (McpsDataIndicationCallback c);
  void SetMcpsDataConfirmCallback (McpsDataConfirmCallback c);

  // interfaces between MAC and PHY
  /**
   *  IEEE 802.15.4-2006 section 6.2.1.3
   *  PD-DATA.indication
   *  Indicates the transfer of an MPDU from PHY to MAC (receiving)
   *  @param psduLength number of bytes in the PSDU
   *  @param p the packet to be transmitted
   *  @param lqi Link quality (LQI) value measured during reception of the PPDU
   */
  void PdDataIndication (uint32_t psduLength, Ptr<Packet> p, uint8_t lqi);

  /**
   *  IEEE 802.15.4-2006 section 6.2.1.2
   *  Confirm the end of transmission of an MPDU to MAC
   *  @param status to report to MAC
   *  PHY PD-DATA.confirm status
   */
  void PdDataConfirm (LrWpanPhyEnumeration status);

  /**
   *  IEEE 802.15.4-2006 section 6.2.2.2
   *  PLME-CCA.confirm status
   *  @param status TRX_OFF, BUSY or IDLE
   */
  void PlmeCcaConfirm (LrWpanPhyEnumeration status);

  /**
   *  IEEE 802.15.4-2006 section 6.2.2.4
   *  PLME-ED.confirm status and energy level
   *  @param status SUCCESS, TRX_OFF or TX_ON
   *  @param energyLevel 0x00-0xff ED level for the channel
   */
  void PlmeEdConfirm (LrWpanPhyEnumeration status, uint8_t energyLevel);

  /**
   *  IEEE 802.15.4-2006 section 6.2.2.6
   *  PLME-GET.confirm
   *  Get attributes per definition from Table 23 in section 6.4.2
   *  @param status SUCCESS or UNSUPPORTED_ATTRIBUTE
   *  @param id the attributed identifier
   *  @param * attribute the attribute value
   */
  void PlmeGetAttributeConfirm (LrWpanPhyEnumeration status,
                                LrWpanPibAttributeIdentifier id,
                                LrWpanPhyPibAttributes* attribute);

  /**
   *  IEEE 802.15.4-2006 section 6.2.2.8
   *  PLME-SET-TRX-STATE.confirm
   *  Set PHY state
   *  @param state in RX_ON,TRX_OFF,FORCE_TRX_OFF,TX_ON
   */
  void PlmeSetTRXStateConfirm (LrWpanPhyEnumeration status);

  /**
   *  IEEE 802.15.4-2006 section 6.2.2.10
   *  PLME-SET.confirm
   *  Set attributes per definition from Table 23 in section 6.4.2
   *  @param status SUCCESS, UNSUPPORTED_ATTRIBUTE, INVALID_PARAMETER, or READ_ONLY
   *  @param id the attributed identifier
   */
  void PlmeSetAttributeConfirm (LrWpanPhyEnumeration status,
                                LrWpanPibAttributeIdentifier id);


  /**
   * CSMA-CA algorithm calls back the MAC after executing channel assessment
   */
  void SetLrWpanMacState (LrWpanMacState macState);

  /**
   * \return current association status
   */
  LrWpanAssociationStatus GetAssociationStatus (void) const;

  /**
   * \param status new association status
   */
  void SetAssociationStatus (LrWpanAssociationStatus status);

  //MAC sublayer constants
  uint64_t m_aBaseSlotDuration;         // 60 symbols in each superframe slot
  uint64_t m_aNumSuperframeSlots;       // 16 slots in each superframe
  uint64_t m_aBaseSuperframeDuration;   // aBaseSlotDuration * aNumSuperframeSlots in symbols
  
  //MAC PIB attributes
  uint64_t m_macBeaconTxTime;           // time the device tx last beacon frame in symbols, only 24 bits used
  uint64_t m_macSyncSymbolOffset;       // symbol boundary is same as m_macBeaconTxTime
  uint64_t m_macBeaconOrder;            // 0..14 and 15 mean no beacon frame sent
  uint64_t m_macSuperframeOrder;        // 0..14 and 15 means superframe shall not remain active after beacon
  bool m_macPromiscuousMode;            // Indicates if MAC sublayer is in receive all mode. True mean accept all frames from PHY.
  uint16_t m_macPanId;                  // 16bits id of PAN on which this device is operating. 0xffff means not asscoiated
  SequenceNumber16 m_macDsn;            // Seq num added to transmitted data or MAC command frame 00-ff
  uint8_t m_macMaxFrameRetries;         // The maximum number of retries allowed after a transmission failure
  
  uint64_t GetMacAckWaitDuration (void) const;
  uint8_t GetMacMaxFrameRetries (void) const;
  void SetMacMaxFrameRetries (uint8_t retries);

protected:
  virtual void DoInitialize (void);
  virtual void DoDispose (void);

private:
  struct TxQueueElement
  {
    uint8_t txQMsduHandle;
    Ptr<Packet> txQPkt;
  };

  void SendAck (uint8_t seqno);
  void RemoveFirstTxQElement (void);
  void ChangeMacState (LrWpanMacState newState);
  void AckWaitTimeout (void);
  bool PrepareRetransmission (void);
  void CheckQueue (void);


  TracedCallback<Ptr<const Packet>, bool> m_macTxQueueTrace;
  /**
   * The trace source fired when packets come into the "top" of the device
   * at the L3/L2 transition, before being queued for transmission.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_macTxTrace;

  /**
   * The trace source fired when packets coming into the "top" of the device
   * at the L3/L2 transition are dropped before being queued for transmission.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_macTxDropTrace;

  /**
   * The trace source fired for packets successfully received by the device
   * immediately before being forwarded up to higher layers (at the L2/L3
   * transition).  This is a promiscuous trace.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_macPromiscRxTrace;

  /**
   * The trace source fired for packets successfully received by the device
   * immediately before being forwarded up to higher layers (at the L2/L3
   * transition).  This is a non-promiscuous trace.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_macRxTrace;

  /**
   * The trace source fired for packets successfully received by the device
   * but dropped before being forwarded up to higher layers (at the L2/L3
   * transition).
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_macRxDropTrace;

  /**
   * A trace source that emulates a non-promiscuous protocol sniffer connected
   * to the device.  Unlike your average everyday sniffer, this trace source
   * will not fire on PACKET_OTHERHOST events.
   *
   * On the transmit size, this trace hook will fire after a packet is dequeued
   * from the device queue for transmission.  In Linux, for example, this would
   * correspond to the point just before a device hard_start_xmit where
   * dev_queue_xmit_nit is called to dispatch the packet to the PF_PACKET
   * ETH_P_ALL handlers.
   *
   * On the receive side, this trace hook will fire when a packet is received,
   * just before the receive callback is executed.  In Linux, for example,
   * this would correspond to the point at which the packet is dispatched to
   * packet sniffers in netif_receive_skb.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_snifferTrace;

  /**
   * A trace source that emulates a promiscuous mode protocol sniffer connected
   * to the device.  This trace source fire on packets destined for any host
   * just like your average everyday packet sniffer.
   *
   * On the transmit size, this trace hook will fire after a packet is dequeued
   * from the device queue for transmission.  In Linux, for example, this would
   * correspond to the point just before a device hard_start_xmit where
   * dev_queue_xmit_nit is called to dispatch the packet to the PF_PACKET
   * ETH_P_ALL handlers.
   *
   * On the receive side, this trace hook will fire when a packet is received,
   * just before the receive callback is executed.  In Linux, for example,
   * this would correspond to the point at which the packet is dispatched to
   * packet sniffers in netif_receive_skb.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_promiscSnifferTrace;

  /*
   * A trace source that fires when the LrWpanMac changes states
   */
  TracedCallback<Time, LrWpanMacState, LrWpanMacState> m_macStateLogger;

  Ptr<LrWpanPhy> m_phy;
  Ptr<LrWpanCsmaCa> m_csmaCa;
  McpsDataIndicationCallback m_mcpsDataIndicationCallback;
  McpsDataConfirmCallback m_mcpsDataConfirmCallback;
  LrWpanMacState m_lrWpanMacState;
  LrWpanAssociationStatus m_associationStatus;
  Ptr<Packet> m_txPkt;  // XXX need packet buffer instead of single packet
  Mac16Address m_shortAddress;
  Mac64Address m_selfExt;
  std::deque<TxQueueElement*> m_txQueue;
  uint8_t m_retransmission;
  EventId m_ackWaitTimeout;
  EventId m_setMacState;
  bool m_macRxOnWhenIdle;
};


} // namespace ns3

#endif /* LR_WPAN_MAC_H */
