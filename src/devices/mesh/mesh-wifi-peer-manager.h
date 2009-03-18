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


#ifndef WIFI_PEER_MAN_H
#define WIFI_PEER_MAN_H

#include "ns3/mac48-address.h"
#include "ns3/wifi-net-device.h"
#include "ns3/ie-dot11s-peer-management.h"
#include "ns3/ie-dot11s-beacon-timing.h"
#include "ns3/mesh-wifi-mac.h"

#include <list>

namespace ns3 {
class MeshWifiMac;
/**
 * \ingroup mesh
 */
class WifiPeerLinkDescriptor : public RefCountBase
{
public:
  WifiPeerLinkDescriptor ();
  /**
   * Beacon loss processing:
   */
  void  SetBeaconInformation (Time lastBeacon, Time BeaconInterval);
  void  SetMaxBeaconLoss (uint8_t maxBeaconLoss);
  /**
   * \brief Methods used to detecet peer link changes
   * \param bool if true - opened new link, if
   * false - link closed
   */
  void  SetLinkStatusCallback (Callback<void, Mac48Address, Mac48Address, bool> cb);
  /**
   * Peer link geeters/setters
   */
  void  SetPeerAddress (Mac48Address macaddr);
  /**
   * Debug purpose
   */
  void  SetLocalAddress (Mac48Address macaddr);
  void  SetLocalLinkId (uint16_t id);
  void  SetPeerLinkId (uint16_t id);
  void  SetLocalAid (uint16_t aid);
  void  SetPeerAid (uint16_t aid);
  void  SetBeaconTimingElement (IeDot11sBeaconTiming beaconTiming);
  void  SetPeerLinkDescriptorElement (
    IeDot11sPeerManagement peerLinkElement
  );
  Mac48Address GetPeerAddress ()const;
  /**
   * Debug purpose
   */
  Mac48Address GetLocalAddress ()const;
  uint16_t GetLocalAid ()const;
  Time  GetLastBeacon ()const;
  Time  GetBeaconInterval ()const;
  IeDot11sBeaconTiming
  GetBeaconTimingElement ()const;
  IeDot11sPeerManagement
  GetPeerLinkDescriptorElement ()const;
  void  ClearTimingElement ();
  /* MLME */
  void  MLMECancelPeerLink (dot11sReasonCode reason);
  void  MLMEPassivePeerLinkOpen ();
  void  MLMEActivePeerLinkOpen ();
  void  MLMEPeeringRequestReject ();
#if 0
  void  MLMEBindSecurityAssociation ();
#endif
  void  SetMac (Ptr<MeshWifiMac> mac);
  void  PeerLinkClose (uint16_t localLinkID,uint16_t peerLinkID, dot11sReasonCode reason);
  void  PeerLinkOpenAccept (uint16_t localLinkId, IeDot11sConfiguration  conf);
  void  PeerLinkOpenReject (uint16_t localLinkId, IeDot11sConfiguration  conf,dot11sReasonCode reason);
  void  PeerLinkConfirmAccept (
    uint16_t localLinkId,
    uint16_t peerLinkId,
    uint16_t peerAid,
    IeDot11sConfiguration  conf
  );
  void  PeerLinkConfirmReject (
    uint16_t localLinkId,
    uint16_t peerLinkId,
    IeDot11sConfiguration  conf,
    dot11sReasonCode reason
  );
  bool  LinkIsEstab () const;
  bool  LinkIsIdle () const;
private:
  enum  PeerState {
    IDLE,
    LISTEN,
    OPN_SNT,
    CNF_RCVD,
    OPN_RCVD,
    ESTAB,
    HOLDING,
  };
  enum  PeerEvent
  {
    CNCL,  /** MLME-CancelPeerLink */
    PASOPN,  /** MLME-PassivePeerLinkOpen */
    ACTOPN,  /** MLME-ActivePeerLinkOpen */
    //BNDSA,     /** MLME-BindSecurityAssociation */
    CLS_ACPT, /** PeerLinkClose_Accept */
    //CLS_IGNR, /** PeerLinkClose_Ignore */
    OPN_ACPT, /** PeerLinkOpen_Accept */
    //OPN_IGNR, /** PeerLinkOpen_Ignore */
    OPN_RJCT, /** PeerLinkOpen_Reject */
    REQ_RJCT, /** PeerLinkOpenReject by internal reason */
    CNF_ACPT, /** PeerLinkConfirm_Accept */
    //CNF_IGNR, /** PeerLinkConfirm_Ignore */
    CNF_RJCT, /** PeerLinkConfirm_Reject */
    TOR1,
    TOR2,
    TOC,
    TOH,
  };
private:
  void StateMachine (PeerEvent event,dot11sReasonCode = REASON11S_RESERVED);
  /** Events handlers */
  void ClearRetryTimer ();
  void ClearConfirmTimer ();
  void ClearHoldingTimer ();
  void SetHoldingTimer ();
  void SetRetryTimer ();
  void SetConfirmTimer ();

  void SendPeerLinkClose (dot11sReasonCode reasoncode );
  void SendPeerLinkOpen ();
  void SendPeerLinkConfirm ();
  /** Private Event */
  void HoldingTimeout ();
  void RetryTimeout ();
  void ConfirmTimeout ();
private:
  Mac48Address m_peerAddress;
  Mac48Address m_localAddress;
  uint16_t m_localLinkId;
  uint16_t m_peerLinkId;
  // Used for beacon timing:
  // All values are stored in microseconds!
  Time  m_lastBeacon;
  Time  m_beaconInterval;
  uint16_t m_assocId; //Assigned Assoc ID
  uint16_t m_peerAssocId; //Assoc Id assigned to me by peer
  //State of our peer Link:
  PeerState m_state;

  IeDot11sConfiguration
  m_configuration;
  // State is a bitfield as defined as follows:
  // This are states for a given
  IeDot11sBeaconTiming
  m_beaconTiming;

  EventId  m_retryTimer;
  EventId  m_holdingTimer;
  EventId  m_confirmTimer;
  uint16_t m_retryCounter;
  /**
   * Beacon loss timers:
   */
  EventId  m_beaconLossTimer;
  uint8_t  m_maxBeaconLoss;
  void  BeaconLoss ();
  Ptr<MeshWifiMac> m_mac;
  Callback<void, Mac48Address, Mac48Address, bool>
  m_linkStatusCallback;
};
/**
 * \ingroup mesh
 */
class WifiPeerManager : public Object
{
public:
  WifiPeerManager ();
  WifiPeerManager (Ptr<MeshWifiMac> mac_pointer);
  ~WifiPeerManager ();
  static TypeId GetTypeId ();
  //Returns a beacon timing element stored for remote station:
  IeDot11sBeaconTiming
  GetIeDot11sBeaconTimingForAddress (Mac48Address portAddress, Mac48Address addr);
  //Returns a list of all addresses, which beacons can be decoded:
  std::vector<Mac48Address>
  GetNeighbourAddressList (Mac48Address portAddress, Mac48Address peerAddress);
  bool AttachPorts (std::vector<Ptr<WifiNetDevice> >);
  //void SetMac (Ptr<MeshWifiMac> mac);
  Time GetNextBeaconShift (Mac48Address portAddress, Time myNextTBTT);

  void SetSentBeaconTimers (
    Mac48Address portAddress,
    Time  ReferenceTBTT,
    Time  BeaconInterval
  );
  void AskIfOpenNeeded (
    Mac48Address portAddress,
    Mac48Address peerAddress
  );
  void SetReceivedBeaconTimers (
    Mac48Address portAddress,
    Mac48Address peerAddress,
    Time  lastBeacon,
    Time  beaconInterval,
    IeDot11sBeaconTiming
    beaconTiming
  );
  void SetOpenReceived (
    Mac48Address portAddress,
    Mac48Address peerAddress,
    IeDot11sPeerManagement
    peerMan,
    IeDot11sConfiguration conf
  );
  void SetConfirmReceived (
    Mac48Address portAddress,
    Mac48Address peerAddress,
    uint16_t peerAid,
    IeDot11sPeerManagement
    peerMan,
    IeDot11sConfiguration meshConfig
  );
  void SetCloseReceived (
    Mac48Address portAddress,
    Mac48Address peerAddress,
    IeDot11sPeerManagement peerMan
  );
  //Using this function MAC
  void ConfigurationMismatch (
    Mac48Address portAddress,
    Mac48Address peerAddress
  );
  //Returns a beacon timing element to added into my beacon:
  IeDot11sBeaconTiming
  GetIeDot11sBeaconTimingForMyBeacon (
    Mac48Address portAddress
  );
  bool IsActiveLink (
    Mac48Address portAddress,
    Mac48Address peerAddress
  );
private:
  struct BeaconInfo
  {
    Time referenceTbtt; //When one of my station's beacons was put into a beacon queue;
    Time beaconInterval; //Beacon interval of my station;
  };
  typedef std::map<Mac48Address, std::vector<Ptr<WifiPeerLinkDescriptor> >, std::less<Mac48Address> >
  PeerDescriptorsMap;
  typedef std::map<Mac48Address, Ptr<MeshWifiMac>,std::less<Mac48Address> > MeshMacMap;
  typedef std::map<Mac48Address, BeaconInfo, std::less<Mac48Address> > BeaconInfoMap;

  //Ptr<MeshWifiMac> m_mac;
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
  Ptr<WifiPeerLinkDescriptor>
  AddDescriptor (
    Mac48Address portAddress,
    Mac48Address peerAddress,
    Time lastBeacon,
    Time beaconInterval
  );
  void  PeerCleanup ();
  //Mechanism of choosing PEERs:
  bool  ShouldSendOpen (Mac48Address portAddress, Mac48Address peerAddress);
  bool  ShouldAcceptOpen (
    Mac48Address portAddress,
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
  void PeerLinkStatus (Mac48Address portAddress, Mac48Address peerAddress, bool status);
};

} //namespace ns3
#endif
