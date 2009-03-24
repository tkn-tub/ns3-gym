/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 *          Aleksey Kovalenko <kovalenko@iitp.ru>
 */


#ifndef DOT11S_PEER_MAN_H
#define DOT11S_PEER_MAN_H

#include "ns3/mac48-address.h"
#include "ns3/wifi-net-device.h"
#include "ns3/ie-dot11s-peer-management.h"
#include "ns3/ie-dot11s-beacon-timing.h"
#include "ns3/ie-dot11s-configuration.h"
#include "ns3/event-id.h"

#include <list>
#include <map>
namespace ns3 {
namespace dot11s {
class PeerManagerMacPlugin;
class PeerLink;
/**
 * \ingroup dot11s
 */
class PeerManagerProtocol : public Object
{
public:
  PeerManagerProtocol ();
  ~PeerManagerProtocol ();
  static TypeId GetTypeId ();
  bool AttachPorts(std::vector<Ptr<WifiNetDevice> >);
  /** \brief Methods that handle beacon sending/receiving procedure.
   * This methods interact with MAC_layer plug-in
   * \{
   */
  /**
   * \brief When we are sending a beacon - we add a timing element to 
   * it and remember the time, when we sent a beacon (for BCA)
   * \param IeDot11sBeaconTiming is a beacon timing element that
   * should be present in beacon
   * \param interface is a interface sending a beacon
   * \param currentTbtt is a time of beacon sending
   * \param beaconInterval is a beacon interval on this interface
   */
  Ptr<IeDot11sBeaconTiming> SendBeacon(uint32_t interface, Time currentTbtt, Time beaconInterval);
  /**
   * \brief When we receive a beacon from peer-station, we remember
   * its beacon timing element (needed for peer choosing mechanism),
   * and remember beacon timers - last beacon and beacon interval to
   * detect beacon loss and cancel links
   * \param interface is a interface on which beacon was received
   * \param timingElement is a timing element of remote beacon
   */
  void ReceiveBeacon(
      uint32_t interface,
      bool meshBeacon,
      IeDot11sBeaconTiming timingElement,
      Mac48Address peerAddress,
      Time receivingTime,
      Time beaconInterval
      );
  /**
   * \}
   */
  /**
   * \brief Methods that handle Peer link management frames
   * interaction:
   * \{
   */
  /**
   * Deliver Peer link management information to the protocol-part
   * \param void is returning value - we pass a frame and forget
   * about it
   * \param uint32_t - is a interface ID of a given MAC (interfaceID rather
   * than MAC address, beacause many interfaces may have the same MAC)
   * \param Mac48Address is address of peer
   * \param uint16_t is association ID, which peer has assigned to
   * us
   * \param IeDot11sConfiguration is mesh configuration element
   * taken from the peer management frame
   * \param IeDot11sPeerManagement is peer link management element
   */
  void ReceivePeerLinkFrame(
      uint32_t interface,
      Mac48Address peerAddress,
      uint16_t aid,
      IeDot11sPeerManagement peerManagementElement,
      IeDot11sConfiguration meshConfig
      );
  /**
   * Cancell peer link due to broken configuration (SSID or Supported
   * rates)
   */
  void ConfigurationMismatch (uint32_t interface, Mac48Address peerAddress);
  /**
   * Checks if there is established link
   */
  bool IsActiveLink (uint32_t interface, Mac48Address peerAddress);
  /**
   * \}
   */
private:
  /**
   * All private structures:
   * * peer link descriptors;
   * * information about received beacons
   * * pointers to proper plugins
   * \{
   */
  struct BeaconInfo
  {
    uint16_t aid; //Assoc ID
    Time referenceTbtt; //When one of my station's beacons was put into a beacon queue;
    Time beaconInterval; //Beacon interval of my station;
  };
  typedef std::vector<Ptr<PeerLink> > PeerLinksOnInterface;
  typedef std::map<uint32_t, PeerLinksOnInterface>  PeerLinksMap;
  typedef std::map<Mac48Address, BeaconInfo>  BeaconsOnInterface;
  typedef std::map<uint32_t, BeaconsOnInterface> BeaconInfoMap;
  typedef std::map<uint32_t, Ptr<PeerManagerMacPlugin> > PeerManagerPluginMap;
private:
  /**
   * Return a position in beacon-storage for a given remote station
   */
  void FillBeaconInfo(uint32_t interface, Mac48Address peerAddress, Time receivingTime, Time beaconInterval);
  Ptr<PeerLink> InitiateLink (uint32_t interface, Mac48Address peerAddress, Time lastBeacon, Time beaconInterval);
  /**
   * External peer-chooser
   * \{
   */
  bool ShouldSendOpen (uint32_t interface, Mac48Address peerAddress);
  bool ShouldAcceptOpen (uint32_t interface, Mac48Address peerAddress, dot11sReasonCode & reasonCode);
  /**
   * \}
   * \brief Indicates changes in peer links
   */
  void PeerLinkStatus (uint32_t interface, Mac48Address peerAddress, bool status);
  /**
   * Removes all links which are idle
   */
  void  PeerCleanup ();
private:
  PeerManagerPluginMap m_plugins;
  /**
   * Information related to beacons:
   * \{
   */
  BeaconInfoMap m_neighbourBeacons;
  static const uint8_t m_maxBeaconLoss = 3;
  /**
   * \}
   * \}
   */
  uint16_t m_lastAssocId;
  uint16_t m_lastLocalLinkId;
  uint8_t m_numberOfActivePeers; //number of established peer links
  uint8_t m_maxNumberOfPeerLinks;
  /**
   * Peer Links
   * \{
   */
  PeerLinksMap m_peerLinks;
  /**
   * \}
   */
  /**
   * Periodically we scan the peer manager list of peers
   * and check if the too many  beacons were lost:
   * \{
   */
  Time  m_peerLinkCleanupPeriod;
  EventId  m_cleanupEvent;
  /**
   * \}
   */
};
  
} // namespace dot11s
} //namespace ns3
#endif
