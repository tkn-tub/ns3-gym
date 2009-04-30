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
                   MakePointerAccessor(&QadhocWifiMac::GetVOQueue,
                                       &QadhocWifiMac::SetVOQueue),
                   MakePointerChecker<EdcaTxopN> ())
    .AddAttribute ("VI_EdcaTxopN",
                   "Queue that manages packets belonging to AC_VI access class",
                   PointerValue (),
                   MakePointerAccessor(&QadhocWifiMac::GetVIQueue,
                                       &QadhocWifiMac::SetVIQueue),
                   MakePointerChecker<EdcaTxopN> ())
    .AddAttribute ("BE_EdcaTxopN",
                   "Queue that manages packets belonging to AC_BE access class",
                   PointerValue (),
                   MakePointerAccessor(&QadhocWifiMac::GetBEQueue,
                                       &QadhocWifiMac::SetBEQueue),
                   MakePointerChecker<EdcaTxopN> ())
    .AddAttribute ("BK_EdcaTxopN",
                   "Queue that manages packets belonging to AC_BK access class",
                   PointerValue (),
                   MakePointerAccessor(&QadhocWifiMac::GetBKQueue,
                                       &QadhocWifiMac::SetBKQueue),
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
  m_voEdca = 0;
  m_viEdca = 0;
  m_beEdca = 0;
  m_bkEdca = 0;
  m_stationManager = 0;
  std::map<AccessClass, Ptr<EdcaTxopN> >::iterator it = m_queues.begin ();
  for (;it != m_queues.end (); it++)
    {
      it->second = 0;
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
  m_voEdca->SetWifiRemoteStationManager (stationManager);
  m_viEdca->SetWifiRemoteStationManager (stationManager);
  m_beEdca->SetWifiRemoteStationManager (stationManager);
  m_bkEdca->SetWifiRemoteStationManager (stationManager);  
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

  WifiRemoteStation *destination = m_stationManager->Lookup (to);
  if (destination->IsBrandNew ())
    {
      // in adhoc mode, we assume that every destination
      // supports all the rates we support.
      for (uint32_t i = 0; i < m_phy->GetNModes (); i++)
        {
          destination->AddSupportedMode (m_phy->GetMode (i));
        }
      destination->RecordDisassociated ();
    }

  uint8_t tid = QosUtilsGetTidForPacket (packet);
  if (tid < 8)
    {
      hdr.SetQosTid (tid);
      AccessClass ac = QosUtilsMapTidToAc (tid);
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
QadhocWifiMac::Receive (Ptr<Packet> packet, WifiMacHeader const *hdr)
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
      //Handling action frames
    }
}

void
QadhocWifiMac::DeaggregateAmsduAndForward (Ptr<Packet> aggregatedPacket,
                                           WifiMacHeader const *hdr)
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
  return m_voEdca;
}

Ptr<EdcaTxopN>
QadhocWifiMac::GetVIQueue (void) const
{
  return m_viEdca;
}

Ptr<EdcaTxopN>
QadhocWifiMac::GetBEQueue (void) const
{
  return m_beEdca;
}

Ptr<EdcaTxopN>
QadhocWifiMac::GetBKQueue (void) const
{
  return m_bkEdca;
}

void
QadhocWifiMac::SetVOQueue (Ptr<EdcaTxopN> voQueue)
{
  m_voEdca = voQueue;
  m_queues.insert (std::make_pair(AC_VO, m_voEdca));
  m_queues[AC_VO]->SetLow (m_low);
  m_queues[AC_VO]->SetManager (m_dcfManager);
  m_queues[AC_VO]->SetTypeOfStation (ADHOC_STA);
  m_queues[AC_VO]->SetTxMiddle (m_txMiddle);
}

void
QadhocWifiMac::SetVIQueue (Ptr<EdcaTxopN> viQueue)
{
  m_viEdca = viQueue;
  m_queues.insert (std::make_pair(AC_VI, m_viEdca));
  m_queues[AC_VI]->SetLow (m_low);
  m_queues[AC_VI]->SetManager (m_dcfManager);
  m_queues[AC_VI]->SetTypeOfStation (ADHOC_STA);
  m_queues[AC_VI]->SetTxMiddle (m_txMiddle);
}

void
QadhocWifiMac::SetBEQueue (Ptr<EdcaTxopN> beQueue)
{
  m_beEdca = beQueue;
  m_queues.insert (std::make_pair(AC_BE, m_beEdca));
  m_queues[AC_BE]->SetLow (m_low);
  m_queues[AC_BE]->SetManager (m_dcfManager);
  m_queues[AC_BE]->SetTypeOfStation (ADHOC_STA);
  m_queues[AC_BE]->SetTxMiddle (m_txMiddle);
}

void
QadhocWifiMac::SetBKQueue (Ptr<EdcaTxopN> bkQueue)
{
  m_bkEdca = bkQueue;
  m_queues.insert (std::make_pair(AC_BK, m_bkEdca));
  m_queues[AC_BK]->SetLow (m_low);
  m_queues[AC_BK]->SetManager (m_dcfManager);
  m_queues[AC_BK]->SetTypeOfStation (ADHOC_STA);
  m_queues[AC_BK]->SetTxMiddle (m_txMiddle);
}

} //namespace ns3
