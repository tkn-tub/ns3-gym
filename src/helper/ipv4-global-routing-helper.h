/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
#ifndef IPV4_GLOBAL_ROUTING_HELPER_H
#define IPV4_GLOBAL_ROUTING_HELPER_H

#include "node-container.h"
#include "ipv4-routing-helper.h"

namespace ns3 {

/**
 * \brief Helper class that adds ns3::Ipv4GlobalRouting objects
 */
class Ipv4GlobalRoutingHelper  : public Ipv4RoutingHelper
{
public:
  Ipv4GlobalRoutingHelper ();
  /**
   * \param node the node on which the routing protocol will run
   * \returns a newly-created routing protocol
   *
   * This method will be called by ns3::InternetStackHelper::Install
   */
  virtual Ptr<Ipv4RoutingProtocol> Create (Ptr<Node> node) const;

  /**
   * @brief Build a routing database and initialize the routing tables of
   * the nodes in the simulation.  Makes all nodes in the simulation into
   * routers.
   *
   * All this function does is call the functions
   * BuildGlobalRoutingDatabase () and  InitializeRoutes ().
   *
   */
  static void PopulateRoutingTables (void);
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
   */
  static void RecomputeRoutingTables (void);
};

} // namespace ns3

#endif /* IPV4_GLOBAL_ROUTING_HELPER_H */
