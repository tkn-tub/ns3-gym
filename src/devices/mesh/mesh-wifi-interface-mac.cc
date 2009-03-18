/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/* 
 * Copyright (c) 2009 IITP RAS
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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 *          Pavel Boyko <boyko@iitp.ru>
 */

#include "ns3/mesh-wifi-interface-mac.h"
#include "ns3/mesh-wifi-beacon.h"
#include "ns3/log.h"
#include "ns3/wifi-phy.h"
#include "ns3/dcf-manager.h"
#include "ns3/mac-rx-middle.h"
#include "ns3/mac-low.h"
#include "ns3/dca-txop.h"
#include "ns3/mesh-wifi-mac-header.h"
#include "ns3/random-variable.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE ("MeshWifiInterfaceMac");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (MeshWifiInterfaceMac);

TypeId
MeshWifiInterfaceMac::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::MeshWifiInterfaceMac")
                      .SetParent<WifiMac> ()
                      .AddConstructor<MeshWifiInterfaceMac> ()
                      .AddAttribute ("BeaconInterval", "Beacon Interval",
                                     TimeValue (Seconds (1.0)),
                                     MakeTimeAccessor (&MeshWifiInterfaceMac::m_beaconInterval),
                                     MakeTimeChecker ()
                                    )
                      .AddAttribute ("RandomStart", "Window when beacon generating starts (uniform random) in seconds",
                                     TimeValue (Seconds (0.1)),
                                     MakeTimeAccessor (&MeshWifiInterfaceMac::m_randomStart),
                                     MakeTimeChecker ()
                                    )
                      .AddAttribute ("SoftwareDelay", "Window of uniformely distributed random software handling delay",
                                     TimeValue (MicroSeconds (500)),
                                     MakeTimeAccessor (&MeshWifiInterfaceMac::m_softwareDelay),
                                     MakeTimeChecker ()
                                    )
                      .AddAttribute ("BeaconGeneration", "Enable/Disable Beaconing.",
                                     BooleanValue (true),
                                     MakeBooleanAccessor (
                                       &MeshWifiInterfaceMac::SetBeaconGeneration,
                                       &MeshWifiInterfaceMac::GetBeaconGeneration
                                     ),
                                     MakeBooleanChecker ()
                                    );
  return tid;
}

MeshWifiInterfaceMac::MeshWifiInterfaceMac ()
{
  NS_LOG_FUNCTION (this);
  
  m_rxMiddle = new MacRxMiddle ();
  m_rxMiddle->SetForwardCallback (MakeCallback (&MeshWifiInterfaceMac::Receive, this));

  m_low = CreateObject<MacLow> ();
  m_low->SetRxCallback (MakeCallback (&MacRxMiddle::Receive, m_rxMiddle));

  m_dcfManager = new DcfManager ();
  m_dcfManager->SetupLowListener (m_low);

  m_beaconDca = CreateObject<DcaTxop> ();
  m_beaconDca->SetLow (m_low);
  m_beaconDca->SetMinCw (0);
  m_beaconDca->SetMaxCw (0);
  m_beaconDca->SetAifsn(1);
  m_beaconDca->SetManager (m_dcfManager);

  m_VO = CreateObject<DcaTxop> ();
  m_VO->SetLow (m_low);
  m_VO->SetMinCw (3);
  m_VO->SetMaxCw (7);
  m_VO->SetManager (m_dcfManager);

  m_BE = CreateObject<DcaTxop> ();
  m_BE->SetLow (m_low);
  m_BE->SetManager (m_dcfManager);
}

MeshWifiInterfaceMac::~MeshWifiInterfaceMac ()
{
  NS_LOG_FUNCTION (this);
}

//-----------------------------------------------------------------------------
// WifiMac inherited
//-----------------------------------------------------------------------------
void
MeshWifiInterfaceMac::SetSlot (Time slotTime)
{
  NS_LOG_FUNCTION (this << slotTime);
  m_dcfManager->SetSlot (slotTime);
  m_slot = slotTime;
}

void
MeshWifiInterfaceMac::SetSifs (Time sifs)
{
  NS_LOG_FUNCTION (this << sifs);
  m_dcfManager->SetSifs (sifs);
  m_sifs = sifs;
}
void
MeshWifiInterfaceMac::SetAckTimeout (Time ackTimeout)
{
  m_low->SetAckTimeout (ackTimeout);
}

void
MeshWifiInterfaceMac::SetCtsTimeout (Time ctsTimeout)
{
  m_low->SetCtsTimeout (ctsTimeout);
}

void
MeshWifiInterfaceMac::SetPifs (Time pifs)
{
  NS_LOG_FUNCTION (this << pifs);
  m_pifs = pifs;
}
void
MeshWifiInterfaceMac::SetEifsNoDifs (Time eifsNoDifs)
{
  NS_LOG_FUNCTION (this << eifsNoDifs);
  m_dcfManager->SetEifsNoDifs (eifsNoDifs);
  m_eifsNoDifs = eifsNoDifs;
}

Time
MeshWifiInterfaceMac::GetSlot () const
  {
    return m_slot;
  }

Time
MeshWifiInterfaceMac::GetSifs () const
  {
    return m_sifs;
  }

Time
MeshWifiInterfaceMac::GetEifsNoDifs () const
  {
    return m_eifsNoDifs;
  }

Time
MeshWifiInterfaceMac::GetAckTimeout () const
  {
    return m_low->GetAckTimeout ();
  }

Time
MeshWifiInterfaceMac::GetCtsTimeout () const
  {
    return m_low->GetCtsTimeout ();
  }

Time
MeshWifiInterfaceMac::GetPifs () const
  {
    return m_low->GetPifs ();
  }

void
MeshWifiInterfaceMac::SetWifiPhy (Ptr<WifiPhy> phy)
{
  NS_LOG_FUNCTION (this << phy);
  m_phy = phy;
  m_dcfManager->SetupPhyListener (phy);
  m_low->SetPhy (phy);
}

void
MeshWifiInterfaceMac::SetWifiRemoteStationManager (Ptr<WifiRemoteStationManager> stationManager)
{
  NS_LOG_FUNCTION (this << stationManager);
  m_stationManager = stationManager;
  m_BE->SetWifiRemoteStationManager (stationManager);
  m_VO->SetWifiRemoteStationManager (stationManager);
  m_beaconDca->SetWifiRemoteStationManager (stationManager);
  m_low->SetWifiRemoteStationManager (stationManager);
}

void
MeshWifiInterfaceMac::Enqueue (Ptr<const Packet> packet, Mac48Address to, Mac48Address from)
{
  NS_LOG_FUNCTION (this << packet << to << from);
  ForwardDown (packet, from, to);
}

void
MeshWifiInterfaceMac::Enqueue (Ptr<const Packet> packet, Mac48Address to)
{
  NS_LOG_FUNCTION (this << packet << to);
  ForwardDown (packet, m_low->GetAddress (), to);
}

bool
MeshWifiInterfaceMac::SupportsSendFrom () const
  {
    return true;
  }

void
MeshWifiInterfaceMac::SetForwardUpCallback (Callback<void,Ptr<Packet>, Mac48Address, Mac48Address> upCallback)
{
  NS_LOG_FUNCTION (this);
  m_upCallback = upCallback;
}

void
MeshWifiInterfaceMac::SetLinkUpCallback (Callback<void> linkUp)
{
  NS_LOG_FUNCTION (this);
  if (!linkUp.IsNull ())
    {
      linkUp ();
    }
}

void
MeshWifiInterfaceMac::SetLinkDownCallback (Callback<void> linkDown)
{
  NS_LOG_FUNCTION (this);
}

Mac48Address
MeshWifiInterfaceMac::GetAddress () const
  {
    return m_address;
  }
Mac48Address
MeshWifiInterfaceMac::GetBssid () const
  {
    return m_address;
  }

Ssid
MeshWifiInterfaceMac::GetSsid () const
  {
    return m_MeshId;
  }

void
MeshWifiInterfaceMac::SetAddress (Mac48Address address)
{
  NS_LOG_FUNCTION (address);
  m_low->SetAddress(address);
  m_address = address;
}

void
MeshWifiInterfaceMac::SetSsid (Ssid ssid)
{
  NS_LOG_FUNCTION (ssid);
  m_MeshId = ssid;
}

void
MeshWifiInterfaceMac::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_rxMiddle;
  delete m_dcfManager;
  //Delete smart pointers:
  m_rxMiddle = 0;
  m_low = 0;
  m_dcfManager = 0;
  m_phy = 0;
  m_BE = 0;
  m_VO = 0;
  m_beaconSendEvent.Cancel ();
  m_beaconDca = 0;
  
  WifiMac::DoDispose ();
}

//-----------------------------------------------------------------------------
// Forward frame up/down
//-----------------------------------------------------------------------------
void
MeshWifiInterfaceMac::ForwardUp (Ptr<Packet> packet, Mac48Address src, Mac48Address dst)
{
  NS_LOG_FUNCTION (this << packet << src);
  m_upCallback (packet, src, dst);
}

void
MeshWifiInterfaceMac::ForwardDown (Ptr<const Packet> packet, Mac48Address from, Mac48Address to)
{
  // 1. Create and add mesh header using routing information
  WifiMacHeader hdr;
  Ptr<Packet> packet_to_send = packet->Copy();
  
  /*
   TODO:
  for all plugins {
    plugin.UpdateOutcomingPacket....(packet, from, to);
  }
  */
  
  /*
  WifiMeshHeader meshHdr;
  
  // TODO: Address 1 we receive from HWMP tag
  HwmpTag tag;
  NS_ASSERT(packet->FindFirstMatchingTag(tag));
  meshHdr.SetMeshTtl(tag.GetTtl());
  meshHdr.SetMeshSeqno(tag.GetSeqno());
#if 0
  NS_LOG_DEBUG(
    "TX Packet sa = "<<from<<
    ", da = "<<to<<
    ", ra = "<<tag.GetAddress()<<
    ", I am "<<GetAddress()<<
    ", ttl = "<<(int)meshHdr.GetMeshTtl()
  );
#endif
  if (to!= Mac48Address::GetBroadcast())
    NS_ASSERT(tag.GetAddress()!=Mac48Address::GetBroadcast());
  
  hdr.SetTypeData ();
  hdr.SetAddr1 (tag.GetAddress());
  hdr.SetAddr2 (GetAddress ());
  hdr.SetAddr3 (to);
  hdr.SetAddr4 (from);
  hdr.SetDsFrom ();
  hdr.SetDsTo ();
  
  packet_to_send->AddHeader(meshHdr);
  */
  
  // 2. Queue frame
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
  m_BE->Queue (packet_to_send, hdr);
}

SupportedRates
MeshWifiInterfaceMac::GetSupportedRates () const
{
  // set the set of supported rates and make sure that we indicate
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

//-----------------------------------------------------------------------------
// Beacons
//-----------------------------------------------------------------------------
void
MeshWifiInterfaceMac::SetSoftwareDelay(Time delay)
{
  NS_ASSERT(delay < m_beaconInterval);
  m_softwareDelay = delay;
}

Time
MeshWifiInterfaceMac::GetSoftwareDelay()
{
  return m_softwareDelay;
}

Time
MeshWifiInterfaceMac::CalcSwDelay()
{
  UniformVariable coefficient(0.0, m_softwareDelay.GetSeconds());
  
  // Be sure that frames don't change order due to different software delays
  Time delay = Seconds(coefficient.GetValue());
  if (delay.GetSeconds() + Simulator::Now().GetSeconds() < m_lastMgtFrame.GetSeconds())
    delay = Seconds(m_lastMgtFrame.GetSeconds() - Simulator::Now().GetSeconds());
  m_lastMgtFrame = Seconds(Simulator::Now().GetSeconds() + delay.GetSeconds());

  NS_ASSERT(delay.GetSeconds() >= 0);
  return delay;
}

void
MeshWifiInterfaceMac::SetBeaconInterval (Time interval)
{
  NS_LOG_FUNCTION (this << interval);
  m_beaconInterval = interval;
  
  NS_ASSERT(m_beaconInterval > m_softwareDelay);
}

Time 
MeshWifiInterfaceMac::GetBeaconInterval() const
{
  return m_beaconInterval;
}

void
MeshWifiInterfaceMac::SetBeaconGeneration (bool enable)
{
  NS_LOG_FUNCTION (this << enable);
  if (enable)
    {
      // Now start sending beacons after some random delay (to avoid collisions)
      UniformVariable coefficient(0.0, m_randomStart.GetSeconds());
      Time randomStart = Seconds(coefficient.GetValue());
      
      m_beaconSendEvent = Simulator::Schedule (randomStart, &MeshWifiInterfaceMac::SendBeacon, this);
    }
  else
    // stop sending beacons
    m_beaconSendEvent.Cancel ();
}

bool
MeshWifiInterfaceMac::GetBeaconGeneration () const
{
  return m_beaconSendEvent.IsRunning ();
}

Time
MeshWifiInterfaceMac::GetTBTT() const
{
  return m_tbtt;
}

void MeshWifiInterfaceMac::ShiftTBTT(Time shift)
{
  // User of ShiftTBTT() must take care don't shift it to the past
  NS_ASSERT(GetTBTT() + shift > Simulator::Now());
  
  m_tbtt += shift;
  // Shift scheduled event
  Simulator::Cancel (m_beaconSendEvent);
  m_beaconSendEvent = Simulator::Schedule (GetTBTT(), &MeshWifiInterfaceMac::SendBeacon, this);
}


void 
MeshWifiInterfaceMac::ScheduleNextBeacon()
{
  m_tbtt += GetBeaconInterval();
  m_beaconSendEvent = Simulator::Schedule (GetTBTT(), &MeshWifiInterfaceMac::SendBeacon, this);
}

void
MeshWifiInterfaceMac::SendBeacon ()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG(GetAddress() <<" is sending beacon");
  
  NS_ASSERT(! m_beaconSendEvent.IsRunning());
  NS_ASSERT(Simulator::Now() == GetTBTT());     // assert that beacon is just on time
   
  // Form & send beacon
  MeshWifiBeacon beacon(GetSsid (), GetSupportedRates (), m_beaconInterval.GetMicroSeconds ());
  
  /*
    TODO ask all plugins to add smth. to beacon
   for all plugins {
     plugin.UpdateBeacon(beacon);
   }
  */
  
  m_beaconDca->Queue(beacon.CreatePacket(), beacon.CreateHeader(GetAddress()));
  
  ScheduleNextBeacon();
}


void
MeshWifiInterfaceMac::Receive (Ptr<Packet> packet, WifiMacHeader const *hdr)
{
  /* TODO
  if (hdr->IsBeacon ())
    {
      MgtMeshBeaconHeader beacon;
      Mac48Address from = hdr->GetAddr2();
      packet->RemoveHeader (beacon);
      NS_LOG_DEBUG("Beacon received from "<<hdr->GetAddr2()<<
                   " to "<<GetAddress()<<
                   " at "<<Simulator::Now ().GetMicroSeconds ()<<
                   " microseconds");
#if 0
      NeighboursTimingUnitsList neighbours;
      neighbours = beacon.GetWifiBeaconTimingElement().GetNeighboursTimingElementsList();
      for (NeighboursTimingUnitsList::const_iterator j = neighbours.begin(); j!= neighbours.end(); j++)
        fprintf(
          stderr,
          "neigbours:\nAID=%u, last_beacon=%u ,beacon_interval=%u\n",
          (*j)->GetAID(),
          (*j)->GetLastBeacon(),
          (*j)->GetBeaconInterval()
        );
#endif
      m_peerManager->SetReceivedBeaconTimers(
        GetAddress(),
        from,
        Simulator::Now (),
        MicroSeconds(beacon.GetBeaconIntervalUs()),
        beacon.GetWifiBeaconTimingElement()
      );
      if (!beacon.GetSsid().IsEqual(GetSsid()))
        return;
      SupportedRates rates = beacon.GetSupportedRates ();
      WifiRemoteStation *peerSta = m_stationManager->Lookup (hdr->GetAddr2 ());
      for (uint32_t i = 0; i < m_phy->GetNModes (); i++)
        {
          WifiMode mode = m_phy->GetMode (i);
          if (rates.IsSupportedRate (mode.GetDataRate ()))
            {
              peerSta->AddSupportedMode (mode);
              if (rates.IsBasicRate (mode.GetDataRate ()))
                {
                  m_stationManager->AddBasicMode (mode);
                }
            }
        }
      // TODO:Chack MeshConfigurationElement(now is nothing
      // to be checked)
      m_peerManager->AskIfOpenNeeded(GetAddress(), from);
      return;
    }
  if (hdr->IsMultihopAction())
    {
      WifiMeshHeader meshHdr;
      //no mesh header parameters are needed here:
      //TODO: check TTL
      packet->RemoveHeader(meshHdr);
      WifiMeshMultihopActionHeader multihopHdr;
      //parse multihop action header:
      packet->RemoveHeader(multihopHdr);
      WifiMeshMultihopActionHeader::ACTION_VALUE
      actionValue = multihopHdr.GetAction();
      switch (multihopHdr.GetCategory())
        {
        case WifiMeshMultihopActionHeader::MESH_PEER_LINK_MGT:
        {
          Mac48Address peerAddress;
          MeshMgtPeerLinkManFrame peer_frame;
          if (hdr->GetAddr1 () != GetAddress ())
            return;
          peerAddress = hdr->GetAddr2();
          packet->RemoveHeader (peer_frame);
          if (actionValue.peerLink != WifiMeshMultihopActionHeader::PEER_LINK_CLOSE)
            {
              //check Supported Rates
              SupportedRates rates = peer_frame.GetSupportedRates();
              for (uint32_t i = 0; i < m_stationManager->GetNBasicModes (); i++)
                {
                  WifiMode mode = m_stationManager->GetBasicMode (i);
                  if (!rates.IsSupportedRate (mode.GetDataRate ()))
                    {
                      m_peerManager->ConfigurationMismatch(GetAddress(), peerAddress);
                      return;
                    }
                }
              //Check SSID
              if (!peer_frame.GetMeshId().IsEqual(GetSsid()))
                {
                  m_peerManager->ConfigurationMismatch(GetAddress(), peerAddress);
                  return;
                }
            }
          switch (actionValue.peerLink)
            {
            case WifiMeshMultihopActionHeader::PEER_LINK_CONFIRM:
              m_peerManager->SetConfirmReceived(
                GetAddress(),
                peerAddress,
                peer_frame.GetAid(),
                peer_frame.GetPeerLinkManagementElement(),
                m_meshConfig
              );
              return;
            case WifiMeshMultihopActionHeader::PEER_LINK_OPEN:
              m_peerManager->SetOpenReceived(
                GetAddress(),
                peerAddress,
                peer_frame.GetPeerLinkManagementElement(),
                m_meshConfig
              );
              return;
            case WifiMeshMultihopActionHeader::PEER_LINK_CLOSE:
              m_peerManager->SetCloseReceived(
                GetAddress(),
                peerAddress,
                peer_frame.GetPeerLinkManagementElement()
              );
              return;
            default:
              return;
            }
          break;
        }
        case WifiMeshMultihopActionHeader::MESH_PATH_SELECTION:
        {
          if (!m_peerManager->IsActiveLink(GetAddress(), hdr->GetAddr2()))
            return;
          switch (actionValue.pathSelection)
            {
            case WifiMeshMultihopActionHeader::PATH_REQUEST:
            {
              WifiPreqInformationElement preq;
              packet->RemoveHeader(preq);
              //TODO:recalculate
              //metric
              m_preqReceived(preq, hdr->GetAddr2(), CalculateMetric(hdr->GetAddr2()));
              return;
            }
            case WifiMeshMultihopActionHeader::PATH_REPLY:
            {
              WifiPrepInformationElement prep;
              packet->RemoveHeader(prep);
              m_prepReceived(prep, hdr->GetAddr2(), CalculateMetric(hdr->GetAddr2()));
            }
            return;
            case WifiMeshMultihopActionHeader::PATH_ERROR:
            {
              WifiPerrInformationElement perr;
              packet->RemoveHeader(perr);
              m_perrReceived(perr, hdr->GetAddr2());
            }
            return;
            case WifiMeshMultihopActionHeader::ROOT_ANNOUNCEMENT:
              return;
            }
        }
        default:
          break;
        }
    }
  if (hdr->IsData())
    {
      NS_ASSERT((hdr->IsFromDs()) && (hdr->IsToDs()));
      NS_ASSERT(hdr->GetAddr4()!=Mac48Address::GetBroadcast());
      //check seqno
      WifiMeshHeader meshHdr;
      packet->RemoveHeader(meshHdr);
      NS_LOG_DEBUG(
        "DATA TA="<< hdr->GetAddr2()<<
        ", da="<<hdr->GetAddr3()<<
        ", sa="<<hdr->GetAddr4()<<
        ", TTL="<<(int)meshHdr.GetMeshTtl());
      HwmpTag tag;
      //mesh header is present within DATA and multihop action frames, so it must be done within MAC
      tag.SetSeqno(meshHdr.GetMeshSeqno());
      tag.SetAddress(hdr->GetAddr2());
      tag.SetTtl(meshHdr.GetMeshTtl());
      //metric should be later
      packet->RemoveAllTags();
      packet->AddTag(tag);
      if (m_peerManager->IsActiveLink(GetAddress(), hdr->GetAddr2()))
        ForwardUp(packet, hdr->GetAddr4(), hdr->GetAddr3());
    }
    */
}
  
} // namespace ns3



