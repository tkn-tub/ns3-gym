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
#include "ns3/simulator.h"
#include "ns3/lr-wpan-mac.h"
#include "ns3/log.h"
#include "ns3/lr-wpan-csmaca.h"

NS_LOG_COMPONENT_DEFINE ("LrWpanMac");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LrWpanMac);

TypeId
LrWpanMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LrWpanMac")
    .SetParent<Object> ()
    .AddConstructor<LrWpanMac> ()
  ;
  return tid;
}

LrWpanMac::LrWpanMac ()
{
  m_lrWpanMacState = MAC_IDLE;
}

LrWpanMac::~LrWpanMac ()
{
}

void
LrWpanMac::DoDispose ()
{
  m_csmaCa->Dispose ();
  m_csmaCa = 0;
  m_txPkt = 0;
  m_phy = 0;
  m_mcpsDataIndicationCallback = MakeNullCallback< void, Ptr<Packet> > ();
  m_mcpsDataConfirmCallback = MakeNullCallback< void, LrWpanMcpsDataConfirmStatus > ();
}

void
LrWpanMac::McpsDataRequest (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);
  if (!m_mcpsDataConfirmCallback.IsNull ())
    {
      // XXX for now, we are very accepting of whatever is passed down to us
      m_mcpsDataConfirmCallback (IEEE_802_15_4_SUCCESS);
    }

  LrWpanMacHeader macHdr (LrWpanMacHeader::LRWPAN_MAC_DATA, 0);        // sequence number set to 0
  macHdr.SetSrcAddrMode (LrWpanMacHeader::SHORTADDR);                  // short addr
  macHdr.SetDstAddrMode (LrWpanMacHeader::NOADDR);
  macHdr.SetSecDisable ();
  macHdr.SetNoPanIdComp ();
  uint16_t shortAddr = 66;
  uint16_t srcPanId = 100;
  WpanAddress srcWpanAddr;
  srcWpanAddr.SetAddress (shortAddr);
  macHdr.SetSrcAddrFields (srcPanId,srcWpanAddr );
  p->AddHeader (macHdr);

  LrWpanMacTrailer macTrailer;
  p->AddTrailer (macTrailer);

  m_txPkt = p;

  // Start CCA process for this packet
  Simulator::ScheduleNow (&LrWpanCsmaCa::Start, m_csmaCa);

}

void LrWpanMac::SetCsmaCa (Ptr<LrWpanCsmaCa> csmaCa)
{
  m_csmaCa = csmaCa;
}

void LrWpanMac::SetPhy (Ptr<LrWpanPhy> phy)
{
  m_phy = phy;
}

Ptr<LrWpanPhy> LrWpanMac::GetPhy (void)
{
  return m_phy;
}

void
LrWpanMac::SetMcpsDataIndicationCallback (McpsDataIndicationCallback c)
{
  m_mcpsDataIndicationCallback = c;
}

void
LrWpanMac::SetMcpsDataConfirmCallback (McpsDataConfirmCallback c)
{
  m_mcpsDataConfirmCallback = c;
}

void
LrWpanMac::PdDataIndication (uint32_t psduLength, Ptr<Packet> p, uint32_t lqi)
{
  NS_LOG_FUNCTION (this << psduLength << p << lqi);

  LrWpanMacHeader receivedMacHdr;
  p->RemoveHeader (receivedMacHdr);
  LrWpanMacTrailer receivedMacTrailer;
  p->RemoveTrailer (receivedMacTrailer);

  if (!m_mcpsDataIndicationCallback.IsNull ())
    {
      // All packets are successfully received for now
      m_mcpsDataIndicationCallback (p);
    }

}


void
LrWpanMac::PdDataConfirm (LrWpanPhyEnumeration status)
{
  NS_LOG_FUNCTION (this << status);
  m_phy->PlmeSetTRXStateRequest (IEEE_802_15_4_PHY_RX_ON);
  m_txPkt = 0;
}

void
LrWpanMac::PlmeCcaConfirm (LrWpanPhyEnumeration status)
{
  NS_LOG_FUNCTION (this << status);
  // Direct this call through the csmaCa object
  m_csmaCa->PlmeCcaConfirm (status);
}

void
LrWpanMac::PlmeEdConfirm (LrWpanPhyEnumeration status, uint8_t energyLevel)
{
  NS_LOG_FUNCTION (this << status << energyLevel);

}

void
LrWpanMac::PlmeGetAttributeConfirm (LrWpanPhyEnumeration status,
                                    LrWpanPibAttributeIdentifier id,
                                    LrWpanPhyPIBAttributes* attribute)
{
  NS_LOG_FUNCTION (this << status << id << attribute);
}

void
LrWpanMac::PlmeSetTRXStateConfirm (LrWpanPhyEnumeration status)
{
  NS_LOG_FUNCTION (this << status);
  if (status == IEEE_802_15_4_PHY_TX_ON || status == IEEE_802_15_4_PHY_SUCCESS)
    {
      if ((m_txPkt) && (m_lrWpanMacState == SET_PHY_TX_ON))
        {
          m_phy->PdDataRequest (m_txPkt->GetSize (), m_txPkt);
        }
    }
}

void
LrWpanMac::PlmeSetAttributeConfirm (LrWpanPhyEnumeration status,
                                    LrWpanPibAttributeIdentifier id)
{
  NS_LOG_FUNCTION (this << status << id);
}

void
LrWpanMac::SetLrWpanMacState (LrWpanMacState macState)
{
  NS_LOG_FUNCTION (this << macState);
  m_lrWpanMacState = macState;
  if ((m_lrWpanMacState == CHANNEL_IDLE) && (m_txPkt))
    {
      // Channel is idle, set transmitter to TX_ON
      m_lrWpanMacState = SET_PHY_TX_ON;
      m_phy->PlmeSetTRXStateRequest (IEEE_802_15_4_PHY_TX_ON);
    }
}
} // namespace ns3
