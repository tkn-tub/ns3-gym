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
    WifiMeshActionHeader actionHdr;
    packet->RemoveHeader (actionHdr);
    WifiMeshActionHeader::ActionValue actionValue = actionHdr.GetAction ();
    // If can not handle - just return;
    if(actionHdr.GetCategory () != WifiMeshActionHeader::MESH_PEER_LINK_MGT)
      return m_protocol->IsActiveLink(m_ifIndex,header.GetAddr2());
    m_stats.recvMgt ++;
    m_stats.recvMgtBytes += packet->GetSize ();
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
        (actionValue.peerLink != WifiMeshActionHeader::PEER_LINK_CLOSE) &&
        !(m_parent->CheckSupportedRates(fields.rates))
        )
      {
        m_protocol->ConfigurationMismatch (m_ifIndex, peerAddress);
        // Broken peer link frame - drop it
        m_stats.brokenMgt ++;
        return false;
      }
    if (
        (actionValue.peerLink != WifiMeshActionHeader::PEER_LINK_CONFIRM) &&
        !fields.meshId.IsEqual(m_parent->GetSsid())
        )
      {
        m_protocol->ConfigurationMismatch (m_ifIndex, peerAddress);
        // Broken peer link frame - drop it
        m_stats.brokenMgt ++;
        return false;
      }
    IePeerManagement peerElement;
    packet->RemoveHeader(peerElement);
    //Check taht frame subtype corresponds peer link subtype
    if(peerElement.SubtypeIsOpen ())
    {
      m_stats.recvOpen ++;
      NS_ASSERT(actionValue.peerLink == WifiMeshActionHeader::PEER_LINK_OPEN);
    }
    if(peerElement.SubtypeIsConfirm ())
    {
      m_stats.recvConfirm ++;
      NS_ASSERT(actionValue.peerLink == WifiMeshActionHeader::PEER_LINK_CONFIRM);
    }
    if(peerElement.SubtypeIsClose ())
    {
      m_stats.recvClose ++;
      NS_ASSERT(actionValue.peerLink == WifiMeshActionHeader::PEER_LINK_CLOSE);
    }
    //Deliver Peer link management frame to protocol:
    m_protocol->ReceivePeerLinkFrame(m_ifIndex, peerAddress, peerMpAddress, fields.aid, peerElement, fields.config);
    // if we can handle a frame - drop it
    return false;
  }
  return m_protocol->IsActiveLink(m_ifIndex,header.GetAddr2());
}
bool
PeerManagerMacPlugin::UpdateOutcomingFrame (Ptr<Packet> packet, WifiMacHeader & header, Mac48Address from, Mac48Address to)
{
  if(header.IsAction ())
  {
    WifiMeshActionHeader actionHdr;
    packet->PeekHeader (actionHdr);
    WifiMeshActionHeader::ActionValue actionValue = actionHdr.GetAction ();
    if(actionHdr.GetCategory () == WifiMeshActionHeader::MESH_PEER_LINK_MGT)
      return true;
  }
  if(header.GetAddr1 ().IsGroup ())
    return true;
  else
  {
    if(m_protocol->IsActiveLink(m_ifIndex,header.GetAddr1()))
      return true;
    else
    {
      m_stats.dropped ++;
      return false;
    }
  }
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
  WifiMeshActionHeader actionHdr;
  if (peerElement.SubtypeIsOpen ())
    {
      m_stats.sendOpen ++;
      WifiMeshActionHeader::ActionValue action;
      action.peerLink = WifiMeshActionHeader::PEER_LINK_OPEN;
      fields.subtype = WifiMeshActionHeader::PEER_LINK_OPEN;
      actionHdr.SetAction (WifiMeshActionHeader::MESH_PEER_LINK_MGT, action);
    }
  if (peerElement.SubtypeIsConfirm ())
    {
      m_stats.sendConfirm ++;
      WifiMeshActionHeader::ActionValue action;
      action.peerLink = WifiMeshActionHeader::PEER_LINK_CONFIRM;
      fields.aid = aid;
      fields.subtype = WifiMeshActionHeader::PEER_LINK_CONFIRM;
      actionHdr.SetAction (WifiMeshActionHeader::MESH_PEER_LINK_MGT, action);
    }
  if (peerElement.SubtypeIsClose ())
    {
      m_stats.sendClose ++;
      WifiMeshActionHeader::ActionValue action;
      action.peerLink = WifiMeshActionHeader::PEER_LINK_CLOSE;
      fields.subtype = WifiMeshActionHeader::PEER_LINK_CLOSE;
      fields.reasonCode = peerElement.GetReasonCode ();
      actionHdr.SetAction (WifiMeshActionHeader::MESH_PEER_LINK_MGT, action);
    }
  plinkFrame.SetPlinkFrameStart(fields);
  packet->AddHeader (plinkFrame);
  packet->AddHeader (actionHdr);
  m_stats.sentMgt ++;
  m_stats.sentMgtBytes += packet->GetSize ();
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
  if(shift != Seconds (0))
    m_stats.beaconShift ++;
  m_parent->ShiftTbtt (shift);
}
PeerManagerMacPlugin::Statistics::Statistics () :
  sendOpen (0),
  sendConfirm (0),
  sendClose (0),
  recvOpen (0),
  recvConfirm (0),
  recvClose (0),
  dropped (0),
  brokenMgt (0),
  sentMgt (0),
  sentMgtBytes (0),
  recvMgt (0),
  recvMgtBytes (0),
  beaconShift (0)
{
}
void
PeerManagerMacPlugin::Statistics::Print (std::ostream & os) const
{
  os << "sendOpen=\"" << sendOpen << "\""
    "sendConfirm=\"" << sendConfirm << "\""
    "sendClose=\"" << sendClose << "\""
    "recvOpen=\"" << recvOpen << "\""
    "recvConfirm=\"" << recvConfirm << "\""
    "recvClose=\"" << recvClose << "\""
    "dropped=\"" << dropped << "\""
    "brokenMgt=\"" << brokenMgt << "\""
    "sentMgt=\"" << sentMgt << "\""
    "sentMgtBytes=\"" << sentMgtBytes << "\""
    "recvMgt=\"" << recvMgt << "\""
    "recvMgtBytes=\"" << recvMgtBytes << "\""
    "beaconShift=\"" << beaconShift << "\"\n";
}
void
PeerManagerMacPlugin::Report (std::ostream & os) const
{
  os << "<PMP-MAC "
    "index=\"" << m_ifIndex << "\">\n";
  m_stats.Print (os);
  os << "</PMP MAC>\n";
}
void
PeerManagerMacPlugin::ResetStats ()
{
  m_stats = Statistics::Statistics ();
}
} // namespace dot11s
} //namespace ns3

