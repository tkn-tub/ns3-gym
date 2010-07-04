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
#include "ns3/simulator.h"
#include "ns3/string.h"
#include "ns3/pointer.h"
#include "ns3/boolean.h"

#include "qos-tag.h"
#include "edca-txop-n.h"
#include "qsta-wifi-mac.h"
#include "mac-low.h"
#include "dcf-manager.h"
#include "mac-rx-middle.h"
#include "mac-tx-middle.h"
#include "wifi-mac-header.h"
#include "msdu-aggregator.h"
#include "amsdu-subframe-header.h"
#include "mgt-headers.h"

NS_LOG_COMPONENT_DEFINE ("QstaWifiMac");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (QstaWifiMac);

TypeId
QstaWifiMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::QstaWifiMac")
    .SetParent<WifiMac> ()
    .AddConstructor<QstaWifiMac> ()
    .AddAttribute ("ProbeRequestTimeout", "The interval between two consecutive probe request attempts.",
                   TimeValue (Seconds (0.05)),
                   MakeTimeAccessor (&QstaWifiMac::m_probeRequestTimeout),
                   MakeTimeChecker ())
    .AddAttribute ("AssocRequestTimeout", "The interval between two consecutive assoc request attempts.",
                   TimeValue (Seconds (0.5)),
                   MakeTimeAccessor (&QstaWifiMac::m_assocRequestTimeout),
                   MakeTimeChecker ())
    .AddAttribute ("MaxMissedBeacons", 
                   "Number of beacons which much be consecutively missed before "
                   "we attempt to restart association.",
                   UintegerValue (10),
                   MakeUintegerAccessor (&QstaWifiMac::m_maxMissedBeacons),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ActiveProbing", "If true, we send probe requests. If false, we don't.",
                   BooleanValue (false),
                   MakeBooleanAccessor (&QstaWifiMac::SetActiveProbing),
                   MakeBooleanChecker ())
    .AddAttribute ("VO_EdcaTxopN",
                   "Queue that manages packets belonging to AC_VO access class",
                   PointerValue (),
                   MakePointerAccessor(&QstaWifiMac::GetVOQueue),
                   MakePointerChecker<EdcaTxopN> ())
    .AddAttribute ("VI_EdcaTxopN",
                   "Queue that manages packets belonging to AC_VI access class",
                   PointerValue (),
                   MakePointerAccessor(&QstaWifiMac::GetVIQueue),
                   MakePointerChecker<EdcaTxopN> ())
    .AddAttribute ("BE_EdcaTxopN",
                   "Queue that manages packets belonging to AC_BE access class",
                   PointerValue (),
                   MakePointerAccessor(&QstaWifiMac::GetBEQueue),
                   MakePointerChecker<EdcaTxopN> ())
    .AddAttribute ("BK_EdcaTxopN",
                   "Queue that manages packets belonging to AC_BK access class",
                   PointerValue (),
                   MakePointerAccessor(&QstaWifiMac::GetBKQueue),
                   MakePointerChecker<EdcaTxopN> ())
    ;
  return tid;
}

QstaWifiMac::QstaWifiMac ()
  : m_state (BEACON_MISSED),
    m_probeRequestEvent (),
    m_assocRequestEvent (),
    m_beaconWatchdogEnd (Seconds (0.0))
{
  NS_LOG_FUNCTION (this);
  m_rxMiddle = new MacRxMiddle ();
  m_rxMiddle->SetForwardCallback (MakeCallback (&QstaWifiMac::Receive, this));
  /*TxMiddle can be shared between all queues */
  m_txMiddle= new MacTxMiddle ();
  
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

QstaWifiMac::~QstaWifiMac ()
{
  NS_LOG_FUNCTION (this);
}

void
QstaWifiMac::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_rxMiddle;
  delete m_txMiddle;
  delete m_dcfManager;
  m_low->Dispose ();
  m_rxMiddle = 0;
  m_txMiddle = 0;
  m_low = 0;
  m_phy = 0;
  m_dcfManager = 0;
  m_stationManager = 0;
  for (Queues::iterator i = m_queues.begin (); i != m_queues.end (); ++i)
    {
      (*i).second = 0;
    }
  WifiMac::DoDispose ();
}

void
QstaWifiMac::SetSlot (Time slotTime)
{
  NS_LOG_FUNCTION (this << slotTime);
  m_dcfManager->SetSlot (slotTime);
  m_low->SetSlotTime (slotTime);
}

void 
QstaWifiMac::SetSifs (Time sifs)
{
  NS_LOG_FUNCTION (this << sifs);
  m_dcfManager->SetSifs (sifs);
  m_low->SetSifs (sifs);
}

void
QstaWifiMac::SetEifsNoDifs (Time eifsNoDifs)
{
  NS_LOG_FUNCTION (this << eifsNoDifs);
  m_dcfManager->SetEifsNoDifs (eifsNoDifs);
}

void
QstaWifiMac::SetAckTimeout (Time ackTimeout)
{
  m_low->SetAckTimeout (ackTimeout);
}

void 
QstaWifiMac::SetBasicBlockAckTimeout (Time blockAckTimeout)
{
  m_low->SetBasicBlockAckTimeout (blockAckTimeout);
}

void 
QstaWifiMac::SetCompressedBlockAckTimeout (Time blockAckTimeout)
{
  m_low->SetCompressedBlockAckTimeout (blockAckTimeout);
}

void
QstaWifiMac::SetCtsTimeout (Time ctsTimeout)
{
  m_low->SetCtsTimeout (ctsTimeout);
}

void
QstaWifiMac::SetPifs (Time pifs)
{
  m_low->SetPifs (pifs);
}

Time
QstaWifiMac::GetSlot (void) const
{
  return m_low->GetSlotTime ();
}

Time
QstaWifiMac::GetSifs (void) const
{
  return m_low->GetSifs ();
}

Time 
QstaWifiMac::GetEifsNoDifs (void) const
{
  return m_dcfManager->GetEifsNoDifs ();
}

Time
QstaWifiMac::GetAckTimeout (void) const
{
 return m_low->GetAckTimeout ();
}

Time 
QstaWifiMac::GetBasicBlockAckTimeout (void) const
{
  return m_low->GetBasicBlockAckTimeout ();
}

Time 
QstaWifiMac::GetCompressedBlockAckTimeout (void) const
{
  return m_low->GetCompressedBlockAckTimeout ();
}

Time
QstaWifiMac::GetCtsTimeout (void) const
{
  return m_low->GetCtsTimeout ();
}

Time 
QstaWifiMac::GetPifs (void) const
{
  return m_low->GetPifs ();
}

void
QstaWifiMac::SetWifiPhy (Ptr<WifiPhy> phy)
{
  m_phy = phy;
  m_dcfManager->SetupPhyListener (phy);
  m_low->SetPhy (phy);
}

void
QstaWifiMac::SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> stationManager)
{
  m_stationManager = stationManager;
  m_queues[AC_VO]->SetWifiRemoteStationManager (stationManager);
  m_queues[AC_VI]->SetWifiRemoteStationManager (stationManager);
  m_queues[AC_BE]->SetWifiRemoteStationManager (stationManager);
  m_queues[AC_BK]->SetWifiRemoteStationManager (stationManager);
  m_low->SetWifiRemoteStationManager (stationManager);
}

void
QstaWifiMac::SetForwardUpCallback (Callback<void,Ptr<Packet>, Mac48Address, Mac48Address> upCallback)
{
  m_forwardUp = upCallback;
}

void
QstaWifiMac::SetLinkUpCallback (Callback<void> linkUp)
{
  m_linkUp = linkUp;
}

void
QstaWifiMac::SetLinkDownCallback (Callback<void> linkDown)
{
  m_linkDown = linkDown;
}

Mac48Address
QstaWifiMac::GetAddress (void) const
{
  return m_low->GetAddress ();
}

Ssid
QstaWifiMac::GetSsid (void) const
{
  return m_ssid;
}

Mac48Address
QstaWifiMac::GetBssid () const
{
  return m_low->GetBssid ();
}

void
QstaWifiMac::SetAddress (Mac48Address address)
{
  NS_LOG_FUNCTION (this << address);
  m_low->SetAddress (address);
}

void
QstaWifiMac::SetSsid (Ssid ssid)
{
  NS_LOG_FUNCTION (this << ssid);
  m_ssid = ssid;
}

void
QstaWifiMac::SetMaxMissedBeacons (uint32_t missed)
{
  NS_LOG_FUNCTION (this << missed);
  m_maxMissedBeacons = missed;
}

void
QstaWifiMac::SetProbeRequestTimeout (Time timeout)
{
  NS_LOG_FUNCTION (this << timeout);
  m_probeRequestTimeout = timeout;
}

void
QstaWifiMac::SetAssocRequestTimeout (Time timeout)
{
  NS_LOG_FUNCTION (this << timeout);
  m_assocRequestTimeout = timeout;
}

void
QstaWifiMac::StartActiveAssociation (void)
{
  NS_LOG_FUNCTION (this);
  TryToEnsureAssociated ();
}

Mac48Address
QstaWifiMac::GetBroadcastBssid (void)
{
  return Mac48Address::GetBroadcast ();
}

void 
QstaWifiMac::SetBssid (Mac48Address bssid)
{
  NS_LOG_FUNCTION (this << bssid);
  m_low->SetBssid (bssid);
}

void
QstaWifiMac::SetActiveProbing (bool enable)
{
  NS_LOG_FUNCTION (this << enable);
  if (enable)
    {
      TryToEnsureAssociated ();
    }
  else
    {
      m_probeRequestEvent.Cancel ();
    }
}

void
QstaWifiMac::ForwardUp (Ptr<Packet> packet, Mac48Address from, Mac48Address to)
{
  NS_LOG_FUNCTION (this << packet << from << to);
  m_forwardUp (packet, from, to);
}

void
QstaWifiMac::SendProbeRequest (void)
{
  NS_LOG_FUNCTION (this);
  WifiMacHeader hdr;
  hdr.SetProbeReq ();
  hdr.SetAddr1 (GetBroadcastBssid ());
  hdr.SetAddr2 (GetAddress ());
  hdr.SetAddr3 (GetBroadcastBssid ());
  hdr.SetDsNotFrom ();
  hdr.SetDsNotTo ();
  Ptr<Packet> packet = Create<Packet> ();
  MgtProbeRequestHeader probe;
  probe.SetSsid (GetSsid ());
  probe.SetSupportedRates (GetSupportedRates ());
  packet->AddHeader (probe);

  /* Which is correct queue for management frames ? */
  m_queues[AC_VO]->Queue (packet, hdr);

  m_probeRequestEvent = Simulator::Schedule (m_probeRequestTimeout,
                                             &QstaWifiMac::ProbeRequestTimeout, this);
}

void
QstaWifiMac::SendAssociationRequest (void)
{
  NS_LOG_FUNCTION (this << GetBssid ());
  WifiMacHeader hdr;
  hdr.SetAssocReq ();
  hdr.SetAddr1 (GetBssid ());
  hdr.SetAddr2 (GetAddress ());
  hdr.SetAddr3 (GetBssid ());
  hdr.SetDsNotFrom ();
  hdr.SetDsNotTo ();
  Ptr<Packet> packet = Create<Packet> ();
  MgtAssocRequestHeader assoc;
  assoc.SetSsid (GetSsid ());
  assoc.SetSupportedRates (GetSupportedRates ());
  packet->AddHeader (assoc);
  
  /* Which is correct queue for management frames ? */
  m_queues[AC_VO]->Queue (packet, hdr);

  m_assocRequestEvent = Simulator::Schedule (m_assocRequestTimeout,
                                             &QstaWifiMac::AssocRequestTimeout, this);
}

void
QstaWifiMac::TryToEnsureAssociated (void)
{
  NS_LOG_FUNCTION (this);
  switch (m_state) {
  case ASSOCIATED:
    return;
    break;
  case WAIT_PROBE_RESP:
    /* we have sent a probe request earlier so we
       do not need to re-send a probe request immediately.
       We just need to wait until probe-request-timeout
       or until we get a probe response
     */
    break;
  case BEACON_MISSED:
    /* we were associated but we missed a bunch of beacons
     * so we should assume we are not associated anymore.
     * We try to initiate a probe request now.
     */
    m_linkDown ();
    m_state = WAIT_PROBE_RESP;
    SendProbeRequest ();
    break;
  case WAIT_ASSOC_RESP:
    /* we have sent an assoc request so we do not need to
       re-send an assoc request right now. We just need to
       wait until either assoc-request-timeout or until
       we get an assoc response.
     */
    break;
  case REFUSED:
    /* we have sent an assoc request and received a negative
       assoc resp. We wait until someone restarts an 
       association with a given ssid.
     */
    break;
  }
}

void
QstaWifiMac::AssocRequestTimeout (void)
{
  NS_LOG_FUNCTION (this);
  m_state = WAIT_ASSOC_RESP;
  SendAssociationRequest ();
}

void
QstaWifiMac::ProbeRequestTimeout (void)
{
  NS_LOG_FUNCTION (this);
  m_state = WAIT_PROBE_RESP;
  SendProbeRequest ();
}

void
QstaWifiMac::MissedBeacons (void)
{
  NS_LOG_FUNCTION (this);
  if (m_beaconWatchdogEnd > Simulator::Now ())
    {
      m_beaconWatchdog = Simulator::Schedule (m_beaconWatchdogEnd - Simulator::Now (),
                                              &QstaWifiMac::MissedBeacons, this);
      return;
    }
  NS_LOG_DEBUG ("beacon missed");
  m_state = BEACON_MISSED;
  TryToEnsureAssociated ();
}

void
QstaWifiMac::RestartBeaconWatchdog (Time delay)
{
  NS_LOG_FUNCTION (this << delay);
  m_beaconWatchdogEnd = std::max (Simulator::Now () + delay, m_beaconWatchdogEnd);
  if (Simulator::GetDelayLeft (m_beaconWatchdog) < delay &&
                               m_beaconWatchdog.IsExpired ())
    {
      NS_LOG_DEBUG ("really restart watchdog.");
      m_beaconWatchdog = Simulator::Schedule (delay, &QstaWifiMac::MissedBeacons, this);
    }
}

bool
QstaWifiMac::IsAssociated (void) const
{
  return m_state == ASSOCIATED;
}

bool 
QstaWifiMac::IsWaitAssocResp (void) const
{
  return m_state == WAIT_ASSOC_RESP;
}

void
QstaWifiMac::Enqueue (Ptr<const Packet> packet, Mac48Address to)
{
  NS_LOG_FUNCTION (this << packet << to);
  if (!IsAssociated ()) 
    {
      TryToEnsureAssociated ();
      return;
    }
  WifiMacHeader hdr;

  hdr.SetType (WIFI_MAC_QOSDATA);
  hdr.SetQosAckPolicy (WifiMacHeader::NORMAL_ACK);
  hdr.SetQosNoAmsdu ();
  hdr.SetQosNoEosp ();
  /* Transmission of multiple frames in the same 
     Txop is not supported for now */
  hdr.SetQosTxopLimit (0);

  hdr.SetAddr1 (GetBssid ());
  hdr.SetAddr2 (m_low->GetAddress ());
  hdr.SetAddr3 (to);
  hdr.SetDsNotFrom ();
  hdr.SetDsTo ();
  
  uint8_t tid = QosUtilsGetTidForPacket (packet);
  if (tid < 8)
    {
      hdr.SetQosTid (tid);
      AcIndex ac = QosUtilsMapTidToAc (tid);
      m_queues[ac]->Queue (packet, hdr);
    }
  else
    {
      //packet is considerated belonging to BestEffort Access Class (AC_BE)
      hdr.SetQosTid (0);
      m_queues[AC_BE]->Queue (packet, hdr);
    }
}

bool
QstaWifiMac::SupportsSendFrom (void) const
{
  return true;
}

void
QstaWifiMac::Receive (Ptr<Packet> packet, const WifiMacHeader *hdr)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (!hdr->IsCtl ());
  if (hdr->GetAddr1 () != GetAddress () &&
      !hdr->GetAddr1 ().IsGroup ())
    {
      NS_LOG_LOGIC ("packet is not for us");
    }
  else if (hdr->IsData ())
    {
      if (!IsAssociated ())
        { 
          NS_LOG_LOGIC ("Received data frame while not associated: ignore");
          return;
        }
      if (!(hdr->IsFromDs () && !hdr->IsToDs ()))
        {
          NS_LOG_LOGIC ("Received data frame not from the DS: ignore");
          return;
        }
      if (hdr->GetAddr2 () != GetBssid ())
        {
          NS_LOG_LOGIC ("Received data frame not from the BSS we are associated with: ignore");
          return;
        }
      if (hdr->GetAddr3 () != GetAddress ())
        {
          if (hdr->IsQosData ())
            {
              if (hdr->IsQosAmsdu ())
                {
                  NS_ASSERT (hdr->GetAddr3 () == GetBssid ());
                  DeaggregateAmsduAndForward (packet, hdr);
                  packet = 0;
                }
              else
                {
                  ForwardUp (packet, hdr->GetAddr3 (), hdr->GetAddr1 ());
                }
            }
          else
            {
              ForwardUp (packet, hdr->GetAddr3 (), hdr->GetAddr1 ());
            }
        }
    }
  else if (hdr->IsBeacon ()) 
    {
      MgtBeaconHeader beacon;
      packet->RemoveHeader (beacon);
      bool goodBeacon = false;
      if (GetSsid ().IsBroadcast () ||
          beacon.GetSsid ().IsEqual (GetSsid ()))
        {
          goodBeacon = true;
        }
      if ((IsWaitAssocResp () || IsAssociated ()) && hdr->GetAddr3 () != GetBssid ())
        {
          goodBeacon = false;
        }
      if (goodBeacon)
        {
          Time delay = MicroSeconds (beacon.GetBeaconIntervalUs () * m_maxMissedBeacons);
          RestartBeaconWatchdog (delay);
          SetBssid (hdr->GetAddr3 ());
        }
      if (goodBeacon && m_state == BEACON_MISSED) 
        {
          m_state = WAIT_ASSOC_RESP;
          SendAssociationRequest ();
        }
    }
  else if (hdr->IsProbeResp ()) 
    {
      if (m_state == WAIT_PROBE_RESP) 
        {
          MgtProbeResponseHeader probeResp;
          packet->RemoveHeader (probeResp);
          if (!probeResp.GetSsid ().IsEqual (GetSsid ()))
            {
              //not a probe resp for our ssid.
              return;
            }
          SetBssid (hdr->GetAddr3 ());
          Time delay = MicroSeconds (probeResp.GetBeaconIntervalUs () * m_maxMissedBeacons);
          RestartBeaconWatchdog (delay);
          if (m_probeRequestEvent.IsRunning ()) 
            {
              m_probeRequestEvent.Cancel ();
            }
          m_state = WAIT_ASSOC_RESP;
          SendAssociationRequest ();
        }
    } 
  else if (hdr->IsAssocResp ()) 
    {
      if (m_state == WAIT_ASSOC_RESP) 
        {
          MgtAssocResponseHeader assocResp;
          packet->RemoveHeader (assocResp);
          if (m_assocRequestEvent.IsRunning ()) 
            {
              m_assocRequestEvent.Cancel ();
            }
          if (assocResp.GetStatusCode ().IsSuccess ()) 
            {
              m_state = ASSOCIATED;
              NS_LOG_DEBUG ("assoc completed"); 
              SupportedRates rates = assocResp.GetSupportedRates ();
              for (uint32_t i = 0; i < m_phy->GetNModes (); i++)
                {
                  WifiMode mode = m_phy->GetMode (i);
                  if (rates.IsSupportedRate (mode.GetDataRate ()))
                    {
                      m_stationManager->AddSupportedMode (hdr->GetAddr2 (), mode);
                      if (rates.IsBasicRate (mode.GetDataRate ()))
                        {
                          m_stationManager->AddBasicMode (mode);
                        }
                    }
                }
              if (!m_linkUp.IsNull ())
                {
                  m_linkUp ();
                }
            } 
          else 
            {
              NS_LOG_DEBUG ("assoc refused");
              m_state = REFUSED;
            }
        }
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
}

SupportedRates
QstaWifiMac::GetSupportedRates (void) const
{
  SupportedRates rates;
  for (uint32_t i = 0; i < m_phy->GetNModes (); i++)
    {
      WifiMode mode = m_phy->GetMode (i);
      rates.AddSupportedRate (mode.GetDataRate ());
    }
  return rates;
}

void
QstaWifiMac::DeaggregateAmsduAndForward (Ptr<Packet> aggregatedPacket, const WifiMacHeader *hdr)
{
  DeaggregatedMsdus packets = MsduAggregator::Deaggregate (aggregatedPacket);
  for (DeaggregatedMsdusCI i = packets.begin (); i != packets.end (); ++i)
    {
      ForwardUp ((*i).first, (*i).second.GetSourceAddr (),
                 (*i).second.GetDestinationAddr ());
    }
}

Ptr<EdcaTxopN>
QstaWifiMac::GetVOQueue (void) const
{
  return m_queues.find (AC_VO)->second;
}

Ptr<EdcaTxopN>
QstaWifiMac::GetVIQueue (void) const
{
  return m_queues.find (AC_VI)->second;
}

Ptr<EdcaTxopN>
QstaWifiMac::GetBEQueue (void) const
{
  return m_queues.find (AC_BE)->second;
}

Ptr<EdcaTxopN>
QstaWifiMac::GetBKQueue (void) const
{
  return m_queues.find (AC_BK)->second;
}

void
QstaWifiMac::SetQueue (enum AcIndex ac)
{
  Ptr<EdcaTxopN> edca = CreateObject<EdcaTxopN> ();
  edca->SetLow (m_low);
  edca->SetManager (m_dcfManager);
  edca->SetTypeOfStation (STA);
  edca->SetTxMiddle (m_txMiddle);
  edca->SetAccessCategory (ac);
  edca->CompleteConfig ();
  m_queues.insert (std::make_pair(ac, edca));
}
void
QstaWifiMac::DoStart ()
{
  for (Queues::iterator i = m_queues.begin (); i != m_queues.end (); ++i)
    {
      i->second->Start ();
    }
  WifiMac::DoStart ();
}

void 
QstaWifiMac::FinishConfigureStandard (enum WifiPhyStandard standard)
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
QstaWifiMac::SendAddBaResponse (const MgtAddBaRequestHeader *reqHdr, Mac48Address originator)
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
