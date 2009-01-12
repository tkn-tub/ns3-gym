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

NS_LOG_COMPONENT_DEFINE ("MacLow");

#undef NS_LOG_APPEND_CONTEXT
#define NS_LOG_APPEND_CONTEXT std::clog << "[mac=" << m_self << "] "


namespace ns3 {

class SnrTag : public Tag
{
public:

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual void Print (std::ostream &os) const;

  void Set (double snr);
  double Get (void) const;
private:
  double m_snr;
};

TypeId 
SnrTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SnrTag")
    .SetParent<Tag> ()
    .AddConstructor<SnrTag> ()
    .AddAttribute ("Snr", "The snr of the last packet received",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&SnrTag::Get),
                   MakeDoubleChecker<double> ())
    ;
  return tid;
}
TypeId 
SnrTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t 
SnrTag::GetSerializedSize (void) const
{
  return sizeof (double);
}
void 
SnrTag::Serialize (TagBuffer i) const
{
  i.WriteDouble (m_snr);
}
void 
SnrTag::Deserialize (TagBuffer i)
{
  m_snr = i.ReadDouble ();
}
void 
SnrTag::Print (std::ostream &os) const
{
  os << "Snr=" << m_snr;
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

std::ostream &operator << (std::ostream &os, const MacLowTransmissionParameters &params)
{
  os << "[" 
     << "send rts=" << params.m_sendRts << ", "
     << "next size=" << params.m_nextSize << ", "
     << "dur=" << params.m_overrideDurationId << ", "
     << "ack=";
  switch (params.m_waitAck) {
  case MacLowTransmissionParameters::ACK_NONE:
    os << "none";
    break;
  case MacLowTransmissionParameters::ACK_NORMAL:
    os << "normal";
    break;
  case MacLowTransmissionParameters::ACK_FAST:
    os << "fast";
    break;
  case MacLowTransmissionParameters::ACK_SUPER_FAST:
    os << "super-fast";
    break;
  }
  os << "]";
  return os;
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
    m_currentPacket (0),
    m_listener (0)
{
  NS_LOG_FUNCTION (this);
  m_lastNavDuration = Seconds (0);
  m_lastNavStart = Seconds (0);
}

MacLow::~MacLow ()
{
  NS_LOG_FUNCTION (this);
}

void 
MacLow::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  CancelAllEvents ();
  m_phy = 0;
  m_stationManager = 0;
}

void
MacLow::CancelAllEvents (void)
{
  NS_LOG_FUNCTION (this);
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
      m_listener = 0;
    }
}

void
MacLow::SetPhy (Ptr<WifiPhy> phy)
{
  m_phy = phy;
  m_phy->SetReceiveOkCallback (MakeCallback (&MacLow::ReceiveOk, this));
  m_phy->SetReceiveErrorCallback (MakeCallback (&MacLow::ReceiveError, this));
}
void 
MacLow::SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> manager)
{
  m_stationManager = manager;
}

void 
MacLow::SetAddress (Mac48Address ad)
{
  m_self = ad;
}
void 
MacLow::SetAckTimeout (Time ackTimeout)
{
  m_ackTimeout = ackTimeout;
}
void 
MacLow::SetCtsTimeout (Time ctsTimeout)
{
  m_ctsTimeout = ctsTimeout;
}
void
MacLow::SetSifs (Time sifs)
{
  m_sifs = sifs;
}
void 
MacLow::SetSlotTime (Time slotTime)
{
  m_slotTime = slotTime;
}
void 
MacLow::SetPifs (Time pifs)
{
  m_pifs = pifs;
}
void
MacLow::SetBssid (Mac48Address bssid)
{
  m_bssid = bssid;
}
Mac48Address 
MacLow::GetAddress (void) const
{
  return m_self;
}
Time 
MacLow::GetAckTimeout (void) const
{
  return m_ackTimeout;
}
Time 
MacLow::GetCtsTimeout (void) const
{
  return m_ctsTimeout;
}
Time
MacLow::GetSifs (void) const
{
  return m_sifs;
}
Time 
MacLow::GetSlotTime (void) const
{
  return m_slotTime;
}
Time 
MacLow::GetPifs (void) const
{
  return m_pifs;
}
Mac48Address 
MacLow::GetBssid (void) const
{
  return m_bssid;
}

void 
MacLow::SetRxCallback (Callback<void,Ptr<Packet>,const WifiMacHeader *> callback)
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
                           MacLowTransmissionParameters params,
                           MacLowTransmissionListener *listener)
{
  NS_LOG_FUNCTION (this << packet << hdr << params << listener);
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
  m_txParams = params;

  //NS_ASSERT (m_phy->IsStateIdle ());

  NS_LOG_DEBUG ("startTx size="<< GetSize (m_currentPacket, &m_currentHdr) << 
            ", to=" << m_currentHdr.GetAddr1()<<", listener="<<m_listener);

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
MacLow::ReceiveError (Ptr<const Packet> packet, double rxSnr)
{
  NS_LOG_FUNCTION (this << packet << rxSnr);
  NS_LOG_DEBUG ("rx failed ");
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
  NS_LOG_FUNCTION (this << packet << rxSnr << txMode << preamble);
  /* A packet is received from the PHY.
   * When we have handled this packet,
   * we handle any packet present in the
   * packet queue.
   */
  WifiMacHeader hdr;
  packet->RemoveHeader (hdr);
  
  bool isPrevNavZero = IsNavZero ();
  NS_LOG_DEBUG ("duration/id=" << hdr.GetDuration ());
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
          hdr.GetAddr1 () == m_self) 
        {
          NS_LOG_DEBUG ("rx RTS from=" << hdr.GetAddr2 () << ", schedule CTS");
          NS_ASSERT (m_sendCtsEvent.IsExpired ());
          WifiRemoteStation *station = GetStation (hdr.GetAddr2 ());
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
          NS_LOG_DEBUG ("rx RTS from=" << hdr.GetAddr2 () << ", cannot schedule CTS");
        }
    } 
  else if (hdr.IsCts () &&
           hdr.GetAddr1 () == m_self &&
           m_ctsTimeoutEvent.IsRunning () &&
           m_currentPacket != 0) 
    {
      NS_LOG_DEBUG ("receive cts from="<<m_currentHdr.GetAddr1 ());
      SnrTag tag;
      packet->FindFirstMatchingTag (tag);
      WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
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
           hdr.GetAddr1 () == m_self &&
           (m_normalAckTimeoutEvent.IsRunning () || 
            m_fastAckTimeoutEvent.IsRunning () ||
            m_superFastAckTimeoutEvent.IsRunning ()) &&
           m_txParams.MustWaitAck ()) 
    {
      NS_LOG_DEBUG ("receive ack from="<<m_currentHdr.GetAddr1 ());
      SnrTag tag;
      packet->FindFirstMatchingTag (tag);
      WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
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
      NS_LOG_DEBUG ("rx drop " << hdr.GetTypeString ());
    } 
  else if (hdr.GetAddr1 () == m_self) 
    {
      WifiRemoteStation *station = GetStation (hdr.GetAddr2 ());
      station->ReportRxOk (rxSnr, txMode);
      
      if (hdr.IsQosData () && hdr.IsQosNoAck ()) 
        {
          NS_LOG_DEBUG ("rx unicast/noAck from="<<hdr.GetAddr2 ());
        } 
      else if (hdr.IsData () || hdr.IsMgt ()) 
        {
          NS_LOG_DEBUG ("rx unicast/sendAck from=" << hdr.GetAddr2 ());
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
  else if (hdr.GetAddr1 ().IsGroup ())
    {
      if (hdr.IsData () || hdr.IsMgt ())
        {
          NS_LOG_DEBUG ("rx group from=" << hdr.GetAddr2 ());
          goto rxPacket;
        }
      else
        {
          // DROP
        }
    }
  else 
    {
      //NS_LOG_DEBUG_VERBOSE ("rx not-for-me from %d", GetSource (packet));
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
uint32_t 
MacLow::GetSize (Ptr<const Packet> packet, const WifiMacHeader *hdr) const
{
  WifiMacTrailer fcs;
  return packet->GetSize () + hdr->GetSize () + fcs.GetSerializedSize ();
}

WifiMode
MacLow::GetRtsTxMode (Ptr<const Packet> packet, const WifiMacHeader *hdr) const
{
  Mac48Address to = hdr->GetAddr1 ();
  return GetStation (to)->GetRtsMode (packet);
}
WifiMode
MacLow::GetDataTxMode (Ptr<const Packet> packet, const WifiMacHeader *hdr) const
{
  Mac48Address to = hdr->GetAddr1 ();
  WifiMacTrailer fcs;
  uint32_t size =  packet->GetSize () + hdr->GetSize () + fcs.GetSerializedSize ();
  return GetStation (to)->GetDataMode (packet, size);
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
MacLow::CalculateOverallTxTime (Ptr<const Packet> packet,
                                WifiMacHeader const*hdr, 
                                MacLowTransmissionParameters const& params) const
{
  Time txTime = Seconds (0);
  WifiMode rtsMode = GetRtsTxMode (packet, hdr);
  WifiMode dataMode = GetDataTxMode (packet, hdr);
  if (params.MustSendRts ()) 
    {
      txTime += m_phy->CalculateTxDuration (GetRtsSize (), rtsMode, WIFI_PREAMBLE_LONG);
      txTime += GetCtsDuration (hdr->GetAddr1 (), rtsMode);
      txTime += GetSifs () * Scalar (2);
    }
  uint32_t dataSize = GetSize (packet, hdr);
  txTime += m_phy->CalculateTxDuration (dataSize, dataMode, WIFI_PREAMBLE_LONG);
  if (params.MustWaitAck ())
    {
      txTime += GetSifs ();
      txTime += GetAckDuration (hdr->GetAddr1 (), dataMode);
    }
  return txTime;
}

Time
MacLow::CalculateTransmissionTime (Ptr<const Packet> packet,
                                   WifiMacHeader const*hdr, 
                                   MacLowTransmissionParameters const& params) const
{
  Time txTime = CalculateOverallTxTime (packet, hdr, params);
  if (params.HasNextPacket ()) 
    {
      WifiMode dataMode = GetDataTxMode (packet, hdr);
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
      hdr.GetAddr2 () == m_bssid) 
    {
      // see section 9.3.2.2 802.11-1999
      DoNavResetNow (duration);
      return;
    }
  // XXX Note that we should also handle CF_END specially here
  // but we don't for now because we do not generate them.
  else if (hdr.GetAddr1 () != m_self)
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
            Scalar (2) * GetSifs () + Scalar (2) * GetSlotTime ();
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
  NS_LOG_FUNCTION (this << packet << hdr << txMode);
  NS_LOG_DEBUG ("send " << hdr->GetTypeString () <<
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
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("cts timeout");
  // XXX: should check that there was no rx start before now.
  // we should restart a new cts timeout now until the expected
  // end of rx if there was a rx start before now.
  WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
  station->ReportRtsFailed ();
  m_currentPacket = 0;
  MacLowTransmissionListener *listener = m_listener;
  m_listener = 0;
  listener->MissedCts ();
}
void
MacLow::NormalAckTimeout (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("normal ack timeout");
  // XXX: should check that there was no rx start before now.
  // we should restart a new ack timeout now until the expected
  // end of rx if there was a rx start before now.
  WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
  station->ReportDataFailed ();
  MacLowTransmissionListener *listener = m_listener;
  m_listener = 0;
  listener->MissedAck ();
}
void
MacLow::FastAckTimeout (void)
{
  NS_LOG_FUNCTION (this);
  WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
  station->ReportDataFailed ();
  MacLowTransmissionListener *listener = m_listener;
  m_listener = 0;
  if (m_phy->IsStateIdle ()) 
    {
      NS_LOG_DEBUG ("fast Ack idle missed");
      listener->MissedAck ();
    }
  else
    {
      NS_LOG_DEBUG ("fast Ack ok");
    }
}
void
MacLow::SuperFastAckTimeout ()
{
  NS_LOG_FUNCTION (this);
  WifiRemoteStation *station = GetStation (m_currentHdr.GetAddr1 ());
  station->ReportDataFailed ();
  MacLowTransmissionListener *listener = m_listener;
  m_listener = 0;
  if (m_phy->IsStateIdle ()) 
    {
      NS_LOG_DEBUG ("super fast Ack failed");
      listener->MissedAck ();
    } 
  else 
    {
      NS_LOG_DEBUG ("super fast Ack ok");
      listener->GotAck (0.0, WifiMode ());
    }
}

void
MacLow::SendRtsForPacket (void)
{
  NS_LOG_FUNCTION (this);
  /* send an RTS for this packet. */
  WifiMacHeader rts;
  rts.SetType (WIFI_MAC_CTL_RTS);
  rts.SetDsNotFrom ();
  rts.SetDsNotTo ();
  rts.SetNoRetry ();
  rts.SetNoMoreFragments ();
  rts.SetAddr1 (m_currentHdr.GetAddr1 ());
  rts.SetAddr2 (m_self);
  WifiMode rtsTxMode = GetRtsTxMode (m_currentPacket, &m_currentHdr);
  Time duration = Seconds (0);
  if (m_txParams.HasDurationId ()) 
    {
      duration += m_txParams.GetDurationId ();
    } 
  else 
    {
      WifiMode dataTxMode = GetDataTxMode (m_currentPacket, &m_currentHdr);
      duration += GetSifs ();
      duration += GetCtsDuration (m_currentHdr.GetAddr1 (), rtsTxMode);
      duration += GetSifs ();
      duration += m_phy->CalculateTxDuration (GetSize (m_currentPacket, &m_currentHdr), 
                                              dataTxMode, WIFI_PREAMBLE_LONG);
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
  WifiMode dataTxMode = GetDataTxMode (m_currentPacket, &m_currentHdr);
  Time txDuration = m_phy->CalculateTxDuration (GetSize (m_currentPacket, &m_currentHdr), dataTxMode, WIFI_PREAMBLE_LONG);
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
  NS_LOG_FUNCTION (this);
  /* send this packet directly. No RTS is needed. */
  StartDataTxTimers ();

  WifiMode dataTxMode = GetDataTxMode (m_currentPacket, &m_currentHdr);
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

WifiRemoteStation *
MacLow::GetStation (Mac48Address ad) const
{
  return m_stationManager->Lookup (ad);
}

void
MacLow::SendCtsAfterRts (Mac48Address source, Time duration, WifiMode rtsTxMode, double rtsSnr)
{
  NS_LOG_FUNCTION (this << source << duration << rtsTxMode << rtsSnr);
  /* send a CTS when you receive a RTS 
   * right after SIFS.
   */
  WifiMode ctsTxMode = GetCtsTxModeForRts (source, rtsTxMode);
  WifiMacHeader cts;
  cts.SetType (WIFI_MAC_CTL_CTS);
  cts.SetDsNotFrom ();
  cts.SetDsNotTo ();
  cts.SetNoMoreFragments ();
  cts.SetNoRetry ();
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
  NS_LOG_FUNCTION (this);
  /* send the third step in a 
   * RTS/CTS/DATA/ACK hanshake 
   */
  NS_ASSERT (m_currentPacket != 0);
  StartDataTxTimers ();

  WifiMode dataTxMode = GetDataTxMode (m_currentPacket, &m_currentHdr);
  Time newDuration = Seconds (0);
  newDuration += GetSifs ();
  newDuration += GetAckDuration (m_currentHdr.GetAddr1 (), dataTxMode);
  Time txDuration = m_phy->CalculateTxDuration (GetSize (m_currentPacket, &m_currentHdr), 
                                                dataTxMode, WIFI_PREAMBLE_LONG);
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
  NS_LOG_FUNCTION (this);
  MacLowTransmissionListener *listener = m_listener;
  m_listener = 0;
  listener->MissedAck ();
  NS_LOG_DEBUG ("fast Ack busy but missed");
}

void
MacLow::SendAckAfterData (Mac48Address source, Time duration, WifiMode dataTxMode, double dataSnr)
{
  NS_LOG_FUNCTION (this);
  /* send an ACK when you receive 
   * a packet after SIFS. 
   */
  WifiMode ackTxMode = GetAckTxModeForData (source, dataTxMode);
  WifiMacHeader ack;
  ack.SetType (WIFI_MAC_CTL_ACK);
  ack.SetDsNotFrom ();
  ack.SetDsNotTo ();
  ack.SetNoRetry ();
  ack.SetNoMoreFragments ();
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
