// -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*-
//
// Copyright (c) 2006 Georgia Tech Research Corporation
// All rights reserved.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: George F. Riley<riley@ece.gatech.edu>
//         Gustavo Carneiro <gjc@inescporto.pt>
//

#ifndef IPV4_STATIC_ROUTING_H
#define IPV4_STATIC_ROUTING_H

#include <list>
#include <stdint.h>
#include "ns3/callback-trace-source.h"
#include "ns3/array-trace-resolver.h"
#include "ns3/ipv4-address.h"
#include "ipv4-header.h"
#include "ns3/ptr.h"
#include "ns3/ipv4.h"

namespace ns3 {

class Packet;
class NetDevice;
class Ipv4Interface;
class Ipv4Address;
class Ipv4Header;
class Ipv4Route;
class Node;
class TraceResolver;
class TraceContext;


class Ipv4StaticRouting : public Ipv4RoutingProtocol
{

public:
  Ipv4StaticRouting () : m_defaultRoute (0) {}

  virtual bool RequestRoute (uint32_t ifIndex,
                             Ipv4Header const &ipHeader,
                             Packet packet,
                             RouteReplyCallback routeReply);

  virtual bool RequestIfIndex (Ipv4Address destination, uint32_t& ifIndex);

  void AddHostRouteTo (Ipv4Address dest, 
                       Ipv4Address nextHop, 
                       uint32_t interface);
  void AddHostRouteTo (Ipv4Address dest, 
                       uint32_t interface);

  void AddNetworkRouteTo (Ipv4Address network, 
                          Ipv4Mask networkMask, 
                          Ipv4Address nextHop, 
                          uint32_t interface);
  void AddNetworkRouteTo (Ipv4Address network, 
                          Ipv4Mask networkMask, 
                          uint32_t interface);
  void SetDefaultRoute (Ipv4Address nextHop, 
                        uint32_t interface);
  uint32_t GetNRoutes (void);
  Ipv4Route *GetDefaultRoute (void);
  Ipv4Route *GetRoute (uint32_t i);
  void RemoveRoute (uint32_t i);

  void AddMulticastRoute (Ipv4Address origin,
                          Ipv4Address group,
                          uint32_t inputInterface,
                          std::vector<uint32_t> outputInterfaces);

  uint32_t GetNMulticastRoutes (void) const;
  Ipv4MulticastRoute *GetMulticastRoute (uint32_t i) const;

  void RemoveMulticastRoute (Ipv4Address origin,
                             Ipv4Address group,
                             uint32_t inputInterface);

  void RemoveMulticastRoute (uint32_t index);

protected:
  void DoDispose (void);

private:
  typedef std::list<Ipv4Route *> HostRoutes;
  typedef std::list<Ipv4Route *>::const_iterator HostRoutesCI;
  typedef std::list<Ipv4Route *>::iterator HostRoutesI;
  typedef std::list<Ipv4Route *> NetworkRoutes;
  typedef std::list<Ipv4Route *>::const_iterator NetworkRoutesCI;
  typedef std::list<Ipv4Route *>::iterator NetworkRoutesI;

  typedef std::list<Ipv4MulticastRoute *> MulticastRoutes;
  typedef std::list<Ipv4MulticastRoute *>::const_iterator MulticastRoutesCI;
  typedef std::list<Ipv4MulticastRoute *>::iterator MulticastRoutesI;

  Ipv4Route *LookupStatic (Ipv4Address dest);
  Ipv4MulticastRoute *LookupStatic (Ipv4Address origin, Ipv4Address group,
                                    uint32_t ifIndex);

  HostRoutes m_hostRoutes;
  NetworkRoutes m_networkRoutes;
  Ipv4Route *m_defaultRoute;
  MulticastRoutes m_multicastRoutes;
};

} // Namespace ns3

#endif /* IPV4_STATIC_ROUTING_H */
