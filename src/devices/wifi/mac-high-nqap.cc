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
#include "mac-high-nqap.h"
#include "mac-stations.h"
#include "dca-txop.h"
#include "wifi-net-device.h"
#include "wifi-mac-header.h"
#include "wifi-default-parameters.h"
#include "mgt-headers.h"
#include "wifi-phy.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/node.h"

NS_LOG_COMPONENT_DEFINE ("MacHighNqap");

#define TRACE(x) \
  NS_LOG_DEBUG(Simulator::Now () << " " << m_phy->GetDevice ()->GetNode ()->GetId () << ":" << \
               m_phy->GetDevice ()->GetIfIndex () << " " << x);

namespace ns3 {

MacHighNqap::MacHighNqap ()
  : m_beaconInterval (WifiDefaultParameters::GetApBeaconInterval ())
{}
MacHighNqap::~MacHighNqap ()
{
  m_phy = 0;
}

void 
MacHighNqap::SetDcaTxop (Ptr<DcaTxop> dca)
{
  m_dca = dca;
  m_dca->SetTxOkCallback (MakeCallback (&MacHighNqap::TxOk, this));
  m_dca->SetTxFailedCallback (MakeCallback (&MacHighNqap::TxFailed, this));
}
void 
MacHighNqap::SetBeaconDcaTxop (Ptr<DcaTxop> dca)
{
  // we do not need to be notified when a beacon has been transmitted
  // successfully or not.
  m_beaconDca = dca;
}
void 
MacHighNqap::SetDevice (WifiNetDevice *device)
{
  m_device = device;
}
void 
MacHighNqap::SetStations (MacStations *stations)
{
  m_stations = stations;
}
void
MacHighNqap::SetPhy (Ptr<WifiPhy> phy)
{
  m_phy = phy;
}
void 
MacHighNqap::SetForwardCallback (ForwardCallback callback)
{
  m_forwardUp = callback;
}
void 
MacHighNqap::SetBeaconInterval (Time interval)
{
  m_beaconInterval = interval;
}
void
MacHighNqap::StartBeaconing (void)
{
  SendOneBeacon ();
}
void 
MacHighNqap::ForwardDown (Ptr<const Packet> packet, Mac48Address from, Mac48Address to)
{
  WifiMacHeader hdr;
  hdr.SetTypeData ();
  hdr.SetAddr1 (to);
  hdr.SetAddr2 (m_device->GetSelfAddress ());
  hdr.SetAddr3 (from);
  hdr.SetDsFrom ();
  hdr.SetDsNotTo ();
  m_dca->Queue (packet, hdr);  
}
void 
MacHighNqap::Queue (Ptr<const Packet> packet, Mac48Address to)
{
  ForwardDown (packet, m_device->GetSelfAddress (), to);
}
SupportedRates
MacHighNqap::GetSupportedRates (void) const
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
  for (uint32_t j = 0; j < m_stations->GetNBasicModes (); j++)
    {
      WifiMode mode = m_stations->GetBasicMode (j);
      rates.SetBasicRate (mode.GetDataRate ());
    }
  return rates;
}
void
MacHighNqap::SendProbeResp (Mac48Address to)
{
  TRACE ("send probe response to="<<to);
  WifiMacHeader hdr;
  hdr.SetProbeResp ();
  hdr.SetAddr1 (to);
  hdr.SetAddr2 (m_device->GetSelfAddress ());
  hdr.SetAddr3 (m_device->GetSelfAddress ());
  hdr.SetDsNotFrom ();
  hdr.SetDsNotTo ();
  Ptr<Packet> packet = Create<Packet> ();
  MgtProbeResponseHeader probe;
  probe.SetSsid (m_device->GetSsid ());
  probe.SetSupportedRates (GetSupportedRates ());
  probe.SetBeaconIntervalUs (m_beaconInterval.GetMicroSeconds ());
  packet->AddHeader (probe);

  m_dca->Queue (packet, hdr);
}
void
MacHighNqap::SendAssocResp (Mac48Address to, bool success)
{
  TRACE ("send assoc response to="<<to);
  WifiMacHeader hdr;
  hdr.SetAssocResp ();
  hdr.SetAddr1 (to);
  hdr.SetAddr2 (m_device->GetSelfAddress ());
  hdr.SetAddr3 (m_device->GetSelfAddress ());
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
  
  m_dca->Queue (packet, hdr);
}
void
MacHighNqap::SendOneBeacon (void)
{
  TRACE ("send beacon to="<<Mac48Address::GetBroadcast ());
  WifiMacHeader hdr;
  hdr.SetBeacon ();
  hdr.SetAddr1 (Mac48Address::GetBroadcast ());
  hdr.SetAddr2 (m_device->GetSelfAddress ());
  hdr.SetAddr3 (m_device->GetSelfAddress ());
  hdr.SetDsNotFrom ();
  hdr.SetDsNotTo ();
  Ptr<Packet> packet = Create<Packet> ();
  MgtBeaconHeader beacon;
  beacon.SetSsid (m_device->GetSsid ());
  beacon.SetSupportedRates (GetSupportedRates ());
  beacon.SetBeaconIntervalUs (m_beaconInterval.GetMicroSeconds ());
  packet->AddHeader (beacon);

  m_beaconDca->Queue (packet, hdr);
  Simulator::Schedule (m_beaconInterval, &MacHighNqap::SendOneBeacon, this);
}
void 
MacHighNqap::TxOk (WifiMacHeader const &hdr)
{
  MacStation *station = m_stations->Lookup (hdr.GetAddr1 ());
  if (hdr.IsAssocResp () && 
      station->IsWaitAssocTxOk ()) 
    {
      TRACE ("associated with sta="<<hdr.GetAddr1 ());
      station->RecordGotAssocTxOk ();
    }
}
void 
MacHighNqap::TxFailed (WifiMacHeader const &hdr)
{
  MacStation *station = m_stations->Lookup (hdr.GetAddr1 ());
  if (hdr.IsAssocResp () && 
      station->IsWaitAssocTxOk ()) 
    {
      TRACE ("assoc failed with sta="<<hdr.GetAddr1 ());
      station->RecordGotAssocTxFailed ();
    }
}
void 
MacHighNqap::Receive (Ptr<Packet> packet, WifiMacHeader const *hdr)
{
  MacStation *station = m_stations->Lookup (hdr->GetAddr2 ());

  if (hdr->IsData ()) 
    {
      if (!hdr->IsFromDs () && 
          hdr->IsToDs () &&
          hdr->GetAddr1 () == m_device->GetSelfAddress () &&
          station->IsAssociated ()) 
        {
          if (hdr->GetAddr3 () == m_device->GetSelfAddress ()) 
            {
              TRACE ("frame for me from="<<hdr->GetAddr2 ());
              m_forwardUp (packet, hdr->GetAddr2 ());
            } 
          else 
            {
              TRACE ("forwarding frame from="<<hdr->GetAddr2 ()<<", to="<<hdr->GetAddr3 ());
              Ptr<Packet> copy = packet->Copy ();
              ForwardDown (packet,
                           hdr->GetAddr2 (), 
                           hdr->GetAddr3 ());
              m_forwardUp (copy, hdr->GetAddr2 ());
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
      else if (hdr->GetAddr1 () == m_device->GetSelfAddress ()) 
        {
          if (hdr->IsAssocReq ()) 
            {
              // first, verify that the the station's supported
              // rate set is compatible with our Basic Rate set
              MgtAssocRequestHeader assocReq;
              packet->RemoveHeader (assocReq);
              SupportedRates rates = assocReq.GetSupportedRates ();
              bool problem = false;
              for (uint32_t i = 0; i < m_stations->GetNBasicModes (); i++)
                {
                  WifiMode mode = m_stations->GetBasicMode (i);
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
                  // record all its supported modes in its associated MacStation
                  for (uint32_t j = 0; j < m_phy->GetNModes (); j++)
                    {
                      WifiMode mode = m_phy->GetMode (j);
                      if (rates.IsSupportedRate (mode.GetDataRate ()))
                        {
                          station->AddSupportedMode (mode);
                        }
                    }
                  station->RecordWaitAssocTxOk ();
                  // send assoc response with success status.
                  SendAssocResp (hdr->GetAddr2 (), true);
                }
            } 
          else if (hdr->IsDisassociation ()) 
            {
              station->RecordDisassociated ();
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
  else 
    {
    /* damn, what could this be ? a control frame ?
     * control frames should never reach the MacHigh so,
     * this is likely to be a bug. assert.
     */
      NS_ASSERT (false);
    }  
}

} // namespace ns3
