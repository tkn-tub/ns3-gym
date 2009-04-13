// -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*-
//
// Copyright (c) 2006 Georgia Tech Research Corporation
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: George F. Riley<riley@ece.gatech.edu>
//

#ifndef IPV4_L3_PROTOCOL_H
#define IPV4_L3_PROTOCOL_H

#include <list>
#include <stdint.h>
#include "ns3/ipv4-address.h"
#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/ipv4.h"
#include "ns3/traced-callback.h"
#include "ns3/ipv4-header.h"
#include "ipv4-static-routing.h"

namespace ns3 {

class Packet;
class NetDevice;
class Ipv4Interface;
class Ipv4Address;
class Ipv4Header;
class Ipv4Route;
class Node;
class Socket;
class Ipv4RawSocketImpl;
class Ipv4L4Protocol;
class Ipv4L4Protocol;
class Icmpv4L4Protocol;


/**
 * \brief Implement the Ipv4 layer.
 * 
 * This is the actual implementation of IP.  It contains APIs to send and
 * receive packets at the IP layer, as well as APIs for IP routing.
 */
class Ipv4L3Protocol : public Object
{
public:
  static TypeId GetTypeId (void);
  static const uint16_t PROT_NUMBER;

  Ipv4L3Protocol();
  virtual ~Ipv4L3Protocol ();

  void SetNode (Ptr<Node> node);

  Ptr<Socket> CreateRawSocket (void);
  void DeleteRawSocket (Ptr<Socket> socket);

  /**
   * \param protocol a template for the protocol to add to this L4 Demux.
   * \returns the L4Protocol effectively added.
   *
   * Invoke Copy on the input template to get a copy of the input
   * protocol which can be used on the Node on which this L4 Demux 
   * is running. The new L4Protocol is registered internally as
   * a working L4 Protocol and returned from this method.
   * The caller does not get ownership of the returned pointer.
   */
  void Insert(Ptr<Ipv4L4Protocol> protocol);
  /**
   * \param protocolNumber number of protocol to lookup
   *        in this L4 Demux
   * \returns a matching L4 Protocol
   *
   * This method is typically called by lower layers
   * to forward packets up the stack to the right protocol.
   * It is also called from NodeImpl::GetUdp for example.
   */
  Ptr<Ipv4L4Protocol> GetProtocol(int protocolNumber) const;
  /**
   * \param protocol protocol to remove from this demux.
   *
   * The input value to this method should be the value
   * returned from the Ipv4L4Protocol::Insert method.
   */
  void Remove (Ptr<Ipv4L4Protocol> protocol);

  /**
   * \param ttl default ttl to use
   *
   * When we need to send an ipv4 packet, we use this default
   * ttl value.
   */
  void SetDefaultTtl (uint8_t ttl);

  /**
   * \param device the device to match
   * \returns the matching interface, zero if not found.
   *
   * Try to find an Ipv4Interface whose NetDevice is equal to
   * the input NetDevice.
   */
  Ptr<Ipv4Interface> FindInterfaceForDevice (Ptr<const NetDevice> device);

  /**
   * Lower layer calls this method after calling L3Demux::Lookup
   * The ARP subclass needs to know from which NetDevice this
   * packet is coming to:
   *    - implement a per-NetDevice ARP cache
   *    - send back arp replies on the right device
   */
  void Receive( Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from,
                const Address &to, NetDevice::PacketType packetType);

  /**
   * \param packet packet to send
   * \param source source address of packet
   * \param destination address of packet
   * \param protocol number of packet
   *
   * Higher-level layers call this method to send a packet
   * down the stack to the MAC and PHY layers.
   */
  void Send (Ptr<Packet> packet, Ipv4Address source, 
	     Ipv4Address destination, uint8_t protocol);


    
  void AddHostRouteTo (Ipv4Address dest, 
                       Ipv4Address nextHop, 
                       uint32_t interface);
  void AddHostRouteTo (Ipv4Address dest, 
                       uint32_t interface);

  void AddNetworkRouteTo (Ipv4Address network, 
                          Ipv4Mask networkMask, 
                          Ipv4Address nextHop, 
                          uint32_t interface);
  void AddNetworkRouteTo (Ipv4Address network, 
                          Ipv4Mask networkMask, 
                          uint32_t interface);
  void SetDefaultRoute (Ipv4Address nextHop, 
                        uint32_t interface);

  void Lookup (Ipv4Header const &ipHeader,
               Ptr<Packet> packet,
               Ipv4RoutingProtocol::RouteReplyCallback routeReply);

  uint32_t GetNRoutes (void);
  Ipv4Route *GetRoute (uint32_t i);
  void RemoveRoute (uint32_t i);

  void AddMulticastRoute (Ipv4Address origin,
                          Ipv4Address group,
                          uint32_t inputInterface,
                          std::vector<uint32_t> outputInterfaces);

  void SetDefaultMulticastRoute (uint32_t onputInterface);

  uint32_t GetNMulticastRoutes (void) const;
  Ipv4MulticastRoute *GetMulticastRoute (uint32_t i) const;

  void RemoveMulticastRoute (Ipv4Address origin,
                             Ipv4Address group,
                             uint32_t inputInterface);
  void RemoveMulticastRoute (uint32_t i);

  uint32_t AddInterface (Ptr<NetDevice> device);
  Ptr<Ipv4Interface> GetInterface (uint32_t i) const;
  uint32_t GetNInterfaces (void) const;

  uint32_t FindInterfaceForAddr (Ipv4Address addr) const;
  uint32_t FindInterfaceForAddr (Ipv4Address addr, Ipv4Mask mask) const;
  int32_t FindInterfaceIndexForDevice (Ptr<NetDevice> device) const;
  
  void JoinMulticastGroup (Ipv4Address origin, Ipv4Address group);
  void LeaveMulticastGroup (Ipv4Address origin, Ipv4Address group);

  uint32_t AddAddress (uint32_t i, Ipv4InterfaceAddress address);
  Ipv4InterfaceAddress GetAddress (uint32_t interfaceIndex, uint32_t addressIndex) const;
  uint32_t GetNAddresses (uint32_t interface) const;

  void SetMetric (uint32_t i, uint16_t metric);
  uint16_t GetMetric (uint32_t i) const;
  Ipv4Address GetSourceAddress (Ipv4Address destination) const;
  bool GetInterfaceForDestination (Ipv4Address destination, 
                                 uint32_t& interface) const;
  uint16_t GetMtu (uint32_t i) const;
  bool IsUp (uint32_t i) const;
  void SetUp (uint32_t i);
  void SetDown (uint32_t i);

  void AddRoutingProtocol (Ptr<Ipv4RoutingProtocol> routingProtocol,
                           int priority);

protected:

  virtual void DoDispose (void);

private:
  Ipv4L3Protocol(const Ipv4L3Protocol &);
  Ipv4L3Protocol &operator = (const Ipv4L3Protocol &);
  void Lookup (uint32_t interface,
               Ipv4Header const &ipHeader,
               Ptr<Packet> packet,
               Ipv4RoutingProtocol::RouteReplyCallback routeReply);

  void SendRealOut (bool found,
                    Ipv4Route const &route,
                    Ptr<Packet> packet,
                    Ipv4Header const &ipHeader);
  bool Forwarding (uint32_t interface, 
                   Ptr<Packet> packet, 
                   Ipv4Header &ipHeader, 
                   Ptr<NetDevice> device);
  void ForwardUp (Ptr<Packet> p, Ipv4Header const&ip, Ptr<Ipv4Interface> incomingInterface);
  uint32_t AddIpv4Interface (Ptr<Ipv4Interface> interface);
  void SetupLoopback (void);
  Ptr<Icmpv4L4Protocol> GetIcmp (void) const;
  bool IsUnicast (Ipv4Address ad, Ipv4Mask interfaceMask) const;
  void DoForward (uint32_t interface, 
                  Ptr<Packet> packet, 
                  Ipv4Header ipHeader);


  typedef std::list<Ptr<Ipv4Interface> > Ipv4InterfaceList;
  typedef std::list<std::pair<Ipv4Address, Ipv4Address> > Ipv4MulticastGroupList;
  typedef std::list< std::pair< int, Ptr<Ipv4RoutingProtocol> > > Ipv4RoutingProtocolList;
  typedef std::list<Ptr<Ipv4RawSocketImpl> > SocketList;
  typedef std::list<Ptr<Ipv4L4Protocol> > L4List_t;

  L4List_t m_protocols;
  Ipv4InterfaceList m_interfaces;
  uint32_t m_nInterfaces;
  uint8_t m_defaultTtl;
  bool m_calcChecksum;
  uint16_t m_identification;
  Ptr<Node> m_node;
  TracedCallback<Ptr<const Packet>, uint32_t> m_txTrace;
  TracedCallback<Ptr<const Packet>, uint32_t> m_rxTrace;
  TracedCallback<Ptr<const Packet> > m_dropTrace;

  Ipv4RoutingProtocolList m_routingProtocols;

  Ptr<Ipv4StaticRouting> m_staticRouting;
  Ipv4MulticastGroupList m_multicastGroups;

  SocketList m_sockets;
};

} // Namespace ns3

#endif /* IPV4_L3_PROTOCOL_H */
