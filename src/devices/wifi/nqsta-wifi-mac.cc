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

#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/uinteger.h"

#include "nqsta-wifi-mac.h"
#include "wifi-mac-header.h"
#include "mgt-headers.h"
#include "wifi-phy.h"
#include "dca-txop.h"
#include "mac-low.h"
#include "dcf-manager.h"
#include "mac-rx-middle.h"

NS_LOG_COMPONENT_DEFINE ("NqstaWifiMac");

/*
 * The state machine for this NQSTA is:
 --------------                                          -----------
 | Associated |   <--------------------      ------->    | Refused |
 --------------                        \    /            -----------
    \                                   \  /
     \    -----------------     -----------------------------
      \-> | Beacon Missed | --> | Wait Association Response |
          -----------------     -----------------------------
	        \                       ^
		 \                      |
		  \    -----------------------
	           \-> | Wait Probe Response |
		       -----------------------
 */

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (NqstaWifiMac);

TypeId 
NqstaWifiMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NqstaWifiMac")
    .SetParent<WifiMac> ()
    .AddConstructor<NqstaWifiMac> ()
    .AddAttribute ("ProbeRequestTimeout", "The interval between two consecutive probe request attempts.",
                   TimeValue (Seconds (0.05)),
                   MakeTimeAccessor (&NqstaWifiMac::m_probeRequestTimeout),
                   MakeTimeChecker ())
    .AddAttribute ("AssocRequestTimeout", "The interval between two consecutive assoc request attempts.",
                   TimeValue (Seconds (0.5)),
                   MakeTimeAccessor (&NqstaWifiMac::m_assocRequestTimeout),
                   MakeTimeChecker ())
    .AddAttribute ("MaxMissedBeacons", 
                   "Number of beacons which much be consecutively missed before "
                   "we attempt to restart association.",
                   UintegerValue (10),
                   MakeUintegerAccessor (&NqstaWifiMac::m_maxMissedBeacons),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ActiveProbing", "If true, we send probe requests. If false, we don't.",
                   BooleanValue (false),
                   MakeBooleanAccessor (&NqstaWifiMac::SetActiveProbing),
                   MakeBooleanChecker ())
    ;
  return tid;
}


NqstaWifiMac::NqstaWifiMac ()
  : m_state (BEACON_MISSED),
    m_probeRequestEvent (),
    m_assocRequestEvent (),
    m_beaconWatchdogEnd (Seconds (0.0))
{
  NS_LOG_FUNCTION (this);
  m_rxMiddle = new MacRxMiddle ();
  m_rxMiddle->SetForwardCallback (MakeCallback (&NqstaWifiMac::Receive, this));

  m_low = CreateObject<MacLow> ();
  m_low->SetRxCallback (MakeCallback (&MacRxMiddle::Receive, m_rxMiddle));

  m_dcfManager = new DcfManager ();
  m_dcfManager->SetupLowListener (m_low);

  m_dca = CreateObject<DcaTxop> ();
  m_dca->SetLow (m_low);
  m_dca->SetManager (m_dcfManager);
}

NqstaWifiMac::~NqstaWifiMac ()
{
  NS_LOG_FUNCTION (this);
}

void
NqstaWifiMac::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  delete m_rxMiddle;
  delete m_dcfManager;
  m_rxMiddle = 0;
  m_low = 0;
  m_dcfManager = 0;
  m_phy = 0;
  m_dca = 0;
  WifiMac::DoDispose ();
}

void 
NqstaWifiMac::SetSlot (Time slotTime)
{
  NS_LOG_FUNCTION (this << slotTime);
  m_dcfManager->SetSlot (slotTime);
  m_low->SetSlotTime (slotTime);
}
void 
NqstaWifiMac::SetSifs (Time sifs)
{
  NS_LOG_FUNCTION (this << sifs);
  m_dcfManager->SetSifs (sifs);
  m_low->SetSifs (sifs);
}
void 
NqstaWifiMac::SetEifsNoDifs (Time eifsNoDifs)
{
  NS_LOG_FUNCTION (this << eifsNoDifs);
  m_dcfManager->SetEifsNoDifs (eifsNoDifs);
  m_eifsNoDifs = eifsNoDifs;
}
void 
NqstaWifiMac::SetAckTimeout (Time ackTimeout)
{
  m_low->SetAckTimeout (ackTimeout);
}
void 
NqstaWifiMac::SetCtsTimeout (Time ctsTimeout)
{
  m_low->SetCtsTimeout (ctsTimeout);
}
void 
NqstaWifiMac::SetPifs (Time pifs)
{
  m_low->SetPifs (pifs);
}
Time 
NqstaWifiMac::GetSlot (void) const
{
  return m_low->GetSlotTime ();
}
Time 
NqstaWifiMac::GetSifs (void) const
{
  return m_low->GetSifs ();
}
Time 
NqstaWifiMac::GetEifsNoDifs (void) const
{
  return m_eifsNoDifs;
}
Time 
NqstaWifiMac::GetAckTimeout (void) const
{
  return m_low->GetAckTimeout ();
}
Time 
NqstaWifiMac::GetCtsTimeout (void) const
{
  return m_low->GetCtsTimeout ();
}
Time 
NqstaWifiMac::GetPifs (void) const
{
  return m_low->GetPifs ();
}

void 
NqstaWifiMac::SetWifiPhy (Ptr<WifiPhy> phy)
{
  m_phy = phy;
  m_dcfManager->SetupPhyListener (phy);
  m_low->SetPhy (phy);
}
void 
NqstaWifiMac::SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> stationManager)
{
  m_stationManager = stationManager;
  m_dca->SetWifiRemoteStationManager (stationManager);
  m_low->SetWifiRemoteStationManager (stationManager);
}
void 
NqstaWifiMac::SetForwardUpCallback (Callback<void,Ptr<Packet>, const Mac48Address &> upCallback)
{
  m_forwardUp = upCallback;
}
void 
NqstaWifiMac::SetLinkUpCallback (Callback<void> linkUp)
{
  m_linkUp = linkUp;
}
void 
NqstaWifiMac::SetLinkDownCallback (Callback<void> linkDown)
{
  m_linkDown = linkDown;
}
Mac48Address 
NqstaWifiMac::GetAddress (void) const
{
  return m_address;
}
Ssid 
NqstaWifiMac::GetSsid (void) const
{
  return m_ssid;
}
Mac48Address 
NqstaWifiMac::GetBssid (void) const
{
  return m_low->GetBssid ();
}
void 
NqstaWifiMac::SetAddress (Mac48Address address)
{
  NS_LOG_FUNCTION (this << address);
  m_address = address;
}
void 
NqstaWifiMac::SetSsid (Ssid ssid)
{
  NS_LOG_FUNCTION (this << ssid);
  m_ssid = ssid;
}

void 
NqstaWifiMac::SetMaxMissedBeacons (uint32_t missed)
{
  NS_LOG_FUNCTION (this << missed);
  m_maxMissedBeacons = missed;
}
void 
NqstaWifiMac::SetProbeRequestTimeout (Time timeout)
{
  NS_LOG_FUNCTION (this << timeout);
  m_probeRequestTimeout = timeout;
}
void 
NqstaWifiMac::SetAssocRequestTimeout (Time timeout)
{
  NS_LOG_FUNCTION (this << timeout);
  m_assocRequestTimeout = timeout;
}

void 
NqstaWifiMac::StartActiveAssociation (void)
{
  NS_LOG_FUNCTION (this);
  TryToEnsureAssociated ();
}

Mac48Address
NqstaWifiMac::GetBroadcastBssid (void)
{
  return Mac48Address::GetBroadcast ();
}

void 
NqstaWifiMac::SetBssid (Mac48Address bssid)
{
  NS_LOG_FUNCTION (this << bssid);
  m_low->SetBssid (bssid);
}
void 
NqstaWifiMac::SetActiveProbing (bool enable)
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
NqstaWifiMac::ForwardUp (Ptr<Packet> packet, const Mac48Address &address)
{
  NS_LOG_FUNCTION (this << packet << address);
  m_forwardUp (packet, address);
}
void
NqstaWifiMac::SendProbeRequest (void)
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
  
  m_dca->Queue (packet, hdr);

  m_probeRequestEvent = Simulator::Schedule (m_probeRequestTimeout,
                                             &NqstaWifiMac::ProbeRequestTimeout, this);
}

void
NqstaWifiMac::SendAssociationRequest (void)
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
  
  m_dca->Queue (packet, hdr);

  m_assocRequestEvent = Simulator::Schedule (m_assocRequestTimeout,
                                             &NqstaWifiMac::AssocRequestTimeout, this);
}
void
NqstaWifiMac::TryToEnsureAssociated (void)
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
NqstaWifiMac::AssocRequestTimeout (void)
{
  NS_LOG_FUNCTION (this);
  m_state = WAIT_ASSOC_RESP;
  SendAssociationRequest ();
}
void
NqstaWifiMac::ProbeRequestTimeout (void)
{
  NS_LOG_FUNCTION (this);
  m_state = WAIT_PROBE_RESP;
  SendProbeRequest ();
}
void 
NqstaWifiMac::MissedBeacons (void)
{
  NS_LOG_FUNCTION (this);
  if (m_beaconWatchdogEnd > Simulator::Now ())
    {
      m_beaconWatchdog = Simulator::Schedule (m_beaconWatchdogEnd - Simulator::Now (),
                                              &NqstaWifiMac::MissedBeacons, this);
      return;
    }
  NS_LOG_DEBUG ("beacon missed");
  m_state = BEACON_MISSED;
  TryToEnsureAssociated ();
}
void 
NqstaWifiMac::RestartBeaconWatchdog (Time delay)
{
  NS_LOG_FUNCTION (this << delay);
  m_beaconWatchdogEnd = std::max (Simulator::Now () + delay, m_beaconWatchdogEnd);
  if (Simulator::GetDelayLeft (m_beaconWatchdog) < delay &&
      m_beaconWatchdog.IsExpired ())
    {
      NS_LOG_DEBUG ("really restart watchdog.");
      m_beaconWatchdog = Simulator::Schedule (delay, &NqstaWifiMac::MissedBeacons, this);
    }
}
bool
NqstaWifiMac::IsAssociated (void)
{
  return (m_state == ASSOCIATED)?true:false;
}

void 
NqstaWifiMac::Enqueue (Ptr<const Packet> packet, Mac48Address to, Mac48Address from)
{
  NS_LOG_FUNCTION (this << packet << to);
  if (!IsAssociated ()) 
    {
      TryToEnsureAssociated ();
      return;
    }
  //NS_LOG_DEBUG ("enqueue size="<<packet->GetSize ()<<", to="<<to);
  WifiMacHeader hdr;
  hdr.SetTypeData ();
  hdr.SetAddr1 (GetBssid ());
  hdr.SetAddr2 (from);
  hdr.SetAddr3 (to);
  hdr.SetDsNotFrom ();
  hdr.SetDsTo ();
  m_dca->Queue (packet, hdr);
}

void 
NqstaWifiMac::Receive (Ptr<Packet> packet, WifiMacHeader const *hdr)
{
  NS_LOG_FUNCTION (this << packet << hdr);
  NS_ASSERT (!hdr->IsCtl ());
  if (hdr->GetAddr1 () != GetAddress () &&
      !hdr->GetAddr1 ().IsBroadcast ()) 
    {
      // packet is not for us
    } 
  else if (hdr->IsData ()) 
    {
      ForwardUp (packet, hdr->GetAddr2 ());
    } 
  else if (hdr->IsProbeReq () ||
           hdr->IsAssocReq ()) 
    {
      /* this is a frame aimed at an AP.
       * so we can safely ignore it.
       */
    } 
  else if (hdr->IsBeacon ()) 
    {
      MgtBeaconHeader beacon;
      packet->RemoveHeader (beacon);
      bool goodBeacon = false;
      if (GetSsid ().IsBroadcast () ||
          beacon.GetSsid ().IsEqual (GetSsid ()))
        {
          Time delay = MicroSeconds (beacon.GetBeaconIntervalUs () * m_maxMissedBeacons);
          RestartBeaconWatchdog (delay);
          goodBeacon = true;
        }
      if (goodBeacon) 
        {
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
              WifiRemoteStation *ap = m_stationManager->Lookup (hdr->GetAddr2 ());
              for (uint32_t i = 0; i < m_phy->GetNModes (); i++)
                {
                  WifiMode mode = m_phy->GetMode (i);
                  if (rates.IsSupportedRate (mode.GetDataRate ()))
                    {
                      ap->AddSupportedMode (mode);
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
}

SupportedRates
NqstaWifiMac::GetSupportedRates (void) const
{
  SupportedRates rates;
  for (uint32_t i = 0; i < m_phy->GetNModes (); i++)
    {
      WifiMode mode = m_phy->GetMode (i);
      rates.AddSupportedRate (mode.GetDataRate ());
    }
  return rates;
}

} // namespace ns3
