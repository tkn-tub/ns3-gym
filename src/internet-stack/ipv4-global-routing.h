// -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*-
//
// Copyright (c) 2008 University of Washington
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
//

#ifndef IPV4_GLOBAL_ROUTING_H
#define IPV4_GLOBAL_ROUTING_H

#include <list>
#include <stdint.h>
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-header.h"
#include "ns3/ptr.h"
#include "ns3/ipv4.h"

namespace ns3 {

class Packet;
class NetDevice;
class Ipv4Interface;
class Ipv4Address;
class Ipv4Header;
class Ipv4Route;
class Node;


/**
 * @brief Global routing protocol for IP version 4 stacks.
 *
 * In ns-3 we have the concept of a pluggable routing protocol.  Routing
 * protocols are added to a list maintained by the Ipv4L3Protocol.  Every 
 * stack gets one routing protocol for free -- the Ipv4StaticRouting routing
 * protocol is added in the constructor of the Ipv4L3Protocol (this is the 
 * piece of code that implements the functionality of the IP layer).
 *
 * As an option to running a dynamic routing protocol, a GlobalRouteManager
 * object has been created to allow users to build routes for all participating
 * nodes.  One can think of this object as a "routing oracle"; it has
 * an omniscient view of the topology, and can construct shortest path
 * routes between all pairs of nodes.  These routes must be stored 
 * somewhere in the node, so therefore this class Ipv4GlobalRouting
 * is used as one of the pluggable routing protocols.  It is kept distinct
 * from Ipv4StaticRouting because these routes may be dynamically cleared
 * and rebuilt in the middle of the simulation, while manually entered
 * routes into the Ipv4StaticRouting may need to be kept distinct.
 *
 * This class deals with Ipv4 unicast routes only.
 *
 * @see Ipv4RoutingProtocol
 * @see GlobalRouteManager
 */
class Ipv4GlobalRouting : public Ipv4RoutingProtocol
{
public:
  static TypeId GetTypeId (void);
/**
 * @brief Construct an empty Ipv4GlobalRouting routing protocol,
 *
 * The Ipv4GlobalRouting class supports host and network unicast routes.
 * This method initializes the lists containing these routes to empty.
 *
 * @see Ipv4GlobalRouting
 */
  Ipv4GlobalRouting ();

/**
 * @brief Request that a check for a route bw performed and if a route is found
 * that the packet be sent on its way using the pre-packaged send callback.
 *
 * The source and destination IP addresses for the packet in question are found
 * in the provided Ipv4Header.  There are two major processing forks depending
 * on the type of destination address.  
 *
 * If the destination address is unicast then the routing table is consulted 
 * for a route to the destination and if it is found, the routeReply callback
 * is executed to send the packet (with the found route).
 * 
 * If the destination address is a multicast, then the method will return
 * false.
 *
 * @param interface The network interface index over which the packed was 
 * received.  If the packet is from a local source, interface will be set to
 * Ipv4RoutingProtocol::INTERFACE_INDEX_ANY.
 * @param ipHeader the Ipv4Header containing the source and destination IP
 * addresses for the packet.
 * @param packet The packet to be sent if a route is found.
 * @param routeReply A callback that packaged up the call to actually send the
 * packet.
 * @return Returns true if a route is found and the packet has been sent,
 * otherwise returns false indicating that the next routing protocol should
 * be consulted.  
 *
 * @see Ipv4GlobalRouting
 * @see Ipv4RoutingProtocol
 */
  virtual bool RequestRoute (uint32_t interface,
                             Ipv4Header const &ipHeader,
                             Ptr<Packet> packet,
                             RouteReplyCallback routeReply);

/**
 * @brief Check to see if we can determine the interface index that will be
 * used if a packet is sent to this destination.
 *
 * This method addresses a problem in the IP stack where a destination address
 * must be present and checksummed into the IP header before the actual 
 * interface over which the packet is sent can be determined.  The answer is
 * to implement a known and intentional cross-layer violation.  This is the
 * endpoint of a call chain that started up quite high in the stack (sockets)
 * and has found its way down to the Ipv4L3Protocol which is consulting the
 * routing protocols for what they would do if presented with a packet of the
 * given destination.
 *
 * If there are multiple paths out of the node, the resolution is performed
 * by Ipv4L3Protocol::GetInterfaceforDestination which has access to more 
 * contextual information that is useful for making a determination.
 *
 * This method will return false on a multicast address.
 *
 * @param destination The Ipv4Address if the destination of a hypothetical 
 * packet.  This may be a multicast group address.
 * @param interface A reference to the interface index over which a packet
 * sent to this destination would be sent.
 * @return Returns true if a route is found to the destination that involves
 * a single output interface index, otherwise returns false indicating that
 * the next routing protocol should be consulted.  
 *
 * @see Ipv4GlobalRouting
 * @see Ipv4RoutingProtocol
 * @see Ipv4L3Protocol
 */
  virtual bool RequestInterface (Ipv4Address destination, uint32_t& interface);

/**
 * @brief Add a host route to the global routing table.
 *
 * @param dest The Ipv4Address destination for this route.
 * @param nextHop The Ipv4Address of the next hop in the route.
 * @param interface The network interface index used to send packets to the
 * destination.
 *
 * @see Ipv4Address
 */
  void AddHostRouteTo (Ipv4Address dest, 
                       Ipv4Address nextHop, 
                       uint32_t interface);
/**
 * @brief Add a host route to the global routing table.
 *
 * @param dest The Ipv4Address destination for this route.
 * @param interface The network interface index used to send packets to the
 * destination.
 *
 * @see Ipv4Address
 */
  void AddHostRouteTo (Ipv4Address dest, 
                       uint32_t interface);

/**
 * @brief Add a network route to the global routing table.
 *
 * @param network The Ipv4Address network for this route.
 * @param networkMask The Ipv4Mask to extract the network.
 * @param nextHop The next hop in the route to the destination network.
 * @param interface The network interface index used to send packets to the
 * destination.
 *
 * @see Ipv4Address
 */
  void AddNetworkRouteTo (Ipv4Address network, 
                          Ipv4Mask networkMask, 
                          Ipv4Address nextHop, 
                          uint32_t interface);

/**
 * @brief Add a network route to the global routing table.
 *
 * @param network The Ipv4Address network for this route.
 * @param networkMask The Ipv4Mask to extract the network.
 * @param interface The network interface index used to send packets to the
 * destination.
 *
 * @see Ipv4Address
 */
  void AddNetworkRouteTo (Ipv4Address network, 
                          Ipv4Mask networkMask, 
                          uint32_t interface);

/**
 * @brief Get the number of individual unicast routes that have been added
 * to the routing table.
 *
 * @warning The default route counts as one of the routes.
 */
  uint32_t GetNRoutes (void);

/**
 * @brief Get a route from the global unicast routing table.
 *
 * Externally, the unicast global routing table appears simply as a table with
 * n entries.  The one sublety of note is that if a default route has been set
 * it will appear as the zeroth entry in the table.  This means that if you
 * add only a default route, the table will have one entry that can be accessed
 * either by explicity calling GetDefaultRoute () or by calling GetRoute (0).
 * 
 * Similarly, if the default route has been set, calling RemoveRoute (0) will
 * remove the default route.
 *
 * @param i The index (into the routing table) of the route to retrieve.  If
 * the default route has been set, it will occupy index zero.
 * @return If route is set, a pointer to that Ipv4Route is returned, otherwise
 * a zero pointer is returned.
 *
 * @see Ipv4Route
 * @see Ipv4GlobalRouting::RemoveRoute
 */
  Ipv4Route *GetRoute (uint32_t i);

/**
 * @brief Remove a route from the global unicast routing table.
 *
 * Externally, the unicast global routing table appears simply as a table with
 * n entries.  The one sublety of note is that if a default route has been set
 * it will appear as the zeroth entry in the table.  This means that if the
 * default route has been set, calling RemoveRoute (0) will remove the
 * default route.
 *
 * @param i The index (into the routing table) of the route to remove.  If
 * the default route has been set, it will occupy index zero.
 *
 * @see Ipv4Route
 * @see Ipv4GlobalRouting::GetRoute
 * @see Ipv4GlobalRouting::AddRoute
 */
  void RemoveRoute (uint32_t i);

protected:
  void DoDispose (void);

private:
  typedef std::list<Ipv4Route *> HostRoutes;
  typedef std::list<Ipv4Route *>::const_iterator HostRoutesCI;
  typedef std::list<Ipv4Route *>::iterator HostRoutesI;
  typedef std::list<Ipv4Route *> NetworkRoutes;
  typedef std::list<Ipv4Route *>::const_iterator NetworkRoutesCI;
  typedef std::list<Ipv4Route *>::iterator NetworkRoutesI;

  Ipv4Route *LookupGlobal (Ipv4Address dest);

  HostRoutes m_hostRoutes;
  NetworkRoutes m_networkRoutes;
};

} // Namespace ns3

#endif /* IPV4_GLOBAL_ROUTING_H */
