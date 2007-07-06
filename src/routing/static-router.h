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

//  
// Roughly equivalent to a static incarnation of the OSPF link state header
// combined with a single Router-LSA Link Record.  Since it's static, there's
// no need for age or sequence number.
//

class StaticRouterLinkRecord
{
public:
  IpV4Address   m_originator;           // Router ID of this router

  enum LinkType {
    PointToPoint = 1,
    TransitNetwork,
    StubNetwork,
    VirtualLink
  };

  LinkType      m_linkType;             // What kind of link this is

  Ipv4Address   m_linkId;               // Neighbor's router ID

  union {
    uint32_t    m_interfaceIndex;       // For unnumbered links
    uint32_t    m_networkMask;          // For Stub Network (self)
  } u;

  uint32_t      m_metric;               // Abstract cost of sending packets
};

//  
// An interface aggregated to a node that provides static routing information
// to a global route manager.  The presence of the interface indicates that
// the node is a router.  The interface is the mechanism by which the router
// advertises its connections to neighboring routers.  We're basically 
// allowing the route manager to query for link state advertisements.
//

class StaticRouter : public Object
{
public:
  static const InterfaceId iid;
  StaticRouter ();

  void SetRouterId (IpV4Address routerId);

  uint32_t GetNumLinkRecords (void);
  bool GetLinkRecord (uint32_t n, StaticRouterLinkRecord &lsa);

protected:
  virtual ~StaticRouter ();

  IpV4Address   m_routerId;             // Router ID of this router

private:
};

} // namespace ns3

#endif /* STATIC_ROUTER_H */
