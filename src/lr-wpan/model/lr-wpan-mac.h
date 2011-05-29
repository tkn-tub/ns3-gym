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
 */
#ifndef LR_WPAN_MAC_H
#define LR_WPAN_MAC_H

#include <stdint.h>
#include <math.h>
#include "ns3/object.h"
#include "ns3/callback.h"
#include "lr-wpan-phy.h"
#include "lr-wpan-mac-header.h"
#include "lr-wpan-mac-trailer.h"


namespace ns3 {

typedef enum
{
  MAC_IDLE,
  CHANNEL_BUSY,
  CHANNEL_IDLE,
  SET_PHY_TX_ON
} LrWpanMacState;

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

// This callback is called after a McpsDataRequest has been called from
// the higher layer.  It returns a status of the outcome of the
// transmission request
typedef Callback< void, LrWpanMcpsDataConfirmStatus > McpsDataConfirmCallback;

// This callback is called after a Mcps has successfully received a
// frame and wants to deliver it to the higher layer.
//
// XXX for now, we do not deliver all of the parameters in section
// 7.1.1.3.1 but just send up the packet.
typedef Callback< void, Ptr<Packet> > McpsDataIndicationCallback;

class LrWpanCsmaCa;

/**
 * Class that implements the LR-WPAN Mac state machine
 */
class LrWpanMac : public Object
{
  friend class LrWpanCsmaCa;
public:
  static TypeId GetTypeId (void);

  LrWpanMac ();
  virtual ~LrWpanMac ();

  // interface between SSCS and MAC
  /**
   * specified in Section 7.1.1.1
   *
   * XXX for now, we do not send down all of the parameters detailed in
   * section 7.1.1.1.1 but just send down the packet.
   * @param p the packet to send
   */
  void McpsDataRequest (Ptr<Packet> p);

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
  void PdDataIndication (uint32_t psduLength, Ptr<Packet> p, uint32_t lqi);

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
                                LrWpanPhyPIBAttributes* attribute);

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

private:
  virtual void DoDispose (void);
  Ptr<LrWpanPhy> m_phy;
  Ptr<LrWpanCsmaCa> m_csmaCa;
  McpsDataIndicationCallback m_mcpsDataIndicationCallback;
  McpsDataConfirmCallback m_mcpsDataConfirmCallback;
  LrWpanMacState m_lrWpanMacState;
  Ptr<Packet> m_txPkt;  // XXX need packet buffer instead of single packet
};


} // namespace ns3

#endif /* LR_WPAN_MAC_H */
