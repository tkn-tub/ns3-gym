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
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/string.h"
#include "ns3/pointer.h"

#include "qos-tag.h"
#include "qap-wifi-mac.h"
#include "dca-txop.h"
#include "edca-txop-n.h"
#include "wifi-phy.h"
#include "dcf-manager.h"
#include "mac-rx-middle.h"
#include "mac-tx-middle.h"
#include "mgt-headers.h"
#include "mac-low.h"
#include "amsdu-subframe-header.h"
#include "msdu-aggregator.h"

NS_LOG_COMPONENT_DEFINE ("QapWifiMac");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (QapWifiMac);

TypeId
QapWifiMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::QapWifiMac")
    .SetParent<WifiMac> ()
    .AddConstructor<QapWifiMac> ()
    .AddAttribute ("BeaconInterval", "Delay between two beacons",
                   TimeValue (Seconds (0.1)),
                   MakeTimeAccessor (&QapWifiMac::GetBeaconInterval,
                                     &QapWifiMac::SetBeaconInterval),
                   MakeTimeChecker ())
    .AddAttribute ("BeaconGeneration", "Whether or not beacons are generated.",
                   BooleanValue (true),
                   MakeBooleanAccessor (&QapWifiMac::SetBeaconGeneration,
                                        &QapWifiMac::GetBeaconGeneration),
                   MakeBooleanChecker ())
    .AddAttribute ("VO_EdcaTxopN",
                   "Queue that manages packets belonging to AC_VO access class",
                   PointerValue (),
                   MakePointerAccessor(&QapWifiMac::GetVOQueue,
                                       &QapWifiMac::SetVOQueue),
                   MakePointerChecker<EdcaTxopN> ())
    .AddAttribute ("VI_EdcaTxopN",
                   "Queue that manages packets belonging to AC_VI access class",
                   PointerValue (),
                   MakePointerAccessor(&QapWifiMac::GetVIQueue,
                                       &QapWifiMac::SetVIQueue),
                   MakePointerChecker<EdcaTxopN> ())
    .AddAttribute ("BE_EdcaTxopN",
                   "Queue that manages packets belonging to AC_BE access class",
                   PointerValue (),
                   MakePointerAccessor(&QapWifiMac::GetBEQueue,
                                       &QapWifiMac::SetBEQueue),
                   MakePointerChecker<EdcaTxopN> ())
    .AddAttribute ("BK_EdcaTxopN",
                   "Queue that manages packets belonging to AC_BK access class",
                   PointerValue (),
                   MakePointerAccessor(&QapWifiMac::GetBKQueue,
                                       &QapWifiMac::SetBKQueue),
                   MakePointerChecker<EdcaTxopN> ())
    ;  
  return tid;
}

QapWifiMac::QapWifiMac ()
{
  NS_LOG_FUNCTION (this);
  m_rxMiddle = new MacRxMiddle ();
  m_rxMiddle->SetForwardCallback (MakeCallback (&QapWifiMac::Receive, this));
  
  m_txMiddle = new MacTxMiddle ();

  m_low = CreateObject<MacLow> ();
  m_low->SetRxCallback (MakeCallback (&MacRxMiddle::Receive, m_rxMiddle));
  
  m_dcfManager = new DcfManager ();
  m_dcfManager->SetupLowListener (m_low);

  m_beaconDca = CreateObject<DcaTxop> ();
  m_beaconDca->SetAifsn(1);
  m_beaconDca->SetMinCw(0);
  m_beaconDca->SetMaxCw(0);
  m_beaconDca->SetLow (m_low);
  m_beaconDca->SetManager (m_dcfManager);
}

QapWifiMac::~QapWifiMac ()
{
  NS_LOG_FUNCTION (this);
}

void
QapWifiMac::DoDispose ()
{
  delete m_rxMiddle;
  m_rxMiddle = 0;
  delete m_txMiddle;
  m_txMiddle = 0;
  delete m_dcfManager;
  m_dcfManager = 0;
  m_low = 0;
  m_phy = 0;
  m_beaconDca = 0;
  m_beaconEvent.Cancel ();
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
QapWifiMac::SetBeaconGeneration (bool enable)
{
  NS_LOG_FUNCTION (this << enable);
  if (enable)
    {
      m_beaconEvent = Simulator::ScheduleNow (&QapWifiMac::SendOneBeacon, this);
    }
  else
    {
      m_beaconEvent.Cancel ();
    }
}

bool
QapWifiMac::GetBeaconGeneration (void) const
{
  return m_beaconEvent.IsRunning ();
}

Time
QapWifiMac::GetBeaconInterval (void) const
{
  return m_beaconInterval;
}

void 
QapWifiMac::SetSlot (Time slotTime)
{
  NS_LOG_FUNCTION (this << slotTime);
  m_dcfManager->SetSlot (slotTime);
  m_low->SetSlotTime (slotTime);
}

void 
QapWifiMac::SetSifs (Time sifs)
{
  NS_LOG_FUNCTION (this << sifs);
  m_dcfManager->SetSifs (sifs);
  m_low->SetSifs (sifs);
}

void 
QapWifiMac::SetEifsNoDifs (Time eifsNoDifs)
{
  NS_LOG_FUNCTION (this << eifsNoDifs);
  m_dcfManager->SetEifsNoDifs (eifsNoDifs);
  m_eifsNoDifs = eifsNoDifs;
}

void 
QapWifiMac::SetAckTimeout (Time ackTimeout)
{
  m_low->SetAckTimeout (ackTimeout);
}

void 
QapWifiMac::SetCtsTimeout (Time ctsTimeout)
{
  m_low->SetCtsTimeout (ctsTimeout);
}

void 
QapWifiMac::SetPifs (Time pifs)
{
  m_low->SetPifs (pifs);
}

Time 
QapWifiMac::GetSlot (void) const
{
  return m_low->GetSlotTime ();
}

Time 
QapWifiMac::GetSifs (void) const
{
  return m_low->GetSifs ();
}

Time 
QapWifiMac::GetEifsNoDifs (void) const
{
  return m_eifsNoDifs;
}

Time 
QapWifiMac::GetAckTimeout (void) const
{
  return m_low->GetAckTimeout ();
}

Time 
QapWifiMac::GetCtsTimeout (void) const
{
  return m_low->GetCtsTimeout ();
}

Time 
QapWifiMac::GetPifs (void) const
{
  return m_low->GetPifs ();
}

void 
QapWifiMac::SetWifiPhy (Ptr<WifiPhy> phy)
{
  NS_LOG_FUNCTION (this << phy);
  m_phy = phy;
  m_dcfManager->SetupPhyListener (phy);
  m_low->SetPhy (phy);
}

void
QapWifiMac::SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> stationManager)
{
  NS_LOG_FUNCTION (this << stationManager);
  m_stationManager = stationManager;
  m_voEdca->SetWifiRemoteStationManager (stationManager);
  m_viEdca->SetWifiRemoteStationManager (stationManager);
  m_beEdca->SetWifiRemoteStationManager (stationManager);
  m_bkEdca->SetWifiRemoteStationManager (stationManager);
  m_beaconDca->SetWifiRemoteStationManager (stationManager);
  m_low->SetWifiRemoteStationManager (stationManager);
}

void 
QapWifiMac::SetForwardUpCallback (Callback<void, Ptr<Packet>, Mac48Address, Mac48Address> upCallback)
{
  NS_LOG_FUNCTION (this);
  m_forwardUp = upCallback;
}

void
QapWifiMac::SetLinkUpCallback (Callback<void> linkUp)
{
  NS_LOG_FUNCTION (this);
  if (!linkUp.IsNull ())
    {
      linkUp ();
    }
}

void 
QapWifiMac::SetLinkDownCallback (Callback<void> linkDown)
{
  NS_LOG_FUNCTION (this);
}

Mac48Address
QapWifiMac::GetAddress () const
{
  return m_low->GetAddress ();
}

Ssid 
QapWifiMac::GetSsid (void) const
{
  return m_ssid;
}

void
QapWifiMac::SetAddress (Mac48Address address)
{
  NS_LOG_FUNCTION (address);
  m_low->SetAddress (address);
  m_low->SetBssid (address);
}

void
QapWifiMac::SetSsid (Ssid ssid)
{
  NS_LOG_FUNCTION (this << ssid);
  m_ssid = ssid;
}

Mac48Address 
QapWifiMac::GetBssid (void) const
{
  return m_low->GetBssid ();
}

void
QapWifiMac::SetBeaconInterval (Time interval)
{
  NS_LOG_FUNCTION (this << interval);
  m_beaconInterval = interval;
}

void
QapWifiMac::StartBeaconing (void)
{
  NS_LOG_FUNCTION (this);
  SendOneBeacon ();
}

void
QapWifiMac::ForwardUp (Ptr<Packet> packet, Mac48Address from, Mac48Address to)
{
  NS_LOG_FUNCTION (this << packet << from);
  m_forwardUp (packet, from, to);
}

void
QapWifiMac::ForwardDown (Ptr<const Packet> packet, Mac48Address from, Mac48Address to)
{
  /* For now Qos AP sends only Qos frame. In the future it should be able to 
     send frames also to Non-Qos Stas.
   */
  NS_LOG_FUNCTION (this << packet << from << to);
  WifiMacHeader hdr;
  hdr.SetType (WIFI_MAC_QOSDATA);
  hdr.SetQosAckPolicy (WifiMacHeader::NORMAL_ACK);
  hdr.SetQosNoEosp ();
  hdr.SetQosNoAmsdu ();
  /* Transmission of multiple frames in the same 
     Txop is not supported for now */
  hdr.SetQosTxopLimit (0);

  hdr.SetAddr1 (to);
  hdr.SetAddr2 (GetAddress ());
  hdr.SetAddr3 (from);
  hdr.SetDsFrom ();
  hdr.SetDsNotTo ();
  
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

void
QapWifiMac::ForwardDown (Ptr<const Packet> packet, Mac48Address from, Mac48Address to,
                         WifiMacHeader const *oldHdr)
{
  /* For now Qos AP sends only Qos frame. In the future it should be able to 
     send frames also to Non-Qos Stas.
   */
  NS_LOG_FUNCTION (this << packet << from << to);
  NS_ASSERT (oldHdr->IsQosData ());
  WifiMacHeader hdr;
  hdr.SetType (WIFI_MAC_QOSDATA);
  hdr.SetQosAckPolicy (WifiMacHeader::NORMAL_ACK);
  hdr.SetQosNoEosp ();
  hdr.SetQosNoAmsdu ();
  /* Transmission of multiple frames in the same 
     Txop is not supported for now */
  hdr.SetQosTxopLimit (0);
  hdr.SetQosTid (oldHdr->GetQosTid ());

  hdr.SetAddr1 (to);
  hdr.SetAddr2 (GetAddress ());
  hdr.SetAddr3 (from);
  hdr.SetDsFrom ();
  hdr.SetDsNotTo ();

  AccessClass ac = QosUtilsMapTidToAc (oldHdr->GetQosTid ());
  m_queues[ac]->Queue (packet, hdr);
}

void
QapWifiMac::Enqueue (Ptr<const Packet> packet, Mac48Address to, Mac48Address from)
{
  NS_LOG_FUNCTION (this << packet << from << to);
  ForwardDown (packet, from, to);
}

void 
QapWifiMac::Enqueue (Ptr<const Packet> packet, Mac48Address to)
{
  NS_LOG_FUNCTION (this << packet << to);
  ForwardDown (packet, m_low->GetAddress (), to);
}

bool 
QapWifiMac::SupportsSendFrom (void) const
{
  return true;
}

SupportedRates
QapWifiMac::GetSupportedRates (void) const
{
  // send the set of supported rates and make sure that we indicate
  // the Basic Rate set in this set of supported rates.
  SupportedRates rates;
  for (uint32_t i = 0; i < m_phy->GetNModes (); i++)
    {
      WifiMode mode = m_phy->GetMode (i);
      rates.AddSupportedRate (mode.GetDataRate ());
    }
  // set the basic rates
  for (uint32_t j = 0; j < m_stationManager->GetNBasicModes (); j++)
    {
      WifiMode mode = m_stationManager->GetBasicMode (j);
      rates.SetBasicRate (mode.GetDataRate ());
    }
  return rates; 
}

void
QapWifiMac::SendProbeResp (Mac48Address to)
{
  NS_LOG_FUNCTION (this << to);
  WifiMacHeader hdr;
  hdr.SetProbeResp ();
  hdr.SetAddr1 (to);
  hdr.SetAddr2 (GetAddress ());
  hdr.SetAddr3 (GetAddress ());
  hdr.SetDsNotFrom ();
  hdr.SetDsNotTo ();
  Ptr<Packet> packet = Create<Packet> ();
  MgtProbeResponseHeader probe;
  probe.SetSsid (GetSsid ());
  probe.SetSupportedRates (GetSupportedRates ());
  probe.SetBeaconIntervalUs (m_beaconInterval.GetMicroSeconds ());
  packet->AddHeader (probe);

  /* Which is correct queue for management frames ? */
  m_queues[AC_VO]->Queue (packet, hdr);
}

void
QapWifiMac::SendAssocResp (Mac48Address to, bool success)
{
  NS_LOG_FUNCTION (this << to << success);
  WifiMacHeader hdr;
  hdr.SetAssocResp ();
  hdr.SetAddr1 (to);
  hdr.SetAddr2 (GetAddress ());
  hdr.SetAddr3 (GetAddress ());
  hdr.SetDsNotFrom ();
  hdr.SetDsNotTo ();
  Ptr<Packet> packet = Create<Packet> ();
  MgtAssocResponseHeader assoc;
  StatusCode code;
  if (success)
    {
      code.SetSuccess ();
    }
  else
    {
      code.SetFailure ();
    }
  assoc.SetSupportedRates (GetSupportedRates ());
  assoc.SetStatusCode (code);
  packet->AddHeader (assoc);
  
  /* Which is correct queue for management frames ? */
  m_queues[AC_VO]->Queue (packet, hdr);
}

void
QapWifiMac::SendOneBeacon (void)
{
  NS_LOG_FUNCTION (this);
  WifiMacHeader hdr;
  hdr.SetBeacon ();
  hdr.SetAddr1 (Mac48Address::GetBroadcast ());
  hdr.SetAddr2 (GetAddress ());
  hdr.SetAddr3 (GetAddress ());
  hdr.SetDsNotFrom ();
  hdr.SetDsNotTo ();
  Ptr<Packet> packet = Create<Packet> ();
  MgtBeaconHeader beacon;
  beacon.SetSsid (GetSsid ());
  beacon.SetSupportedRates (GetSupportedRates ());
  beacon.SetBeaconIntervalUs (m_beaconInterval.GetMicroSeconds ());
  
  packet->AddHeader (beacon);

  m_beaconDca->Queue (packet, hdr);
  m_beaconEvent = Simulator::Schedule (m_beaconInterval, &QapWifiMac::SendOneBeacon, this);  
}

void
QapWifiMac::TxOk (WifiMacHeader const &hdr)
{
  NS_LOG_FUNCTION (this);
  WifiRemoteStation *station = m_stationManager->Lookup (hdr.GetAddr1 ());
  if (hdr.IsAssocResp () && 
      station->IsWaitAssocTxOk ()) 
    {
      NS_LOG_DEBUG ("associated with sta="<<hdr.GetAddr1 ());
      station->RecordGotAssocTxOk ();
    }
}

void
QapWifiMac::TxFailed (WifiMacHeader const &hdr)
{
  NS_LOG_FUNCTION (this);
  WifiRemoteStation *station = m_stationManager->Lookup (hdr.GetAddr1 ());
  if (hdr.IsAssocResp () && 
      station->IsWaitAssocTxOk ()) 
    {
      NS_LOG_DEBUG ("assoc failed with sta="<<hdr.GetAddr1 ());
      station->RecordGotAssocTxFailed ();
    }
}

void
QapWifiMac::Receive (Ptr<Packet> packet, WifiMacHeader const *hdr)
{
  NS_LOG_FUNCTION (this << packet << hdr);

  Mac48Address from = hdr->GetAddr2 ();
  WifiRemoteStation *fromStation = m_stationManager->Lookup (from);
  
  if (hdr->IsData ())
    {
      Mac48Address bssid = hdr->GetAddr1 ();
      if (!hdr->IsFromDs () && 
          hdr->IsToDs () &&
          bssid == GetAddress () &&
          fromStation->IsAssociated ())
        {
          Mac48Address to = hdr->GetAddr3 ();
          WifiRemoteStation *toStation = m_stationManager->Lookup (to);
          
          if (to == GetAddress ())
            {
              NS_LOG_DEBUG ("frame for me (Qap) from="<<from);
              if (hdr->IsQosData ())
                {
                  if (hdr->IsQosAmsdu ())
                    {
                      NS_LOG_DEBUG ("Received A-MSDU from="<<from<<", size="<<packet->GetSize ());
                      DeaggregateAmsduAndForward (packet, hdr);
                      packet = 0;
                    }
                  else
                    {
                      ForwardUp (packet, from, bssid);
                    }
                }
              else
                {
                  ForwardUp (packet, from, bssid);
                }
            }
          else if (to.IsBroadcast () ||
                   to.IsMulticast () ||
                   toStation->IsAssociated ())
            {
              NS_LOG_DEBUG ("forwarding frame from="<<from<<", to="<<to);
              Ptr<Packet> copy = packet->Copy ();
              ForwardDown (packet, from, to, hdr);
              ForwardUp (copy, from, to);
            }
          else
            {
              ForwardUp (packet, from, to);
            }
        }
      else if (hdr->IsFromDs () &&
               hdr->IsToDs ()) 
        {
          // this is an AP-to-AP frame
          // we ignore for now.
        }
      else 
        {
          // we can ignore these frames since 
          // they are not targeted at the AP
        }
    }
  else if (hdr->IsMgt ())
    {
      if (hdr->IsProbeReq ()) 
        {
          NS_ASSERT (hdr->GetAddr1 ().IsBroadcast ());
          SendProbeResp (hdr->GetAddr2 ());
        }
      else if (hdr->GetAddr1 () == GetAddress ()) 
        {
          if (hdr->IsAssocReq ()) 
            {
              // first, verify that the the station's supported
              // rate set is compatible with our Basic Rate set
              MgtAssocRequestHeader assocReq;
              packet->RemoveHeader (assocReq);
              SupportedRates rates = assocReq.GetSupportedRates ();
              bool problem = false;
              for (uint32_t i = 0; i < m_stationManager->GetNBasicModes (); i++)
                {
                  WifiMode mode = m_stationManager->GetBasicMode (i);
                  if (!rates.IsSupportedRate (mode.GetDataRate ()))
                   {
                     problem = true;
                     break;
                   }
                }
             if (problem)
               {
                 // one of the Basic Rate set mode is not
                 // supported by the station. So, we return an assoc
                 // response with an error status.
                 SendAssocResp (hdr->GetAddr2 (), false);
               }
             else
               {
                 // station supports all rates in Basic Rate Set.
                 // record all its supported modes in its associated WifiRemoteStation
                 for (uint32_t j = 0; j < m_phy->GetNModes (); j++)
                   {
                     WifiMode mode = m_phy->GetMode (j);
                     if (rates.IsSupportedRate (mode.GetDataRate ()))
                       {
                         fromStation->AddSupportedMode (mode);
                       }
                    }
                  fromStation->RecordWaitAssocTxOk ();
                  // send assoc response with success status.
                  SendAssocResp (hdr->GetAddr2 (), true);
                }
            }
          else if (hdr->IsDisassociation ()) 
            {
              fromStation->RecordDisassociated ();
            } 
          else if (hdr->IsReassocReq ()) 
            {
              /* we don't support reassoc frames for now */
            } 
          else if (hdr->IsAuthentication () ||
                   hdr->IsDeauthentication ()) 
                 {
                   /*
                   */
                 } 
          else 
            {
              /* unknown mgt frame
              */
            }
        }
    }
}

void
QapWifiMac::DeaggregateAmsduAndForward (Ptr<Packet> aggregatedPacket, WifiMacHeader const *hdr)
{
  DeaggregatedMsdus packets = MsduAggregator::Deaggregate (aggregatedPacket);
  for (DeaggregatedMsdusCI i = packets.begin (); i != packets.end (); ++i)
    {
      if ((*i).second.GetDestinationAddr () == GetAddress ())
        {
          ForwardUp ((*i).first, (*i).second.GetSourceAddr (),
                     (*i).second.GetDestinationAddr ());
        }
      else
        {
          Mac48Address from = (*i).second.GetSourceAddr ();
          Mac48Address to = (*i).second.GetDestinationAddr ();
          NS_LOG_DEBUG ("forwarding QoS frame from="<<from<<", to="<<to);
          ForwardDown ((*i).first, from, to, hdr);
        }
    }
}

Ptr<EdcaTxopN>
QapWifiMac::GetVOQueue (void) const
{
  return m_voEdca;
}

Ptr<EdcaTxopN>
QapWifiMac::GetVIQueue (void) const
{
  return m_viEdca;
}

Ptr<EdcaTxopN>
QapWifiMac::GetBEQueue (void) const
{
  return m_beEdca;
}

Ptr<EdcaTxopN>
QapWifiMac::GetBKQueue (void) const
{
  return m_bkEdca;
}

void
QapWifiMac::SetVOQueue (Ptr<EdcaTxopN> voQueue)
{
  m_voEdca = voQueue;
  m_queues.insert (std::make_pair(AC_VO, m_voEdca));
  m_queues[AC_VO]->SetLow (m_low);
  m_queues[AC_VO]->SetManager (m_dcfManager);
  m_queues[AC_VO]->SetTypeOfStation (AP);
  m_queues[AC_VO]->SetTxMiddle (m_txMiddle);
  m_queues[AC_VO]->SetTxOkCallback (MakeCallback (&QapWifiMac::TxOk, this));
  m_queues[AC_VO]->SetTxFailedCallback (MakeCallback (&QapWifiMac::TxFailed, this));
}

void
QapWifiMac::SetVIQueue (Ptr<EdcaTxopN> viQueue)
{
  m_viEdca = viQueue;
  m_queues.insert (std::make_pair(AC_VI, m_viEdca));
  m_queues[AC_VI]->SetLow (m_low);
  m_queues[AC_VI]->SetManager (m_dcfManager);
  m_queues[AC_VI]->SetTypeOfStation (AP);
  m_queues[AC_VI]->SetTxMiddle (m_txMiddle);
  m_queues[AC_VI]->SetTxOkCallback (MakeCallback (&QapWifiMac::TxOk, this));
  m_queues[AC_VI]->SetTxFailedCallback (MakeCallback (&QapWifiMac::TxFailed, this));
}

void
QapWifiMac::SetBEQueue (Ptr<EdcaTxopN> beQueue)
{
  m_beEdca = beQueue;
  m_queues.insert (std::make_pair(AC_BE, m_beEdca));
  m_queues[AC_BE]->SetLow (m_low);
  m_queues[AC_BE]->SetManager (m_dcfManager);
  m_queues[AC_BE]->SetTypeOfStation (AP);
  m_queues[AC_BE]->SetTxMiddle (m_txMiddle);
  m_queues[AC_BE]->SetTxOkCallback (MakeCallback (&QapWifiMac::TxOk, this));
  m_queues[AC_BE]->SetTxFailedCallback (MakeCallback (&QapWifiMac::TxFailed, this));
}

void
QapWifiMac::SetBKQueue (Ptr<EdcaTxopN> bkQueue)
{
  m_bkEdca = bkQueue;
  m_queues.insert (std::make_pair(AC_BK, m_bkEdca));
  m_queues[AC_BK]->SetLow (m_low);
  m_queues[AC_BK]->SetManager (m_dcfManager);
  m_queues[AC_BK]->SetTypeOfStation (AP);
  m_queues[AC_BK]->SetTxMiddle (m_txMiddle);
  m_queues[AC_BK]->SetTxOkCallback (MakeCallback (&QapWifiMac::TxOk, this));
  m_queues[AC_BK]->SetTxFailedCallback (MakeCallback (&QapWifiMac::TxFailed, this));
}

}  //namespace ns3
