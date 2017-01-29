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
#include "ns3/pointer.h"
#include "edca-txop-n.h"
#include "mac-low.h"
#include "dcf-manager.h"
#include "dcf-state.h"
#include "mac-tx-middle.h"
#include "wifi-mac-trailer.h"
#include "random-stream.h"
#include "wifi-mac-queue.h"
#include "qos-blocked-destinations.h"
#include "ns3/simulator.h"

#undef NS_LOG_APPEND_CONTEXT
#define NS_LOG_APPEND_CONTEXT if (m_low != 0) { std::clog << "[mac=" << m_low->GetAddress () << "] "; }

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("EdcaTxopN");

NS_OBJECT_ENSURE_REGISTERED (EdcaTxopN);

TypeId
EdcaTxopN::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EdcaTxopN")
    .SetParent<ns3::DcaTxop> ()
    .SetGroupName ("Wifi")
    .AddConstructor<EdcaTxopN> ()
    .AddTraceSource ("BackoffTrace",
                     "Trace source for backoff values",
                     MakeTraceSourceAccessor (&EdcaTxopN::m_backoffTrace),
                     "ns3::TracedValue::Uint32Callback")
    .AddTraceSource ("CwTrace",
                     "Trace source for contention window values",
                     MakeTraceSourceAccessor (&EdcaTxopN::m_cwTrace),
                     "ns3::TracedValue::Uint32Callback")
    .AddTraceSource ("TxopTrace",
                     "Trace source for txop start and duration times",
                     MakeTraceSourceAccessor (&EdcaTxopN::m_txopTrace),
                     "ns3::TracedValueCallback::Time")
  ;
  return tid;
}

EdcaTxopN::EdcaTxopN ()
  : m_msduAggregator (0),
    m_mpduAggregator (0),
    m_typeOfStation (STA),
    m_blockAckType (COMPRESSED_BLOCK_ACK),
    m_startTxop (Seconds (0)),
    m_isAccessRequestedForRts (false)
{
  NS_LOG_FUNCTION (this);
  m_qosBlockedDestinations = new QosBlockedDestinations ();
  m_baManager = new BlockAckManager ();
  m_baManager->SetQueue (m_queue);
  m_baManager->SetBlockAckType (m_blockAckType);
  m_baManager->SetBlockDestinationCallback (MakeCallback (&QosBlockedDestinations::Block, m_qosBlockedDestinations));
  m_baManager->SetUnblockDestinationCallback (MakeCallback (&QosBlockedDestinations::Unblock, m_qosBlockedDestinations));
  m_baManager->SetMaxPacketDelay (m_queue->GetMaxDelay ());
  m_baManager->SetTxOkCallback (MakeCallback (&EdcaTxopN::BaTxOk, this));
  m_baManager->SetTxFailedCallback (MakeCallback (&EdcaTxopN::BaTxFailed, this));
}

EdcaTxopN::~EdcaTxopN ()
{
  NS_LOG_FUNCTION (this);
}

void
EdcaTxopN::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  delete m_qosBlockedDestinations;
  delete m_baManager;
  m_qosBlockedDestinations = 0;
  m_baManager = 0;
  m_msduAggregator = 0;
  m_mpduAggregator = 0;
  DcaTxop::DoDispose ();
}

bool
EdcaTxopN::GetBaAgreementExists (Mac48Address address, uint8_t tid) const
{
  return m_baManager->ExistsAgreement (address, tid);
}

uint32_t
EdcaTxopN::GetNOutstandingPacketsInBa (Mac48Address address, uint8_t tid) const
{
  return m_baManager->GetNBufferedPackets (address, tid);
}

uint32_t
EdcaTxopN::GetNRetryNeededPackets (Mac48Address recipient, uint8_t tid) const
{
  return m_baManager->GetNRetryNeededPackets (recipient, tid);
}

void
EdcaTxopN::CompleteAmpduTransfer (Mac48Address recipient, uint8_t tid)
{
  m_baManager->CompleteAmpduExchange (recipient, tid);
}

void
EdcaTxopN::SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> remoteManager)
{
  DcaTxop::SetWifiRemoteStationManager (remoteManager);
  NS_LOG_FUNCTION (this << remoteManager);
  m_baManager->SetWifiRemoteStationManager (m_stationManager);
}

void
EdcaTxopN::SetTypeOfStation (TypeOfStation type)
{
  NS_LOG_FUNCTION (this << (uint16_t)type);
  m_typeOfStation = type;
}

TypeOfStation
EdcaTxopN::GetTypeOfStation (void) const
{
  NS_LOG_FUNCTION (this);
  return m_typeOfStation;
}

bool
EdcaTxopN::NeedsAccess (void) const
{
  NS_LOG_FUNCTION (this);
  return !m_queue->IsEmpty () || m_currentPacket != 0 || m_baManager->HasPackets ();
}

uint16_t EdcaTxopN::GetNextSequenceNumberFor (WifiMacHeader *hdr)
{
  return m_txMiddle->GetNextSequenceNumberFor (hdr);
}

uint16_t EdcaTxopN::PeekNextSequenceNumberFor (WifiMacHeader *hdr)
{
  return m_txMiddle->PeekNextSequenceNumberFor (hdr);
}

Ptr<const Packet>
EdcaTxopN::PeekNextRetransmitPacket (WifiMacHeader &header,Mac48Address recipient, uint8_t tid, Time *timestamp)
{
  return m_baManager->PeekNextPacketByTidAndAddress (header,recipient,tid, timestamp);
}

void
EdcaTxopN::RemoveRetransmitPacket (uint8_t tid, Mac48Address recipient, uint16_t seqnumber)
{
  m_baManager->RemovePacket (tid, recipient, seqnumber);
}

void
EdcaTxopN::NotifyAccessGranted (void)
{
  NS_LOG_FUNCTION (this);
  m_isAccessRequestedForRts = false;
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
      m_currentPacket = m_baManager->GetNextPacket (m_currentHdr);
      if (m_currentPacket == 0)
        {
          if (m_queue->PeekFirstAvailable (&m_currentHdr, m_currentPacketTimestamp, m_qosBlockedDestinations) == 0)
            {
              NS_LOG_DEBUG ("no available packets in the queue");
              return;
            }
          if (m_currentHdr.IsQosData () && !m_currentHdr.GetAddr1 ().IsBroadcast ()
              && m_stationManager->GetQosSupported (m_currentHdr.GetAddr1 ())
              && !m_baManager->ExistsAgreement (m_currentHdr.GetAddr1 (), m_currentHdr.GetQosTid ())
              && SetupBlockAckIfNeeded ())
            {
              return;
            }
          m_currentPacket = m_queue->DequeueFirstAvailable (&m_currentHdr, m_currentPacketTimestamp, m_qosBlockedDestinations);
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
  MacLowTransmissionParameters params;
  params.DisableOverrideDurationId ();
  if (m_currentHdr.GetAddr1 ().IsGroup ())
    {
      params.DisableRts ();
      params.DisableAck ();
      params.DisableNextData ();
      m_low->StartTransmission (m_currentPacket, &m_currentHdr, params, this);
      NS_LOG_DEBUG ("tx broadcast");
    }
  else if (m_currentHdr.GetType () == WIFI_MAC_CTL_BACKREQ)
    {
      SendBlockAckRequest (m_currentBar);
    }
  else
    {
      if (m_currentHdr.IsQosData () && m_currentHdr.IsQosBlockAck ())
        {
          params.DisableAck ();
        }
      else
        {
          params.EnableAck ();
        }
      if (((m_currentHdr.IsQosData () && !m_currentHdr.IsQosAmsdu ())
           || (m_currentHdr.IsData () && !m_currentHdr.IsQosData ()))
          && (m_blockAckThreshold == 0 || m_blockAckType == BASIC_BLOCK_ACK)
          && NeedFragmentation ())
        {
          //With COMPRESSED_BLOCK_ACK fragmentation must be avoided.
          params.DisableRts ();
          WifiMacHeader hdr;
          Ptr<Packet> fragment = GetFragmentPacket (&hdr);
          if (IsLastFragment ())
            {
              NS_LOG_DEBUG ("fragmenting last fragment size=" << fragment->GetSize ());
              params.DisableNextData ();
            }
          else
            {
              NS_LOG_DEBUG ("fragmenting size=" << fragment->GetSize ());
              params.EnableNextData (GetNextFragmentSize ());
            }
          m_low->StartTransmission (fragment, &hdr, params, this);
        }
      else
        {
          WifiMacHeader peekedHdr;
          Time tstamp;
          if (m_currentHdr.IsQosData ()
              && m_queue->PeekByTidAndAddress (&peekedHdr, m_currentHdr.GetQosTid (),
                                               WifiMacHeader::ADDR1, m_currentHdr.GetAddr1 (), &tstamp)
              && !m_currentHdr.GetAddr1 ().IsBroadcast ()
              && m_msduAggregator != 0 && !m_currentHdr.IsRetry ())
            {
              /* here is performed aggregation */
              Ptr<Packet> currentAggregatedPacket = Create<Packet> ();
              m_msduAggregator->Aggregate (m_currentPacket, currentAggregatedPacket,
                                           MapSrcAddressForAggregation (peekedHdr),
                                           MapDestAddressForAggregation (peekedHdr));
              bool aggregated = false;
              bool isAmsdu = false;
              Ptr<const Packet> peekedPacket = m_queue->PeekByTidAndAddress (&peekedHdr, m_currentHdr.GetQosTid (),
                                                                             WifiMacHeader::ADDR1,
                                                                             m_currentHdr.GetAddr1 (), &tstamp);
              while (peekedPacket != 0)
                {
                  aggregated = m_msduAggregator->Aggregate (peekedPacket, currentAggregatedPacket,
                                                            MapSrcAddressForAggregation (peekedHdr),
                                                            MapDestAddressForAggregation (peekedHdr));
                  if (aggregated)
                    {
                      isAmsdu = true;
                      m_queue->Remove (peekedPacket);
                    }
                  else
                    {
                      break;
                    }
                  peekedPacket = m_queue->PeekByTidAndAddress (&peekedHdr, m_currentHdr.GetQosTid (),
                                                               WifiMacHeader::ADDR1, m_currentHdr.GetAddr1 (), &tstamp);
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
          params.DisableNextData ();
          m_startTxop = Simulator::Now ();
          m_low->StartTransmission (m_currentPacket, &m_currentHdr, params, this);
          if (!GetAmpduExist (m_currentHdr.GetAddr1 ()))
            {
              CompleteTx ();
            }
        }
    }
}

void EdcaTxopN::NotifyInternalCollision (void)
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
          packet = m_baManager->PeekNextPacket (header);
        }
      else
        {
          packet = m_queue->Peek (&header);
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
              m_stationManager->ReportFinalDataFailed (header.GetAddr1 (), &header);
            }
          else
            {
              m_stationManager->ReportDataFailed (header.GetAddr1 (), &header);
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
              packet = m_queue->Peek (&header);
            }
          m_dcf->ResetCw ();
        }
      else
        {
          m_dcf->UpdateFailedCw ();
        }
    }
  m_backoffTrace = m_rng->GetNext (0, m_dcf->GetCw ());
  m_dcf->StartBackoffNow (m_backoffTrace);
  RestartAccessIfNeeded ();
}

void
EdcaTxopN::NotifyCollision (void)
{
  NS_LOG_FUNCTION (this);
  m_backoffTrace = m_rng->GetNext (0, m_dcf->GetCw ());
  m_dcf->StartBackoffNow (m_backoffTrace);
  RestartAccessIfNeeded ();
}

void
EdcaTxopN::MissedCts (void)
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
      m_dcf->ResetCw ();
      m_cwTrace = m_dcf->GetCw ();
    }
  else
    {
      m_dcf->UpdateFailedCw ();
      m_cwTrace = m_dcf->GetCw ();
    }
  m_backoffTrace = m_rng->GetNext (0, m_dcf->GetCw ());
  m_dcf->StartBackoffNow (m_backoffTrace);
  RestartAccessIfNeeded ();
}

void
EdcaTxopN::GotAck (void)
{
  NS_LOG_FUNCTION (this);
  if (!NeedFragmentation ()
      || IsLastFragment ()
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
                  m_baManager->TearDownBlockAck (m_currentHdr.GetAddr1 (), delBa.GetTid ());
                }
              else
                {
                  m_low->DestroyBlockAckAgreement (m_currentHdr.GetAddr1 (), delBa.GetTid ());
                }
            }
        }
      m_currentPacket = 0;
      m_dcf->ResetCw ();
      if (!HasTxop ())
        {
          if (m_currentHdr.IsQosData () && GetTxopLimit () > NanoSeconds (0))
            {
              m_txopTrace (m_startTxop, Simulator::Now () - m_startTxop);
            }
          m_cwTrace = m_dcf->GetCw ();
          m_backoffTrace = m_rng->GetNext (0, m_dcf->GetCw ());
          m_dcf->StartBackoffNow (m_backoffTrace);
          RestartAccessIfNeeded ();
        }
    }
  else
    {
      NS_LOG_DEBUG ("got ack. tx not done, size=" << m_currentPacket->GetSize ());
    }
}

void
EdcaTxopN::MissedAck (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("missed ack");
  if (!NeedDataRetransmission (m_currentPacket, m_currentHdr))
    {
      NS_LOG_DEBUG ("Ack Fail");
      m_stationManager->ReportFinalDataFailed (m_currentHdr.GetAddr1 (), &m_currentHdr);
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
      m_dcf->ResetCw ();
      m_cwTrace = m_dcf->GetCw ();
    }
  else
    {
      NS_LOG_DEBUG ("Retransmit");
      m_currentHdr.SetRetry ();
      m_dcf->UpdateFailedCw ();
      m_cwTrace = m_dcf->GetCw ();
    }
  m_backoffTrace = m_rng->GetNext (0, m_dcf->GetCw ());
  m_dcf->StartBackoffNow (m_backoffTrace);
  RestartAccessIfNeeded ();
}

void
EdcaTxopN::MissedBlockAck (uint8_t nMpdus)
{
  NS_LOG_FUNCTION (this << (uint16_t)nMpdus);
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
          //standard says when loosing a BlockAck originator may send a BAR page 139
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
      m_dcf->UpdateFailedCw ();
      m_cwTrace = m_dcf->GetCw ();
    }
  else
    {
      NS_LOG_DEBUG ("Block Ack Request Fail");
      //to reset the dcf.
      m_currentPacket = 0;
      m_dcf->ResetCw ();
      m_cwTrace = m_dcf->GetCw ();
    }
  m_backoffTrace = m_rng->GetNext (0, m_dcf->GetCw ());
  m_dcf->StartBackoffNow (m_backoffTrace);
  RestartAccessIfNeeded ();
}

Ptr<MsduAggregator>
EdcaTxopN::GetMsduAggregator (void) const
{
  return m_msduAggregator;
}

Ptr<MpduAggregator>
EdcaTxopN::GetMpduAggregator (void) const
{
  return m_mpduAggregator;
}

void
EdcaTxopN::RestartAccessIfNeeded (void)
{
  NS_LOG_FUNCTION (this);
  if ((m_currentPacket != 0
       || !m_queue->IsEmpty () || m_baManager->HasPackets ())
      && !m_dcf->IsAccessRequested ())
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
          packet = m_baManager->PeekNextPacket (hdr);
        }
      else if (m_queue->PeekFirstAvailable (&hdr, m_currentPacketTimestamp, m_qosBlockedDestinations) != 0)
        {
          packet = m_queue->PeekFirstAvailable (&hdr, m_currentPacketTimestamp, m_qosBlockedDestinations);
        }
      if (packet != 0)
        {
          m_isAccessRequestedForRts = m_stationManager->NeedRts (hdr.GetAddr1 (), &hdr, packet, m_low->GetDataTxVector (packet, &hdr));
        }
      else
        {
          m_isAccessRequestedForRts = false;
        }
      m_manager->RequestAccess (m_dcf);
    }
}

void
EdcaTxopN::StartAccessIfNeeded (void)
{
  NS_LOG_FUNCTION (this);
  if (m_currentPacket == 0
      && (!m_queue->IsEmpty () || m_baManager->HasPackets ())
      && !m_dcf->IsAccessRequested ())
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
          packet = m_baManager->PeekNextPacket (hdr);
        }
      else if (m_queue->PeekFirstAvailable (&hdr, m_currentPacketTimestamp, m_qosBlockedDestinations) != 0)
        {
          packet = m_queue->PeekFirstAvailable (&hdr, m_currentPacketTimestamp, m_qosBlockedDestinations);
        }
      if (packet != 0)
        {
          m_isAccessRequestedForRts = m_stationManager->NeedRts (hdr.GetAddr1 (), &hdr, packet, m_low->GetDataTxVector (packet, &hdr));
        }
      else
        {
          m_isAccessRequestedForRts = false;
        }
      m_manager->RequestAccess (m_dcf);
    }
}

bool
EdcaTxopN::NeedBarRetransmission (void)
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
EdcaTxopN::StartNextPacket (void)
{
  NS_LOG_FUNCTION (this);
  Time txopLimit = GetTxopLimit ();
  NS_ASSERT (txopLimit == NanoSeconds (0) || Simulator::Now () - m_startTxop <= txopLimit);
  WifiMacHeader hdr = m_currentHdr;
  Time tstamp;
  Ptr<const Packet> peekedPacket = m_baManager->GetNextPacket (hdr);
  if (peekedPacket == 0)
    {
      peekedPacket = m_queue->PeekByTidAndAddress (&hdr,
                                                   m_currentHdr.GetQosTid (),
                                                   WifiMacHeader::ADDR1,
                                                   m_currentHdr.GetAddr1 (),
                                                   &tstamp);
    }
  if ((m_currentHdr.IsQosBlockAck () && peekedPacket == 0) || m_baManager->HasBar (m_currentBar))
    {
      SendBlockAckRequest (m_currentBar);
      return;
    }
  else if (peekedPacket == 0)
    {
      if (txopLimit > NanoSeconds (0))
        {
          NS_ASSERT (Simulator::Now () - m_startTxop <= txopLimit);
          m_txopTrace (m_startTxop, Simulator::Now () - m_startTxop);
        }
      return;
    }
  MacLowTransmissionParameters params;
  params.DisableOverrideDurationId ();
  params.DisableNextData ();
  if (m_currentHdr.IsQosData () && m_currentHdr.IsQosBlockAck ())
    {
      params.DisableAck ();
    }
  else
    {
      params.EnableAck ();
    }
  if (txopLimit >= GetLow ()->CalculateOverallTxTime (peekedPacket, &hdr, params))
    {
      NS_LOG_DEBUG ("start next packet");
      m_currentPacket = m_queue->DequeueByTidAndAddress (&hdr,
                                                         m_currentHdr.GetQosTid (),
                                                         WifiMacHeader::ADDR1,
                                                         m_currentHdr.GetAddr1 ());
      m_currentHdr = hdr;
      NS_ASSERT (m_currentPacket != 0);
      uint16_t sequence = m_txMiddle->GetNextSequenceNumberFor (&m_currentHdr);
      m_currentHdr.SetSequenceNumber (sequence);
      m_stationManager->UpdateFragmentationThreshold ();
      m_currentHdr.SetFragmentNumber (0);
      m_currentHdr.SetNoMoreFragments ();
      m_currentHdr.SetNoRetry ();
      m_fragmentNumber = 0;
      VerifyBlockAck ();
      GetLow ()->StartTransmission (m_currentPacket, &m_currentHdr, params, this);
      if (!GetAmpduExist (m_currentHdr.GetAddr1 ()))
        {
          CompleteTx ();
        }
    }
  else if (txopLimit > NanoSeconds (0))
    {
      m_txopTrace (m_startTxop, Simulator::Now () - m_startTxop);
    }
}

Time
EdcaTxopN::GetTxopRemaining (void) const
{
  Time remainingTxop = GetTxopLimit ();
  remainingTxop -= (Simulator::Now () - m_startTxop);
  if (remainingTxop < MicroSeconds (0))
    {
      remainingTxop = MicroSeconds (0);
    }
  NS_LOG_FUNCTION (this << remainingTxop);
  return remainingTxop;
}

bool
EdcaTxopN::HasTxop (void) const
{
  NS_LOG_FUNCTION (this);
  WifiMacHeader hdr;
  Time tstamp;
  if (!m_currentHdr.IsQosData ())
    {
      return false;
    }

  Ptr<const Packet> peekedPacket = m_queue->PeekByTidAndAddress (&hdr,
                                                                 m_currentHdr.GetQosTid (),
                                                                 WifiMacHeader::ADDR1,
                                                                 m_currentHdr.GetAddr1 (),
                                                                 &tstamp);
  if (peekedPacket == 0)
    {
      return false;
    }

  MacLowTransmissionParameters params;
  params.DisableOverrideDurationId ();
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
EdcaTxopN::EndTxNoAck (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("a transmission that did not require an ACK just finished");
  if (m_currentHdr.IsQosData ()  && m_currentHdr.IsQosBlockAck () && GetTxopLimit () > NanoSeconds (0))
    {
      m_txopTrace (m_startTxop, Simulator::Now () - m_startTxop);
    }
  m_currentPacket = 0;
  m_dcf->ResetCw ();
  m_cwTrace = m_dcf->GetCw ();
  m_backoffTrace = m_rng->GetNext (0, m_dcf->GetCw ());
  m_dcf->StartBackoffNow (m_backoffTrace);
  StartAccessIfNeeded ();
}

bool
EdcaTxopN::NeedFragmentation (void) const
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
  return m_stationManager->NeedFragmentation (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                              m_currentPacket);
}

Ptr<Packet>
EdcaTxopN::GetFragmentPacket (WifiMacHeader *hdr)
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
EdcaTxopN::SetAccessCategory (AcIndex ac)
{
  NS_LOG_FUNCTION (this << (uint16_t)ac);
  m_ac = ac;
}

Mac48Address
EdcaTxopN::MapSrcAddressForAggregation (const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this << &hdr);
  Mac48Address retval;
  if (m_typeOfStation == STA || m_typeOfStation == ADHOC_STA)
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
EdcaTxopN::MapDestAddressForAggregation (const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this << &hdr);
  Mac48Address retval;
  if (m_typeOfStation == AP || m_typeOfStation == ADHOC_STA)
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
EdcaTxopN::SetMsduAggregator (Ptr<MsduAggregator> aggr)
{
  NS_LOG_FUNCTION (this << aggr);
  m_msduAggregator = aggr;
}

void
EdcaTxopN::SetMpduAggregator (Ptr<MpduAggregator> aggr)
{
  NS_LOG_FUNCTION (this << aggr);
  m_mpduAggregator = aggr;
}

void
EdcaTxopN::PushFront (Ptr<const Packet> packet, const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this << packet << &hdr);
  WifiMacTrailer fcs;
  m_stationManager->PrepareForQueue (hdr.GetAddr1 (), &hdr, packet);
  m_queue->PushFront (packet, hdr);
  StartAccessIfNeeded ();
}

void
EdcaTxopN::GotAddBaResponse (const MgtAddBaResponseHeader *respHdr, Mac48Address recipient)
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
EdcaTxopN::GotDelBaFrame (const MgtDelBaHeader *delBaHdr, Mac48Address recipient)
{
  NS_LOG_FUNCTION (this << delBaHdr << recipient);
  NS_LOG_DEBUG ("received DELBA frame from=" << recipient);
  m_baManager->TearDownBlockAck (recipient, delBaHdr->GetTid ());
}

void
EdcaTxopN::GotBlockAck (const CtrlBAckResponseHeader *blockAck, Mac48Address recipient, double rxSnr, WifiMode txMode, double dataSnr)
{
  NS_LOG_FUNCTION (this << blockAck << recipient << rxSnr << txMode.GetUniqueName () << dataSnr);
  NS_LOG_DEBUG ("got block ack from=" << recipient);
  m_baManager->NotifyGotBlockAck (blockAck, recipient, rxSnr, txMode, dataSnr);
  if (!m_txOkCallback.IsNull ())
    {
      m_txOkCallback (m_currentHdr);
    }
  m_currentPacket = 0;
  m_dcf->ResetCw ();
  if (!HasTxop ())
    {
      if (m_currentHdr.IsQosData () && GetTxopLimit () > NanoSeconds (0))
        {
          m_txopTrace (m_startTxop, Simulator::Now () - m_startTxop);
        }
      m_cwTrace = m_dcf->GetCw ();
      m_backoffTrace = m_rng->GetNext (0, m_dcf->GetCw ());
      m_dcf->StartBackoffNow (m_backoffTrace);
      RestartAccessIfNeeded ();
    }
}

void
EdcaTxopN::VerifyBlockAck (void)
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

bool EdcaTxopN::GetAmpduExist (Mac48Address dest) const
{
  NS_LOG_FUNCTION (this << dest);
  if (m_aMpduEnabled.find (dest) != m_aMpduEnabled.end ())
    {
      return m_aMpduEnabled.find (dest)->second;
    }
  return false;
}

void EdcaTxopN::SetAmpduExist (Mac48Address dest, bool enableAmpdu)
{
  NS_LOG_FUNCTION (this << dest << enableAmpdu);
  if (m_aMpduEnabled.find (dest) != m_aMpduEnabled.end () && m_aMpduEnabled.find (dest)->second != enableAmpdu)
    {
      m_aMpduEnabled.erase (m_aMpduEnabled.find (dest));
    }
  if (m_aMpduEnabled.find (dest) == m_aMpduEnabled.end ())
    {
      m_aMpduEnabled.insert (std::make_pair (dest, enableAmpdu));
    }
}

void
EdcaTxopN::CompleteTx (void)
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
EdcaTxopN::CompleteMpduTx (Ptr<const Packet> packet, WifiMacHeader hdr, Time tstamp)
{
  NS_ASSERT (hdr.IsQosData ());
  m_baManager->StorePacket (packet, hdr, tstamp);
  m_baManager->NotifyMpduTransmission (hdr.GetAddr1 (), hdr.GetQosTid (),
                                       m_txMiddle->GetNextSeqNumberByTidAndAddress (hdr.GetQosTid (),
                                                                                    hdr.GetAddr1 ()), WifiMacHeader::NORMAL_ACK);
}

bool
EdcaTxopN::SetupBlockAckIfNeeded ()
{
  NS_LOG_FUNCTION (this);
  uint8_t tid = m_currentHdr.GetQosTid ();
  Mac48Address recipient = m_currentHdr.GetAddr1 ();
  uint32_t packets = m_queue->GetNPacketsByTidAndAddress (tid, WifiMacHeader::ADDR1, recipient);
  if ((m_blockAckThreshold > 0 && packets >= m_blockAckThreshold)
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
EdcaTxopN::SendBlockAckRequest (const Bar &bar)
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

  MacLowTransmissionParameters params;
  params.DisableRts ();
  params.DisableNextData ();
  params.DisableOverrideDurationId ();
  if (bar.immediate)
    {
      if (m_blockAckType == BASIC_BLOCK_ACK)
        {
          params.EnableBasicBlockAck ();
        }
      else if (m_blockAckType == COMPRESSED_BLOCK_ACK)
        {
          params.EnableCompressedBlockAck ();
        }
      else if (m_blockAckType == MULTI_TID_BLOCK_ACK)
        {
          NS_FATAL_ERROR ("Multi-tid block ack is not supported");
        }
    }
  else
    {
      //Delayed block ack
      params.EnableAck ();
    }
  m_low->StartTransmission (m_currentPacket, &m_currentHdr, params, this);
}

void
EdcaTxopN::CompleteConfig (void)
{
  NS_LOG_FUNCTION (this);
  m_baManager->SetTxMiddle (m_txMiddle);
  m_low->RegisterEdcaForAc (m_ac, this);
  m_baManager->SetBlockAckInactivityCallback (MakeCallback (&EdcaTxopN::SendDelbaFrame, this));
}

void
EdcaTxopN::SetBlockAckThreshold (uint8_t threshold)
{
  NS_LOG_FUNCTION (this << (uint16_t)threshold);
  m_blockAckThreshold = threshold;
  m_baManager->SetBlockAckThreshold (threshold);
}

void
EdcaTxopN::SetBlockAckInactivityTimeout (uint16_t timeout)
{
  NS_LOG_FUNCTION (this << timeout);
  m_blockAckInactivityTimeout = timeout;
}

uint8_t
EdcaTxopN::GetBlockAckThreshold (void) const
{
  NS_LOG_FUNCTION (this);
  return m_blockAckThreshold;
}

void
EdcaTxopN::SendAddBaRequest (Mac48Address dest, uint8_t tid, uint16_t startSeq,
                             uint16_t timeout, bool immediateBAck)
{
  NS_LOG_FUNCTION (this << dest << (uint16_t)tid << startSeq << timeout << immediateBAck);
  NS_LOG_DEBUG ("sent ADDBA request to " << dest);
  WifiMacHeader hdr;
  hdr.SetAction ();
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

  MacLowTransmissionParameters params;
  params.EnableAck ();
  params.DisableRts ();
  params.DisableNextData ();
  params.DisableOverrideDurationId ();

  m_low->StartTransmission (m_currentPacket, &m_currentHdr, params, this);
}

void
EdcaTxopN::SendDelbaFrame (Mac48Address addr, uint8_t tid, bool byOriginator)
{
  NS_LOG_FUNCTION (this << addr << (uint16_t)tid << byOriginator);
  WifiMacHeader hdr;
  hdr.SetAction ();
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
    }
  else
    {
      delbaHdr.SetByRecipient ();
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
EdcaTxopN::DoInitialize ()
{
  NS_LOG_FUNCTION (this);
  m_dcf->ResetCw ();
  m_cwTrace = m_dcf->GetCw ();
  m_backoffTrace = m_rng->GetNext (0, m_dcf->GetCw ());
  m_dcf->StartBackoffNow (m_backoffTrace);
}

void
EdcaTxopN::BaTxOk (const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this << hdr);
  if (!m_txOkCallback.IsNull ())
    {
      m_txOkCallback (m_currentHdr);
    }
}

void
EdcaTxopN::BaTxFailed (const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this << hdr);
  if (!m_txFailedCallback.IsNull ())
    {
      m_txFailedCallback (m_currentHdr);
    }
}

bool
EdcaTxopN::IsEdca ()
{
  return true;
}

} //namespace ns3
