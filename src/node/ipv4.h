/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * All rights reserved.
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef IPV4_H
#define IPV4_H

#include <stdint.h>
#include "ns3/ipv4-address.h"
#include "ns3/object.h"
#include "ns3/callback.h"
#include "ipv4-route.h"

namespace ns3 {

class Node;
class NetDevice;
class Packet;
class Ipv4Route;
class Ipv4Header; // FIXME: ipv4-header.h needs to move from module
                  // "internet-node" to module "node"

/**
 * \brief Base class for IPv4 routing protocols.
 *
 * This class represents the interface between the IPv4 routing core
 * and a specific IPv4 routing protocol.  The interface is
 * asynchronous (callback based) in order to support reactive routing
 * protocols (e.g. AODV).
 */
class Ipv4RoutingProtocol : public Object
{
public:
  // void (*RouteReply) (bool found, Ipv4Route route, Packet packet, Ipv4Header const &ipHeader);


  /**
   * \brief Callback to be invoked when route discovery is completed
   *
   * \param bool flag indicating whether a route was actually found;
   * when this is false, the Ipv4Route parameter is ignored
   *
   * \param Ipv4Route the route found
   *
   * \param Packet the packet for which a route was requested; can be
   * modified by the routing protocol
   *
   * \param Ipv4Header the IP header supplied to the route request
   * method (possibly modified in case a new routing header is
   * inserted and consequently the protocol type has to change).
   *
   */
  typedef Callback<void, bool, const Ipv4Route&, Packet, const Ipv4Header&> RouteReplyCallback;

  /**
   * \brief Asynchronously requests a route for a given packet and IP header
   *
   * \param ipHeader IP header of the packet
   * \param packet packet that is being sent or forwarded
   * \param routeReply callback that will receive the route reply
   *
   * \returns true if the routing protocol should be able to get the
   * route, false otherwise.
   *
   * This method is called whenever a node's IPv4 forwarding engine
   * needs to lookup a route for a given packet and IP header.
   *
   * The routing protocol implementation may determine immediately it
   * should not be handling this particular the route request.  For
   * instance, a routing protocol may decline to search for routes for
   * certain classes of addresses, like link-local.  In this case,
   * RequestRoute() should return false and the routeReply callback
   * must not be invoked.
   *
   * If the routing protocol implementations assumes it can provide
   * the requested route, then it should return true, and the
   * routeReply callback must be invoked, either immediately before
   * returning true (synchronously), or in the future (asynchronous).
   * The routing protocol may use any information available in the IP
   * header and packet as routing key, although most routing protocols
   * use only the destination address (as given by
   * ipHeader.GetDestination ()).  The routing protocol is also
   * allowed to add a new header to the packet, which will appear
   * immediately after the IP header, although most routing do not
   * insert any extra header.
   */
  virtual bool RequestRoute (const Ipv4Header &ipHeader,
                             Packet packet,
                             RouteReplyCallback routeReply) = 0;
};

/**
 * \brief Access to the Ipv4 forwarding table and to the ipv4 interfaces
 *
 * This class allows you to create ipv4 interfaces based on a NetDevice.
 * Multiple interfaces can be created for a single NetDevice, hence
 * achieving multihoming.
 *
 * This class also allows you to control the content of the ipv4 
 * forwarding table.
 */
class Ipv4 : public Object
{
public:
  static const InterfaceId iid;
  Ipv4 ();
  virtual ~Ipv4 ();

  /**
   * \brief Register a new routing protocol to be used in this IPv4 stack
   * 
   * \param routingProtocol new routing protocol implementation object
   * \param priority priority to give to this routing protocol.
   * Values may range between -32768 and +32767.  The priority 0
   * corresponds to static routing table lookups, higher values have
   * more priority.  The order by which routing protocols with the
   * same priority value are consulted is undefined.
   */
  virtual void AddRoutingProtocol (Ptr<Ipv4RoutingProtocol> routingProtocol,
                                   int16_t priority) = 0;
  
  /**
   * \param dest destination address
   * \param nextHop address of next hop.
   * \param interface interface of next hop.
   *
   * Add route to host dest through host nextHop 
   * on interface.
   */
  virtual void AddHostRouteTo (Ipv4Address dest, 
			       Ipv4Address nextHop, 
			       uint32_t interface) = 0;
  /**
   * \param dest destination address
   * \param interface of next hop
   *
   * add route to host dest on interface.
   */
  virtual void AddHostRouteTo (Ipv4Address dest, 
			       uint32_t interface) = 0;

  /**
   * \param network destination network
   * \param networkMask netmask of destination network
   * \param nextHop address of next hop
   * \param interface interface of next hop
   * 
   * add route to network dest with netmask 
   * through host nextHop on interface
   */
  virtual void AddNetworkRouteTo (Ipv4Address network, 
				  Ipv4Mask networkMask, 
				  Ipv4Address nextHop, 
				  uint32_t interface) = 0;

  /**
   * \param network destination network
   * \param networkMask netmask of destination network
   * \param interface interface of next hop
   *
   * add route to network dest with netmask 
   * on interface
   */
  virtual void AddNetworkRouteTo (Ipv4Address network, 
				  Ipv4Mask networkMask, 
				  uint32_t interface) = 0;
  /**
   * \param nextHop address of default next hop
   * \param interface interface of default next hop.
   * 
   * set the default route to host nextHop on
   * interface. 
   */
  virtual void SetDefaultRoute (Ipv4Address nextHop, 
				uint32_t interface) = 0;

  /**
   * \returns the number of entries in the routing table.
   */
  virtual uint32_t GetNRoutes (void) = 0;
  /**
   * \param i index of route to return
   * \returns the route whose index is i
   */
  virtual Ipv4Route GetRoute (uint32_t i) = 0;
  /**
   * \param i index of route to remove from routing table.
   */
  virtual void RemoveRoute (uint32_t i) = 0;
  
  /**
   * \param device device to add to the list of ipv4 interfaces
   *        which can be used as output interfaces during packet forwarding.
   * \returns the index of the ipv4 interface added.
   *
   * Once a device has been added, it can never be removed: if you want
   * to disable it, you can invoke Ipv4::SetDown which will
   * make sure that it is never used during packet forwarding.
   */
  virtual uint32_t AddInterface (Ptr<NetDevice> device) = 0;
  /**
   * \returns the number of interfaces added by the user.
   */
  virtual uint32_t GetNInterfaces (void) = 0;  

  /**
   * \param i index of ipv4 interface
   * \returns the NetDevice associated with the ipv4 interface index
   */
  virtual Ptr<NetDevice> GetNetDevice (uint32_t i) = 0;

  /**
   * \param i index of ipv4 interface
   * \param address address to associate to the underlying ipv4 interface
   */
  virtual void SetAddress (uint32_t i, Ipv4Address address) = 0;
  /**
   * \param i index of ipv4 interface
   * \param mask mask to associate to the underlying ipv4 interface
   */
  virtual void SetNetworkMask (uint32_t i, Ipv4Mask mask) = 0;
  /**
   * \param i index of ipv4 interface
   * \returns the mask associated to the underlying ipv4 interface
   */
  virtual Ipv4Mask GetNetworkMask (uint32_t i) const = 0;
  /**
   * \param i index of ipv4 interface
   * \returns the address associated to the underlying ipv4 interface
   */
  virtual Ipv4Address GetAddress (uint32_t i) const = 0;
  /**
   * \param i index of ipv4 interface
   * \returns the Maximum Transmission Unit (in bytes) associated
   *          to the underlying ipv4 interface
   */
  virtual uint16_t GetMtu (uint32_t i) const = 0;
  /**
   * \param i index of ipv4 interface
   * \returns true if the underlying interface is in the "up" state,
   *          false otherwise.
   */
  virtual bool IsUp (uint32_t i) const = 0;
  /**
   * \param i index of ipv4 interface
   * 
   * Set the interface into the "up" state. In this state, it is
   * considered valid during ipv4 forwarding.
   */
  virtual void SetUp (uint32_t i) = 0;
  /**
   * \param i index of ipv4 interface
   *
   * Set the interface into the "down" state. In this state, it is
   * ignored during ipv4 forwarding.
   */
  virtual void SetDown (uint32_t i) = 0;
  
};

/**
 * Convenience functions (Doxygen still needed)
 *
 * Return the ifIndex corresponding to the Ipv4Address provided.
 */
uint32_t GetIfIndexByIpv4Address (Ptr<Node> node, 
                                  Ipv4Address a, 
                                  Ipv4Mask amask = Ipv4Mask("255.255.255.255"));

bool GetIpv4RouteToDestination (Ptr<Node> node, Ipv4Route& route, 
                                Ipv4Address a, 
                                Ipv4Mask amask = Ipv4Mask("255.255.255.255"));

} // namespace ns3 

#endif /* IPV4_H */
