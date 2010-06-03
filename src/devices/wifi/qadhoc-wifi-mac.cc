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
#include "ns3/pointer.h"
#include "ns3/log.h"
#include "ns3/string.h"

#include "qos-tag.h"
#include "edca-txop-n.h"
#include "qadhoc-wifi-mac.h"
#include "mac-low.h"
#include "dcf-manager.h"
#include "mac-rx-middle.h"
#include "mac-tx-middle.h"
#include "wifi-mac-header.h"
#include "msdu-aggregator.h"
#include "amsdu-subframe-header.h"
#include "mgt-headers.h"

NS_LOG_COMPONENT_DEFINE ("QadhocWifiMac");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (QadhocWifiMac);

TypeId
QadhocWifiMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::QadhocWifiMac")
    .SetParent<WifiMac> ()
    .AddConstructor<QadhocWifiMac> ()
    .AddAttribute ("VO_EdcaTxopN",
                   "Queue that manages packets belonging to AC_VO access class",
                   PointerValue (),
                   MakePointerAccessor(&QadhocWifiMac::GetVOQueue),
                   MakePointerChecker<EdcaTxopN> ())
    .AddAttribute ("VI_EdcaTxopN",
                   "Queue that manages packets belonging to AC_VI access class",
                   PointerValue (),
                   MakePointerAccessor(&QadhocWifiMac::GetVIQueue),
                   MakePointerChecker<EdcaTxopN> ())
    .AddAttribute ("BE_EdcaTxopN",
                   "Queue that manages packets belonging to AC_BE access class",
                   PointerValue (),
                   MakePointerAccessor(&QadhocWifiMac::GetBEQueue),
                   MakePointerChecker<EdcaTxopN> ())
    .AddAttribute ("BK_EdcaTxopN",
                   "Queue that manages packets belonging to AC_BK access class",
                   PointerValue (),
                   MakePointerAccessor(&QadhocWifiMac::GetBKQueue),
                   MakePointerChecker<EdcaTxopN> ())
    ;
  return tid;
}

QadhocWifiMac::QadhocWifiMac ()
{
  NS_LOG_FUNCTION (this);
  m_rxMiddle = new MacRxMiddle ();
  m_rxMiddle->SetForwardCallback (MakeCallback (&QadhocWifiMac::Receive, this));

  m_txMiddle = new MacTxMiddle ();

  m_low = CreateObject<MacLow> ();
  m_low->SetRxCallback (MakeCallback (&MacRxMiddle::Receive, m_rxMiddle));

  m_dcfManager = new DcfManager ();
  m_dcfManager->SetupLowListener (m_low);

  // Construct the EDCAFs. The ordering is important - highest
  // priority (see Table 9-1 in IEEE 802.11-2007) must be created
  // first.
  SetQueue (AC_VO);
  SetQueue (AC_VI);
  SetQueue (AC_BE);
  SetQueue (AC_BK);
}

QadhocWifiMac::~QadhocWifiMac ()
{
  NS_LOG_FUNCTION (this);
}

void
QadhocWifiMac::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  delete m_rxMiddle;
  m_rxMiddle = 0;
  delete m_txMiddle;
  m_txMiddle = 0;
  delete m_dcfManager;
  m_dcfManager = 0;
  m_low = 0;
  m_phy = 0;
  m_stationManager = 0;
  for (Queues::iterator i = m_queues.begin (); i != m_queues.end (); ++i)
    {
      (*i).second = 0;
    }
  WifiMac::DoDispose ();
}

void
QadhocWifiMac::SetSlot (Time slotTime)
{
  m_dcfManager->SetSlot (slotTime);
  m_low->SetSlotTime (slotTime);
}

void
QadhocWifiMac::SetSifs (Time sifs)
{
  m_dcfManager->SetSifs (sifs);
  m_low->SetSifs (sifs);
}

void
QadhocWifiMac::SetEifsNoDifs (Time eifsNoDifs)
{
  m_dcfManager->SetEifsNoDifs (eifsNoDifs);
  m_eifsNoDifs = eifsNoDifs;
}

void
QadhocWifiMac::SetAckTimeout (Time ackTimeout)
{
  m_low->SetAckTimeout (ackTimeout);
}

void 
QadhocWifiMac::SetBasicBlockAckTimeout (Time blockAckTimeout)
{
  m_low->SetBasicBlockAckTimeout (blockAckTimeout);
}

void 
QadhocWifiMac::SetCompressedBlockAckTimeout (Time blockAckTimeout)
{
  m_low->SetCompressedBlockAckTimeout (blockAckTimeout);
}

void
QadhocWifiMac::SetCtsTimeout (Time ctsTimeout)
{
  m_low->SetCtsTimeout (ctsTimeout);
}

void
QadhocWifiMac::SetPifs (Time pifs)
{
  m_low->SetPifs (pifs);
}

Time
QadhocWifiMac::GetSlot (void) const
{
  return m_low->GetSlotTime ();
}

Time
QadhocWifiMac::GetSifs (void) const
{
  return m_low->GetSifs ();
}

Time
QadhocWifiMac::GetEifsNoDifs (void) const
{
  return m_eifsNoDifs;
}

Time
QadhocWifiMac::GetAckTimeout (void) const
{
  return m_low->GetAckTimeout ();
}

Time 
QadhocWifiMac::GetBasicBlockAckTimeout (void) const
{
  return m_low->GetBasicBlockAckTimeout ();
}

Time 
QadhocWifiMac::GetCompressedBlockAckTimeout (void) const
{
  return m_low->GetCompressedBlockAckTimeout ();
}

Time
QadhocWifiMac::GetCtsTimeout (void) const
{
  return m_low->GetCtsTimeout ();
}

Time
QadhocWifiMac::GetPifs (void) const
{
  return m_low->GetPifs ();
}

void
QadhocWifiMac::SetWifiPhy (Ptr<WifiPhy> phy)
{
  m_phy = phy;
  m_dcfManager->SetupPhyListener (phy);
  m_low->SetPhy (phy);
}

void
QadhocWifiMac::SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> stationManager)
{
  NS_LOG_FUNCTION (this << stationManager);
  m_stationManager = stationManager;
  for (Queues::iterator i = m_queues.begin (); i != m_queues.end (); ++i)
    {
      (*i).second->SetWifiRemoteStationManager (stationManager);
    }
  m_low->SetWifiRemoteStationManager (stationManager);
}

void
QadhocWifiMac::Enqueue (Ptr<const Packet> packet, Mac48Address to, Mac48Address from)
{
  NS_FATAL_ERROR ("Adhoc does not support a from != m_low->GetAddress ()");
}

void
QadhocWifiMac::Enqueue (Ptr<const Packet> packet, Mac48Address to)
{
  /* For now Qos adhoc stations sends only Qos frame. In the future they 
   * should be able to send frames also to Non-Qos Stas.
   */
  NS_LOG_FUNCTION (packet->GetSize () << to);
  WifiMacHeader hdr;
  hdr.SetType (WIFI_MAC_QOSDATA);
  hdr.SetQosAckPolicy (WifiMacHeader::NORMAL_ACK);
  hdr.SetQosNoEosp ();
  hdr.SetQosNoAmsdu ();
  /* Transmission of multiple frames in the same 
     Txop is not supported for now */
  hdr.SetQosTxopLimit (0);
  
  hdr.SetAddr1 (to);
  hdr.SetAddr2 (m_low->GetAddress ());
  hdr.SetAddr3 (GetBssid ());
  hdr.SetDsNotFrom ();
  hdr.SetDsNotTo ();

  if (m_stationManager->IsBrandNew (to))
    {
      // in adhoc mode, we assume that every destination
      // supports all the rates we support.
      for (uint32_t i = 0; i < m_phy->GetNModes (); i++)
        {
          m_stationManager->AddSupportedMode (to, m_phy->GetMode (i));
        }
      m_stationManager->RecordDisassociated (to);
    }

  uint8_t tid = QosUtilsGetTidForPacket (packet);
  if (tid < 8)
    {
      hdr.SetQosTid (tid);
      AcIndex ac = QosUtilsMapTidToAc (tid);
      m_queues[ac]->Queue (packet, hdr);
    }
  else
    {
      //packet is considerated belonging to BestEffort AC
      hdr.SetQosTid (0);
      m_queues[AC_BE]->Queue (packet, hdr);
    }
}

bool
QadhocWifiMac::SupportsSendFrom (void) const
{
  return false;
}

void
QadhocWifiMac::SetForwardUpCallback (Callback<void,Ptr<Packet>, Mac48Address, Mac48Address> upCallback)
{
  m_forwardUp = upCallback;
}

void
QadhocWifiMac::SetLinkUpCallback (Callback<void> linkUp)
{
  // an Adhoc network is always UP.
  linkUp ();
}

void
QadhocWifiMac::SetLinkDownCallback (Callback<void> linkDown)
{}

Mac48Address
QadhocWifiMac::GetAddress (void) const
{
  return m_low->GetAddress ();
}

Ssid
QadhocWifiMac::GetSsid (void) const
{
  return m_ssid;
}

void
QadhocWifiMac::SetAddress (Mac48Address address)
{
  m_low->SetAddress (address);
  m_low->SetBssid (address);
}

void
QadhocWifiMac::SetSsid (Ssid ssid)
{
  NS_LOG_FUNCTION (this << ssid);
  // XXX: here, we should start a special adhoc network
  m_ssid = ssid;
}

Mac48Address
QadhocWifiMac::GetBssid (void) const
{
  return m_low->GetBssid ();
}

void
QadhocWifiMac::ForwardUp (Ptr<Packet> packet, Mac48Address from, Mac48Address to)
{
  NS_LOG_FUNCTION (this << packet << from);
  m_forwardUp (packet, from, to);
}

void
QadhocWifiMac::Receive (Ptr<Packet> packet, const WifiMacHeader *hdr)
{
  NS_LOG_FUNCTION (this << packet << hdr);
  NS_ASSERT (!hdr->IsCtl ());
  Mac48Address from = hdr->GetAddr2 ();
  Mac48Address to = hdr->GetAddr1 ();
  if (hdr->IsData ())
    {
      if (hdr->IsQosData () && hdr->IsQosAmsdu ())
        {
          NS_LOG_DEBUG ("Received A-MSDU from"<<from);
          DeaggregateAmsduAndForward (packet, hdr);
        }
      else
        {
          ForwardUp (packet, from, to);
        }
    }
  else if (hdr->IsMgt ())
    {
      if (hdr->IsAction ())
        {
          WifiActionHeader actionHdr;
          packet->RemoveHeader (actionHdr);
          if (actionHdr.GetCategory () == WifiActionHeader::BLOCK_ACK &&
              actionHdr.GetAction().blockAck == WifiActionHeader::BLOCK_ACK_ADDBA_REQUEST)
            {
              MgtAddBaRequestHeader reqHdr;
              packet->RemoveHeader (reqHdr);
              SendAddBaResponse (&reqHdr, hdr->GetAddr2 ());
            }
          else if (actionHdr.GetCategory () == WifiActionHeader::BLOCK_ACK &&
                   actionHdr.GetAction().blockAck == WifiActionHeader::BLOCK_ACK_ADDBA_REQUEST)
            {
              MgtAddBaResponseHeader respHdr;
              packet->RemoveHeader (respHdr);
              m_queues[QosUtilsMapTidToAc (respHdr.GetTid ())]->GotAddBaResponse (&respHdr, hdr->GetAddr2 ());
            }
          else if (actionHdr.GetCategory () == WifiActionHeader::BLOCK_ACK &&
                   actionHdr.GetAction().blockAck == WifiActionHeader::BLOCK_ACK_DELBA)
            {
              MgtDelBaHeader delBaHdr;
              packet->RemoveHeader (delBaHdr);
              if (delBaHdr.IsByOriginator ())
                {
                  /* Delba frame was sent by originator, this means that an ingoing established
                     agreement exists in MacLow */
                     m_low->DestroyBlockAckAgreement (hdr->GetAddr2 (), delBaHdr.GetTid ());                }
              else
                {
                  /* We must notify correct queue tear down of agreement */
                  AcIndex ac = QosUtilsMapTidToAc (delBaHdr.GetTid ());
                  m_queues[ac]->GotDelBaFrame (&delBaHdr, hdr->GetAddr2 ());
                }
            }
        }
    }
}

void
QadhocWifiMac::DeaggregateAmsduAndForward (Ptr<Packet> aggregatedPacket,
                                           const WifiMacHeader *hdr)
{
  DeaggregatedMsdus packets = MsduAggregator::Deaggregate (aggregatedPacket);
  for (DeaggregatedMsdusCI i = packets.begin (); i != packets.end (); ++i)
    {
      ForwardUp ((*i).first, (*i).second.GetSourceAddr (),
                 (*i).second.GetDestinationAddr ());
    }
}

Ptr<EdcaTxopN>
QadhocWifiMac::GetVOQueue (void) const
{
  return m_queues.find (AC_VO)->second;
}

Ptr<EdcaTxopN>
QadhocWifiMac::GetVIQueue (void) const
{
  return m_queues.find (AC_VI)->second;
}

Ptr<EdcaTxopN>
QadhocWifiMac::GetBEQueue (void) const
{
  return m_queues.find (AC_BE)->second;
}
void
QadhocWifiMac::DoStart ()
{
  for (Queues::iterator i = m_queues.begin (); i != m_queues.end (); ++i)
    {
      i->second->Start ();
    }
  WifiMac::DoStart ();
}

Ptr<EdcaTxopN>
QadhocWifiMac::GetBKQueue (void) const
{
  return m_queues.find (AC_BK)->second;
}

void
QadhocWifiMac::SetQueue (enum AcIndex ac)
{
  Ptr<EdcaTxopN> edca = CreateObject<EdcaTxopN> ();
  edca->SetLow (m_low);
  edca->SetManager (m_dcfManager);
  edca->SetTypeOfStation (ADHOC_STA);
  edca->SetTxMiddle (m_txMiddle);
  edca->SetAccessCategory (ac);
  edca->CompleteConfig ();
  m_queues.insert (std::make_pair(ac, edca));
}

void 
QadhocWifiMac::FinishConfigureStandard (enum WifiPhyStandard standard)
{
  switch (standard)
    {
    case WIFI_PHY_STANDARD_80211p_CCH:
      ConfigureCCHDcf (m_queues[AC_BK], 15, 511, AC_BK);
      ConfigureCCHDcf (m_queues[AC_BE], 15, 511, AC_BE);
      ConfigureCCHDcf (m_queues[AC_VI], 15, 511, AC_VI);
      ConfigureCCHDcf (m_queues[AC_VO], 15, 511, AC_VO);
      break;
    case WIFI_PHY_STANDARD_80211p_SCH:
      ConfigureDcf (m_queues[AC_BK], 15, 511, AC_BK);
      ConfigureDcf (m_queues[AC_BE], 15, 511, AC_BE);
      ConfigureDcf (m_queues[AC_VI], 15, 511, AC_VI);
      ConfigureDcf (m_queues[AC_VO], 15, 511, AC_VO);
      break;
    case WIFI_PHY_STANDARD_holland:
      // fall through
    case WIFI_PHY_STANDARD_80211a:
      // fall through
    case WIFI_PHY_STANDARD_80211_10Mhz:
      // fall through
    case WIFI_PHY_STANDARD_80211_5Mhz:
      ConfigureDcf (m_queues[AC_BK], 15, 1023, AC_BK);
      ConfigureDcf (m_queues[AC_BE], 15, 1023, AC_BE);
      ConfigureDcf (m_queues[AC_VI], 15, 1023, AC_VI);
      ConfigureDcf (m_queues[AC_VO], 15, 1023, AC_VO);
      break;
    case WIFI_PHY_STANDARD_80211b:
      ConfigureDcf (m_queues[AC_BK], 31, 1023, AC_BK);
      ConfigureDcf (m_queues[AC_BE], 31, 1023, AC_BE);
      ConfigureDcf (m_queues[AC_VI], 31, 1023, AC_VI);
      ConfigureDcf (m_queues[AC_VO], 31, 1023, AC_VO);
      break;
    default:
      NS_ASSERT (false);
      break;
    }
}


void
QadhocWifiMac::SendAddBaResponse (const MgtAddBaRequestHeader *reqHdr, Mac48Address originator)
{
  NS_LOG_FUNCTION (this);
  WifiMacHeader hdr;
  hdr.SetAction ();
  hdr.SetAddr1 (originator);
  hdr.SetAddr2 (m_low->GetAddress ());
  hdr.SetAddr3 (m_low->GetAddress ());
  hdr.SetDsNotFrom ();
  hdr.SetDsNotTo ();

  MgtAddBaResponseHeader respHdr;
  StatusCode code;
  code.SetSuccess ();
  respHdr.SetStatusCode (code);
  //Here a control about queues type?
  respHdr.SetAmsduSupport (reqHdr->IsAmsduSupported ());

  if (reqHdr->IsImmediateBlockAck ())
    {
      respHdr.SetImmediateBlockAck ();
    }
  else
    {
      respHdr.SetDelayedBlockAck ();
    }
  respHdr.SetTid (reqHdr->GetTid ());
  /* For now there's not no control about limit of reception.
     We assume that receiver has no limit on reception.
     However we assume that a receiver sets a bufferSize in order to satisfy
     next equation:
     (bufferSize + 1) % 16 = 0
     So if a recipient is able to buffer a packet, it should be also able to buffer
     all possible packet's fragments.
     See section 7.3.1.14 in IEEE802.11e for more details. */
  respHdr.SetBufferSize (1023);
  respHdr.SetTimeout (reqHdr->GetTimeout ());

  WifiActionHeader actionHdr;
  WifiActionHeader::ActionValue action;
  action.blockAck = WifiActionHeader::BLOCK_ACK_ADDBA_RESPONSE;
  actionHdr.SetAction (WifiActionHeader::BLOCK_ACK, action);

  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (respHdr);
  packet->AddHeader (actionHdr);
  
  /* ns3::MacLow have to buffer all correctly received packet for this block ack session */
  m_low->CreateBlockAckAgreement (&respHdr, originator, reqHdr->GetStartingSequence ());

  //Better a management queue? 
  m_queues[QosUtilsMapTidToAc (reqHdr->GetTid ())]->PushFront (packet, hdr);
}
} //namespace ns3
