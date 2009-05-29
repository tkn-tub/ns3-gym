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
 */

#ifndef IPV4_LIST_ROUTING_H
#define IPV4_LIST_ROUTING_H

#include "ipv4-routing-protocol.h"

namespace ns3 {

/**
 * \ingroup ipv4 
 */

/**
 * This class is a specialization of Ipv4RoutingProtocol that allows 
 * other instances of Ipv4RoutingProtocol to be inserted in a 
 * prioritized list.  Routing protocols in the list are consulted one
 * by one, from highest to lowest proirity, until a routing protocol
 * is found that will take the packet (this corresponds to a non-zero
 * return value to RouteOutput, or a return value of true to RouteInput).
 * The order by which routing protocols with the same priority value 
 * are consulted is undefined.
 * 
 */
class Ipv4ListRouting : public Ipv4RoutingProtocol
{
public:
  static TypeId GetTypeId (void);
  /**
   * \brief Register a new routing protocol to be used in this IPv4 stack
   *
   * \param routingProtocol new routing protocol implementation object
   * \param priority priority to give to this routing protocol.
   * Values may range between -32768 and +32767.  
   */
  virtual void AddRoutingProtocol (Ptr<Ipv4RoutingProtocol> routingProtocol, int16_t priority) = 0;

  virtual uint32_t GetNRoutingProtocols (void) const = 0;

  virtual Ptr<Ipv4RoutingProtocol> GetRoutingProtocol (uint32_t index, int16_t& priority) const = 0;
};

} //namespace ns3

#endif /* IPV4_LIST_ROUTING_H */
