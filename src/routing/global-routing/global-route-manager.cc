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

#include "ns3/assert.h"
#include "ns3/debug.h"
#include "ns3/simulation-singleton.h"
#include "global-route-manager.h"
#include "global-route-manager-impl.h"

namespace ns3 {

// ---------------------------------------------------------------------------
//
// GlobalRouteManager Implementation
//
// ---------------------------------------------------------------------------

  void
GlobalRouteManager::PopulateRoutingTables () 
{
  SelectRouterNodes ();
  BuildGlobalRoutingDatabase ();
  InitializeRoutes ();
}

  void
GlobalRouteManager::SelectRouterNodes () 
{
  SimulationSingleton<GlobalRouteManagerImpl>::Get ()->
    SelectRouterNodes ();
}

  void
GlobalRouteManager::BuildGlobalRoutingDatabase () 
{
  SimulationSingleton<GlobalRouteManagerImpl>::Get ()->
    BuildGlobalRoutingDatabase ();
}

  void
GlobalRouteManager::InitializeRoutes ()
{
  SimulationSingleton<GlobalRouteManagerImpl>::Get ()->
    InitializeRoutes ();
}

  uint32_t
GlobalRouteManager::AllocateRouterId ()
{
  static uint32_t routerId = 0;
  return routerId++;
}


} // namespace ns3
