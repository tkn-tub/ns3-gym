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
#include "ns3/callback.h"
#include "ns3/mac48-address.h"
#include "ns3/event-id.h"
#include "ie-dot11s-beacon-timing.h"
#include "ie-dot11s-peer-management.h"
#include "ie-dot11s-configuration.h"
#include "peer-management-plugin.h"
namespace ns3 {
namespace dot11s {
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
  static TypeId GetTypeId();
  /// C-tor create empty link
  PeerLink ();
  ~PeerLink ();
  void DoDispose ();
  
  /// Process beacon received from peer
  void SetBeaconInformation (Time lastBeacon, Time BeaconInterval);
  /**
   * \brief Method used to detecet peer link changes
   * 
   * \param bool if true - opened new link, if false - link closed
   */
  void  SetLinkStatusCallback (Callback<void, uint32_t, Mac48Address, bool> cb);
  /**
   * \name Peer link geeters/setters
   * \{
   */
  void SetPeerAddress (Mac48Address macaddr);
  void SetPeerMeshPointAddress (Mac48Address macaddr);
  void SetInterface (uint32_t interface);
  void SetLocalLinkId (uint16_t id);
  void SetPeerLinkId (uint16_t id);
  void SetLocalAid (uint16_t aid);
  void SetPeerAid (uint16_t aid);
  void SetBeaconTimingElement (IeBeaconTiming beaconTiming);
  void SetPeerLinkDescriptorElement (IePeerManagement peerLinkElement);
  Mac48Address GetPeerAddress () const;
  uint16_t GetLocalAid () const;
  Time GetLastBeacon () const;
  Time GetBeaconInterval () const;
  IeBeaconTiming GetBeaconTimingElement ()const;
  IePeerManagement GetPeerLinkDescriptorElement ()const;
  //\}
  
  /**
   * \name MLME
   * \{
   */
  /// MLME-CancelPeerLink.request
  void MLMECancelPeerLink (PmpReasonCode reason);
  /// MLME-PassivePeerLinkOpen.request
  void MLMEPassivePeerLinkOpen ();
  /// MLME-ActivePeerLinkOpen.request
  void MLMEActivePeerLinkOpen ();
  /// MLME-PeeringRequestReject
  void MLMEPeeringRequestReject ();
  /// Callback type for MLME-SignalPeerLinkStatus event
  typedef Callback<void, uint32_t, Mac48Address, Mac48Address, bool> SignalStatusCallback; 
  /// Set callback
  void MLMESetSignalStatusCallback (SignalStatusCallback);
  //\}
  ///\brief Statistics
  void Report (std::ostream & os) const;
private:
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
  void SetMacPlugin(Ptr<PeerManagerMacPlugin> plugin);
private:
  /// Peer link states, see 802.11s draft 11B.3.3.1
  enum  PeerState {
    IDLE,       
    OPN_SNT,
    CNF_RCVD,
    OPN_RCVD,
    ESTAB,
    HOLDING,
  };
  /// Peer link events, see 802.11s draft 11B.3.3.2
  enum  PeerEvent
  {
    CNCL,       ///< MLME-CancelPeerLink
    PASOPN,     ///< MLME-PassivePeerLinkOpen
    ACTOPN,     ///< MLME-ActivePeerLinkOpen
    CLS_ACPT,   ///< PeerLinkClose_Accept
    OPN_ACPT,   ///< PeerLinkOpen_Accept
    OPN_RJCT,   ///< PeerLinkOpen_Reject
    REQ_RJCT,   ///< PeerLinkOpenReject by internal reason
    CNF_ACPT,   ///< PeerLinkConfirm_Accept
    CNF_RJCT,   ///< PeerLinkConfirm_Reject
    TOR1,       ///< Timeout of retry timer
    TOR2,       ///< also timeout of retry timer
    TOC,        ///< Timeout of confirm timer
    TOH,        ///< Timeout of holding (gracefull closing) timer
  };
  
private:
  /// State transition
  void StateMachine (PeerEvent event, PmpReasonCode = REASON11S_RESERVED);
  
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
  
private:
  ///The number of interface I am associated with
  uint32_t m_interface;
  /// pointer to mac plugin, which is responsible for peer management
  Ptr<PeerManagerMacPlugin> m_macPlugin;
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
  
  /// Current state
  PeerState m_state;
  /// Mesh interface configuration
  IeConfiguration m_configuration;
  
  // State is a bitfield as defined as follows:
  // This are states for a given
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
  uint16_t  m_maxBeaconLoss;
  //\}
  
  /// Several successive beacons were lost, close link
  void BeaconLoss ();
   
  /// How to report my status change
  SignalStatusCallback m_linkStatusCallback;
};
  
} // namespace dot11s
} //namespace ns3
#endif /* PEERLLINK_H_ */
