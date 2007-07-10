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
#ifndef STATIC_ROUTER_H
#define STATIC_ROUTER_H

#include <stdint.h>
#include <list>
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/channel.h"
#include "ns3/ipv4-address.h"

namespace ns3 {

/**
 * \brief A single link record for a link state advertisement
 *
 * For Type 3 link (Stub), 
 */
class StaticRouterLinkRecord
{
public:
  //
  // For Type 1 link (PointToPoint), set m_linkId to Router ID of 
  // neighboring router.
  // 
  // For Type 3 link (Stub), set m_linkId to neighbor's IP address
  //  
  Ipv4Address m_linkId;         

  //
  // For Type 1 link (PointToPoint), set m_linkData to local IP address  
  // 
  // For Type 3 link (Stub), set m_linkData to mask 0xffffffff
  //
  Ipv4Address m_linkData;    // for links to RouterLSA, 

  enum LinkType {
    PointToPoint = 1,
    TransitNetwork,
    StubNetwork,
    VirtualLink
  } m_linkType;
  
  uint32_t m_metric;  
};

/**  
 * \brief a Link State Advertisement (LSA) for a router, used in static routing
 * 
 * Roughly equivalent to a static incarnation of the OSPF link state header
 * combined with a list of Link Records.  Since it's static, there's
 * no need for age or sequence number.  See RFC 2328, Appendix A.
 */
class StaticRouterLSA
{
public:
  StaticRouterLSA();
  ~StaticRouterLSA();

  uint32_t AddLinkRecord (StaticRouterLinkRecord* lr);

  Ipv4Address  m_linkStateId;     // set to the NodeId
  Ipv4Address  m_advertisingRtr;  // set to the NodeId

  typedef std::list<StaticRouterLinkRecord*> ListOfLinkRecords_t;
  ListOfLinkRecords_t m_linkRecords;
};

/**
 * \brief An interface aggregated to a node to provide static routing info
 *
 * An interface aggregated to a node that provides static routing information
 * to a global route manager.  The presence of the interface indicates that
 * the node is a router.  The interface is the mechanism by which the router
 * advertises its connections to neighboring routers.  We're basically 
 * allowing the route manager to query for link state advertisements.
 */
class StaticRouter : public Object
{
public:
  static const InterfaceId iid;
  StaticRouter (Ptr<Node> node);

  uint32_t GetNumLSAs (void);
  bool GetLSA (uint32_t n, StaticRouterLSA &lsa);

protected:
  virtual ~StaticRouter ();

  Ptr<Node>     m_node;
  uint32_t      m_numLSAs;

  Ptr<NetDevice> GetAdjacent(Ptr<NetDevice> nd, Ptr<Channel> ch);
  uint32_t FindIfIndexForDevice(Ptr<Node> node, Ptr<NetDevice> nd);

private:
};

} // namespace ns3

#endif /* STATIC_ROUTER_H */
