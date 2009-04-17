/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2007 University of Washington
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
 * Authors:  Craig Dowell (craigdo@ee.washington.edu)
 *           Tom Henderson (tomhend@u.washington.edu)
 */

#ifndef GLOBAL_ROUTE_MANAGER_IMPL_H
#define GLOBAL_ROUTE_MANAGER_IMPL_H

#include <stdint.h>
#include <list>
#include <queue>
#include <map>
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"
#include "ns3/node-container.h"
#include "global-router-interface.h"

namespace ns3 {

const uint32_t SPF_INFINITY = 0xffffffff;

class CandidateQueue;
class Ipv4GlobalRouting;

/**
 * @brief Vertex used in shortest path first (SPF) computations. See RFC 2328,
 * Section 16.
 *
 * Each router in the simulation is associated with an SPFVertex object.  When
 * calculating routes, each of these routers is, in turn, chosen as the "root"
 * of the calculation and routes to all of the other routers are eventually
 * saved in the routing tables of each of the chosen nodes.  Each of these 
 * routers in the calculation has an associated SPFVertex.
 *
 * The "Root" vertex is the SPFVertex representing the router that is having
 * its routing tables set.  The SPFVertex objects representing other routers
 * or networks in the simulation are arranged in the SPF tree.  It is this 
 * tree that represents the Shortest Paths to the other networks.
 *
 * Each SPFVertex has a pointer to the Global Router Link State Advertisement
 * (LSA) that its underlying router has exported.  Within these LSAs are
 * Global Router Link Records that describe the point to point links from the
 * underlying router to other nodes (represented by other SPFVertex objects)
 * in the simulation topology.  The combination of the arrangement of the 
 * SPFVertex objects in the SPF tree, along with the details of the link
 * records that connect them provide the information required to construct the
 * required routes.
 */
class SPFVertex
{
public:
/**
 * @brief Enumeration of the possible types of SPFVertex objects.
 * @internal
 *
 * Currently we use VertexRouter to identify objects that represent a router 
 * in the simulation topology, and VertexNetwork to identify objects that 
 * represent a network.
 */
  enum VertexType {
    VertexUnknown = 0,  /**< Uninitialized Link Record */
    VertexRouter,       /**< Vertex representing a router in the topology */
    VertexNetwork       /**< Vertex representing a network in the topology */
  };

/**
 * @brief Construct an empty ("uninitialized") SPFVertex (Shortest Path First 
 * Vertex).
 * @internal
 *
 * The Vertex Type is set to VertexUnknown, the Vertex ID is set to 
 * 255.255.255.255, and the distance from root is set to infinity 
 * (UINT32_MAX).  The referenced Link State Advertisement (LSA) is set to 
 * null as is the parent SPFVertex.  The outgoing interface index is set to
 * infinity, the next hop address is set to 0.0.0.0 and the list of children
 * of the SPFVertex is initialized to empty.
 *
 * @see VertexType
 */
  SPFVertex();

/**
 * @brief Construct an initialized SPFVertex (Shortest Path First Vertex).
 * @internal
 *
 * The Vertex Type is initialized to VertexRouter and the Vertex ID is found
 * from the Link State ID of the Link State Advertisement (LSA) passed as a
 * parameter.  The Link State ID is set to the Router ID of the advertising
 * router.  The referenced LSA (m_lsa) is set to the given LSA.  Other than 
 * these members, initialization is as in the default constructor.
 * of the SPFVertex is initialized to empty.
 *
 * @see SPFVertex::SPFVertex ()
 * @see VertexType
 * @see GlobalRoutingLSA
 * @param lsa The Link State Advertisement used for finding initial values.
 */
  SPFVertex(GlobalRoutingLSA* lsa);

/**
 * @brief Destroy an SPFVertex (Shortest Path First Vertex).
 * @internal
 *
 * The children vertices of the SPFVertex are recursively deleted.
 *
 * @see SPFVertex::SPFVertex ()
 */
  ~SPFVertex();

/**
 * @brief Get the Vertex Type field of a SPFVertex object.
 * @internal
 *
 * The Vertex Type describes the kind of simulation object a given SPFVertex
 * represents.
 *
 * @see VertexType
 * @returns The VertexType of the current SPFVertex object.
 */
  VertexType GetVertexType (void) const;

/**
 * @brief Set the Vertex Type field of a SPFVertex object.
 * @internal
 *
 * The Vertex Type describes the kind of simulation object a given SPFVertex
 * represents.
 *
 * @see VertexType
 * @param type The new VertexType for the current SPFVertex object.
 */
  void SetVertexType (VertexType type);

/**
 * @brief Get the Vertex ID field of a SPFVertex object.
 * @internal
 *
 * The Vertex ID uniquely identifies the simulation object a given SPFVertex
 * represents.  Typically, this is the Router ID for SPFVertex objects 
 * representing routers, and comes from the Link State Advertisement of a 
 * router aggregated to a node in the simulation.  These IDs are allocated
 * automatically by the routing environment and look like IP addresses 
 * beginning at 0.0.0.0 and monotonically increasing as new routers are
 * instantiated.
 *
 * @returns The Ipv4Address Vertex ID of the current SPFVertex object.
 */
  Ipv4Address GetVertexId (void) const;

/**
 * @brief Set the Vertex ID field of a SPFVertex object.
 * @internal
 *
 * The Vertex ID uniquely identifies the simulation object a given SPFVertex
 * represents.  Typically, this is the Router ID for SPFVertex objects 
 * representing routers, and comes from the Link State Advertisement of a 
 * router aggregated to a node in the simulation.  These IDs are allocated
 * automatically by the routing environment and look like IP addresses 
 * beginning at 0.0.0.0 and monotonically increase as new routers are
 * instantiated.  This method is an explicit override of the automatically
 * generated value.
 *
 * @param id The new Ipv4Address Vertex ID for the current SPFVertex object.
 */
  void SetVertexId (Ipv4Address id);

/**
 * @brief Get the Global Router Link State Advertisement returned by the 
 * Global Router represented by this SPFVertex during the route discovery 
 * process.
 * @internal
 *
 * @see GlobalRouter
 * @see GlobalRoutingLSA
 * @see GlobalRouter::DiscoverLSAs ()
 * @returns A pointer to the GlobalRoutingLSA found by the router represented
 * by this SPFVertex object.
 */
  GlobalRoutingLSA* GetLSA (void) const;

/**
 * @brief Set the Global Router Link State Advertisement returned by the 
 * Global Router represented by this SPFVertex during the route discovery 
 * process.
 * @internal
 *
 * @see SPFVertex::GetLSA ()
 * @see GlobalRouter
 * @see GlobalRoutingLSA
 * @see GlobalRouter::DiscoverLSAs ()
 * @warning Ownership of the LSA is transferred to the "this" SPFVertex.  You
 * must not delete the LSA after calling this method.
 * @param lsa A pointer to the GlobalRoutingLSA.
 */
  void SetLSA (GlobalRoutingLSA* lsa);

/**
 * @brief Get the distance from the root vertex to "this" SPFVertex object.
 * @internal
 *
 * Each router in the simulation is associated with an SPFVertex object.  When
 * calculating routes, each of these routers is, in turn, chosen as the "root"
 * of the calculation and routes to all of the other routers are eventually
 * saved in the routing tables of each of the chosen nodes.  Each of these 
 * routers in the calculation has an associated SPFVertex.
 *
 * The "Root" vertex is then the SPFVertex representing the router that is
 * having its routing tables set.  The "this" SPFVertex is the vertex to which
 * a route is being calculated from the root.  The distance from the root that
 * we're asking for is the number of hops from the root vertex to the vertex
 * in question.
 *
 * The distance is calculated during route discovery and is stored in a
 * member variable.  This method simply fetches that value.
 *
 * @returns The distance, in hops, from the root SPFVertex to "this" SPFVertex.
 */
  uint32_t GetDistanceFromRoot (void) const;

/**
 * @brief Set the distance from the root vertex to "this" SPFVertex object.
 * @internal
 *
 * Each router in the simulation is associated with an SPFVertex object.  When
 * calculating routes, each of these routers is, in turn, chosen as the "root"
 * of the calculation and routes to all of the other routers are eventually
 * saved in the routing tables of each of the chosen nodes.  Each of these 
 * routers in the calculation has an associated SPFVertex.
 *
 * The "Root" vertex is then the SPFVertex representing the router that is
 * having its routing tables set.  The "this" SPFVertex is the vertex to which
 * a route is being calculated from the root.  The distance from the root that
 * we're asking for is the number of hops from the root vertex to the vertex
 * in question.
 *
 * @param distance The distance, in hops, from the root SPFVertex to "this"
 * SPFVertex.
 */
  void SetDistanceFromRoot (uint32_t distance);

/**
 * @brief Get the interface ID that should be used to begin forwarding packets
 * from the root SPFVertex to "this" SPFVertex.
 * @internal
 *
 * Each router node in the simulation is associated with an SPFVertex object.
 * When calculating routes, each of these routers is, in turn, chosen as the 
 * "root" of the calculation and routes to all of the other routers are
 * eventually saved in the routing tables of each of the chosen nodes.
 *
 * The "Root" vertex is then the SPFVertex representing the router that is
 * having its routing tables set.  The "this" SPFVertex is the vertex that
 * represents the host or network to which a route is being calculated from 
 * the root.  The outgoing interface that we're asking for is the interface
 * index on the root node that should be used to start packets along the
 * path to "this" vertex.
 *
 * When initializing the root SPFVertex, the interface ID is determined by
 * examining the Global Router Link Records of the Link State Advertisement
 * generated by the root node's GlobalRouter.  These interfaces are used to 
 * forward packets off of the root's network down those links.  As other 
 * vertices are discovered which are further away from the root, they will 
 * be accessible down one of the paths begun by a Global Router Link Record.
 * 
 * To forward packets to these hosts or networks, the root node must begin
 * the forwarding process by sending the packets to the interface of that
 * first hop.  This means that the first hop address and interface ID must
 * be the same for all downstream SPFVertices.  We call this "inheriting"
 * the interface and next hop.
 *
 * In this method, the root node is asking, "which of my local interfaces
 * should I use to get a packet to the network or host represented by 'this'
 * SPFVertex."
 *
 * @see GlobalRouter
 * @see GlobalRoutingLSA
 * @see GlobalRoutingLinkRecord
 * @returns The interface index to use when forwarding packets to the host
 * or network represented by "this" SPFVertex.
 */
  uint32_t GetOutgoingInterfaceId (void) const;

/**
 * @brief Set the interface ID that should be used to begin forwarding packets
 * from the root SPFVertex to "this" SPFVertex.
 * @internal
 *
 * Each router node in the simulation is associated with an SPFVertex object.
 * When calculating routes, each of these routers is, in turn, chosen as the 
 * "root" of the calculation and routes to all of the other routers are
 * eventually saved in the routing tables of each of the chosen nodes.
 *
 * The "Root" vertex is then the SPFVertex representing the router that is
 * having its routing tables set.  The "this" SPFVertex is the vertex that
 * represents the host or network to which a route is being calculated from 
 * the root.  The outgoing interface that we're asking for is the interface
 * index on the root node that should be used to start packets along the
 * path to "this" vertex.
 *
 * When initializing the root SPFVertex, the interface ID is determined by
 * examining the Global Router Link Records of the Link State Advertisement
 * generated by the root node's GlobalRouter.  These interfaces are used to 
 * forward packets off of the root's network down those links.  As other 
 * vertices are discovered which are further away from the root, they will 
 * be accessible down one of the paths begun by a Global Router Link Record.
 * 
 * To forward packets to these hosts or networks, the root node must begin
 * the forwarding process by sending the packets to the interface of that
 * first hop.  This means that the first hop address and interface ID must
 * be the same for all downstream SPFVertices.  We call this "inheriting"
 * the interface and next hop.
 *
 * In this method, we are letting the root node know which of its local
 * interfaces it should use to get a packet to the network or host represented
 * by "this" SPFVertex.
 *
 * @see GlobalRouter
 * @see GlobalRoutingLSA
 * @see GlobalRoutingLinkRecord
 * @param id The interface index to use when forwarding packets to the host or
 * network represented by "this" SPFVertex.
 */
  void SetOutgoingInterfaceId (uint32_t id);

/**
 * @brief Get the IP address that should be used to begin forwarding packets 
 * from the root SPFVertex to "this" SPFVertex.
 * @internal
 *
 * Each router node in the simulation is associated with an SPFVertex object.
 * When calculating routes, each of these routers is, in turn, chosen as the 
 * "root" of the calculation and routes to all of the other routers are
 * eventually saved in the routing tables of each of the chosen nodes.
 *
 * The "Root" vertex is then the SPFVertex representing the router that is
 * having its routing tables set.  The "this" SPFVertex is the vertex that
 * represents the host or network to which a route is being calculated from 
 * the root.  The IP address that we're asking for is the address on the 
 * remote side of a link off of the root node that should be used as the
 * destination for packets along the path to "this" vertex.
 *
 * When initializing the root SPFVertex, the IP address used when forwarding
 * packets is determined by examining the Global Router Link Records of the
 * Link State Advertisement generated by the root node's GlobalRouter.  This
 * address is used to forward packets off of the root's network down those
 * links.  As other vertices / nodes are discovered which are further away
 * from the root, they will be accessible down one of the paths via a link
 * described by one of these Global Router Link Records.
 * 
 * To forward packets to these hosts or networks, the root node must begin
 * the forwarding process by sending the packets to a first hop router down
 * an interface.  This means that the first hop address and interface ID must
 * be the same for all downstream SPFVertices.  We call this "inheriting"
 * the interface and next hop.
 *
 * In this method, the root node is asking, "which router should I send a
 * packet to in order to get that packet to the network or host represented 
 * by 'this' SPFVertex."
 *
 * @see GlobalRouter
 * @see GlobalRoutingLSA
 * @see GlobalRoutingLinkRecord
 * @returns The IP address to use when forwarding packets to the host
 * or network represented by "this" SPFVertex.
 */
  Ipv4Address GetNextHop (void) const;

/**
 * @brief Set the IP address that should be used to begin forwarding packets 
 * from the root SPFVertex to "this" SPFVertex.
 * @internal
 *
 * Each router node in the simulation is associated with an SPFVertex object.
 * When calculating routes, each of these routers is, in turn, chosen as the 
 * "root" of the calculation and routes to all of the other routers are
 * eventually saved in the routing tables of each of the chosen nodes.
 *
 * The "Root" vertex is then the SPFVertex representing the router that is
 * having its routing tables set.  The "this" SPFVertex is the vertex that
 * represents the host or network to which a route is being calculated from 
 * the root.  The IP address that we're asking for is the address on the 
 * remote side of a link off of the root node that should be used as the
 * destination for packets along the path to "this" vertex.
 *
 * When initializing the root SPFVertex, the IP address used when forwarding
 * packets is determined by examining the Global Router Link Records of the
 * Link State Advertisement generated by the root node's GlobalRouter.  This
 * address is used to forward packets off of the root's network down those
 * links.  As other vertices / nodes are discovered which are further away
 * from the root, they will be accessible down one of the paths via a link
 * described by one of these Global Router Link Records.
 * 
 * To forward packets to these hosts or networks, the root node must begin
 * the forwarding process by sending the packets to a first hop router down
 * an interface.  This means that the first hop address and interface ID must
 * be the same for all downstream SPFVertices.  We call this "inheriting"
 * the interface and next hop.
 *
 * In this method we are telling the root node which router it should send
 * should I send a packet to in order to get that packet to the network or
 * host represented by 'this' SPFVertex."
 *
 * @see GlobalRouter
 * @see GlobalRoutingLSA
 * @see GlobalRoutingLinkRecord
 * @param nextHop The IP address to use when forwarding packets to the host
 * or network represented by "this" SPFVertex.
 */
  void SetNextHop (Ipv4Address nextHop);

/**
 * @brief Get a pointer to the SPFVector that is the parent of "this" 
 * SPFVertex.
 * @internal
 *
 * Each router node in the simulation is associated with an SPFVertex object.
 * When calculating routes, each of these routers is, in turn, chosen as the 
 * "root" of the calculation and routes to all of the other routers are
 * eventually saved in the routing tables of each of the chosen nodes.
 *
 * The "Root" vertex is then the SPFVertex representing the router that is
 * having its routing tables set and is the root of the SPF tree.
 *
 * This method returns a pointer to the parent node of "this" SPFVertex
 * (both of which reside in that SPF tree).
 *
 * @returns A pointer to the SPFVertex that is the parent of "this" SPFVertex
 * in the SPF tree.
 */
  SPFVertex* GetParent (void) const;

/**
 * @brief Set the pointer to the SPFVector that is the parent of "this" 
 * SPFVertex.
 * @internal
 *
 * Each router node in the simulation is associated with an SPFVertex object.
 * When calculating routes, each of these routers is, in turn, chosen as the 
 * "root" of the calculation and routes to all of the other routers are
 * eventually saved in the routing tables of each of the chosen nodes.
 *
 * The "Root" vertex is then the SPFVertex representing the router that is
 * having its routing tables set and is the root of the SPF tree.
 *
 * This method sets the parent pointer of "this" SPFVertex (both of which
 * reside in that SPF tree).
 *
 * @param parent A pointer to the SPFVertex that is the parent of "this" 
 * SPFVertex* in the SPF tree.
 */
  void SetParent (SPFVertex* parent);

/**
 * @brief Get the number of children of "this" SPFVertex.
 * @internal
 *
 * Each router node in the simulation is associated with an SPFVertex object.
 * When calculating routes, each of these routers is, in turn, chosen as the 
 * "root" of the calculation and routes to all of the other routers are
 * eventually saved in the routing tables of each of the chosen nodes.
 *
 * The "Root" vertex is then the SPFVertex representing the router that is
 * having its routing tables set and is the root of the SPF tree.  Each vertex
 * in the SPF tree can have a number of children that represent host or 
 * network routes available via that vertex.
 *
 * This method returns the number of children of "this" SPFVertex (which 
 * reside in the SPF tree).
 *
 * @returns The number of children of "this" SPFVertex (which reside in the
 * SPF tree).
 */
  uint32_t GetNChildren (void) const;

/**
 * @brief Get a borrowed SPFVertex pointer to the specified child of "this" 
 * SPFVertex.
 * @internal
 *
 * Each router node in the simulation is associated with an SPFVertex object.
 * When calculating routes, each of these routers is, in turn, chosen as the 
 * "root" of the calculation and routes to all of the other routers are
 * eventually saved in the routing tables of each of the chosen nodes.
 *
 * The "Root" vertex is then the SPFVertex representing the router that is
 * having its routing tables set and is the root of the SPF tree.  Each vertex
 * in the SPF tree can have a number of children that represent host or 
 * network routes available via that vertex.
 *
 * This method the number of children of "this" SPFVertex (which reside in
 * the SPF tree.
 *
 * @see SPFVertex::GetNChildren
 * @param n The index (from 0 to the number of children minus 1) of the 
 * child SPFVertex to return.
 * @warning The pointer returned by GetChild () is a borrowed pointer.  You
 * do not have any ownership of the underlying object and must not delete
 * that object.
 * @returns A pointer to the specified child SPFVertex (which resides in the
 * SPF tree).
 */
  SPFVertex* GetChild (uint32_t n) const;

/**
 * @brief Get a borrowed SPFVertex pointer to the specified child of "this" 
 * SPFVertex.
 * @internal
 *
 * Each router node in the simulation is associated with an SPFVertex object.
 * When calculating routes, each of these routers is, in turn, chosen as the 
 * "root" of the calculation and routes to all of the other routers are
 * eventually saved in the routing tables of each of the chosen nodes.
 *
 * The "Root" vertex is then the SPFVertex representing the router that is
 * having its routing tables set and is the root of the SPF tree.  Each vertex
 * in the SPF tree can have a number of children that represent host or 
 * network routes available via that vertex.
 *
 * This method the number of children of "this" SPFVertex (which reside in
 * the SPF tree.
 *
 * @see SPFVertex::GetNChildren
 * @warning Ownership of the pointer added to the children of "this" 
 * SPFVertex is transferred to the "this" SPFVertex.  You must not delete the
 * (now) child SPFVertex after calling this method.
 * @param child A pointer to the SPFVertex (which resides in the SPF tree) to
 * be added to the list of children of "this" SPFVertex.
 * @returns The number of children of "this" SPFVertex after the addition of
 * the new child.
 */
  uint32_t AddChild (SPFVertex* child);

  /**
   * @brief Set the value of the VertexProcessed flag
   *
   * Flag to note whether vertex has been processed in stage two of 
   * SPF computation
   * @param value boolean value to set the flag
   */ 
  void SetVertexProcessed (bool value);

  /**
   * @brief Check the value of the VertexProcessed flag
   *
   * Flag to note whether vertex has been processed in stage two of 
   * SPF computation
   * @returns value of underlying flag
   */ 
  bool IsVertexProcessed (void) const;
private:
  VertexType m_vertexType;
  Ipv4Address m_vertexId;
  GlobalRoutingLSA* m_lsa;
  uint32_t m_distanceFromRoot;
  uint32_t m_rootOif;
  Ipv4Address m_nextHop;
  SPFVertex* m_parent;
  typedef std::list<SPFVertex*> ListOfSPFVertex_t;
  ListOfSPFVertex_t m_children;
  bool m_vertexProcessed; 

/**
 * @brief The SPFVertex copy construction is disallowed.  There's no need for
 * it and a compiler provided shallow copy would be wrong.
 */
  SPFVertex (SPFVertex& v);

/**
 * @brief The SPFVertex copy assignment operator is disallowed.  There's no 
 * need for it and a compiler provided shallow copy would be wrong.
 */
  SPFVertex& operator= (SPFVertex& v);
};

/**
 * @brief The Link State DataBase (LSDB) of the Global Route Manager.
 *
 * Each node in the simulation participating in global routing has a
 * GlobalRouter interface.  The primary job of this interface is to export
 * Global Router Link State Advertisements (LSAs).  These advertisements in
 * turn contain a number of Global Router Link Records that describe the 
 * point to point links from the underlying node to other nodes (that will 
 * also export their own LSAs.
 *
 * This class implements a searchable database of LSAs gathered from every
 * router in the simulation.
 */
class GlobalRouteManagerLSDB
{
public:
/**
 * @brief Construct an empty Global Router Manager Link State Database.
 * @internal
 *
 * The database map composing the Link State Database is initialized in
 * this constructor.
 */
  GlobalRouteManagerLSDB ();

/**
 * @brief Destroy an empty Global Router Manager Link State Database.
 * @internal
 *
 * The database map is walked and all of the Link State Advertisements stored
 * in the database are freed; then the database map itself is clear ()ed to
 * release any remaining resources.
 */
  ~GlobalRouteManagerLSDB ();

/**
 * @brief Insert an IP address / Link State Advertisement pair into the Link
 * State Database.
 * @internal
 *
 * The IPV4 address and the GlobalRoutingLSA given as parameters are converted
 * to an STL pair and are inserted into the database map.
 *
 * @see GlobalRoutingLSA
 * @see Ipv4Address
 * @param addr The IP address associated with the LSA.  Typically the Router 
 * ID.
 * @param lsa A pointer to the Link State Advertisement for the router.
 */
  void Insert(Ipv4Address addr, GlobalRoutingLSA* lsa);

/**
 * @brief Look up the Link State Advertisement associated with the given
 * link state ID (address).
 * @internal
 *
 * The database map is searched for the given IPV4 address and corresponding
 * GlobalRoutingLSA is returned.
 *
 * @see GlobalRoutingLSA
 * @see Ipv4Address
 * @param addr The IP address associated with the LSA.  Typically the Router 
 * ID.
 * @returns A pointer to the Link State Advertisement for the router specified
 * by the IP address addr.
 */
  GlobalRoutingLSA* GetLSA (Ipv4Address addr) const;
/**
 * @brief Look up the Link State Advertisement associated with the given
 * link state ID (address).  This is a variation of the GetLSA call
 * to allow the LSA to be found by matching addr with the LinkData field
 * of the TransitNetwork link record.
 * @internal
 *
 * @see GetLSA
 * @param addr The IP address associated with the LSA.  Typically the Router 
 * @returns A pointer to the Link State Advertisement for the router specified
 * by the IP address addr.
 * ID.
 */
  GlobalRoutingLSA* GetLSAByLinkData (Ipv4Address addr) const;

/**
 * @brief Set all LSA flags to an initialized state, for SPF computation
 * @internal
 *
 * This function walks the database and resets the status flags of all of the
 * contained Link State Advertisements to LSA_SPF_NOT_EXPLORED.  This is done
 * prior to each SPF calculation to reset the state of the SPFVertex structures
 * that will reference the LSAs during the calculation.
 *
 * @see GlobalRoutingLSA
 * @see SPFVertex
 */
  void Initialize ();

private:
  typedef std::map<Ipv4Address, GlobalRoutingLSA*> LSDBMap_t;
  typedef std::pair<Ipv4Address, GlobalRoutingLSA*> LSDBPair_t;

  LSDBMap_t m_database;
/**
 * @brief GlobalRouteManagerLSDB copy construction is disallowed.  There's no 
 * need for it and a compiler provided shallow copy would be wrong.
 */
  GlobalRouteManagerLSDB (GlobalRouteManagerLSDB& lsdb);

/**
 * @brief The SPFVertex copy assignment operator is disallowed.  There's no 
 * need for it and a compiler provided shallow copy would be wrong.
 */
  GlobalRouteManagerLSDB& operator= (GlobalRouteManagerLSDB& lsdb);
};

/**
 * @brief A global router implementation.
 *
 * This singleton object can query interface each node in the system
 * for a GlobalRouter interface.  For those nodes, it fetches one or
 * more Link State Advertisements and stores them in a local database.
 * Then, it can compute shortest paths on a per-node basis to all routers, 
 * and finally configure each of the node's forwarding tables.
 *
 * The design is guided by OSPFv2 RFC 2328 section 16.1.1 and quagga ospfd.
 */
class GlobalRouteManagerImpl
{
public:
  GlobalRouteManagerImpl ();
  virtual ~GlobalRouteManagerImpl ();
/**
 * @brief Delete all static routes on all nodes that have a
 * GlobalRouterInterface
 *
 * TODO:  separate manually assigned static routes from static routes that
 * the global routing code injects, and only delete the latter
 * @internal
 *
 */
  virtual void DeleteGlobalRoutes ();
/**
 * @brief Select which nodes in the system are to be router nodes and 
 * aggregate the appropriate interfaces onto those nodes.
 * @internal
 *
 */
  virtual void SelectRouterNodes ();

/**
 * @brief Select which nodes in the system are to be router nodes and 
 * aggregate the appropriate interfaces onto those nodes.
 * @internal
 *
 */
  virtual void SelectRouterNodes (NodeContainer c);

/**
 * @brief Build the routing database by gathering Link State Advertisements
 * from each node exporting a GlobalRouter interface.
 * @internal
 */
  virtual void BuildGlobalRoutingDatabase ();

/**
 * @brief Compute routes using a Dijkstra SPF computation and populate
 * per-node forwarding tables
 * @internal
 */
  virtual void InitializeRoutes ();

/**
 * @brief Debugging routine; allow client code to supply a pre-built LSDB
 * @internal
 */
  void DebugUseLsdb (GlobalRouteManagerLSDB*);

/**
 * @brief Debugging routine; call the core SPF from the unit tests
 * @internal
 */
  void DebugSPFCalculate (Ipv4Address root);

private:
/**
 * @brief GlobalRouteManagerImpl copy construction is disallowed.
 * There's no  need for it and a compiler provided shallow copy would be 
 * wrong.
 */
  GlobalRouteManagerImpl (GlobalRouteManagerImpl& srmi);

/**
 * @brief Global Route Manager Implementation assignment operator is
 * disallowed.  There's no  need for it and a compiler provided shallow copy
 * would be hopelessly wrong.
 */
  GlobalRouteManagerImpl& operator= (GlobalRouteManagerImpl& srmi);

  SPFVertex* m_spfroot;
  GlobalRouteManagerLSDB* m_lsdb;
  void SPFCalculate (Ipv4Address root);
  void SPFProcessStubs (SPFVertex* v);
  void SPFNext (SPFVertex*, CandidateQueue&);
  int SPFNexthopCalculation (SPFVertex* v, SPFVertex* w, 
    GlobalRoutingLinkRecord* l, uint32_t distance);
  void SPFVertexAddParent (SPFVertex* v);
  GlobalRoutingLinkRecord* SPFGetNextLink (SPFVertex* v, SPFVertex* w, 
    GlobalRoutingLinkRecord* prev_link);
  void SPFIntraAddRouter (SPFVertex* v);
  void SPFIntraAddTransit (SPFVertex* v);
  void SPFIntraAddStub (GlobalRoutingLinkRecord *l, SPFVertex* v);
  uint32_t FindOutgoingInterfaceId (Ipv4Address a, 
    Ipv4Mask amask = Ipv4Mask("255.255.255.255"));

  // Local cache of the Ipv4GlobalRouting objects, indexed by nodeId
  typedef std::list< std::pair< uint32_t, Ptr<Ipv4GlobalRouting> > > Ipv4GlobalRoutingList;
  void AddGlobalRoutingProtocol (uint32_t nodeId, Ptr<Ipv4GlobalRouting> proto);
  Ptr<Ipv4GlobalRouting> GetGlobalRoutingProtocol (uint32_t nodeId);
  Ipv4GlobalRoutingList m_routingProtocols;
};

} // namespace ns3

#endif /* GLOBAL_ROUTE_MANAGER_IMPL_H */
