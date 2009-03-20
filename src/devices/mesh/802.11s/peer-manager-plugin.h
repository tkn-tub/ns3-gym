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
#include "peer-manager-protocol.h"

namespace ns3 {
class MeshWifiInterfaceMac;
class IeDot11sConfiguration;
class IeDot11sPeerManagement;
class Dot11sPeerManagerProtocol;
/**
 * \ingroup dot11s
 * 
 * \brief This is plugin to Mesh WiFi MAC, which implements
 * interface to dot11s peer management protocol: it takes proper
 * frames from MAC-layer, extracts peer link management information
 * element and mesh configuration element and passes it to main part
 * of protocol
 */
class Dot11sPeerManagerMacPlugin : public MeshWifiInterfaceMacPlugin
{
public:
  Dot11sPeerManagerMacPlugin ();
  ~Dot11sPeerManagerMacPlugin ();
  /**
   * \brief Inherited from plugin abstract class
   * \{
   */
  void SetParent (Ptr<MeshWifiInterfaceMac> parent);
  bool Receive (Ptr<Packet> packet, const WifiMacHeader & header);
  bool UpdateOutcomingFrame (Ptr<Packet> packet, WifiMacHeader & header, Mac48Address from, Mac48Address to) const;
  void UpdateBeacon (MeshWifiBeacon & beacon) const;
  /**
   * \}
   */
  bool BindWithProtocol(Ptr<Dot11sPeerManagerProtocol>);
  void SetPeerManagerProtcol(Ptr<Dot11sPeerManagerProtocol> protocol);
  /**
   * Deliver Peer link management information to the protocol-part
   * \param void is returning value - we pass a frame and forget
   * about it
   * \param uint32_t - is a port ID of a given MAC (portID rather
   * than MAC address, beacause many ports may have the same MAC)
   * \param Mac48Address is address of peer
   * \param uint16_t is association ID, which peer has assigned to
   * us
   * \param IeDot11sConfiguration is mesh configuration element
   * taken from the peer management frame
   * \param IeDot11sPeerManagement is peer link management element
   */
  void SetDeliverPeerLinkFrameCallbback (
      Callback<void, uint32_t, Mac48Address, uint16_t, IeDot11sConfiguration, IeDot11sPeerManagement>
      );
  /**
   * \brief Forms and sends peer link management frame.
   */
  void SendPeerLinkManagementFrame(Mac48Address peerAddress, uint16_t aid, IeDot11sPeerManagement peerElement, IeDot11sConfiguration meshConfig);
  IeDot11sConfiguration AskPeerLinkManagementElement();
private:
  Callback<void, uint32_t, Mac48Address, uint16_t, IeDot11sConfiguration, IeDot11sPeerManagement> m_deliverPeerManFrame;
  Callback<void> m_beaconCallback;
  Ptr<MeshWifiInterfaceMac> m_parent;
  Ptr<Dot11sPeerManagerProtocol> m_protocol;
  /**
   * Create peer link management frames:
   * \{
   */
  Ptr<Packet> CreatePeerLinkOpenFrame();
  Ptr<Packet> CreatePeerLinkConfirmFrame();
  Ptr<Packet> CreatePeerLinkCloseFrame();
  /**
   * \}
   */
};
} //namespace ns3
#endif

