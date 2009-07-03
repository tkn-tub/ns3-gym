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

#include <list>
#include "ns3/ipv4-routing-protocol.h"

namespace ns3 {

/**
 * \ingroup ipv4Routing 
 *
 * This class is a specialization of Ipv4RoutingProtocol that allows 
 * other instances of Ipv4RoutingProtocol to be inserted in a 
 * prioritized list.  Routing protocols in the list are consulted one
 * by one, from highest to lowest priority, until a routing protocol
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

  Ipv4ListRouting ();  
  virtual ~Ipv4ListRouting ();  

  /**
   * \brief Register a new routing protocol to be used in this IPv4 stack
   *
   * \param routingProtocol new routing protocol implementation object
   * \param priority priority to give to this routing protocol.
   * Values may range between -32768 and +32767.  
   */
  virtual void AddRoutingProtocol (Ptr<Ipv4RoutingProtocol> routingProtocol, int16_t priority);
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
  virtual Ptr<Ipv4RoutingProtocol> GetRoutingProtocol (uint32_t index, int16_t& priority) const;

  // Below are from Ipv4RoutingProtocol
  virtual Ptr<Ipv4Route> RouteOutput (Ptr<Packet> p, const Ipv4Header &header, uint32_t oif, Socket::SocketErrno &sockerr);

  virtual bool RouteInput (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev,
                           UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                           LocalDeliverCallback lcb, ErrorCallback ecb);
  virtual void NotifyInterfaceUp (uint32_t interface);
  virtual void NotifyInterfaceDown (uint32_t interface);
  virtual void NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address);
  virtual void NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address);
  virtual void SetIpv4 (Ptr<Ipv4> ipv4);

protected:
  void DoDispose (void);
private:
  typedef std::pair<int16_t, Ptr<Ipv4RoutingProtocol> > Ipv4RoutingProtocolEntry;
  typedef std::list<Ipv4RoutingProtocolEntry> Ipv4RoutingProtocolList;
  Ipv4RoutingProtocolList m_routingProtocols;
  static bool Compare (const Ipv4RoutingProtocolEntry& a, const Ipv4RoutingProtocolEntry& b);
  Ptr<Ipv4> m_ipv4;

};

} //namespace ns3

#endif /* IPV4_LIST_ROUTING_H */
