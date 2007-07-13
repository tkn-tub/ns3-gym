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
#include "ns3/ipv4.h"
#include "static-router.h"
#include "static-route-manager.h"
#include "candidate-queue.h"

NS_DEBUG_COMPONENT_DEFINE ("StaticRouteManager");

namespace ns3 {

SPFVertex::SPFVertex () : 
  m_vertexType(VertexUnknown), 
  m_vertexId("255.255.255.255"), 
  m_lsa(0),
  m_parent(0),
  m_children(),
  m_distanceFromRoot(SPF_INFINITY), 
  m_rootOif(SPF_INFINITY),
  m_nextHop("0.0.0.0")
{
}

SPFVertex::SPFVertex (StaticRouterLSA* lsa) : 
  m_vertexType(VertexRouter), 
  m_vertexId(lsa->m_linkStateId),
  m_lsa(lsa),
  m_parent(0),
  m_children(),
  m_distanceFromRoot(SPF_INFINITY), 
  m_rootOif(SPF_INFINITY),
  m_nextHop("0.0.0.0")
{
}


SPFVertex::~SPFVertex ()
{
}

StaticRouteManagerLSDB::~StaticRouteManagerLSDB()
{
  NS_DEBUG("StaticRouteManagerLSDB::~StaticRouteManagerLSDB ()");

  LSDBMap_t::iterator i;
  for (i= m_database.begin(); i!= m_database.end(); i++)
    {
      NS_DEBUG("StaticRouteManagerLSDB::~StaticRouteManagerLSDB():free LSA");
      StaticRouterLSA* temp = i->second;
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
      StaticRouterLSA* temp = i->second;
      temp->m_stat = StaticRouterLSA::LSA_SPF_NOT_EXPLORED;
    }
}

void
StaticRouteManagerLSDB::Insert(Ipv4Address addr, StaticRouterLSA* lsa)
{
  NS_DEBUG("StaticRouteManagerLSDB::Insert ()");
  m_database.insert(LSDBPair_t(addr, lsa));
}

StaticRouterLSA*
StaticRouteManagerLSDB::GetLSA (Ipv4Address addr)
{
  NS_DEBUG("StaticRouteManagerLSDB::GetLSA ()");
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

StaticRouteManager::StaticRouteManager () : m_spfroot(0) 
{
  m_lsdb = new StaticRouteManagerLSDB ();
}

StaticRouteManager::~StaticRouteManager ()
{
  NS_DEBUG("StaticRouteManager::~StaticRouteManager ()");

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
  NS_DEBUG("StaticRouteManager::BuildStaticRoutingDatabase()");

  // Walk the list of nodes.  QI for StaticRouter interface.
  // if node has a StaticRouter interface, grab the LSAs 
  // from it and stick them in the LSDB
  typedef std::vector < Ptr<Node> >::iterator Iterator;
  for (Iterator i = NodeList::Begin(); i != NodeList::End(); i++)
    {
      Ptr<Node> node = *i;

      Ptr<StaticRouter> rtr = 
        node->QueryInterface<StaticRouter> (StaticRouter::iid);
      NS_ASSERT_MSG(rtr, "QI for <StaticRouter> interface failed");

      // You must call DiscoverLSAs () before trying to use any 
      // routing info or to update LSAs.  Subsequently you may use 
      // GetNumLSAs().  If you call GetNumLSAs () before calling 
      // DiscoverLSAs () will get zero as the number.
      uint32_t numLSAs = rtr->DiscoverLSAs();
      NS_DEBUG ("Discover LSAs:  Found " << numLSAs << " LSAs");

      for (uint32_t j = 0; j < numLSAs; ++j)
        {
          StaticRouterLSA* lsa = new StaticRouterLSA ();
          rtr->GetLSA(j, *lsa);
          NS_DEBUG ("LSA " << j);
          NS_DEBUG ("----------------------------");
          NS_DEBUG (*lsa);
          m_lsdb->Insert (lsa->m_linkStateId, lsa); 
        }
    }
}

// For each node that is a static router (which can be determined by
// the presence of StaticRouter interface), run Dijkstra SPF calculation
// on the database rooted at that router, and populate the node
// forwarding tables
void
StaticRouteManager::InitializeRoutes ()
{
  NS_DEBUG("StaticRouteManager::InitializeRoutes ()");
  //    This function parallels RFC2328, Section 16.1.1, and quagga ospfd
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

  // Iterate for each node that is a router in the topology
  typedef std::vector < Ptr<Node> >::iterator Iterator;
  for (Iterator i = NodeList::Begin(); i != NodeList::End(); i++)
    {
      Ptr<Node> node = *i;
      
      Ptr<StaticRouter> rtr = 
        node->QueryInterface<StaticRouter> (StaticRouter::iid);
      NS_ASSERT_MSG(rtr, "QI for <StaticRouter> interface failed");
      if (rtr && rtr->GetNumLSAs () )
        {
          SPFCalculate(rtr->GetRouterId ());
        }
    }
}


// Derived from quagga ospf_spf_next()
// RFC2328 Section 16.1 (2).
// v is on the SPF tree.  Examine the links in v's LSA.  Update the list
// of candidates with any vertices not already on the list.  If a lower-cost
// path is found to a vertex already on the candidate list, store the new cost.
void
StaticRouteManager::SPFNext(SPFVertex* v, CandidateQueue& candidate)
{
  SPFVertex* w = 0;
  StaticRouterLSA* w_lsa = 0;
  uint32_t distance = 0;

  NS_DEBUG("StaticRouteManager::SPFNext ()");
  if (v->m_vertexType == SPFVertex::VertexRouter) 
    {
      // Always true for now, since all our LSAs are RouterLSAs
      if (true)
        {
          NS_DEBUG ("SPFNext: Examining " << v->m_vertexId << "'s " <<
            v->m_lsa->m_linkRecords.size() << " link records");
          for ( StaticRouterLSA::ListOfLinkRecords_t::iterator i = 
                v->m_lsa->m_linkRecords.begin();
                i != v->m_lsa->m_linkRecords.end();
                i++ )
            {
              // (a) If this is a link to a stub network, examine the next
              // link in V's LSA.  Links to stub networks will be
              // considered in the second stage of the shortest path
              // calculation. 
              StaticRouterLinkRecord* l = *i;
              if (l->m_linkType == StaticRouterLinkRecord::StubNetwork)
                {
                  NS_DEBUG("SPFNext: Found a Stub record to " 
                    << l->m_linkId);
                  continue;
                }
                // (b) Otherwise, W is a transit vertex (router or transit
                // network).  Look up the vertex W's LSA (router-LSA or
                // network-LSA) in Area A's link state database. 
              if (l->m_linkType == StaticRouterLinkRecord::PointToPoint)
                {
                  // Lookup the vertex W's LSA 
                  w_lsa = m_lsdb->GetLSA(l->m_linkId);
                  NS_ASSERT(w_lsa);
                  NS_DEBUG("SPFNext:  Found a P2P record from " << 
                    v->m_vertexId << " to " << w_lsa->m_linkStateId);
                  // (c) If vertex W is already on the shortest-path tree, 
                  //  examine the next link in the LSA. 
                  if (w_lsa->m_stat == StaticRouterLSA::LSA_SPF_IN_SPFTREE) 
                    {
                      NS_DEBUG("SPFNext: Skipping->  LSA "<< 
                        w_lsa->m_linkStateId << " already in SPF tree");
                      continue;
                    }
                  // (d) Calculate the link state cost D of the resulting path
                  // from the root to vertex W.  D is equal to the sum of 
                  // the link state cost of the (already calculated) 
                  // shortest path to vertex V and the advertised cost of 
                  // the link between vertices V and W.  
                  distance = v->m_distanceFromRoot + l->m_metric;

                  NS_DEBUG("SPFNext: Considering w_lsa " << 
                    w_lsa->m_linkStateId);
                  // Here, W is either already in candidate list or not
                  if (w_lsa->m_stat == StaticRouterLSA::LSA_SPF_NOT_EXPLORED)
                    {
                        w = new SPFVertex(w_lsa);
                      // Calculate nexthop to W
                      if (SPFNexthopCalculation(v, w, l, distance))
                        {
                          w_lsa->m_stat = StaticRouterLSA::LSA_SPF_CANDIDATE;
                          candidate.Push(w);
                          NS_DEBUG("SPFNext:  Pushing " << w->m_vertexId
                            << ", parent vertexId: " << v->m_vertexId);
                        }
                    }
                  } else if (w_lsa->m_stat == 
                             StaticRouterLSA::LSA_SPF_CANDIDATE)
                    {
                      //Get the vertex from candidates
                      w = candidate.Find(w_lsa->m_linkStateId);
                      if (w->m_distanceFromRoot < distance)
                        {
                          continue; // not a shorter path
                        }
                       // equal to
                       else if (w->m_distanceFromRoot == distance)
                         {
                           // Do nothing-- not doing equal-cost multipath
                         }
                       else
                         {
                          // Found a lower-cost path to W.
                          // nexthop_calculation is conditional, if it finds
                          // valid nexthop it will call spf_add_parents, which
                          // will flush the old parents
                           if (SPFNexthopCalculation(v, w, l, distance))
                             {
                               // Decrease the key of the node in the heap,
                               // re-sort the heap. 
                               candidate.Reorder();
                             }
                          }    
                }  // point-to-point
            } // for loop
        } 
     }
}

// Derived from quagga ospf_next_hop_calculation()
// 16.1.1.  Calculate nexthop from root through V (parent) to
// vertex W (destination), with given distance from root->W.
//
// For now, this is greatly simplified from the quagga code
//                  
int
StaticRouteManager::SPFNexthopCalculation (
  SPFVertex* v, 
  SPFVertex* w,
  StaticRouterLinkRecord* l,
  uint32_t distance)
{
  NS_DEBUG("StaticRouteManager::SPFNexthopCalculation ()");
  if (v == m_spfroot)
    {
      // parent of w is the root itself
      // calculate the interfaceid of the router that corresponds
      // to link l between v and w and store it in w->m_rootOif
      // This rootOif is then used when installing host routes for the
      // destinations covered by this vertex.  Store also the next hop
      // IP address.
      //
      // Find the outgoing interface on v corresponding to the link l
      // between v and w
      if (w->m_vertexType == SPFVertex::VertexRouter) 
        {
          // l is a link from v to w
          // l2 will be a link from w to v
          StaticRouterLinkRecord *l2 = 0;
          l2 = SPFGetNextLink(w,v,l2);
          w->m_nextHop = l2->m_linkData;
          // Find interface corresponding to link's IP address
          w->m_rootOif = FindOutgoingInterfaceId(l->m_linkData); 

          NS_DEBUG("SPFNexthopCalculation: Next hop from " << 
            v->m_vertexId << " to " << w->m_vertexId << 
            " goes through next hop " << w->m_nextHop <<
            " via outgoing interface " << w->m_rootOif);
        }
    }
  else 
    {
       // Inherit the rootOif and nextHop from the current parent
       w->m_rootOif = v->m_rootOif;
       w->m_nextHop = v->m_nextHop;
    }
  w->m_distanceFromRoot = distance;
  w->m_parent = v;
  return 1;
}

// Derived from quagga ospf_get_next_link
// Find the next link after prev_link from v to w.  If prev_link is
// NULL, return the first link from v to w.  Ignore stub and virtual links;
// these link types will never be returned.
//
StaticRouterLinkRecord* 
StaticRouteManager::SPFGetNextLink(
  SPFVertex* v,
  SPFVertex* w,
  StaticRouterLinkRecord* prev_link
  ) 
{
  NS_DEBUG("StaticRouteManager::SPFGetNextLink ()");
  bool skip = true;
  StaticRouterLinkRecord* l;
  if (prev_link == 0)
    {
      skip = false;
    }
  
  for ( StaticRouterLSA::ListOfLinkRecords_t::iterator i = 
        v->m_lsa->m_linkRecords.begin();
        i != v->m_lsa->m_linkRecords.end();
        i++ )
    {
        l = *i;
        if (l->m_linkType != StaticRouterLinkRecord::PointToPoint)
          {
            continue;
          }
        if (l->m_linkId == w->m_vertexId) {
          NS_DEBUG("SPFGetNextLink: Found matching link l:  linkId=" <<
            l->m_linkId << " linkData=" << l->m_linkData);
          if (skip == false) 
            {
              NS_DEBUG("SPFGetNextLink: Returning the found link");
              return l;
            }
          else
            {
              NS_DEBUG("SPFGetNextLink: Skipping the found link");
              skip = false;
              continue;
            }
        }
    }
  return 0;
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

  SPFVertex *v;

  m_lsdb->Initialize ();

  // The candidate queue is a priority queue of SPFVertex objects, with
  // the top of the queue being the closest vertex in terms of 
  // distanceFromRoot.  Initially, this queue is empty.
  //
  CandidateQueue candidate;
  NS_ASSERT(candidate.Size() == 0);
  //
  // Initialize the shortest-path tree to only the router doing the 
  // calculation.
  //
  v= new SPFVertex(m_lsdb->GetLSA(root));
  // This vertex is the root of the SPF tree
  v->m_distanceFromRoot = 0;
  m_spfroot= v;  
  v->m_lsa->m_stat = StaticRouterLSA::LSA_SPF_IN_SPFTREE;

  for (;;)
    {
      // RFC2328 16.1. (2). 
      SPFNext(v , candidate);

      // RFC2328 16.1. (3). 
      // If at this step the candidate list is empty, the shortest-
      // path tree (of transit vertices) has been completely built and
      // this stage of the procedure terminates. 
      if (candidate.Size() == 0)
        break;
      // Otherwise, choose the vertex belonging to the candidate list
      // that is closest to the root, and add it to the shortest-path
      // tree (removing it from the candidate list in the 
      // process). 
      // Extract from the candidates the node with the lower key. 
      v = candidate.Top();
      candidate.Pop();
      // Update stat field in vertex. 
      NS_DEBUG("SPFCalculate: Popping vertex" << v->m_vertexId);
      v->m_lsa->m_stat = StaticRouterLSA::LSA_SPF_IN_SPFTREE;
      SPFVertexAddParent(v);
      // Note that when there is a choice of vertices closest to the
      // root, network vertices must be chosen before router vertices
      // in order to necessarily find all equal-cost paths. 
      // We don't do this at this moment, we should add the treatment
      // above codes. -- kunihiro. 

      // RFC2328 16.1. (4). 
      SPFIntraAddRouter (v);

      // RFC2328 16.1. (5). 
      // Iterate the algorithm by returning to Step 2.
    } // end loop until no more candidate vertices 

  // Second stage of SPF calculation procedure's  
  // NOTYET:  ospf_spf_process_stubs (area, area->spf, new_table);

  DeleteSPFVertexChain(m_spfroot);  
  m_spfroot = 0;
}

// XXX this should probably be a method on Ipv4
uint32_t
StaticRouteManager::FindOutgoingInterfaceId(Ipv4Address a)
{

  Ipv4Address routerId = m_spfroot->m_vertexId;

  std::vector<Ptr<Node> >::iterator i = NodeList::Begin(); 
  for (; i != NodeList::End(); i++)
    {
      Ptr<Node> node = *i;

      Ptr<StaticRouter> rtr = 
        node->QueryInterface<StaticRouter> (StaticRouter::iid);
      NS_ASSERT_MSG(rtr, 
        "StaticRouteManager::SPFIntraAddRouter (): "
        "QI for <StaticRouter> interface failed");
      if (rtr->GetRouterId () == routerId)
        {
          Ptr<Ipv4> ipv4 = node->QueryInterface<Ipv4> (Ipv4::iid);
          NS_ASSERT_MSG(ipv4, 
            "StaticRouteManager::SPFIntraAddRouter (): "
            "QI for <Ipv4> interface failed");
          for (uint32_t i = 0; i < ipv4->GetNInterfaces(); i++)
            {
              if (ipv4->GetAddress (i) == a) {
                NS_DEBUG("FindOutgoingInterfaceId: Interface match for " << a);
                return i;
              }
            }
        }
     }
  return 0;
}

// derived from quagga ospf_intra_add_router()
//
// This is where we add host routes to the routing tables
void
StaticRouteManager::SPFIntraAddRouter(SPFVertex* v)
{
   // This vertex has just been added to the SPF tree
   // - the vertex should have a valid m_root_oid corresponding
   //   to the outgoing interface on the root router of the tree
   //   that corresponds to the path to it
   // - the vertex has an m_lsa field that has a number of link
   //   records.  For each point to point record, the m_linkData
   //   is a destination IP address to which we add a host route
   //

  NS_ASSERT_MSG(m_spfroot, 
    "StaticRouteManager::SPFIntraAddRouter (): Root pointer not set");

  Ipv4Address routerId = m_spfroot->m_vertexId;

  std::vector<Ptr<Node> >::iterator i = NodeList::Begin(); 
  for (; i != NodeList::End(); i++)
    {
      Ptr<Node> node = *i;

      Ptr<StaticRouter> rtr = 
        node->QueryInterface<StaticRouter> (StaticRouter::iid);
      NS_ASSERT_MSG(rtr, 
        "StaticRouteManager::SPFIntraAddRouter (): "
        "QI for <StaticRouter> interface failed");

      if (rtr->GetRouterId () == routerId)
        {
          NS_DEBUG("StaticRouteManager::SPFIntraAddRouter (): "
            "setting routes for node " << node->GetId ());

          Ptr<Ipv4> ipv4 = node->QueryInterface<Ipv4> (Ipv4::iid);
          NS_ASSERT_MSG(ipv4, 
            "StaticRouteManager::SPFIntraAddRouter (): "
            "QI for <Ipv4> interface failed");

          StaticRouterLSA *lsa = v->m_lsa;
          NS_ASSERT_MSG(lsa, 
            "StaticRouteManager::SPFIntraAddRouter (): "
            "Expected valid LSA in SPFVertex* v");

          uint32_t nLinkRecords = lsa->GetNLinkRecords ();

          NS_ASSERT_MSG((nLinkRecords & 1) == 0,
            "StaticRouteManager::SPFIntraAddRouter (): "
            "Expected exen number of Link Records");

          for (uint32_t j = 0; j < nLinkRecords; j += 2)
            {
              StaticRouterLinkRecord *lrp2p = lsa->GetLinkRecord (j);
              NS_ASSERT_MSG(
                lrp2p->m_linkType == StaticRouterLinkRecord::PointToPoint,
                "StaticRouteManager::SPFIntraAddRouter (): "
                "Expected PointToPoint Link Record");

              StaticRouterLinkRecord *lrstub = lsa->GetLinkRecord (j + 1);
              NS_ASSERT_MSG(
                lrstub->m_linkType == StaticRouterLinkRecord::StubNetwork,
                "StaticRouteManager::SPFIntraAddRouter (): "
                "Expected StubNetwork Link Record");
      
              NS_DEBUG("StaticRouteManager::SPFIntraAddRouter (): "
                "Add route to " << lrp2p->m_linkData <<
                " using next hop " << v->m_nextHop <<
                " via interface " << v->m_rootOif);

              ipv4->AddHostRouteTo(lrp2p->m_linkData, v->m_nextHop, 
                v->m_rootOif);
            }
        }
    }
}

// Derived from quagga ospf_vertex_add_parents()
// Add a vertex to the list of children in each of its parents. 
void
StaticRouteManager::SPFVertexAddParent(SPFVertex* v)
{
  // For now, only one parent (not doing equal-cost multipath)
  v->m_parent->m_children.push_back(v);
}

void
StaticRouteManager::DeleteSPFVertexChain(SPFVertex* spfroot)
{
  // spfroot is the root of all SPFVertex created during the SPF process
  // each vertex has a list of children
  // Recursively, delete all of the SPFVertex children of each SPFVertex
  // then delete root itself
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

  CandidateQueue candidate;

  for (int i = 0; i < 100; ++i)
    {
      SPFVertex *v = new SPFVertex;
      v->m_distanceFromRoot = rand () % 100;
      candidate.Push (v);
    }

  uint32_t lastDistance = 0;

  for (int i = 0; i < 100; ++i)
    {
      SPFVertex *v = candidate.Top ();
      candidate.Pop ();
      if (v->m_distanceFromRoot < lastDistance)
        {
          ok = false;
        }
      lastDistance = v->m_distanceFromRoot;
    }

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

  // Test the database 
  StaticRouteManagerLSDB* srmlsdb = new StaticRouteManagerLSDB();
  srmlsdb->Insert(lsa0->m_linkStateId, lsa0);
  srmlsdb->Insert(lsa1->m_linkStateId, lsa1);
  srmlsdb->Insert(lsa2->m_linkStateId, lsa2);
  srmlsdb->Insert(lsa3->m_linkStateId, lsa3);
  NS_ASSERT(lsa2 == srmlsdb->GetLSA(lsa2->m_linkStateId));

  // We need a dummy node to populate the routing tables
  Ptr<StaticRouterTestNode> n2 = Create<StaticRouterTestNode> ();

  // XXX next, calculate routes based on the manually created LSDB
  StaticRouteManager* srm = new StaticRouteManager();
  srm->DebugUseLsdb (srmlsdb);
  srm->DebugSPFCalculate(lsa0->m_linkStateId);  // node n0

  // This delete clears the srm, which deletes the LSDB, which clears 
  // all of the LSAs, which each destroys the attached LinkRecords.
  delete srm;

  return ok;
}

// Instantiate this class for the unit tests
static StaticRouteManagerTest g_staticRouteManagerTest;

} // namespace ns3

#endif 
