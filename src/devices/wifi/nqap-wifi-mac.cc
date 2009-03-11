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
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/node.h"

#include "nqap-wifi-mac.h"
#include "dca-txop.h"
#include "wifi-mac-header.h"
#include "mgt-headers.h"
#include "wifi-phy.h"
#include "dcf-manager.h"
#include "mac-rx-middle.h"
#include "mac-low.h"
#include "wifi-mac-trailer.h"
#include "ns3/pointer.h"

NS_LOG_COMPONENT_DEFINE ("NqapWifiMac");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (NqapWifiMac);

#undef NS_LOG_APPEND_CONTEXT
#define NS_LOG_APPEND_CONTEXT if (m_low != 0) {std::clog << "[mac=" << m_low->GetAddress () << "] ";}

TypeId 
NqapWifiMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NqapWifiMac")
    .SetParent<WifiMac> ()
    .AddConstructor<NqapWifiMac> ()
    .AddAttribute ("BeaconInterval", "Delay between two beacons",
                   TimeValue (Seconds (0.1)),
                   MakeTimeAccessor (&NqapWifiMac::GetBeaconInterval,
                                     &NqapWifiMac::SetBeaconInterval),
                   MakeTimeChecker ())
    .AddAttribute ("BeaconGeneration", "Whether or not beacons are generated.",
                   BooleanValue (true),
                   MakeBooleanAccessor (&NqapWifiMac::SetBeaconGeneration,
                                        &NqapWifiMac::GetBeaconGeneration),
                   MakeBooleanChecker ())
    .AddAttribute ("DcaTxop", "The DcaTxop object",
                   PointerValue (),
                   MakePointerAccessor (&NqapWifiMac::DoGetDcaTxop),
                   MakePointerChecker<DcaTxop> ()) 
    ;
  return tid;
}

NqapWifiMac::NqapWifiMac ()
{
  NS_LOG_FUNCTION (this);
  m_rxMiddle = new MacRxMiddle ();
  m_rxMiddle->SetForwardCallback (MakeCallback (&NqapWifiMac::Receive, this));

  m_low = CreateObject<MacLow> ();
  m_low->SetRxCallback (MakeCallback (&MacRxMiddle::Receive, m_rxMiddle));

  m_dcfManager = new DcfManager ();
  m_dcfManager->SetupLowListener (m_low);

  m_dca = CreateObject<DcaTxop> ();
  m_dca->SetLow (m_low);
  m_dca->SetManager (m_dcfManager);
  m_dca->SetTxOkCallback (MakeCallback (&NqapWifiMac::TxOk, this));
  m_dca->SetTxFailedCallback (MakeCallback (&NqapWifiMac::TxFailed, this));

  m_beaconDca = CreateObject<DcaTxop> ();
  m_beaconDca->SetAifsn(1);
  m_beaconDca->SetMinCw(0);
  m_beaconDca->SetMaxCw(0);
  m_beaconDca->SetLow (m_low);
  m_beaconDca->SetManager (m_dcfManager);
}
NqapWifiMac::~NqapWifiMac ()
{
  NS_LOG_FUNCTION (this);
}

void
NqapWifiMac::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  delete m_rxMiddle;
  delete m_dcfManager;
  m_rxMiddle = 0;
  m_low = 0;
  m_dcfManager = 0;
  m_phy = 0;
  m_dca = 0;
  m_beaconDca = 0;
  m_beaconEvent.Cancel ();
  WifiMac::DoDispose ();
}

void
NqapWifiMac::SetBeaconGeneration (bool enable)
{
  NS_LOG_FUNCTION (this << enable);
  if (enable)
    {
      m_beaconEvent = Simulator::ScheduleNow (&NqapWifiMac::SendOneBeacon, this);
    }
  else
    {
      m_beaconEvent.Cancel ();
    }
}

bool
NqapWifiMac::GetBeaconGeneration (void) const
{
  return m_beaconEvent.IsRunning ();
}
Time 
NqapWifiMac::GetBeaconInterval (void) const
{
  return m_beaconInterval;
}

void 
NqapWifiMac::SetSlot (Time slotTime)
{
  NS_LOG_FUNCTION (this << slotTime);
  m_dcfManager->SetSlot (slotTime);
  m_low->SetSlotTime (slotTime);
}
void 
NqapWifiMac::SetSifs (Time sifs)
{
  NS_LOG_FUNCTION (this << sifs);
  m_dcfManager->SetSifs (sifs);
  m_low->SetSifs (sifs);
}
void 
NqapWifiMac::SetEifsNoDifs (Time eifsNoDifs)
{
  NS_LOG_FUNCTION (this << eifsNoDifs);
  m_dcfManager->SetEifsNoDifs (eifsNoDifs);
  m_eifsNoDifs = eifsNoDifs;
}
void 
NqapWifiMac::SetAckTimeout (Time ackTimeout)
{
  m_low->SetAckTimeout (ackTimeout);
}
void 
NqapWifiMac::SetCtsTimeout (Time ctsTimeout)
{
  m_low->SetCtsTimeout (ctsTimeout);
}
void 
NqapWifiMac::SetPifs (Time pifs)
{
  m_low->SetPifs (pifs);
}
Time 
NqapWifiMac::GetSlot (void) const
{
  return m_low->GetSlotTime ();
}
Time 
NqapWifiMac::GetSifs (void) const
{
  return m_low->GetSifs ();
}
Time 
NqapWifiMac::GetEifsNoDifs (void) const
{
  return m_eifsNoDifs;
}
Time 
NqapWifiMac::GetAckTimeout (void) const
{
  return m_low->GetAckTimeout ();
}
Time 
NqapWifiMac::GetCtsTimeout (void) const
{
  return m_low->GetCtsTimeout ();
}
Time 
NqapWifiMac::GetPifs (void) const
{
  return m_low->GetPifs ();
}
void 
NqapWifiMac::SetWifiPhy (Ptr<WifiPhy> phy)
{
  NS_LOG_FUNCTION (this << phy);
  m_phy = phy;
  m_dcfManager->SetupPhyListener (phy);
  m_low->SetPhy (phy);
}
void 
NqapWifiMac::SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> stationManager)
{
  NS_LOG_FUNCTION (this << stationManager);
  m_stationManager = stationManager;
  m_dca->SetWifiRemoteStationManager (stationManager);
  m_beaconDca->SetWifiRemoteStationManager (stationManager);
  m_low->SetWifiRemoteStationManager (stationManager);
}
void 
NqapWifiMac::SetForwardUpCallback (Callback<void,Ptr<Packet>, Mac48Address, Mac48Address> upCallback)
{
  NS_LOG_FUNCTION (this);
  m_upCallback = upCallback;
}
void 
NqapWifiMac::SetLinkUpCallback (Callback<void> linkUp)
{
  NS_LOG_FUNCTION (this);
  if (!linkUp.IsNull ())
    {
      linkUp ();
    }
}
void 
NqapWifiMac::SetLinkDownCallback (Callback<void> linkDown)
{
  NS_LOG_FUNCTION (this);
}
Mac48Address 
NqapWifiMac::GetAddress (void) const
{
  return m_low->GetAddress ();
}
Ssid 
NqapWifiMac::GetSsid (void) const
{
  return m_ssid;
}
void 
NqapWifiMac::SetAddress (Mac48Address address)
{
  NS_LOG_FUNCTION (address);
  m_low->SetAddress (address);
  m_low->SetBssid (address);
}
void 
NqapWifiMac::SetSsid (Ssid ssid)
{
  NS_LOG_FUNCTION (ssid);
  m_ssid = ssid;
}
Mac48Address 
NqapWifiMac::GetBssid (void) const
{
  return m_low->GetBssid ();
}


void 
NqapWifiMac::SetBeaconInterval (Time interval)
{
  NS_LOG_FUNCTION (this << interval);
  m_beaconInterval = interval;
}
void
NqapWifiMac::StartBeaconing (void)
{
  NS_LOG_FUNCTION (this);
  SendOneBeacon ();
}
void 
NqapWifiMac::ForwardUp (Ptr<Packet> packet, Mac48Address from, Mac48Address to)
{
  NS_LOG_FUNCTION (this << packet << from);
  NotifyRx (packet);
  m_upCallback (packet, from, to);
}

void 
NqapWifiMac::ForwardDown (Ptr<const Packet> packet, Mac48Address from, Mac48Address to)
{
  NS_LOG_FUNCTION (this << packet << from << to);
  WifiMacHeader hdr;
  hdr.SetTypeData ();
  hdr.SetAddr1 (to);
  hdr.SetAddr2 (GetAddress ());
  hdr.SetAddr3 (from);
  hdr.SetDsFrom ();
  hdr.SetDsNotTo ();

  NotifyTx (packet);
  m_dca->Queue (packet, hdr);  
}
void 
NqapWifiMac::Enqueue (Ptr<const Packet> packet, Mac48Address to, Mac48Address from)
{
  NS_LOG_FUNCTION (this << packet << to << from);
  ForwardDown (packet, from, to);
}
void 
NqapWifiMac::Enqueue (Ptr<const Packet> packet, Mac48Address to)
{
  NS_LOG_FUNCTION (this << packet << to);
  ForwardDown (packet, m_low->GetAddress (), to);
}
bool 
NqapWifiMac::SupportsSendFrom (void) const
{
  return true;
}
SupportedRates
NqapWifiMac::GetSupportedRates (void) const
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
NqapWifiMac::SendProbeResp (Mac48Address to)
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

  m_dca->Queue (packet, hdr);
}
void
NqapWifiMac::SendAssocResp (Mac48Address to, bool success)
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
  
  m_dca->Queue (packet, hdr);
}
void
NqapWifiMac::SendOneBeacon (void)
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
  m_beaconEvent = Simulator::Schedule (m_beaconInterval, &NqapWifiMac::SendOneBeacon, this);
}
void 
NqapWifiMac::TxOk (WifiMacHeader const &hdr)
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
NqapWifiMac::TxFailed (WifiMacHeader const &hdr)
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
NqapWifiMac::Receive (Ptr<Packet> packet, WifiMacHeader const *hdr)
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
              NS_LOG_DEBUG ("frame for me from="<<from);
              ForwardUp (packet, from, bssid);
            } 
          else if (to.IsGroup () ||
                   toStation->IsAssociated ())
            {
              NS_LOG_DEBUG ("forwarding frame from="<<from<<", to="<<to);
              Ptr<Packet> copy = packet->Copy ();
              ForwardDown (packet,
                           from, 
                           to);
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
          NotifyRxDrop (packet);
        } 
      else 
        {
          // we can ignore these frames since 
          // they are not targeted at the AP
          NotifyRxDrop (packet);
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
  else 
    {
    /* damn, what could this be ? a control frame ?
     * control frames should never reach the MacHigh so,
     * this is likely to be a bug. assert.
     */
      NS_ASSERT (false);
    }  
}
Ptr<DcaTxop>
NqapWifiMac::DoGetDcaTxop(void) const
{
  return m_dca;
}

} // namespace ns3
