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

#include "ie-dot11s-configuration.h"
#include "ie-dot11s-peer-management.h"
#include "dot11s-mac-header.h"
#include "peer-management-plugin.h"
#include "peer-management-protocol.h"
#include "peer-link-frame.h"
#include "ns3/mesh-wifi-interface-mac.h"
#include "ns3/simulator.h"
#include "ns3/wifi-mac-header.h"

namespace ns3 {
namespace dot11s {
PeerManagerMacPlugin::PeerManagerMacPlugin (uint32_t interface, Ptr<PeerManagementProtocol> protocol)
{
  m_ifIndex = interface;
  m_protocol = protocol;
}

PeerManagerMacPlugin::~PeerManagerMacPlugin ()
{
}

void
PeerManagerMacPlugin::SetParent (Ptr<MeshWifiInterfaceMac> parent)
{
  m_parent = parent;
}

bool
PeerManagerMacPlugin::Receive (Ptr<Packet> const_packet, const WifiMacHeader & header)
{
  // First of all we copy a packet, because we need to remove some
  //headers
  Ptr<Packet> packet = const_packet->Copy();
  if(header.IsBeacon())
  {
    IeBeaconTiming beaconTiming;
    Ptr<Packet> myBeacon = packet->Copy();
    MgtBeaconHeader beacon_hdr;
    myBeacon->RemoveHeader(beacon_hdr);
    bool meshBeacon = false;
    if(beaconTiming.FindFirst(myBeacon))
      meshBeacon = true;
    m_protocol->UpdatePeerBeaconTiming(
        m_ifIndex,
        meshBeacon,
        beaconTiming,
        header.GetAddr2(),
        Simulator::Now(),
        MicroSeconds(beacon_hdr.GetBeaconIntervalUs())
        );
    // Beacon shall not be dropeed. May be needed to another plugins
    return true;
  }
  if(header.IsMultihopAction())
  {
    Dot11sMacHeader meshHdr;
    packet->RemoveHeader (meshHdr);
    WifiMeshMultihopActionHeader multihopHdr;
    //parse multihop action header:
    packet->RemoveHeader (multihopHdr);
    WifiMeshMultihopActionHeader::ACTION_VALUE actionValue = multihopHdr.GetAction ();
    // If can not handle - just return;
    if(multihopHdr.GetCategory () != WifiMeshMultihopActionHeader::MESH_PEER_LINK_MGT)
      return true;
    NS_ASSERT(meshHdr.GetMeshTtl () == 1);
    NS_ASSERT(meshHdr.GetAddressExt () == 1);
    Mac48Address peerAddress = header.GetAddr2 ();
    Mac48Address peerMpAddress = meshHdr.GetAddr4 ();
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
        m_protocol->ConfigurationMismatch (m_ifIndex, peerAddress);
        // Broken peer link frame - drop it
        return false;
      }
      if (!fields.meshId.IsEqual(m_parent->GetSsid()))
      {
        m_protocol->ConfigurationMismatch (m_ifIndex, peerAddress);
        // Broken peer link frame - drop it
        return true;
      }
    }
    // MeshConfiguration Element - exists in all peer link management
    // frames except CLOSE
    IeConfiguration meshConfig;
    if(fields.subtype != IePeerManagement::PEER_CLOSE)
      packet->RemoveHeader(meshConfig);
    IePeerManagement peerElement;
    packet->RemoveHeader(peerElement);
    // Check the correspondance betwee action valuse and peer link
    // management element subtypes:
    switch (actionValue.peerLink)
    {
      case WifiMeshMultihopActionHeader::PEER_LINK_CONFIRM:
        NS_ASSERT(fields.subtype == IePeerManagement::PEER_CONFIRM);
        break;
      case WifiMeshMultihopActionHeader::PEER_LINK_OPEN:
        NS_ASSERT(fields.subtype == IePeerManagement::PEER_OPEN);
        break;
      case WifiMeshMultihopActionHeader::PEER_LINK_CLOSE:
        NS_ASSERT(fields.subtype == IePeerManagement::PEER_CLOSE);
        break;
      default:
        // Protocol can not define which frame is it - pass further
        return true;
    }
    //Deliver Peer link management frame to protocol:
    m_protocol->ReceivePeerLinkFrame(m_ifIndex, peerAddress, peerMpAddress, fields.aid, peerElement, meshConfig);
    // if we can handle a frame - drop it
    return false;
  }
  return m_protocol->IsActiveLink(m_ifIndex,header.GetAddr2());
}

bool
PeerManagerMacPlugin::UpdateOutcomingFrame (Ptr<Packet> packet, WifiMacHeader & header, Mac48Address from, Mac48Address to) const
{
  return true;
}

void
PeerManagerMacPlugin::UpdateBeacon (MeshWifiBeacon & beacon) const
{
  Ptr<IeBeaconTiming>  beaconTiming = m_protocol->GetBeaconTimingElement(m_ifIndex);
  beacon.AddInformationElement(beaconTiming);
}

void
PeerManagerMacPlugin::SendPeerLinkManagementFrame(
      Mac48Address peerAddress,
      Mac48Address peerMpAddress,
      uint16_t aid,
      IePeerManagement peerElement,
      IeConfiguration meshConfig
      )
{
  //Create a packet:
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (peerElement);
  if(!peerElement.SubtypeIsClose())
    packet->AddHeader (meshConfig);
  PeerLinkFrameStart::PlinkFrameStartFields fields;
  fields.subtype = peerElement.GetSubtype();
  fields.aid = aid;
  fields.rates = m_parent->GetSupportedRates ();
  fields.meshId = m_parent->GetSsid ();
  PeerLinkFrameStart plinkFrame;
  plinkFrame.SetPlinkFrameStart(fields);
  packet->AddHeader (plinkFrame);
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
  Dot11sMacHeader meshHdr;
  meshHdr.SetMeshTtl (1);
  meshHdr.SetMeshSeqno (0);
  meshHdr.SetAddressExt(1);
  meshHdr.SetAddr4(m_protocol->GetAddress ());
  packet->AddHeader (meshHdr);
  //Wifi Mac header:
  WifiMacHeader hdr;
  hdr.SetMultihopAction ();
  hdr.SetAddr1 (peerAddress);
  hdr.SetAddr2 (m_parent->GetAddress ());
  hdr.SetAddr3 (peerMpAddress);
  hdr.SetDsNotFrom ();
  hdr.SetDsNotTo ();
  m_parent->SendManagementFrame(packet, hdr);
}

Mac48Address
PeerManagerMacPlugin::GetAddress () const
{
  if(m_parent !=  0)
    return m_parent->GetAddress ();
  else return Mac48Address::Mac48Address();
}
std::pair<Time, Time> 
PeerManagerMacPlugin::GetBeaconInfo() const
{
  std::pair<Time,Time> retval;
  retval.first = m_parent->GetTbtt ();
  retval.second = m_parent->GetBeaconInterval ();
  return retval;
}
void
PeerManagerMacPlugin::SetBeaconShift(Time shift)
{
  m_parent->ShiftTbtt (shift);
}

} // namespace dot11s
} //namespace ns3

