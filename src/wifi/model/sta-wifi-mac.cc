/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Mirko Banchi <mk.banchi@gmail.com>
 */

#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "sta-wifi-mac.h"
#include "wifi-phy.h"
#include "mac-low.h"
#include "mgt-headers.h"
#include "snr-tag.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("StaWifiMac");

NS_OBJECT_ENSURE_REGISTERED (StaWifiMac);

TypeId
StaWifiMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::StaWifiMac")
    .SetParent<InfrastructureWifiMac> ()
    .SetGroupName ("Wifi")
    .AddConstructor<StaWifiMac> ()
    .AddAttribute ("ProbeRequestTimeout", "The duration to actively probe the channel.",
                   TimeValue (Seconds (0.05)),
                   MakeTimeAccessor (&StaWifiMac::m_probeRequestTimeout),
                   MakeTimeChecker ())
    .AddAttribute ("WaitBeaconTimeout", "The duration to dwell on a channel while passively scanning for beacon",
                   TimeValue (MilliSeconds (120)),
                   MakeTimeAccessor (&StaWifiMac::m_waitBeaconTimeout),
                   MakeTimeChecker ())
    .AddAttribute ("AssocRequestTimeout", "The interval between two consecutive association request attempts.",
                   TimeValue (Seconds (0.5)),
                   MakeTimeAccessor (&StaWifiMac::m_assocRequestTimeout),
                   MakeTimeChecker ())
    .AddAttribute ("MaxMissedBeacons",
                   "Number of beacons which much be consecutively missed before "
                   "we attempt to restart association.",
                   UintegerValue (10),
                   MakeUintegerAccessor (&StaWifiMac::m_maxMissedBeacons),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ActiveProbing",
                   "If true, we send probe requests. If false, we don't."
                   "NOTE: if more than one STA in your simulation is using active probing, "
                   "you should enable it at a different simulation time for each STA, "
                   "otherwise all the STAs will start sending probes at the same time resulting in collisions. "
                   "See bug 1060 for more info.",
                   BooleanValue (false),
                   MakeBooleanAccessor (&StaWifiMac::SetActiveProbing, &StaWifiMac::GetActiveProbing),
                   MakeBooleanChecker ())
    .AddTraceSource ("Assoc", "Associated with an access point.",
                     MakeTraceSourceAccessor (&StaWifiMac::m_assocLogger),
                     "ns3::Mac48Address::TracedCallback")
    .AddTraceSource ("DeAssoc", "Association with an access point lost.",
                     MakeTraceSourceAccessor (&StaWifiMac::m_deAssocLogger),
                     "ns3::Mac48Address::TracedCallback")
    .AddTraceSource ("BeaconArrival",
                     "Time of beacons arrival from associated AP",
                     MakeTraceSourceAccessor (&StaWifiMac::m_beaconArrival),
                     "ns3::Time::TracedCallback")
  ;
  return tid;
}

StaWifiMac::StaWifiMac ()
  : m_state (UNASSOCIATED),
    m_waitBeaconEvent (),
    m_probeRequestEvent (),
    m_assocRequestEvent (),
    m_beaconWatchdogEnd (Seconds (0))
{
  NS_LOG_FUNCTION (this);

  //Let the lower layers know that we are acting as a non-AP STA in
  //an infrastructure BSS.
  SetTypeOfStation (STA);
}

void
StaWifiMac::DoInitialize (void)
{
  NS_LOG_FUNCTION (this);
  StartScanning ();
}

StaWifiMac::~StaWifiMac ()
{
  NS_LOG_FUNCTION (this);
}

void
StaWifiMac::SetActiveProbing (bool enable)
{
  NS_LOG_FUNCTION (this << enable);
  m_activeProbing = enable;
  if (m_state == WAIT_PROBE_RESP || m_state == WAIT_BEACON)
    {
      NS_LOG_DEBUG ("STA is still scanning, reset scanning process");
      StartScanning ();
    }
}

bool
StaWifiMac::GetActiveProbing (void) const
{
  return m_activeProbing;
}

void
StaWifiMac::SetWifiPhy (const Ptr<WifiPhy> phy)
{
  NS_LOG_FUNCTION (this << phy);
  RegularWifiMac::SetWifiPhy (phy);
  m_phy->SetCapabilitiesChangedCallback (MakeCallback (&StaWifiMac::PhyCapabilitiesChanged, this));
}

void
StaWifiMac::SetWifiRemoteStationManager (const Ptr<WifiRemoteStationManager> stationManager)
{
  NS_LOG_FUNCTION (this << stationManager);
  RegularWifiMac::SetWifiRemoteStationManager (stationManager);
  m_stationManager->SetPcfSupported (GetPcfSupported ());
}

void
StaWifiMac::SendProbeRequest (void)
{
  NS_LOG_FUNCTION (this);
  WifiMacHeader hdr;
  hdr.SetType (WIFI_MAC_MGT_PROBE_REQUEST);
  hdr.SetAddr1 (Mac48Address::GetBroadcast ());
  hdr.SetAddr2 (GetAddress ());
  hdr.SetAddr3 (Mac48Address::GetBroadcast ());
  hdr.SetDsNotFrom ();
  hdr.SetDsNotTo ();
  Ptr<Packet> packet = Create<Packet> ();
  MgtProbeRequestHeader probe;
  probe.SetSsid (GetSsid ());
  probe.SetSupportedRates (GetSupportedRates ());
  if (GetHtSupported () || GetVhtSupported () || GetHeSupported ())
    {
      probe.SetExtendedCapabilities (GetExtendedCapabilities ());
      probe.SetHtCapabilities (GetHtCapabilities ());
    }
  if (GetVhtSupported () || GetHeSupported ())
    {
      probe.SetVhtCapabilities (GetVhtCapabilities ());
    }
  if (GetHeSupported ())
    {
      probe.SetHeCapabilities (GetHeCapabilities ());
    }
  packet->AddHeader (probe);

  //The standard is not clear on the correct queue for management
  //frames if we are a QoS AP. The approach taken here is to always
  //use the non-QoS for these regardless of whether we have a QoS
  //association or not.
  m_txop->Queue (packet, hdr);
}

void
StaWifiMac::SendAssociationRequest (bool isReassoc)
{
  NS_LOG_FUNCTION (this << GetBssid () << isReassoc);
  WifiMacHeader hdr;
  hdr.SetType (isReassoc ? WIFI_MAC_MGT_REASSOCIATION_REQUEST : WIFI_MAC_MGT_ASSOCIATION_REQUEST);
  hdr.SetAddr1 (GetBssid ());
  hdr.SetAddr2 (GetAddress ());
  hdr.SetAddr3 (GetBssid ());
  hdr.SetDsNotFrom ();
  hdr.SetDsNotTo ();
  Ptr<Packet> packet = Create<Packet> ();
  if (!isReassoc)
    {
      MgtAssocRequestHeader assoc;
      assoc.SetSsid (GetSsid ());
      assoc.SetSupportedRates (GetSupportedRates ());
      assoc.SetCapabilities (GetCapabilities ());
      assoc.SetListenInterval (0);
      if (GetHtSupported () || GetVhtSupported () || GetHeSupported ())
        {
          assoc.SetExtendedCapabilities (GetExtendedCapabilities ());
          assoc.SetHtCapabilities (GetHtCapabilities ());
        }
      if (GetVhtSupported () || GetHeSupported ())
        {
          assoc.SetVhtCapabilities (GetVhtCapabilities ());
        }
      if (GetHeSupported ())
        {
          assoc.SetHeCapabilities (GetHeCapabilities ());
        }
      packet->AddHeader (assoc);
    }
  else
    {
      MgtReassocRequestHeader reassoc;
      reassoc.SetCurrentApAddress (GetBssid ());
      reassoc.SetSsid (GetSsid ());
      reassoc.SetSupportedRates (GetSupportedRates ());
      reassoc.SetCapabilities (GetCapabilities ());
      reassoc.SetListenInterval (0);
      if (GetHtSupported () || GetVhtSupported () || GetHeSupported ())
        {
          reassoc.SetExtendedCapabilities (GetExtendedCapabilities ());
          reassoc.SetHtCapabilities (GetHtCapabilities ());
        }
      if (GetVhtSupported () || GetHeSupported ())
        {
          reassoc.SetVhtCapabilities (GetVhtCapabilities ());
        }
      if (GetHeSupported ())
        {
          reassoc.SetHeCapabilities (GetHeCapabilities ());
        }
      packet->AddHeader (reassoc);
    }

  //The standard is not clear on the correct queue for management
  //frames if we are a QoS AP. The approach taken here is to always
  //use the non-QoS for these regardless of whether we have a QoS
  //association or not.
  m_txop->Queue (packet, hdr);

  if (m_assocRequestEvent.IsRunning ())
    {
      m_assocRequestEvent.Cancel ();
    }
  m_assocRequestEvent = Simulator::Schedule (m_assocRequestTimeout,
                                             &StaWifiMac::AssocRequestTimeout, this);
}

void
StaWifiMac::SendCfPollResponse (void)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (GetPcfSupported ());
  m_txop->SendCfFrame (WIFI_MAC_DATA_NULL, GetBssid ());
}

void
StaWifiMac::TryToEnsureAssociated (void)
{
  NS_LOG_FUNCTION (this);
  switch (m_state)
    {
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
    case WAIT_BEACON:
      /* we have initiated passive scanning, continue to wait
         and gather beacons
       */
      break;
    case UNASSOCIATED:
      /* we were associated but we missed a bunch of beacons
       * so we should assume we are not associated anymore.
       * We try to initiate a scan now.
       */
      m_linkDown ();
      StartScanning ();
      break;
    case WAIT_ASSOC_RESP:
      /* we have sent an association request so we do not need to
         re-send an association request right now. We just need to
         wait until either assoc-request-timeout or until
         we get an association response.
       */
      break;
    case REFUSED:
      /* we have sent an association request and received a negative
         association response. We wait until someone restarts an
         association with a given ssid.
       */
      break;
    }
}

void
StaWifiMac::StartScanning (void)
{
  NS_LOG_FUNCTION (this);
  m_candidateAps.clear ();
  if (m_probeRequestEvent.IsRunning ())
    {
      m_probeRequestEvent.Cancel ();
    }
  if (m_waitBeaconEvent.IsRunning ())
    {
      m_waitBeaconEvent.Cancel ();
    }
  if (GetActiveProbing ())
    {
      SetState (WAIT_PROBE_RESP);
      SendProbeRequest ();
      m_probeRequestEvent = Simulator::Schedule (m_probeRequestTimeout,
                                                 &StaWifiMac::ScanningTimeout,
                                                 this);
    }
  else
    {
      SetState (WAIT_BEACON);
      m_waitBeaconEvent = Simulator::Schedule (m_waitBeaconTimeout,
                                               &StaWifiMac::ScanningTimeout,
                                               this);
    }
}

void
StaWifiMac::ScanningTimeout (void)
{
  NS_LOG_FUNCTION (this);
  if (!m_candidateAps.empty ())
    {
      ApInfo bestAp = m_candidateAps.front();
      m_candidateAps.erase(m_candidateAps.begin ());
      NS_LOG_DEBUG ("Attempting to associate with BSSID " << bestAp.m_bssid);
      Time beaconInterval;
      if (bestAp.m_activeProbing)
        {
          UpdateApInfoFromProbeResp (bestAp.m_probeResp, bestAp.m_apAddr, bestAp.m_bssid);
          beaconInterval = MicroSeconds (bestAp.m_probeResp.GetBeaconIntervalUs ());
        }
      else
        {
          UpdateApInfoFromBeacon (bestAp.m_beacon, bestAp.m_apAddr, bestAp.m_bssid);
          beaconInterval = MicroSeconds (bestAp.m_beacon.GetBeaconIntervalUs ());
        }

      Time delay = beaconInterval * m_maxMissedBeacons;
      RestartBeaconWatchdog (delay);
      SetState (WAIT_ASSOC_RESP);
      SendAssociationRequest (false);
    }
  else
    {
      NS_LOG_DEBUG ("Exhausted list of candidate AP; restart scanning");
      StartScanning ();
    }
}

void
StaWifiMac::AssocRequestTimeout (void)
{
  NS_LOG_FUNCTION (this);
  SetState (WAIT_ASSOC_RESP);
  SendAssociationRequest (false);
}

void
StaWifiMac::MissedBeacons (void)
{
  NS_LOG_FUNCTION (this);
  if (m_beaconWatchdogEnd > Simulator::Now ())
    {
      if (m_beaconWatchdog.IsRunning ())
        {
          m_beaconWatchdog.Cancel ();
        }
      m_beaconWatchdog = Simulator::Schedule (m_beaconWatchdogEnd - Simulator::Now (),
                                              &StaWifiMac::MissedBeacons, this);
      return;
    }
  NS_LOG_DEBUG ("beacon missed");
  SetState (UNASSOCIATED);
  TryToEnsureAssociated ();
}

void
StaWifiMac::RestartBeaconWatchdog (Time delay)
{
  NS_LOG_FUNCTION (this << delay);
  m_beaconWatchdogEnd = std::max (Simulator::Now () + delay, m_beaconWatchdogEnd);
  if (Simulator::GetDelayLeft (m_beaconWatchdog) < delay
      && m_beaconWatchdog.IsExpired ())
    {
      NS_LOG_DEBUG ("really restart watchdog.");
      m_beaconWatchdog = Simulator::Schedule (delay, &StaWifiMac::MissedBeacons, this);
    }
}

bool
StaWifiMac::IsAssociated (void) const
{
  return m_state == ASSOCIATED;
}

bool
StaWifiMac::IsWaitAssocResp (void) const
{
  return m_state == WAIT_ASSOC_RESP;
}

void
StaWifiMac::Enqueue (Ptr<const Packet> packet, Mac48Address to)
{
  NS_LOG_FUNCTION (this << packet << to);
  if (!IsAssociated ())
    {
      NotifyTxDrop (packet);
      TryToEnsureAssociated ();
      return;
    }
  WifiMacHeader hdr;

  //If we are not a QoS AP then we definitely want to use AC_BE to
  //transmit the packet. A TID of zero will map to AC_BE (through \c
  //QosUtilsMapTidToAc()), so we use that as our default here.
  uint8_t tid = 0;

  //For now, an AP that supports QoS does not support non-QoS
  //associations, and vice versa. In future the AP model should
  //support simultaneously associated QoS and non-QoS STAs, at which
  //point there will need to be per-association QoS state maintained
  //by the association state machine, and consulted here.
  if (GetQosSupported ())
    {
      hdr.SetType (WIFI_MAC_QOSDATA);
      hdr.SetQosAckPolicy (WifiMacHeader::NORMAL_ACK);
      hdr.SetQosNoEosp ();
      hdr.SetQosNoAmsdu ();
      //Transmission of multiple frames in the same TXOP is not
      //supported for now
      hdr.SetQosTxopLimit (0);

      //Fill in the QoS control field in the MAC header
      tid = QosUtilsGetTidForPacket (packet);
      //Any value greater than 7 is invalid and likely indicates that
      //the packet had no QoS tag, so we revert to zero, which'll
      //mean that AC_BE is used.
      if (tid > 7)
        {
          tid = 0;
        }
      hdr.SetQosTid (tid);
    }
  else
    {
      hdr.SetType (WIFI_MAC_DATA);
    }
  if (GetQosSupported () || GetHtSupported () || GetVhtSupported () || GetHeSupported ())
    {
      hdr.SetNoOrder (); // explicitly set to 0 for the time being since HT/VHT/HE control field is not yet implemented (set it to 1 when implemented)
    }

  hdr.SetAddr1 (GetBssid ());
  hdr.SetAddr2 (m_low->GetAddress ());
  hdr.SetAddr3 (to);
  hdr.SetDsNotFrom ();
  hdr.SetDsTo ();

  if (GetQosSupported ())
    {
      //Sanity check that the TID is valid
      NS_ASSERT (tid < 8);
      m_edca[QosUtilsMapTidToAc (tid)]->Queue (packet, hdr);
    }
  else
    {
      m_txop->Queue (packet, hdr);
    }
}

void
StaWifiMac::Receive (Ptr<Packet> packet, const WifiMacHeader *hdr)
{
  NS_LOG_FUNCTION (this << packet << hdr);
  NS_ASSERT (!hdr->IsCtl ());
  if (hdr->GetAddr3 () == GetAddress ())
    {
      NS_LOG_LOGIC ("packet sent by us.");
      return;
    }
  else if (hdr->GetAddr1 () != GetAddress ()
           && !hdr->GetAddr1 ().IsGroup ())
    {
      NS_LOG_LOGIC ("packet is not for us");
      NotifyRxDrop (packet);
      return;
    }
  else if ((hdr->GetAddr1 () == GetAddress ()) && (hdr->GetAddr2 () == GetBssid ()) && hdr->IsCfPoll ())
    {
      SendCfPollResponse ();
    }
  if (hdr->IsData ())
    {
      if (!IsAssociated ())
        {
          NS_LOG_LOGIC ("Received data frame while not associated: ignore");
          NotifyRxDrop (packet);
          return;
        }
      if (!(hdr->IsFromDs () && !hdr->IsToDs ()))
        {
          NS_LOG_LOGIC ("Received data frame not from the DS: ignore");
          NotifyRxDrop (packet);
          return;
        }
      if (hdr->GetAddr2 () != GetBssid ())
        {
          NS_LOG_LOGIC ("Received data frame not from the BSS we are associated with: ignore");
          NotifyRxDrop (packet);
          return;
        }
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
      else if (hdr->HasData ())
        {
          ForwardUp (packet, hdr->GetAddr3 (), hdr->GetAddr1 ());
        }
      return;
    }
  else if (hdr->IsProbeReq ()
           || hdr->IsAssocReq ()
           || hdr->IsReassocReq ())
    {
      //This is a frame aimed at an AP, so we can safely ignore it.
      NotifyRxDrop (packet);
      return;
    }
  else if (hdr->IsBeacon ())
    {
      NS_LOG_DEBUG ("Beacon received");
      MgtBeaconHeader beacon;
      packet->RemoveHeader (beacon);
      CapabilityInformation capabilities = beacon.GetCapabilities ();
      NS_ASSERT (capabilities.IsEss ());
      bool goodBeacon = false;
      if (GetSsid ().IsBroadcast ()
          || beacon.GetSsid ().IsEqual (GetSsid ()))
        {
          NS_LOG_LOGIC ("Beacon is for our SSID");
          goodBeacon = true;
        }
      CfParameterSet cfParameterSet = beacon.GetCfParameterSet ();
      if (cfParameterSet.GetCFPCount () == 0)
        {
          //see section 9.3.2.2 802.11-1999
          if (GetPcfSupported ())
            {
              m_low->DoNavStartNow (MicroSeconds (cfParameterSet.GetCFPMaxDurationUs ()));
            }
          else
            {
              m_low->DoNavStartNow (MicroSeconds (cfParameterSet.GetCFPDurRemainingUs ()));
            }
        }
      SupportedRates rates = beacon.GetSupportedRates ();
      bool bssMembershipSelectorMatch = false;
      for (uint8_t i = 0; i < m_phy->GetNBssMembershipSelectors (); i++)
        {
          uint8_t selector = m_phy->GetBssMembershipSelector (i);
          if (rates.IsBssMembershipSelectorRate (selector))
            {
              NS_LOG_LOGIC ("Beacon is matched to our BSS membership selector");
              bssMembershipSelectorMatch = true;
            }
        }
      if (m_phy->GetNBssMembershipSelectors () > 0 && bssMembershipSelectorMatch == false)
        {
          NS_LOG_LOGIC ("No match for BSS membership selector");
          goodBeacon = false;
        }
      if ((IsWaitAssocResp () || IsAssociated ()) && hdr->GetAddr3 () != GetBssid ())
        {
          NS_LOG_LOGIC ("Beacon is not for us");
          goodBeacon = false;
        }
      if (goodBeacon && m_state == ASSOCIATED)
        {
          m_beaconArrival (Simulator::Now ());
          Time delay = MicroSeconds (beacon.GetBeaconIntervalUs () * m_maxMissedBeacons);
          RestartBeaconWatchdog (delay);
          UpdateApInfoFromBeacon (beacon, hdr->GetAddr2 (), hdr->GetAddr3 ());
        }
      if (goodBeacon && m_state == WAIT_BEACON)
        {
          NS_LOG_DEBUG ("Beacon received while scanning from " << hdr->GetAddr2 ());
          SnrTag snrTag;
          bool removed = packet->RemovePacketTag (snrTag);
          NS_ASSERT (removed);
          ApInfo apInfo;
          apInfo.m_apAddr = hdr->GetAddr2 ();
          apInfo.m_bssid = hdr->GetAddr3 ();
          apInfo.m_activeProbing = false;
          apInfo.m_snr = snrTag.Get ();
          apInfo.m_beacon = beacon;
          UpdateCandidateApList (apInfo);
        }
      return;
    }
  else if (hdr->IsProbeResp ())
    {
      if (m_state == WAIT_PROBE_RESP)
        {
          NS_LOG_DEBUG ("Probe response received while scanning from " << hdr->GetAddr2 ());
          MgtProbeResponseHeader probeResp;
          packet->RemoveHeader (probeResp);
          if (!probeResp.GetSsid ().IsEqual (GetSsid ()))
            {
              NS_LOG_DEBUG ("Probe response is not for our SSID");
              return;
            }
          SnrTag snrTag;
          bool removed = packet->RemovePacketTag (snrTag);
          NS_ASSERT (removed);
          ApInfo apInfo;
          apInfo.m_apAddr = hdr->GetAddr2 ();
          apInfo.m_bssid = hdr->GetAddr3 ();
          apInfo.m_activeProbing = true;
          apInfo.m_snr = snrTag.Get ();
          apInfo.m_probeResp = probeResp;
          UpdateCandidateApList (apInfo);
        }
      return;
    }
  else if (hdr->IsAssocResp () || hdr->IsReassocResp ())
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
              SetState (ASSOCIATED);
              if (hdr->IsReassocResp ())
                {
                  NS_LOG_DEBUG ("reassociation done");
                }
              else
                {
                  NS_LOG_DEBUG ("association completed");
                }
              UpdateApInfoFromAssocResp (assocResp, hdr->GetAddr2 ());
              if (!m_linkUp.IsNull ())
                {
                  m_linkUp ();
                }
            }
          else
            {
              NS_LOG_DEBUG ("association refused");
              if (m_candidateAps.empty ())
                {
                  SetState (REFUSED);
                }
              else
                {
                  ScanningTimeout ();
                }
            }
        }
      return;
    }

  //Invoke the receive handler of our parent class to deal with any
  //other frames. Specifically, this will handle Block Ack-related
  //Management Action frames.
  RegularWifiMac::Receive (packet, hdr);
}

void
StaWifiMac::UpdateCandidateApList (ApInfo newApInfo)
{
  NS_LOG_FUNCTION (this << newApInfo.m_bssid << newApInfo.m_apAddr << newApInfo.m_snr << newApInfo.m_activeProbing << newApInfo.m_beacon << newApInfo.m_probeResp);
  // Remove duplicate ApInfo entry
  for (std::vector<ApInfo>::iterator i = m_candidateAps.begin(); i != m_candidateAps.end(); ++i)
    {
      if (newApInfo.m_bssid == (*i).m_bssid)
        {
          m_candidateAps.erase(i);
          break;
        }
    }
  // Insert before the entry with lower SNR
  for (std::vector<ApInfo>::iterator i = m_candidateAps.begin(); i != m_candidateAps.end(); ++i)
    {
      if (newApInfo.m_snr > (*i).m_snr)
        {
          m_candidateAps.insert (i, newApInfo);
          return;
        }
    }
  // If new ApInfo is the lowest, insert at back
  m_candidateAps.push_back(newApInfo);
}

void
StaWifiMac::UpdateApInfoFromBeacon (MgtBeaconHeader beacon, Mac48Address apAddr, Mac48Address bssid)
{
  NS_LOG_FUNCTION (this << beacon << apAddr << bssid);
  SetBssid (bssid);
  CapabilityInformation capabilities = beacon.GetCapabilities ();
  SupportedRates rates = beacon.GetSupportedRates ();
  for (uint8_t i = 0; i < m_phy->GetNModes (); i++)
    {
      WifiMode mode = m_phy->GetMode (i);
      if (rates.IsSupportedRate (mode.GetDataRate (m_phy->GetChannelWidth ())))
        {
          m_stationManager->AddSupportedMode (apAddr, mode);
        }
    }
  bool isShortPreambleEnabled = capabilities.IsShortPreamble ();
  if (GetErpSupported ())
    {
      ErpInformation erpInformation = beacon.GetErpInformation ();
      isShortPreambleEnabled &= !erpInformation.GetBarkerPreambleMode ();
      if (erpInformation.GetUseProtection () != 0)
        {
          m_stationManager->SetUseNonErpProtection (true);
        }
      else
        {
          m_stationManager->SetUseNonErpProtection (false);
        }
      if (capabilities.IsShortSlotTime () == true)
        {
          //enable short slot time
          SetSlot (MicroSeconds (9));
        }
      else
        {
          //disable short slot time
          SetSlot (MicroSeconds (20));
        }
    }
  if (GetQosSupported ())
    {
      bool qosSupported = false;
      EdcaParameterSet edcaParameters = beacon.GetEdcaParameterSet ();
      if (edcaParameters.IsQosSupported ())
        {
          qosSupported = true;
          //The value of the TXOP Limit field is specified as an unsigned integer, with the least significant octet transmitted first, in units of 32 μs.
          SetEdcaParameters (AC_BE, edcaParameters.GetBeCWmin (), edcaParameters.GetBeCWmax (), edcaParameters.GetBeAifsn (), 32 * MicroSeconds (edcaParameters.GetBeTxopLimit ()));
          SetEdcaParameters (AC_BK, edcaParameters.GetBkCWmin (), edcaParameters.GetBkCWmax (), edcaParameters.GetBkAifsn (), 32 * MicroSeconds (edcaParameters.GetBkTxopLimit ()));
          SetEdcaParameters (AC_VI, edcaParameters.GetViCWmin (), edcaParameters.GetViCWmax (), edcaParameters.GetViAifsn (), 32 * MicroSeconds (edcaParameters.GetViTxopLimit ()));
          SetEdcaParameters (AC_VO, edcaParameters.GetVoCWmin (), edcaParameters.GetVoCWmax (), edcaParameters.GetVoAifsn (), 32 * MicroSeconds (edcaParameters.GetVoTxopLimit ()));
        }
      m_stationManager->SetQosSupport (apAddr, qosSupported);
    }
  if (GetHtSupported ())
    {
      HtCapabilities htCapabilities = beacon.GetHtCapabilities ();
      if (!htCapabilities.IsSupportedMcs (0))
        {
          m_stationManager->RemoveAllSupportedMcs (apAddr);
        }
      else
        {
          m_stationManager->AddStationHtCapabilities (apAddr, htCapabilities);
          HtOperation htOperation = beacon.GetHtOperation ();
          if (htOperation.GetNonGfHtStasPresent ())
            {
              m_stationManager->SetUseGreenfieldProtection (true);
            }
          else
            {
              m_stationManager->SetUseGreenfieldProtection (false);
            }
          if (!GetVhtSupported () && GetRifsSupported () && htOperation.GetRifsMode ())
            {
              m_stationManager->SetRifsPermitted (true);
            }
          else
            {
              m_stationManager->SetRifsPermitted (false);
            }
        }
    }
  if (GetVhtSupported ())
    {
      VhtCapabilities vhtCapabilities = beacon.GetVhtCapabilities ();
      //we will always fill in RxHighestSupportedLgiDataRate field at TX, so this can be used to check whether it supports VHT
      if (vhtCapabilities.GetRxHighestSupportedLgiDataRate () > 0)
        {
          m_stationManager->AddStationVhtCapabilities (apAddr, vhtCapabilities);
          VhtOperation vhtOperation = beacon.GetVhtOperation ();
          for (uint8_t i = 0; i < m_phy->GetNMcs (); i++)
            {
              WifiMode mcs = m_phy->GetMcs (i);
              if (mcs.GetModulationClass () == WIFI_MOD_CLASS_VHT && vhtCapabilities.IsSupportedRxMcs (mcs.GetMcsValue ()))
                {
                  m_stationManager->AddSupportedMcs (apAddr, mcs);
                }
            }
        }
    }
  if (GetHtSupported () || GetVhtSupported ())
    {
      ExtendedCapabilities extendedCapabilities = beacon.GetExtendedCapabilities ();
      //TODO: to be completed
    }
  if (GetHeSupported ())
    {
      HeCapabilities heCapabilities = beacon.GetHeCapabilities ();
      //todo: once we support non constant rate managers, we should add checks here whether HE is supported by the peer
      m_stationManager->AddStationHeCapabilities (apAddr, heCapabilities);
      HeOperation heOperation = beacon.GetHeOperation ();
      for (uint8_t i = 0; i < m_phy->GetNMcs (); i++)
        {
          WifiMode mcs = m_phy->GetMcs (i);
          if (mcs.GetModulationClass () == WIFI_MOD_CLASS_HE && heCapabilities.IsSupportedRxMcs (mcs.GetMcsValue ()))
            {
              m_stationManager->AddSupportedMcs (apAddr, mcs);
            }
        }
    }
  m_stationManager->SetShortPreambleEnabled (isShortPreambleEnabled);
  m_stationManager->SetShortSlotTimeEnabled (capabilities.IsShortSlotTime ());
}

void
StaWifiMac::UpdateApInfoFromProbeResp (MgtProbeResponseHeader probeResp, Mac48Address apAddr, Mac48Address bssid)
{
  NS_LOG_FUNCTION (this << probeResp << apAddr << bssid);
  CapabilityInformation capabilities = probeResp.GetCapabilities ();
  SupportedRates rates = probeResp.GetSupportedRates ();
  for (uint8_t i = 0; i < m_phy->GetNBssMembershipSelectors (); i++)
    {
      uint8_t selector = m_phy->GetBssMembershipSelector (i);
      if (!rates.IsBssMembershipSelectorRate (selector))
        {
          NS_LOG_DEBUG ("Supported rates do not fit with the BSS membership selector");
          return;
        }
    }
  for (uint8_t i = 0; i < m_phy->GetNModes (); i++)
    {
      WifiMode mode = m_phy->GetMode (i);
      if (rates.IsSupportedRate (mode.GetDataRate (m_phy->GetChannelWidth ())))
        {
          m_stationManager->AddSupportedMode (apAddr, mode);
          if (rates.IsBasicRate (mode.GetDataRate (m_phy->GetChannelWidth ())))
            {
              m_stationManager->AddBasicMode (mode);
            }
        }
    }

  bool isShortPreambleEnabled = capabilities.IsShortPreamble ();
  if (GetErpSupported ())
    {
      bool isErpAllowed = false;
      for (uint8_t i = 0; i < m_phy->GetNModes (); i++)
        {
          WifiMode mode = m_phy->GetMode (i);
          if (mode.GetModulationClass () == WIFI_MOD_CLASS_ERP_OFDM && rates.IsSupportedRate (mode.GetDataRate (m_phy->GetChannelWidth ())))
            {
              isErpAllowed = true;
              break;
            }
        }
      if (!isErpAllowed)
        {
          //disable short slot time and set cwMin to 31
          SetSlot (MicroSeconds (20));
          ConfigureContentionWindow (31, 1023);
        }
      else
        {
          ErpInformation erpInformation = probeResp.GetErpInformation ();
          isShortPreambleEnabled &= !erpInformation.GetBarkerPreambleMode ();
          if (m_stationManager->GetShortSlotTimeEnabled ())
            {
              //enable short slot time
              SetSlot (MicroSeconds (9));
            }
          else
            {
              //disable short slot time
              SetSlot (MicroSeconds (20));
            }
          ConfigureContentionWindow (15, 1023);
        }
    }
  m_stationManager->SetShortPreambleEnabled (isShortPreambleEnabled);
  m_stationManager->SetShortSlotTimeEnabled (capabilities.IsShortSlotTime ());
  SetBssid (bssid);
}

void
StaWifiMac::UpdateApInfoFromAssocResp (MgtAssocResponseHeader assocResp, Mac48Address apAddr)
{
  NS_LOG_FUNCTION (this << assocResp << apAddr);
  CapabilityInformation capabilities = assocResp.GetCapabilities ();
  SupportedRates rates = assocResp.GetSupportedRates ();
  bool isShortPreambleEnabled = capabilities.IsShortPreamble ();
  if (GetErpSupported ())
    {
      bool isErpAllowed = false;
      for (uint8_t i = 0; i < m_phy->GetNModes (); i++)
        {
          WifiMode mode = m_phy->GetMode (i);
          if (mode.GetModulationClass () == WIFI_MOD_CLASS_ERP_OFDM && rates.IsSupportedRate (mode.GetDataRate (m_phy->GetChannelWidth ())))
            {
              isErpAllowed = true;
              break;
            }
        }
      if (!isErpAllowed)
        {
          //disable short slot time and set cwMin to 31
          SetSlot (MicroSeconds (20));
          ConfigureContentionWindow (31, 1023);
        }
      else
        {
          ErpInformation erpInformation = assocResp.GetErpInformation ();
          isShortPreambleEnabled &= !erpInformation.GetBarkerPreambleMode ();
          if (m_stationManager->GetShortSlotTimeEnabled ())
            {
              //enable short slot time
              SetSlot (MicroSeconds (9));
            }
          else
            {
              //disable short slot time
              SetSlot (MicroSeconds (20));
            }
          ConfigureContentionWindow (15, 1023);
        }
    }
  m_stationManager->SetShortPreambleEnabled (isShortPreambleEnabled);
  m_stationManager->SetShortSlotTimeEnabled (capabilities.IsShortSlotTime ());
  if (GetQosSupported ())
    {
      bool qosSupported = false;
      EdcaParameterSet edcaParameters = assocResp.GetEdcaParameterSet ();
      if (edcaParameters.IsQosSupported ())
        {
          qosSupported = true;
          //The value of the TXOP Limit field is specified as an unsigned integer, with the least significant octet transmitted first, in units of 32 μs.
          SetEdcaParameters (AC_BE, edcaParameters.GetBeCWmin (), edcaParameters.GetBeCWmax (), edcaParameters.GetBeAifsn (), 32 * MicroSeconds (edcaParameters.GetBeTxopLimit ()));
          SetEdcaParameters (AC_BK, edcaParameters.GetBkCWmin (), edcaParameters.GetBkCWmax (), edcaParameters.GetBkAifsn (), 32 * MicroSeconds (edcaParameters.GetBkTxopLimit ()));
          SetEdcaParameters (AC_VI, edcaParameters.GetViCWmin (), edcaParameters.GetViCWmax (), edcaParameters.GetViAifsn (), 32 * MicroSeconds (edcaParameters.GetViTxopLimit ()));
          SetEdcaParameters (AC_VO, edcaParameters.GetVoCWmin (), edcaParameters.GetVoCWmax (), edcaParameters.GetVoAifsn (), 32 * MicroSeconds (edcaParameters.GetVoTxopLimit ()));
        }
      m_stationManager->SetQosSupport (apAddr, qosSupported);
    }
  if (GetHtSupported ())
    {
      HtCapabilities htCapabilities = assocResp.GetHtCapabilities ();
      if (!htCapabilities.IsSupportedMcs (0))
        {
          m_stationManager->RemoveAllSupportedMcs (apAddr);
        }
      else
        {
          m_stationManager->AddStationHtCapabilities (apAddr, htCapabilities);
          HtOperation htOperation = assocResp.GetHtOperation ();
          if (htOperation.GetNonGfHtStasPresent ())
            {
              m_stationManager->SetUseGreenfieldProtection (true);
            }
          else
            {
              m_stationManager->SetUseGreenfieldProtection (false);
            }
          if (!GetVhtSupported () && GetRifsSupported () && htOperation.GetRifsMode ())
            {
              m_stationManager->SetRifsPermitted (true);
            }
          else
            {
              m_stationManager->SetRifsPermitted (false);
            }
        }
    }
  if (GetVhtSupported ())
    {
      VhtCapabilities vhtCapabilities = assocResp.GetVhtCapabilities ();
      //we will always fill in RxHighestSupportedLgiDataRate field at TX, so this can be used to check whether it supports VHT
      if (vhtCapabilities.GetRxHighestSupportedLgiDataRate () > 0)
        {
          m_stationManager->AddStationVhtCapabilities (apAddr, vhtCapabilities);
          VhtOperation vhtOperation = assocResp.GetVhtOperation ();
        }
    }
  if (GetHeSupported ())
    {
      HeCapabilities hecapabilities = assocResp.GetHeCapabilities ();
      //todo: once we support non constant rate managers, we should add checks here whether HE is supported by the peer
      m_stationManager->AddStationHeCapabilities (apAddr, hecapabilities);
      HeOperation heOperation = assocResp.GetHeOperation ();
    }
  for (uint8_t i = 0; i < m_phy->GetNModes (); i++)
    {
      WifiMode mode = m_phy->GetMode (i);
      if (rates.IsSupportedRate (mode.GetDataRate (m_phy->GetChannelWidth ())))
        {
          m_stationManager->AddSupportedMode (apAddr, mode);
          if (rates.IsBasicRate (mode.GetDataRate (m_phy->GetChannelWidth ())))
            {
              m_stationManager->AddBasicMode (mode);
            }
        }
    }
  if (GetHtSupported ())
    {
      HtCapabilities htCapabilities = assocResp.GetHtCapabilities ();
      for (uint8_t i = 0; i < m_phy->GetNMcs (); i++)
        {
          WifiMode mcs = m_phy->GetMcs (i);
          if (mcs.GetModulationClass () == WIFI_MOD_CLASS_HT && htCapabilities.IsSupportedMcs (mcs.GetMcsValue ()))
            {
              m_stationManager->AddSupportedMcs (apAddr, mcs);
              //here should add a control to add basic MCS when it is implemented
            }
        }
    }
  if (GetVhtSupported ())
    {
      VhtCapabilities vhtcapabilities = assocResp.GetVhtCapabilities ();
      for (uint8_t i = 0; i < m_phy->GetNMcs (); i++)
        {
          WifiMode mcs = m_phy->GetMcs (i);
          if (mcs.GetModulationClass () == WIFI_MOD_CLASS_VHT && vhtcapabilities.IsSupportedRxMcs (mcs.GetMcsValue ()))
            {
              m_stationManager->AddSupportedMcs (apAddr, mcs);
              //here should add a control to add basic MCS when it is implemented
            }
        }
    }
  if (GetHtSupported () || GetVhtSupported ())
    {
      ExtendedCapabilities extendedCapabilities = assocResp.GetExtendedCapabilities ();
      //TODO: to be completed
    }
  if (GetHeSupported ())
    {
      HeCapabilities heCapabilities = assocResp.GetHeCapabilities ();
      for (uint8_t i = 0; i < m_phy->GetNMcs (); i++)
        {
          WifiMode mcs = m_phy->GetMcs (i);
          if (mcs.GetModulationClass () == WIFI_MOD_CLASS_HE && heCapabilities.IsSupportedRxMcs (mcs.GetMcsValue ()))
            {
              m_stationManager->AddSupportedMcs (apAddr, mcs);
              //here should add a control to add basic MCS when it is implemented
            }
        }
    }
}

SupportedRates
StaWifiMac::GetSupportedRates (void) const
{
  SupportedRates rates;
  for (uint8_t i = 0; i < m_phy->GetNModes (); i++)
    {
      WifiMode mode = m_phy->GetMode (i);
      uint64_t modeDataRate = mode.GetDataRate (m_phy->GetChannelWidth ());
      NS_LOG_DEBUG ("Adding supported rate of " << modeDataRate);
      rates.AddSupportedRate (modeDataRate);
    }
  if (GetHtSupported () || GetVhtSupported () || GetHeSupported ())
    {
      for (uint8_t i = 0; i < m_phy->GetNBssMembershipSelectors (); i++)
        {
          rates.AddBssMembershipSelectorRate (m_phy->GetBssMembershipSelector (i));
        }
    }
  return rates;
}

CapabilityInformation
StaWifiMac::GetCapabilities (void) const
{
  CapabilityInformation capabilities;
  capabilities.SetShortPreamble (m_phy->GetShortPlcpPreambleSupported () || GetErpSupported ());
  capabilities.SetShortSlotTime (GetShortSlotTimeSupported () && GetErpSupported ());
  if (GetPcfSupported ())
    {
      capabilities.SetCfPollable ();
    }
  return capabilities;
}

void
StaWifiMac::SetState (MacState value)
{
  if (value == ASSOCIATED
      && m_state != ASSOCIATED)
    {
      m_assocLogger (GetBssid ());
    }
  else if (value != ASSOCIATED
           && m_state == ASSOCIATED)
    {
      m_deAssocLogger (GetBssid ());
    }
  m_state = value;
}

void
StaWifiMac::SetEdcaParameters (AcIndex ac, uint32_t cwMin, uint32_t cwMax, uint8_t aifsn, Time txopLimit)
{
  Ptr<QosTxop> edca = m_edca.find (ac)->second;
  edca->SetMinCw (cwMin);
  edca->SetMaxCw (cwMax);
  edca->SetAifsn (aifsn);
  edca->SetTxopLimit (txopLimit);
}

void
StaWifiMac::PhyCapabilitiesChanged (void)
{
  NS_LOG_FUNCTION (this);
  if (IsAssociated ())
    {
      NS_LOG_DEBUG ("PHY capabilities changed: send reassociation request");
      SetState (WAIT_ASSOC_RESP);
      SendAssociationRequest (true);
    }
}

} //namespace ns3
