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
#include "ns3/boolean.h"

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
                   TimeValue (MicroSeconds (102400)),
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
                   MakePointerAccessor(&QapWifiMac::GetVOQueue),
                   MakePointerChecker<EdcaTxopN> ())
    .AddAttribute ("VI_EdcaTxopN",
                   "Queue that manages packets belonging to AC_VI access class",
                   PointerValue (),
                   MakePointerAccessor(&QapWifiMac::GetVIQueue),
                   MakePointerChecker<EdcaTxopN> ())
    .AddAttribute ("BE_EdcaTxopN",
                   "Queue that manages packets belonging to AC_BE access class",
                   PointerValue (),
                   MakePointerAccessor(&QapWifiMac::GetBEQueue),
                   MakePointerChecker<EdcaTxopN> ())
    .AddAttribute ("BK_EdcaTxopN",
                   "Queue that manages packets belonging to AC_BK access class",
                   PointerValue (),
                   MakePointerAccessor(&QapWifiMac::GetBKQueue),
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

  // Construct the EDCAFs. The ordering is important - highest
  // priority (see Table 9-1 in IEEE 802.11-2007) must be created
  // first.
  SetQueue (AC_VO);
  SetQueue (AC_VI);
  SetQueue (AC_BE);
  SetQueue (AC_BK);

  m_enableBeaconGeneration = false;
}

QapWifiMac::~QapWifiMac ()
{
  NS_LOG_FUNCTION (this);
}

void
QapWifiMac::DoDispose ()
{
  delete m_rxMiddle;
  delete m_txMiddle;
  delete m_dcfManager;
  m_low->Dispose ();
  m_rxMiddle = 0;
  m_txMiddle = 0;
  m_dcfManager = 0;
  m_low = 0;
  m_phy = 0;
  m_beaconDca = 0;
  m_enableBeaconGeneration = false;
  m_beaconEvent.Cancel ();
  m_stationManager = 0;
  for (Queues::iterator i = m_queues.begin (); i != m_queues.end (); ++i)
    {
      (*i).second = 0;
    }
  WifiMac::DoDispose ();
}

void
QapWifiMac::SetBeaconGeneration (bool enable)
{
  NS_LOG_FUNCTION (this << enable);
  if (!enable)
    {
      m_beaconEvent.Cancel ();
    }
  else if (enable && !m_enableBeaconGeneration)
    {
      m_beaconEvent = Simulator::ScheduleNow (&QapWifiMac::SendOneBeacon, this);
    }
  m_enableBeaconGeneration = enable;
}

bool
QapWifiMac::GetBeaconGeneration (void) const
{
  return m_enableBeaconGeneration;
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
}

void 
QapWifiMac::SetAckTimeout (Time ackTimeout)
{
  m_low->SetAckTimeout (ackTimeout);
}

void 
QapWifiMac::SetBasicBlockAckTimeout (Time blockAckTimeout)
{
  m_low->SetBasicBlockAckTimeout (blockAckTimeout);
}

void
QapWifiMac::SetCompressedBlockAckTimeout (Time blockAckTimeout)
{
  m_low->SetCompressedBlockAckTimeout (blockAckTimeout);
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
  return m_dcfManager->GetEifsNoDifs ();
}

Time 
QapWifiMac::GetAckTimeout (void) const
{
  return m_low->GetAckTimeout ();
}

Time 
QapWifiMac::GetBasicBlockAckTimeout () const
{
  return m_low->GetBasicBlockAckTimeout ();
}

Time 
QapWifiMac::GetCompressedBlockAckTimeout () const
{
  return m_low->GetCompressedBlockAckTimeout ();
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
  for (Queues::iterator i = m_queues.begin (); i != m_queues.end (); ++i)
    {
      (*i).second->SetWifiRemoteStationManager (stationManager);
    }
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
  if ((interval.GetMicroSeconds () % 1024) != 0)
    {
      NS_LOG_WARN ("beacon interval should be multiple of 1024us, see IEEE Std. 802.11-2007, section 11.1.1.1");
    }
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

void
QapWifiMac::ForwardDown (Ptr<const Packet> packet, Mac48Address from, Mac48Address to,
                         const WifiMacHeader *oldHdr)
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

  AcIndex ac = QosUtilsMapTidToAc (oldHdr->GetQosTid ());
  m_queues[ac]->Queue (packet, hdr);
}

void
QapWifiMac::Enqueue (Ptr<const Packet> packet, Mac48Address to, Mac48Address from)
{
  NS_LOG_FUNCTION (this << packet << from << to);
  if (to.IsBroadcast () || m_stationManager->IsAssociated (to))
    {
      ForwardDown (packet, from, to);
    }
}

void 
QapWifiMac::Enqueue (Ptr<const Packet> packet, Mac48Address to)
{
  NS_LOG_FUNCTION (this << packet << to);
  if (to.IsBroadcast () || m_stationManager->IsAssociated (to))
    {
      ForwardDown (packet, m_low->GetAddress (), to);
    }
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
QapWifiMac::TxOk (const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this);
  if (hdr.IsAssocResp () && 
      m_stationManager->IsWaitAssocTxOk (hdr.GetAddr1 ())) 
    {
      NS_LOG_DEBUG ("associated with sta="<<hdr.GetAddr1 ());
      m_stationManager->RecordGotAssocTxOk (hdr.GetAddr1 ());
    }
}

void
QapWifiMac::TxFailed (const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this);
  if (hdr.IsAssocResp () && 
      m_stationManager->IsWaitAssocTxOk (hdr.GetAddr1 ())) 
    {
      NS_LOG_DEBUG ("assoc failed with sta="<<hdr.GetAddr1 ());
      m_stationManager->RecordGotAssocTxFailed (hdr.GetAddr1 ());
    }
}

void
QapWifiMac::Receive (Ptr<Packet> packet, const WifiMacHeader *hdr)
{
  NS_LOG_FUNCTION (this << packet << hdr);

  Mac48Address from = hdr->GetAddr2 ();
  
  if (hdr->IsData ())
    {
      Mac48Address bssid = hdr->GetAddr1 ();
      if (!hdr->IsFromDs () && 
          hdr->IsToDs () &&
          bssid == GetAddress () &&
          m_stationManager->IsAssociated (from))
        {
          Mac48Address to = hdr->GetAddr3 ();
          
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
          else if (to.IsGroup () ||
                   m_stationManager->IsAssociated (to))
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
                         m_stationManager->AddSupportedMode (from, mode);
                       }
                   }
                 m_stationManager->RecordWaitAssocTxOk (from);
                 // send assoc response with success status.
                 SendAssocResp (hdr->GetAddr2 (), true);
               }
            }
          else if (hdr->IsDisassociation ()) 
            {
              m_stationManager->RecordDisassociated (from);
            } 
          else if (hdr->IsReassocReq ()) 
            {
              /* we don't support reassoc frames for now */
            }
          else if (hdr->IsAction ())
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
                       actionHdr.GetAction().blockAck == WifiActionHeader::BLOCK_ACK_ADDBA_RESPONSE)
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
                      m_low->DestroyBlockAckAgreement (hdr->GetAddr2 (), delBaHdr.GetTid ());
                    }
                  else
                    {
                      /* We must notify correct queue tear down of agreement */
                      AcIndex ac = QosUtilsMapTidToAc (delBaHdr.GetTid ());
                      m_queues[ac]->GotDelBaFrame (&delBaHdr, hdr->GetAddr2 ());
                    }
                }
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
QapWifiMac::DeaggregateAmsduAndForward (Ptr<Packet> aggregatedPacket, const WifiMacHeader *hdr)
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
  return m_queues.find (AC_VO)->second;
}

Ptr<EdcaTxopN>
QapWifiMac::GetVIQueue (void) const
{
  return m_queues.find (AC_VI)->second;
}

Ptr<EdcaTxopN>
QapWifiMac::GetBEQueue (void) const
{
  return m_queues.find (AC_BE)->second;
}

Ptr<EdcaTxopN>
QapWifiMac::GetBKQueue (void) const
{
  return m_queues.find (AC_BK)->second;
}

void
QapWifiMac::SetQueue (enum AcIndex ac)
{
  Ptr<EdcaTxopN> edca = CreateObject<EdcaTxopN> ();
  edca->SetLow (m_low);
  edca->SetManager (m_dcfManager);
  edca->SetTypeOfStation (AP);
  edca->SetTxMiddle (m_txMiddle);
  edca->SetTxOkCallback (MakeCallback (&QapWifiMac::TxOk, this));
  edca->SetTxFailedCallback (MakeCallback (&QapWifiMac::TxFailed, this));
  edca->SetAccessCategory (ac);
  edca->CompleteConfig ();
  m_queues.insert (std::make_pair(ac, edca));
}

void 
QapWifiMac::FinishConfigureStandard (enum WifiPhyStandard standard)
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
QapWifiMac::DoStart (void)
{
  m_beaconDca->Start ();
  for (Queues::iterator i = m_queues.begin (); i != m_queues.end (); ++i)
    {
      i->second->Start ();
    }
  m_beaconEvent.Cancel ();
  if (m_enableBeaconGeneration)
    {
      m_beaconEvent = Simulator::ScheduleNow (&QapWifiMac::SendOneBeacon, this);
    }
  WifiMac::DoStart ();
}

void
QapWifiMac::SendAddBaResponse (const MgtAddBaRequestHeader *reqHdr, Mac48Address originator)
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

}  //namespace ns3
