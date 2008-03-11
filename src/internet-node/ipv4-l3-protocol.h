// -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*-
//
// Copyright (c) 2006 Georgia Tech Research Corporation
// All rights reserved.
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
#include "ns3/ipv4.h"
#include "ns3/traced-callback.h"
#include "ipv4-header.h"
#include "ipv4-static-routing.h"

namespace ns3 {

class Packet;
class NetDevice;
class Ipv4Interface;
class Ipv4Address;
class Ipv4Header;
class Ipv4Route;
class Node;


class Ipv4L3Protocol : public Object
{
public:
  static TypeId GetTypeId (void);
  static const uint16_t PROT_NUMBER;

  Ipv4L3Protocol();
  virtual ~Ipv4L3Protocol ();

  void SetNode (Ptr<Node> node);

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
  void Receive( Ptr<NetDevice> device, Ptr<Packet> p, uint16_t protocol, const Address &from);

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
  
  void JoinMulticastGroup (Ipv4Address origin, Ipv4Address group);
  void LeaveMulticastGroup (Ipv4Address origin, Ipv4Address group);

  void SetAddress (uint32_t i, Ipv4Address address);
  void SetNetworkMask (uint32_t i, Ipv4Mask mask);
  Ipv4Mask GetNetworkMask (uint32_t t) const;
  Ipv4Address GetAddress (uint32_t i) const;
  void SetMetric (uint32_t i, uint16_t metric);
  uint16_t GetMetric (uint32_t i) const;
  bool GetIfIndexForDestination (Ipv4Address destination, 
                                 uint32_t& ifIndex) const;
  uint16_t GetMtu (uint32_t i) const;
  bool IsUp (uint32_t i) const;
  void SetUp (uint32_t i);
  void SetDown (uint32_t i);

  void AddRoutingProtocol (Ptr<Ipv4RoutingProtocol> routingProtocol,
                           int priority);

protected:

  virtual void DoDispose (void);

private:
  void Lookup (uint32_t ifIndex,
               Ipv4Header const &ipHeader,
               Ptr<Packet> packet,
               Ipv4RoutingProtocol::RouteReplyCallback routeReply);

  void SendRealOut (bool found,
                    Ipv4Route const &route,
                    Ptr<Packet> packet,
                    Ipv4Header const &ipHeader);
  bool Forwarding (uint32_t ifIndex, 
                   Ptr<Packet> packet, 
                   Ipv4Header &ipHeader, 
                   Ptr<NetDevice> device);
  void ForwardUp (Ptr<Packet> p, Ipv4Header const&ip, Ptr<Ipv4Interface> incomingInterface);
  uint32_t AddIpv4Interface (Ptr<Ipv4Interface> interface);
  void SetupLoopback (void);

  typedef std::list<Ptr<Ipv4Interface> > Ipv4InterfaceList;
  typedef std::list<std::pair<Ipv4Address, Ipv4Address> > 
    Ipv4MulticastGroupList;
  typedef std::list< std::pair< int, Ptr<Ipv4RoutingProtocol> > > Ipv4RoutingProtocolList;

  Ipv4InterfaceList m_interfaces;
  uint32_t m_nInterfaces;
  uint8_t m_defaultTtl;
  uint16_t m_identification;
  Ptr<Node> m_node;
  TracedCallback<Ptr<const Packet>, uint32_t> m_txTrace;
  TracedCallback<Ptr<const Packet>, uint32_t> m_rxTrace;
  TracedCallback<Ptr<const Packet> > m_dropTrace;

  Ipv4RoutingProtocolList m_routingProtocols;

  Ptr<Ipv4StaticRouting> m_staticRouting;
  Ipv4MulticastGroupList m_multicastGroups;
};

} // Namespace ns3

#endif /* IPV4_L3_PROTOCOL_H */
