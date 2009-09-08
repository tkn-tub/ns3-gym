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

#ifndef IPV6_LIST_ROUTING_H
#define IPV6_LIST_ROUTING_H

#include <list>
#include "ns3/ipv6-routing-protocol.h"

namespace ns3 {

/**
 * \ingroup routing 
 * \defgroup ipv6ListRouting Ipv6 List Routing
 */
/**
 * \ingroup ipv6ListRouting
 *
 * This class is a specialization of Ipv6RoutingProtocol that allows 
 * other instances of Ipv6RoutingProtocol to be inserted in a 
 * prioritized list.  Routing protocols in the list are consulted one
 * by one, from highest to lowest priority, until a routing protocol
 * is found that will take the packet (this corresponds to a non-zero
 * return value to RouteOutput, or a return value of true to RouteInput).
 * The order by which routing protocols with the same priority value 
 * are consulted is undefined.
 * 
 */
class Ipv6ListRouting : public Ipv6RoutingProtocol
{
public:
  static TypeId GetTypeId (void);

  Ipv6ListRouting ();  
  virtual ~Ipv6ListRouting ();  

  /**
   * \brief Register a new routing protocol to be used in this IPv4 stack
   *
   * \param routingProtocol new routing protocol implementation object
   * \param priority priority to give to this routing protocol.
   * Values may range between -32768 and +32767.  
   */
  virtual void AddRoutingProtocol (Ptr<Ipv6RoutingProtocol> routingProtocol, int16_t priority);
  /**
   * \return number of routing protocols in the list
   */
  virtual uint32_t GetNRoutingProtocols (void) const;
  /**
   * Return pointer to routing protocol stored at index, with the
   * first protocol (index 0) the highest priority, the next one (index 1)
   * the second highest priority, and so on.  The priority parameter is an
   * output parameter and it returns the integer priority of the protocol.
   * 
   * \return pointer to routing protocol indexed by 
   * \param index index of protocol to return
   * \param priority output parameter, set to the priority of the protocol
            being returned
   */
  virtual Ptr<Ipv6RoutingProtocol> GetRoutingProtocol (uint32_t index, int16_t& priority) const;

  // Below are from Ipv6RoutingProtocol
  virtual Ptr<Ipv6Route> RouteOutput (Ptr<Packet> p, const Ipv6Header &header, uint32_t oif, Socket::SocketErrno &sockerr);

  virtual bool RouteInput (Ptr<const Packet> p, const Ipv6Header &header, Ptr<const NetDevice> idev,
                           UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                           LocalDeliverCallback lcb, ErrorCallback ecb);
  virtual void NotifyInterfaceUp (uint32_t interface);
  virtual void NotifyInterfaceDown (uint32_t interface);
  virtual void NotifyAddAddress (uint32_t interface, Ipv6InterfaceAddress address);
  virtual void NotifyRemoveAddress (uint32_t interface, Ipv6InterfaceAddress address);
  virtual void NotifyAddRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse = Ipv6Address::GetZero ());
  virtual void NotifyRemoveRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse = Ipv6Address::GetZero ());
  virtual void SetIpv6 (Ptr<Ipv6> ipv6);

protected:
  void DoDispose (void);
private:
  typedef std::pair<int16_t, Ptr<Ipv6RoutingProtocol> > Ipv6RoutingProtocolEntry;
  typedef std::list<Ipv6RoutingProtocolEntry> Ipv6RoutingProtocolList;
  Ipv6RoutingProtocolList m_routingProtocols;
  static bool Compare (const Ipv6RoutingProtocolEntry& a, const Ipv6RoutingProtocolEntry& b);
  Ptr<Ipv6> m_ipv6;

};

} //namespace ns3

#endif /* IPV6_LIST_ROUTING_H */

