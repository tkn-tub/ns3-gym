/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 The Georgia Institute of Technology 
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
 * Authors: Josh Pelkey <jpelkey@gatech.edu>
 */

#ifndef IPV4_NIX_VECTOR_ROUTING_H
#define IPV4_NIX_VECTOR_ROUTING_H

#include <map>

#include "ns3/channel.h"
#include "ns3/node-container.h"
#include "ns3/node-list.h"
#include "ns3/net-device-container.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/ipv4-route.h"
#include "ns3/nix-vector.h"
#include "ns3/bridge-net-device.h"
#include "ns3/nstime.h"

namespace ns3 {

/**
 * \defgroup nix-vector-routing Nix-Vector Routing
 *
 * Nix-vector routing is a simulation specific routing protocol and is
 * intended for large network topologies.
 */

/**
 * \ingroup nix-vector-routing
 * Map of Ipv4Address to NixVector
 */
typedef std::map<Ipv4Address, Ptr<NixVector> > NixMap_t;
/**
 * \ingroup nix-vector-routing
 * Map of Ipv4Address to Ipv4Route
 */
typedef std::map<Ipv4Address, Ptr<Ipv4Route> > Ipv4RouteMap_t;

/**
 * \ingroup nix-vector-routing
 * Nix-vector routing protocol
 */
class Ipv4NixVectorRouting : public Ipv4RoutingProtocol
{
public:
  Ipv4NixVectorRouting ();
  ~Ipv4NixVectorRouting ();
  /**
   * @brief The Interface ID of the Global Router interface.
   * @return The Interface ID
   * @see Object::GetObject ()
   */
  static TypeId GetTypeId (void);
  /**
   * @brief Set the Node pointer of the node for which this
   * routing protocol is to be placed
   *
   * @param node Node pointer
   */
  void SetNode (Ptr<Node> node);

  /**
   * @brief Called when run-time link topology change occurs
   * which iterates through the node list and flushes any
   * nix vector caches
   *
   * \internal
   * \c const is used here due to need to potentially flush the cache
   * in const methods such as PrintRoutingTable.  Caches are stored in
   * mutable variables and flushed in const methods.
   */
  void FlushGlobalNixRoutingCache (void) const;

private:

  /**
   * Flushes the cache which stores nix-vector based on
   * destination IP
   */
  void FlushNixCache (void) const;

  /**
   * Flushes the cache which stores the Ipv4 route
   * based on the destination IP
   */
  void FlushIpv4RouteCache (void) const;

  /**
   * Upon a run-time topology change caches are
   * flushed and the total number of neighbors is
   * reset to zero
   */
  void ResetTotalNeighbors (void);

  /**
   * Takes in the source node and dest IP and calls GetNodeByIp,
   * BFS, accounting for any output interface specified, and finally
   * BuildNixVector to return the built nix-vector
   *
   * \param source Source node
   * \param dest Destination node address
   * \param oif Preferred output interface
   * \returns The NixVector to be used in routing.
   */
  Ptr<NixVector> GetNixVector (Ptr<Node> source, Ipv4Address dest, Ptr<NetDevice> oif);

  /**
   * Checks the cache based on dest IP for the nix-vector
   * \param address Address to check
   * \returns The NixVector to be used in routing.
   */
  Ptr<NixVector> GetNixVectorInCache (Ipv4Address address);

  /**
   * Checks the cache based on dest IP for the Ipv4Route
   * \param address Address to check
   * \returns The cached route.
   */
  Ptr<Ipv4Route> GetIpv4RouteInCache (Ipv4Address address);

  /**
   * Given a net-device returns all the adjacent net-devices,
   * essentially getting the neighbors on that channel
   * \param [in] netDevice the NetDevice attached to the channel.
   * \param [in] channel the channel to check
   * \param [out] netDeviceContainer the NetDeviceContainer of the NetDevices in the channel.
   */
  void GetAdjacentNetDevices (Ptr<NetDevice> netDevice, Ptr<Channel> channel, NetDeviceContainer & netDeviceContainer);

  /**
   * Iterates through the node list and finds the one
   * corresponding to the given Ipv4Address
   * \param dest destination node IP
   * \return The node with the specified IP.
   */
  Ptr<Node> GetNodeByIp (Ipv4Address dest);

  /**
   * Recurses the parent vector, created by BFS and actually builds the nixvector
   * \param [in] parentVector Parent vector for retracing routes
   * \param [in] source Source Node index
   * \param [in] dest Destination Node index
   * \param [out] nixVector the NixVector to be used for routing
   * \returns true on success, false otherwise.
   */
  bool BuildNixVector (const std::vector< Ptr<Node> > & parentVector, uint32_t source, uint32_t dest, Ptr<NixVector> nixVector);

  /**
   * Special variation of BuildNixVector for when a node is sending to itself
   * \param [out] nixVector the NixVector to be used for routing
   * \returns true on success, false otherwise.
   */
  bool BuildNixVectorLocal (Ptr<NixVector> nixVector);

  /**
   * Simple iterates through the nodes net-devices and determines
   * how many neighbors it has
   * \returns the number of neighbors.
   */
  uint32_t FindTotalNeighbors (void);

  /**
   * Determine if the NetDevice is bridged
   * \param nd the NetDevice to check
   * \returns the bridging NetDevice (or null if the NetDevice is not bridged)
   */
  Ptr<BridgeNetDevice> NetDeviceIsBridged (Ptr<NetDevice> nd) const;


  /**
   * Nix index is with respect to the neighbors.  The net-device index must be
   * derived from this
   * \param [in] nodeIndex Nix Node index
   * \param [out] gatewayIp IP address of the gateway
   * \returns the index of the NetDevice in the node.
   */
  uint32_t FindNetDeviceForNixIndex (uint32_t nodeIndex, Ipv4Address & gatewayIp);

  /**
   * \brief Breadth first search algorithm.
   * \param [in] numberOfNodes total number of nodes
   * \param [in] source Source Node
   * \param [in] dest Destination Node
   * \param [out] parentVector Parent vector for retracing routes
   * \param [in] oif specific output interface to use from source node, if not null
   * \returns false if dest not found, true o.w.
   */
  bool BFS (uint32_t numberOfNodes,
            Ptr<Node> source,
            Ptr<Node> dest,
            std::vector< Ptr<Node> > & parentVector,
            Ptr<NetDevice> oif);

  void DoDispose (void);

  /* From Ipv4RoutingProtocol */
  virtual Ptr<Ipv4Route> RouteOutput (Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr);
  virtual bool RouteInput (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev,
                           UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                           LocalDeliverCallback lcb, ErrorCallback ecb);
  virtual void NotifyInterfaceUp (uint32_t interface);
  virtual void NotifyInterfaceDown (uint32_t interface);
  virtual void NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address);
  virtual void NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address);
  virtual void SetIpv4 (Ptr<Ipv4> ipv4);
  virtual void PrintRoutingTable (Ptr<OutputStreamWrapper> stream, Time::Unit unit = Time::S) const;
 
  /**
   * Flushes routing caches if required.
   */
  void CheckCacheStateAndFlush (void) const;

  /**
   * Flag to mark when caches are dirty and need to be flushed.  
   * Used for lazy cleanup of caches when there are many topology changes.
   */
  static bool g_isCacheDirty;

  /** Cache stores nix-vectors based on destination ip */
  mutable NixMap_t m_nixCache;

  /** Cache stores Ipv4Routes based on destination ip */
  mutable Ipv4RouteMap_t m_ipv4RouteCache;

  Ptr<Ipv4> m_ipv4; //!< IPv4 object
  Ptr<Node> m_node; //!< Node object

  /** Total neighbors used for nix-vector to determine number of bits */
  uint32_t m_totalNeighbors;
};
} // namespace ns3

#endif /* IPV4_NIX_VECTOR_ROUTING_H */
