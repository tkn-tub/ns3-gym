/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * All rights reserved.
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef I_IPV4_H
#define I_IPV4_H

#include <stdint.h>
#include "ns3/ipv4-address.h"
#include "ns3/interface.h"

namespace ns3 {

class NetDevice;
class Packet;
class Ipv4Route;

class IIpv4 : public Interface
{
public:
  static const InterfaceId iid;
  IIpv4 ();
  virtual ~IIpv4 ();
    
  /**
   * \param dest destination address
   * \param nextHop address of next hop.
   * \param interface interface of next hop.
   *
   * add route to host dest through host nextHop 
   * on interface.
   */
  virtual void AddHostRouteTo (Ipv4Address dest, 
			       Ipv4Address nextHop, 
			       uint32_t interface) = 0;
  /**
   * \param dest destination address
   * \param interface of next hop
   *
   * add route to host dest on interface.
   */
  virtual void AddHostRouteTo (Ipv4Address dest, 
			       uint32_t interface) = 0;

  /**
   * \param network destination network
   * \param networkMask netmask of destination network
   * \param nextHop address of next hop
   * \param interface interface of next hop
   * 
   * add route to network dest with netmask 
   * through host nextHop on interface
   */
  virtual void AddNetworkRouteTo (Ipv4Address network, 
				  Ipv4Mask networkMask, 
				  Ipv4Address nextHop, 
				  uint32_t interface) = 0;

  /**
   * \param network destination network
   * \param networkMask netmask of destination network
   * \param interface interface of next hop
   *
   * add route to network dest with netmask 
   * on interface
   */
  virtual void AddNetworkRouteTo (Ipv4Address network, 
				  Ipv4Mask networkMask, 
				  uint32_t interface) = 0;
  /**
   * \param nextHop address of default next hop
   * \param interface interface of default next hop.
   * 
   * set the default route to host nextHop on
   * interface. 
   */
  virtual void SetDefaultRoute (Ipv4Address nextHop, 
				uint32_t interface) = 0;

  /**
   * \returns the number of entries in the routing table.
   */
  virtual uint32_t GetNRoutes (void) = 0;
  /**
   * \param i index of route to return
   * \returns the route whose index is i
   */
  virtual Ipv4Route *GetRoute (uint32_t i) = 0;
  /**
   * \param i index of route to remove from routing table.
   */
  virtual void RemoveRoute (uint32_t i) = 0;
  
  /**
   * \param interface interface to add to the list of ipv4 interfaces
   * which can be used as output interfaces during packet forwarding.
   * \returns the index of the interface added.
   *
   * Once an interface has been added, it can never be removed: if you want
   * to disable it, you can invoke Ipv4Interface::SetDown which will
   * make sure that it is never used during packet forwarding.
   */
  virtual uint32_t AddInterface (Ptr<NetDevice> device) = 0;
  /**
   * \returns the number of interfaces added by the user.
   */
  virtual uint32_t GetNInterfaces (void) = 0;  

  virtual void SetAddress (uint32_t i, Ipv4Address address) = 0;
  virtual void SetNetworkMask (uint32_t i, Ipv4Mask mask) = 0;
  virtual Ipv4Mask GetNetworkMask (uint32_t t) const = 0;
  virtual Ipv4Address GetAddress (uint32_t i) const = 0;
  virtual uint16_t GetMtu (uint32_t i) const = 0;
  virtual bool IsUp (uint32_t i) const = 0;
  virtual void SetUp (uint32_t i) = 0;
  virtual void SetDown (uint32_t i) = 0;
  
};

} // namespace ns3 

#endif /* I_IPV4_H */
