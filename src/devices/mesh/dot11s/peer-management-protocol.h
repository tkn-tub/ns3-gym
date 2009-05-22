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
#include "ns3/net-device.h"
#include "ns3/event-id.h"
#include "ns3/nstime.h"
#include "ie-dot11s-beacon-timing.h"
#include "ie-dot11s-peer-management.h"
#include "ie-dot11s-id.h"

#include <map>
namespace ns3 {
class MeshPointDevice;
namespace dot11s {
class PeerManagerMacPlugin;
class PeerLink;
class IePeerManagement;
class IeConfiguration;
/**
 * \ingroup dot11s
 * 
 * \brief 802.11s Peer Management Protocol model 
 */
class PeerManagementProtocol : public Object
{
public:
  PeerManagementProtocol ();
  ~PeerManagementProtocol ();
  static TypeId GetTypeId ();
  void DoDispose ();
  /** 
   * \brief Install PMP on given mesh point. 
   * 
   * Installing protocol cause installing its interface MAC plugins.
   *  
   * Also MP aggregates all installed protocols, PMP protocol can be accessed 
   * via MeshPointDevice::GetObject<PeerManagementProtocol>();
   */
  bool Install(Ptr<MeshPointDevice>);
  /** 
   * \brief Methods that handle beacon sending/receiving procedure.
   * 
   * This methods interact with MAC_layer plug-in
   * \{
   */
  /**
   * \brief When we are sending a beacon - we fill beacon timing
   * element
   * \param IeBeaconTiming is a beacon timing element that
   * should be present in beacon
   * \param interface is a interface sending a beacon
   */
  Ptr<IeBeaconTiming> GetBeaconTimingElement(uint32_t interface);
  /**
   * \brief When we receive a beacon from peer-station, we remember
   * its beacon timing element (needed for peer choosing mechanism),
   * and remember beacon timers - last beacon and beacon interval to
   * detect beacon loss and cancel links
   * \param interface is a interface on which beacon was received
   * \param timingElement is a timing element of remote beacon
   */
  void UpdatePeerBeaconTiming(
      uint32_t interface,
      bool meshBeacon,
      IeBeaconTiming timingElement,
      Mac48Address peerAddress,
      Time receivingTime,
      Time beaconInterval
      );
  //\}
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
   * \param Mac48Address is address of peer mesh point device (equal
   * to peer address when only one interface)
   * \param uint16_t is association ID, which peer has assigned to
   * us
   * \param IeConfiguration is mesh configuration element
   * taken from the peer management frame
   * \param IePeerManagement is peer link management element
   */
  void ReceivePeerLinkFrame(
      uint32_t interface,
      Mac48Address peerAddress,
      Mac48Address peerMeshPointAddress,
      uint16_t aid,
      IePeerManagement peerManagementElement,
      IeConfiguration meshConfig
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
  //\}
  ///\brief Needed by external module to do MLME
  Ptr<PeerLink> FindPeerLink(uint32_t interface, Mac48Address peerAddress);
  void SetPeerLinkStatusCallback (Callback<void, Mac48Address, Mac48Address, uint32_t, bool> cb);
  std::vector<Mac48Address> GetActiveLinks(uint32_t interface);
  ///\brief needed by plugins to set global source address
  Mac48Address GetAddress ();
  ///\Needed to fill mesh configuration
  uint8_t GetNumberOfLinks ();
  void SetMeshId (char const meshId[32], uint8_t length);
  Ptr<IeMeshId> GetMeshId() const;
  ///\brief: Report statistics
  void Report (std::ostream &) const;
  void ResetStats ();
private:
  /** \name Private structures
   * \{
   */
  /// Keeps information about beacon of peer station: beacon interval, association ID, last time we have received a beacon
  struct BeaconInfo
  {
    uint16_t aid; //Assoc ID
    Time referenceTbtt; //When one of my station's beacons was put into a beacon queue;
    Time beaconInterval; //Beacon interval of my station;
  };
  /// We keep a vector of pointers to PeerLink class. This vector
  /// keeps all peer links at a given interface.
  typedef std::vector<Ptr<PeerLink> > PeerLinksOnInterface;
  /// This map keeps all peer links.
  ///\param uint32_t is interface ID
  typedef std::map<uint32_t, PeerLinksOnInterface>  PeerLinksMap;
  ///\brief This map keeps relationship between peer address and its
  /// beacon information
  typedef std::map<Mac48Address, BeaconInfo>  BeaconsOnInterface;
  ///\brief This map keeps beacon information on all intefaces
  typedef std::map<uint32_t, BeaconsOnInterface> BeaconInfoMap;
  ///\brief this vector keeps pointers to MAC-plugins
  typedef std::map<uint32_t, Ptr<PeerManagerMacPlugin> > PeerManagerPluginMap;
  ///\}
private:
  /**
   * Return a position in beacon-storage for a given remote station
   */
  void FillBeaconInfo(uint32_t interface, Mac48Address peerAddress, Time receivingTime, Time beaconInterval);
  Ptr<PeerLink> InitiateLink (
      uint32_t interface,
      Mac48Address peerAddress,
      Mac48Address peerMeshPointAddress,
      Time lastBeacon,
      Time beaconInterval
      );
  /**
   * \name External peer-chooser
   * \{
   */
  bool ShouldSendOpen (uint32_t interface, Mac48Address peerAddress);
  bool ShouldAcceptOpen (uint32_t interface, Mac48Address peerAddress, PmpReasonCode & reasonCode);
  /**
   * \}
   * \brief Indicates changes in peer links
   */
  void PeerLinkStatus (uint32_t interface, Mac48Address peerAddress, Mac48Address peerMeshPointAddres, bool status);
  ///\brief BCA
  Time GetNextBeaconShift (uint32_t interface);
private:
  PeerManagerPluginMap m_plugins;
  Mac48Address m_address;
  Ptr<IeMeshId> m_meshId;
  /**
   * \name Information related to beacons:
   * \{
   */
  BeaconInfoMap m_neighbourBeacons;
  ///\}
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
  ///\brief Callback to notify about peer link changes:
  ///\param Mac48Address is peer address of mesh point
  ///\param Mac48Address is peer address of interface
  ///\param uint32_t - interface ID
  ///\param bool is staus - true when new link has appeared, false -
  //when link was closed
  Callback <void, Mac48Address, Mac48Address, uint32_t, bool> m_peerStatusCallback;
  ///\}
  //Keeps statistics
  struct Statistics {
    uint16_t linksOpened;
    uint16_t linksClosed;

    Statistics () : linksOpened (0), linksClosed (0) {};
    void Print (std::ostream & os) const;
  };
  struct Statistics m_stats;

};
  
} // namespace dot11s
} //namespace ns3
#endif
