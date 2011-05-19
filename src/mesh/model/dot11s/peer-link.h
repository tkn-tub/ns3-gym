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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 *          Aleksey Kovalenko <kovalenko@iitp.ru>
 */

#ifndef PEERLLINK_H_
#define PEERLLINK_H_

#include "ns3/nstime.h"
#include "ns3/object.h"
#include "ns3/callback.h"
#include "ns3/mac48-address.h"
#include "ns3/event-id.h"
#include "ns3/ie-dot11s-beacon-timing.h"
#include "ns3/ie-dot11s-peer-management.h"
#include "ns3/ie-dot11s-configuration.h"
namespace ns3 {
namespace dot11s {
class PeerManagementProtocolMac;
/**
 * \ingroup dot11s
 *
 * \brief Peer link model for 802.11s Peer Management protocol
 */
class PeerLink : public Object
{
public:
  friend class PeerManagementProtocol;
  /// Support object system
  static TypeId GetTypeId ();
  /// C-tor create empty link
  PeerLink ();
  ~PeerLink ();
  void DoDispose ();
  /// Peer Link state:
  enum  PeerState {
    IDLE,
    OPN_SNT,
    CNF_RCVD,
    OPN_RCVD,
    ESTAB,
    HOLDING,
  };
  /// Process beacon received from peer
  void SetBeaconInformation (Time lastBeacon, Time BeaconInterval);
  /**
   * \brief Method used to detect peer link changes
   *
   * \param cb is a callback, which notifies, that on interface (uint32_t), peer link
   * with address (Mac48Address) was opened (bool is true) or closed (bool is false) 
   */
  void  SetLinkStatusCallback (Callback<void, uint32_t, Mac48Address, bool> cb);
  /**
   * \name Peer link getters/setters
   * \{
   */
  void SetPeerAddress (Mac48Address macaddr);
  void SetPeerMeshPointAddress (Mac48Address macaddr);
  void SetInterface (uint32_t interface);
  void SetLocalLinkId (uint16_t id);
  void SetLocalAid (uint16_t aid);
  uint16_t GetPeerAid () const;
  void SetBeaconTimingElement (IeBeaconTiming beaconTiming);
  Mac48Address GetPeerAddress () const;
  uint16_t GetLocalAid () const;
  Time GetLastBeacon () const;
  Time GetBeaconInterval () const;
  IeBeaconTiming GetBeaconTimingElement () const;
  //IePeerManagement GetPeerLinkDescriptorElement ()const;
  //\}

  /**
   * \name MLME
   * \{
   */
  /// MLME-CancelPeerLink.request
  void MLMECancelPeerLink (PmpReasonCode reason);
  /// MLME-ActivePeerLinkOpen.request
  void MLMEActivePeerLinkOpen ();
  /// MLME-PeeringRequestReject
  void MLMEPeeringRequestReject ();
  /// Callback type for MLME-SignalPeerLinkStatus event
  typedef Callback<void, uint32_t, Mac48Address, Mac48Address, PeerLink::PeerState, PeerLink::PeerState> SignalStatusCallback;
  /// Set callback
  void MLMESetSignalStatusCallback (SignalStatusCallback);
  /// Reports about transmission success/failure
  void TransmissionSuccess ();
  void TransmissionFailure ();
  //\}
  ///\brief Statistics
  void Report (std::ostream & os) const;
private:
  /// Peer link events, see 802.11s draft 11B.3.3.2
  enum  PeerEvent
  {
    CNCL,       ///< Cancel peer link
    ACTOPN,     ///< Active peer link open
    CLS_ACPT,   ///< PeerLinkClose_Accept
    OPN_ACPT,   ///< PeerLinkOpen_Accept
    OPN_RJCT,   ///< PeerLinkOpen_Reject
    REQ_RJCT,   ///< PeerLinkOpenReject by internal reason
    CNF_ACPT,   ///< PeerLinkConfirm_Accept
    CNF_RJCT,   ///< PeerLinkConfirm_Reject
    TOR1,       ///< Timeout of retry timer
    TOR2,       ///< also timeout of retry timer
    TOC,        ///< Timeout of confirm timer
    TOH,        ///< Timeout of holding (graceful closing) timer
  };
  /// State transition
  void StateMachine (PeerEvent event, PmpReasonCode = REASON11S_RESERVED);
  /**
   * \name Link response to received management frames
   *
   * \attention In all this methods {local/peer}LinkID correspond to _peer_ station, as written in
   * received frame, e.g. I am peerLinkID and peer link is localLinkID .
   *
   * \{
   */
  /// Close link
  void Close (uint16_t localLinkID, uint16_t peerLinkID, PmpReasonCode reason);
  /// Accept open link
  void OpenAccept (uint16_t localLinkId, IeConfiguration conf, Mac48Address peerMp);
  /// Reject open link
  void OpenReject (uint16_t localLinkId, IeConfiguration conf, Mac48Address peerMp, PmpReasonCode reason);
  /// Confirm accept
  void ConfirmAccept (
    uint16_t localLinkId,
    uint16_t peerLinkId,
    uint16_t peerAid,
    IeConfiguration conf,
    Mac48Address peerMp
    );
  /// Confirm reject
  void  ConfirmReject (
    uint16_t localLinkId,
    uint16_t peerLinkId,
    IeConfiguration  conf,
    Mac48Address peerMp,
    PmpReasonCode reason
    );
  //\}
  /// True if link is established
  bool  LinkIsEstab () const;
  /// True if link is idle. Link can be deleted in this state
  bool  LinkIsIdle () const;
  /**
   * Set pointer to MAC-plugin, which is responsible for sending peer
   * link management frames
   */
  void SetMacPlugin (Ptr<PeerManagementProtocolMac> plugin);
  /**
   * \name Event handlers
   * \{
   */
  void ClearRetryTimer ();
  void ClearConfirmTimer ();
  void ClearHoldingTimer ();
  void SetHoldingTimer ();
  void SetRetryTimer ();
  void SetConfirmTimer ();
  //\}

  /**
   * \name Work with management frames
   * \{
   */
  void SendPeerLinkClose (PmpReasonCode reasoncode);
  void SendPeerLinkOpen ();
  void SendPeerLinkConfirm ();
  //\}

  /**
   * \name Timeout handlers
   * \{
   */
  void HoldingTimeout ();
  void RetryTimeout ();
  void ConfirmTimeout ();
  //\}
  /// Several successive beacons were lost, close link
  void BeaconLoss ();
private:

  PeerLink& operator= (const PeerLink &);
  PeerLink (const PeerLink &);

  ///The number of interface I am associated with
  uint32_t m_interface;
  /// pointer to MAC plugin, which is responsible for peer management
  Ptr<PeerManagementProtocolMac> m_macPlugin;
  /// Peer address
  Mac48Address m_peerAddress;
  /// Mesh point address, equal to peer address in case of single
  //interface mesh point
  Mac48Address m_peerMeshPointAddress;
  /// My ID of this link
  uint16_t m_localLinkId;
  /// Peer ID of this link
  uint16_t m_peerLinkId;
  /// My association ID
  uint16_t m_assocId;
  /// Assoc Id assigned to me by peer
  uint16_t m_peerAssocId;

  /// When last beacon was received
  Time  m_lastBeacon;
  /// Current beacon interval on corresponding interface
  Time  m_beaconInterval;
  /// How many successive packets were failed to transmit
  uint16_t m_packetFail;

  /// Current state
  PeerState m_state;
  /**
   * \brief Mesh interface configuration
   * \attention Is not used now, nothing to configure :)
   */
  IeConfiguration m_configuration;
  /// Beacon timing element received from the peer. Needed by BCA
  IeBeaconTiming m_beaconTiming;

  /**
   * \name Timers & counters used for internal state transitions
   * \{
   */
  uint16_t m_dot11MeshMaxRetries;
  Time     m_dot11MeshRetryTimeout;
  Time     m_dot11MeshHoldingTimeout;
  Time     m_dot11MeshConfirmTimeout;

  EventId  m_retryTimer;
  EventId  m_holdingTimer;
  EventId  m_confirmTimer;
  uint16_t m_retryCounter;
  EventId  m_beaconLossTimer;
  uint16_t m_maxBeaconLoss;
  uint16_t m_maxPacketFail;
  //\}
  /// How to report my status change
  SignalStatusCallback m_linkStatusCallback;
};

} // namespace dot11s
} //namespace ns3
#endif /* PEERLLINK_H_ */
