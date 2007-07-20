/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */
#ifndef STATIC_ROUTE_MANAGER_H
#define STATIC_ROUTE_MANAGER_H

#include <stdint.h>
#include <list>
#include <queue>
#include <map>
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"
#include "static-router.h"

namespace ns3 {

const uint32_t SPF_INFINITY = 0xffffffff;

class CandidateQueue;

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
 * Each SPFVertex has a pointer to the Static Router Link State Advertisement
 * (LSA) that its underlying router has exported.  Within these LSAs are
 * Static Router Link Records that describe the point to point links from the
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
 * @enum Enumeration of the possible types of SPFVertex objects.  Currently 
 * we use VertexRouter to identify objects that represent a router in the 
 * simulation topology, and VertexNetwork to identify objects that represent
 * a network.
 */
  enum VertexType {
    VertexUnknown = 0,  /**< Uninitialized Link Record */
    VertexRouter,       /**< Vertex representing a router in the topology */
    VertexNetwork       /**< Vertex representing a network in the topology */
  };
/**
 * @brief Construct an empty ("uninitialized") SPFVertex (Shortest Path First 
 * Vertex).
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
 * @see StaticRouterLSA
 * @param lsa The Link State Advertisement used for finding initial values.
 */
  SPFVertex(StaticRouterLSA* lsa);
/**
 * @brief Destroy an SPFVertex (Shortest Path First Vertex).
 *
 * The children vertices of the SPFVertex are recursively deleted.
 *
 * @see SPFVertex::SPFVertex ()
 */
  ~SPFVertex();
/**
 * @brief Get the Vertex Type field of a SPFVertex object.
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
 *
 * The Vertex Type describes the kind of simulation object a given SPFVertex
 * represents.
 *
 * @see VertexType
 * @param The new VertexType for the current SPFVertex object.
 */
  void SetVertexType (VertexType type);
/**
 * @brief Get the Vertex ID field of a SPFVertex object.
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
 *
 * The Vertex ID uniquely identifies the simulation object a given SPFVertex
 * represents.  Typically, this is the Router ID for SPFVertex objects 
 * representing routers, and comes from the Link State Advertisement of a 
 * router aggregated to a node in the simulation.  These IDs are allocated
 * automatically by the routing environment and look like IP addresses 
 * beginning at 0.0.0.0 and monotonically increasing as new routers are
 * instantiated.
 *
 * @param id The new Ipv4Address Vertex ID for the current SPFVertex object.
 */
  void SetVertexId (Ipv4Address id);
/**
 * @brief Get the Static Router Link State Advertisement returned by the 
 * Static Router represented by this SPFVertex during the route discovery 
 * process.
 *
 * @see StaticRouter
 * @see StaticRouterLSA
 * @see StaticRouter::DiscoverLSAs ()
 * @returns A pointer to the StaticRouterLSA found by the router represented
 * by this SPFVertex object.
 */
  StaticRouterLSA* GetLSA (void) const;
/**
 * @brief Set the Static Router Link State Advertisement returned by the 
 * Static Router represented by this SPFVertex during the route discovery 
 * process.
 *
 * @see SPFVertex::GetLSA ()
 * @see StaticRouter
 * @see StaticRouterLSA
 * @see StaticRouter::DiscoverLSAs ()
 * @warning Ownership of the LSA is transferred to the "this" SPFVertex.  You
 * must not delete the LSA after calling this method.
 * @param A pointer to the StaticRouterLSA.
 */
  void SetLSA (StaticRouterLSA* lsa);
/**
 * @brief Get the distance from the root vertex to "this" SPFVertex object.
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
 * examining the Static Router Link Records of the Link State Advertisement
 * generated by the root node's StaticRouter.  These interfaces are used to 
 * forward packets off of the root's network down those links.  As other 
 * vertices are discovered which are further away from the root, they will 
 * be accessible down one of the paths begun by a Static Router Link Record.
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
 * @see StaticRouter
 * @see StaticRouterLSA
 * @see StaticRouterLinkRecord
 * @returns The interface index to use when forwarding packets to the host
 * or network represented by "this" SPFVertex.
 */
  uint32_t GetOutgoingInterfaceId (void) const;
/**
 * @brief Set the interface ID that should be used to begin forwarding packets
 * from the root SPFVertex to "this" SPFVertex.
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
 * examining the Static Router Link Records of the Link State Advertisement
 * generated by the root node's StaticRouter.  These interfaces are used to 
 * forward packets off of the root's network down those links.  As other 
 * vertices are discovered which are further away from the root, they will 
 * be accessible down one of the paths begun by a Static Router Link Record.
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
 * @see StaticRouter
 * @see StaticRouterLSA
 * @see StaticRouterLinkRecord
 * @param id The interface index to use when forwarding packets to the host or
 * network represented by "this" SPFVertex.
 */
  void SetOutgoingInterfaceId (uint32_t id);
/**
 * @brief Get the IP address that should be used to begin forwarding packets 
 * from the root SPFVertex to "this" SPFVertex.
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
 * packets is determined by examining the Static Router Link Records of the
 * Link State Advertisement generated by the root node's StaticRouter.  This
 * address is used to forward packets off of the root's network down those
 * links.  As other vertices / nodes are discovered which are further away
 * from the root, they will be accessible down one of the paths via a link
 * described by one of these Static Router Link Records.
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
 * @see StaticRouter
 * @see StaticRouterLSA
 * @see StaticRouterLinkRecord
 * @returns The IP address to use when forwarding packets to the host
 * or network represented by "this" SPFVertex.
 */
  Ipv4Address GetNextHop (void) const;
/**
 * @brief Set the IP address that should be used to begin forwarding packets 
 * from the root SPFVertex to "this" SPFVertex.
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
 * packets is determined by examining the Static Router Link Records of the
 * Link State Advertisement generated by the root node's StaticRouter.  This
 * address is used to forward packets off of the root's network down those
 * links.  As other vertices / nodes are discovered which are further away
 * from the root, they will be accessible down one of the paths via a link
 * described by one of these Static Router Link Records.
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
 * @see StaticRouter
 * @see StaticRouterLSA
 * @see StaticRouterLinkRecord
 * @param nextHop The IP address to use when forwarding packets to the host
 * or network represented by "this" SPFVertex.
 */
  void SetNextHop (Ipv4Address nextHop);
/**
 * @brief Get a pointer to the SPFVector that is the parent of "this" 
 * SPFVertex.
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
 * @warning Ownership of the pointer added to the children of "this" 
 * SPFVertex is transferred to the "this" SPFVertex.  You must not delete the
 * (now) child SPFVertex after calling this method.
 * @param child A pointer to the SPFVertex (which resides in the SPF tree) to
 * be added to the list of children of "this" SPFVertex.
 * @returns The number of children of "this" SPFVertex after the addition of
 * the new child.
 */
  uint32_t AddChild (SPFVertex* child);

private:
  VertexType m_vertexType;
  Ipv4Address m_vertexId;
  StaticRouterLSA* m_lsa;
  uint32_t m_distanceFromRoot;
  uint32_t m_rootOif;
  Ipv4Address m_nextHop;
  SPFVertex* m_parent;
  typedef std::list<SPFVertex*> ListOfSPFVertex_t;
  ListOfSPFVertex_t m_children;
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
 * @brief The Link State DataBase (LSDB) of the Static Route Manager.
 *
 * Each node in the simulation participating in static routing has a
 * StaticRouter interface.  The primary job of this interface is to export
 * Static Router Link State Advertisements (LSAs).  These advertisements in
 * turn contain a number of Static Router Link Records that describe the 
 * point to point links from the underlying node to other nodes (that will 
 * also export their own LSAs.
 *
 * This class implements a searchable database of LSAs gathered from every
 * router in the simulation.
 */
class StaticRouteManagerLSDB
{
public:
/**
 * @brief Construct an empty Static Router Manager Link State Database.
 *
 * The database map composing the Link State Database is initialized in
 * this constructor.
 */
  StaticRouteManagerLSDB ();
/**
 * @brief Destroy an empty Static Router Manager Link State Database.
 *
 * The database map is walked and all of the Link State Advertisements stored
 * in the database are freed; then the database map itself is clear ()ed to
 * release any remaining resources.
 */
  ~StaticRouteManagerLSDB ();
/**
 * @brief Insert an IP address / Link State Advertisement pair into the Link
 * State Database.
 *
 * The IPV4 address and the StaticRouterLSA given as parameters are converted
 * to an STL pair and are inserted into the database map.
 *
 * @see StaticRouterLSA
 * @see Ipv4Address
 * @param addr The IP address associated with the LSA.  Typically the Router 
 * ID.
 * @param lsa A pointer to the Link State Advertisement for the router.
 */
  void Insert(Ipv4Address addr, StaticRouterLSA* lsa);
/**
 * @brief Look up the Link State Advertisement associated with the given
 * IP Address.
 *
 * The database map is searched for the given IPV4 address and corresponding
 * StaticRouterLSA is returned.
 *
 * @see StaticRouterLSA
 * @see Ipv4Address
 * @param addr The IP address associated with the LSA.  Typically the Router 
 * ID.
 * @returns A pointer to the Link State Advertisement for the router specified
 * by the IP address addr.
 */
  StaticRouterLSA* GetLSA (Ipv4Address addr) const;
/**
 * @brief Set all LSA flags to an initialized state, for SPF computation
 *
 * This function walks the database and resets the status flags of all of the
 * contained Link State Advertisements to LSA_SPF_NOT_EXPLORED.  This is done
 * prior to each SPF calculation to reset the state of the SPFVertex structures
 * that will reference the LSAs during the calculation.
 *
 * @see StaticRouterLSA
 * @see SPFVertex
 */
  void Initialize ();

private:
  typedef std::map<Ipv4Address, StaticRouterLSA*> LSDBMap_t;
  typedef std::pair<Ipv4Address, StaticRouterLSA*> LSDBPair_t;

  LSDBMap_t m_database;
/**
 * @brief StaticRouteManagerLSDB copy construction is disallowed.  There's no 
 * need for it and a compiler provided shallow copy would be hopelessly wrong.
 */
  StaticRouteManagerLSDB (StaticRouteManagerLSDB& lsdb);
/**
 * @brief The SPFVertex copy assignment operator is disallowed.  There's no 
 * need for it and a compiler provided shallow copy would be wrong.
 */
  StaticRouteManagerLSDB& operator= (StaticRouteManagerLSDB& lsdb);
};

/**
 * @brief A global static router
 *
 * This singleton object can query interface each node in the system
 * for a StaticRouter interface.  For those nodes, it fetches one or
 * more Link State Advertisements and stores them in a local database.
 * Then, it can compute shortest paths on a per-node basis to all routers, 
 * and finally configure each of the node's forwarding tables.
 *
 * The design is guided by OSPFv2 RFC 2328 section 16.1.1 and quagga ospfd.
 */
class StaticRouteManager : public Object
{
public:
  static const InterfaceId iid;
  StaticRouteManager ();
/**
 * @brief Build the routing database by gathering Link State Advertisements
 * from each node exporting a StaticRouter interface.
 *
 */
  virtual void BuildStaticRoutingDatabase();
/**
 * @brief Compute routes using a Dijkstra SPF computation and populate
 * per-node forwarding tables
 */
  virtual void InitializeRoutes();
/**
 * @brief Debugging routine; allow client code to supply a pre-built LSDB
 */
  void DebugUseLsdb (StaticRouteManagerLSDB*);
/**
 * @brief Debugging routine; call the core SPF from the unit tests
 */
  void DebugSPFCalculate (Ipv4Address root);

  virtual ~StaticRouteManager ();

protected:

private:
/**
 * @brief Static Route Manager copy construction is disallowed.  There's no 
 * need for it and a compiler provided shallow copy would be hopelessly wrong.
 *
 */
  StaticRouteManager (StaticRouteManager& srm);
/**
 * @brief Static Router copy assignment operator is disallowed.  There's no 
 * need for it and a compiler provided shallow copy would be hopelessly wrong.
 */
  StaticRouteManager& operator= (StaticRouteManager& srm);

  SPFVertex* m_spfroot;
  StaticRouteManagerLSDB* m_lsdb;
  void SPFCalculate (Ipv4Address root);
  void SPFNext (SPFVertex*, CandidateQueue&);
  int SPFNexthopCalculation (SPFVertex* v, SPFVertex* w, 
    StaticRouterLinkRecord* l, uint32_t distance);
  void SPFVertexAddParent(SPFVertex* v);
  StaticRouterLinkRecord* SPFGetNextLink(SPFVertex* v, SPFVertex* w, 
    StaticRouterLinkRecord* prev_link);
  void SPFIntraAddRouter(SPFVertex* v);
  uint32_t FindOutgoingInterfaceId(Ipv4Address a);
};

} // namespace ns3

#endif /* STATIC_ROUTE_MANAGER_H */
