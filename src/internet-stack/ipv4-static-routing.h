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
//         Gustavo Carneiro <gjc@inescporto.pt>
//

#ifndef IPV4_STATIC_ROUTING_H
#define IPV4_STATIC_ROUTING_H

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
 * @brief Static routing protocol for IP version 4 stacks.
 *
 * In ns-3 we have the concept of a pluggable routing protocol.  Routing
 * protocols are added to a list maintained by the Ipv4L3Protocol.  Every 
 * stack gets one routing protocol for free -- the Ipv4StaticRouting routing
 * protocol is added in the constructor of the Ipv4L3Protocol (this is the 
 * piece of code that implements the functionality of the IP layer).
 *
 * The Ipv4StaticRouting class inherits from the abstract base class 
 * Ipv4RoutingProtocol that defines the interface methods that a routing 
 * protocol must support.
 *
 * When a packet arrives in the Ipv4L3Protocol for transmission, it comes
 * either from a local source via Ipv4L3Protocol::Send or from a remote 
 * source via Ipv4L3Protocol::Forwarding.  In both cases, a function is called
 * (Ipv4L3Protocol::Lookup) to look up the routing information for the packet.
 *
 * The lookup function iterates through the list of routing protocols asking
 * each to see if it can find a route and send the packet.  A callback is 
 * provided during each of these calls that should be considered a pre-
 * packaged send call.  This is done to allow asynchronous calls into 
 * routing subsystems in order to support on-demand routing, for example.  The
 * method for requesting this operation is Ipv4StaticRouting::RequestRoute for
 * the static routing protocol.
 *
 * Each routing protocol is also free to implement its own methods for managing
 * routes which you will find below.  This class manages a set of "static" or
 * manually configured routes for host, network and multicast routes.
 *
 * @see Ipv4RoutingProtocol
 * @see Ipv4L3Protocol::AddRoutingProtocol
 * @see Ipv4L3Protocol::Ipv4L3Protocol
 */
class Ipv4StaticRouting : public Ipv4RoutingProtocol
{
public:
/**
 * @brief Construct an empty Ipv4StaticRouting routing protocol,
 * @internal
 *
 * The Ipv4StaticRouting class supports host, network and multicast routes.
 * This method initializes the lists containing these routes to empty.
 *
 * @see Ipv4StaticRouting
 */
  Ipv4StaticRouting ();

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
 * If the destination address is a multicast, then the exact processing steps
 * depend on whether or not the packet has been sourced locally.  This is 
 * determined by the parameter ifIndex.  This is the interface index over which
 * this packet was received.  If the packet has not been received over a
 * network interface, this index will be set to 
 * Ipv4RoutingProtocol::IF_INDEX_ANY (a very large number).  In that case, 
 * we want to avoid the requirement that an explicit route out of each node 
 * must be set, so we don't do anything here.
 * 
 * If the packet is a multicast destination and has been received over a 
 * network interface, a call to this method implies that the packet is being
 * forwarded.  In that case, there must be an explicit route out of the node.
 * A multicast route references the source address, the destination address
 * (the multicast group) and the input interface in order to find a route.
 * We consult the multicast routing table and, if a route is found, send the
 * packet out of as many interfaces as required using the provided callback
 * (think of it as a pre-packaged send call).
 *
 * @param ifIndex The network interface index over which the packed was 
 * received.  If the packet is from a local source, ifIndex will be set to
 * Ipv4RoutingProtocol::IF_INDEX_ANY.
 * @param ipHeader the Ipv4Header containing the source and destination IP
 * addresses for the packet.
 * @param packet The packet to be sent if a route is found.
 * @param routeReply A callback that packaged up the call to actually send the
 * packet.
 * @return Returns true if a route is found and the packet has been sent,
 * otherwise returns false indicating that the next routing protocol should
 * be consulted.  In practice, the static routing protocol is the last chance
 * protocol.
 *
 * @see Ipv4StaticRouting
 * @see Ipv4RoutingProtocol
 */
  virtual bool RequestRoute (uint32_t ifIndex,
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
 * Note that the a single interface index is returned.  This means that if
 * the destination address is a multicast, and an explicit route is present
 * that includeds multiple output interfaces, that route cannot be used.
 * 
 * If there are multiple paths out of the node, the resolution is performed
 * by Ipv4L3Protocol::GetIfIndexforDestination which has access to more 
 * contextual information that is useful for making a determination.
 *
 * @param destination The Ipv4Address if the destination of a hypothetical 
 * packet.  This may be a multicast group address.
 * @param ifIndex A reference to the interface index over which a packet
 * sent to this destination would be sent.
 * @return Returns true if a route is found to the destination that involves
 * a single output interface index, otherwise returns false indicating that
 * the next routing protocol should be consulted.  In practice, the static 
 * routing protocol is the last chance protocol.
 *
 * @see Ipv4StaticRouting
 * @see Ipv4RoutingProtocol
 * @see Ipv4L3Protocol
 */
  virtual bool RequestIfIndex (Ipv4Address destination, uint32_t& ifIndex);

/**
 * @brief Add a host route to the static routing table.
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
 * @brief Add a host route to the static routing table.
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
 * @brief Add a network route to the static routing table.
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
 * @brief Add a network route to the static routing table.
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
 * @brief Add a default route to the static routing table.
 *
 * This method tells the routing system what to do in the case where a specific
 * route to a destination is not found.  The system forwards packets to the
 * specified node in the hope that it knows better how to route the packet.
 * 
 * If the default route is set, it is returned as the selected route from 
 * LookupStatic irrespective of destination address if no specific route is
 * found.
 *
 * @param nextHop The Ipv4Address to send packets to in the hope that they
 * will be forwarded correctly.
 * @param interface The network interface index used to send packets.
 *
 * @see Ipv4Address
 * @see Ipv4StaticRouting::Lookup
 */
  void SetDefaultRoute (Ipv4Address nextHop, 
                        uint32_t interface);

/**
 * @brief Get the number of individual unicast routes that have been added
 * to the routing table.
 *
 * @warning The default route counts as one of the routes.
 */
  uint32_t GetNRoutes (void);

/**
 * @brief Get the default route from the static routing table.
 *
 * @return If the default route is set, a pointer to that Ipv4Route is
 * returned, otherwise a zero pointer is returned.
 *
 * @see Ipv4Route
 */
  Ipv4Route *GetDefaultRoute (void);

/**
 * @brief Get a route from the static unicast routing table.
 *
 * Externally, the unicast static routing table appears simply as a table with
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
 * @see Ipv4StaticRouting::RemoveRoute
 */
  Ipv4Route *GetRoute (uint32_t i);

/**
 * @brief Remove a route from the static unicast routing table.
 *
 * Externally, the unicast static routing table appears simply as a table with
 * n entries.  The one sublety of note is that if a default route has been set
 * it will appear as the zeroth entry in the table.  This means that if the
 * default route has been set, calling RemoveRoute (0) will remove the
 * default route.
 *
 * @param i The index (into the routing table) of the route to remove.  If
 * the default route has been set, it will occupy index zero.
 *
 * @see Ipv4Route
 * @see Ipv4StaticRouting::GetRoute
 * @see Ipv4StaticRouting::AddRoute
 */
  void RemoveRoute (uint32_t i);

/**
 * @brief Add a multicast route to the static routing table.
 *
 * A multicast route must specify an origin IP address, a multicast group and
 * an input network interface index as conditions and provide a vector of
 * output network interface indices over which packets matching the conditions
 * are sent.
 *
 * Typically there are two main types of multicast routes:  routes of the 
 * first kind are used during forwarding.  All of the conditions must be
 * exlicitly provided.  The second kind of routes are used to get packets off
 * of a local node.  The difference is in the input interface.  Routes for
 * forwarding will always have an explicit input interface specified.  Routes
 * off of a node will always set the input interface to a wildcard specified
 * by the index Ipv4RoutingProtocol::IF_INDEX_ANY.
 *
 * For routes off of a local node wildcards may be used in the origin and
 * multicast group addresses.  The wildcard used for Ipv4Adresses is that 
 * address returned by Ipv4Address::GetAny () -- typically "0.0.0.0".  Usage
 * of a wildcard allows one to specify default behavior to varying degrees.
 *
 * For example, making the origin address a wildcard, but leaving the 
 * multicast group specific allows one (in the case of a node with multiple
 * interfaces) to create different routes using different output interfaces
 * for each multicast group.
 *
 * If the origin and multicast addresses are made wildcards, you have created
 * essentially a default multicast address that can forward to multiple 
 * interfaces.  Compare this to the actual default multicast address that is
 * limited to specifying a single output interface for compatibility with
 * existing functionality in other systems.
 * 
 * @param origin The Ipv4Address of the origin of packets for this route.  May
 * be Ipv4Address:GetAny for open groups.
 * @param group The Ipv4Address of the multicast group or this route.
 * @param inputInterface The input network interface index over which to 
 * expect packets destined for this route.  May be
 * Ipv4RoutingProtocol::IF_INDEX_ANY for packets of local origin.
 * @param outputInterfaces A vector of network interface indices used to specify
 * how to send packets to the destination(s).
 *
 * @see Ipv4Address
 */
  void AddMulticastRoute (Ipv4Address origin,
                          Ipv4Address group,
                          uint32_t inputInterface,
                          std::vector<uint32_t> outputInterfaces);

/**
 * @brief Add a default multicast route to the static routing table.
 *
 * This is the multicast equivalent of the unicast version SetDefaultRoute.
 * We tell the routing system what to do in the case where a specific route
 * to a destination multicast group is not found.  The system forwards 
 * packets out the specified interface in the hope that "something out there"
 * knows better how to route the packet.  This method is only used in 
 * initially sending packets off of a host.  The default multicast route is
 * not consulted during forwarding -- exact routes must be specified using
 * AddMulticastRoute for that case.
 *
 * Since we're basically sending packets to some entity we think may know
 * better what to do, we don't pay attention to "subtleties" like origin
 * address, nor do we worry about forwarding out multiple  interfaces.  If the
 * default multicast route is set, it is returned as the selected route from 
 * LookupStatic irrespective of origin or multicast group if another specific
 * route is not found.
 *
 * @param outputInterface The network interface index used to specify where
 * to send packets in the case of unknown routes.
 *
 * @see Ipv4Address
 */
  void SetDefaultMulticastRoute (uint32_t outputInterface);

/**
 * @brief Get the number of individual multicast routes that have been added
 * to the routing table.
 *
 * @warning The default multicast route counts as one of the routes.
 */
  uint32_t GetNMulticastRoutes (void) const;

/**
 * @brief Get a route from the static multicast routing table.
 *
 * Externally, the multicast static routing table appears simply as a table 
 * with n entries.  The one sublety of note is that if a default route has 
 * been set it will appear as the zeroth entry in the table.  This means that 
 * if you add only a default route, the table will have one entry that can be
 * accessed either by explicity calling GetDefaultMulticastRoute () or by
 * calling GetMulticastRoute (0).
 * 
 * Similarly, if the default route has been set, calling 
 * RemoveMulticastRoute (0) will remove the default route.
 *
 * @param i The index (into the routing table) of the multicast route to
 * retrieve.  If the default route has been set, it will occupy index zero.
 * @return If route \e i is set, a pointer to that Ipv4MulticastRoute is
 * returned, otherwise a zero pointer is returned.
 *
 * @see Ipv4MulticastRoute
 * @see Ipv4StaticRouting::RemoveRoute
 */
  Ipv4MulticastRoute *GetMulticastRoute (uint32_t i) const;

/**
 * @brief Get the default multicast route from the static routing table.
 *
 * @return If the default route is set, a pointer to that Ipv4MulticastRoute is
 * returned, otherwise a zero pointer is returned.
 *
 * @see Ipv4Route
 */
  Ipv4MulticastRoute *GetDefaultMulticastRoute (void) const;

/**
 * @brief Remove a route from the static multicast routing table.
 *
 * Externally, the multicast static routing table appears simply as a table 
 * with n entries.  The one sublety of note is that if a default multicast
 * route has been set it will appear as the zeroth entry in the table.  This
 * means that the default route may be removed by calling this method with
 * appropriate wildcard parameters.
 *
 * This method causes the multicast routing table to be searched for the first
 * route that matches the parameters and removes it.
 *
 * Wildcards may be provided to this function, but the wildcards are used to
 * exacly match wildcards in the routes (see AddMulticastRoute).  That is,
 * calling RemoveMulticastRoute with the origin set to "0.0.0.0" will not
 * remove routes with any address in the origin, but will only remove routes
 * with "0.0.0.0" set as the the origin.
 *
 * @param origin The IP address specified as the origin of packets for the
 * route.
 * @param group The IP address specified as the multicast group addres of
 * the route.
 * @param inputInterface The network interface index specified as the expected
 * input interface for the route.
 * @returns true if a route was found and removed, false otherwise.
 *
 * @see Ipv4MulticastRoute
 * @see Ipv4StaticRouting::AddMulticastRoute
 */
  bool RemoveMulticastRoute (Ipv4Address origin,
                             Ipv4Address group,
                             uint32_t inputInterface);

/**
 * @brief Remove a route from the static multicast routing table.
 *
 * Externally, the multicast static routing table appears simply as a table 
 * with n entries.  The one sublety of note is that if a default multicast
 * route has been set it will appear as the zeroth entry in the table.  This 
 * means that if the default route has been set, calling 
 * RemoveMulticastRoute (0) will remove the default route.
 *
 * @param index The index (into the multicast routing table) of the route to
 * remove.  If the default route has been set, it will occupy index zero.
 *
 * @see Ipv4Route
 * @see Ipv4StaticRouting::GetRoute
 * @see Ipv4StaticRouting::AddRoute
 */
  void RemoveMulticastRoute (uint32_t index);

protected:
  void DoDispose (void);

private:
  typedef std::list<Ipv4Route *> HostRoutes;
  typedef std::list<Ipv4Route *>::const_iterator HostRoutesCI;
  typedef std::list<Ipv4Route *>::iterator HostRoutesI;
  typedef std::list<Ipv4Route *> NetworkRoutes;
  typedef std::list<Ipv4Route *>::const_iterator NetworkRoutesCI;
  typedef std::list<Ipv4Route *>::iterator NetworkRoutesI;

  typedef std::list<Ipv4MulticastRoute *> MulticastRoutes;
  typedef std::list<Ipv4MulticastRoute *>::const_iterator MulticastRoutesCI;
  typedef std::list<Ipv4MulticastRoute *>::iterator MulticastRoutesI;

  Ipv4Route *LookupStatic (Ipv4Address dest);
  Ipv4MulticastRoute *LookupStatic (Ipv4Address origin, Ipv4Address group,
                                    uint32_t ifIndex);

  HostRoutes m_hostRoutes;
  NetworkRoutes m_networkRoutes;
  Ipv4Route *m_defaultRoute;
  Ipv4MulticastRoute *m_defaultMulticastRoute;
  MulticastRoutes m_multicastRoutes;
};

} // Namespace ns3

#endif /* IPV4_STATIC_ROUTING_H */
