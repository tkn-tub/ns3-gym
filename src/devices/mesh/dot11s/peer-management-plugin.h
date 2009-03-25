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

#ifndef PEER_MANAGER_MAC_PLUGIN_H_
#define PEER_MANAGER_MAC_PLUGIN_H_

#include "ns3/mesh-wifi-interface-mac-plugin.h"
#include "peer-management-protocol.h"

namespace ns3 {
class MeshWifiInterfaceMac;
namespace dot11s {
class IeConfiguration;
class IePeerManagement;
class PeerManagerProtocol;
/**
 * \ingroup dot11s
 * 
 * \brief This is plugin to Mesh WiFi MAC, which implements
 * interface to dot11s peer management protocol: it takes proper
 * frames from MAC-layer, extracts peer link management information
 * element and mesh configuration element and passes it to main part
 * of protocol
 */
class PeerManagerMacPlugin : public MeshWifiInterfaceMacPlugin
{
public:
  PeerManagerMacPlugin (uint32_t interface, Ptr<PeerManagerProtocol> protocol);
  ~PeerManagerMacPlugin ();
  ///\name Inherited from plugin abstract class
  ///\{
  void SetParent (Ptr<MeshWifiInterfaceMac> parent);
  bool Receive (Ptr<Packet> packet, const WifiMacHeader & header);
  bool UpdateOutcomingFrame (Ptr<Packet> packet, WifiMacHeader & header, Mac48Address from, Mac48Address to) const;
  void UpdateBeacon (MeshWifiBeacon & beacon) const;
  ///\}
private:
  friend class PeerManagerProtocol;
  friend class PeerLink; 
  ///\name BCA functionallity:
  ///\{
  ///\brief Fills TBTT and beacon interval. Needed by BCA
  ///functionallity
  ///\param first in retval is TBTT
  ///\param second in retval is beacon interval
  std::pair<Time, Time> GetBeaconInfo() const;
  void SetBeaconShift(Time shift);
  ///\}
  void SetPeerManagerProtcol(Ptr<PeerManagerProtocol> protocol);
  void SendPeerLinkManagementFrame(
      Mac48Address peerAddress,
      uint16_t aid,
      IePeerManagement peerElement,
      IeConfiguration meshConfig
      );
  ///\brief DUBUG only - to print established links
  Mac48Address GetAddress () const;
private:
  ///\name Information about MAC and protocol:
  ///\{
  Ptr<MeshWifiInterfaceMac> m_parent;
  uint32_t m_ifIndex;
  Ptr<PeerManagerProtocol> m_protocol;
   ///\}
   ///\name Create peer link management frames:
   ///\{
  Ptr<Packet> CreatePeerLinkOpenFrame();
  Ptr<Packet> CreatePeerLinkConfirmFrame();
  Ptr<Packet> CreatePeerLinkCloseFrame();
  ///This structure keeps all fields in peer link management frame,
  ///which are not subclasses of WifiInformationElement
  struct PlinkFrameStart {
    uint8_t subtype;
    uint16_t aid;
    SupportedRates rates;
    uint16_t qos;
  };
  /// \name Parses the start of the frame, where there are no
  /// WifiInformationElements exist
  PlinkFrameStart ParsePlinkFrame(Ptr<const Packet> packet);
  ///\}
};
  
} // namespace dot11s
} //namespace ns3
#endif

