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

#include "mac-high-nqsta.h"
#include "wifi-mac-header.h"
#include "wifi-net-device.h"
#include "mgt-headers.h"
#include "wifi-phy.h"
#include "dca-txop.h"
#include "mac-stations.h"

NS_LOG_COMPONENT_DEFINE ("MacHighNqsta");

#define TRACE(x) \
NS_LOG_DEBUG (Simulator::Now () << " " << x);

/*
 * The state machine for this NQSTA is:
 --------------            -----------
 | Associated |   <--------------------      ------->    | Refused |
 --------------      \    /      -----------
    \           \  /
     \    -----------------     -----------------------------
      \-> | Beacon Missed | --> | Wait Association Response |
    -----------------     -----------------------------
    \           ^
     \          |
      \    -----------------------
       \-> | Wait Probe Response |
           -----------------------
 */


namespace ns3 {

MacHighNqsta::MacHighNqsta ()
  : m_state (BEACON_MISSED),
    m_probeRequestTimeout (Seconds (0.5)),
    m_assocRequestTimeout (Seconds (0.5)),
    m_probeRequestEvent (),
    m_assocRequestEvent (),
    m_beaconWatchdogEnd (Seconds (0.0))
{
  // this is the default value for the number of beacons missed 
  // before attempting to reassociate.
  m_maxMissedBeacons = 10;
}

MacHighNqsta::~MacHighNqsta ()
{
  m_phy = 0;
}

void 
MacHighNqsta::SetDcaTxop (DcaTxop *dca)
{
  m_dca = dca;
}
void 
MacHighNqsta::SetDevice (WifiNetDevice *device)
{
  m_device = device;
}
void 
MacHighNqsta::SetForwardCallback (ForwardCallback callback)
{
  m_forward = callback;
}
void 
MacHighNqsta::SetAssociatedCallback (AssociatedCallback callback)
{
  m_associatedCallback = callback;
}
void 
MacHighNqsta::SetDisAssociatedCallback (DisAssociatedCallback callback)
{
  m_disAssociatedCallback = callback;
}
void 
MacHighNqsta::SetPhy (Ptr<WifiPhy> phy)
{
  m_phy = phy;
}
void 
MacHighNqsta::SetStations (MacStations *stations)
{
  m_stations = stations;
}

void 
MacHighNqsta::SetMaxMissedBeacons (uint32_t missed)
{
  m_maxMissedBeacons = missed;
}
void 
MacHighNqsta::SetProbeRequestTimeout (Time timeout)
{
  m_probeRequestTimeout = timeout;
}
void 
MacHighNqsta::SetAssocRequestTimeout (Time timeout)
{
  m_assocRequestTimeout = timeout;
}

Mac48Address 
MacHighNqsta::GetBssid (void) const
{
  return m_bssid;
}
void 
MacHighNqsta::SetBssid (Mac48Address bssid)
{
  m_bssid = bssid;

}
void 
MacHighNqsta::StartActiveAssociation (void)
{
  TryToEnsureAssociated ();
}

Mac48Address
MacHighNqsta::GetBroadcastBssid (void)
{
  return Mac48Address::GetBroadcast ();
}

void
MacHighNqsta::SendProbeRequest (void)
{
  TRACE ("send probe request");
  WifiMacHeader hdr;
  hdr.SetProbeReq ();
  hdr.SetAddr1 (GetBroadcastBssid ());
  hdr.SetAddr2 (m_device->GetSelfAddress ());
  hdr.SetAddr3 (GetBroadcastBssid ());
  hdr.SetDsNotFrom ();
  hdr.SetDsNotTo ();
  Packet packet;
  MgtProbeRequestHeader probe;
  probe.SetSsid (m_device->GetSsid ());
  probe.SetSupportedRates (GetSupportedRates ());
  packet.AddHeader (probe);
  
  m_dca->Queue (packet, hdr);

  m_probeRequestEvent = Simulator::Schedule (m_probeRequestTimeout,
                                             &MacHighNqsta::ProbeRequestTimeout, this);
}

void
MacHighNqsta::SendAssociationRequest (void)
{
  TRACE ("send assoc request to=" << GetBssid ());
  WifiMacHeader hdr;
  hdr.SetAssocReq ();
  hdr.SetAddr1 (GetBssid ());
  hdr.SetAddr2 (m_device->GetSelfAddress ());
  hdr.SetAddr3 (GetBssid ());
  hdr.SetDsNotFrom ();
  hdr.SetDsNotTo ();
  Packet packet;
  MgtAssocRequestHeader assoc;
  assoc.SetSsid (m_device->GetSsid ());
  assoc.SetSupportedRates (GetSupportedRates ());
  packet.AddHeader (assoc);
  
  m_dca->Queue (packet, hdr);

  m_assocRequestEvent = Simulator::Schedule (m_assocRequestTimeout,
                                             &MacHighNqsta::AssocRequestTimeout, this);
}
void
MacHighNqsta::TryToEnsureAssociated (void)
{
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
    m_disAssociatedCallback ();
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
MacHighNqsta::AssocRequestTimeout (void)
{
  TRACE ("assoc request timeout");
  m_state = WAIT_ASSOC_RESP;
  SendAssociationRequest ();
}
void
MacHighNqsta::ProbeRequestTimeout (void)
{
  TRACE ("probe request timeout");
  m_state = WAIT_PROBE_RESP;
  SendProbeRequest ();
}
void 
MacHighNqsta::MissedBeacons (void)
{
  if (m_beaconWatchdogEnd > Simulator::Now ())
    {
      m_beaconWatchdog = Simulator::Schedule (Simulator::Now () - m_beaconWatchdogEnd, 
                                              &MacHighNqsta::MissedBeacons, this);
      return;
    }
  m_state = BEACON_MISSED;
}
void 
MacHighNqsta::RestartBeaconWatchdog (Time delay)
{
  m_beaconWatchdogEnd = std::max (Simulator::Now () + delay, m_beaconWatchdogEnd);
  if (Simulator::GetDelayLeft (m_beaconWatchdog) < delay &&
      m_beaconWatchdog.IsExpired ())
    {
      m_beaconWatchdog = Simulator::Schedule (delay, &MacHighNqsta::MissedBeacons, this);
    }
}
bool
MacHighNqsta::IsAssociated (void)
{
  return (m_state == ASSOCIATED)?true:false;
}

void 
MacHighNqsta::Queue (Packet packet, Mac48Address to)
{
  if (!IsAssociated ()) 
    {
      TryToEnsureAssociated ();
      return;
    }
  //TRACE ("enqueue size="<<packet.GetSize ()<<", to="<<to);
  WifiMacHeader hdr;
  hdr.SetTypeData ();
  hdr.SetAddr1 (GetBssid ());
  hdr.SetAddr2 (m_device->GetSelfAddress ());
  hdr.SetAddr3 (to);
  hdr.SetDsNotFrom ();
  hdr.SetDsTo ();
  m_dca->Queue (packet, hdr);
}

void 
MacHighNqsta::Receive (Packet packet, WifiMacHeader const *hdr)
{
  NS_ASSERT (!hdr->IsCtl ());
  if (hdr->GetAddr1 () != m_device->GetSelfAddress () &&
      !hdr->GetAddr1 ().IsBroadcast ()) 
    {
      // packet is not for us
    } 
  else if (hdr->IsData ()) 
    {
      m_forward (packet, hdr->GetAddr2 ());
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
      packet.RemoveHeader (beacon);
      bool goodBeacon = false;
      if (m_device->GetSsid ().IsBroadcast () ||
          beacon.GetSsid ().IsEqual (m_device->GetSsid ()))
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
          packet.RemoveHeader (probeResp);
          if (!probeResp.GetSsid ().IsEqual (m_device->GetSsid ()))
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
          packet.RemoveHeader (assocResp);
          if (m_assocRequestEvent.IsRunning ()) 
            {
              m_assocRequestEvent.Cancel ();
            }
          if (assocResp.GetStatusCode ().IsSuccess ()) 
            {
              m_state = ASSOCIATED;
              TRACE ("assoc completed"); 
              SupportedRates rates = assocResp.GetSupportedRates ();
              MacStation *ap = m_stations->Lookup (hdr->GetAddr2 ());
              for (uint32_t i = 0; i < m_phy->GetNModes (); i++)
                {
                  WifiMode mode = m_phy->GetMode (i);
                  if (rates.IsSupportedRate (mode.GetPhyRate ()))
                    {
                      ap->AddSupportedMode (mode);
                      if (rates.IsBasicRate (mode.GetPhyRate ()))
                        {
                          m_stations->AddBasicMode (mode);
                        }
                    }
                }
              m_associatedCallback ();
            } 
          else 
            {
              TRACE ("assoc refused");
              m_state = REFUSED;
            }
        }
    }
}

SupportedRates
MacHighNqsta::GetSupportedRates (void) const
{
  SupportedRates rates;
  for (uint32_t i = 0; i < m_phy->GetNModes (); i++)
    {
      WifiMode mode = m_phy->GetMode (i);
      rates.AddSupportedRate (mode.GetPhyRate ());
    }
  return rates;
}

} // namespace ns3
