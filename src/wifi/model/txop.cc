/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/simulator.h"
#include "ns3/random-variable-stream.h"
#include "txop.h"
#include "channel-access-manager.h"
#include "wifi-mac-queue.h"
#include "mac-tx-middle.h"
#include "mac-low.h"
#include "wifi-remote-station-manager.h"

#undef NS_LOG_APPEND_CONTEXT
#define NS_LOG_APPEND_CONTEXT if (m_low != 0) { std::clog << "[mac=" << m_low->GetAddress () << "] "; }

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Txop");

NS_OBJECT_ENSURE_REGISTERED (Txop);

TypeId
Txop::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Txop")
    .SetParent<ns3::Object> ()
    .SetGroupName ("Wifi")
    .AddConstructor<Txop> ()
    .AddAttribute ("MinCw", "The minimum value of the contention window.",
                   UintegerValue (15),
                   MakeUintegerAccessor (&Txop::SetMinCw,
                                         &Txop::GetMinCw),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MaxCw", "The maximum value of the contention window.",
                   UintegerValue (1023),
                   MakeUintegerAccessor (&Txop::SetMaxCw,
                                         &Txop::GetMaxCw),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Aifsn", "The AIFSN: the default value conforms to non-QOS.",
                   UintegerValue (2),
                   MakeUintegerAccessor (&Txop::SetAifsn,
                                         &Txop::GetAifsn),
                   MakeUintegerChecker<uint8_t> ())
    .AddAttribute ("TxopLimit", "The TXOP limit: the default value conforms to non-QoS.",
                   TimeValue (MilliSeconds (0)),
                   MakeTimeAccessor (&Txop::SetTxopLimit,
                                     &Txop::GetTxopLimit),
                   MakeTimeChecker ())
    .AddAttribute ("Queue", "The WifiMacQueue object",
                   PointerValue (),
                   MakePointerAccessor (&Txop::GetWifiMacQueue),
                   MakePointerChecker<WifiMacQueue> ())
  ;
  return tid;
}

Txop::Txop ()
  : m_channelAccessManager (0),
    m_cwMin (0),
    m_cwMax (0),
    m_cw (0),
    m_accessRequested (false),
    m_backoffSlots (0),
    m_backoffStart (Seconds (0.0)),
    m_currentPacket (0)
{
  NS_LOG_FUNCTION (this);
  m_queue = CreateObject<WifiMacQueue> ();
  m_rng = CreateObject<UniformRandomVariable> ();
}

Txop::~Txop ()
{
  NS_LOG_FUNCTION (this);
}

void
Txop::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_queue = 0;
  m_low = 0;
  m_stationManager = 0;
  m_rng = 0;
  m_txMiddle = 0;
  m_channelAccessManager = 0;
}

void
Txop::SetChannelAccessManager (const Ptr<ChannelAccessManager> manager)
{
  NS_LOG_FUNCTION (this << manager);
  m_channelAccessManager = manager;
  m_channelAccessManager->Add (this);
}

void Txop::SetTxMiddle (const Ptr<MacTxMiddle> txMiddle)
{
  NS_LOG_FUNCTION (this);
  m_txMiddle = txMiddle;
}

void
Txop::SetMacLow (const Ptr<MacLow> low)
{
  NS_LOG_FUNCTION (this << low);
  m_low = low;
}

void
Txop::SetWifiRemoteStationManager (const Ptr<WifiRemoteStationManager> remoteManager)
{
  NS_LOG_FUNCTION (this << remoteManager);
  m_stationManager = remoteManager;
}

void
Txop::SetTxOkCallback (TxOk callback)
{
  NS_LOG_FUNCTION (this << &callback);
  m_txOkCallback = callback;
}

void
Txop::SetTxFailedCallback (TxFailed callback)
{
  NS_LOG_FUNCTION (this << &callback);
  m_txFailedCallback = callback;
}

void
Txop::SetTxDroppedCallback (TxDropped callback)
{
  NS_LOG_FUNCTION (this << &callback);
  m_txDroppedCallback = callback;
  m_queue->TraceConnectWithoutContext ("Drop", MakeCallback (&Txop::TxDroppedPacket, this));
}

void
Txop::TxDroppedPacket (Ptr<const WifiMacQueueItem> item)
{
  if (!m_txDroppedCallback.IsNull ())
    {
      m_txDroppedCallback (item->GetPacket ());
    }
}

Ptr<WifiMacQueue >
Txop::GetWifiMacQueue () const
{
  NS_LOG_FUNCTION (this);
  return m_queue;
}

void
Txop::SetMinCw (uint32_t minCw)
{
  NS_LOG_FUNCTION (this << minCw);
  bool changed = (m_cwMin != minCw);
  m_cwMin = minCw;
  if (changed == true)
    {
      ResetCw ();
    }
}

void
Txop::SetMaxCw (uint32_t maxCw)
{
  NS_LOG_FUNCTION (this << maxCw);
  bool changed = (m_cwMax != maxCw);
  m_cwMax = maxCw;
  if (changed == true)
    {
      ResetCw ();
    }
}

uint32_t
Txop::GetCw (void) const
{
  return m_cw;
}

void
Txop::ResetCw (void)
{
  NS_LOG_FUNCTION (this);
  m_cw = m_cwMin;
}

void
Txop::UpdateFailedCw (void)
{
  NS_LOG_FUNCTION (this);
  //see 802.11-2012, section 9.19.2.5
  m_cw = std::min ( 2 * (m_cw + 1) - 1, m_cwMax);
}

uint32_t
Txop::GetBackoffSlots (void) const
{
  return m_backoffSlots;
}

Time
Txop::GetBackoffStart (void) const
{
  return m_backoffStart;
}

void
Txop::UpdateBackoffSlotsNow (uint32_t nSlots, Time backoffUpdateBound)
{
  NS_LOG_FUNCTION (this << nSlots << backoffUpdateBound);
  m_backoffSlots -= nSlots;
  m_backoffStart = backoffUpdateBound;
  NS_LOG_DEBUG ("update slots=" << nSlots << " slots, backoff=" << m_backoffSlots);
}

void
Txop::StartBackoffNow (uint32_t nSlots)
{
  NS_LOG_FUNCTION (this << nSlots);
  if (m_backoffSlots != 0)
    {
      NS_LOG_DEBUG ("reset backoff from " << m_backoffSlots << " to " << nSlots << " slots");
    }
  else
    {
      NS_LOG_DEBUG ("start backoff=" << nSlots << " slots");
    }
  m_backoffSlots = nSlots;
  m_backoffStart = Simulator::Now ();
}

void
Txop::SetAifsn (uint8_t aifsn)
{
  NS_LOG_FUNCTION (this << +aifsn);
  m_aifsn = aifsn;
}

void
Txop::SetTxopLimit (Time txopLimit)
{
  NS_LOG_FUNCTION (this << txopLimit);
  NS_ASSERT_MSG ((txopLimit.GetMicroSeconds () % 32 == 0), "The TXOP limit must be expressed in multiple of 32 microseconds!");
  m_txopLimit = txopLimit;
}

uint32_t
Txop::GetMinCw (void) const
{
  return m_cwMin;
}

uint32_t
Txop::GetMaxCw (void) const
{
  return m_cwMax;
}

uint8_t
Txop::GetAifsn (void) const
{
  return m_aifsn;
}

Time
Txop::GetTxopLimit (void) const
{
  return m_txopLimit;
}

void
Txop::Queue (Ptr<const Packet> packet, const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this << packet << &hdr);
  m_stationManager->PrepareForQueue (hdr.GetAddr1 (), &hdr, packet);
  m_queue->Enqueue (Create<WifiMacQueueItem> (packet, hdr));
  StartAccessIfNeeded ();
}

int64_t
Txop::AssignStreams (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);
  m_rng->SetStream (stream);
  return 1;
}

void
Txop::RestartAccessIfNeeded (void)
{
  NS_LOG_FUNCTION (this);
  if ((m_currentPacket != 0
       || !m_queue->IsEmpty ())
      && !IsAccessRequested ()
      && !m_low->IsCfPeriod ())
    {
      m_channelAccessManager->RequestAccess (this);
    }
}

void
Txop::StartAccessIfNeeded (void)
{
  NS_LOG_FUNCTION (this);
  if (m_currentPacket == 0
      && !m_queue->IsEmpty ()
      && !IsAccessRequested ()
      && !m_low->IsCfPeriod ())
    {
      m_channelAccessManager->RequestAccess (this);
    }
}

Ptr<MacLow>
Txop::GetLow (void) const
{
  return m_low;
}

void
Txop::DoInitialize ()
{
  NS_LOG_FUNCTION (this);
  ResetCw ();
  StartBackoffNow (m_rng->GetInteger (0, GetCw ()));
}

bool
Txop::NeedRtsRetransmission (Ptr<const Packet> packet, const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this);
  return m_stationManager->NeedRetransmission (hdr.GetAddr1 (), &hdr, packet);
}

bool
Txop::NeedDataRetransmission (Ptr<const Packet> packet, const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this);
  return m_stationManager->NeedRetransmission (hdr.GetAddr1 (), &hdr, packet);
}

bool
Txop::NeedFragmentation (void) const
{
  NS_LOG_FUNCTION (this);
  return m_stationManager->NeedFragmentation (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                              m_currentPacket);
}

void
Txop::NextFragment (void)
{
  NS_LOG_FUNCTION (this);
  m_fragmentNumber++;
}

uint32_t
Txop::GetFragmentSize (void) const
{
  NS_LOG_FUNCTION (this);
  return m_stationManager->GetFragmentSize (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                            m_currentPacket, m_fragmentNumber);
}

bool
Txop::IsLastFragment (void) const
{
  NS_LOG_FUNCTION (this);
  return m_stationManager->IsLastFragment (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                           m_currentPacket, m_fragmentNumber);
}

uint32_t
Txop::GetNextFragmentSize (void) const
{
  NS_LOG_FUNCTION (this);
  return m_stationManager->GetFragmentSize (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                            m_currentPacket, m_fragmentNumber + 1);
}

uint32_t
Txop::GetFragmentOffset (void) const
{
  NS_LOG_FUNCTION (this);
  return m_stationManager->GetFragmentOffset (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                              m_currentPacket, m_fragmentNumber);
}

Ptr<Packet>
Txop::GetFragmentPacket (WifiMacHeader *hdr)
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

bool
Txop::IsAccessRequested (void) const
{
  return m_accessRequested;
}

void
Txop::NotifyAccessRequested (void)
{
  NS_LOG_FUNCTION (this);
  m_accessRequested = true;
}

void
Txop::NotifyAccessGranted (void)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (m_accessRequested);
  m_accessRequested = false;
  if (m_currentPacket == 0)
    {
      if (m_queue->IsEmpty ())
        {
          NS_LOG_DEBUG ("queue empty");
          return;
        }
      Ptr<WifiMacQueueItem> item = m_queue->Dequeue ();
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
      NS_LOG_DEBUG ("dequeued size=" << m_currentPacket->GetSize () <<
                    ", to=" << m_currentHdr.GetAddr1 () <<
                    ", seq=" << m_currentHdr.GetSequenceControl ());
    }
  if (m_currentHdr.GetAddr1 ().IsGroup ())
    {
      m_currentParams.DisableRts ();
      m_currentParams.DisableAck ();
      m_currentParams.DisableNextData ();
      NS_LOG_DEBUG ("tx broadcast");
      GetLow ()->StartTransmission (m_currentPacket, &m_currentHdr, m_currentParams, this);
    }
  else
    {
      m_currentParams.EnableAck ();
      if (NeedFragmentation ())
        {
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
          GetLow ()->StartTransmission (fragment, &hdr, m_currentParams, this);
        }
      else
        {
          m_currentParams.DisableNextData ();
          GetLow ()->StartTransmission (m_currentPacket, &m_currentHdr, m_currentParams, this);
        }
    }
}

void
Txop::NotifyInternalCollision (void)
{
  NS_LOG_FUNCTION (this);
  NotifyCollision ();
}

void
Txop::NotifyCollision (void)
{
  NS_LOG_FUNCTION (this);
  StartBackoffNow (m_rng->GetInteger (0, GetCw ()));
  RestartAccessIfNeeded ();
}

void
Txop::NotifyChannelSwitching (void)
{
  NS_LOG_FUNCTION (this);
  m_queue->Flush ();
  m_currentPacket = 0;
}

void
Txop::NotifySleep (void)
{
  NS_LOG_FUNCTION (this);
  if (m_currentPacket != 0)
    {
      m_queue->PushFront (Create<WifiMacQueueItem> (m_currentPacket, m_currentHdr));
      m_currentPacket = 0;
    }
}

void
Txop::NotifyOff (void)
{
  NS_LOG_FUNCTION (this);
  m_queue->Flush ();
  m_currentPacket = 0;
}

void
Txop::NotifyWakeUp (void)
{
  NS_LOG_FUNCTION (this);
  RestartAccessIfNeeded ();
}

void
Txop::NotifyOn (void)
{
  NS_LOG_FUNCTION (this);
  StartAccessIfNeeded ();
}

void
Txop::MissedCts (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("missed cts");
  if (!NeedRtsRetransmission (m_currentPacket, m_currentHdr))
    {
      NS_LOG_DEBUG ("Cts Fail");
      m_stationManager->ReportFinalRtsFailed (m_currentHdr.GetAddr1 (), &m_currentHdr);
      if (!m_txFailedCallback.IsNull ())
        {
          m_txFailedCallback (m_currentHdr);
        }
      //to reset the dcf.
      m_currentPacket = 0;
      ResetCw ();
    }
  else
    {
      UpdateFailedCw ();
    }
  StartBackoffNow (m_rng->GetInteger (0, GetCw ()));
  RestartAccessIfNeeded ();
}

void
Txop::GotAck (void)
{
  NS_LOG_FUNCTION (this);
  if (!NeedFragmentation ()
      || IsLastFragment ())
    {
      NS_LOG_DEBUG ("got ack. tx done.");
      if (!m_txOkCallback.IsNull ())
        {
          m_txOkCallback (m_currentHdr);
        }

      /* we are not fragmenting or we are done fragmenting
       * so we can get rid of that packet now.
       */
      m_currentPacket = 0;
      ResetCw ();
      StartBackoffNow (m_rng->GetInteger (0, GetCw ()));
      RestartAccessIfNeeded ();
    }
  else
    {
      NS_LOG_DEBUG ("got ack. tx not done, size=" << m_currentPacket->GetSize ());
    }
}

void
Txop::MissedAck (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("missed ack");
  if (!NeedDataRetransmission (m_currentPacket, m_currentHdr))
    {
      NS_LOG_DEBUG ("Ack Fail");
      m_stationManager->ReportFinalDataFailed (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                               m_currentPacket->GetSize ());
      if (!m_txFailedCallback.IsNull ())
        {
          m_txFailedCallback (m_currentHdr);
        }
      //to reset the dcf.
      m_currentPacket = 0;
      ResetCw ();
    }
  else
    {
      NS_LOG_DEBUG ("Retransmit");
      m_currentHdr.SetRetry ();
      UpdateFailedCw ();
    }
  StartBackoffNow (m_rng->GetInteger (0, GetCw ()));
  RestartAccessIfNeeded ();
}

void
Txop::GotCfEnd (void)
{
  NS_LOG_FUNCTION (this);
  if (m_currentPacket != 0)
    {
      RestartAccessIfNeeded ();
    }
  else
    {
      StartAccessIfNeeded ();
    }
}

void
Txop::MissedCfPollResponse (bool expectedCfAck)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("missed response to CF-POLL");
  if (expectedCfAck)
    {
      if (!NeedDataRetransmission (m_currentPacket, m_currentHdr))
        {
          NS_LOG_DEBUG ("Ack Fail");
          m_stationManager->ReportFinalDataFailed (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                                   m_currentPacket->GetSize ());
          m_currentPacket = 0;
        }
      else
        {
          NS_LOG_DEBUG ("Retransmit");
          m_currentHdr.SetRetry ();
        }
    }
  if (!m_txFailedCallback.IsNull ())
    {
      m_txFailedCallback (m_currentHdr);
    }
}

void
Txop::StartNextFragment (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("start next packet fragment");
  /* this callback is used only for fragments. */
  NextFragment ();
  WifiMacHeader hdr;
  Ptr<Packet> fragment = GetFragmentPacket (&hdr);
  m_currentParams.EnableAck ();
  m_currentParams.DisableRts ();
  if (IsLastFragment ())
    {
      m_currentParams.DisableNextData ();
    }
  else
    {
      m_currentParams.EnableNextData (GetNextFragmentSize ());
    }
  GetLow ()->StartTransmission (fragment, &hdr, m_currentParams, this);
}

void
Txop::Cancel (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("transmission cancelled");
}

void
Txop::EndTxNoAck (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("a transmission that did not require an ACK just finished");
  m_currentPacket = 0;
  ResetCw ();
  StartBackoffNow (m_rng->GetInteger (0, GetCw ()));
  if (!m_txOkCallback.IsNull ())
    {
      m_txOkCallback (m_currentHdr);
    }
  StartAccessIfNeeded ();
}

void
Txop::SendCfFrame (WifiMacType frameType, Mac48Address addr)
{
  NS_LOG_FUNCTION (this << frameType << addr);
  NS_ASSERT (m_low->IsCfPeriod ());
  if (m_currentPacket != 0 && frameType != WIFI_MAC_CTL_END)
    {
      if (!NeedDataRetransmission (m_currentPacket, m_currentHdr))
        {
          m_stationManager->ReportFinalDataFailed (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                                   m_currentPacket->GetSize ());
          m_currentPacket = 0;
        }
      else
        {
          m_currentHdr.SetRetry ();
        }
    }
  else if ((m_queue->GetNPacketsByAddress (addr) > 0) && (frameType != WIFI_MAC_CTL_END)) //if no packet for that dest, send to another dest?
    {
      Ptr<WifiMacQueueItem> item = m_queue->DequeueByAddress (addr);
      NS_ASSERT (item != 0);
      m_currentPacket = item->GetPacket ();
      m_currentHdr = item->GetHeader ();
      uint16_t sequence = m_txMiddle->GetNextSequenceNumberFor (&m_currentHdr);
      m_currentHdr.SetSequenceNumber (sequence);
      m_currentHdr.SetFragmentNumber (0);
      m_currentHdr.SetNoMoreFragments ();
      m_currentHdr.SetNoRetry ();
    }
  else
    {
      m_currentPacket = Create<Packet> ();
      m_currentHdr.SetNoRetry ();
    }

  if (m_currentPacket->GetSize () > 0)
    {
      switch (frameType)
        {
        case WIFI_MAC_DATA_NULL_CFPOLL:
          m_currentHdr.SetType (WIFI_MAC_DATA_CFPOLL);
          break;
        case WIFI_MAC_DATA_NULL:
          m_currentHdr.SetType (WIFI_MAC_DATA);
          break;
        default:
          NS_ASSERT (false);
          break;
        }
    }
  else
    {
      m_currentHdr.SetType (frameType);
    }
  m_currentHdr.SetAddr1 (addr);
  m_currentHdr.SetAddr2 (m_low->GetAddress ());
  if (frameType == WIFI_MAC_DATA_NULL)
    {
      m_currentHdr.SetAddr3 (m_low->GetBssid ());
      m_currentHdr.SetDsTo ();
      m_currentHdr.SetDsNotFrom ();
    }
  else
    {
      m_currentHdr.SetAddr3 (m_low->GetAddress ());
      m_currentHdr.SetDsNotTo ();
      m_currentHdr.SetDsFrom ();
    }
  m_channelAccessManager->RequestAccess (this, true);
}

bool
Txop::CanStartNextPolling () const
{
  return (!m_channelAccessManager->IsBusy () && GetLow ()->CanTransmitNextCfFrame ());
}

bool
Txop::IsQosTxop () const
{
  return false;
}

void
Txop::StartNextPacket (void)
{
  NS_LOG_WARN ("StartNext should not be called for non QoS!");
}

void
Txop::GotBlockAck (const CtrlBAckResponseHeader *blockAck, Mac48Address recipient, double rxSnr, WifiMode txMode, double dataSnr)
{
  NS_LOG_WARN ("GotBlockAck should not be called for non QoS!");
}

void
Txop::MissedBlockAck (uint8_t nMpdus)
{
  NS_LOG_WARN ("MissedBlockAck should not be called for non QoS!");
}

bool
Txop::HasTxop (void) const
{
  return false;
}

} //namespace ns3
