/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include "ns3/traced-value.h"
#include "ie-dot11s-beacon-timing.h"
#include "ie-dot11s-peer-management.h"
#include "peer-link.h"

#include <map>
namespace ns3 {
class MeshPointDevice;
class UniformRandomVariable;
namespace dot11s {
class PeerManagementProtocolMac;
class PeerLink;
class IeMeshId;
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
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();
  void DoDispose ();
  /**
   * \brief Install PMP on given mesh point.
   * \return true if successful
   *
   * Installing protocol causes installation of its interface MAC plugins.
   *
   * Also MP aggregates all installed protocols, PMP protocol can be accessed
   * via MeshPointDevice::GetObject<PeerManagementProtocol>();
   */
  bool Install (Ptr<MeshPointDevice>);
  /**
   * \brief Methods that handle beacon sending/receiving procedure.
   *
   * \name This methods interact with MAC_layer plug-in
   * \{
   */
  /**
   * \brief When we are sending a beacon - we fill beacon timing
   * element
   * \return IeBeaconTiming is a beacon timing element that should be present in beacon
   * \param interface is a interface sending a beacon
   */
  Ptr<IeBeaconTiming> GetBeaconTimingElement (uint32_t interface);
  /**
   * \brief To initiate peer link we must notify about received beacon
   * \param interface the interface where a beacon was received from
   * \param peerAddress address of station, which sent a beacon
   * \param beaconInterval beacon interval (needed by beacon loss counter)
   * \param beaconTiming beacon timing element (needed by BCA)
   */
  void ReceiveBeacon (uint32_t interface, Mac48Address peerAddress, Time beaconInterval, Ptr<IeBeaconTiming> beaconTiming);
  // \}
  /**
   * \brief Methods that handle Peer link management frames
   * interaction:
   * \{
   */
  /**
   * Deliver Peer link management information to the protocol-part
   * \param interface is a interface ID of a given MAC (interfaceID rather
   * than MAC address, because many interfaces may have the same MAC)
   * \param peerAddress is address of peer
   * \param peerMeshPointAddress is address of peer mesh point device (equal
   * to peer address when only one interface)
   * \param aid is association ID, which peer has assigned to us
   * \param peerManagementElement is peer link management element
   * \param meshConfig is mesh configuration element taken from the peer management frame
   */
  void ReceivePeerLinkFrame (
    uint32_t interface,
    Mac48Address peerAddress,
    Mac48Address peerMeshPointAddress,
    uint16_t aid,
    IePeerManagement peerManagementElement,
    IeConfiguration meshConfig
    );
  /**
   * \brief Cancels peer link due to broken configuration (Mesh ID or Supported
   * rates)
   */
  void ConfigurationMismatch (uint32_t interface, Mac48Address peerAddress);
  /**
   * \brief Cancels peer link due to successive transmission failures
   */
  void TransmissionFailure (uint32_t interface, const Mac48Address peerAddress);
  /**
   * \brief resets transmission failure statistics
   */
  void TransmissionSuccess (uint32_t interface, const Mac48Address peerAddress);
  /**
   * \brief Checks if there is established link
   */
  bool IsActiveLink (uint32_t interface, Mac48Address peerAddress);
  // \}
  ///\name Interface to other protocols (MLME)
  // \{
  /// Set peer link status change callback
  void SetPeerLinkStatusCallback (Callback<void, Mac48Address, Mac48Address, uint32_t, bool> cb);
  /// Find active peer link by my interface and peer interface MAC
  Ptr<PeerLink> FindPeerLink (uint32_t interface, Mac48Address peerAddress);
  /// Get list of all active peer links
  std::vector < Ptr<PeerLink> > GetPeerLinks () const;
  /// Get list of active peers of my given interface
  std::vector<Mac48Address> GetPeers (uint32_t interface) const;
  /**
   * Get mesh point address. \todo this used by plugins only. Now MAC plugins can ask MP address directly from main MAC
   *
   * \return Mac48Address
   */
  Mac48Address GetAddress ();
  /**
   * Get number of links
   * \returns the number of links
   */
  uint8_t GetNumberOfLinks ();
  /**
   * Set mesh ID to a string value 
   * \param s the mesh ID string value
   */
  void SetMeshId (std::string s);
  /**
   * Get mesh ID information element
   * \returns the mesh ID information element
   */
  Ptr<IeMeshId> GetMeshId () const;
  /**
   * Enable or disable beacon collision avoidance
   * \param enable true to enable beacon collision avoidance
   */
  void SetBeaconCollisionAvoidance (bool enable);
  /**
   * Get beacon collision avoidance
   * \returns the beacon collision avoidance flag
   */
  bool GetBeaconCollisionAvoidance () const;
  /**
   * Notify about beacon send event, needed to schedule BCA
   * \param interface the interface to use
   * \param beaconInterval the beacon interval
   */
  void NotifyBeaconSent (uint32_t interface, Time beaconInterval);
  // \}
  
  /**
   * \brief: Report statistics
   * \param os the output stream
   */
  void Report (std::ostream & os) const;
  /// Reset statistics function
  void ResetStats ();
  /**
   * Assign a fixed random variable stream number to the random variables
   * used by this model.  Return the number of streams (possibly zero) that
   * have been assigned.
   *
   * \param stream first stream index to use
   * \return the number of stream indices assigned by this model
   */
  int64_t AssignStreams (int64_t stream);

  /**
   * TracedCallback signature for link open/close events.
   *
   * \param [in] src MAC address of source interface.
   * \param [in] dst MAC address of destination interface.
   */
  typedef void (* LinkOpenCloseTracedCallback)
    (Mac48Address src, const Mac48Address dst);
   

private:
  virtual void DoInitialize ();
  
  // Private structures
  /// Keeps information about beacon of peer station: beacon interval, association ID, last time we have received a beacon
  struct BeaconInfo
  {
    uint16_t aid; ///< Assoc ID
    Time referenceTbtt; ///< When one of my station's beacons was put into a beacon queue;
    Time beaconInterval; ///< Beacon interval of my station;
  };
  /// We keep a vector of pointers to PeerLink class. This vector
  /// keeps all peer links at a given interface.
  typedef std::vector<Ptr<PeerLink> > PeerLinksOnInterface;
  /// This map keeps all peer links.
  typedef std::map<uint32_t, PeerLinksOnInterface>  PeerLinksMap;
  /// This map keeps relationship between peer address and its beacon information
  typedef std::map<Mac48Address, BeaconInfo>  BeaconsOnInterface;
  ///\brief This map keeps beacon information on all interfaces
  typedef std::map<uint32_t, BeaconsOnInterface> BeaconInfoMap;
  ///\brief this vector keeps pointers to MAC-plugins
  typedef std::map<uint32_t, Ptr<PeerManagementProtocolMac> > PeerManagementProtocolMacMap;

private:
  /**
   * assignment operator
   *
   * \param peer the value to assign
   * \returns the assigned value
   */
  PeerManagementProtocol& operator= (const PeerManagementProtocol & peer);
  /// type conversion operator
  PeerManagementProtocol (const PeerManagementProtocol &);

  /**
   * Initiate link functon
   *
   * \param interface the interface to use
   * \param peerAddress the peer address
   * \param peerMeshPointAddress the peer mesh point address
   * \returns the peer link
   */
  Ptr<PeerLink> InitiateLink (
    uint32_t interface,
    Mac48Address peerAddress,
    Mac48Address peerMeshPointAddress
    );
  /**
   * \name External peer-chooser
   * \param interface the interface to use
   * \param peerAddress the peer address
   * \returns true is should send an open
   */
  bool ShouldSendOpen (uint32_t interface, Mac48Address peerAddress);
  /**
   * \name External peer-chooser
   */
  bool ShouldAcceptOpen (uint32_t interface, Mac48Address peerAddress, PmpReasonCode & reasonCode);
  /**
   * \brief Indicates changes in peer links
   */
  void PeerLinkStatus (uint32_t interface, Mac48Address peerAddress, Mac48Address peerMeshPointAddres, PeerLink::PeerState ostate, PeerLink::PeerState nstate);
  ///\brief BCA
  void CheckBeaconCollisions (uint32_t interface);
  /// Shift own beacon function
  void ShiftOwnBeacon (uint32_t interface);
  /**
   * \name Time<-->TU converters:
   */
  Time TuToTime (int x);
  /**
   * \name Time<-->TU converters:
   */
  int TimeToTu (Time x);

  /// Aux. method to register open links
  void NotifyLinkOpen (Mac48Address peerMp, Mac48Address peerIface, Mac48Address myIface, uint32_t interface);
  /// Aux. method to register closed links
  void NotifyLinkClose (Mac48Address peerMp, Mac48Address peerIface, Mac48Address myIface, uint32_t interface);
private:
  PeerManagementProtocolMacMap m_plugins; ///< plugins
  Mac48Address m_address; ///< addresss
  Ptr<IeMeshId> m_meshId; ///< mesh ID

  uint16_t m_lastAssocId; ///< last associated ID
  uint16_t m_lastLocalLinkId; ///< last local link ID
  uint8_t m_maxNumberOfPeerLinks; ///< maimum number of peer links
  /// Flag which enables BCA
  bool m_enableBca;
  /// Beacon can be shifted at [-m_maxBeaconShift; +m_maxBeaconShift] TUs
  uint16_t m_maxBeaconShift;
  /// Last beacon at each interface
  std::map<uint32_t, Time> m_lastBeacon;
  /// Beacon interval at each interface
  std::map<uint32_t, Time> m_beaconInterval;

  /**
   * \name Peer Links
   */
  PeerLinksMap m_peerLinks;
  /**
   * \brief Callback to notify about peer link changes:
   * Mac48Address is peer address of mesh point,
   * Mac48Address is peer address of interface,
   * uint32_t - interface ID,
   * bool is status - true when new link has appeared, false - when link was closed,
   */
  Callback <void, Mac48Address, Mac48Address, uint32_t, bool> m_peerStatusCallback;

  /// Simple link open/close trace source type. Addresses are: src interface, dst interface
  typedef TracedCallback <Mac48Address, Mac48Address> LinkEventCallback;
  /// LinkOpen trace source
  LinkEventCallback m_linkOpenTraceSrc;
  /// LinkClose trace source
  LinkEventCallback m_linkCloseTraceSrc;

  /// Statistics structure
  struct Statistics {
    uint16_t linksTotal; ///< total links
    uint16_t linksOpened; ///< opened links
    uint16_t linksClosed; ///< links closed

    /**
     * Constructor
     *
     * \param t 
     */
    Statistics (uint16_t t = 0);
    /**
     * Print function
     * \param os the output stream to print to
     */
    void Print (std::ostream & os) const;
  };
  struct Statistics m_stats; ///< statistics

  /// Add randomness to beacon shift
  Ptr<UniformRandomVariable> m_beaconShift;
};

} // namespace dot11s
} // namespace ns3
#endif
