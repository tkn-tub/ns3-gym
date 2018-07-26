/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006, 2009 INRIA
 * Copyright (c) 2009 MIRKO BANCHI
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Mirko Banchi <mk.banchi@gmail.com>
 */

#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/random-variable-stream.h"
#include "qos-txop.h"
#include "channel-access-manager.h"
#include "mac-tx-middle.h"
#include "mgt-headers.h"
#include "wifi-mac-trailer.h"
#include "wifi-mac-queue.h"
#include "mac-low.h"
#include "qos-blocked-destinations.h"
#include "wifi-remote-station-manager.h"
#include "msdu-aggregator.h"
#include "mpdu-aggregator.h"
#include "ctrl-headers.h"

#undef NS_LOG_APPEND_CONTEXT
#define NS_LOG_APPEND_CONTEXT if (m_low != 0) { std::clog << "[mac=" << m_low->GetAddress () << "] "; }

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("QosTxop");

NS_OBJECT_ENSURE_REGISTERED (QosTxop);

TypeId
QosTxop::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::QosTxop")
    .SetParent<ns3::Txop> ()
    .SetGroupName ("Wifi")
    .AddConstructor<QosTxop> ()
    .AddTraceSource ("BackoffTrace",
                     "Trace source for backoff values",
                     MakeTraceSourceAccessor (&QosTxop::m_backoffTrace),
                     "ns3::TracedValueCallback::Uint32")
    .AddTraceSource ("CwTrace",
                     "Trace source for contention window values",
                     MakeTraceSourceAccessor (&QosTxop::m_cwTrace),
                     "ns3::TracedValueCallback::Uint32")
    .AddTraceSource ("TxopTrace",
                     "Trace source for txop start and duration times",
                     MakeTraceSourceAccessor (&QosTxop::m_txopTrace),
                     "ns3::TracedValueCallback::Time")
  ;
  return tid;
}

QosTxop::QosTxop ()
  : m_msduAggregator (0),
    m_mpduAggregator (0),
    m_typeOfStation (STA),
    m_blockAckType (COMPRESSED_BLOCK_ACK),
    m_startTxop (Seconds (0)),
    m_isAccessRequestedForRts (false),
    m_currentIsFragmented (false)
{
  NS_LOG_FUNCTION (this);
  m_qosBlockedDestinations = Create<QosBlockedDestinations> ();
  m_baManager = CreateObject<BlockAckManager> ();
  m_baManager->SetQueue (m_queue);
  m_baManager->SetBlockAckType (m_blockAckType);
  m_baManager->SetBlockDestinationCallback (MakeCallback (&QosBlockedDestinations::Block, m_qosBlockedDestinations));
  m_baManager->SetUnblockDestinationCallback (MakeCallback (&QosBlockedDestinations::Unblock, m_qosBlockedDestinations));
  m_baManager->SetMaxPacketDelay (m_queue->GetMaxDelay ());
  m_baManager->SetTxOkCallback (MakeCallback (&QosTxop::BaTxOk, this));
  m_baManager->SetTxFailedCallback (MakeCallback (&QosTxop::BaTxFailed, this));
}

QosTxop::~QosTxop ()
{
  NS_LOG_FUNCTION (this);
}

void
QosTxop::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_baManager = 0;
  m_qosBlockedDestinations = 0;
  m_msduAggregator = 0;
  m_mpduAggregator = 0;
  Txop::DoDispose ();
}

bool
QosTxop::GetBaAgreementExists (Mac48Address address, uint8_t tid) const
{
  return m_baManager->ExistsAgreement (address, tid);
}

void
QosTxop::CompleteAmpduTransfer (Mac48Address recipient, uint8_t tid)
{
  m_baManager->CompleteAmpduExchange (recipient, tid);
}

void
QosTxop::SetWifiRemoteStationManager (const Ptr<WifiRemoteStationManager> remoteManager)
{
  Txop::SetWifiRemoteStationManager (remoteManager);
  NS_LOG_FUNCTION (this << remoteManager);
  m_baManager->SetWifiRemoteStationManager (m_stationManager);
}

void
QosTxop::SetTypeOfStation (TypeOfStation type)
{
  NS_LOG_FUNCTION (this << +type);
  m_typeOfStation = type;
}

TypeOfStation
QosTxop::GetTypeOfStation (void) const
{
  return m_typeOfStation;
}

uint16_t
QosTxop::GetNextSequenceNumberFor (WifiMacHeader *hdr)
{
  return m_txMiddle->GetNextSequenceNumberFor (hdr);
}

uint16_t
QosTxop::PeekNextSequenceNumberFor (WifiMacHeader *hdr)
{
  return m_txMiddle->PeekNextSequenceNumberFor (hdr);
}

Ptr<const Packet>
QosTxop::PeekNextRetransmitPacket (WifiMacHeader &header, uint8_t tid, Time *timestamp)
{
  return m_baManager->PeekNextPacketByTidAndAddress (header, tid, timestamp);
}

void
QosTxop::RemoveRetransmitPacket (uint8_t tid, Mac48Address recipient, uint16_t seqnumber)
{
  m_baManager->RemovePacket (tid, recipient, seqnumber);
}

void
QosTxop::NotifyAccessGranted (void)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (m_accessRequested);
  m_accessRequested = false;
  m_isAccessRequestedForRts = false;
  m_startTxop = Simulator::Now ();
  if (m_currentPacket == 0)
    {
      if (m_queue->IsEmpty () && !m_baManager->HasPackets ())
        {
          NS_LOG_DEBUG ("queue is empty");
          return;
        }
      if (m_baManager->HasBar (m_currentBar))
        {
          SendBlockAckRequest (m_currentBar);
          return;
        }
      /* check if packets need retransmission are stored in BlockAckManager */
      m_currentPacket = m_baManager->GetNextPacket (m_currentHdr, true);
      if (m_currentPacket == 0)
        {
          Ptr<const WifiMacQueueItem> item = m_queue->PeekFirstAvailable (m_qosBlockedDestinations);
          if (item == 0)
            {
              NS_LOG_DEBUG ("no available packets in the queue");
              return;
            }
          m_currentHdr = item->GetHeader ();
          m_currentPacketTimestamp = item->GetTimeStamp ();
          if (m_currentHdr.IsQosData () && !m_currentHdr.GetAddr1 ().IsBroadcast ()
              && m_stationManager->GetQosSupported (m_currentHdr.GetAddr1 ())
              && !m_baManager->ExistsAgreement (m_currentHdr.GetAddr1 (), m_currentHdr.GetQosTid ())
              && SetupBlockAckIfNeeded ())
            {
              return;
            }
          item = m_queue->DequeueFirstAvailable (m_qosBlockedDestinations);
          m_currentPacket = item->GetPacket ();
          m_currentHdr = item->GetHeader ();
          m_currentPacketTimestamp = item->GetTimeStamp ();
          NS_ASSERT (m_currentPacket != 0);

          uint16_t sequence = m_txMiddle->GetNextSequenceNumberFor (&m_currentHdr);
          m_currentHdr.SetSequenceNumber (sequence);
          m_stationManager->UpdateFragmentationThreshold ();
          m_currentHdr.SetFragmentNumber (0);
          m_currentHdr.SetNoMoreFragments ();
          m_currentHdr.SetNoRetry ();
          m_fragmentNumber = 0;
          NS_LOG_DEBUG ("dequeued size=" << m_currentPacket->GetSize () <<
                        ", to=" << m_currentHdr.GetAddr1 () <<
                        ", seq=" << m_currentHdr.GetSequenceControl ());
          if (m_currentHdr.IsQosData () && !m_currentHdr.GetAddr1 ().IsBroadcast ())
            {
              VerifyBlockAck ();
            }
        }
    }
  if (m_currentHdr.GetAddr1 ().IsGroup ())
    {
      m_currentParams.DisableRts ();
      m_currentParams.DisableAck ();
      m_currentParams.DisableNextData ();
      NS_LOG_DEBUG ("tx broadcast");
      m_low->StartTransmission (m_currentPacket, &m_currentHdr, m_currentParams, this);
    }
  else if (m_currentHdr.GetType () == WIFI_MAC_CTL_BACKREQ)
    {
      SendBlockAckRequest (m_currentBar);
    }
  else
    {
      if (m_currentHdr.IsQosData () && m_currentHdr.IsQosBlockAck ())
        {
          m_currentParams.DisableAck ();
        }
      else
        {
          m_currentParams.EnableAck ();
        }
      //With COMPRESSED_BLOCK_ACK fragmentation must be avoided.
      if (((m_currentHdr.IsQosData () && !m_currentHdr.IsQosAmsdu ())
           || (m_currentHdr.IsData () && !m_currentHdr.IsQosData ()))
          && (GetBlockAckThreshold () == 0 || m_blockAckType == BASIC_BLOCK_ACK)
          && NeedFragmentation ())
        {
          m_currentIsFragmented = true;
          m_currentParams.DisableRts ();
          WifiMacHeader hdr;
          Ptr<Packet> fragment = GetFragmentPacket (&hdr);
          if (IsLastFragment ())
            {
              NS_LOG_DEBUG ("fragmenting last fragment size=" << fragment->GetSize ());
              m_currentParams.DisableNextData ();
            }
          else
            {
              NS_LOG_DEBUG ("fragmenting size=" << fragment->GetSize ());
              m_currentParams.EnableNextData (GetNextFragmentSize ());
            }
          m_low->StartTransmission (fragment, &hdr, m_currentParams, this);
        }
      else
        {
          m_currentIsFragmented = false;
          WifiMacHeader peekedHdr;
          Ptr<const WifiMacQueueItem> item;
          if (m_currentHdr.IsQosData ()
              && (item = m_queue->PeekByTidAndAddress (m_currentHdr.GetQosTid (),
                                                       m_currentHdr.GetAddr1 ()))
              && !m_currentHdr.GetAddr1 ().IsBroadcast ()
              && m_msduAggregator != 0 && !m_currentHdr.IsRetry ())
            {
              peekedHdr = item->GetHeader ();
              /* here is performed aggregation */
              Ptr<Packet> currentAggregatedPacket = Create<Packet> ();
              m_msduAggregator->Aggregate (m_currentPacket, currentAggregatedPacket,
                                           MapSrcAddressForAggregation (peekedHdr),
                                           MapDestAddressForAggregation (peekedHdr));
              bool aggregated = false;
              bool isAmsdu = false;
              Ptr<const WifiMacQueueItem> peekedItem = m_queue->PeekByTidAndAddress (m_currentHdr.GetQosTid (),
                                                                                     m_currentHdr.GetAddr1 ());
              while (peekedItem != 0)
                {
                  peekedHdr = peekedItem->GetHeader ();
                  aggregated = m_msduAggregator->Aggregate (peekedItem->GetPacket (), currentAggregatedPacket,
                                                            MapSrcAddressForAggregation (peekedHdr),
                                                            MapDestAddressForAggregation (peekedHdr));
                  if (aggregated)
                    {
                      isAmsdu = true;
                      m_queue->Remove (peekedItem->GetPacket ());
                    }
                  else
                    {
                      break;
                    }
                  peekedItem = m_queue->PeekByTidAndAddress (m_currentHdr.GetQosTid (),
                                                             m_currentHdr.GetAddr1 ());
                }
              if (isAmsdu)
                {
                  m_currentHdr.SetQosAmsdu ();
                  m_currentHdr.SetAddr3 (m_low->GetBssid ());
                  m_currentPacket = currentAggregatedPacket;
                  currentAggregatedPacket = 0;
                  NS_LOG_DEBUG ("tx unicast A-MSDU");
                }
            }
          m_currentParams.DisableNextData ();
          m_low->StartTransmission (m_currentPacket, &m_currentHdr, m_currentParams, this);
          if (!GetAmpduExist (m_currentHdr.GetAddr1 ()))
            {
              CompleteTx ();
            }
        }
    }
}

void QosTxop::NotifyInternalCollision (void)
{
  NS_LOG_FUNCTION (this);
  bool resetDcf = false;
  // If an internal collision is experienced, the frame involved may still
  // be sitting in the queue, and m_currentPacket may still be null.
  Ptr<const Packet> packet;
  WifiMacHeader header;
  if (m_currentPacket == 0)
    {
      if (m_baManager->HasPackets ())
        {
          packet = m_baManager->GetNextPacket (header, false);
        }
      else
        {
          Ptr<const WifiMacQueueItem> item = m_queue->Peek ();
          if (item)
            {
              packet = item->GetPacket ();
              header = item->GetHeader ();
            }
        }
    }
  else
    {
      packet = m_currentPacket;
      header = m_currentHdr;
    }
  if (packet != 0)
    {
      if (m_isAccessRequestedForRts)
        {
          if (!NeedRtsRetransmission (packet, header))
            {
              resetDcf = true;
              m_stationManager->ReportFinalRtsFailed (header.GetAddr1 (), &header);
            }
          else
            {
              m_stationManager->ReportRtsFailed (header.GetAddr1 (), &header);
            }
        }
      else if (header.GetAddr1 () == Mac48Address::GetBroadcast ())
        {
          resetDcf = false;
        }
      else
        {
          if (!NeedDataRetransmission (packet, header))
            {
              resetDcf = true;
              m_stationManager->ReportFinalDataFailed (header.GetAddr1 (), &header, packet->GetSize ());
            }
          else
            {
              m_stationManager->ReportDataFailed (header.GetAddr1 (), &header, packet->GetSize ());
            }
        }
      if (resetDcf)
        {
          NS_LOG_DEBUG ("reset DCF");
          if (!m_txFailedCallback.IsNull ())
            {
              m_txFailedCallback (header);
            }
          //to reset the dcf.
          if (m_currentPacket)
            {
              NS_LOG_DEBUG ("Discarding m_currentPacket");
              m_currentPacket = 0;
            }
          else
            {
              NS_LOG_DEBUG ("Dequeueing and discarding head of queue");
              m_queue->Remove ();
            }
          ResetCw ();
        }
      else
        {
          UpdateFailedCw ();
        }
    }
  m_backoffTrace = m_rng->GetInteger (0, GetCw ());
  StartBackoffNow (m_backoffTrace);
  RestartAccessIfNeeded ();
}

void
QosTxop::NotifyCollision (void)
{
  NS_LOG_FUNCTION (this);
  m_backoffTrace = m_rng->GetInteger (0, GetCw ());
  StartBackoffNow (m_backoffTrace);
  RestartAccessIfNeeded ();
}

void
QosTxop::MissedCts (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("missed cts");
  if (!NeedRtsRetransmission (m_currentPacket, m_currentHdr))
    {
      NS_LOG_DEBUG ("Cts Fail");
      bool resetCurrentPacket = true;
      m_stationManager->ReportFinalRtsFailed (m_currentHdr.GetAddr1 (), &m_currentHdr);
      if (!m_txFailedCallback.IsNull ())
        {
          m_txFailedCallback (m_currentHdr);
        }
      if (GetAmpduExist (m_currentHdr.GetAddr1 ()) || m_currentHdr.IsQosData ())
        {
          uint8_t tid = GetTid (m_currentPacket, m_currentHdr);
          m_low->FlushAggregateQueue (tid);

          if (GetBaAgreementExists (m_currentHdr.GetAddr1 (), tid))
            {
              NS_LOG_DEBUG ("Transmit Block Ack Request");
              CtrlBAckRequestHeader reqHdr;
              reqHdr.SetType (COMPRESSED_BLOCK_ACK);
              reqHdr.SetStartingSequence (m_txMiddle->PeekNextSequenceNumberFor (&m_currentHdr));
              reqHdr.SetTidInfo (tid);
              reqHdr.SetHtImmediateAck (true);
              Ptr<Packet> bar = Create<Packet> ();
              bar->AddHeader (reqHdr);
              Bar request (bar, m_currentHdr.GetAddr1 (), tid, reqHdr.MustSendHtImmediateAck ());
              m_currentBar = request;
              WifiMacHeader hdr;
              hdr.SetType (WIFI_MAC_CTL_BACKREQ);
              hdr.SetAddr1 (request.recipient);
              hdr.SetAddr2 (m_low->GetAddress ());
              hdr.SetAddr3 (m_low->GetBssid ());
              hdr.SetDsNotTo ();
              hdr.SetDsNotFrom ();
              hdr.SetNoRetry ();
              hdr.SetNoMoreFragments ();
              m_currentPacket = request.bar;
              m_currentHdr = hdr;
              resetCurrentPacket = false;
            }
        }
      //to reset the dcf.
      if (resetCurrentPacket == true)
        {
          m_currentPacket = 0;
        }
      ResetCw ();
      m_cwTrace = GetCw ();
    }
  else
    {
      UpdateFailedCw ();
      m_cwTrace = GetCw ();
    }
  m_backoffTrace = m_rng->GetInteger (0, GetCw ());
  StartBackoffNow (m_backoffTrace);
  RestartAccessIfNeeded ();
}

void
QosTxop::GotAck (void)
{
  NS_LOG_FUNCTION (this);
  if (!m_currentIsFragmented
      || !m_currentParams.HasNextPacket ()
      || m_currentHdr.IsQosAmsdu ())
    {
      NS_LOG_DEBUG ("got ack. tx done.");
      if (!m_txOkCallback.IsNull ())
        {
          m_txOkCallback (m_currentHdr);
        }

      if (m_currentHdr.IsAction ())
        {
          WifiActionHeader actionHdr;
          Ptr<Packet> p = m_currentPacket->Copy ();
          p->RemoveHeader (actionHdr);
          if (actionHdr.GetCategory () == WifiActionHeader::BLOCK_ACK
              && actionHdr.GetAction ().blockAck == WifiActionHeader::BLOCK_ACK_DELBA)
            {
              MgtDelBaHeader delBa;
              p->PeekHeader (delBa);
              if (delBa.IsByOriginator ())
                {
                  m_baManager->DestroyAgreement (m_currentHdr.GetAddr1 (), delBa.GetTid ());
                }
              else
                {
                  m_low->DestroyBlockAckAgreement (m_currentHdr.GetAddr1 (), delBa.GetTid ());
                }
            }
        }
      m_currentPacket = 0;
      ResetCw ();
      if (!HasTxop ())
        {
          if (m_currentHdr.IsQosData () && GetTxopLimit ().IsStrictlyPositive ())
            {
              m_txopTrace (m_startTxop, Simulator::Now () - m_startTxop);
            }
          m_cwTrace = GetCw ();
          m_backoffTrace = m_rng->GetInteger (0, GetCw ());
          StartBackoffNow (m_backoffTrace);
          RestartAccessIfNeeded ();
        }
    }
  else
    {
      NS_LOG_DEBUG ("got ack. tx not done, size=" << m_currentPacket->GetSize ());
      if (!HasTxop ())
        {
          if (m_currentHdr.IsQosData () && GetTxopLimit ().IsStrictlyPositive ())
            {
              m_txopTrace (m_startTxop, Simulator::Now () - m_startTxop);
              m_cwTrace = GetCw ();
              m_backoffTrace = m_rng->GetInteger (0, GetCw ());
              StartBackoffNow (m_backoffTrace);
              m_fragmentNumber++;
              RestartAccessIfNeeded ();
            }
        }
    }
}

void
QosTxop::MissedAck (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("missed ack");
  if (!NeedDataRetransmission (m_currentPacket, m_currentHdr))
    {
      NS_LOG_DEBUG ("Ack Fail");
      m_stationManager->ReportFinalDataFailed (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                               m_currentPacket->GetSize ());
      bool resetCurrentPacket = true;
      if (!m_txFailedCallback.IsNull ())
        {
          m_txFailedCallback (m_currentHdr);
        }
      if (GetAmpduExist (m_currentHdr.GetAddr1 ()) || m_currentHdr.IsQosData ())
        {
          uint8_t tid = GetTid (m_currentPacket, m_currentHdr);

          if (GetBaAgreementExists (m_currentHdr.GetAddr1 (), tid))
            {
              //send Block ACK Request in order to shift WinStart at the receiver
              NS_LOG_DEBUG ("Transmit Block Ack Request");
              CtrlBAckRequestHeader reqHdr;
              reqHdr.SetType (COMPRESSED_BLOCK_ACK);
              reqHdr.SetStartingSequence (m_txMiddle->PeekNextSequenceNumberFor (&m_currentHdr));
              reqHdr.SetTidInfo (tid);
              reqHdr.SetHtImmediateAck (true);
              Ptr<Packet> bar = Create<Packet> ();
              bar->AddHeader (reqHdr);
              Bar request (bar, m_currentHdr.GetAddr1 (), tid, reqHdr.MustSendHtImmediateAck ());
              m_currentBar = request;
              WifiMacHeader hdr;
              hdr.SetType (WIFI_MAC_CTL_BACKREQ);
              hdr.SetAddr1 (request.recipient);
              hdr.SetAddr2 (m_low->GetAddress ());
              hdr.SetAddr3 (m_low->GetBssid ());
              hdr.SetDsNotTo ();
              hdr.SetDsNotFrom ();
              hdr.SetNoRetry ();
              hdr.SetNoMoreFragments ();
              m_currentPacket = request.bar;
              m_currentHdr = hdr;
              resetCurrentPacket = false;
            }
        }
      //to reset the dcf.
      if (resetCurrentPacket == true)
        {
          m_currentPacket = 0;
        }
      ResetCw ();
      m_cwTrace = GetCw ();
    }
  else
    {
      NS_LOG_DEBUG ("Retransmit");
      m_currentHdr.SetRetry ();
      UpdateFailedCw ();
      m_cwTrace = GetCw ();
    }
  m_backoffTrace = m_rng->GetInteger (0, GetCw ());
  StartBackoffNow (m_backoffTrace);
  RestartAccessIfNeeded ();
}

void
QosTxop::MissedBlockAck (uint8_t nMpdus)
{
  NS_LOG_FUNCTION (this << +nMpdus);
  uint8_t tid = GetTid (m_currentPacket, m_currentHdr);
  if (GetAmpduExist (m_currentHdr.GetAddr1 ()))
    {
      m_stationManager->ReportAmpduTxStatus (m_currentHdr.GetAddr1 (), tid, 0, nMpdus, 0, 0);
    }
  if (NeedBarRetransmission ())
    {
      if (!GetAmpduExist (m_currentHdr.GetAddr1 ()))
        {
          //should i report this to station addressed by ADDR1?
          NS_LOG_DEBUG ("Retransmit block ack request");
          m_currentHdr.SetRetry ();
        }
      else
        {
          //standard says when losing a BlockAck originator may send a BAR page 139
          NS_LOG_DEBUG ("Transmit Block Ack Request");
          CtrlBAckRequestHeader reqHdr;
          reqHdr.SetType (COMPRESSED_BLOCK_ACK);
          if (m_currentHdr.IsQosData ())
            {
              reqHdr.SetStartingSequence (m_currentHdr.GetSequenceNumber ());
            }
          else if (m_currentHdr.IsBlockAckReq ())
            {
              CtrlBAckRequestHeader baReqHdr;
              m_currentPacket->PeekHeader (baReqHdr);
              reqHdr.SetStartingSequence (baReqHdr.GetStartingSequence ());
            }
          else if (m_currentHdr.IsBlockAck ())
            {
              CtrlBAckResponseHeader baRespHdr;
              m_currentPacket->PeekHeader (baRespHdr);
              reqHdr.SetStartingSequence (m_currentHdr.GetSequenceNumber ());
            }
          reqHdr.SetTidInfo (tid);
          reqHdr.SetHtImmediateAck (true);
          Ptr<Packet> bar = Create<Packet> ();
          bar->AddHeader (reqHdr);
          Bar request (bar, m_currentHdr.GetAddr1 (), tid, reqHdr.MustSendHtImmediateAck ());
          m_currentBar = request;
          WifiMacHeader hdr;
          hdr.SetType (WIFI_MAC_CTL_BACKREQ);
          hdr.SetAddr1 (request.recipient);
          hdr.SetAddr2 (m_low->GetAddress ());
          hdr.SetAddr3 (m_low->GetBssid ());
          hdr.SetDsNotTo ();
          hdr.SetDsNotFrom ();
          hdr.SetNoRetry ();
          hdr.SetNoMoreFragments ();

          m_currentPacket = request.bar;
          m_currentHdr = hdr;
        }
      UpdateFailedCw ();
      m_cwTrace = GetCw ();
    }
  else
    {
      NS_LOG_DEBUG ("Block Ack Request Fail");
      //to reset the dcf.
      m_currentPacket = 0;
      ResetCw ();
      m_cwTrace = GetCw ();
    }
  m_backoffTrace = m_rng->GetInteger (0, GetCw ());
  StartBackoffNow (m_backoffTrace);
  RestartAccessIfNeeded ();
}

Ptr<MsduAggregator>
QosTxop::GetMsduAggregator (void) const
{
  return m_msduAggregator;
}

Ptr<MpduAggregator>
QosTxop::GetMpduAggregator (void) const
{
  return m_mpduAggregator;
}

void
QosTxop::RestartAccessIfNeeded (void)
{
  NS_LOG_FUNCTION (this);
  if ((m_currentPacket != 0
       || !m_queue->IsEmpty () || m_baManager->HasPackets ())
      && !IsAccessRequested ())
    {
      Ptr<const Packet> packet;
      WifiMacHeader hdr;
      if (m_currentPacket != 0)
        {
          packet = m_currentPacket;
          hdr = m_currentHdr;
        }
      else if (m_baManager->HasPackets ())
        {
          packet = m_baManager->GetNextPacket (hdr, false);
        }
      else
        {
          Ptr<const WifiMacQueueItem> item = m_queue->PeekFirstAvailable (m_qosBlockedDestinations);
          if (item)
            {
              packet = item->GetPacket ();
              hdr = item->GetHeader ();
              m_currentPacketTimestamp = item->GetTimeStamp ();
            }
        }
      if (packet != 0)
        {
          m_isAccessRequestedForRts = m_stationManager->NeedRts (hdr.GetAddr1 (), &hdr, packet, m_low->GetDataTxVector (packet, &hdr));
        }
      else
        {
          m_isAccessRequestedForRts = false;
        }
      m_channelAccessManager->RequestAccess (this);
    }
}

void
QosTxop::StartAccessIfNeeded (void)
{
  //NS_LOG_FUNCTION (this);
  if (m_currentPacket == 0
      && (!m_queue->IsEmpty () || m_baManager->HasPackets ())
      && !IsAccessRequested ())
    {
      Ptr<const Packet> packet;
      WifiMacHeader hdr;
      if (m_baManager->HasPackets ())
        {
          packet = m_baManager->GetNextPacket (hdr, false);
        }
      else
        {
          Ptr<const WifiMacQueueItem> item = m_queue->PeekFirstAvailable (m_qosBlockedDestinations);
          if (item)
            {
              packet = item->GetPacket ();
              hdr = item->GetHeader ();
              m_currentPacketTimestamp = item->GetTimeStamp ();
            }
        }
      if (packet != 0)
        {
          m_isAccessRequestedForRts = m_stationManager->NeedRts (hdr.GetAddr1 (), &hdr, packet, m_low->GetDataTxVector (packet, &hdr));
        }
      else
        {
          m_isAccessRequestedForRts = false;
        }
      m_channelAccessManager->RequestAccess (this);
    }
}

bool
QosTxop::NeedBarRetransmission (void)
{
  uint8_t tid = 0;
  uint16_t seqNumber = 0;
  if (m_currentHdr.IsQosData ())
    {
      tid = m_currentHdr.GetQosTid ();
      seqNumber = m_currentHdr.GetSequenceNumber ();
    }
  else if (m_currentHdr.IsBlockAckReq ())
    {
      CtrlBAckRequestHeader baReqHdr;
      m_currentPacket->PeekHeader (baReqHdr);
      tid = baReqHdr.GetTidInfo ();
      seqNumber = baReqHdr.GetStartingSequence ();
    }
  else if (m_currentHdr.IsBlockAck ())
    {
      CtrlBAckResponseHeader baRespHdr;
      m_currentPacket->PeekHeader (baRespHdr);
      tid = baRespHdr.GetTidInfo ();
      seqNumber = m_currentHdr.GetSequenceNumber ();
    }
  return m_baManager->NeedBarRetransmission (tid, seqNumber, m_currentHdr.GetAddr1 ());
}

void
QosTxop::StartNextPacket (void)
{
  NS_LOG_FUNCTION (this);
  Time txopLimit = GetTxopLimit ();
  NS_ASSERT (txopLimit.IsZero () || Simulator::Now () - m_startTxop <= txopLimit);
  WifiMacHeader hdr = m_currentHdr;
  Ptr<const Packet> peekedPacket = m_baManager->GetNextPacket (hdr, true);
  if (peekedPacket == 0)
    {
      Ptr<const WifiMacQueueItem> peekedItem = m_queue->PeekByTidAndAddress (m_currentHdr.GetQosTid (),
                                                                             m_currentHdr.GetAddr1 ());
      if (peekedItem)
        {
          peekedPacket = peekedItem->GetPacket ();
          hdr = peekedItem->GetHeader ();
        }
    }
  if ((m_currentHdr.IsQosBlockAck () && peekedPacket == 0) || m_baManager->HasBar (m_currentBar))
    {
      SendBlockAckRequest (m_currentBar);
      return;
    }
  else if (peekedPacket == 0)
    {
      if (txopLimit.IsStrictlyPositive ())
        {
          NS_ASSERT (Simulator::Now () - m_startTxop <= txopLimit);
          m_txopTrace (m_startTxop, Simulator::Now () - m_startTxop);
        }
      return;
    }
  m_currentParams.DisableNextData ();
  if (m_currentHdr.IsQosData () && m_currentHdr.IsQosBlockAck ())
    {
      m_currentParams.DisableAck ();
    }
  else
    {
      m_currentParams.EnableAck ();
    }
  if (txopLimit >= GetLow ()->CalculateOverallTxTime (peekedPacket, &hdr, m_currentParams))
    {
      NS_LOG_DEBUG ("start next packet");
      Ptr<WifiMacQueueItem> item = m_queue->DequeueByTidAndAddress (m_currentHdr.GetQosTid (),
                                                                    m_currentHdr.GetAddr1 ());
      NS_ASSERT (item != 0);
      m_currentPacket = item->GetPacket ();
      m_currentHdr = item->GetHeader ();
      NS_ASSERT (m_currentPacket != 0);
      uint16_t sequence = m_txMiddle->GetNextSequenceNumberFor (&m_currentHdr);
      m_currentHdr.SetSequenceNumber (sequence);
      m_stationManager->UpdateFragmentationThreshold ();
      m_currentHdr.SetFragmentNumber (0);
      m_currentHdr.SetNoMoreFragments ();
      m_currentHdr.SetNoRetry ();
      m_fragmentNumber = 0;
      VerifyBlockAck ();
      GetLow ()->StartTransmission (m_currentPacket, &m_currentHdr, m_currentParams, this);
      if (!GetAmpduExist (m_currentHdr.GetAddr1 ()))
        {
          CompleteTx ();
        }
    }
  else if (txopLimit.IsStrictlyPositive ())
    {
      m_txopTrace (m_startTxop, Simulator::Now () - m_startTxop);
    }
}

Time
QosTxop::GetTxopRemaining (void) const
{
  Time remainingTxop = GetTxopLimit ();
  remainingTxop -= (Simulator::Now () - m_startTxop);
  if (remainingTxop.IsStrictlyNegative ())
    {
      remainingTxop = Seconds (0);
    }
  NS_LOG_FUNCTION (this << remainingTxop);
  return remainingTxop;
}

bool
QosTxop::HasTxop (void) const
{
  NS_LOG_FUNCTION (this);
  WifiMacHeader hdr;
  if (!m_currentHdr.IsQosData () || GetTxopLimit ().IsZero ())
    {
      return false;
    }

  Ptr<const WifiMacQueueItem> peekedItem = m_queue->PeekByTidAndAddress (m_currentHdr.GetQosTid (),
                                                                         m_currentHdr.GetAddr1 ());
  if (peekedItem == 0)
    {
      return false;
    }

  Ptr<const Packet> peekedPacket = peekedItem->GetPacket ();
  hdr = peekedItem->GetHeader ();
  MacLowTransmissionParameters params = m_currentParams;
  if (m_currentHdr.IsQosData () && m_currentHdr.IsQosBlockAck ())
    {
      params.DisableAck ();
    }
  else
    {
      params.EnableAck ();
    }

  Time duration = GetLow ()->CalculateOverallTxTime (peekedPacket, &hdr, params);
  if (m_currentPacket != 0)
    {
      //take into account current transmission in duration
      duration += GetLow ()->CalculateOverallTxTime (m_currentPacket, &m_currentHdr, params);
    }
  return (GetTxopRemaining () >= duration);
}

void
QosTxop::EndTxNoAck (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("a transmission that did not require an ACK just finished");
  if (m_currentHdr.IsQosData () && m_currentHdr.IsQosBlockAck () && GetTxopLimit ().IsStrictlyPositive ())
    {
      m_txopTrace (m_startTxop, Simulator::Now () - m_startTxop);
    }
  m_currentPacket = 0;
  ResetCw ();
  m_cwTrace = GetCw ();
  m_backoffTrace = m_rng->GetInteger (0, GetCw ());
  StartBackoffNow (m_backoffTrace);
  StartAccessIfNeeded ();
}

bool
QosTxop::NeedFragmentation (void) const
{
  NS_LOG_FUNCTION (this);
  if (m_stationManager->HasVhtSupported ()
      || m_stationManager->HasHeSupported ()
      || GetAmpduExist (m_currentHdr.GetAddr1 ())
      || (m_stationManager->HasHtSupported ()
          && m_currentHdr.IsQosData ()
          && GetBaAgreementExists (m_currentHdr.GetAddr1 (), GetTid (m_currentPacket, m_currentHdr))
          && GetMpduAggregator ()->GetMaxAmpduSize () >= m_currentPacket->GetSize ()))
    {
      //MSDU is not fragmented when it is transmitted using an HT-immediate or
      //HT-delayed Block Ack agreement or when it is carried in an A-MPDU.
      return false;
    }
  bool needTxopFragmentation = false;
  if (GetTxopLimit ().IsStrictlyPositive () && m_currentHdr.IsData ())
    {
      needTxopFragmentation = (GetLow ()->CalculateOverallTxTime (m_currentPacket, &m_currentHdr, m_currentParams) > GetTxopLimit ());
    }
  return (needTxopFragmentation || m_stationManager->NeedFragmentation (m_currentHdr.GetAddr1 (), &m_currentHdr, m_currentPacket));
}

bool
QosTxop::IsTxopFragmentation () const
{
  if (GetTxopLimit ().IsZero ())
    {
      return false;
    }
  if (!m_stationManager->NeedFragmentation (m_currentHdr.GetAddr1 (), &m_currentHdr, m_currentPacket)
      || (GetTxopFragmentSize () < m_stationManager->GetFragmentSize (m_currentHdr.GetAddr1 (), &m_currentHdr,m_currentPacket, 0)))
    {
      return true;
    }
  return false;
}

uint32_t
QosTxop::GetTxopFragmentSize () const
{
  Time txopDuration = GetTxopLimit ();
  if (txopDuration.IsZero ())
    {
      return 0;
    }
  uint32_t maxSize = m_currentPacket->GetSize ();
  uint32_t minSize = 0;
  uint32_t size = 0;
  bool found = false;
  while (!found)
    {
      size = (minSize + ((maxSize - minSize) / 2));
      if (GetLow ()->CalculateOverallTxTime (m_currentPacket, &m_currentHdr, m_currentParams, size) > txopDuration)
        {
          maxSize = size;
        }
      else
        {
          minSize = size;
        }
      if (GetLow ()->CalculateOverallTxTime (m_currentPacket, &m_currentHdr, m_currentParams, size) <= txopDuration
          && GetLow ()->CalculateOverallTxTime (m_currentPacket, &m_currentHdr, m_currentParams, size + 1) > txopDuration)
        {
          found = true;
        }
    }
  NS_ASSERT (size != 0);
  return size;
}

uint32_t
QosTxop::GetNTxopFragment () const
{
  uint32_t fragmentSize = GetTxopFragmentSize ();
  uint32_t nFragments = (m_currentPacket->GetSize () / fragmentSize);
  if ((m_currentPacket->GetSize () % fragmentSize) > 0)
    {
      nFragments++;
    }
  NS_LOG_DEBUG ("GetNTxopFragment returning " << nFragments);
  return nFragments;
}

uint32_t
QosTxop::GetTxopFragmentOffset (uint32_t fragmentNumber) const
{
  if (fragmentNumber == 0)
    {
      return 0;
    }
  uint32_t offset = 0;
  uint32_t fragmentSize = GetTxopFragmentSize ();
  uint32_t nFragments = (m_currentPacket->GetSize () / fragmentSize);
  if ((m_currentPacket->GetSize () % fragmentSize) > 0)
    {
      nFragments++;
    }
  if (fragmentNumber < nFragments)
    {
      offset = (fragmentNumber * fragmentSize);
    }
  else
    {
      NS_ASSERT (false);
    }
  NS_LOG_DEBUG ("GetTxopFragmentOffset returning " << offset);
  return offset;
}

uint32_t
QosTxop::GetNextTxopFragmentSize (uint32_t fragmentNumber) const
{
  NS_LOG_FUNCTION (this << fragmentNumber);
  uint32_t fragmentSize = GetTxopFragmentSize ();
  uint32_t nFragments = GetNTxopFragment ();
  if (fragmentNumber >= nFragments)
    {
      NS_LOG_DEBUG ("GetNextTxopFragmentSize returning 0");
      return 0;
    }
  if (fragmentNumber == nFragments - 1)
    {
      fragmentSize = (m_currentPacket->GetSize () - ((nFragments - 1) * fragmentSize));
    }
  NS_LOG_DEBUG ("GetNextTxopFragmentSize returning " << fragmentSize);
  return fragmentSize;
}

uint32_t
QosTxop::GetFragmentSize (void) const
{
  uint32_t size;
  if (IsTxopFragmentation ())
    {
      size = GetNextTxopFragmentSize (m_fragmentNumber);
    }
  else
    {
      size = m_stationManager->GetFragmentSize (m_currentHdr.GetAddr1 (), &m_currentHdr,m_currentPacket, m_fragmentNumber);
    }
  return size;
}

uint32_t
QosTxop::GetNextFragmentSize (void) const
{
  uint32_t size;
  if (IsTxopFragmentation ())
    {
      size = GetNextTxopFragmentSize (m_fragmentNumber + 1);
    }
  else
    {
      size = m_stationManager->GetFragmentSize (m_currentHdr.GetAddr1 (), &m_currentHdr,m_currentPacket, m_fragmentNumber + 1);
    }
  return size;
}

uint32_t
QosTxop::GetFragmentOffset (void) const
{
  uint32_t offset;
  if (IsTxopFragmentation ())
    {
      offset = GetTxopFragmentOffset (m_fragmentNumber);
    }
  else
    {
      offset = m_stationManager->GetFragmentOffset (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                                    m_currentPacket, m_fragmentNumber);
    }
  return offset;
}

bool
QosTxop::IsLastFragment (void) const
{
  bool isLastFragment;
  if (IsTxopFragmentation ())
    {
      isLastFragment = (m_fragmentNumber == GetNTxopFragment () - 1);
    }
  else
    {
      isLastFragment = m_stationManager->IsLastFragment (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                                         m_currentPacket, m_fragmentNumber);
    }
  return isLastFragment;
}

Ptr<Packet>
QosTxop::GetFragmentPacket (WifiMacHeader *hdr)
{
  NS_LOG_FUNCTION (this << hdr);
  *hdr = m_currentHdr;
  hdr->SetFragmentNumber (m_fragmentNumber);
  uint32_t startOffset = GetFragmentOffset ();
  Ptr<Packet> fragment;
  if (IsLastFragment ())
    {
      hdr->SetNoMoreFragments ();
    }
  else
    {
      hdr->SetMoreFragments ();
    }
  fragment = m_currentPacket->CreateFragment (startOffset,
                                              GetFragmentSize ());
  return fragment;
}

void
QosTxop::SetAccessCategory (AcIndex ac)
{
  NS_LOG_FUNCTION (this << +ac);
  m_ac = ac;
}

Mac48Address
QosTxop::MapSrcAddressForAggregation (const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this << &hdr);
  Mac48Address retval;
  if (GetTypeOfStation () == STA || GetTypeOfStation () == ADHOC_STA)
    {
      retval = hdr.GetAddr2 ();
    }
  else
    {
      retval = hdr.GetAddr3 ();
    }
  return retval;
}

Mac48Address
QosTxop::MapDestAddressForAggregation (const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this << &hdr);
  Mac48Address retval;
  if (GetTypeOfStation () == AP || GetTypeOfStation () == ADHOC_STA)
    {
      retval = hdr.GetAddr1 ();
    }
  else
    {
      retval = hdr.GetAddr3 ();
    }
  return retval;
}

void
QosTxop::SetMsduAggregator (const Ptr<MsduAggregator> aggr)
{
  NS_LOG_FUNCTION (this << aggr);
  m_msduAggregator = aggr;
}

void
QosTxop::SetMpduAggregator (const Ptr<MpduAggregator> aggr)
{
  NS_LOG_FUNCTION (this << aggr);
  m_mpduAggregator = aggr;
}

void
QosTxop::PushFront (Ptr<const Packet> packet, const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this << packet << &hdr);
  WifiMacTrailer fcs;
  m_stationManager->PrepareForQueue (hdr.GetAddr1 (), &hdr, packet);
  m_queue->PushFront (Create<WifiMacQueueItem> (packet, hdr));
  StartAccessIfNeeded ();
}

void
QosTxop::GotAddBaResponse (const MgtAddBaResponseHeader *respHdr, Mac48Address recipient)
{
  NS_LOG_FUNCTION (this << respHdr << recipient);
  NS_LOG_DEBUG ("received ADDBA response from " << recipient);
  uint8_t tid = respHdr->GetTid ();
  if (m_baManager->ExistsAgreementInState (recipient, tid, OriginatorBlockAckAgreement::PENDING))
    {
      if (respHdr->GetStatusCode ().IsSuccess ())
        {
          NS_LOG_DEBUG ("block ack agreement established with " << recipient);
          m_baManager->UpdateAgreement (respHdr, recipient);
        }
      else
        {
          NS_LOG_DEBUG ("discard ADDBA response" << recipient);
          m_baManager->NotifyAgreementUnsuccessful (recipient, tid);
        }
    }
  RestartAccessIfNeeded ();
}

void
QosTxop::GotDelBaFrame (const MgtDelBaHeader *delBaHdr, Mac48Address recipient)
{
  NS_LOG_FUNCTION (this << delBaHdr << recipient);
  NS_LOG_DEBUG ("received DELBA frame from=" << recipient);
  m_baManager->DestroyAgreement (recipient, delBaHdr->GetTid ());
}

void
QosTxop::GotBlockAck (const CtrlBAckResponseHeader *blockAck, Mac48Address recipient, double rxSnr, WifiMode txMode, double dataSnr)
{
  NS_LOG_FUNCTION (this << blockAck << recipient << rxSnr << txMode.GetUniqueName () << dataSnr);
  NS_LOG_DEBUG ("got block ack from=" << recipient);
  m_baManager->NotifyGotBlockAck (blockAck, recipient, rxSnr, txMode, dataSnr);
  if (!m_txOkCallback.IsNull ())
    {
      m_txOkCallback (m_currentHdr);
    }
  m_currentPacket = 0;
  ResetCw ();
  if (!HasTxop ())
    {
      if (m_currentHdr.IsQosData () && GetTxopLimit ().IsStrictlyPositive ())
        {
          m_txopTrace (m_startTxop, Simulator::Now () - m_startTxop);
        }
      m_cwTrace = GetCw ();
      m_backoffTrace = m_rng->GetInteger (0, GetCw ());
      StartBackoffNow (m_backoffTrace);
      RestartAccessIfNeeded ();
    }
}

void
QosTxop::VerifyBlockAck (void)
{
  NS_LOG_FUNCTION (this);
  uint8_t tid = m_currentHdr.GetQosTid ();
  Mac48Address recipient = m_currentHdr.GetAddr1 ();
  uint16_t sequence = m_currentHdr.GetSequenceNumber ();
  if (m_baManager->ExistsAgreementInState (recipient, tid, OriginatorBlockAckAgreement::INACTIVE))
    {
      m_baManager->SwitchToBlockAckIfNeeded (recipient, tid, sequence);
    }
  if ((m_baManager->ExistsAgreementInState (recipient, tid, OriginatorBlockAckAgreement::ESTABLISHED))
      && (GetMpduAggregator () == 0 || GetMpduAggregator ()->GetMaxAmpduSize () == 0))
    {
      m_currentHdr.SetQosAckPolicy (WifiMacHeader::BLOCK_ACK);
    }
}

bool QosTxop::GetAmpduExist (Mac48Address dest) const
{
  NS_LOG_FUNCTION (this << dest);
  auto it = m_aMpduEnabled.find (dest);
  if (it != m_aMpduEnabled.end ())
    {
      return it->second;
    }
  return false;
}

void QosTxop::SetAmpduExist (Mac48Address dest, bool enableAmpdu)
{
  NS_LOG_FUNCTION (this << dest << enableAmpdu);
  m_aMpduEnabled[dest] = enableAmpdu;
}

void
QosTxop::CompleteTx (void)
{
  NS_LOG_FUNCTION (this);
  if (m_currentHdr.IsQosData () && m_currentHdr.IsQosBlockAck ())
    {
      if (!m_currentHdr.IsRetry ())
        {
          m_baManager->StorePacket (m_currentPacket, m_currentHdr, m_currentPacketTimestamp);
        }
      m_baManager->NotifyMpduTransmission (m_currentHdr.GetAddr1 (), m_currentHdr.GetQosTid (),
                                           m_txMiddle->GetNextSeqNumberByTidAndAddress (m_currentHdr.GetQosTid (),
                                                                                        m_currentHdr.GetAddr1 ()), WifiMacHeader::BLOCK_ACK);
    }
}

void
QosTxop::CompleteMpduTx (Ptr<const Packet> packet, WifiMacHeader hdr, Time tstamp)
{
  NS_ASSERT (hdr.IsQosData ());
  m_baManager->StorePacket (packet, hdr, tstamp);
  m_baManager->NotifyMpduTransmission (hdr.GetAddr1 (), hdr.GetQosTid (),
                                       m_txMiddle->GetNextSeqNumberByTidAndAddress (hdr.GetQosTid (),
                                                                                    hdr.GetAddr1 ()), WifiMacHeader::NORMAL_ACK);
}

bool
QosTxop::SetupBlockAckIfNeeded ()
{
  NS_LOG_FUNCTION (this);
  uint8_t tid = m_currentHdr.GetQosTid ();
  Mac48Address recipient = m_currentHdr.GetAddr1 ();
  uint32_t packets = m_queue->GetNPacketsByTidAndAddress (tid, recipient);
  if ((GetBlockAckThreshold () > 0 && packets >= GetBlockAckThreshold ())
      || (m_mpduAggregator != 0 && m_mpduAggregator->GetMaxAmpduSize () > 0 && packets > 1)
      || m_stationManager->HasVhtSupported ()
      || m_stationManager->HasHeSupported ())
    {
      /* Block ack setup */
      uint16_t startingSequence = m_txMiddle->GetNextSeqNumberByTidAndAddress (tid, recipient);
      SendAddBaRequest (recipient, tid, startingSequence, m_blockAckInactivityTimeout, true);
      return true;
    }
  return false;
}

void
QosTxop::SendBlockAckRequest (const Bar &bar)
{
  NS_LOG_FUNCTION (this << &bar);
  WifiMacHeader hdr;
  hdr.SetType (WIFI_MAC_CTL_BACKREQ);
  hdr.SetAddr1 (bar.recipient);
  hdr.SetAddr2 (m_low->GetAddress ());
  hdr.SetAddr3 (m_low->GetBssid ());
  hdr.SetDsNotTo ();
  hdr.SetDsNotFrom ();
  hdr.SetNoRetry ();
  hdr.SetNoMoreFragments ();

  m_currentPacket = bar.bar;
  m_currentHdr = hdr;

  m_currentParams.DisableRts ();
  m_currentParams.DisableNextData ();
  if (bar.immediate)
    {
      if (m_blockAckType == BASIC_BLOCK_ACK)
        {
          m_currentParams.EnableBasicBlockAck ();
        }
      else if (m_blockAckType == COMPRESSED_BLOCK_ACK)
        {
          m_currentParams.EnableCompressedBlockAck ();
        }
      else if (m_blockAckType == MULTI_TID_BLOCK_ACK)
        {
          NS_FATAL_ERROR ("Multi-tid block ack is not supported");
        }
    }
  else
    {
      //Delayed block ack
      m_currentParams.EnableAck ();
    }
  m_low->StartTransmission (m_currentPacket, &m_currentHdr, m_currentParams, this);
}

void
QosTxop::CompleteConfig (void)
{
  NS_LOG_FUNCTION (this);
  m_baManager->SetTxMiddle (m_txMiddle);
  m_low->RegisterEdcaForAc (m_ac, this);
  m_baManager->SetBlockAckInactivityCallback (MakeCallback (&QosTxop::SendDelbaFrame, this));
}

void
QosTxop::SetBlockAckThreshold (uint8_t threshold)
{
  NS_LOG_FUNCTION (this << +threshold);
  m_blockAckThreshold = threshold;
  m_baManager->SetBlockAckThreshold (threshold);
}

void
QosTxop::SetBlockAckInactivityTimeout (uint16_t timeout)
{
  NS_LOG_FUNCTION (this << timeout);
  m_blockAckInactivityTimeout = timeout;
}

uint8_t
QosTxop::GetBlockAckThreshold (void) const
{
  NS_LOG_FUNCTION (this);
  return m_blockAckThreshold;
}

void
QosTxop::SendAddBaRequest (Mac48Address dest, uint8_t tid, uint16_t startSeq,
                           uint16_t timeout, bool immediateBAck)
{
  NS_LOG_FUNCTION (this << dest << +tid << startSeq << timeout << immediateBAck);
  NS_LOG_DEBUG ("sent ADDBA request to " << dest);
  WifiMacHeader hdr;
  hdr.SetType (WIFI_MAC_MGT_ACTION);
  hdr.SetAddr1 (dest);
  hdr.SetAddr2 (m_low->GetAddress ());
  hdr.SetAddr3 (m_low->GetAddress ());
  hdr.SetDsNotTo ();
  hdr.SetDsNotFrom ();

  WifiActionHeader actionHdr;
  WifiActionHeader::ActionValue action;
  action.blockAck = WifiActionHeader::BLOCK_ACK_ADDBA_REQUEST;
  actionHdr.SetAction (WifiActionHeader::BLOCK_ACK, action);

  Ptr<Packet> packet = Create<Packet> ();
  /*Setting ADDBARequest header*/
  MgtAddBaRequestHeader reqHdr;
  reqHdr.SetAmsduSupport (true);
  if (immediateBAck)
    {
      reqHdr.SetImmediateBlockAck ();
    }
  else
    {
      reqHdr.SetDelayedBlockAck ();
    }
  reqHdr.SetTid (tid);
  /* For now we don't use buffer size field in the ADDBA request frame. The recipient
   * will choose how many packets it can receive under block ack.
   */
  reqHdr.SetBufferSize (0);
  reqHdr.SetTimeout (timeout);
  reqHdr.SetStartingSequence (startSeq);

  m_baManager->CreateAgreement (&reqHdr, dest);

  packet->AddHeader (reqHdr);
  packet->AddHeader (actionHdr);

  m_currentPacket = packet;
  m_currentHdr = hdr;

  uint16_t sequence = m_txMiddle->GetNextSequenceNumberFor (&m_currentHdr);
  m_currentHdr.SetSequenceNumber (sequence);
  m_stationManager->UpdateFragmentationThreshold ();
  m_currentHdr.SetFragmentNumber (0);
  m_currentHdr.SetNoMoreFragments ();
  m_currentHdr.SetNoRetry ();

  m_currentParams.EnableAck ();
  m_currentParams.DisableRts ();
  m_currentParams.DisableNextData ();

  m_low->StartTransmission (m_currentPacket, &m_currentHdr, m_currentParams, this);
}

void
QosTxop::SendDelbaFrame (Mac48Address addr, uint8_t tid, bool byOriginator)
{
  NS_LOG_FUNCTION (this << addr << +tid << byOriginator);
  WifiMacHeader hdr;
  hdr.SetType (WIFI_MAC_MGT_ACTION);
  hdr.SetAddr1 (addr);
  hdr.SetAddr2 (m_low->GetAddress ());
  hdr.SetAddr3 (m_low->GetAddress ());
  hdr.SetDsNotTo ();
  hdr.SetDsNotFrom ();

  MgtDelBaHeader delbaHdr;
  delbaHdr.SetTid (tid);
  if (byOriginator)
    {
      delbaHdr.SetByOriginator ();
      m_baManager->DestroyAgreement (addr, tid);
    }
  else
    {
      delbaHdr.SetByRecipient ();
      m_low->DestroyBlockAckAgreement (addr, tid);
    }

  WifiActionHeader actionHdr;
  WifiActionHeader::ActionValue action;
  action.blockAck = WifiActionHeader::BLOCK_ACK_DELBA;
  actionHdr.SetAction (WifiActionHeader::BLOCK_ACK, action);

  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (delbaHdr);
  packet->AddHeader (actionHdr);

  PushFront (packet, hdr);
}

void
QosTxop::DoInitialize ()
{
  NS_LOG_FUNCTION (this);
  ResetCw ();
  m_cwTrace = GetCw ();
  m_backoffTrace = m_rng->GetInteger (0, GetCw ());
  StartBackoffNow (m_backoffTrace);
}

void
QosTxop::BaTxOk (const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this << hdr);
  if (!m_txOkCallback.IsNull ())
    {
      m_txOkCallback (m_currentHdr);
    }
}

void
QosTxop::BaTxFailed (const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this << hdr);
  if (!m_txFailedCallback.IsNull ())
    {
      m_txFailedCallback (m_currentHdr);
    }
}

bool
QosTxop::IsQosTxop () const
{
  return true;
}

} //namespace ns3

