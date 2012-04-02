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

#ifndef PEER_MANAGEMENT_PROTOCOL_MAC_H
#define PEER_MANAGEMENT_PROTOCOL_MAC_H

#include "ns3/mesh-wifi-interface-mac-plugin.h"

namespace ns3 {
class MeshWifiInterfaceMac;
namespace dot11s {
class PeerManagementProtocol;
class IeConfiguration;
class IePeerManagement;
class PeerManagementProtocol;
/**
 * \ingroup dot11s
 *
 * \brief This is plugin to Mesh WiFi MAC, which implements
 * interface to dot11s peer management protocol: it takes proper
 * frames from MAC-layer, extracts peer link management information
 * element and mesh configuration element and passes it to main part
 * of protocol
 */
class PeerManagementProtocolMac : public MeshWifiInterfaceMacPlugin
{
public:
  PeerManagementProtocolMac (uint32_t interface, Ptr<PeerManagementProtocol> protocol);
  ~PeerManagementProtocolMac ();
  ///\name Inherited from plugin abstract class
  // \{
  void SetParent (Ptr<MeshWifiInterfaceMac> parent);
  bool Receive (Ptr<Packet> packet, const WifiMacHeader & header);
  bool UpdateOutcomingFrame (Ptr<Packet> packet, WifiMacHeader & header, Mac48Address from, Mac48Address to);
  void UpdateBeacon (MeshWifiBeacon & beacon) const;
  // \}
  ///\name Statistics
  // \{
  void Report (std::ostream &) const;
  void ResetStats ();
  uint32_t GetLinkMetric (Mac48Address peerAddress);
  // \}
private:
  PeerManagementProtocolMac& operator= (const PeerManagementProtocolMac &);
  PeerManagementProtocolMac (const PeerManagementProtocolMac &);

  friend class PeerManagementProtocol;
  friend class PeerLink;
  ///\name Create peer link management frames
  // \{
  /**
   * \brief This structure keeps all fields in peer link management frame,
   * which are not subclasses of WifiInformationElement
   */
  struct PlinkFrameStart
  {
    uint8_t subtype;
    uint16_t aid;
    SupportedRates rates;
    uint16_t qos;
  };
  Ptr<Packet> CreatePeerLinkOpenFrame ();
  Ptr<Packet> CreatePeerLinkConfirmFrame ();
  Ptr<Packet> CreatePeerLinkCloseFrame ();
  /// Parses the start of the frame, where no WifiInformationElements exist
  PlinkFrameStart ParsePlinkFrame (Ptr<const Packet> packet);
  // \}
  ///  Closes link when a proper number of successive transmissions have failed
  void TxError (WifiMacHeader const &hdr);
  void TxOk (WifiMacHeader const &hdr);
  /// BCA functionality
  void SetBeaconShift (Time shift);
  void SetPeerManagerProtcol (Ptr<PeerManagementProtocol> protocol);
  void SendPeerLinkManagementFrame (
    Mac48Address peerAddress,
    Mac48Address peerMpAddress,
    uint16_t aid,
    IePeerManagement peerElement,
    IeConfiguration meshConfig
    );
  ///\brief debug only, used to print established links
  Mac48Address GetAddress () const;
  ///\name Statistics
  // \{
  struct Statistics
  {
    uint16_t txOpen;
    uint16_t txConfirm;
    uint16_t txClose;
    uint16_t rxOpen;
    uint16_t rxConfirm;
    uint16_t rxClose;
    uint16_t dropped;
    uint16_t brokenMgt;
    uint16_t txMgt;
    uint32_t txMgtBytes;
    uint16_t rxMgt;
    uint32_t rxMgtBytes;
    uint16_t beaconShift;

    Statistics ();
    void Print (std::ostream & os) const;
  };
private:
  struct Statistics m_stats;
  ///\}
  ///\name Information about MAC and protocol:
  // \{
  Ptr<MeshWifiInterfaceMac> m_parent;
  uint32_t m_ifIndex;
  Ptr<PeerManagementProtocol> m_protocol;
  // \}
};

} // namespace dot11s
} // namespace ns3

#endif /* PEER_MANAGEMENT_PROTOCOL_MAC_H */

