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
#include "dca-txop.h"
#include "dcf-manager.h"
#include "dcf-state.h"
#include "wifi-mac-queue.h"
#include "mac-tx-middle.h"

#undef NS_LOG_APPEND_CONTEXT
#define NS_LOG_APPEND_CONTEXT if (m_low != 0) { std::clog << "[mac=" << m_low->GetAddress () << "] "; }

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("DcaTxop");

NS_OBJECT_ENSURE_REGISTERED (DcaTxop);

TypeId
DcaTxop::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DcaTxop")
    .SetParent<ns3::Object> ()
    .SetGroupName ("Wifi")
    .AddConstructor<DcaTxop> ()
    .AddAttribute ("MinCw", "The minimum value of the contention window.",
                   UintegerValue (15),
                   MakeUintegerAccessor (&DcaTxop::SetMinCw,
                                         &DcaTxop::GetMinCw),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MaxCw", "The maximum value of the contention window.",
                   UintegerValue (1023),
                   MakeUintegerAccessor (&DcaTxop::SetMaxCw,
                                         &DcaTxop::GetMaxCw),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Aifsn", "The AIFSN: the default value conforms to simple DCA.",
                   UintegerValue (2),
                   MakeUintegerAccessor (&DcaTxop::SetAifsn,
                                         &DcaTxop::GetAifsn),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("TxopLimit", "The TXOP limit: the default value conforms to simple DCA.",
                   TimeValue (MilliSeconds (0)),
                   MakeTimeAccessor (&DcaTxop::SetTxopLimit,
                                     &DcaTxop::GetTxopLimit),
                   MakeTimeChecker ())
    .AddAttribute ("Queue", "The WifiMacQueue object",
                   PointerValue (),
                   MakePointerAccessor (&DcaTxop::GetQueue),
                   MakePointerChecker<WifiMacQueue> ())
  ;
  return tid;
}

DcaTxop::DcaTxop ()
  : m_manager (0),
    m_currentPacket (0)
{
  NS_LOG_FUNCTION (this);
  m_dcf = CreateObject<DcfState> (this);
  m_queue = CreateObject<WifiMacQueue> ();
  m_rng = CreateObject<UniformRandomVariable> ();
}

DcaTxop::~DcaTxop ()
{
  NS_LOG_FUNCTION (this);
}

void
DcaTxop::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_queue = 0;
  m_low = 0;
  m_stationManager = 0;
  m_dcf = 0;
  m_rng = 0;
  m_txMiddle = 0;
}

void
DcaTxop::SetManager (const Ptr<DcfManager> manager)
{
  NS_LOG_FUNCTION (this << manager);
  m_manager = manager;
  m_manager->Add (m_dcf);
}

void DcaTxop::SetTxMiddle (const Ptr<MacTxMiddle> txMiddle)
{
  NS_LOG_FUNCTION (this);
  m_txMiddle = txMiddle;
}

void
DcaTxop::SetLow (const Ptr<MacLow> low)
{
  NS_LOG_FUNCTION (this << low);
  m_low = low;
}

void
DcaTxop::SetWifiRemoteStationManager (const Ptr<WifiRemoteStationManager> remoteManager)
{
  NS_LOG_FUNCTION (this << remoteManager);
  m_stationManager = remoteManager;
}

void
DcaTxop::SetTxOkCallback (TxOk callback)
{
  NS_LOG_FUNCTION (this << &callback);
  m_txOkCallback = callback;
}

void
DcaTxop::SetTxFailedCallback (TxFailed callback)
{
  NS_LOG_FUNCTION (this << &callback);
  m_txFailedCallback = callback;
}

void
DcaTxop::SetTxDroppedCallback (TxDropped callback)
{
  NS_LOG_FUNCTION (this << &callback);
  m_txDroppedCallback = callback;
  m_queue->TraceConnectWithoutContext ("Drop", MakeCallback (&DcaTxop::TxDroppedPacket, this));
}

void
DcaTxop::TxDroppedPacket (Ptr<const WifiMacQueueItem> item)
{
  if (!m_txDroppedCallback.IsNull ())
    {
      m_txDroppedCallback (item->GetPacket ());
    }
}

Ptr<WifiMacQueue >
DcaTxop::GetQueue () const
{
  NS_LOG_FUNCTION (this);
  return m_queue;
}

void
DcaTxop::SetMinCw (uint32_t minCw)
{
  NS_LOG_FUNCTION (this << minCw);
  m_dcf->SetCwMin (minCw);
}

void
DcaTxop::SetMaxCw (uint32_t maxCw)
{
  NS_LOG_FUNCTION (this << maxCw);
  m_dcf->SetCwMax (maxCw);
}

void
DcaTxop::SetAifsn (uint32_t aifsn)
{
  NS_LOG_FUNCTION (this << aifsn);
  m_dcf->SetAifsn (aifsn);
}

void
DcaTxop::SetTxopLimit (Time txopLimit)
{
  NS_LOG_FUNCTION (this << txopLimit);
  m_dcf->SetTxopLimit (txopLimit);
}

uint32_t
DcaTxop::GetMinCw (void) const
{
  return m_dcf->GetCwMin ();
}

uint32_t
DcaTxop::GetMaxCw (void) const
{
  return m_dcf->GetCwMax ();
}

uint32_t
DcaTxop::GetAifsn (void) const
{
  return m_dcf->GetAifsn ();
}

Time
DcaTxop::GetTxopLimit (void) const
{
  return m_dcf->GetTxopLimit ();
}

void
DcaTxop::Queue (Ptr<const Packet> packet, const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this << packet << &hdr);
  m_stationManager->PrepareForQueue (hdr.GetAddr1 (), &hdr, packet);
  m_queue->Enqueue (Create<WifiMacQueueItem> (packet, hdr));
  StartAccessIfNeeded ();
}

int64_t
DcaTxop::AssignStreams (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);
  m_rng->SetStream (stream);
  return 1;
}

void
DcaTxop::RestartAccessIfNeeded (void)
{
  NS_LOG_FUNCTION (this);
  if ((m_currentPacket != 0
       || !m_queue->IsEmpty ())
      && !m_dcf->IsAccessRequested ())
    {
      m_manager->RequestAccess (m_dcf);
    }
}

void
DcaTxop::StartAccessIfNeeded (void)
{
  NS_LOG_FUNCTION (this);
  if (m_currentPacket == 0
      && !m_queue->IsEmpty ()
      && !m_dcf->IsAccessRequested ())
    {
      m_manager->RequestAccess (m_dcf);
    }
}

Ptr<MacLow>
DcaTxop::GetLow (void) const
{
  NS_LOG_FUNCTION (this);
  return m_low;
}

void
DcaTxop::DoInitialize ()
{
  NS_LOG_FUNCTION (this);
  m_dcf->ResetCw ();
  m_dcf->StartBackoffNow (m_rng->GetInteger (0, m_dcf->GetCw ()));
}

bool
DcaTxop::NeedRtsRetransmission (Ptr<const Packet> packet, const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this);
  return m_stationManager->NeedRtsRetransmission (hdr.GetAddr1 (), &hdr, packet);
}

bool
DcaTxop::NeedDataRetransmission (Ptr<const Packet> packet, const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this);
  return m_stationManager->NeedDataRetransmission (hdr.GetAddr1 (), &hdr, packet);
}

bool
DcaTxop::NeedFragmentation (void) const
{
  NS_LOG_FUNCTION (this);
  return m_stationManager->NeedFragmentation (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                              m_currentPacket);
}

void
DcaTxop::NextFragment (void)
{
  NS_LOG_FUNCTION (this);
  m_fragmentNumber++;
}

uint32_t
DcaTxop::GetFragmentSize (void) const
{
  NS_LOG_FUNCTION (this);
  return m_stationManager->GetFragmentSize (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                            m_currentPacket, m_fragmentNumber);
}

bool
DcaTxop::IsLastFragment (void) const
{
  NS_LOG_FUNCTION (this);
  return m_stationManager->IsLastFragment (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                           m_currentPacket, m_fragmentNumber);
}

uint32_t
DcaTxop::GetNextFragmentSize (void) const
{
  NS_LOG_FUNCTION (this);
  return m_stationManager->GetFragmentSize (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                            m_currentPacket, m_fragmentNumber + 1);
}

uint32_t
DcaTxop::GetFragmentOffset (void) const
{
  NS_LOG_FUNCTION (this);
  return m_stationManager->GetFragmentOffset (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                              m_currentPacket, m_fragmentNumber);
}

Ptr<Packet>
DcaTxop::GetFragmentPacket (WifiMacHeader *hdr)
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
DcaTxop::NotifyAccessGranted (void)
{
  NS_LOG_FUNCTION (this);
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
  m_currentParams.DisableOverrideDurationId ();
  if (m_currentHdr.GetAddr1 ().IsGroup ())
    {
      m_currentParams.DisableRts ();
      m_currentParams.DisableAck ();
      m_currentParams.DisableNextData ();
      GetLow ()->StartTransmission (m_currentPacket, &m_currentHdr, m_currentParams, this);
      NS_LOG_DEBUG ("tx broadcast");
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
DcaTxop::NotifyInternalCollision (void)
{
  NS_LOG_FUNCTION (this);
  NotifyCollision ();
}

void
DcaTxop::NotifyCollision (void)
{
  NS_LOG_FUNCTION (this);
  m_dcf->StartBackoffNow (m_rng->GetInteger (0, m_dcf->GetCw ()));
  RestartAccessIfNeeded ();
}

void
DcaTxop::NotifyChannelSwitching (void)
{
  NS_LOG_FUNCTION (this);
  m_queue->Flush ();
  m_currentPacket = 0;
}

void
DcaTxop::NotifySleep (void)
{
  NS_LOG_FUNCTION (this);
  if (m_currentPacket != 0)
    {
      m_queue->PushFront (Create<WifiMacQueueItem> (m_currentPacket, m_currentHdr));
      m_currentPacket = 0;
    }
}

void
DcaTxop::NotifyWakeUp (void)
{
  NS_LOG_FUNCTION (this);
  RestartAccessIfNeeded ();
}

void
DcaTxop::MissedCts (void)
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
      m_dcf->ResetCw ();
    }
  else
    {
      m_dcf->UpdateFailedCw ();
    }
  m_dcf->StartBackoffNow (m_rng->GetInteger (0, m_dcf->GetCw ()));
  RestartAccessIfNeeded ();
}

void
DcaTxop::GotAck (void)
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
      m_dcf->ResetCw ();
      m_dcf->StartBackoffNow (m_rng->GetInteger (0, m_dcf->GetCw ()));
      RestartAccessIfNeeded ();
    }
  else
    {
      NS_LOG_DEBUG ("got ack. tx not done, size=" << m_currentPacket->GetSize ());
    }
}

void
DcaTxop::MissedAck (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("missed ack");
  if (!NeedDataRetransmission (m_currentPacket, m_currentHdr))
    {
      NS_LOG_DEBUG ("Ack Fail");
      m_stationManager->ReportFinalDataFailed (m_currentHdr.GetAddr1 (), &m_currentHdr);
      if (!m_txFailedCallback.IsNull ())
        {
          m_txFailedCallback (m_currentHdr);
        }
      //to reset the dcf.
      m_currentPacket = 0;
      m_dcf->ResetCw ();
    }
  else
    {
      NS_LOG_DEBUG ("Retransmit");
      m_currentHdr.SetRetry ();
      m_dcf->UpdateFailedCw ();
    }
  m_dcf->StartBackoffNow (m_rng->GetInteger (0, m_dcf->GetCw ()));
  RestartAccessIfNeeded ();
}

void
DcaTxop::StartNextFragment (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("start next packet fragment");
  /* this callback is used only for fragments. */
  NextFragment ();
  WifiMacHeader hdr;
  Ptr<Packet> fragment = GetFragmentPacket (&hdr);
  m_currentParams.EnableAck ();
  m_currentParams.DisableRts ();
  m_currentParams.DisableOverrideDurationId ();
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
DcaTxop::Cancel (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("transmission cancelled");
}

void
DcaTxop::EndTxNoAck (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("a transmission that did not require an ACK just finished");
  m_currentPacket = 0;
  m_dcf->ResetCw ();
  m_dcf->StartBackoffNow (m_rng->GetInteger (0, m_dcf->GetCw ()));
  StartAccessIfNeeded ();
}

bool
DcaTxop::IsEdca ()
{
  return false;
}

void
DcaTxop::StartNextPacket (void)
{
  NS_LOG_WARN ("StartNext should not be called for non QoS!");
}

void
DcaTxop::GotBlockAck (const CtrlBAckResponseHeader *blockAck, Mac48Address recipient, double rxSnr, WifiMode txMode, double dataSnr)
{
  NS_LOG_WARN ("GotBlockAck should not be called for non QoS!");
}

void
DcaTxop::MissedBlockAck (uint8_t nMpdus)
{
  NS_LOG_WARN ("MissedBlockAck should not be called for non QoS!");
}

bool
DcaTxop::HasTxop (void) const
{
  return false;
}

} //namespace ns3
