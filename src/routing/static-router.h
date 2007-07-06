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
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"

namespace ns3 {

/**
 * \brief A single link record for a link state advertisement
 *
 */
class StaticRouterLinkRecord
{
public:
  uint32_t m_linkId;
  Ipv4Address m_linkData;

  enum LinkType {
    PointToPoint = 1,
    TransitNetwork,
    StubNetwork,
    VirtualLink
  };
  
  uint32_t m_metric;
}

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
  enum LSType {
    RouterLSA = 1,
    NetworkLSA
  } m_LSType;
  uint32_t  m_linkStateId;
  Ipv4Address m_advertising_rtr;
  uint32_t m_numLinks;

  typedef std::list<StaticRouterLinkRecord> type_listOfLinkRecords;
  type_listOfLinkRecords m_listOfLinkRecords;
  type_listOfLinkRecords::iterator m_iter;
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

private:
  Ptr<Node> m_node;
};

} // namespace ns3

#endif /* STATIC_ROUTER_H */
