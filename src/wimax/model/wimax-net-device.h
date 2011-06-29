/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008, 2009 INRIA, UDcast
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
 * Authors: Jahanzeb Farooq <jahanzeb.farooq@sophia.inria.fr>
 *          Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 */

#ifndef WIMAX_NET_DEVICE_H
#define WIMAX_NET_DEVICE_H

#include "ns3/net-device.h"
#include "ns3/event-id.h"
#include "ns3/mac48-address.h"
#include "cid.h"
#include "wimax-connection.h"
#include "cid-factory.h"
#include "mac-messages.h"
#include "dl-mac-messages.h"
#include "ul-mac-messages.h"
#include "ns3/traced-callback.h"
#include "ns3/nstime.h"
#include "ns3/log.h"
#include "wimax-phy.h"
#include "wimax-mac-header.h"

namespace ns3 {

class Node;
class Packet;
class TraceContext;
class TraceResolver;
class Channel;
class WimaxChannel;
class PacketBurst;
class BurstProfileManager;
class ConnectionManager;
class ServiceFlowManager;
class BandwidthManager;
class UplinkScheduler;

/**
 * \defgroup wimax WiMAX Models
 *
 * This section documents the API of the ns-3 wimax module. For a generic functional description, please refer to the ns-3 manual.
 */

/**
 * \brief Hold together all Wimax-related objects in a NetDevice.
 * \ingroup wimax
 *
 * This class holds together ns3::WimaxPhy, ns3::WimaxConnection,
 * ns3::ConectionManager, ns3::BurstProfileManager, and 
 * ns3::BandwidthManager.
 */
class WimaxNetDevice : public NetDevice
{
public:
  enum Direction
  {
    DIRECTION_DOWNLINK, DIRECTION_UPLINK
  };

  enum RangingStatus
  {
    RANGING_STATUS_EXPIRED, RANGING_STATUS_CONTINUE, RANGING_STATUS_ABORT, RANGING_STATUS_SUCCESS
  };

  static TypeId GetTypeId (void);
  WimaxNetDevice (void);
  virtual ~WimaxNetDevice (void);
  /**
   * \param ttg transmit/receive transition gap
   */
  void SetTtg (uint16_t ttg);
  /**
   * \returns transmit/receive transition gap
   */
  uint16_t GetTtg (void) const;
  /**
   * \param rtg receive/transmit transition gap
   */
  void SetRtg (uint16_t rtg);
  /**
   * \returns receive/transmit transition gap
   */
  uint16_t GetRtg (void) const;
  void Attach (Ptr<WimaxChannel> channel);
  /**
   * \param phy the phy layer to use.
   */
  void SetPhy (Ptr<WimaxPhy> phy);
  /**
   * \returns a pointer to the physical layer.
   */
  Ptr<WimaxPhy> GetPhy (void) const;

  /**
   * \param wimaxChannel the channel to be used
   */
  void SetChannel (Ptr<WimaxChannel> wimaxChannel);

  uint64_t GetChannel (uint8_t index) const;

  void SetNrFrames (uint32_t nrFrames);
  uint32_t GetNrFrames (void) const;
  /**
   * \param address the mac address of the net device
   */
  void SetMacAddress (Mac48Address address);
  /**
   * \returns the mac address of the net device
   */
  Mac48Address GetMacAddress (void) const;
  void SetState (uint8_t state);
  uint8_t GetState (void) const;
  /**
   * \returns the initial ranging connection
   */
  Ptr<WimaxConnection> GetInitialRangingConnection (void) const;
  /**
    * \returns the broadcast connection
    */
  Ptr<WimaxConnection> GetBroadcastConnection (void) const;

  void SetCurrentDcd (Dcd dcd);
  Dcd GetCurrentDcd (void) const;
  void SetCurrentUcd (Ucd ucd);
  Ucd GetCurrentUcd (void) const;
  /**
   * \returns the connection manager of the device
   */
  Ptr<ConnectionManager> GetConnectionManager (void) const;

  /**
   * \param  connectionManager the commection manager to be installed in the device
   */
  virtual void SetConnectionManager (Ptr<ConnectionManager> connectionManager );

  /**
   * \returns the burst profile manager currently installed in the device
   */
  Ptr<BurstProfileManager> GetBurstProfileManager (void) const;

  /**
   * \param burstProfileManager the burst profile manager to be installed on the device
   */
  void SetBurstProfileManager (Ptr<BurstProfileManager> burstProfileManager);

  /**
   * \returns the bandwidth manager installed on the device
   */
  Ptr<BandwidthManager> GetBandwidthManager (void) const;

  /**
   * \param bandwidthManager the bandwidth manager to be installed on the device
   */
  void SetBandwidthManager (Ptr<BandwidthManager> bandwidthManager);

  /*
   * \brief Creates the initial ranging and broadcast connections
   */
  void CreateDefaultConnections (void);

  virtual void Start (void) = 0;
  virtual void Stop (void) = 0;

  void SetReceiveCallback (void);

  void ForwardUp (Ptr<Packet> packet, const Mac48Address &source, const Mac48Address &dest);

  virtual bool Enqueue (Ptr<Packet> packet, const MacHeaderType &hdrType, Ptr<WimaxConnection> connection) = 0;
  void ForwardDown (Ptr<PacketBurst> burst, WimaxPhy::ModulationType modulationType);

  // temp, shall be private
  static uint8_t m_direction; // downlink or uplink

  static Time m_frameStartTime; // temp, to determine the frame start time at SS side, shall actually be determined by frame start preamble

  virtual void SetName (const std::string name);
  virtual std::string GetName (void) const;
  virtual void SetIfIndex (const uint32_t index);
  virtual uint32_t GetIfIndex (void) const;
  virtual Ptr<Channel> GetPhyChannel (void) const;
  virtual Ptr<Channel> GetChannel (void) const;
  virtual void SetAddress (Address address);
  virtual Address GetAddress (void) const;
  virtual bool SetMtu (const uint16_t mtu);
  virtual uint16_t GetMtu (void) const;
  virtual bool IsLinkUp (void) const;
  virtual void SetLinkChangeCallback (Callback<void> callback);
  virtual bool IsBroadcast (void) const;
  virtual Address GetBroadcast (void) const;
  virtual bool IsMulticast (void) const;
  virtual Address GetMulticast (void) const;
  virtual Address MakeMulticastAddress (Ipv4Address multicastGroup) const;
  virtual bool IsPointToPoint (void) const;
  virtual bool Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);
  virtual void SetNode (Ptr<Node> node);
  virtual Ptr<Node> GetNode (void) const;
  virtual bool NeedsArp (void) const;
  virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);
  virtual void AddLinkChangeCallback (Callback<void> callback);
  virtual bool SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
  virtual void SetPromiscReceiveCallback (PromiscReceiveCallback cb);
  NetDevice::PromiscReceiveCallback GetPromiscReceiveCallback (void);
  virtual bool SupportsSendFrom (void) const;

  TracedCallback<Ptr<const Packet>, const Mac48Address&> m_traceRx;
  TracedCallback<Ptr<const Packet>, const Mac48Address&> m_traceTx;

  virtual void DoDispose (void);
  virtual Address GetMulticast (Ipv6Address addr) const;
  virtual Address GetMulticast (Ipv4Address multicastGroup) const;
  virtual bool IsBridge (void) const;

  bool IsPromisc (void);
  void NotifyPromiscTrace (Ptr<Packet> p);

private:
  WimaxNetDevice (const WimaxNetDevice &);
  WimaxNetDevice & operator= (const WimaxNetDevice &);

  static const uint16_t MAX_MSDU_SIZE = 1500;
  // recommended by wimax forum.
  static const uint16_t DEFAULT_MSDU_SIZE = 1400;

  virtual bool DoSend (Ptr<Packet> packet,
                       const Mac48Address& source,
                       const Mac48Address& dest,
                       uint16_t protocolNumber) = 0;
  virtual void DoReceive (Ptr<Packet> packet) = 0;
  virtual Ptr<WimaxChannel> DoGetChannel (void) const;
  void Receive (Ptr<const PacketBurst> burst);
  void InitializeChannels (void);

  Ptr<Node> m_node;
  Ptr<WimaxPhy> m_phy;
  NetDevice::ReceiveCallback m_forwardUp;
  NetDevice::PromiscReceiveCallback m_promiscRx;

  uint32_t m_ifIndex;
  std::string m_name;
  bool m_linkUp;
  Callback<void> m_linkChange;
  mutable uint16_t m_mtu;

  // temp, shall be in BS. defined here to allow SS to access. SS shall actually determine it from DLFP, shall be moved to BS after DLFP is implemented
  static uint32_t m_nrFrames;

  // not sure if it shall be included here
  std::vector<uint64_t> m_dlChannels;

  Mac48Address m_address;
  uint8_t m_state;
  uint32_t m_symbolIndex;

  // length of TTG and RTG, in units of PSs
  uint16_t m_ttg;
  uint16_t m_rtg;

  Dcd m_currentDcd;
  Ucd m_currentUcd;

  Ptr<WimaxConnection> m_initialRangingConnection;
  Ptr<WimaxConnection> m_broadcastConnection;

  Ptr<ConnectionManager> m_connectionManager;
  Ptr<BurstProfileManager> m_burstProfileManager;
  Ptr<BandwidthManager> m_bandwidthManager;

  Ptr<Object> m_mobility;

};

} // namespace ns3

#endif /* WIMAX_NET_DEVICE_H */
