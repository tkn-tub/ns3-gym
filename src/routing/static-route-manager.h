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
#include <map>
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"
#include "static-router.h"

namespace ns3 {

const uint32_t SPF_INFINITY = 0xffffffff;

/**
 * \brief Vertex used in shortest path first (SPF) computations
 *
 * See RFC 2328, Section 16.
 */
class SPFVertex
{
public:
  SPFVertex();
  ~SPFVertex();
  void Initialize ();

  enum VertexType {
    VertexUnknown = 0,
    VertexRouter,
    VertexNetwork
  } m_vertexType;

  Ipv4Address m_vertexId;

  StaticRouterLSA* m_lsa;  // This pointer owns LSA for mem. mgmt purposes

  typedef std::list<SPFVertex> type_listOfSPFVertex;
  type_listOfSPFVertex m_parents;
  type_listOfSPFVertex m_children;
  type_listOfSPFVertex::iterator m_iter;

  uint32_t m_distanceFromRoot;

  bool m_stat;  // true if LSA is in SPF tree already
};

/**
 * \brief The Link State DataBase (LSDB) of a static router
 */
class StaticRouteManagerLSDB
{
public:
  ~StaticRouteManagerLSDB ();
  void Insert(Ipv4Address addr, SPFVertex* vertex);
  void Insert(Ipv4Address addr, StaticRouterLSA* lsa);
  SPFVertex* GetVertex (Ipv4Address addr);
  /**
   * \brief Set all SPFVertex to an initialized state, for SPF computation
   */
  void Initialize ();

  typedef std::map<Ipv4Address, SPFVertex*> LSDBMap_t;
  typedef std::pair<Ipv4Address, SPFVertex*> LSDBPair_t;
  LSDBMap_t m_database;
};

/**
 * \brief A global static router
 *
 * This singleton object can query interface each node in the system
 * for a StaticRouter interface.  For those nodes, it fetches one or
 * more LSAs and stores them in a local database.  Then, it
 * can compute shortest paths on a per-node basis to all routers, and
 * finally configure each of the node's forwarding tables.
 *
 * The design is guided by OSPFv2 RFC 2328 section 16.1.1
 * and quagga ospfd
 */
class StaticRouteManager : public Object
{
public:
  static const InterfaceId iid;
  StaticRouteManager ();
  /**
   * \brief Build routing database by gathering LSA from each routing node
   */
  virtual void BuildStaticRoutingDatabase();
  /**
   * \brief Compute routes using Dijkstra SPF computation, and populate
   *        per-node forwarding tables
   */
  virtual void InitializeRoutes();

  /**
   * \brief Debugging routine; allow client code to supply a pre-built LSDB
   */
  void DebugUseLsdb (StaticRouteManagerLSDB*);
  /**
   * \brief Debugging routine; call the core SPF from the unit tests
   */
  void DebugSPFCalculate (Ipv4Address root);

  virtual ~StaticRouteManager ();

protected:

private:
  StaticRouteManagerLSDB* m_lsdb;
  void SPFCalculate (Ipv4Address root);
  void SPFNext (SPFVertex*/*,candidate */);
};

} // namespace ns3

#endif /* STATIC_ROUTE_MANAGER_H */
