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
 *          Pavel Boyko <boyko@iitp.ru>
 */


#ifndef L2ROUTING_NET_DEVICE_H
#define L2ROUTING_NET_DEVICE_H

#include "ns3/net-device.h"
#include "ns3/mac48-address.h"
#include "ns3/bridge-channel.h"
#include "ns3/mesh-l2-routing-protocol.h"

namespace ns3 {
  
class Node;
/**
 * \ingroup mesh
 *
 * \brief Virtual net device modeling mesh point. 
 *
 * Mesh point is a virtual net device which is responsible for
 *   - Aggreagating and coordinating 1..* real devices -- mesh interfaces, see MeshInterfaceDevice class. 
 *   - Hosting all mesh-related level 2 protocols. 
 * 
 * One of hosted L2 protocols must inplement L2RoutingProtocol interface and is used for packets forwarding.
 *
 * From the level 3 point of view MeshPointDevice is similar to BridgeNetDevice, but the packets, 
 * which going through may be changed (because L2 protocols may require their own headers or tags).
 * 
 * Attributes: TODO
 */
class MeshPointDevice : public NetDevice
{
public:
  /// Object type ID for NS3 object system
  static TypeId GetTypeId ();
  /// C-tor create empty (without interfaces and protocols) mesh point
  MeshPointDevice ();
  /// D-tor
  virtual ~MeshPointDevice ();
  
  ///\name Interfaces 
  //\{
  /**
   * \brief Attach new interface to the station. Interface must support 48-bit MAC address and SendFrom method.
   * 
   * \attention Only MeshPointDevice can have IP address, but not individual interfaces. 
   */
  void AddInterface (Ptr<NetDevice> port);
  /**
   * \return number of interfaces
   */
  uint32_t GetNInterfaces () const;
  /**
   * \return interface device by its index (aka ID)
   * \param id is interface id, 0 <= id < GetNInterfaces
   */
  Ptr<NetDevice> GetInterface (uint32_t id) const;
  /**
   * \return vector of interfaces
   */
  std::vector<Ptr<NetDevice> > GetInterfaces () const;
  //\}
  
  ///\name Protocols
  //\{
  /// Register routing protocol to be used. Protocol must be alredy installed on this mesh point.
  void SetRoutingProtocol (Ptr<MeshL2RoutingProtocol> protocol);
  /// Access current routing protocol
  Ptr<MeshL2RoutingProtocol> GetRoutingProtocol() const;
  //\}
  
  ///\name NetDevice interface for upper layers
  //\{
  virtual void SetName (const std::string name);
  virtual std::string GetName () const;
  virtual void SetIfIndex (const uint32_t index);
  virtual uint32_t GetIfIndex () const;
  virtual Ptr<Channel> GetChannel () const;
  virtual Address GetAddress () const;
  virtual bool SetMtu (const uint16_t mtu);
  virtual uint16_t GetMtu () const;
  virtual bool IsLinkUp () const;
  virtual void SetLinkChangeCallback (Callback<void> callback);
  virtual bool IsBroadcast () const;
  virtual Address GetBroadcast () const;
  virtual bool IsMulticast () const;
  virtual Address GetMulticast (Ipv4Address multicastGroup) const;
  virtual bool IsPointToPoint () const;
  virtual bool IsBridge () const;
  virtual bool Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);
  virtual bool SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
  virtual Ptr<Node> GetNode () const;
  virtual void SetNode (Ptr<Node> node);
  virtual bool NeedsArp () const;
  virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);
  virtual void SetPromiscReceiveCallback (NetDevice::PromiscReceiveCallback cb);
  virtual bool SupportsSendFrom () const;
  virtual Address GetMulticast (Ipv6Address addr) const;
  virtual void DoDispose ();
  //\}
  
private:
  /// Receive packet from interface
  void ReceiveFromDevice (Ptr<NetDevice> device, Ptr<const Packet> packet, uint16_t protocol,
                          Address const &source, Address const &destination, PacketType packetType);
  /// Forward packet down to interfaces 
  void Forward (Ptr<NetDevice> incomingPort, Ptr<const Packet> packet,
                uint16_t protocol, const Mac48Address src, const Mac48Address dst);
  /**
   * Response callback for L2 routing protocol. This will be executed when routing information is ready.
   * 
   * \param success     True is route found. TODO: diagnose routing errors
   * \param packet      Packet to send
   * \param src         Source MAC address
   * \param dst         Destination MAC address
   * \param protocol    Protocol ID
   * \param outIface    Interface to use (ID) for send (decided by routing protocol). All interfaces will be used if outIface = 0xffffffff
   */
  void DoSend (bool success, Ptr<Packet> packet, Mac48Address src, Mac48Address dst, uint16_t protocol, uint32_t iface);
  
private:
  /// Receive action
  NetDevice::ReceiveCallback   m_rxCallback;
  /// Promisc receive action
  NetDevice::PromiscReceiveCallback  m_promiscRxCallback;
  /// Mesh point MAC address, supposed to be the address of the first added interface
  Mac48Address m_address;
  /// Parent node
  Ptr<Node> m_node;
  /// Station name
  std::string m_name;
  /// List of interfaces
  std::vector< Ptr<NetDevice> > m_ifaces;
  /// If index 
  uint32_t m_ifIndex;
  /// MTU in bytes
  uint16_t m_mtu;
  /// Virtual channel for upper layers
  Ptr<BridgeChannel> m_channel;
  
  /// Routing request callback
  Callback<bool,
           uint32_t,
           Mac48Address,
           Mac48Address,
           Ptr<const Packet>,
           uint16_t,
           MeshL2RoutingProtocol::RouteReplyCallback>  m_requestRoute;
  
  /// Routing response callback, this is supplied to mesh routing protocol
  MeshL2RoutingProtocol::RouteReplyCallback  m_myResponse;
  /// Current routing protocol, used mainly by GetRoutingProtocol
  Ptr<MeshL2RoutingProtocol> m_routingProtocol;
};
} //namespace ns3
#endif
