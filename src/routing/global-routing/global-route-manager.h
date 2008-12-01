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

#ifndef GLOBAL_ROUTE_MANAGER_H
#define GLOBAL_ROUTE_MANAGER_H

#include "ns3/node-container.h"

namespace ns3 {

/**
 * @brief A global global router
 *
 * This singleton object can query interface each node in the system
 * for a GlobalRouter interface.  For those nodes, it fetches one or
 * more Link State Advertisements and stores them in a local database.
 * Then, it can compute shortest paths on a per-node basis to all routers, 
 * and finally configure each of the node's forwarding tables.
 *
 * The design is guided by OSPFv2 RFC 2328 section 16.1.1 and quagga ospfd.
 */
class GlobalRouteManager
{
public:
/**
 * @brief Build a routing database and initialize the routing tables of
 * the nodes in the simulation.  Makes all nodes in the simulation into
 * routers.
 *
 * All this function does is call the three functions
 * SelectRouterNodes (), BuildGlobalRoutingDatabase (), and
 * InitializeRoutes ().
 *
 * @see SelectRouterNodes ();
 * @see BuildGlobalRoutingDatabase ();
 * @see InitializeRoutes ();
 */
  static void PopulateRoutingTables ();

/**
 * @brief Build a routing database and initialize the routing tables of
 * the nodes in the simulation.  Makes the nodes in the provided container
 * into routers.
 *
 * All this function does is call the three functions
 * SelectRouterNodes (), BuildGlobalRoutingDatabase (), and
 * InitializeRoutes ().
 *
 * @see SelectRouterNodes (Node Container c);
 * @see BuildGlobalRoutingDatabase ();
 * @see InitializeRoutes ();
 */
  static void PopulateRoutingTables (NodeContainer c);

 /**
  *@brief Remove all routes that were previously installed in a prior call
 * to either PopulateRoutingTables() or RecomputeRoutingTables(), and 
 * add a new set of routes.  
 * 
 * This method does not change the set of nodes
 * over which GlobalRouting is being used, but it will dynamically update
 * its representation of the global topology before recomputing routes.
 * Users must first call PopulateRoutingTables() and then may subsequently
 * call RecomputeRoutingTables() at any later time in the simulation.
 *
 * @see DeleteGlobalRoutes ();
 * @see BuildGlobalRoutingDatabase ();
 * @see InitializeRoutes ();
 */
 static void RecomputeRoutingTables ();

/**
 * @brief Delete all static routes on all nodes that have a 
 * GlobalRouterInterface
 *
 */
  static void DeleteGlobalRoutes ();

/**
 * @brief Select which nodes in the system are to be router nodes and 
 * aggregate the appropriate interfaces onto those nodes.
 * @internal
 *
 */
  static void SelectRouterNodes ();

/**
 * @brief Select which nodes in the system are to be router nodes and 
 * aggregate the appropriate interfaces onto those nodes.
 * @internal
 *
 */
  static void SelectRouterNodes (NodeContainer c);

/**
 * @brief Allocate a 32-bit router ID from monotonically increasing counter.
 */
  static uint32_t AllocateRouterId ();

private:

/**
 * @brief Build the routing database by gathering Link State Advertisements
 * from each node exporting a GlobalRouter interface.
 * @internal
 *
 */
  static void BuildGlobalRoutingDatabase ();

/**
 * @brief Compute routes using a Dijkstra SPF computation and populate
 * per-node forwarding tables
 * @internal
 */
  static void InitializeRoutes ();

/**
 * @brief Global Route Manager copy construction is disallowed.  There's no 
 * need for it and a compiler provided shallow copy would be wrong.
 *
 */
  GlobalRouteManager (GlobalRouteManager& srm);

/**
 * @brief Global Router copy assignment operator is disallowed.  There's no 
 * need for it and a compiler provided shallow copy would be wrong.
 */
  GlobalRouteManager& operator= (GlobalRouteManager& srm);
};

} // namespace ns3

#endif /* GLOBAL_ROUTE_MANAGER_H */
