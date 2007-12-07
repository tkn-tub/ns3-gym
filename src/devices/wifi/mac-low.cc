/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
#include "ns3/simulator.h"
#include "ns3/tag.h"
#include "ns3/log.h"
#include "ns3/node.h"

#include "mac-low.h"
#include "wifi-phy.h"
#include "wifi-mac-trailer.h"
#include "wifi-net-device.h"
#include "mac-stations.h"
#include "mac-parameters.h"

NS_LOG_COMPONENT_DEFINE ("MacLow");

#define MY_DEBUG(x) \
  NS_LOG_DEBUG (Simulator::Now () << " " << m_device->GetNode ()->GetId () << ":" << \
                m_device->GetIfIndex () << " " << x)

namespace ns3 {

class SnrTag : public Tag
{
public:
  SnrTag ();
  SnrTag (const SnrTag &o);
  ~SnrTag ();
  static uint32_t GetUid (void);
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator i) const;
  uint32_t Deserialize (Buffer::Iterator i);

  void Set (double snr);
  double Get (void) const;
private:
  double m_snr;
};

SnrTag::SnrTag ()
  : m_snr (0.0)
{}
SnrTag::SnrTag (const SnrTag &o)
  : m_snr (o.m_snr)
{}
SnrTag::~SnrTag ()
{}
uint32_t 
SnrTag::GetUid (void)
{
  static uint32_t uid = AllocateUid<SnrTag> ("SnrTag.ns3.inria.fr");
  return uid;
}
void 
SnrTag::Print (std::ostream &os) const
{
  os << "snr="<<m_snr;
}
uint32_t 
SnrTag::GetSerializedSize (void) const
{
  return 0;
}
void 
SnrTag::Serialize (Buffer::Iterator i) const
{
  // would need to serialize double to platform-independent format.
}
uint32_t 
SnrTag::Deserialize (Buffer::Iterator i)
{
  // would need to deserialize double from platform-independent format.
  return 0;
}
void 
SnrTag::Set (double snr)
{
  m_snr = snr;
}
double 
SnrTag::Get (void) const
{
  return m_snr;
}


MacLowTransmissionListener::MacLowTransmissionListener ()
{}
MacLowTransmissionListener::~MacLowTransmissionListener ()
{}
MacLowNavListener::MacLowNavListener ()
{}
MacLowNavListener::~MacLowNavListener ()
{}

MacLowTransmissionParameters::MacLowTransmissionParameters ()
  : m_nextSize (0),
    m_waitAck (ACK_NONE),
    m_sendRts (false),
    m_overrideDurationId (Seconds (0))
{}
void 
MacLowTransmissionParameters::EnableNextData (uint32_t size)
{
  m_nextSize = size;
}
void 
MacLowTransmissionParameters::DisableNextData (void)
{
  m_nextSize = 0;
}
void 
MacLowTransmissionParameters::EnableOverrideDurationId (Time durationId)
{
  m_overrideDurationId = durationId;
}
void 
MacLowTransmissionParameters::DisableOverrideDurationId (void)
{
  m_overrideDurationId = Seconds (0);
}
void 
MacLowTransmissionParameters::EnableSuperFastAck (void)
{
  m_waitAck = ACK_SUPER_FAST;
}
void 
MacLowTransmissionParameters::EnableFastAck (void)
{
  m_waitAck = ACK_FAST;
}
void 
MacLowTransmissionParameters::EnableAck (void)
{
  m_waitAck = ACK_NORMAL;
}
void 
MacLowTransmissionParameters::DisableAck (void)
{
  m_waitAck = ACK_NONE;
}
void 
MacLowTransmissionParameters::EnableRts (void)
{
  m_sendRts = true;
}
void 
MacLowTransmissionParameters::DisableRts (void)
{
  m_sendRts = false;
}
bool 
MacLowTransmissionParameters::MustWaitAck (void) const
{
  return (m_waitAck != ACK_NONE)?true:false;
}
bool 
MacLowTransmissionParameters::MustWaitNormalAck (void) const
{
  return (m_waitAck == ACK_NORMAL)?true:false;
}
bool 
MacLowTransmissionParameters::MustWaitFastAck (void) const
{
  return (m_waitAck == ACK_FAST)?true:false;
}
bool 
MacLowTransmissionParameters::MustWaitSuperFastAck (void) const
{
  return (m_waitAck == ACK_SUPER_FAST)?true:false;
}
bool 
MacLowTransmissionParameters::MustSendRts (void) const
{
  return m_sendRts;
}
bool 
MacLowTransmissionParameters::HasDurationId (void) const
{
  return (m_overrideDurationId != Seconds (0))?true:false;
}
Time
MacLowTransmissionParameters::GetDurationId (void) const
{
  NS_ASSERT (m_overrideDurationId != Seconds (0));
  return m_overrideDurationId;
}
bool 
MacLowTransmissionParameters::HasNextPacket (void) const
{
  return (m_nextSize != 0)?true:false;
}
uint32_t 
MacLowTransmissionParameters::GetNextPacketSize (void) const
{
  NS_ASSERT (HasNextPacket ());
  return m_nextSize;
}



MacLow::MacLow ()
  : m_normalAckTimeoutEvent (),
    m_fastAckTimeoutEvent (),
    m_superFastAckTimeoutEvent (),
    m_fastAckFailedTimeoutEvent (),
    m_ctsTimeoutEvent (),
    m_sendCtsEvent (),
    m_sendAckEvent (),
    m_sendDataEvent (),
    m_waitSifsEvent (),
    m_currentPacket (0)
{
  m_lastNavDuration = Seconds (0);
  m_lastNavStart = Seconds (0);
}

MacLow::~MacLow ()
{
  CancelAllEvents ();
  m_phy = 0;
}

void
MacLow::CancelAllEvents (void)
{
  bool oneRunning = false;
  if (m_normalAckTimeoutEvent.IsRunning ()) 
    {
      m_normalAckTimeoutEvent.Cancel ();
      oneRunning = true;
    }
  if (m_fastAckTimeoutEvent.IsRunning ()) 
    {
      m_fastAckTimeoutEvent.Cancel ();
      oneRunning = true;
    }
  if (m_superFastAckTimeoutEvent.IsRunning ()) 
    {
      m_superFastAckTimeoutEvent.Cancel ();
      oneRunning = true;
    }
  if (m_fastAckFailedTimeoutEvent.IsRunning ()) 
    {
      m_fastAckFailedTimeoutEvent.Cancel ();
      oneRunning = true;
    }
  if (m_ctsTimeoutEvent.IsRunning ()) 
    {
      m_ctsTimeoutEvent.Cancel ();
      oneRunning = true;
    }
  if (m_sendCtsEvent.IsRunning ()) 
    {
      m_sendCtsEvent.Cancel ();
      oneRunning = true;
    }
  if (m_sendAckEvent.IsRunning ()) 
    {
      m_sendAckEvent.Cancel ();
      oneRunning = true;
    }
  if (m_sendDataEvent.IsRunning ()) 
    {
      m_sendDataEvent.Cancel ();
      oneRunning = true;
    }
  if (m_waitSifsEvent.IsRunning ()) 
    {
      m_waitSifsEvent.Cancel ();
      oneRunning = true;
    }
  if (oneRunning && m_listener != 0) 
    {
      m_listener->Cancel ();
    }
}

/****************************************************************************
 *  API methods below.
 ****************************************************************************/

void
MacLow::SetDevice (Ptr<WifiNetDevice> device)
{
  m_device = device;
}
Ptr<NetDevice> 
MacLow::GetDevice (void) const
{
  return m_device;
}
void
MacLow::SetPhy (Ptr<WifiPhy> phy)
{
  m_phy = phy;
}
void 
MacLow::SetParameters (MacParameters *parameters)
{
  m_parameters = parameters;
}
void 
MacLow::SetStations (MacStations *stations)
{
  m_stations = stations;
}
void 
MacLow::SetRxCallback (MacLowRxCallback callback)
{
  m_rxCallback = callback;
}
void 
MacLow::RegisterNavListener (MacLowNavListener *listener)
{
  m_navListeners.push_back (listener);
}


void 
MacLow::StartTransmission (Ptr<const Packet> packet, 
                           WifiMacHeader const*hdr, 
                           MacLowTransmissionParameters parameters,
                           MacLowTransmissionListener *listener)
{
  /* m_currentPacket is not NULL because someone started
   * a transmission and was interrupted before one of:
   *   - ctsTimeout
   *   - sendDataAfterCTS
   * expired. This means that one of these timers is still
   * running. They are all cancelled below anyway by the 
   * call to CancelAllEvents (because of at least one
   * of these two timer) which will trigger a call to the
   * previous listener's cancel method.
   *
   * This typically happens because the high-priority 
   * QapScheduler has taken access to the channel from
   * one of the Edca of the QAP.
   */
  m_currentPacket = packet->Copy ();
  m_currentHdr = *hdr;
  CancelAllEvents ();
  m_listener = listener;
  m_txParams = parameters;

  //NS_ASSERT (m_phy->IsStateIdle ());

  MY_DEBUG ("startTx size="<< GetCurrentSize () << ", to=" << m_currentHdr.GetAddr1()<<", listener="<<m_listener);

  if (m_txParams.MustSendRts ()) 
    {
      SendRtsForPacket ();
    } 
  else 
    {
      SendDataPacket ();
    }

  /* When this method completes, we have taken ownership of the medium. */
  NS_ASSERT (m_phy->IsStateTx ());  
}

void
MacLow::ReceiveError (Ptr<Packet> packet, double rxSnr)
{
  MY_DEBUG ("rx failed ");
  m_dropError (packet);
  if (m_txParams.MustWaitFastAck ()) 
    {
      NS_ASSERT (m_fastAckFailedTimeoutEvent.IsExpired ());
      m_fastAckFailedTimeoutEvent = Simulator::Schedule (GetSifs (), 
                                                         &MacLow::FastAckFailedTimeout, this);
    }
  return;
}

void 
MacLow::ReceiveOk (Ptr<Packet> packet, double rxSnr, WifiMode txMode, WifiPreamble preamble)
{
  /* A packet is received from the PHY.
   * When we have handled this packet,
   * we handle any packet present in the
   * packet queue.
   */
  WifiMacHeader hdr;
  packet->RemoveHeader (hdr);
  
  bool isPrevNavZero = IsNavZero ();
  MY_DEBUG ("duration/id=" << hdr.GetDuration ());
  NotifyNav (hdr, txMode, preamble);
  if (hdr.IsRts ()) 
    {
      /* see section 9.2.5.7 802.11-1999
       * A STA that is addressed by an RTS frame shall transmit a CTS frame after a SIFS 
       * period if the NAV at the STA receiving the RTS frame indicates that the medium is 
       * idle. If the NAV at the STA receiving the RTS indicates the medium is not idle, 
       * that STA shall not respond to the RTS frame.
       */
      if (isPrevNavZero &&
          hdr.GetAddr1 () == m_device->GetSelfAddress ()) 
        {
          MY_DEBUG ("rx RTS from=" << hdr.GetAddr2 () << ", schedule CTS");
          NS_ASSERT (m_sendCtsEvent.IsExpired ());
          MacStation *station = m_stations->Lookup (hdr.GetAddr2 ());
          station->ReportRxOk (rxSnr, txMode);
          m_sendCtsEvent = Simulator::Schedule (GetSifs (),
                                                &MacLow::SendCtsAfterRts, this,
                                                hdr.GetAddr2 (), 
                                                hdr.GetDuration (),
                                                txMode,
                                                rxSnr);
        } 
      else 
        {
          MY_DEBUG ("rx RTS from=" << hdr.GetAddr2 () << ", cannot schedule CTS");
        }
    } 
  else if (hdr.IsCts () &&
           hdr.GetAddr1 () == m_device->GetSelfAddress () &&
           m_ctsTimeoutEvent.IsRunning () &&
           m_currentPacket != 0) 
    {
      MY_DEBUG ("receive cts from="<<m_currentHdr.GetAddr1 ());
      SnrTag tag;
      packet->PeekTag (tag);
      MacStation *station = GetStation (m_currentHdr.GetAddr1 ());
      station->ReportRxOk (rxSnr, txMode);
      station->ReportRtsOk (rxSnr, txMode, tag.Get ());
      
      m_ctsTimeoutEvent.Cancel ();
      m_listener->GotCts (rxSnr, txMode);
      NS_ASSERT (m_sendDataEvent.IsExpired ());
      m_sendDataEvent = Simulator::Schedule (GetSifs (), 
                                             &MacLow::SendDataAfterCts, this, 
                                             hdr.GetAddr1 (),
                                             hdr.GetDuration (),
                                             txMode);
    } 
  else if (hdr.IsAck () &&
           hdr.GetAddr1 () == m_device->GetSelfAddress () &&
           (m_normalAckTimeoutEvent.IsRunning () || 
            m_fastAckTimeoutEvent.IsRunning () ||
            m_superFastAckTimeoutEvent.IsRunning ()) &&
           m_txParams.MustWaitAck ()) 
    {
      MY_DEBUG ("receive ack from="<<m_currentHdr.GetAddr1 ());
      SnrTag tag;
      packet->PeekTag (tag);
      MacStation *station = GetStation (m_currentHdr.GetAddr1 ());
      station->ReportRxOk (rxSnr, txMode);
      station->ReportDataOk (rxSnr, txMode, tag.Get ());
      bool gotAck = false;
      if (m_txParams.MustWaitNormalAck () &&
          m_normalAckTimeoutEvent.IsRunning ()) 
        {
          m_normalAckTimeoutEvent.Cancel ();
          gotAck = true;
        }
      if (m_txParams.MustWaitFastAck () &&
          m_fastAckTimeoutEvent.IsRunning ()) 
        {
          m_fastAckTimeoutEvent.Cancel ();
          gotAck = true;
        }
      if (gotAck) 
        {
          m_listener->GotAck (rxSnr, txMode);
        }
    if (m_txParams.HasNextPacket ()) 
      {
        m_waitSifsEvent = Simulator::Schedule (GetSifs (), 
                                               &MacLow::WaitSifsAfterEndTx, this);
      }
    } 
  else if (hdr.IsCtl ()) 
    {
      MY_DEBUG ("rx drop " << hdr.GetTypeString ());
    } 
  else if (hdr.GetAddr1 () == m_device->GetSelfAddress ()) 
    {
      MacStation *station = GetStation (hdr.GetAddr2 ());
      station->ReportRxOk (rxSnr, txMode);
      
      if (hdr.IsQosData () && hdr.IsQosNoAck ()) 
        {
          MY_DEBUG ("rx unicast/noAck from="<<hdr.GetAddr2 ());
        } 
      else if (hdr.IsData () || hdr.IsMgt ()) 
        {
          MY_DEBUG ("rx unicast/sendAck from=" << hdr.GetAddr2 ());
          NS_ASSERT (m_sendAckEvent.IsExpired ());
          m_sendAckEvent = Simulator::Schedule (GetSifs (),
                                                &MacLow::SendAckAfterData, this,
                                                hdr.GetAddr2 (), 
                                                hdr.GetDuration (),
                                                txMode,
                                                rxSnr);
        }
      goto rxPacket;
    } 
  else if (hdr.GetAddr1 ().IsBroadcast ()) 
    {
      if (hdr.IsData () || hdr.IsMgt ()) 
        {
          MY_DEBUG ("rx broadcast from=" << hdr.GetAddr2 ());
          goto rxPacket;
        } 
      else 
        {
          // DROP.
        }
    } 
  else 
    {
      //MY_DEBUG_VERBOSE ("rx not-for-me from %d", GetSource (packet));
    }
  return;
 rxPacket:
  WifiMacTrailer fcs;
  packet->RemoveTrailer (fcs);
  m_rxCallback (packet, &hdr);
  return;
}

uint32_t 
MacLow::GetAckSize (void) const
{
  WifiMacHeader ack;
  ack.SetType (WIFI_MAC_CTL_ACK);
  return ack.GetSize () + 4;
}
uint32_t 
MacLow::GetRtsSize (void) const
{
  WifiMacHeader rts;
  rts.SetType (WIFI_MAC_CTL_RTS);
  return rts.GetSize () + 4;
}
Time
MacLow::GetAckDuration (Mac48Address to, WifiMode dataTxMode) const
{
  WifiMode ackMode = GetAckTxModeForData (to, dataTxMode);
  return m_phy->CalculateTxDuration (GetAckSize (), ackMode, WIFI_PREAMBLE_LONG);
}
Time
MacLow::GetCtsDuration (Mac48Address to, WifiMode rtsTxMode) const
{
  WifiMode ctsMode = GetCtsTxModeForRts (to, rtsTxMode);
  return m_phy->CalculateTxDuration (GetCtsSize (), ctsMode, WIFI_PREAMBLE_LONG);
}
uint32_t 
MacLow::GetCtsSize (void) const
{
  WifiMacHeader cts;
  cts.SetType (WIFI_MAC_CTL_CTS);
  return cts.GetSize () + 4;
}
Time
MacLow::GetSifs (void) const
{
  return m_parameters->GetSifs ();
}
Time
MacLow::GetPifs (void) const
{
  return m_parameters->GetPifs ();
}
Time
MacLow::GetAckTimeout (void) const
{
  return m_parameters->GetAckTimeout ();
}
Time
MacLow::GetCtsTimeout (void) const
{
  return m_parameters->GetCtsTimeout ();
}
uint32_t 
MacLow::GetCurrentSize (void) const
{
  WifiMacTrailer fcs;
  return m_currentPacket->GetSize () + m_currentHdr.GetSize () + fcs.GetSerializedSize ();
}

WifiMode
MacLow::GetRtsTxMode (Mac48Address to) const
{
  return GetStation (to)->GetRtsMode ();
}
WifiMode
MacLow::GetDataTxMode (Mac48Address to, uint32_t size) const
{
  return GetStation (to)->GetDataMode (size);
}

WifiMode
MacLow::GetCtsTxModeForRts (Mac48Address to, WifiMode rtsTxMode) const
{
  return GetStation (to)->GetCtsMode (rtsTxMode);
}
WifiMode
MacLow::GetAckTxModeForData (Mac48Address to, WifiMode dataTxMode) const
{
  return GetStation (to)->GetAckMode (dataTxMode);
}


Time
MacLow::CalculateOverallTxTime (uint32_t dataSize, Mac48Address to, 
                                MacLowTransmissionParameters const& params) const
{
  Time txTime = Seconds (0);
  WifiMode rtsMode = GetRtsTxMode (to);
  WifiMode dataMode = GetDataTxMode (to, dataSize);
  if (params.MustSendRts ()) 
    {
      txTime += m_phy->CalculateTxDuration (GetRtsSize (), rtsMode, WIFI_PREAMBLE_LONG);
      txTime += GetCtsDuration (m_currentHdr.GetAddr1 (), rtsMode);
      txTime += GetSifs () * Scalar (2);
    }
  txTime += m_phy->CalculateTxDuration (dataSize, dataMode, WIFI_PREAMBLE_LONG);
  if (params.MustWaitAck ())
    {
      txTime += GetSifs ();
      txTime += GetAckDuration (m_currentHdr.GetAddr1 (), dataMode);
    }
  return txTime;
}

Time
MacLow::CalculateTransmissionTime (uint32_t dataSize, Mac48Address to, 
                                   MacLowTransmissionParameters const& params) const
{
  Time txTime = CalculateOverallTxTime (dataSize, to, params);
  if (params.HasNextPacket ()) 
    {
      WifiMode dataMode = GetDataTxMode (to, dataSize );
      txTime += GetSifs ();
      txTime += m_phy->CalculateTxDuration (params.GetNextPacketSize (), dataMode, WIFI_PREAMBLE_LONG);
    }
  return txTime;
}

void
MacLow::NotifyNav (const WifiMacHeader &hdr, WifiMode txMode, WifiPreamble preamble)
{
  NS_ASSERT (m_lastNavStart <= Simulator::Now ());
  Time duration = hdr.GetDuration ();

  if (hdr.IsCfpoll () &&
      hdr.GetAddr2 () == m_device->GetBssid ()) 
    {
      // see section 9.3.2.2 802.11-1999
      DoNavResetNow (duration);
      return;
    }
  // XXX Note that we should also handle CF_END specially here
  // but we don't for now because we do not generate them.
  else if (hdr.GetAddr1 () == m_device->GetSelfAddress ())
    {
      // see section 9.2.5.4 802.11-1999
      bool navUpdated = DoNavStartNow (duration);
      if (hdr.IsRts () && navUpdated)
        {
          /**
           * A STA that used information from an RTS frame as the most recent basis to update its NAV setting 
           * is permitted to reset its NAV if no PHY-RXSTART.indication is detected from the PHY during a 
           * period with a duration of (2 * aSIFSTime) + (CTS_Time) + (2 * aSlotTime) starting at the 
           * PHY-RXEND.indication corresponding to the detection of the RTS frame. The “CTS_Time” shall 
           * be calculated using the length of the CTS frame and the data rate at which the RTS frame 
           * used for the most recent NAV update was received.
           */
          WifiMacHeader cts;
          cts.SetType (WIFI_MAC_CTL_CTS);
          Time navCounterResetCtsMissedDelay = 
            m_phy->CalculateTxDuration (cts.GetSerializedSize (), txMode, preamble) +
            Scalar (2) * m_parameters->GetSifs () + Scalar (2) * m_parameters->GetSlotTime ();
          m_navCounterResetCtsMissed = Simulator::Schedule (navCounterResetCtsMissedDelay,
                                                            &MacLow::NavCounterResetCtsMissed, this,
                                                            Simulator::Now ());
        }
    }
}

void
MacLow::NavCounterResetCtsMissed (Time rtsEndRxTime)
{
  if (m_phy->GetLastRxStartTime () > rtsEndRxTime)
    {
      DoNavResetNow (Seconds (0.0));
    }
}

void
MacLow::DoNavResetNow (Time duration)
{
  for (NavListenersCI i = m_navListeners.begin (); i != m_navListeners.end (); i++) 
    {
      (*i)->NavReset (duration);
    }
  m_lastNavStart = Simulator::Now ();
  m_lastNavStart = duration;
}
bool
MacLow::DoNavStartNow (Time duration)
{
  for (NavListenersCI i = m_navListeners.begin (); i != m_navListeners.end (); i++) 
    {
      (*i)->NavStart (duration);
    }
  Time newNavEnd = Simulator::Now () + duration;
  Time oldNavEnd = m_lastNavStart + m_lastNavDuration;
  if (newNavEnd > oldNavEnd)
    {
      m_lastNavStart = Simulator::Now ();
      m_lastNavDuration = duration;
      return true;
    }
  return false;
}

void
MacLow::ForwardDown (Ptr<const Packet> packet, WifiMacHeader const* hdr, 
                     WifiMode txMode)
{
  MY_DEBUG ("send " << hdr->GetTypeString () <<
            ", to=" << hdr->GetAddr1 () <<
            ", size=" << packet->GetSize () <<
            ", mode=" << txMode <<
            ", duration=" << hdr->GetDuration () <<
            ", seq=0x"<< std::hex << m_currentHdr.GetSequenceControl () << std::dec);
  m_phy->SendPacket (packet, txMode, WIFI_PREAMBLE_LONG, 0);
  /* 
   * We have to notify the NAV of transmitted packets because of the 802.11e
   * requirement from section 9.9.1.4 that each EDCAF update its NAV from the
   * transmission of any other EDCAF within the same QSTA.
   */
  Time txDuration = m_phy->CalculateTxDuration (packet->GetSize (), txMode, WIFI_PREAMBLE_LONG);
  Simulator::Schedule (txDuration, &MacLow::NotifyNav, this, *hdr, txMode, WIFI_PREAMBLE_LONG);
}

void
MacLow::CtsTimeout (void)
{
  MY_DEBUG ("cts timeout");
  // XXX: should check that there was no rx start before now.
  // we should restart a new cts timeout now until the expected
  // end of rx if there was a rx start before now.
  MacStation *station = GetStation (m_currentHdr.GetAddr1 ());
  station->ReportRtsFailed ();
  m_currentPacket = 0;
  MacLowTransmissionListener *listener = m_listener;
  m_listener = 0;
  listener->MissedCts ();
}
void
MacLow::NormalAckTimeout (void)
{
  MY_DEBUG ("normal ack timeout");
  // XXX: should check that there was no rx start before now.
  // we should restart a new ack timeout now until the expected
  // end of rx if there was a rx start before now.
  MacStation *station = GetStation (m_currentHdr.GetAddr1 ());
  station->ReportDataFailed ();
  MacLowTransmissionListener *listener = m_listener;
  m_listener = 0;
  listener->MissedAck ();
}
void
MacLow::FastAckTimeout (void)
{
  MacStation *station = GetStation (m_currentHdr.GetAddr1 ());
  station->ReportDataFailed ();
  MacLowTransmissionListener *listener = m_listener;
  m_listener = 0;
  if (m_phy->IsStateIdle ()) 
    {
      MY_DEBUG ("fast Ack idle missed");
      listener->MissedAck ();
    }
  else
    {
      MY_DEBUG ("fast Ack ok");
    }
}
void
MacLow::SuperFastAckTimeout ()
{
  MacStation *station = GetStation (m_currentHdr.GetAddr1 ());
  station->ReportDataFailed ();
  MacLowTransmissionListener *listener = m_listener;
  m_listener = 0;
  if (m_phy->IsStateIdle ()) 
    {
      MY_DEBUG ("super fast Ack failed");
      listener->MissedAck ();
    } 
  else 
    {
      MY_DEBUG ("super fast Ack ok");
      listener->GotAck (0.0, WifiMode ());
    }
}

void
MacLow::SendRtsForPacket (void)
{
  /* send an RTS for this packet. */
  WifiMacHeader rts;
  rts.SetType (WIFI_MAC_CTL_RTS);
  rts.SetDsNotFrom ();
  rts.SetDsNotTo ();
  rts.SetAddr1 (m_currentHdr.GetAddr1 ());
  rts.SetAddr2 (m_device->GetSelfAddress ());
  WifiMode rtsTxMode = GetRtsTxMode (m_currentHdr.GetAddr1 ());
  Time duration = Seconds (0);
  if (m_txParams.HasDurationId ()) 
    {
      duration += m_txParams.GetDurationId ();
    } 
  else 
    {
      WifiMode dataTxMode = GetDataTxMode (m_currentHdr.GetAddr1 (), GetCurrentSize ());
      duration += GetSifs ();
      duration += GetCtsDuration (m_currentHdr.GetAddr1 (), rtsTxMode);
      duration += GetSifs ();
      duration += m_phy->CalculateTxDuration (GetCurrentSize (), dataTxMode, WIFI_PREAMBLE_LONG);
      duration += GetSifs ();
      duration += GetAckDuration (m_currentHdr.GetAddr1 (), dataTxMode);
    }
  rts.SetDuration (duration);

  Time txDuration = m_phy->CalculateTxDuration (GetRtsSize (), rtsTxMode, WIFI_PREAMBLE_LONG);
  Time timerDelay = txDuration + GetCtsTimeout ();

  NS_ASSERT (m_ctsTimeoutEvent.IsExpired ());
  m_ctsTimeoutEvent = Simulator::Schedule (timerDelay, &MacLow::CtsTimeout, this);

  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (rts);
  WifiMacTrailer fcs;
  packet->AddTrailer (fcs);

  ForwardDown (packet, &rts, rtsTxMode);
}

void
MacLow::StartDataTxTimers (void)
{
  WifiMode dataTxMode = GetDataTxMode (m_currentHdr.GetAddr1 (), GetCurrentSize ());
  Time txDuration = m_phy->CalculateTxDuration (GetCurrentSize (), dataTxMode, WIFI_PREAMBLE_LONG);
  if (m_txParams.MustWaitNormalAck ()) 
    {
      Time timerDelay = txDuration + GetAckTimeout ();
      NS_ASSERT (m_normalAckTimeoutEvent.IsExpired ());
      m_normalAckTimeoutEvent = Simulator::Schedule (timerDelay, &MacLow::NormalAckTimeout, this);
    } 
  else if (m_txParams.MustWaitFastAck ()) 
    {
      Time timerDelay = txDuration + GetPifs ();
      NS_ASSERT (m_fastAckTimeoutEvent.IsExpired ());
      m_fastAckTimeoutEvent = Simulator::Schedule (timerDelay, &MacLow::FastAckTimeout, this);
    } 
  else if (m_txParams.MustWaitSuperFastAck ()) 
    {
      Time timerDelay = txDuration + GetPifs ();
      NS_ASSERT (m_superFastAckTimeoutEvent.IsExpired ());
      m_superFastAckTimeoutEvent = Simulator::Schedule (timerDelay, 
                                                        &MacLow::SuperFastAckTimeout, this);
    } 
  else if (m_txParams.HasNextPacket ()) 
    {
      Time delay = txDuration + GetSifs ();
      NS_ASSERT (m_waitSifsEvent.IsExpired ());
      m_waitSifsEvent = Simulator::Schedule (delay, &MacLow::WaitSifsAfterEndTx, this);
    } 
  else 
    {
      // since we do not expect any timer to be triggered.
      m_listener = 0;
    }
}

void
MacLow::SendDataPacket (void)
{
  /* send this packet directly. No RTS is needed. */
  StartDataTxTimers ();

  WifiMode dataTxMode = GetDataTxMode (m_currentHdr.GetAddr1 (), GetCurrentSize ());
  Time duration = Seconds (0.0);
  if (m_txParams.HasDurationId ()) 
    {
      duration += m_txParams.GetDurationId ();
    } 
  else 
    {
      if (m_txParams.MustWaitAck ()) 
        {
          duration += GetSifs ();
          duration += GetAckDuration (m_currentHdr.GetAddr1 (), dataTxMode);
        }
      if (m_txParams.HasNextPacket ()) 
        {
          duration += GetSifs ();
          duration += m_phy->CalculateTxDuration (m_txParams.GetNextPacketSize (), 
                                                  dataTxMode, WIFI_PREAMBLE_LONG);
          if (m_txParams.MustWaitAck ()) 
            {
              duration += GetSifs ();
              duration += GetAckDuration (m_currentHdr.GetAddr1 (), dataTxMode);
            }
        }
    }
  m_currentHdr.SetDuration (duration);

  m_currentPacket->AddHeader (m_currentHdr);
  WifiMacTrailer fcs;
  m_currentPacket->AddTrailer (fcs);

  ForwardDown (m_currentPacket, &m_currentHdr, dataTxMode);
  m_currentPacket = 0;
}

bool 
MacLow::IsNavZero (void) const
{
  if (m_lastNavStart + m_lastNavDuration < Simulator::Now ()) 
    {
      return true;
    } 
  else 
    {
      return false;
    }
}

MacStation *
MacLow::GetStation (Mac48Address ad) const
{
  return m_stations->Lookup (ad);
}

void
MacLow::SendCtsAfterRts (Mac48Address source, Time duration, WifiMode rtsTxMode, double rtsSnr)
{
  /* send a CTS when you receive a RTS 
   * right after SIFS.
   */
  WifiMode ctsTxMode = GetCtsTxModeForRts (source, rtsTxMode);
  WifiMacHeader cts;
  cts.SetType (WIFI_MAC_CTL_CTS);
  cts.SetDsNotFrom ();
  cts.SetDsNotTo ();
  cts.SetAddr1 (source);
  duration -= GetCtsDuration (source, rtsTxMode);
  duration -= GetSifs ();
  NS_ASSERT (duration >= MicroSeconds (0));
  cts.SetDuration (duration);

  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (cts);
  WifiMacTrailer fcs;
  packet->AddTrailer (fcs);

  struct SnrTag tag;
  tag.Set (rtsSnr);
  packet->AddTag (tag);

  ForwardDown (packet, &cts, ctsTxMode);
}

void
MacLow::SendDataAfterCts (Mac48Address source, Time duration, WifiMode txMode)
{
  /* send the third step in a 
   * RTS/CTS/DATA/ACK hanshake 
   */
  NS_ASSERT (m_currentPacket != 0);
  StartDataTxTimers ();

  WifiMode dataTxMode = GetDataTxMode (m_currentHdr.GetAddr1 (), GetCurrentSize ());
  Time newDuration = Seconds (0);
  newDuration += GetSifs ();
  newDuration += GetAckDuration (m_currentHdr.GetAddr1 (), dataTxMode);
  Time txDuration = m_phy->CalculateTxDuration (GetCurrentSize (), dataTxMode, WIFI_PREAMBLE_LONG);
  duration -= txDuration;
  duration -= GetSifs ();

  duration = std::max (duration, newDuration);
  NS_ASSERT (duration >= MicroSeconds (0));
  m_currentHdr.SetDuration (duration);

  m_currentPacket->AddHeader (m_currentHdr);
  WifiMacTrailer fcs;
  m_currentPacket->AddTrailer (fcs);

  ForwardDown (m_currentPacket, &m_currentHdr, dataTxMode);
  m_currentPacket = 0;
}

void 
MacLow::WaitSifsAfterEndTx (void)
{
  m_listener->StartNext ();
}

void
MacLow::FastAckFailedTimeout (void)
{
  MacLowTransmissionListener *listener = m_listener;
  m_listener = 0;
  listener->MissedAck ();
  MY_DEBUG ("fast Ack busy but missed");
}

void
MacLow::SendAckAfterData (Mac48Address source, Time duration, WifiMode dataTxMode, double dataSnr)
{
  /* send an ACK when you receive 
   * a packet after SIFS. 
   */
  WifiMode ackTxMode = GetAckTxModeForData (source, dataTxMode);
  WifiMacHeader ack;
  ack.SetType (WIFI_MAC_CTL_ACK);
  ack.SetDsNotFrom ();
  ack.SetDsNotTo ();
  ack.SetAddr1 (source);
  duration -= GetAckDuration (source, dataTxMode);
  duration -= GetSifs ();
  NS_ASSERT (duration >= MicroSeconds (0));
  ack.SetDuration (duration);

  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (ack);
  WifiMacTrailer fcs;
  packet->AddTrailer (fcs);

  struct SnrTag tag;
  tag.Set (dataSnr);
  packet->AddTag (tag);

  ForwardDown (packet, &ack, ackTxMode);
}

} // namespace ns3
