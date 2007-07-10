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
#include <queue>
#include "ns3/assert.h"
#include "ns3/fatal-error.h"
#include "ns3/debug.h"
#include "ns3/node-list.h"
#include "static-router.h"
#include "static-route-manager.h"

NS_DEBUG_COMPONENT_DEFINE ("StaticRouteManager");

namespace ns3 {

SPFVertex::SPFVertex () : 
  m_vertexType(VertexUnknown), 
  m_vertexId("255.255.255.255"), 
  m_lsa(0),
  m_distanceFromRoot(SPF_INFINITY), 
  m_stat(false)
{
}

SPFVertex::~SPFVertex ()
{
  delete m_lsa;
}

void
SPFVertex::Initialize ()
{
  m_distanceFromRoot = SPF_INFINITY;
  m_stat = false;
  // XXX previous = 0
}


StaticRouteManagerLSDB::~StaticRouteManagerLSDB()
{
  NS_DEBUG("StaticRouteManagerLSDB::~StaticRouteManagerLSDB ()");

  LSDBMap_t::iterator i;
  for (i= m_database.begin(); i!= m_database.end(); i++)
    {
      NS_DEBUG("StaticRouteManagerLSDB::~StaticRouteManagerLSDB():free vertex");
      SPFVertex* temp = i->second;
      delete temp;
    }
  NS_DEBUG("StaticRouteManagerLSDB::~StaticRouteManagerLSDB ():  clear map");
  m_database.clear();
}

void
StaticRouteManagerLSDB::Initialize()
{
  NS_DEBUG("StaticRouteManagerLSDB::Initialize ()");

  LSDBMap_t::iterator i;
  for (i= m_database.begin(); i!= m_database.end(); i++)
    {
      SPFVertex* temp = i->second;
      temp->Initialize();
    }
}

void
StaticRouteManagerLSDB::Insert(Ipv4Address addr, SPFVertex* vertex)
{
    m_database.insert(LSDBPair_t(addr, vertex));
}

SPFVertex*
StaticRouteManagerLSDB::GetVertex (Ipv4Address addr)
{
  // Look up an LSA by its address
  LSDBMap_t::iterator i;
  for (i= m_database.begin(); i!= m_database.end(); i++)
  {
    if (i->first == addr)
    {
      return i->second;
    }
  }
  return 0;
}

StaticRouteManager::StaticRouteManager () 
{
  m_lsdb = new StaticRouteManagerLSDB ();
}

StaticRouteManager::~StaticRouteManager ()
{
  if (m_lsdb)
    delete m_lsdb;
}

void
StaticRouteManager::DebugUseLsdb (StaticRouteManagerLSDB* lsdb)
{
  if (m_lsdb)
    delete m_lsdb;
  m_lsdb = lsdb;
}

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
//
// Should call DiscoverLSAs () before trying to use any routing info or to
// update LSAs.  Subsequently you may use GetNumLSAs().  If you call
// GetNumLSAs () before calling DiscoverLSAs () will get zero as the number.
// 
      uint32_t numLSAs = rtr->DiscoverLSAs();
      NS_DEBUG_UNCOND ("Found " << numLSAs << " LSAs");

      for (uint32_t j = 0; j < numLSAs; ++j)
        {
          StaticRouterLSA lsa;
          rtr->GetLSA(j, lsa);
          NS_DEBUG_UNCOND ("LSA " << j);
          NS_DEBUG_UNCOND ("----------------------------");
          NS_DEBUG_UNCOND(lsa);
        }
    }
}

  // For each node that is a static router (which can be determined by
  // the presence of StaticRouter interface), run Dijkstra SPF calculation
  // on the database rooted at that router, and populate the node
  // forwarding tables
  //
void
StaticRouteManager::InitializeRoutes ()
{
  NS_DEBUG_UNCOND("StaticRouteManager::InitializeRoutes ()");
//      This function parallels RFC2328, Section 16.1.1, and quagga ospfd
//
//      This calculation yields the set of intra-area routes associated
//      with an area (called hereafter Area A).  A router calculates the
//      shortest-path tree using itself as the root.  The formation
//      of the shortest path tree is done here in two stages.  In the
//      first stage, only links between routers and transit networks are
//      considered.  Using the Dijkstra algorithm, a tree is formed from
//      this subset of the link state database.  In the second stage,
//      leaves are added to the tree by considering the links to stub
//      networks.

//      The area's link state database is represented as a directed graph.  
//      The graph's vertices are routers, transit networks and stub networks.  
//      The first stage of the procedure (i.e., the Dijkstra algorithm)
//      can now be summarized as follows. At each iteration of the
//      algorithm, there is a list of candidate vertices.  Paths from
//      the root to these vertices have been found, but not necessarily
//      the shortest ones.  However, the paths to the candidate vertex
//      that is closest to the root are guaranteed to be shortest; this
//      vertex is added to the shortest-path tree, removed from the
//      candidate list, and its adjacent vertices are examined for
//      possible addition to/modification of the candidate list.  The
//      algorithm then iterates again.  It terminates when the candidate
//      list becomes empty. 

    // For each node that is a router in the topology
  typedef std::vector < Ptr<Node> >::iterator Iterator;
  for (Iterator i = NodeList::Begin(); i != NodeList::End(); i++)
    {
      Ptr<Node> node = *i;
      NS_DEBUG_UNCOND ("node="<< node->GetId () );
      
      Ptr<StaticRouter> rtr = 
        node->QueryInterface<StaticRouter> (StaticRouter::iid);
      NS_ASSERT_MSG(rtr, "QI for <StaticRouter> interface failed");
      if (rtr && rtr->GetNumLSAs () )
        {
          SPFCalculate(rtr->GetRouterId ());
        }
    }
}


// quagga ospf_spf_next
void
StaticRouteManager::SPFNext()
{
  // if LSA == Router_LSA, examine links
  // a) if link to stub, skip
  // b) W is transit
}

// quagga ospf_spf_calculate
void
StaticRouteManager::DebugSPFCalculate(Ipv4Address root)
{
  SPFCalculate(root);
}

// quagga ospf_spf_calculate
void
StaticRouteManager::SPFCalculate(Ipv4Address root)
{
  NS_DEBUG("StaticRouteManager::SPFCalculate ()");

  // The SPFVertex objects may have state from a previous computation
  m_lsdb->Initialize();
  SPFVertex* v;

  // Make a priority queue of int using a vector container
  //    priority_queue<int, vector<int>, less<int> > pq;
  //
  //priority_queue<SPFVertex*> candidate;
  //
  // Initialize the shortest-path tree to only the router doing the 
  // calculation.
  //
  v= m_lsdb->GetVertex(root);
  // Set LSA position to LSA_SPF_IN_SPFTREE. This vertex is the root of the
  // spanning tree. 
  v->m_distanceFromRoot = 0;
  v->m_stat = true;
  
#if 0
  for (;;)
    {
      /* RFC2328 16.1. (2). */
      ospf_spf_next (v, area, candidate);
  
      /* RFC2328 16.1. (3). */
      /* If at this step the candidate list is empty, the shortest-
 *          path tree (of transit vertices) has been completely built and
 *                   this stage of the procedure terminates. */
      if (candidate->size == 0)
        break;
  
      /* Otherwise, choose the vertex belonging to the candidate list
 *          that is closest to the root, and add it to the shortest-path
 *                   tree (removing it from the candidate list in the
 *                            process). */
      /* Extract from the candidates the node with the lower key. */
      v = (struct vertex *) pqueue_dequeue (candidate);
      /* Update stat field in vertex. */
      *(v->stat) = LSA_SPF_IN_SPFTREE;

      ospf_vertex_add_parent (v);
      /* Note that when there is a choice of vertices closest to the
 *          root, network vertices must be chosen before router vertices
 *                   in order to necessarily find all equal-cost paths. */
      /* We don't do this at this moment, we should add the treatment
 *          above codes. -- kunihiro. */

      /* RFC2328 16.1. (4). */
      if (v->type == OSPF_VERTEX_ROUTER)
        ospf_intra_add_router (new_rtrs, v, area);
      else
        ospf_intra_add_transit (new_table, v, area);

      /* RFC2328 16.1. (5). */
      /* Iterate the algorithm by returning to Step 2. */

    } /* end loop until no more candidate vertices */

  /* Second stage of SPF calculation procedure's  */
  ospf_spf_process_stubs (area, area->spf, new_table);
  
  /* Free candidate queue. */
  pqueue_delete (candidate);

#endif


}


} // namespace ns3

#ifdef RUN_SELF_TESTS

#include "ns3/test.h"

namespace ns3 {

class StaticRouterTestNode : public Node
{
public:
  StaticRouterTestNode();

private:
  virtual void DoAddDevice (Ptr<NetDevice> device) const {};
  virtual TraceResolver *DoCreateTraceResolver (TraceContext const &context);
};

StaticRouterTestNode::StaticRouterTestNode ()
{
//  Ptr<Ipv4L3Protocol> ipv4 = Create<Ipv4L3Protocol> (this);
}

TraceResolver*
StaticRouterTestNode::DoCreateTraceResolver (TraceContext const &context)
{
  return 0;
}


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
  lsa0->m_linkStateId.Set("0.0.0.0");
  lsa0->m_advertisingRtr.Set("0.0.0.0");
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

  // Create four vertices to store these four LSAs
  SPFVertex* v0 = new SPFVertex ();
  v0->m_lsa = lsa0;
  v0->m_vertexType = SPFVertex::VertexRouter;
  SPFVertex* v1 = new SPFVertex ();
  v1->m_lsa = lsa1;
  v0->m_vertexType = SPFVertex::VertexRouter;
  SPFVertex* v2 = new SPFVertex ();
  v2->m_lsa = lsa2;
  v0->m_vertexType = SPFVertex::VertexRouter;
  SPFVertex* v3 = new SPFVertex ();
  v3->m_lsa = lsa3;
  v0->m_vertexType = SPFVertex::VertexRouter;
  
  // Test the database 
  StaticRouteManagerLSDB* srmlsdb = new StaticRouteManagerLSDB();
  srmlsdb->Insert(lsa0->m_linkStateId, v0);
  srmlsdb->Insert(lsa1->m_linkStateId, v1);
  srmlsdb->Insert(lsa2->m_linkStateId, v2);
  srmlsdb->Insert(lsa3->m_linkStateId, v3);
  NS_ASSERT(v2 == srmlsdb->GetVertex(lsa2->m_linkStateId));

  // We need a dummy node to populate the routing tables
  Ptr<StaticRouterTestNode> n2 = Create<StaticRouterTestNode> ();

  // XXX next, calculate routes based on the manually created LSDB
  StaticRouteManager* srm = new StaticRouteManager();
  srm->DebugUseLsdb (srmlsdb);
  srm->DebugSPFCalculate(lsa0->m_linkStateId);  // node n0

  // This delete clears the srm, which deletes the LSDB, which clears 
  // all of the vertices, which each destroy the matching LSAs, which each
  // destroys the attached LinkRecords.
  delete srm;

  return ok;
}

// Instantiate this class for the unit tests
static StaticRouteManagerTest g_staticRouteManagerTest;

} // namespace ns3

#endif 
