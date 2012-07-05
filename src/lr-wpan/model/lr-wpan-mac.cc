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
 */
#include "ns3/simulator.h"
#include "ns3/lr-wpan-mac.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/lr-wpan-csmaca.h"
#include "ns3/lr-wpan-phy.h"
#include "ns3/mac16-address.h"
#include "ns3/mac64-address.h"

NS_LOG_COMPONENT_DEFINE ("LrWpanMac");

#undef NS_LOG_APPEND_CONTEXT
#define NS_LOG_APPEND_CONTEXT                                   \
  std::clog << "[address " << m_shortAddress << "] ";

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LrWpanMac);

const uint32_t LrWpanMac::aMinMPDUOverhead = 9; // Table 85

TypeId
LrWpanMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LrWpanMac")
    .SetParent<Object> ()
    .AddConstructor<LrWpanMac> ()
    .AddAttribute ("PanId", "16-bit identifier of the associated PAN",
                   UintegerValue (),
                   MakeUintegerAccessor (&LrWpanMac::m_macPanId),
                   MakeUintegerChecker<uint16_t> ())
    .AddTraceSource ("MacTx",
                     "Trace source indicating a packet has arrived for transmission by this device",
                     MakeTraceSourceAccessor (&LrWpanMac::m_macTxTrace))
    .AddTraceSource ("MacTxDrop",
                     "Trace source indicating a packet has been dropped by the device before transmission",
                     MakeTraceSourceAccessor (&LrWpanMac::m_macTxDropTrace))
    .AddTraceSource ("MacPromiscRx",
                     "A packet has been received by this device, has been passed up from the physical layer "
                     "and is being forwarded up the local protocol stack.  This is a promiscuous trace,",
                     MakeTraceSourceAccessor (&LrWpanMac::m_macPromiscRxTrace))
    .AddTraceSource ("MacRx",
                     "A packet has been received by this device, has been passed up from the physical layer "
                     "and is being forwarded up the local protocol stack.  This is a non-promiscuous trace,",
                     MakeTraceSourceAccessor (&LrWpanMac::m_macRxTrace))
    .AddTraceSource ("MacRxDrop",
                     "Trace source indicating a packet was received, but dropped before being forwarded up the stack",
                     MakeTraceSourceAccessor (&LrWpanMac::m_macRxDropTrace))
    .AddTraceSource ("Sniffer",
                     "Trace source simulating a non-promiscuous packet sniffer attached to the device",
                     MakeTraceSourceAccessor (&LrWpanMac::m_snifferTrace))
    .AddTraceSource ("PromiscSniffer",
                     "Trace source simulating a promiscuous packet sniffer attached to the device",
                     MakeTraceSourceAccessor (&LrWpanMac::m_promiscSnifferTrace))
    .AddTraceSource ("MacState",
                     "The state of LrWpan Mac",
                     MakeTraceSourceAccessor (&LrWpanMac::m_macStateLogger))
  ;
  return tid;
}

LrWpanMac::LrWpanMac ()
{

  ChangeMacState (MAC_IDLE);
  m_macPanId = 0;
  m_associationStatus = ASSOCIATED;
  m_selfExt = Mac64Address::Allocate ();
  m_macPromiscuousMode = false;

  SeedManager::SetSeed (100);
  UniformVariable uniformVar;
  uniformVar = UniformVariable (0, 255);
  m_macDsn = SequenceNumber16 (uniformVar.GetValue ());
  m_shortAddress = Mac16Address ("00:00");
}

LrWpanMac::~LrWpanMac ()
{
}

void
LrWpanMac::DoDispose ()
{
  if (m_csmaCa != 0)
    {
      m_csmaCa->Dispose ();
      m_csmaCa = 0;
    }
  m_txPkt = 0;
  for (uint32_t i = 0; i < m_txQueue.size (); i++)
    {
      m_txQueue[i]->txQPkt = 0;
      delete m_txQueue[i];
    }
  m_txQueue.clear ();
  m_phy = 0;
  m_mcpsDataIndicationCallback = MakeNullCallback< void, McpsDataIndicationParams, Ptr<Packet> > ();
  m_mcpsDataConfirmCallback = MakeNullCallback< void, McpsDataConfirmParams > ();
}


void
LrWpanMac::SetShortAddress (Mac16Address address)
{
  //NS_LOG_FUNCTION (this << address);
  m_shortAddress = address;
}

void
LrWpanMac::SetExtendedAddress (Mac64Address address)
{
  //NS_LOG_FUNCTION (this << address);
  m_selfExt = address;
}


Mac16Address
LrWpanMac::GetShortAddress () const
{
  NS_LOG_FUNCTION (this);
  return m_shortAddress;
}

Mac64Address
LrWpanMac::GetExtendedAddress () const
{
  NS_LOG_FUNCTION (this);
  return m_selfExt;
}
void
LrWpanMac::McpsDataRequest (McpsDataRequestParams params, Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);
  McpsDataConfirmParams confirmParams;
  confirmParams.m_msduHandle = params.m_msduHandle;

  LrWpanMacHeader macHdr (LrWpanMacHeader::LRWPAN_MAC_DATA, (uint8_t)m_macDsn.GetValue ());
  m_macDsn++;
  if (m_macDsn > SequenceNumber16 (255))
    {
      m_macDsn = m_macDsn - SequenceNumber16 (255);
    }

  if (p->GetSize () > LrWpanPhy::aMaxPhyPacketSize - aMinMPDUOverhead)
    {
      // Note, this is just testing maximum theoretical frame size per the spec
      // The frame could still be too large once headers are put on
      // in which case the phy will reject it instead
      NS_LOG_ERROR (this << " packet too big: " << p->GetSize ());
      confirmParams.m_status = IEEE_802_15_4_FRAME_TOO_LONG;
      if (!m_mcpsDataConfirmCallback.IsNull ())
        {
          m_mcpsDataConfirmCallback (confirmParams);
        }
      return;
    }

  if ((params.m_srcAddrMode == NO_PANID_ADDR)
      && (params.m_dstAddrMode == NO_PANID_ADDR))
    {
      NS_LOG_ERROR (this << " Can not send packet with no Address field" );
      confirmParams.m_status = IEEE_802_15_4_INVALID_ADDRESS;
      if (!m_mcpsDataConfirmCallback.IsNull ())
        {
          m_mcpsDataConfirmCallback (confirmParams);
        }
      return;
    }
  switch (params.m_srcAddrMode)
    {
    case NO_PANID_ADDR:
      macHdr.SetSrcAddrMode (params.m_srcAddrMode);
      macHdr.SetNoPanIdComp ();
      break;
    case ADDR_MODE_RESERVED:
      macHdr.SetSrcAddrMode (params.m_srcAddrMode);
      break;
    case SHORT_ADDR:
      macHdr.SetSrcAddrMode (params.m_srcAddrMode);
      macHdr.SetSrcAddrFields (GetPanId (), GetShortAddress ());
      break;
    case EXT_ADDR:
      macHdr.SetSrcAddrMode (params.m_srcAddrMode);
      macHdr.SetSrcAddrFields (GetPanId (), GetExtendedAddress ());
      break;
    default:
      NS_LOG_ERROR (this << " Can not send packet with incorrect Source Address mode = " << params.m_srcAddrMode);
      confirmParams.m_status = IEEE_802_15_4_INVALID_ADDRESS;
      if (!m_mcpsDataConfirmCallback.IsNull ())
        {
          m_mcpsDataConfirmCallback (confirmParams);
        }
      return;
    }

  macHdr.SetDstAddrMode (params.m_dstAddrMode);
  macHdr.SetDstAddrFields (params.m_dstPanId, params.m_dstAddr);
  macHdr.SetSecDisable ();
  //extract the last 3 bits in TxOptions and map to macHdr
  int b0 = params.m_txOptions & 1;
  int b1 = params.m_txOptions & 2;
  int b2 = params.m_txOptions & 4;
  if (b0 == 1)
    {
      macHdr.SetAckReq ();
    }
  else if (b0 == 0)
    {
      macHdr.SetNoAckReq ();
    }
  else
    {
      confirmParams.m_status = IEEE_802_15_4_INVALID_PARAMETER;
      NS_LOG_ERROR (this << "Incorrect TxOptions bit 0 not 0/1");
      if (!m_mcpsDataConfirmCallback.IsNull ())
        {
          m_mcpsDataConfirmCallback (confirmParams);
        }
      return;
    }

  //if is Slotted CSMA means its beacon enabled
  if (m_csmaCa->isSlottedCsmaCa ())
    {
      if (b1 == 2)
        {
          //TODO:GTS Transmission
        }
      else if (b1 == 0)
        {
          //TODO:CAP Transmission
        }
      else
        {
          NS_LOG_ERROR (this << "Incorrect TxOptions bit 1 not 0/1");
          confirmParams.m_status = IEEE_802_15_4_INVALID_PARAMETER;
          if (!m_mcpsDataConfirmCallback.IsNull ())
            {
              m_mcpsDataConfirmCallback (confirmParams);
            }
          return;
        }
    }
  else
    {
      if (b1 != 0)
        {
          NS_LOG_ERROR (this << "for non-beacon-enables PAN, bit 1 should always be set to 0");
          confirmParams.m_status = IEEE_802_15_4_INVALID_PARAMETER;
          if (!m_mcpsDataConfirmCallback.IsNull ())
            {
              m_mcpsDataConfirmCallback (confirmParams);
            }
          return;
        }
    }

  if (b2 == 4)
    {
      //TODO :indirect tx
    }
  else if (b2 == 0)
    {
      //TODO :direct tx
    }
  else
    {
      NS_LOG_ERROR (this << "Incorrect TxOptions bit 2 not 0/1");
      confirmParams.m_status = IEEE_802_15_4_INVALID_PARAMETER;
      if (!m_mcpsDataConfirmCallback.IsNull ())
        {
          m_mcpsDataConfirmCallback (confirmParams);
        }
      return;
    }

  p->AddHeader (macHdr);

  LrWpanMacTrailer macTrailer;
  p->AddTrailer (macTrailer);

  m_macTxTrace (p);
  confirmParams.m_status = IEEE_802_15_4_SUCCESS;
  if (!m_mcpsDataConfirmCallback.IsNull ())
    {
      m_mcpsDataConfirmCallback (confirmParams);
    }

  TxQueueElement *txQElement = new TxQueueElement;
  txQElement->txQMsduHandle = params.m_msduHandle;
  txQElement->txQPkt = p;
  if (m_txQueue.empty ())
    {
      m_txPkt = txQElement->txQPkt->Copy ();
      m_txQueue.push_front (txQElement); // the first packet in the queue is currently worked on
      // Start CCA process for this packet
      Simulator::ScheduleNow (&LrWpanCsmaCa::Start, m_csmaCa);
    }
  else
    {
      // if queue is not empty put this packet at the end of Tx queue
      m_txQueue.push_back (txQElement);
    }
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

  bool acceptFrame;

  Ptr<Packet> originalPkt = p->Copy (); // because we will strip headers
  LrWpanMacHeader receivedMacHdr;
  p->RemoveHeader (receivedMacHdr);
  LrWpanMacTrailer receivedMacTrailer;
  p->RemoveTrailer (receivedMacTrailer);

  m_promiscSnifferTrace (originalPkt);

  McpsDataIndicationParams params;
  params.m_dsn = receivedMacHdr.GetSeqNum ();
  params.m_mpduLinkQuality = lqi;
  params.m_srcPanId = receivedMacHdr.GetSrcPanId ();
  params.m_srcAddrMode = receivedMacHdr.GetSrcAddrMode ();
  if (params.m_srcAddrMode == SHORT_ADDR)
    {
      params.m_srcAddr = receivedMacHdr.GetShortSrcAddr ();
    }
  params.m_dstPanId = receivedMacHdr.GetDstPanId ();
  params.m_dstAddrMode = receivedMacHdr.GetDstAddrMode ();
  if (params.m_dstAddrMode == SHORT_ADDR)
    {
      params.m_dstAddr = receivedMacHdr.GetShortDstAddr ();
    }

  NS_LOG_DEBUG ("Packet from " << params.m_srcAddr << " to " << params.m_dstAddr);

  // from sec 7.5.6.2 Reception and rejection, Std802.15.4-2006
  // level 1 filtering, test FCS field and reject if frame fails
  // level 2 filtering if promiscuous mode pass frame to higher layer otherwise perform level 3 filtering
  // level 3 filtering accept frame
  // if Frame type and version is not reserved, and
  // if there is a dstPanId then dstPanId=m_macPanId or broadcastPanI, and
  // if there is a shortDstAddr then shortDstAddr =shortMacAddr or broadcastAddr, and
  // if beacon frame then srcPanId = m_macPanId
  // if only srcAddr field in Data or Command frame,accept frame if srcPanId=m_macPanId

  m_macPromiscRxTrace (originalPkt);
  // XXX no rejection tracing (to macRxDropTrace) being performed below

  if (m_macPromiscuousMode)
    {
      //level 2 filtering
      if (!m_mcpsDataIndicationCallback.IsNull ())
        {
          NS_LOG_DEBUG ("promiscuous mode, forwarding up");
          m_mcpsDataIndicationCallback (params, p);
        }
      else
        {
          NS_LOG_ERROR (this << " Data Indication Callback not initialised");
        }
    }
  else
    {
      //level 3 frame filtering
      acceptFrame = (receivedMacHdr.GetType () != LrWpanMacHeader::LRWPAN_MAC_RESERVED);

      if (acceptFrame)
        {
          acceptFrame = (receivedMacHdr.GetFrameVer () <= 1);
        }

      if (acceptFrame
          && (receivedMacHdr.GetDstAddrMode () > 1))
        {
          acceptFrame = receivedMacHdr.GetDstPanId () == m_macPanId
            || receivedMacHdr.GetDstPanId () == 0xffff;
        }

      if (acceptFrame
          && (receivedMacHdr.GetDstAddrMode () == 2))
        {
          acceptFrame = receivedMacHdr.GetShortDstAddr () == m_shortAddress
            || receivedMacHdr.GetShortDstAddr () == Mac16Address ("ff:ff");        // check for broadcast addrs
        }

      if (acceptFrame
          && (receivedMacHdr.GetDstAddrMode () == 3))
        {
          acceptFrame = (receivedMacHdr.GetExtDstAddr () == m_selfExt);
        }

      if (acceptFrame
          && (receivedMacHdr.GetType () == LrWpanMacHeader::LRWPAN_MAC_BEACON))
        {
          if (m_macPanId == 0xffff)
            {
              acceptFrame = true;
            }
          else
            {
              acceptFrame = receivedMacHdr.GetSrcPanId () == m_macPanId;
            }
        }

      if (acceptFrame
          && ((receivedMacHdr.GetType () == LrWpanMacHeader::LRWPAN_MAC_DATA)
              || (receivedMacHdr.GetType () == LrWpanMacHeader::LRWPAN_MAC_COMMAND))
          && (receivedMacHdr.GetSrcAddrMode () > 1))
        {
          acceptFrame = receivedMacHdr.GetSrcPanId () == m_macPanId; //TODO need to check if PAN coord
        }

      if (acceptFrame)
        {
          m_macRxTrace (originalPkt);
          if (!m_mcpsDataIndicationCallback.IsNull ())
            {
              NS_LOG_DEBUG ("PdDataIndication():  Packet is for me; forwarding up");
              m_mcpsDataIndicationCallback (params, p);
            }
          else
            {
              NS_LOG_ERROR (this << " Data Indication Callback not initialised");
            }
        }
      else
        {
          m_macRxDropTrace (originalPkt);
          p = 0;
        }
    }

}


void
LrWpanMac::PdDataConfirm (LrWpanPhyEnumeration status)
{
  NS_LOG_FUNCTION (this << status << m_txQueue.empty () ? 0 : m_txQueue.size ());
  m_phy->PlmeSetTRXStateRequest (IEEE_802_15_4_PHY_RX_ON);

  if (status == IEEE_802_15_4_PHY_SUCCESS || status == IEEE_802_15_4_PHY_UNSPECIFIED)
    {
      // remove the copy of the packet that was just sent
      TxQueueElement *txQElement = m_txQueue.front ();
      txQElement->txQPkt = 0;
      delete txQElement;
      m_txQueue.pop_front ();
      ChangeMacState (CHANNEL_IDLE);
      if (!m_txQueue.empty ())
        {
          NS_LOG_FUNCTION ("PdDataConfirm:  start CCA process for next packet");
          // Start CCA process for next packet in the tx queue
          m_txPkt = m_txQueue.front ()->txQPkt->Copy ();
          Simulator::ScheduleNow (&LrWpanCsmaCa::Start, m_csmaCa);
        }
    }
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
          m_promiscSnifferTrace (m_txPkt);
          m_snifferTrace (m_txPkt);
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
  NS_LOG_FUNCTION (this << "mac state = " << macState);

  McpsDataConfirmParams confirmParams;

  ChangeMacState (macState);
  if ((m_lrWpanMacState == CHANNEL_IDLE) && (m_txPkt))
    {
      // Channel is idle, set transmitter to TX_ON
      NS_LOG_DEBUG ( this << " channel idle");
      ChangeMacState (SET_PHY_TX_ON);
      m_phy->PlmeSetTRXStateRequest (IEEE_802_15_4_PHY_TX_ON);
    }
  else if ((m_lrWpanMacState == CHANNEL_ACCESS_FAILURE) && (m_txPkt))
    {
      // cannot find a clear channel, drop the current packet.
      NS_LOG_DEBUG ( this << " cannot find clear channel");
      confirmParams.m_msduHandle = m_txQueue.front ()->txQMsduHandle;
      confirmParams.m_status = IEEE_802_15_4_CHANNEL_ACCESS_FAILURE;
      m_macTxDropTrace (m_txPkt);
      // remove the copy of the packet that was just sent
      TxQueueElement *txQElement = m_txQueue.front ();
      txQElement->txQPkt = 0;
      delete txQElement;
      m_txQueue.pop_front ();
      if (!m_mcpsDataConfirmCallback.IsNull ())
        {
          m_mcpsDataConfirmCallback (confirmParams);
        }
      // if there are more packets waiting to be sent
      if (!m_txQueue.empty ())
        {
          // Start CCA process for next packet in the tx queue
          m_txPkt = m_txQueue.front ()->txQPkt->Copy ();
          Simulator::ScheduleNow (&LrWpanCsmaCa::Start, m_csmaCa);
        }
    }

}

LrWpanAssociationStatus
LrWpanMac::GetAssociationStatus (void) const
{
  return m_associationStatus;
}

void
LrWpanMac::SetAssociationStatus (LrWpanAssociationStatus status)
{
  m_associationStatus = status;
}

uint16_t
LrWpanMac::GetPanId (void) const
{
  return m_macPanId;
}

void
LrWpanMac::SetPanId (uint16_t panId)
{
  m_macPanId = panId;
}

void
LrWpanMac::ChangeMacState (LrWpanMacState newState)
{
  NS_LOG_LOGIC (this << " change lrwpan mac state from "
                     << m_lrWpanMacState << " to "
                     << newState);
  m_macStateLogger (Simulator::Now (), m_lrWpanMacState, newState);
  m_lrWpanMacState = newState;
}
} // namespace ns3
