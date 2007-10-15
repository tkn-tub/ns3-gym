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
#include "mgt-headers.h"
#include "ns3/assert.h"

#define noNQAP_DEBUG 1

#ifdef NQAP_DEBUG
#include <iostream>
#include "ns3/simulator.h"
#  define TRACE(x) \
std::cout << "NQAP now=" << Simulator::NowUs () << "us " << x << std::endl;
#else
#  define TRACE(x)
#endif


namespace ns3 {

MacHighNqap::MacHighNqap ()
  : m_beaconIntervalUs (500000)
{}
MacHighNqap::~MacHighNqap ()
{}

void 
MacHighNqap::SetDcaTxop (DcaTxop *dca)
{
  m_dca = dca;
  m_dca->SetTxOkCallback (MakeCallback (&MacHighNqap::TxOk, this));
  m_dca->SetTxFailedCallback (MakeCallback (&MacHighNqap::TxFailed, this));
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
MacHighNqap::SetForwardCallback (ForwardCallback callback)
{
  m_forwardUp = callback;
}
void 
MacHighNqap::SetSupportedRates (SupportedRates rates)
{
  m_rates = rates;
}
void 
MacHighNqap::SetBeaconIntervalUs (uint64_t us)
{
  m_beaconIntervalUs = us;
}
void 
MacHighNqap::ForwardDown (Packet packet, Mac48Address from, Mac48Address to)
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
MacHighNqap::Queue (Packet packet, Mac48Address to)
{
  ForwardDown (packet, m_device->GetSelfAddress (), to);
}
SupportedRates
MacHighNqap::GetSupportedRates (void)
{
  return m_rates;
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
  Packet packet;
  MgtProbeResponseHeader probe;
  probe.SetSsid (m_device->GetSsid ());
  SupportedRates rates = GetSupportedRates ();
  probe.SetSupportedRates (rates);
  probe.SetBeaconIntervalUs (m_beaconIntervalUs);
  packet.AddHeader (probe);
  
  m_dca->Queue (packet, hdr);
}
void
MacHighNqap::SendAssocResp (Mac48Address to)
{
  TRACE ("send assoc response to="<<to);
  WifiMacHeader hdr;
  hdr.SetAssocResp ();
  hdr.SetAddr1 (to);
  hdr.SetAddr2 (m_device->GetSelfAddress ());
  hdr.SetAddr3 (m_device->GetSelfAddress ());
  hdr.SetDsNotFrom ();
  hdr.SetDsNotTo ();
  Packet packet;
  MgtAssocResponseHeader assoc;
  StatusCode code;
  code.SetSuccess ();
  assoc.SetStatusCode (code);
  packet.AddHeader (assoc);
  
  m_dca->Queue (packet, hdr);
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
MacHighNqap::Receive (Packet packet, WifiMacHeader const *hdr)
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
              m_forwardUp (packet, hdr->GetAddr2 ());
            } 
          else 
            {
              ForwardDown (packet,
                           hdr->GetAddr2 (), 
                           hdr->GetAddr3 ());
              m_forwardUp (packet, hdr->GetAddr2 ());
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
              station->RecordWaitAssocTxOk ();
              SendAssocResp (hdr->GetAddr2 ());
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
