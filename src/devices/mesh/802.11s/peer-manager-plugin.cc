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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */

#include "ns3/mesh-wifi-interface-mac.h"
#include "ie-dot11s-configuration.h"
#include "ie-dot11s-peer-management.h"
#include "peer-manager-plugin.h"
#include "ns3/mesh-wifi-mac-header.h"
#include "ns3/simulator.h"
#include "ns3/peer-link-frame.h"
#include "ns3/wifi-mac-header.h"
#include "ns3/mesh-wifi-mac-header.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE("PeerManager");
namespace ns3 {
Dot11sPeerManagerMacPlugin::Dot11sPeerManagerMacPlugin (uint32_t interface, Ptr<Dot11sPeerManagerProtocol> protocol)
{
  m_ifIndex = interface;
  m_protocol = protocol;
}

Dot11sPeerManagerMacPlugin::~Dot11sPeerManagerMacPlugin ()
{
}

void
Dot11sPeerManagerMacPlugin::SetParent (Ptr<MeshWifiInterfaceMac> parent)
{
  NS_LOG_UNCOND("ADD PARENT");
  m_parent = parent;
}

bool
Dot11sPeerManagerMacPlugin::Receive (Ptr<Packet> packet, const WifiMacHeader & header)
{
  NS_LOG_UNCOND("received a frame");
  if(header.IsBeacon())
  {
    NS_LOG_UNCOND("Beacon recevied by PM from"<<header.GetAddr2 ());
    IeDot11sBeaconTiming beaconTiming;
    Ptr<Packet> myBeacon = packet->Copy();
    MgtBeaconHeader beacon_hdr;
    myBeacon->RemoveHeader(beacon_hdr);
    if(myBeacon->GetSize () == 0)
      NS_LOG_UNCOND("Empty");
    bool meshBeacon = false;
    if(beaconTiming.FindMyInformationElement(myBeacon))
      meshBeacon = true;
    m_protocol->ReceiveBeacon(
        m_ifIndex,
        meshBeacon,
        beaconTiming,
        header.GetAddr2(),
        Simulator::Now(),
        MicroSeconds(beacon_hdr.GetBeaconIntervalUs())
        );
  }
  if(header.IsMultihopAction())
  {
    if (header.GetAddr1 () != m_parent->GetAddress ())
      return true;

    WifiMeshHeader meshHdr;
    packet->RemoveHeader (meshHdr);
    WifiMeshMultihopActionHeader multihopHdr;
    //parse multihop action header:
    packet->RemoveHeader (multihopHdr);
    WifiMeshMultihopActionHeader::ACTION_VALUE actionValue = multihopHdr.GetAction ();
    if(multihopHdr.GetCategory () != WifiMeshMultihopActionHeader::MESH_PEER_LINK_MGT)
      return false;
    Mac48Address peerAddress = header.GetAddr2 ();
    PeerLinkFrameStart::PlinkFrameStartFields fields;
    {
      PeerLinkFrameStart peerFrame;
      packet->RemoveHeader (peerFrame);
      fields = peerFrame.GetFields();
    }
    if (actionValue.peerLink != WifiMeshMultihopActionHeader::PEER_LINK_CLOSE)
    {
      if(!(m_parent->CheckSupportedRates(fields.rates)))
      {
        NS_ASSERT(false);
        //m_peerManager->ConfigurationMismatch (GetAddress(), peerAddress);
        return true;
      }
      if (!fields.meshId.IsEqual(m_parent->GetSsid()))
      {
        NS_ASSERT(false);
        //m_peerManager->ConfigurationMismatch (GetAddress(), peerAddress);
        return true;
      }
    }
  
    switch (actionValue.peerLink)
    {
      case WifiMeshMultihopActionHeader::PEER_LINK_CONFIRM:
        return true;
      case WifiMeshMultihopActionHeader::PEER_LINK_OPEN:
        NS_LOG_UNCOND("OPEN RECEIVED");
        NS_ASSERT(false);
        return true;
      case WifiMeshMultihopActionHeader::PEER_LINK_CLOSE:
        return true;
      default:
        return false;
    }
  } 
  return false;
}

bool
Dot11sPeerManagerMacPlugin::UpdateOutcomingFrame (Ptr<Packet> packet, WifiMacHeader & header, Mac48Address from, Mac48Address to) const
{
  return false;
}

void
Dot11sPeerManagerMacPlugin::UpdateBeacon (MeshWifiBeacon & beacon) const
{
#if 0
  NS_LOG_UNCOND("I am sending a beacon");
  Ptr<IeDot11sPrep>  prep = Create<IeDot11sPrep> ();
#endif
  Ptr<IeDot11sBeaconTiming>  beaconTiming = 
    m_protocol->SendBeacon(
        m_ifIndex,
        Simulator::Now(),
        MicroSeconds(beacon.BeaconHeader().GetBeaconIntervalUs()));
  beacon.AddInformationElement(beaconTiming);
}
void
Dot11sPeerManagerMacPlugin::SendPeerLinkManagementFrame(
      Mac48Address peerAddress,
      uint16_t aid,
      IeDot11sPeerManagement peerElement,
      IeDot11sConfiguration meshConfig
      )
{
  NS_LOG_UNCOND("sending open");
  //Create a packet:
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader(peerElement);
  packet->AddHeader(meshConfig);
  PeerLinkFrameStart::PlinkFrameStartFields fields;
  fields.subtype = peerElement.GetSubtype();
  fields.aid = aid;
  fields.rates = m_parent->GetSupportedRates ();
  fields.meshId = m_parent->GetSsid ();
  PeerLinkFrameStart plinkFrame;
  plinkFrame.SetPlinkFrameStart(fields);
  packet->AddHeader(plinkFrame);
  //Create an 802.11 frame header:
  //Send management frame to MAC:
  WifiMeshMultihopActionHeader multihopHdr;
  if (peerElement.SubtypeIsOpen ())
    {
      WifiMeshMultihopActionHeader::ACTION_VALUE action;
      action.peerLink = WifiMeshMultihopActionHeader::PEER_LINK_OPEN;
      multihopHdr.SetAction (WifiMeshMultihopActionHeader::MESH_PEER_LINK_MGT, action);
    }
  if (peerElement.SubtypeIsConfirm ())
    {
      WifiMeshMultihopActionHeader::ACTION_VALUE action;
      action.peerLink = WifiMeshMultihopActionHeader::PEER_LINK_CONFIRM;
      multihopHdr.SetAction (WifiMeshMultihopActionHeader::MESH_PEER_LINK_MGT, action);
    }
  if (peerElement.SubtypeIsClose ())
    {
      WifiMeshMultihopActionHeader::ACTION_VALUE action;
      action.peerLink = WifiMeshMultihopActionHeader::PEER_LINK_CLOSE;
      multihopHdr.SetAction (WifiMeshMultihopActionHeader::MESH_PEER_LINK_MGT, action);
    }
  packet->AddHeader (multihopHdr);
  //mesh header:
  WifiMeshHeader meshHdr;
  meshHdr.SetMeshTtl (1);
  meshHdr.SetMeshSeqno (0);
  packet->AddHeader (meshHdr);
  //Wifi Mac header:
  WifiMacHeader hdr;
  hdr.SetMultihopAction ();
  hdr.SetAddr1 (peerAddress);
  hdr.SetAddr2 (m_parent->GetAddress ());
  hdr.SetAddr3 (m_parent->GetAddress ());
  hdr.SetDsNotFrom ();
  hdr.SetDsNotTo ();
  m_parent->SendManagementFrame(packet, hdr);
}
}//namespace ns3
