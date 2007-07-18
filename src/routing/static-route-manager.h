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
 * \brief Vertex used in shortest path first (SPF) computations
 *
 * See RFC 2328, Section 16.
 */
class SPFVertex
{
public:
  SPFVertex();
  SPFVertex(StaticRouterLSA*);
  ~SPFVertex();

  enum VertexType {
    VertexUnknown = 0,
    VertexRouter,
    VertexNetwork
  };

  void SetVertexType (VertexType type);
  VertexType GetVertexType (void) const;

  void SetVertexId (Ipv4Address idV);
  Ipv4Address GetVertexId (void) const;

  void SetLSA (StaticRouterLSA* lsa);
  StaticRouterLSA* GetLSA (void) const;

  void SetDistanceFromRoot (uint32_t distance);
  uint32_t GetDistanceFromRoot (void) const;

  void SetOutgoingInterfaceId (uint32_t id);
  uint32_t GetOutgoingInterfaceId (void) const;

  void SetNextHop (Ipv4Address nextHop);
  Ipv4Address GetNextHop (void) const;

  void SetParent (SPFVertex* parent);
  SPFVertex* GetParent (void) const;

  uint32_t GetNChildren (void) const;
  SPFVertex* GetChild (uint32_t n) const;

  uint32_t AddChild (SPFVertex* child);

private:
  VertexType m_vertexType;
  Ipv4Address m_vertexId;  // router id
  StaticRouterLSA* m_lsa;  // This pointer owns LSA for mem. mgmt purposes
  uint32_t m_distanceFromRoot;
  uint32_t m_rootOif;
  Ipv4Address m_nextHop;

  // need to keep track of current parent vertex
  SPFVertex* m_parent;

  //  m_children lists the leaves from your SPF tree
  typedef std::list<SPFVertex*> ListOfSPFVertex_t;
  ListOfSPFVertex_t m_children;
/**
 * The SPFVertex copy construction is disallowed.  There's no need for
 * it and a compiler provided shallow copy would be wrong.
 */
  SPFVertex (SPFVertex& v);
/**
 * The SPFVertex copy assignment operator is disallowed.  There's no need for
 * it and a compiler provided shallow copy would be wrong.
 */
  SPFVertex& operator= (SPFVertex& v);
};

/**
 * \brief The Link State DataBase (LSDB) of a static router
 */
class StaticRouteManagerLSDB
{
public:
  StaticRouteManagerLSDB ();
  ~StaticRouteManagerLSDB ();
  void Insert(Ipv4Address addr, StaticRouterLSA* lsa);
  StaticRouterLSA* GetLSA (Ipv4Address addr);
  /**
   * \brief Set all LSA flags to an initialized state, for SPF computation
   */
  void Initialize ();

  typedef std::map<Ipv4Address, StaticRouterLSA*> LSDBMap_t;
  typedef std::pair<Ipv4Address, StaticRouterLSA*> LSDBPair_t;
  LSDBMap_t m_database;
private:
/**
 * StaticRouteManagerLSDB copy construction is disallowed.  There's no 
 * need for it and a compiler provided shallow copy would be hopelessly wrong.
 */
  StaticRouteManagerLSDB (StaticRouteManagerLSDB& lsdb);
/**
 * The SPFVertex copy assignment operator is disallowed.  There's no need for
 * it and a compiler provided shallow copy would be wrong.
 */
  StaticRouteManagerLSDB& operator= (StaticRouteManagerLSDB& lsdb);
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
/**
 * Static Route Manager copy construction is disallowed.  There's no need for
 * it and a compiler provided shallow copy would be hopelessly wrong.
 *
 */
  StaticRouteManager (StaticRouteManager& srm);
/**
 * Static Router copy assignment operator is disallowed.  There's no need for
 * it and a compiler provided shallow copy would be hopelessly wrong.
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
