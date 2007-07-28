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

#ifndef GLOBAL_ROUTE_MANAGER_H
#define GLOBAL_ROUTE_MANAGER_H

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
 * the nodes in the simulation.
 *
 * All this function does is call  BuildGlobalRoutingDatabase () and
 * InitializeRoutes ().
 *
 * @see BuildGlobalRoutingDatabase ();
 * @see InitializeRoutes ();
 */
  static void PopulateRoutingTables ();

/**
 * @brief Allocate a 32-bit router ID from monotonically increasing counter.
 */
  static uint32_t AllocateRouterId ();

private:
/**
 * @brief Select which nodes in the system are to be router nodes and 
 * aggregate the appropriate interfaces onto those nodes.
 *
 */
  static void SelectRouterNodes ();

/**
 * @brief Build the routing database by gathering Link State Advertisements
 * from each node exporting a GlobalRouter interface.
 *
 */
  static void BuildGlobalRoutingDatabase ();

/**
 * @brief Compute routes using a Dijkstra SPF computation and populate
 * per-node forwarding tables
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
