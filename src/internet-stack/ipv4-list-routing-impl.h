/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 */

#ifndef IPV4_LIST_ROUTING_IMPL_H
#define IPV4_LIST_ROUTING_IMPL_H

#include <list>
#include "ns3/ipv4-list-routing.h"

namespace ns3 {

class Ipv4StaticRouting;

class Ipv4ListRoutingImpl : public Ipv4ListRouting
{
public:
  static TypeId GetTypeId (void);

  Ipv4ListRoutingImpl ();
  ~Ipv4ListRoutingImpl ();

  virtual Ptr<Ipv4Route> RouteOutput (const Ipv4Header &header, uint32_t oif, Socket::SocketErrno &sockerr);
  virtual bool RouteInput (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev, 
                           UnicastForwardCallback ucb, MulticastForwardCallback mcb, 
                           LocalDeliverCallback lcb, ErrorCallback ecb);

  virtual void AddRoutingProtocol (Ptr<Ipv4RoutingProtocol> routingProtocol, int16_t priority);

  virtual uint32_t GetNRoutingProtocols (void) const;

  virtual Ptr<Ipv4RoutingProtocol> GetRoutingProtocol (uint32_t index, int16_t& priority) const;
  virtual Ptr<Ipv4StaticRouting> GetStaticRouting (void) const;


  void SetNode (Ptr<Node> node);
  Ptr<Node> GetNode (void) const;

protected:
  void DoDispose (void);
private:
  typedef std::list< std::pair< int, Ptr<Ipv4RoutingProtocol> > > Ipv4RoutingProtocolList;
  Ipv4RoutingProtocolList m_routingProtocols;
  Ptr<Node> m_node;

};

} // Namespace ns3

#endif /* IPV4_LIST_ROUTING_IMPL_H */
