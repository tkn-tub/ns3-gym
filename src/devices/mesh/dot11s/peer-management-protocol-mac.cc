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
#include "peer-management-protocol-mac.h"
#include "peer-management-protocol.h"
#include "peer-link-frame.h"
#include "ns3/mesh-wifi-interface-mac.h"
#include "ns3/simulator.h"
#include "ns3/wifi-mac-header.h"
namespace ns3 {
namespace dot11s {
PeerManagementProtocolMac::PeerManagementProtocolMac (uint32_t interface, Ptr<PeerManagementProtocol> protocol)
{
  m_ifIndex = interface;
  m_protocol = protocol;
}

PeerManagementProtocolMac::~PeerManagementProtocolMac ()
{
}

void
PeerManagementProtocolMac::SetParent (Ptr<MeshWifiInterfaceMac> parent)
{
  m_parent = parent;
}

bool
PeerManagementProtocolMac::Receive (Ptr<Packet> const_packet, const WifiMacHeader & header)
{
  // First of all we copy a packet, because we need to remove some
  //headers
  Ptr<Packet> packet = const_packet->Copy();
  if(header.IsBeacon())
  {
    IeBeaconTiming beaconTiming;
    IeMeshId meshId;
    Ptr<Packet> myBeacon = packet->Copy();
    MgtBeaconHeader beacon_hdr;
    myBeacon->RemoveHeader(beacon_hdr);
    meshId.FindFirst(myBeacon);
    bool meshBeacon = false;
    if (
        (beaconTiming.FindFirst(myBeacon)) &&
        (m_protocol->GetMeshId ()->IsEqual(meshId))
        )
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
    if(actionHdr.GetCategory () != WifiMeshActionHeader::MESH_PEERING_MGT)
      return m_protocol->IsActiveLink(m_ifIndex,header.GetAddr2());
    m_stats.rxMgt ++;
    m_stats.rxMgtBytes += packet->GetSize ();
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
        !fields.meshId.IsEqual(*(m_protocol->GetMeshId()))
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
      m_stats.rxOpen ++;
      NS_ASSERT(actionValue.peerLink == WifiMeshActionHeader::PEER_LINK_OPEN);
    }
    if(peerElement.SubtypeIsConfirm ())
    {
      m_stats.rxConfirm ++;
      NS_ASSERT(actionValue.peerLink == WifiMeshActionHeader::PEER_LINK_CONFIRM);
    }
    if(peerElement.SubtypeIsClose ())
    {
      m_stats.rxClose ++;
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
PeerManagementProtocolMac::UpdateOutcomingFrame (Ptr<Packet> packet, WifiMacHeader & header, Mac48Address from, Mac48Address to)
{
  if(header.IsAction ())
  {
    WifiMeshActionHeader actionHdr;
    packet->PeekHeader (actionHdr);
    WifiMeshActionHeader::ActionValue actionValue = actionHdr.GetAction ();
    if(actionHdr.GetCategory () == WifiMeshActionHeader::MESH_PEERING_MGT)
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
PeerManagementProtocolMac::UpdateBeacon (MeshWifiBeacon & beacon) const
{
  Ptr<IeBeaconTiming>  beaconTiming = m_protocol->GetBeaconTimingElement(m_ifIndex);
  beacon.AddInformationElement(beaconTiming);
  beacon.AddInformationElement(m_protocol->GetMeshId ());
}

void
PeerManagementProtocolMac::SendPeerLinkManagementFrame(
      Mac48Address peerAddress,
      Mac48Address peerMpAddress,
      uint16_t aid,
      IePeerManagement peerElement,
      IeConfiguration meshConfig
      )
{
  //Create a packet:
  meshConfig.SetNeighborCount (m_protocol->GetNumberOfLinks ());
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (peerElement);
  PeerLinkFrameStart::PlinkFrameStartFields fields;
  fields.rates = m_parent->GetSupportedRates ();
  fields.capability = 0;
  fields.meshId = *(m_protocol->GetMeshId ());
  fields.config = meshConfig;
  PeerLinkFrameStart plinkFrame;
  //Create an 802.11 frame header:
  //Send management frame to MAC:
  WifiMeshActionHeader actionHdr;
  if (peerElement.SubtypeIsOpen ())
    {
      m_stats.txOpen ++;
      WifiMeshActionHeader::ActionValue action;
      action.peerLink = WifiMeshActionHeader::PEER_LINK_OPEN;
      fields.subtype = WifiMeshActionHeader::PEER_LINK_OPEN;
      actionHdr.SetAction (WifiMeshActionHeader::MESH_PEERING_MGT, action);
    }
  if (peerElement.SubtypeIsConfirm ())
    {
      m_stats.txConfirm ++;
      WifiMeshActionHeader::ActionValue action;
      action.peerLink = WifiMeshActionHeader::PEER_LINK_CONFIRM;
      fields.aid = aid;
      fields.subtype = WifiMeshActionHeader::PEER_LINK_CONFIRM;
      actionHdr.SetAction (WifiMeshActionHeader::MESH_PEERING_MGT, action);
    }
  if (peerElement.SubtypeIsClose ())
    {
      m_stats.txClose ++;
      WifiMeshActionHeader::ActionValue action;
      action.peerLink = WifiMeshActionHeader::PEER_LINK_CLOSE;
      fields.subtype = WifiMeshActionHeader::PEER_LINK_CLOSE;
      fields.reasonCode = peerElement.GetReasonCode ();
      actionHdr.SetAction (WifiMeshActionHeader::MESH_PEERING_MGT, action);
    }
  plinkFrame.SetPlinkFrameStart(fields);
  packet->AddHeader (plinkFrame);
  packet->AddHeader (actionHdr);
  m_stats.txMgt ++;
  m_stats.txMgtBytes += packet->GetSize ();
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
PeerManagementProtocolMac::GetAddress () const
{
  if(m_parent !=  0)
    return m_parent->GetAddress ();
  else return Mac48Address::Mac48Address();
}
std::pair<Time, Time> 
PeerManagementProtocolMac::GetBeaconInfo() const
{
  std::pair<Time,Time> retval;
  retval.first = m_parent->GetTbtt ();
  retval.second = m_parent->GetBeaconInterval ();
  return retval;
}
void
PeerManagementProtocolMac::SetBeaconShift(Time shift)
{
  if(shift != Seconds (0))
    m_stats.beaconShift ++;
  m_parent->ShiftTbtt (shift);
}
PeerManagementProtocolMac::Statistics::Statistics () :
  txOpen (0),
  txConfirm (0),
  txClose (0),
  rxOpen (0),
  rxConfirm (0),
  rxClose (0),
  dropped (0),
  brokenMgt (0),
  txMgt (0),
  txMgtBytes (0),
  rxMgt (0),
  rxMgtBytes (0),
  beaconShift (0)
{
}
void
PeerManagementProtocolMac::Statistics::Print (std::ostream & os) const
{
  os << "<Statistics "
    "txOpen=\"" << txOpen << "\"\n"
    "txConfirm=\"" << txConfirm << "\"\n"
    "txClose=\"" << txClose << "\"\n"
    "rxOpen=\"" << rxOpen << "\"\n"
    "rxConfirm=\"" << rxConfirm << "\"\n"
    "rxClose=\"" << rxClose << "\"\n"
    "dropped=\"" << dropped << "\"\n"
    "brokenMgt=\"" << brokenMgt << "\"\n"
    "txMgt=\"" << txMgt << "\"\n"
    "txMgtBytes=\"" << (double)txMgtBytes /1024.0 << "\"\n"
    "rxMgt=\"" << rxMgt << "\"\n"
    "rxMgtBytes=\"" << (double)rxMgtBytes / 1024.0 << "K\"\n"
    "beaconShift=\"" << beaconShift << "\"/>\n";
}
void
PeerManagementProtocolMac::Report (std::ostream & os) const
{
  os << "<PeerManagerPlugin "
    "address=\"" << m_parent->GetAddress () << "\">\n";
  m_stats.Print (os);
  os << "</PeerManagerPlugin>\n";
}
void
PeerManagementProtocolMac::ResetStats ()
{
  m_stats = Statistics::Statistics ();
}
uint32_t
PeerManagementProtocolMac::GetLinkMetric (Mac48Address peerAddress)
{
  return m_parent->GetLinkMetric (peerAddress);
}
} // namespace dot11s
} //namespace ns3

