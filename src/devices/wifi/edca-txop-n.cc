/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 * Author: Mirko Banchi <mk.banchi@gmail.com>
 */
#include "ns3/log.h"
#include "ns3/assert.h"

#include "edca-txop-n.h"
#include "mac-low.h"
#include "dcf-manager.h"
#include "mac-tx-middle.h"
#include "wifi-mac-trailer.h"
#include "wifi-mac.h"
#include "random-stream.h"
#include "wifi-mac-queue.h"
#include "msdu-aggregator.h"

NS_LOG_COMPONENT_DEFINE ("EdcaTxopN");

#define MY_DEBUG(x) \
  NS_LOG_DEBUG (m_low->GetAddress () << " " << x)

namespace ns3 {

class EdcaTxopN::Dcf : public DcfState
{
public:
  Dcf (EdcaTxopN *txop)
    : m_txop (txop)
  {}
private:
  virtual void DoNotifyAccessGranted (void) {
    m_txop->NotifyAccessGranted ();
  }
  virtual void DoNotifyInternalCollision (void) {
    m_txop->NotifyInternalCollision ();
  }
  virtual void DoNotifyCollision (void) {
    m_txop->NotifyCollision ();
  }
  EdcaTxopN *m_txop;
};

class EdcaTxopN::TransmissionListener : public MacLowTransmissionListener
{
public:
  TransmissionListener (EdcaTxopN *txop)
    : MacLowTransmissionListener (),
      m_txop (txop) {}
      
  virtual ~TransmissionListener () {}

  virtual void GotCts (double snr, WifiMode txMode) {
    m_txop->GotCts (snr, txMode);
  }
  virtual void MissedCts (void) {
    m_txop->MissedCts ();
  }
  virtual void GotAck (double snr, WifiMode txMode) {
    m_txop->GotAck (snr, txMode);
  }
  virtual void MissedAck (void) {
    m_txop->MissedAck ();
  }
  virtual void StartNext (void) {
    m_txop->StartNext ();
  }
  virtual void Cancel (void) {
    m_txop->Cancel ();
  }

private:
  EdcaTxopN *m_txop;
};

NS_OBJECT_ENSURE_REGISTERED (EdcaTxopN);

TypeId
EdcaTxopN::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EdcaTxopN")
    .SetParent<Object> ()
    .AddConstructor<EdcaTxopN> ()
    .AddAttribute ("MinCw", "The minimun value of the contention window.",
                   UintegerValue (31),
                   MakeUintegerAccessor (&EdcaTxopN::SetMinCw,
                                         &EdcaTxopN::GetMinCw),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MaxCw", "The maximum value of the contention window.",
                   UintegerValue (1023),
                   MakeUintegerAccessor (&EdcaTxopN::SetMaxCw,
                                         &EdcaTxopN::GetMaxCw),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Aifsn", "The AIFSN: the default value conforms to simple DCA.",
                   UintegerValue (3),
                   MakeUintegerAccessor (&EdcaTxopN::SetAifsn,
                                         &EdcaTxopN::GetAifsn),
                   MakeUintegerChecker<uint32_t> ())
    ;
  return tid;
}

EdcaTxopN::EdcaTxopN ()
  : m_manager (0),
    m_currentPacket(0),
    m_aggregator (0)
{
  NS_LOG_FUNCTION (this);
  m_transmissionListener = new EdcaTxopN::TransmissionListener (this);
  m_dcf = new EdcaTxopN::Dcf (this);
  m_queue = CreateObject<WifiMacQueue> ();
  m_rng = new RealRandomStream ();
}

EdcaTxopN::~EdcaTxopN ()
{
  NS_LOG_FUNCTION (this);
}

void
EdcaTxopN::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_queue = 0;
  m_low = 0;
  m_stationManager = 0;
  delete m_transmissionListener;
  delete m_dcf;
  delete m_rng;
  m_transmissionListener = 0;
  m_dcf = 0;
  m_rng = 0;
  m_txMiddle = 0;
  m_aggregator = 0;
}

void
EdcaTxopN::SetManager (DcfManager *manager)
{
  NS_LOG_FUNCTION (this << manager);
  m_manager = manager;
  m_manager->Add (m_dcf);
}

void
EdcaTxopN::SetTxOkCallback (TxOk callback)
{
  m_txOkCallback = callback;
}

void 
EdcaTxopN::SetTxFailedCallback (TxFailed callback)
{
  m_txFailedCallback = callback;
}

void
EdcaTxopN::SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> remoteManager)
{
  NS_LOG_FUNCTION (this << remoteManager);
  m_stationManager = remoteManager;
}
void
EdcaTxopN::SetTypeOfStation (enum TypeOfStation type)
{
  NS_LOG_FUNCTION (this << type);
  m_typeOfStation = type;
}

enum TypeOfStation
EdcaTxopN::GetTypeOfStation (void) const
{
  return m_typeOfStation;
}

void 
EdcaTxopN::SetMaxQueueSize (uint32_t size)
{
  NS_LOG_FUNCTION (this << size);
  m_queue->SetMaxSize (size);
}

void
EdcaTxopN::SetMaxQueueDelay (Time delay)
{
  NS_LOG_FUNCTION (this << delay);
  m_queue->SetMaxDelay (delay);
}

void 
EdcaTxopN::SetMinCw (uint32_t minCw)
{
  NS_LOG_FUNCTION (this << minCw);
  m_dcf->SetCwMin (minCw);
}

void 
EdcaTxopN::SetMaxCw (uint32_t maxCw)
{
  NS_LOG_FUNCTION (this << maxCw);
  m_dcf->SetCwMax (maxCw);
}

void 
EdcaTxopN::SetAifsn (uint32_t aifsn)
{
  NS_LOG_FUNCTION (this << aifsn);
  m_dcf->SetAifsn (aifsn);
}

uint32_t 
EdcaTxopN::GetMinCw (void) const
{
  return m_dcf->GetCwMin ();
}

uint32_t 
EdcaTxopN::GetMaxCw (void) const
{
  return m_dcf->GetCwMax ();
}

uint32_t 
EdcaTxopN::GetAifsn (void) const
{
  return m_dcf->GetAifsn ();
}

void
EdcaTxopN::SetTxMiddle (MacTxMiddle *txMiddle)
{
  m_txMiddle = txMiddle;
}

Ptr<MacLow>
EdcaTxopN::Low (void)
{
  return m_low;
}

void
EdcaTxopN::SetLow(Ptr<MacLow> low)
{
  NS_LOG_FUNCTION (this << low);
  m_low = low;
}

bool
EdcaTxopN::NeedsAccess (void) const
{
  return !m_queue->IsEmpty () || m_currentPacket != 0;
}

void
EdcaTxopN::NotifyAccessGranted (void)
{
  NS_LOG_FUNCTION (this);
  if (m_currentPacket == 0)
    {
      if (m_queue->IsEmpty ())
        {
          MY_DEBUG ("queue is empty");
          return; 
        }
      m_currentPacket = m_queue->Dequeue (&m_currentHdr);
      NS_ASSERT (m_currentPacket != 0);
      
      uint16_t sequence = m_txMiddle->GetNextSequenceNumberfor (&m_currentHdr);
      m_currentHdr.SetSequenceNumber (sequence);
      m_currentHdr.SetFragmentNumber (0);
      m_currentHdr.SetNoMoreFragments ();
      m_currentHdr.SetNoRetry ();
      m_fragmentNumber = 0;
      MY_DEBUG ("dequeued size="<<m_currentPacket->GetSize ()<<
                ", to="<<m_currentHdr.GetAddr1 ()<<
                ", seq="<<m_currentHdr.GetSequenceControl ());
    }
  MacLowTransmissionParameters params;
  params.DisableOverrideDurationId ();
  if (m_currentHdr.GetAddr1 ().IsBroadcast ()) 
    {
      params.DisableRts ();
      params.DisableAck ();
      params.DisableNextData ();
      m_low->StartTransmission (m_currentPacket,
                                 &m_currentHdr,
                                 params,
                                 m_transmissionListener);
      
      m_currentPacket = 0;
      m_dcf->ResetCw ();
      m_dcf->StartBackoffNow (m_rng->GetNext (0, m_dcf->GetCw ()));
      StartAccessIfNeeded ();
      MY_DEBUG ("tx broadcast");
    }
  else
    {
      params.EnableAck ();
      if (NeedFragmentation () && ((m_currentHdr.IsQosData () &&
                                    !m_currentHdr.IsQosAmsdu ()) ||
                                    m_currentHdr.IsData ()))
        {
          params.DisableRts ();
          WifiMacHeader hdr;
          Ptr<Packet> fragment = GetFragmentPacket (&hdr);
          if (IsLastFragment ()) 
            {
              MY_DEBUG ("fragmenting last fragment size=" << fragment->GetSize ());
              params.DisableNextData ();
            } 
          else 
            {
              MY_DEBUG ("fragmenting size=" << fragment->GetSize ());
              params.EnableNextData (GetNextFragmentSize ());
            }
          m_low->StartTransmission (fragment, &hdr, params, 
                                     m_transmissionListener);
        }
      else
        {
          WifiMacHeader peekedHdr;
          if (m_currentHdr.IsQosData () &&
              m_queue->PeekByTidAndAddress (&peekedHdr, m_currentHdr.GetQosTid (), 
                                            WifiMacHeader::ADDR1, m_currentHdr.GetAddr1 ()) &&
              !m_currentHdr.GetAddr1 ().IsBroadcast () &&
              m_aggregator != 0)
            {
              /* here is performed aggregation */
              Ptr<Packet> currentAggregatedPacket = Create<Packet> ();
              m_aggregator->Aggregate (m_currentPacket, currentAggregatedPacket,
                                       MapSrcAddressForAggregation (peekedHdr),
                                       MapDestAddressForAggregation (peekedHdr));
              bool aggregated = false;
              bool isAmsdu = false;
              Ptr<const Packet> peekedPacket = m_queue->PeekByTidAndAddress (&peekedHdr, m_currentHdr.GetQosTid (), 
                                                                       WifiMacHeader::ADDR1, 
                                                                       m_currentHdr.GetAddr1 ());
              while (peekedPacket != 0)
                {
                  aggregated = m_aggregator->Aggregate (peekedPacket, currentAggregatedPacket,
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
                                                               WifiMacHeader::ADDR1, m_currentHdr.GetAddr1 ());
                }
              if (isAmsdu)
                {
                  m_currentHdr.SetQosAmsdu ();
                  m_currentHdr.SetAddr3 (m_low->GetBssid ());
                  m_currentPacket = currentAggregatedPacket;
                  currentAggregatedPacket = 0;
                  MY_DEBUG ("tx unicast A-MSDU");
                }
            }
          if (NeedRts ())
            {
              params.EnableRts ();
              MY_DEBUG ("tx unicast rts");
            } 
          else 
            {
              params.DisableRts ();
              MY_DEBUG ("tx unicast");
            }
          params.DisableNextData ();
          m_low->StartTransmission (m_currentPacket, &m_currentHdr,
                                    params, m_transmissionListener);
        }
    }
}

void EdcaTxopN::NotifyInternalCollision (void)
{
  NS_LOG_FUNCTION (this);
  NotifyCollision ();
}

void
EdcaTxopN::NotifyCollision (void)
{
  NS_LOG_FUNCTION (this);
  m_dcf->StartBackoffNow (m_rng->GetNext (0, m_dcf->GetCw ()));
  RestartAccessIfNeeded ();
}

void 
EdcaTxopN::GotCts (double snr, WifiMode txMode)
{
  NS_LOG_FUNCTION (this << snr << txMode);
  MY_DEBUG ("got cts");
}

void 
EdcaTxopN::MissedCts (void)
{
  NS_LOG_FUNCTION (this);
  MY_DEBUG ("missed cts");
  if (!NeedRtsRetransmission ())
    {
      MY_DEBUG ("Cts Fail");
      WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
      station->ReportFinalRtsFailed ();
      if (!m_txFailedCallback.IsNull ()) 
        {
          m_txFailedCallback (m_currentHdr);
        }
      // to reset the dcf.
      m_currentPacket = 0;
      m_dcf->ResetCw ();
    } 
  else 
    {
      m_dcf->UpdateFailedCw ();
    }
  m_dcf->StartBackoffNow (m_rng->GetNext (0, m_dcf->GetCw ()));
  RestartAccessIfNeeded ();
}

void
EdcaTxopN::Queue (Ptr<const Packet> packet, WifiMacHeader const &hdr)
{
  NS_LOG_FUNCTION (this << packet << &hdr);
  WifiMacTrailer fcs;
  uint32_t fullPacketSize = hdr.GetSerializedSize () + packet->GetSize () + fcs.GetSerializedSize ();
  WifiRemoteStation *station = GetStation (hdr.GetAddr1 ());
  station->PrepareForQueue (packet, fullPacketSize);
  m_queue->Enqueue (packet, hdr);
  StartAccessIfNeeded ();
}

void
EdcaTxopN::GotAck (double snr, WifiMode txMode)
{
  NS_LOG_FUNCTION (this << snr << txMode);
  if (!NeedFragmentation () ||
      IsLastFragment () ||
      m_currentHdr.IsQosAmsdu ()) 
    {
      MY_DEBUG ("got ack. tx done.");
      if (!m_txOkCallback.IsNull ())
        {
           m_txOkCallback (m_currentHdr);
        }
      m_currentPacket = 0;
         
      m_dcf->ResetCw ();
      m_dcf->StartBackoffNow (m_rng->GetNext (0, m_dcf->GetCw ()));
      RestartAccessIfNeeded ();
    } 
  else 
    {
      MY_DEBUG ("got ack. tx not done, size="<<m_currentPacket->GetSize ());
    }
}

void
EdcaTxopN::MissedAck (void)
{
  NS_LOG_FUNCTION (this);
  MY_DEBUG ("missed ack");
  if (!NeedDataRetransmission ()) 
    {
      MY_DEBUG ("Ack Fail");
      WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
      station->ReportFinalDataFailed ();
      if (!m_txFailedCallback.IsNull ()) 
        {
          m_txFailedCallback (m_currentHdr);
        }
      // to reset the dcf.
      m_currentPacket = 0;
      m_dcf->ResetCw ();
    } 
  else 
    {
      MY_DEBUG ("Retransmit");
      m_currentHdr.SetRetry ();
      m_dcf->UpdateFailedCw ();
    }
  m_dcf->StartBackoffNow (m_rng->GetNext (0, m_dcf->GetCw ()));
  RestartAccessIfNeeded ();
}

Ptr<MsduAggregator>
EdcaTxopN::GetMsduAggregator (void) const
{
  return m_aggregator;
}

void
EdcaTxopN::RestartAccessIfNeeded (void)
{
  NS_LOG_FUNCTION (this);
  if ((m_currentPacket != 0 ||
       !m_queue->IsEmpty ()) &&
       !m_dcf->IsAccessRequested ())
    {
      m_manager->RequestAccess (m_dcf);
    }
}

void
EdcaTxopN::StartAccessIfNeeded (void)
{
  NS_LOG_FUNCTION (this);
  if (m_currentPacket == 0 &&
      !m_queue->IsEmpty () &&
      !m_dcf->IsAccessRequested ())
    {
      m_manager->RequestAccess (m_dcf);
    }
}

bool
EdcaTxopN::NeedRts (void)
{
  WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
  return station->NeedRts (m_currentPacket);
}

bool
EdcaTxopN::NeedRtsRetransmission (void)
{
  WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
  return station->NeedRtsRetransmission (m_currentPacket);
}

bool
EdcaTxopN::NeedDataRetransmission (void)
{
  WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
  return station->NeedDataRetransmission (m_currentPacket);
}

void
EdcaTxopN::NextFragment (void)
{
  m_fragmentNumber++;
}

void 
EdcaTxopN::StartNext (void)
{
  NS_LOG_FUNCTION (this);
  MY_DEBUG ("start next packet fragment");
  /* this callback is used only for fragments. */
  NextFragment ();
  WifiMacHeader hdr;
  Ptr<Packet> fragment = GetFragmentPacket (&hdr);
  MacLowTransmissionParameters params;
  params.EnableAck ();
  params.DisableRts ();
  params.DisableOverrideDurationId ();
  if (IsLastFragment ()) 
    {
      params.DisableNextData ();
    } 
  else 
    {
      params.EnableNextData (GetNextFragmentSize ());
    }
  Low ()->StartTransmission (fragment, &hdr, params, m_transmissionListener);
}

void
EdcaTxopN::Cancel (void)
{
  NS_LOG_FUNCTION (this);
  MY_DEBUG ("transmission cancelled");
}

bool
EdcaTxopN::NeedFragmentation (void) const
{
  WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
  return station->NeedFragmentation (m_currentPacket);
}

uint32_t
EdcaTxopN::GetFragmentSize (void)
{
  WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
  return station->GetFragmentSize (m_currentPacket, m_fragmentNumber);
}

uint32_t
EdcaTxopN::GetNextFragmentSize (void) 
{
  WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
  return station->GetFragmentSize (m_currentPacket, m_fragmentNumber + 1);
}

uint32_t
EdcaTxopN::GetFragmentOffset (void) 
{
  WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
  return station->GetFragmentOffset (m_currentPacket, m_fragmentNumber);
}

WifiRemoteStation *
EdcaTxopN::GetStation (Mac48Address ad) const
{
  return m_stationManager->Lookup (ad);
}

bool
EdcaTxopN::IsLastFragment (void) const
{
  WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
  return station->IsLastFragment (m_currentPacket, m_fragmentNumber);
}

Ptr<Packet>
EdcaTxopN::GetFragmentPacket (WifiMacHeader *hdr)
{
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

Mac48Address
EdcaTxopN::MapSrcAddressForAggregation (WifiMacHeader const &hdr)
{
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
EdcaTxopN::MapDestAddressForAggregation (WifiMacHeader const &hdr)
{
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
  m_aggregator = aggr;
}

} //namespace ns3
