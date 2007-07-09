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
  bool ok = true;

  // Build fake link state database; four routers (0-3), 3 point-to-point
  // links
  //
  //   n0
  //      \ link 0
  //       \          link 2
  //        n2 -------------------------n3
  //       /
  //      / link 1
  //    n1
  //
  //  link0:  10.1.1.1/30, 10.1.1.2/30
  //  link1:  10.1.2.1/30, 10.1.2.2/30
  //  link2:  10.1.3.1/30, 10.1.3.2/30
  //
  // Router 0
  StaticRouterLinkRecord* lr0 = new StaticRouterLinkRecord();
  lr0->m_linkId.Set(2);  // router ID 0.0.0.2
  lr0->m_linkData.Set("10.1.1.1");
  lr0->m_linkType = StaticRouterLinkRecord::PointToPoint;
  lr0->m_metric = 1;
  StaticRouterLinkRecord* lr1 = new StaticRouterLinkRecord();
  lr1->m_linkId.Set("10.1.1.1");  
  lr1->m_linkData.Set("255.255.255.252");
  lr1->m_linkType = StaticRouterLinkRecord::StubNetwork;
  lr1->m_metric = 1;
  StaticRouterLSA* lsa0 = new StaticRouterLSA();
  lsa0->m_linkStateId.Set(1);
  lsa0->m_advertisingRtr.Set(1);
  lsa0->AddLinkRecord(lr0);
  lsa0->AddLinkRecord(lr1);

  // Router 1
  StaticRouterLinkRecord* lr2 = new StaticRouterLinkRecord();
  lr2->m_linkId.Set(2);  // router ID 0.0.0.2
  lr2->m_linkData.Set("10.1.2.1");
  lr2->m_linkType = StaticRouterLinkRecord::PointToPoint;
  lr2->m_metric = 1;
  StaticRouterLinkRecord* lr3 = new StaticRouterLinkRecord();
  lr3->m_linkId.Set("10.1.2.1");  
  lr3->m_linkData.Set("255.255.255.252");
  lr3->m_linkType = StaticRouterLinkRecord::StubNetwork;
  lr3->m_metric = 1;
  StaticRouterLSA* lsa1 = new StaticRouterLSA();
  lsa1->m_linkStateId.Set(1);
  lsa1->m_advertisingRtr.Set(1);
  lsa1->AddLinkRecord(lr2);
  lsa1->AddLinkRecord(lr3);
  
  // Router 2 
  StaticRouterLinkRecord* lr4 = new StaticRouterLinkRecord();
  lr4->m_linkId.Set("0.0.0.0");
  lr4->m_linkData.Set("10.1.1.2");
  lr4->m_linkType = StaticRouterLinkRecord::PointToPoint;
  lr4->m_metric = 1;
  StaticRouterLinkRecord* lr5 = new StaticRouterLinkRecord();
  lr5->m_linkId.Set("10.1.1.2");  
  lr5->m_linkData.Set("255.255.255.252");
  lr5->m_linkType = StaticRouterLinkRecord::StubNetwork;
  lr5->m_metric = 1;
  StaticRouterLinkRecord* lr6 = new StaticRouterLinkRecord();
  lr6->m_linkId.Set(1);  
  lr6->m_linkData.Set("10.1.2.2");
  lr6->m_linkType = StaticRouterLinkRecord::PointToPoint;
  lr6->m_metric = 1;
  StaticRouterLinkRecord* lr7 = new StaticRouterLinkRecord();
  lr7->m_linkId.Set("10.1.2.2");  
  lr7->m_linkData.Set("255.255.255.252");
  lr7->m_linkType = StaticRouterLinkRecord::StubNetwork;
  lr7->m_metric = 1;
  StaticRouterLinkRecord* lr8 = new StaticRouterLinkRecord();
  lr8->m_linkId.Set(3);  
  lr8->m_linkData.Set("10.1.3.2");
  lr8->m_linkType = StaticRouterLinkRecord::PointToPoint;
  lr8->m_metric = 1;
  StaticRouterLinkRecord* lr9 = new StaticRouterLinkRecord();
  lr9->m_linkId.Set("10.1.3.2");  
  lr9->m_linkData.Set("255.255.255.252");
  lr9->m_linkType = StaticRouterLinkRecord::StubNetwork;
  lr9->m_metric = 1;
  StaticRouterLSA* lsa2 = new StaticRouterLSA();
  lsa2->m_linkStateId.Set(2);
  lsa2->m_advertisingRtr.Set(2);
  lsa2->AddLinkRecord(lr4);
  lsa2->AddLinkRecord(lr5);
  lsa2->AddLinkRecord(lr6);
  lsa2->AddLinkRecord(lr7);
  lsa2->AddLinkRecord(lr8);
  lsa2->AddLinkRecord(lr9);

  // Router 3
  StaticRouterLinkRecord* lr10 = new StaticRouterLinkRecord();
  lr10->m_linkId.Set(2);  // router ID 0.0.0.2
  lr10->m_linkData.Set("10.1.2.1");
  lr10->m_linkType = StaticRouterLinkRecord::PointToPoint;
  lr10->m_metric = 1;
  StaticRouterLinkRecord* lr11 = new StaticRouterLinkRecord();
  lr11->m_linkId.Set("10.1.2.1");  
  lr11->m_linkData.Set("255.255.255.252");
  lr11->m_linkType = StaticRouterLinkRecord::StubNetwork;
  lr11->m_metric = 1;
  StaticRouterLSA* lsa3 = new StaticRouterLSA();
  lsa3->m_linkStateId.Set(3);
  lsa3->m_advertisingRtr.Set(3);
  lsa3->AddLinkRecord(lr2);
  lsa3->AddLinkRecord(lr3);

  // Add four LSAs to the database; XXX todo next

  delete lsa0;
  delete lsa1;
  delete lsa2;
  delete lsa3;

  return ok;
}

static StaticRouteManagerTest g_staticRouteManagerTest;

} // namespace ns3

#endif 
