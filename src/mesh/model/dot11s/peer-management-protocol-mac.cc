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
#include "ns3/mesh-information-element-vector.h"
#include "ns3/log.h"
namespace ns3 {
namespace dot11s {
PeerManagementProtocolMac::PeerManagementProtocolMac (uint32_t interface,
                                                      Ptr<PeerManagementProtocol> protocol)
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
  m_parent->TraceConnectWithoutContext ("TxErrHeader", MakeCallback (&PeerManagementProtocolMac::TxError, this));
  m_parent->TraceConnectWithoutContext ("TxOkHeader",  MakeCallback (&PeerManagementProtocolMac::TxOk,    this));
}
void
PeerManagementProtocolMac::TxError (WifiMacHeader const &hdr)
{
  m_protocol->TransmissionFailure (m_ifIndex, hdr.GetAddr1 ());
}
void
PeerManagementProtocolMac::TxOk (WifiMacHeader const &hdr)
{
  m_protocol->TransmissionSuccess (m_ifIndex, hdr.GetAddr1 ());
}
bool
PeerManagementProtocolMac::Receive (Ptr<Packet> const_packet, const WifiMacHeader & header)
{
  // First of all we copy a packet, because we need to remove some
  //headers
  Ptr<Packet> packet = const_packet->Copy ();
  if (header.IsBeacon ())
    {
      MgtBeaconHeader beacon_hdr;
      packet->RemoveHeader (beacon_hdr);
      MeshInformationElementVector elements;
      packet->RemoveHeader (elements);
      Ptr<IeBeaconTiming> beaconTiming = DynamicCast<IeBeaconTiming> (elements.FindFirst (IE11S_BEACON_TIMING));
      Ptr<IeMeshId> meshId = DynamicCast<IeMeshId> (elements.FindFirst (IE11S_MESH_ID));

      if ((meshId != 0) && (m_protocol->GetMeshId ()->IsEqual (*meshId)))
        {
          m_protocol->ReceiveBeacon (m_ifIndex, header.GetAddr2 (), MicroSeconds (
                                       beacon_hdr.GetBeaconIntervalUs ()), beaconTiming);
        }
      // Beacon shall not be dropped. May be needed to another plugins
      return true;
    }
  if (header.IsAction ())
    {
      WifiActionHeader actionHdr;
      packet->RemoveHeader (actionHdr);
      WifiActionHeader::ActionValue actionValue = actionHdr.GetAction ();
      // If can not handle - just return;
      if (actionHdr.GetCategory () != WifiActionHeader::MESH_PEERING_MGT)
        {
          return m_protocol->IsActiveLink (m_ifIndex, header.GetAddr2 ());
        }
      m_stats.rxMgt++;
      m_stats.rxMgtBytes += packet->GetSize ();
      Mac48Address peerAddress = header.GetAddr2 ();
      Mac48Address peerMpAddress = header.GetAddr3 ();
      PeerLinkFrameStart::PlinkFrameStartFields fields;
      {
        PeerLinkFrameStart peerFrame;
        peerFrame.SetPlinkFrameSubtype ((uint8_t) actionValue.peerLink);
        packet->RemoveHeader (peerFrame);
        fields = peerFrame.GetFields ();
        NS_ASSERT (fields.subtype == actionValue.peerLink);
      }
      if ((actionValue.peerLink != WifiActionHeader::PEER_LINK_CLOSE) && !(m_parent->CheckSupportedRates (
                                                                             fields.rates)))
        {
          m_protocol->ConfigurationMismatch (m_ifIndex, peerAddress);
          // Broken peer link frame - drop it
          m_stats.brokenMgt++;
          return false;
        }
      if ((actionValue.peerLink != WifiActionHeader::PEER_LINK_CONFIRM) && !fields.meshId.IsEqual (
            *(m_protocol->GetMeshId ())))
        {
          m_protocol->ConfigurationMismatch (m_ifIndex, peerAddress);
          // Broken peer link frame - drop it
          m_stats.brokenMgt++;
          return false;
        }
      Ptr<IePeerManagement> peerElement;
      //Peer Management element is the last element in this frame - so, we can use MeshInformationElementVector
      MeshInformationElementVector elements;
      packet->RemoveHeader (elements);
      peerElement = DynamicCast<IePeerManagement>(elements.FindFirst (IE11S_PEERING_MANAGEMENT));
      NS_ASSERT (peerElement != 0);
      //Check taht frame subtype corresponds peer link subtype
      if (peerElement->SubtypeIsOpen ())
        {
          m_stats.rxOpen++;
          NS_ASSERT (actionValue.peerLink == WifiActionHeader::PEER_LINK_OPEN);
        }
      if (peerElement->SubtypeIsConfirm ())
        {
          m_stats.rxConfirm++;
          NS_ASSERT (actionValue.peerLink == WifiActionHeader::PEER_LINK_CONFIRM);
        }
      if (peerElement->SubtypeIsClose ())
        {
          m_stats.rxClose++;
          NS_ASSERT (actionValue.peerLink == WifiActionHeader::PEER_LINK_CLOSE);
        }
      //Deliver Peer link management frame to protocol:
      m_protocol->ReceivePeerLinkFrame (m_ifIndex, peerAddress, peerMpAddress, fields.aid, *peerElement,
                                        fields.config);
      // if we can handle a frame - drop it
      return false;
    }
  return m_protocol->IsActiveLink (m_ifIndex, header.GetAddr2 ());
}
bool
PeerManagementProtocolMac::UpdateOutcomingFrame (Ptr<Packet> packet, WifiMacHeader & header,
                                                 Mac48Address from, Mac48Address to)
{
  if (header.IsAction ())
    {
      WifiActionHeader actionHdr;
      packet->PeekHeader (actionHdr);
      if (actionHdr.GetCategory () == WifiActionHeader::MESH_PEERING_MGT)
        {
          return true;
        }
    }
  if (header.GetAddr1 ().IsGroup ())
    {
      return true;
    }
  else
    {
      if (m_protocol->IsActiveLink (m_ifIndex, header.GetAddr1 ()))
        {
          return true;
        }
      else
        {
          m_stats.dropped++;
          return false;
        }
    }
}
void
PeerManagementProtocolMac::UpdateBeacon (MeshWifiBeacon & beacon) const
{
  if (m_protocol->GetBeaconCollisionAvoidance ())
    {
      Ptr<IeBeaconTiming> beaconTiming = m_protocol->GetBeaconTimingElement (m_ifIndex);
      beacon.AddInformationElement (beaconTiming);
    }
  beacon.AddInformationElement (m_protocol->GetMeshId ());
  m_protocol->NotifyBeaconSent (m_ifIndex, beacon.GetBeaconInterval ());
}

void
PeerManagementProtocolMac::SendPeerLinkManagementFrame (Mac48Address peerAddress, Mac48Address peerMpAddress,
                                                        uint16_t aid, IePeerManagement peerElement, IeConfiguration meshConfig)
{
  //Create a packet:
  meshConfig.SetNeighborCount (m_protocol->GetNumberOfLinks ());
  Ptr<Packet> packet = Create<Packet> ();
  MeshInformationElementVector elements;
  elements.AddInformationElement (Ptr<IePeerManagement> (&peerElement));
  packet->AddHeader (elements);
  PeerLinkFrameStart::PlinkFrameStartFields fields;
  fields.rates = m_parent->GetSupportedRates ();
  fields.capability = 0;
  fields.meshId = *(m_protocol->GetMeshId ());
  fields.config = meshConfig;
  PeerLinkFrameStart plinkFrame;
  //Create an 802.11 frame header:
  //Send management frame to MAC:
  WifiActionHeader actionHdr;
  if (peerElement.SubtypeIsOpen ())
    {
      m_stats.txOpen++;
      WifiActionHeader::ActionValue action;
      action.peerLink = WifiActionHeader::PEER_LINK_OPEN;
      fields.subtype = WifiActionHeader::PEER_LINK_OPEN;
      actionHdr.SetAction (WifiActionHeader::MESH_PEERING_MGT, action);
    }
  if (peerElement.SubtypeIsConfirm ())
    {
      m_stats.txConfirm++;
      WifiActionHeader::ActionValue action;
      action.peerLink = WifiActionHeader::PEER_LINK_CONFIRM;
      fields.aid = aid;
      fields.subtype = WifiActionHeader::PEER_LINK_CONFIRM;
      actionHdr.SetAction (WifiActionHeader::MESH_PEERING_MGT, action);
    }
  if (peerElement.SubtypeIsClose ())
    {
      m_stats.txClose++;
      WifiActionHeader::ActionValue action;
      action.peerLink = WifiActionHeader::PEER_LINK_CLOSE;
      fields.subtype = WifiActionHeader::PEER_LINK_CLOSE;
      fields.reasonCode = peerElement.GetReasonCode ();
      actionHdr.SetAction (WifiActionHeader::MESH_PEERING_MGT, action);
    }
  plinkFrame.SetPlinkFrameStart (fields);
  packet->AddHeader (plinkFrame);
  packet->AddHeader (actionHdr);
  m_stats.txMgt++;
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
  m_parent->SendManagementFrame (packet, hdr);
}

Mac48Address
PeerManagementProtocolMac::GetAddress () const
{
  if (m_parent != 0)
    {
      return m_parent->GetAddress ();
    }
  else
    {
      return Mac48Address ();
    }
}
void
PeerManagementProtocolMac::SetBeaconShift (Time shift)
{
  if (shift != Seconds (0))
    {
      m_stats.beaconShift++;
    }
  m_parent->ShiftTbtt (shift);
}
PeerManagementProtocolMac::Statistics::Statistics () :
  txOpen (0), txConfirm (0), txClose (0), rxOpen (0), rxConfirm (0), rxClose (0), dropped (0), brokenMgt (0),
  txMgt (0), txMgtBytes (0), rxMgt (0), rxMgtBytes (0), beaconShift (0)
{
}
void
PeerManagementProtocolMac::Statistics::Print (std::ostream & os) const
{
  os << "<Statistics "
  "txOpen=\"" << txOpen << "\"" << std::endl <<
  "txConfirm=\"" << txConfirm << "\"" << std::endl <<
  "txClose=\"" << txClose << "\"" << std::endl <<
  "rxOpen=\"" << rxOpen << "\"" << std::endl <<
  "rxConfirm=\"" << rxConfirm << "\"" << std::endl <<
  "rxClose=\"" << rxClose << "\"" << std::endl <<
  "dropped=\"" << dropped << "\"" << std::endl <<
  "brokenMgt=\"" << brokenMgt << "\"" << std::endl <<
  "txMgt=\"" << txMgt << "\"" << std::endl <<
  "txMgtBytes=\"" << txMgtBytes << "\"" << std::endl <<
  "rxMgt=\"" << rxMgt << "\"" << std::endl <<
  "rxMgtBytes=\"" << rxMgtBytes << "\"" << std::endl <<
  "beaconShift=\"" << beaconShift << "\"/>" << std::endl;
}
void
PeerManagementProtocolMac::Report (std::ostream & os) const
{
  os << "<PeerManagementProtocolMac "
  "address=\"" << m_parent->GetAddress () << "\">" << std::endl;
  m_stats.Print (os);
  os << "</PeerManagementProtocolMac>" << std::endl;
}
void
PeerManagementProtocolMac::ResetStats ()
{
  m_stats = Statistics ();
}
uint32_t
PeerManagementProtocolMac::GetLinkMetric (Mac48Address peerAddress)
{
  return m_parent->GetLinkMetric (peerAddress);
}
} // namespace dot11s
} // namespace ns3

