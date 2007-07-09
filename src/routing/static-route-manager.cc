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
#include <utility>
#include <vector>
#include "ns3/assert.h"
#include "ns3/fatal-error.h"
#include "ns3/debug.h"
#include "ns3/node-list.h"
#include "static-router.h"
#include "static-route-manager.h"

NS_DEBUG_COMPONENT_DEFINE ("StaticRouteManager");

namespace ns3 {

StaticRouteManager::StaticRouteManager ()
{
}

StaticRouteManager::~StaticRouteManager ()
{}

void
StaticRouteManager::BuildStaticRoutingDatabase () 
{
  // walk list of nodes.  QI for StaticRouter interface.
  // if node has a StaticRouter interface, grab the LSAs 
  // from it and stick them in the LSDB
  typedef std::vector < Ptr<Node> >::iterator Iterator;
  for (Iterator i = NodeList::Begin(); i != NodeList::End(); i++)
    {
      Ptr<Node> node = *i;
      NS_DEBUG_UNCOND ("node="<< node->GetId () );

      Ptr<StaticRouter> rtr = 
        node->QueryInterface<StaticRouter> (StaticRouter::iid);
      NS_ASSERT_MSG(rtr, "QI for <StaticRouter> interface failed");

      uint32_t numLSAs = rtr->GetNumLSAs();
      NS_DEBUG_UNCOND (numLSAs << "LSAs");
    }
}

void
StaticRouteManager::InitializeRoutes ()
{
  // For each node that is a static router (which can be determined by
  // the presence of StaticRouter interface), run Dijkstra SPF calculation
  // on the database rooted at that router, and populate the node
  // forwarding tables
}

} // namespace ns3

#ifdef RUN_SELF_TESTS

#include "ns3/test.h"

namespace ns3 {

class StaticRouteManagerTest : public Test {
public:
  StaticRouteManagerTest ();
  virtual ~StaticRouteManagerTest ();
  virtual bool RunTests (void);
};

StaticRouteManagerTest::StaticRouteManagerTest ()
  : Test ("StaticRouteManager")
{
}

StaticRouteManagerTest::~StaticRouteManagerTest ()
{}

bool
StaticRouteManagerTest::RunTests (void)
{
  DebugComponentEnable("StaticRouteManager");
  bool ok = true;
#if 0
  StaticRouterLinkRecord* lr1 = new StaticRouterLinkRecord();
  lr1->m_linkId.Set(1);
  lr1->m_linkData.Set(0xffffffff);
  lr1->m_linkType = StaticRouterLinkRecord::PointToPoint;
  lr1->m_metric = 1;
  StaticRouterLSA* lsa1 = new StaticRouterLSA();
  lsa1->m_linkStateId.Set(1);
  lsa1->m_advertisingRtr.Set(1);
  lsa1->Add(lr1);
  
  delete lsa1;
#endif
  return ok;
}

static StaticRouteManagerTest g_staticRouteManagerTest;

} // namespace ns3

#endif 
