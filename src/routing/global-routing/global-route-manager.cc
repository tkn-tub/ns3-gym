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
 * Author: Tom Henderson (tomhend@u.washington.edu)
 */

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulation-singleton.h"
#include "ns3/node-container.h"
#include "global-route-manager.h"
#include "global-route-manager-impl.h"

namespace ns3 {

// ---------------------------------------------------------------------------
//
// GlobalRouteManager Implementation
//
// ---------------------------------------------------------------------------

  void
GlobalRouteManager::PopulateRoutingTables (void) 
{
  SelectRouterNodes ();
  BuildGlobalRoutingDatabase ();
  InitializeRoutes ();
}

  void
GlobalRouteManager::PopulateRoutingTables (NodeContainer c) 
{
  SelectRouterNodes (c);
  BuildGlobalRoutingDatabase ();
  InitializeRoutes ();
}

  void
GlobalRouteManager::SelectRouterNodes (void) 
{
  SimulationSingleton<GlobalRouteManagerImpl>::Get ()->
    SelectRouterNodes ();
}

  void
GlobalRouteManager::SelectRouterNodes (NodeContainer c) 
{
  SimulationSingleton<GlobalRouteManagerImpl>::Get ()->
    SelectRouterNodes (c);
}

  void
GlobalRouteManager::BuildGlobalRoutingDatabase (void) 
{
  SimulationSingleton<GlobalRouteManagerImpl>::Get ()->
    BuildGlobalRoutingDatabase ();
}

  void
GlobalRouteManager::InitializeRoutes (void)
{
  SimulationSingleton<GlobalRouteManagerImpl>::Get ()->
    InitializeRoutes ();
}

  uint32_t
GlobalRouteManager::AllocateRouterId (void)
{
  static uint32_t routerId = 0;
  return routerId++;
}


} // namespace ns3
