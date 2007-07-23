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

namespace ns3 {

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
class StaticRouteManager
{
public:
/**
 * @brief Build a routing database and initialize the routing tables of
 * the nodes in the simulation.
 *
 * All this function does is call  BuildStaticRoutingDatabase () and
 * InitializeRoutes ().  There's no reason to export the two-step process
 * since a high-level caller has no need to know how we're implementing the
 * process.
 *
 * @see BuildStaticRoutingDatabase ();
 * @see InitializeRoutes ();
 */
  static void PopulateRoutingTables ();
private:
/**
 * @brief Build the routing database by gathering Link State Advertisements
 * from each node exporting a StaticRouter interface.
 *
 */
  static void BuildStaticRoutingDatabase ();
/**
 * @brief Compute routes using a Dijkstra SPF computation and populate
 * per-node forwarding tables
 */
  static void InitializeRoutes ();
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
};

} // namespace ns3

#endif /* STATIC_ROUTE_MANAGER_H */
