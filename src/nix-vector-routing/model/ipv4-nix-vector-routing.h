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

namespace ns3 {

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
   *
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
   */
  void FlushGlobalNixRoutingCache (void);

private:
  /* flushes the cache which stores nix-vector based on
   * destination IP */
  void FlushNixCache (void);

  /* flushes the cache which stores the Ipv4 route
   * based on the destination IP */
  void FlushIpv4RouteCache (void);

  /* upon a run-time topology change caches are
   * flushed and the total number of neighbors is
   * reset to zero */
  void ResetTotalNeighbors (void);

  /*  takes in the source node and dest IP and calls GetNodeByIp,
   *  BFS, accounting for any output interface specified, and finally
   *  BuildNixVector to return the built nix-vector */
  Ptr<NixVector> GetNixVector (Ptr<Node>, Ipv4Address, Ptr<NetDevice>);

  /* checks the cache based on dest IP for the nix-vector */
  Ptr<NixVector> GetNixVectorInCache (Ipv4Address);

  /* checks the cache based on dest IP for the Ipv4Route */
  Ptr<Ipv4Route> GetIpv4RouteInCache (Ipv4Address);

  /* given a net-device returns all the adjacent net-devices,
   * essentially getting the neighbors on that channel */
  void GetAdjacentNetDevices (Ptr<NetDevice>, Ptr<Channel>, NetDeviceContainer &);

  /* iterates through the node list and finds the one
   * corresponding to the given Ipv4Address */
  Ptr<Node> GetNodeByIp (Ipv4Address);

  /* Recurses the parent vector, created by BFS and actually builds the nixvector */
  bool BuildNixVector (const std::vector< Ptr<Node> > & parentVector, uint32_t source, uint32_t dest, Ptr<NixVector> nixVector);

  /* special variation of BuildNixVector for when a node is sending to itself */
  bool BuildNixVectorLocal (Ptr<NixVector> nixVector);

  /* simple iterates through the nodes net-devices and determines
   * how many neighbors it has */
  uint32_t FindTotalNeighbors (void);

  /* determine if the netdevice is bridged */
  Ptr<BridgeNetDevice> NetDeviceIsBridged (Ptr<NetDevice> nd) const;


  /* Nix index is with respect to the neighbors.  The net-device index must be
   * derived from this */
  uint32_t FindNetDeviceForNixIndex (uint32_t nodeIndex, Ipv4Address & gatewayIp);

  /* Breadth first search algorithm
   * Param1: total number of nodes
   * Param2: Source Node
   * Param3: Dest Node
   * Param4: (returned) Parent vector for retracing routes
   * Param5: specific output interface to use from source node, if not null
   * Returns: false if dest not found, true o.w.
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
  virtual void PrintRoutingTable (Ptr<OutputStreamWrapper> stream) const;


  /* cache stores nix-vectors based on destination ip */
  NixMap_t m_nixCache;

  /* cache stores Ipv4Routes based on destination ip */
  Ipv4RouteMap_t m_ipv4RouteCache;

  Ptr<Ipv4> m_ipv4;
  Ptr<Node> m_node;

  /* total neighbors used for nix-vector to determine
   * number of bits */
  uint32_t m_totalNeighbors;
};
} // namespace ns3

#endif /* IPV4_NIX_VECTOR_ROUTING_H */
