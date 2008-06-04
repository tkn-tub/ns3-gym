/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
class Ipv4Header;

/**
 * \ingroup node
 * \defgroup ipv4 Ipv4
 */

/**
 * \ingroup ipv4
 *
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
  typedef Callback<void, bool, const Ipv4Route&, Ptr<Packet>, const Ipv4Header&> RouteReplyCallback;

  /**
   * \brief Asynchronously requests a route for a given packet and IP header
   *
   * \param ifIndex The interface index on which the packet was received.
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
  virtual bool RequestRoute (uint32_t ifIndex,
                             const Ipv4Header &ipHeader,
                             Ptr<Packet> packet,
                             RouteReplyCallback routeReply) = 0;

/**
 * \brief Synchronously check to see if we can determine the interface index 
 * that will be used if a packet is sent to this destination.
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
 * \param destination The Ipv4Address if the destination of a hypothetical 
 * packet.  This may be a multicast group address.
 * \param ifIndex A reference to the interface index over which a packet
 * sent to this destination would be sent.
 * \return Returns true if a route is found to the destination that involves
 * a single output interface index, otherwise false.
 *
 * \see Ipv4StaticRouting
 * \see Ipv4RoutingProtocol
 * \see Ipv4L3Protocol
 */
  virtual bool RequestIfIndex (Ipv4Address destination, 
                              uint32_t& ifIndex) = 0;

  static const uint32_t IF_INDEX_ANY = 0xffffffff;
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
  static TypeId GetTypeId (void);
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
   * \brief Add a static multicast route for a given multicast source and 
   *        group.
   *
   * \param origin The Ipv4 address of the multicast source.
   * \param group The multicast group address.
   * \param inputInterface The interface index over which the packet arrived.
   * \param outputInterfaces The list of output interface indices over which 
   *        the packet should be sent (excluding the inputInterface).
   */
  virtual void AddMulticastRoute (Ipv4Address origin,
                                  Ipv4Address group,
                                  uint32_t inputInterface,
                                  std::vector<uint32_t> outputInterfaces) = 0;
  /**
   * \brief Remove a static multicast route for a given multicast source and
   *        group.
   *
   * \param origin The Ipv4 address of the multicast source.
   * \param group The multicast group address.
   * \param inputInterface The interface index over which the packet arrived.
   */
  virtual void RemoveMulticastRoute (Ipv4Address origin,
                                     Ipv4Address group,
                                     uint32_t inputInterface) = 0;
  
  /**
   * \brief Set the default static multicast route.
   *
   * \param outputInterface The network output interface index over which 
   *        packets without specific routes should be sent.
   */
  virtual void SetDefaultMulticastRoute (uint32_t outputInterface) = 0;

  /**
   * \returns the number of entries in the multicast routing table.
   */
  virtual uint32_t GetNMulticastRoutes (void) const = 0;

  /**
   * \param i index of route to return
   * \returns the route whose index is i
   */
  virtual Ipv4MulticastRoute GetMulticastRoute (uint32_t i) const = 0;

  /**
   * \param i index of route to remove from routing table.
   */
  virtual void RemoveMulticastRoute (uint32_t i) = 0;

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
   * \brief Find and return the interface ID of the interface that has been
   *        assigned the specified IP address.
   * \param addr The IP address assigned to the interface of interest.
   * \returns The index of the ipv4 interface with the given address.
   *
   * Each IP interface has an IP address associated with it.  It is often 
   * useful to search the list of interfaces for one that corresponds to 
   * a known IP Address.  This call takes an IP address as a parameter and
   * returns the interface index of the first interface that has been assigned
   * that address.  If the address is not found, this function asserts.
   */
  virtual uint32_t FindInterfaceForAddr (Ipv4Address addr) const = 0;

  /**
   * \brief Find and return the interface ID of the interface that has been
   *        assigned the specified (masked) IP address.
   * \param addr The IP address assigned to the interface of interest.
   * \param mask The address mask to be used in address matching.
   * \returns The index of the ipv4 interface with the given address.
   *
   * Each IP interface has an IP address associated with it.  It is often 
   * useful to search the list of interfaces for one that corresponds to 
   * a known IP Address.  This call takes an IP address and an IP address
   * mask as parameters and returns the interface index of the first interface
   * that matches the masked IP address.
   */
  virtual uint32_t FindInterfaceForAddr (Ipv4Address addr, 
    Ipv4Mask mask) const = 0;

  /**
   * \brief Find and return the interface ID of the interface that has been
   *        associated with the specified (masked) NetDevice
   * \param nd The net device of the interface of interest.
   * \returns The index of the ipv4 interface associated with the given net 
   *          device or -1 if not found.
   *
   * Each IP interface is associated with a net device.  It is often 
   * useful to search the list of interfaces for one that corresponds to 
   * a known net device.  This call takes an smart pointer to a net device
   * and returns the interface index of the first interface that matches the
   * net device.
   */
  virtual int32_t FindInterfaceForDevice(Ptr<NetDevice> nd) const = 0;

  /**
   * \param i index of ipv4 interface
   * \returns the NetDevice associated with the ipv4 interface index
   */
  virtual Ptr<NetDevice> GetNetDevice (uint32_t i) = 0;

  /**
   * \brief Join a multicast group for a given multicast source and 
   *        group.
   *
   * \param origin The Ipv4 address of the multicast source.
   * \param group The multicast group address.
   */
  virtual void JoinMulticastGroup (Ipv4Address origin, Ipv4Address group) = 0;

  /**
   * \brief Leave a multicast group for a given multicast source and 
   *        group.
   *
   * \param origin The Ipv4 address of the multicast source.
   * \param group The multicast group address.
   */
  virtual void LeaveMulticastGroup (Ipv4Address origin, Ipv4Address group) = 0;

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
   * \param metric routing metric (cost) associated to the underlying 
   *          ipv4 interface
   */
  virtual void SetMetric (uint32_t i, uint16_t metric) = 0;

  /**
   * \param i index of ipv4 interface
   * \returns routing metric (cost) associated to the underlying 
   *          ipv4 interface
   */
  virtual uint16_t GetMetric (uint32_t i) const = 0;

  /**
   * \param i index of ipv4 interface
   * \returns the address associated to the underlying ipv4 interface
   */
  virtual Ipv4Address GetAddress (uint32_t i) const = 0;

  /**
   * \param destination The IP address of a hypothetical destination.
   * \returns The IP address assigned to the interface that will be used
   * if we were to send a packet to destination.
   */
  virtual Ipv4Address GetSourceAddress (Ipv4Address destination) const = 0;

  /**
   * \param dest The IP address of a hypothetical destination.
   * \param ifIndex filled in with the interface index that will be used to
   *        send a packet to the hypothetical destination.
   * \returns true if a single interface can be identified, false otherwise.
   */
  virtual bool GetIfIndexForDestination (Ipv4Address dest,
                                         uint32_t &ifIndex) const = 0;

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

  /**
   * \brief Convenience function to return the ifIndex corresponding
   * to the Ipv4Address provided
   *
   * \param addr Ipv4Address
   * \param mask corresponding Ipv4Mask
   * \returns ifIndex corresponding to a/amask
   */
  virtual uint32_t GetIfIndexByAddress (Ipv4Address addr, 
    Ipv4Mask mask = Ipv4Mask("255.255.255.255"));
};

} // namespace ns3 

#endif /* IPV4_H */
