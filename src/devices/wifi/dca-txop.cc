/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

#include "ns3/assert.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/node.h"
#include "ns3/uinteger.h"

#include "dca-txop.h"
#include "dcf-manager.h"
#include "mac-low.h"
#include "wifi-mac-queue.h"
#include "mac-tx-middle.h"
#include "wifi-mac-trailer.h"
#include "wifi-mac.h"
#include "random-stream.h"

NS_LOG_COMPONENT_DEFINE ("DcaTxop");

#define MY_DEBUG(x) \
  NS_LOG_DEBUG (m_low->GetAddress () << " " << x)


namespace ns3 {

class DcaTxop::Dcf : public DcfState
{
public:
  Dcf (DcaTxop *txop)
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

  DcaTxop *m_txop;
};

class DcaTxop::TransmissionListener : public MacLowTransmissionListener {
public:
  TransmissionListener (DcaTxop *txop)
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
  DcaTxop *m_txop;
};

TypeId 
DcaTxop::GetTypeId (void)
{
  static TypeId tid = TypeId ("DcaTxop")
    .SetParent<Object> ()
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
    ;
  return tid;
}

DcaTxop::DcaTxop ()
  : m_manager (0),
    m_currentPacket (0)
{
  NS_LOG_FUNCTION (this);
  m_transmissionListener = new DcaTxop::TransmissionListener (this);
  m_dcf = new DcaTxop::Dcf (this);
  m_queue = CreateObject<WifiMacQueue> ();
  m_rng = new RealRandomStream ();
  m_txMiddle = new MacTxMiddle ();
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
  delete m_transmissionListener;
  delete m_dcf;
  delete m_rng;
  delete m_txMiddle;
  m_transmissionListener = 0;
  m_dcf = 0;
  m_rng = 0;
  m_txMiddle = 0;
}

void
DcaTxop::SetManager (DcfManager *manager)
{
  NS_LOG_FUNCTION (this << manager);
  m_manager = manager;
  m_manager->Add (m_dcf);
}

void 
DcaTxop::SetLow (Ptr<MacLow> low)
{
  NS_LOG_FUNCTION (this << low);
  m_low = low;
}
void 
DcaTxop::SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> remoteManager)
{
  NS_LOG_FUNCTION (this << remoteManager);
  m_stationManager = remoteManager;
}
void 
DcaTxop::SetTxOkCallback (TxOk callback)
{
  m_txOkCallback = callback;
}
void 
DcaTxop::SetTxFailedCallback (TxFailed callback)
{
  m_txFailedCallback = callback;
}

void 
DcaTxop::SetMaxQueueSize (uint32_t size)
{
  NS_LOG_FUNCTION (this << size);
  m_queue->SetMaxSize (size);
}
void 
DcaTxop::SetMaxQueueDelay (Time delay)
{
  NS_LOG_FUNCTION (this << delay);
  m_queue->SetMaxDelay (delay);
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

void 
DcaTxop::Queue (Ptr<const Packet> packet, WifiMacHeader const &hdr)
{
  NS_LOG_FUNCTION (this << packet << &hdr);
  WifiMacTrailer fcs;
  uint32_t fullPacketSize = hdr.GetSerializedSize () + packet->GetSize () + fcs.GetSerializedSize ();
  WifiRemoteStation *station = GetStation (hdr.GetAddr1 ());
  station->PrepareForQueue (packet, fullPacketSize);
  m_queue->Enqueue (packet, hdr);
  StartAccessIfNeeded ();
}

WifiRemoteStation *
DcaTxop::GetStation (Mac48Address ad) const
{
  return m_stationManager->Lookup (ad);
}

void
DcaTxop::RestartAccessIfNeeded (void)
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
DcaTxop::StartAccessIfNeeded (void)
{
  NS_LOG_FUNCTION (this);
  if (m_currentPacket == 0 &&
      !m_queue->IsEmpty () &&
      !m_dcf->IsAccessRequested ())
    {
      m_manager->RequestAccess (m_dcf);
    }
}


Ptr<MacLow>
DcaTxop::Low (void)
{
  return m_low;
}

bool
DcaTxop::NeedRts (void)
{
  WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
  return station->NeedRts (m_currentPacket);
}

bool
DcaTxop::NeedRtsRetransmission (void)
{
  WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
  return station->NeedRtsRetransmission (m_currentPacket);
}

bool
DcaTxop::NeedDataRetransmission (void)
{
  WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
  return station->NeedDataRetransmission (m_currentPacket);
}
bool
DcaTxop::NeedFragmentation (void)
{
  WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
  return station->NeedFragmentation (m_currentPacket);
}

void
DcaTxop::NextFragment (void)
{
  m_fragmentNumber++;
}

uint32_t
DcaTxop::GetFragmentSize (void)
{
  WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
  return station->GetFragmentSize (m_currentPacket, m_fragmentNumber);
}
bool
DcaTxop::IsLastFragment (void) 
{
  WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
  return station->IsLastFragment (m_currentPacket, m_fragmentNumber);
}

uint32_t
DcaTxop::GetNextFragmentSize (void) 
{
  WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
  return station->GetFragmentSize (m_currentPacket, m_fragmentNumber + 1);
}

uint32_t
DcaTxop::GetFragmentOffset (void) 
{
  WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
  return station->GetFragmentOffset (m_currentPacket, m_fragmentNumber);
}

Ptr<Packet>
DcaTxop::GetFragmentPacket (WifiMacHeader *hdr)
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

bool 
DcaTxop::NeedsAccess (void) const
{
  return !m_queue->IsEmpty () || m_currentPacket != 0;
}
void 
DcaTxop::NotifyAccessGranted (void)
{
  NS_LOG_FUNCTION (this);
  if (m_currentPacket == 0) 
    {
      if (m_queue->IsEmpty ()) 
        {
          MY_DEBUG ("queue empty");
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
      Low ()->StartTransmission (m_currentPacket,
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
      
      if (NeedFragmentation ()) 
        {
          params.DisableRts ();
          WifiMacHeader hdr;
          Ptr<Packet> fragment = GetFragmentPacket (&hdr);
          if (IsLastFragment ()) 
            {
              MY_DEBUG ("fragmenting last fragment size="<<fragment->GetSize ());
              params.DisableNextData ();
            } 
          else 
            {
              MY_DEBUG ("fragmenting size="<<fragment->GetSize ());
              params.EnableNextData (GetNextFragmentSize ());
            }
          Low ()->StartTransmission (fragment, &hdr, params, 
                                     m_transmissionListener);
        } 
      else 
        {
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
          Low ()->StartTransmission (m_currentPacket, &m_currentHdr,
                                     params, m_transmissionListener);
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
  MY_DEBUG ("collision");
  m_dcf->StartBackoffNow (m_rng->GetNext (0, m_dcf->GetCw ()));
  RestartAccessIfNeeded ();
}

void 
DcaTxop::GotCts (double snr, WifiMode txMode)
{
  NS_LOG_FUNCTION (this << snr << txMode);
  MY_DEBUG ("got cts");
}
void 
DcaTxop::MissedCts (void)
{
  NS_LOG_FUNCTION (this);
  MY_DEBUG ("missed cts");
  if (!NeedRtsRetransmission ())
    {
      MY_DEBUG ("Cts Fail");
      WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
      station->ReportFinalRtsFailed ();
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
DcaTxop::GotAck (double snr, WifiMode txMode)
{
  NS_LOG_FUNCTION (this << snr << txMode);
  if (!NeedFragmentation () ||
      IsLastFragment ()) 
    {
      MY_DEBUG ("got ack. tx done.");
      if (!m_txOkCallback.IsNull ()) 
        {
          m_txOkCallback (m_currentHdr);
        }

      /* we are not fragmenting or we are done fragmenting
       * so we can get rid of that packet now.
       */
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
DcaTxop::MissedAck (void)
{
  NS_LOG_FUNCTION (this);
  MY_DEBUG ("missed ack");
  if (!NeedDataRetransmission ()) 
    {
      MY_DEBUG ("Ack Fail");
      WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
      station->ReportFinalDataFailed ();
      // to reset the dcf.    
      m_currentPacket = 0;
      m_dcf->ResetCw ();
    } 
  else 
    {
      MY_DEBUG ("Retransmit");
      m_currentHdr.SetRetry ();
      if (!m_txFailedCallback.IsNull ()) 
        {
          m_txFailedCallback (m_currentHdr);
        }
      m_dcf->UpdateFailedCw ();
    }
  m_dcf->StartBackoffNow (m_rng->GetNext (0, m_dcf->GetCw ()));
  RestartAccessIfNeeded ();
}
void 
DcaTxop::StartNext (void)
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
DcaTxop::Cancel (void)
{
  NS_LOG_FUNCTION (this);
  MY_DEBUG ("transmission cancelled");
  /**
   * This happens in only one case: in an AP, you have two DcaTxop:
   *   - one is used exclusively for beacons and has a high priority.
   *   - the other is used for everything else and has a normal
   *     priority.
   *
   * If the normal queue tries to send a unicast data frame, but 
   * if the tx fails (ack timeout), it starts a backoff. If the beacon
   * queue gets a tx oportunity during this backoff, it will trigger
   * a call to this Cancel function.
   *
   * Since we are already doing a backoff, we will get access to
   * the medium when we can, we have nothing to do here. We just 
   * ignore the cancel event and wait until we are given again a 
   * tx oportunity.
   *
   * Note that this is really non-trivial because each of these
   * frames is assigned a sequence number from the same sequence 
   * counter (because this is a non-802.11e device) so, the scheme
   * described here fails to ensure in-order delivery of frames
   * at the receiving side. This, however, does not matter in
   * this case because we assume that the receiving side does not
   * update its <seq,ad> tupple for packets whose destination
   * address is a broadcast address.
   */
}

} // namespace ns3
