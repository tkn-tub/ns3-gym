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
  if(header.IsAction())
  {
    WifiMeshMultihopActionHeader multihopHdr;
    //parse multihop action header:
    packet->RemoveHeader (multihopHdr);
    WifiMeshMultihopActionHeader::ActionValue actionValue = multihopHdr.GetAction ();
    // If can not handle - just return;
    if(multihopHdr.GetCategory () != WifiMeshMultihopActionHeader::MESH_PEER_LINK_MGT)
      return true;
    Mac48Address peerAddress = header.GetAddr2 ();
    Mac48Address peerMpAddress = header.GetAddr3 ();
    PeerLinkFrameStart::PlinkFrameStartFields fields;
    {
      PeerLinkFrameStart peerFrame;
      peerFrame.SetPlinkFrameSubtype((uint8_t)actionValue.peerLink);
      packet->RemoveHeader (peerFrame);
      fields = peerFrame.GetFields();
      NS_ASSERT(fields.subtype == actionValue.peerLink);
    }
    if (
        (actionValue.peerLink != WifiMeshMultihopActionHeader::PEER_LINK_CLOSE) &&
        !(m_parent->CheckSupportedRates(fields.rates))
        )
      {
        m_protocol->ConfigurationMismatch (m_ifIndex, peerAddress);
        // Broken peer link frame - drop it
        return false;
      }
    if (
        (actionValue.peerLink != WifiMeshMultihopActionHeader::PEER_LINK_CONFIRM) &&
        !fields.meshId.IsEqual(m_parent->GetSsid())
        )
      {
        m_protocol->ConfigurationMismatch (m_ifIndex, peerAddress);
        // Broken peer link frame - drop it
        return false;
      }
    IePeerManagement peerElement;
    packet->RemoveHeader(peerElement);
    //Check taht frame subtype corresponds peer link subtype
    if(peerElement.SubtypeIsOpen ())
    {
      NS_ASSERT(actionValue.peerLink == WifiMeshMultihopActionHeader::PEER_LINK_OPEN);
    }
    if(peerElement.SubtypeIsConfirm ())
    {
      NS_ASSERT(actionValue.peerLink == WifiMeshMultihopActionHeader::PEER_LINK_CONFIRM);
    }
    if(peerElement.SubtypeIsClose ())
    {
      NS_ASSERT(actionValue.peerLink == WifiMeshMultihopActionHeader::PEER_LINK_CLOSE);
    }
    //Deliver Peer link management frame to protocol:
    m_protocol->ReceivePeerLinkFrame(m_ifIndex, peerAddress, peerMpAddress, fields.aid, peerElement, fields.config);
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
  PeerLinkFrameStart::PlinkFrameStartFields fields;
  fields.rates = m_parent->GetSupportedRates ();
  fields.meshId = m_parent->GetSsid ();
  fields.config = meshConfig;
  PeerLinkFrameStart plinkFrame;
  //Create an 802.11 frame header:
  //Send management frame to MAC:
  WifiMeshMultihopActionHeader multihopHdr;
  if (peerElement.SubtypeIsOpen ())
    {
      WifiMeshMultihopActionHeader::ActionValue action;
      action.peerLink = WifiMeshMultihopActionHeader::PEER_LINK_OPEN;
      fields.subtype = WifiMeshMultihopActionHeader::PEER_LINK_OPEN;
      multihopHdr.SetAction (WifiMeshMultihopActionHeader::MESH_PEER_LINK_MGT, action);
    }
  if (peerElement.SubtypeIsConfirm ())
    {
      WifiMeshMultihopActionHeader::ActionValue action;
      action.peerLink = WifiMeshMultihopActionHeader::PEER_LINK_CONFIRM;
      fields.aid = aid;
      fields.subtype = WifiMeshMultihopActionHeader::PEER_LINK_CONFIRM;
      multihopHdr.SetAction (WifiMeshMultihopActionHeader::MESH_PEER_LINK_MGT, action);
    }
  if (peerElement.SubtypeIsClose ())
    {
      WifiMeshMultihopActionHeader::ActionValue action;
      action.peerLink = WifiMeshMultihopActionHeader::PEER_LINK_CLOSE;
      fields.subtype = WifiMeshMultihopActionHeader::PEER_LINK_CLOSE;
      fields.reasonCode = peerElement.GetReasonCode ();
      multihopHdr.SetAction (WifiMeshMultihopActionHeader::MESH_PEER_LINK_MGT, action);
    }
  plinkFrame.SetPlinkFrameStart(fields);
  packet->AddHeader (plinkFrame);
  packet->AddHeader (multihopHdr);
  // Wifi Mac header:
  WifiMacHeader hdr;
  hdr.SetAction ();
  hdr.SetAddr1 (peerAddress);
  hdr.SetAddr2 (m_parent->GetAddress ());
  //Addr is not used here, we use it as our MP address
  hdr.SetAddr3 (m_protocol->GetAddress ());
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

