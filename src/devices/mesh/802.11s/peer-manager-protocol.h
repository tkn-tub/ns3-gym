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

#include "peer-manager-plugin.h"

#include <list>
namespace ns3 {
class Dot11sPeerManagerMacPlugin;
class PeerLink;
/**
 * \ingroup dot11s
 */
class Dot11sPeerManagerProtocol : public Object
{
public:
  Dot11sPeerManagerProtocol ();
  ~Dot11sPeerManagerProtocol ();
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
      bool dropeed,
      Mac48Address peerAddress,
      uint16_t aid,
      IeDot11sPeerManagement peerManagementElement,
      IeDot11sConfiguration meshConfig
      );
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
  typedef std::map<uint32_t, std::vector<Ptr<PeerLink> > >  PeerDescriptorsMap;
  typedef std::map<Mac48Address, BeaconInfo>  BeaconInterfaceInfoMap;
  typedef std::map<uint32_t, BeaconInterfaceInfoMap> BeaconInfoMap;
  typedef std::map<uint32_t, Ptr<Dot11sPeerManagerMacPlugin> > PeerManagerPluginMap;

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
#if 0
  //Maximum peers that may be opened:
  uint8_t  m_maxNumberOfPeerLinks;
  /**
   * Peer manager identify interface by address
   * of MAC. So, for every interface we store
   * list of peer descriptors.
   */
  PeerDescriptorsMap m_peerDescriptors;
  /**
   * List of MAC pointers - to iteract with each
   * mac
   */
  MeshMacMap m_macPointers;
  uint8_t  m_numberOfActivePeers; //number of established peer links
  uint16_t m_assocId;  //last stored assoc ID
  uint16_t m_localLinkId;  //last stored local link ID
  //This Variables used in beacon miss auto-cleanup:
  //How many beacons may we lose before the link is
  //considered to be broken:
  uint8_t  m_maxBeaconLoss;
  //Periodically we scan the peer manager list of peers
  //and check if the too many  beacons were lost:
  Time  m_peerLinkCleanupPeriod;
  EventId  m_cleanupEvent;
  Ptr<WifiPeerLinkDescriptor> AddDescriptor (
    Mac48Address interfaceAddress,
    Mac48Address peerAddress,
    Time lastBeacon,
    Time beaconInterval
  );
  void  PeerCleanup ();
  //Mechanism of choosing PEERs:
  bool  ShouldSendOpen (Mac48Address interfaceAddress, Mac48Address peerAddress);
  bool  ShouldAcceptOpen (
    Mac48Address interfaceAddress,
    Mac48Address peerAddress,
    dot11sReasonCode & reasonCode
  );
  //Needed for Beacon Collision Avoidance module:
  BeaconInfoMap m_myBeaconInfo;
  /**
   * Peer link Open/Close callbacks: We need to
   * inform MAC about this events.
   * \brief Interaction with peer link
   * descriptor - notify that peer link was
   * opened or closed
   * \param status true - peer link opened, peer
   * link closed otherwise
   */
  void PeerLinkStatus (Mac48Address interfaceAddress, Mac48Address peerAddress, bool status);
#endif
};
} //namespace ns3
#endif
