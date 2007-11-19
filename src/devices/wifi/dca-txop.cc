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
#include "ns3/net-device.h"
#include "ns3/node.h"

#include "dca-txop.h"
#include "dcf-manager.h"
#include "mac-parameters.h"
#include "mac-low.h"
#include "wifi-mac-queue.h"
#include "mac-tx-middle.h"
#include "wifi-phy.h"
#include "random-stream.h"

NS_LOG_COMPONENT_DEFINE ("DcaTxop");

#define MY_DEBUG(x) \
  NS_LOG_DEBUG (Simulator::Now () << " " << m_low->GetDevice ()->GetNode ()->GetId () << ":" << \
                m_low->GetDevice ()->GetIfIndex () << " " << x)


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

DcaTxop::DcaTxop (uint32_t minCw, uint32_t maxCw, uint32_t aifsn, DcfManager *manager)
  : m_manager (manager),
    m_hasCurrent (false),
    m_ssrc (0),
    m_slrc (0)

{
  m_transmissionListener = new DcaTxop::TransmissionListener (this);
  m_dcf = new DcaTxop::Dcf (this);
  m_dcf->SetCwBounds (minCw, maxCw);
  m_dcf->SetAifsn (aifsn);
  m_manager->Add (m_dcf);
  m_queue = new WifiMacQueue ();
  m_rng = new RealRandomStream ();
}

DcaTxop::~DcaTxop ()
{
  delete m_transmissionListener;
  delete m_queue;
  delete m_dcf;
  m_transmissionListener = 0;
  m_queue = 0;
  m_dcf = 0;
}

void 
DcaTxop::SetLow (MacLow *low)
{
  m_low = low;
}
void 
DcaTxop::SetParameters (MacParameters *parameters)
{
  m_parameters = parameters;
}
void 
DcaTxop::SetTxMiddle (MacTxMiddle *txMiddle)
{
  m_txMiddle = txMiddle;
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
  m_queue->SetMaxSize (size);
}
void 
DcaTxop::SetMaxQueueDelay (Time delay)
{
  m_queue->SetMaxDelay (delay);
}

void 
DcaTxop::Queue (Packet packet, WifiMacHeader const &hdr)
{
  m_queue->Enqueue (packet, hdr);
  StartAccessIfNeeded ();
}

void
DcaTxop::RestartAccessIfNeeded (void)
{
  if ((m_hasCurrent ||
       !m_queue->IsEmpty ()) &&
      !m_dcf->IsAccessRequested ())
    {
      m_manager->RequestAccess (m_dcf);
    }
}

void
DcaTxop::StartAccessIfNeeded (void)
{
  if (!m_hasCurrent &&
      !m_queue->IsEmpty () &&
      !m_dcf->IsAccessRequested ())
    {
      m_manager->RequestAccess (m_dcf);
    }
}


MacLow *
DcaTxop::Low (void)
{
  return m_low;
}

MacParameters *
DcaTxop::Parameters (void)
{
  return m_parameters;
}


bool
DcaTxop::NeedRts (void)
{
  if (m_currentPacket.GetSize () > Parameters ()->GetRtsCtsThreshold ()) 
    {
      return true;
    } 
  else 
    {
      return false;
    }
}

bool
DcaTxop::NeedFragmentation (void)
{
  if (m_currentPacket.GetSize () > Parameters ()->GetFragmentationThreshold ()) 
    {
      return true;
    } 
  else 
    {
      return false;
    }
}

uint32_t
DcaTxop::GetNFragments (void)
{
  uint32_t nFragments = m_currentPacket.GetSize () / Parameters ()->GetFragmentationThreshold () + 1;
  return nFragments;
}
void
DcaTxop::NextFragment (void)
{
  m_fragmentNumber++;
}

uint32_t
DcaTxop::GetLastFragmentSize (void)
{
  uint32_t lastFragmentSize = m_currentPacket.GetSize () %
    Parameters ()->GetFragmentationThreshold ();
  return lastFragmentSize;
}

uint32_t
DcaTxop::GetFragmentSize (void)
{
  return Parameters ()->GetFragmentationThreshold ();
}
bool
DcaTxop::IsLastFragment (void) 
{
  if (m_fragmentNumber == (GetNFragments () - 1)) 
    {
      return true;
    } 
  else 
    {
      return false;
    }
}

uint32_t
DcaTxop::GetNextFragmentSize (void) 
{
  if (IsLastFragment ()) 
    {
      return 0;
    }
  
  uint32_t nextFragmentNumber = m_fragmentNumber + 1;
  if (nextFragmentNumber == (GetNFragments () - 1)) 
    {
      return GetLastFragmentSize ();
    } 
  else 
    {
      return GetFragmentSize ();
    }
}

Packet 
DcaTxop::GetFragmentPacket (WifiMacHeader *hdr)
{
  *hdr = m_currentHdr;
  hdr->SetFragmentNumber (m_fragmentNumber);
  uint32_t startOffset = m_fragmentNumber * GetFragmentSize ();
  Packet fragment;
  if (IsLastFragment ()) 
    {
      hdr->SetNoMoreFragments ();
      fragment = m_currentPacket.CreateFragment (startOffset, 
                                                 GetLastFragmentSize ());
    } 
  else 
    {
      hdr->SetMoreFragments ();
      fragment = m_currentPacket.CreateFragment (startOffset, 
                                                 GetFragmentSize ());
    }
  return fragment;
}

bool 
DcaTxop::NeedsAccess (void) const
{
  return !m_queue->IsEmpty () || m_hasCurrent;
}
void 
DcaTxop::NotifyAccessGranted (void)
{
  if (!m_hasCurrent) 
    {
      if (m_queue->IsEmpty ()) 
        {
          MY_DEBUG ("queue empty");
          return;
        }
      bool found;
      m_currentPacket = m_queue->Dequeue (&m_currentHdr, &found);
      NS_ASSERT (found);
      m_hasCurrent = true;
      NS_ASSERT (m_hasCurrent);
      uint16_t sequence = m_txMiddle->GetNextSequenceNumberfor (&m_currentHdr);
      m_currentHdr.SetSequenceNumber (sequence);
      m_currentHdr.SetFragmentNumber (0);
      m_currentHdr.SetNoMoreFragments ();
      m_ssrc = 0;
      m_slrc = 0;
      m_fragmentNumber = 0;
      MY_DEBUG ("dequeued size="<<m_currentPacket.GetSize ()<<
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
      m_hasCurrent = false;
      m_dcf->ResetCw ();
      m_dcf->StartBackoffNow (m_rng->GetNext (0, m_dcf->GetCw ()));
      MY_DEBUG ("tx broadcast");
    } 
  else 
    {
      params.EnableAck ();
      
      if (NeedFragmentation ()) 
        {
          params.DisableRts ();
          WifiMacHeader hdr;
          Packet fragment = GetFragmentPacket (&hdr);
          if (IsLastFragment ()) 
            {
              MY_DEBUG ("fragmenting last fragment size="<<fragment.GetSize ());
              params.DisableNextData ();
            } 
          else 
            {
              MY_DEBUG ("fragmenting size="<<fragment.GetSize ());
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
          // We need to make a copy in case we need to 
          // retransmit the packet: the MacLow modifies the input
          // Packet so, we would retransmit a modified packet
          // if we were not to make a copy.
          // XXX the comment above and the code below do not
          // make sense anymore. So, we should remove both.
          Packet copy = m_currentPacket;
          Low ()->StartTransmission (copy, &m_currentHdr,
                                     params, m_transmissionListener);
        }
    }
}

void 
DcaTxop::NotifyInternalCollision (void)
{
  NotifyCollision ();
}
void 
DcaTxop::NotifyCollision (void)
{
  MY_DEBUG ("collision");
  m_dcf->StartBackoffNow (m_rng->GetNext (0, m_dcf->GetCw ()));
  RestartAccessIfNeeded ();
}

void 
DcaTxop::GotCts (double snr, WifiMode txMode)
{
  MY_DEBUG ("got cts");
  m_ssrc = 0;
}
void 
DcaTxop::MissedCts (void)
{
  MY_DEBUG ("missed cts");
  m_ssrc++;
  m_ctstimeoutTrace (m_ssrc);
  if (m_ssrc > Parameters ()->GetMaxSsrc ()) 
    {
      // to reset the dcf.
      m_hasCurrent = false;
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
  m_slrc = 0;
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
      m_hasCurrent = false;
      m_dcf->ResetCw ();
      m_dcf->StartBackoffNow (m_rng->GetNext (0, m_dcf->GetCw ()));
      RestartAccessIfNeeded ();
    } 
  else 
    {
      MY_DEBUG ("got ack. tx not done, size="<<m_currentPacket.GetSize ());
    }
}
void 
DcaTxop::MissedAck (void)
{
  MY_DEBUG ("missed ack");
  m_slrc++;
  m_acktimeoutTrace (m_slrc);
  if (m_slrc > Parameters ()->GetMaxSlrc ()) 
    {
      // to reset the dcf.    
      m_hasCurrent = false;
      m_dcf->ResetCw ();
    } 
  else 
    {
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
  MY_DEBUG ("start next packet fragment");
  /* this callback is used only for fragments. */
  NextFragment ();
  WifiMacHeader hdr;
  Packet fragment = GetFragmentPacket (&hdr);
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
