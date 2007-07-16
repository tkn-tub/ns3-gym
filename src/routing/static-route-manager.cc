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
    for ( t_listOfSPFVertex::iterator i = m_children.begin ();
      i != m_children.end ();
      i++)
    {
      SPFVertex *p = *i;
      delete p;
      p = 0;
      *i = 0;
    }
  m_children.clear();
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
    {
      delete m_lsdb;
    }
}

void
StaticRouteManager::DebugUseLsdb (StaticRouteManagerLSDB* lsdb)
{
  if (m_lsdb)
    delete m_lsdb;
  m_lsdb = lsdb;
}

//
// In order to build the routing database, we need to walk the list of nodes
// in the system and look for those that support the StaticRouter interface.
// These routers will export a number of Link State Advertisements (LSAs)
// that describe the links and networks that are "adjacent" (i.e., that are
// on the other side of a point-to-point link).  We take these LSAs and put
// add them to the Link State DataBase (LSDB) from which the routes will 
// ultimately be computed.
//
void
StaticRouteManager::BuildStaticRoutingDatabase () 
{
  NS_DEBUG("StaticRouteManager::BuildStaticRoutingDatabase()");
//
// Walk the list of nodes looking for the StaticRouter Interface.
//
  typedef std::vector < Ptr<Node> >::iterator Iterator;
  for (Iterator i = NodeList::Begin(); i != NodeList::End(); i++)
    {
      Ptr<Node> node = *i;

      Ptr<StaticRouter> rtr = 
        node->QueryInterface<StaticRouter> (StaticRouter::iid);
//      
// Ignore nodes that aren't participating in routing.
//
      if (!rtr)
        {
          continue;
        }
//
// You must call DiscoverLSAs () before trying to use any routing info or to
// update LSAs.  DiscoverLSAs () drives the process of discovering routes in
// the StaticRouter.  Afterward, you may use GetNumLSAs (), which is a very
// computationally inexpensive call.  If you call GetNumLSAs () before calling 
// DiscoverLSAs () will get zero as the number since no routes have been 
// found.
//
      uint32_t numLSAs = rtr->DiscoverLSAs();
      NS_DEBUG ("Discover LSAs:  Found " << numLSAs << " LSAs");

      for (uint32_t j = 0; j < numLSAs; ++j)
        {
          StaticRouterLSA* lsa = new StaticRouterLSA ();
//
// This is the call to actually fetch a Link State Advertisement from the 
// router.
//
          rtr->GetLSA(j, *lsa);
          NS_DEBUG ("LSA " << j);
          NS_DEBUG (*lsa);
//
// Write the newly discovered link state advertisement to the database.
//
          m_lsdb->Insert (lsa->m_linkStateId, lsa); 
        }
    }
}

//
// For each node that is a static router (which is determined by the presence
// of an aggregated StaticRouter interface), run the Dijkstra SPF calculation
// on the database rooted at that router, and populate the node forwarding
// tables.
//
// This function parallels RFC2328, Section 16.1.1, and quagga ospfd
//
// This calculation yields the set of intra-area routes associated
// with an area (called hereafter Area A).  A router calculates the
// shortest-path tree using itself as the root.  The formation
// of the shortest path tree is done here in two stages.  In the
// first stage, only links between routers and transit networks are
// considered.  Using the Dijkstra algorithm, a tree is formed from
// this subset of the link state database.  In the second stage,
// leaves are added to the tree by considering the links to stub
// networks.
//
// The area's link state database is represented as a directed graph.  
// The graph's vertices are routers, transit networks and stub networks.  
//
// The first stage of the procedure (i.e., the Dijkstra algorithm)
// can now be summarized as follows. At each iteration of the
// algorithm, there is a list of candidate vertices.  Paths from
// the root to these vertices have been found, but not necessarily
// the shortest ones.  However, the paths to the candidate vertex
// that is closest to the root are guaranteed to be shortest; this
// vertex is added to the shortest-path tree, removed from the
// candidate list, and its adjacent vertices are examined for
// possible addition to/modification of the candidate list.  The
// algorithm then iterates again.  It terminates when the candidate
// list becomes empty. 
//
void
StaticRouteManager::InitializeRoutes ()
{
  NS_DEBUG("StaticRouteManager::InitializeRoutes ()");
//
// Walk the list of nodes in the system.
//
  typedef std::vector < Ptr<Node> >::iterator Iterator;
  for (Iterator i = NodeList::Begin(); i != NodeList::End(); i++)
    {
      Ptr<Node> node = *i;
//
// Look for the StaticRouter interface that indicates that the node is
// participating in routing.
//
      Ptr<StaticRouter> rtr = 
        node->QueryInterface<StaticRouter> (StaticRouter::iid);
//
// if the node has a static router interface, then run the static routing
// algorithms.
//
      if (rtr && rtr->GetNumLSAs () )
        {
          SPFCalculate(rtr->GetRouterId ());
        }
    }
}

//
// This method is derived from quagga ospf_spf_next().  See RFC2328 Section 
// 16.1 (2) for further details.
//
// We're passed a parameter <v> that is a vertex which is already in the SPF
// tree.  A vertex represents a router node.  We also get a reference to the
// SPF candidate queue, which is a priority queue containing the shortest paths
// to the networks we know about.
//
// We examine the links in v's LSA and update the listof candidates with any
// vertices not already on the list.  If a lower-cost path is found to a
// vertex already on the candidate list, store the new (lower) cost.
//
void
StaticRouteManager::SPFNext(SPFVertex* v, CandidateQueue& candidate)
{
  SPFVertex* w = 0;
  StaticRouterLSA* w_lsa = 0;
  uint32_t distance = 0;

  NS_DEBUG("StaticRouteManager::SPFNext ()");
  if (v->m_vertexType == SPFVertex::VertexRouter) 
    {
//
// Always true for now, since all our LSAs are RouterLSAs.
//
      if (true)
        {
          NS_DEBUG ("SPFNext: Examining " << v->m_vertexId << "'s " <<
            v->m_lsa->m_linkRecords.size() << " link records");
//
// Walk the list of link records in the link state advertisemnt associated with
// the "current" router (represented by vertex <v>).
//
          for ( StaticRouterLSA::ListOfLinkRecords_t::iterator i = 
                v->m_lsa->m_linkRecords.begin();
                i != v->m_lsa->m_linkRecords.end();
                i++ )
            {
//
// (a) If this is a link to a stub network, examine the next link in V's LSA.
// Links to stub networks will be considered in the second stage of the
// shortest path calculation.
//
              StaticRouterLinkRecord* l = *i;
              if (l->m_linkType == StaticRouterLinkRecord::StubNetwork)
                {
                  NS_DEBUG("SPFNext: Found a Stub record to " 
                    << l->m_linkId);
                  continue;
                }
//
// (b) Otherwise, W is a transit vertex (router or transit network).  Look up
// the vertex W's LSA (router-LSA or network-LSA) in Area A's link state
// database. 
//
              if (l->m_linkType == StaticRouterLinkRecord::PointToPoint)
                {
//
// Lookup the link state advertisement of the new link -- we call it <w> in
// the link state database.
//
                  w_lsa = m_lsdb->GetLSA(l->m_linkId);
                  NS_ASSERT(w_lsa);
                  NS_DEBUG("SPFNext:  Found a P2P record from " << 
                    v->m_vertexId << " to " << w_lsa->m_linkStateId);
//
// (c) If vertex W is already on the shortest-path tree, examine the next
// link in the LSA.
//
// If the link is to a router that is already in the shortest path first tree
// then we have it covered -- ignore it.
//
                  if (w_lsa->m_stat == StaticRouterLSA::LSA_SPF_IN_SPFTREE) 
                    {
                      NS_DEBUG("SPFNext: Skipping->  LSA "<< 
                        w_lsa->m_linkStateId << " already in SPF tree");
                      continue;
                    }
//
// The link is to a router we haven't dealt with yet.
//
// (d) Calculate the link state cost D of the resulting path from the root to 
// vertex W.  D is equal to the sum of the link state cost of the (already 
// calculated) shortest path to vertex V and the advertised cost of the link
// between vertices V and W.  
//
                  distance = v->m_distanceFromRoot + l->m_metric;

                  NS_DEBUG("SPFNext: Considering w_lsa " << 
                    w_lsa->m_linkStateId);

                  if (w_lsa->m_stat == StaticRouterLSA::LSA_SPF_NOT_EXPLORED)
                    {
//
// If we havent yet considered the link represented by <w> we have to create 
// a new SPFVertex to represent it.
//
                        w = new SPFVertex(w_lsa);
//
// We need to figure out how to actually get to the new router represented
// by <w>.  This will (among other things0 find the next hop address to send
// packets destined fo this network to, and also find the outbound interface
// used to forward the packets.
//
                      if (SPFNexthopCalculation(v, w, l, distance))
                        {
                          w_lsa->m_stat = StaticRouterLSA::LSA_SPF_CANDIDATE;
//
// Push this new vertex onto the priority queue (ordered by distance from the
// root node).
//
                          candidate.Push(w);
                          NS_DEBUG("SPFNext:  Pushing " << w->m_vertexId
                            << ", parent vertexId: " << v->m_vertexId);
                        }
                    }
                  } else if (w_lsa->m_stat == 
                             StaticRouterLSA::LSA_SPF_CANDIDATE)
                    {
//
// We have already considered the link represented by <w>.  What wse have to
// do now is to decide if this new router represents a route with a shorter
// distance metric.
//
// So, locate the vertex in the candidate queue and take a look at the 
// distance.
                      w = candidate.Find(w_lsa->m_linkStateId);
                      if (w->m_distanceFromRoot < distance)
                        {
//
// This is not a shorter path, so don't do anything.
//
                          continue;
                        }
                       else if (w->m_distanceFromRoot == distance)
                         {
//
// This path is one with an equal cost.  Do nothing for now -- we're not doing
// equal-cost multipath cases yet.
//
                         }
                       else
                         {
// 
// this path represents a new, lower-cost path to <w> (the vertex we found in
// the current link record of the link state advertisement of the current root
// (vertex <v>)
//
// N.B. the nexthop_calculation is conditional, if it finds a valid nexthop
// it will call spf_add_parents, which will flush the old parents
//
                           if (SPFNexthopCalculation(v, w, l, distance))
                             {
//
// If we've changed the cost to get to the vertex represented by <w>, we 
// must reorder the priority queue keyed to that cost.
//
                               candidate.Reorder();
                             }
                         }    
                }  // point-to-point
            } // for loop
        } 
     }
}

//
// This method is derived from quagga ospf_next_hop_calculation() 16.1.1.  
//
// Calculate the nexthop from the root through V (parent) to vertex W 
// (destination), with given distance from root->W.
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
//
// If we're calculating the next hop information from a node (v) that is the 
// root, then we need to store the information needed to forward to the 
// given network (w).  We need to know the interface ID to use to forward the 
// packets, and we need to know the IP address of the router to which we need
// to send the packets (the next hop address).
//
  if (v == m_spfroot)
    {
//
// We're going from the root to a vertex representing a router ...
// 
      if (w->m_vertexType == SPFVertex::VertexRouter) 
        {
//
// We need to find both sides of the link we're examining.  We are considering
// a link "from" vertex v "to" vertex w over the link represented by the link
// record l.  We have the information from the perspective of v, now we need 
// to get the information from the perspective of w, specifically the point
// to point link record describing the link from w to v.
//
          StaticRouterLinkRecord *l2 = 0;
          l2 = SPFGetNextLink(w,v,l2);
// 
// At this point, <l> is the link record from <v> to <w>; and <l2> is the
// link record from <w> to <v>.  The next hop address of the destination is
// the link data field of the static router link record (which is the local IP
// address in the case of a point-to-point link).  This means that in order to
// get to the network w, you send packets to the other side of the point-to-
// point link -- the router on that network.
//
          w->m_nextHop = l2->m_linkData;
// 
// Now find the interface corresponding to the point to point link's IP 
// address.
//
          w->m_rootOif = FindOutgoingInterfaceId(l->m_linkData); 

          NS_DEBUG("SPFNexthopCalculation: Next hop from " << 
            v->m_vertexId << " to " << w->m_vertexId << 
            " goes through next hop " << w->m_nextHop <<
            " via outgoing interface " << w->m_rootOif);
        }
    }
  else 
    {
//
// If we're calculating the next hop information from a node (v) that is 
// *not* the root, then we need to "inherit" the information needed to
// forward from the parent (who will have inherited, ultimately, from the 
// root.
//
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
      v = candidate.Pop();
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

  delete m_spfroot;
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
        "StaticRouteManager::FindOutgoingInterfaceId (): "
        "QI for <StaticRouter> interface failed");
      if (rtr->GetRouterId () == routerId)
        {
          Ptr<Ipv4> ipv4 = node->QueryInterface<Ipv4> (Ipv4::iid);
          NS_ASSERT_MSG(ipv4, 
            "StaticRouteManager::FindOutgoingInterfaceId (): "
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
              StaticRouterLinkRecord *lr = lsa->GetLinkRecord (j);
              if (lr->m_linkType != StaticRouterLinkRecord::PointToPoint)
                {
                  continue;
                }

              NS_DEBUG("StaticRouteManager::SPFIntraAddRouter (): "
                "Add route to " << lr->m_linkData <<
                " using next hop " << v->m_nextHop <<
                " via interface " << v->m_rootOif);

              ipv4->AddHostRouteTo(lr->m_linkData, v->m_nextHop,
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
      SPFVertex *v = candidate.Pop ();
      if (v->m_distanceFromRoot < lastDistance)
        {
          ok = false;
        }
      lastDistance = v->m_distanceFromRoot;
      delete v;
      v = 0;
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

  // XXX next, calculate routes based on the manually created LSDB
  StaticRouteManager* srm = new StaticRouteManager();
  srm->DebugUseLsdb (srmlsdb);  // manually add in an LSDB
  // Note-- this will succeed without any nodes in the topology
  // because the NodeList is empty
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
