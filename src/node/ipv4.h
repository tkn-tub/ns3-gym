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
#include "ns3/object.h"

namespace ns3 {

class NetDevice;
class Packet;
class Ipv4Route;
/**
 * \brief Access to the Ipv4 forwarding table and to the ipv4 interfaces
 *
 * This class allows you to create ipv4 interfaces based on a NetDevice.
 * Multiple interfaces can be created for a single NetDevice, hence
 * achieving multihoming.
 *
 * This class also allows you to control the content of the ipv4 
 * forwarding table.
 */
class IIpv4 : public Object
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
   * Add route to host dest through host nextHop 
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
   * \param device device to add to the list of ipv4 interfaces
   *        which can be used as output interfaces during packet forwarding.
   * \returns the index of the ipv4 interface added.
   *
   * Once a device has been added, it can never be removed: if you want
   * to disable it, you can invoke IIpv4::SetDown which will
   * make sure that it is never used during packet forwarding.
   */
  virtual uint32_t AddInterface (Ptr<NetDevice> device) = 0;
  /**
   * \returns the number of interfaces added by the user.
   */
  virtual uint32_t GetNInterfaces (void) = 0;  

  /**
   * \param i index of ipv4 interface
   * \returns the NetDevice associated with the ipv4 interface index
   */
  virtual Ptr<NetDevice> GetNetDevice (uint32_t i) = 0;

  /**
   * \param i index of ipv4 interface
   * \param address address to associate to the underlying ipv4 interface
   */
  virtual void SetAddress (uint32_t i, Ipv4Address address) = 0;
  /**
   * \param i index of ipv4 interface
   * \param mask mask to associate to the underlying ipv4 interface
   */
  virtual void SetNetworkMask (uint32_t i, Ipv4Mask mask) = 0;
  /**
   * \param i index of ipv4 interface
   * \returns the mask associated to the underlying ipv4 interface
   */
  virtual Ipv4Mask GetNetworkMask (uint32_t i) const = 0;
  /**
   * \param i index of ipv4 interface
   * \returns the address associated to the underlying ipv4 interface
   */
  virtual Ipv4Address GetAddress (uint32_t i) const = 0;
  /**
   * \param i index of ipv4 interface
   * \returns the Maximum Transmission Unit (in bytes) associated
   *          to the underlying ipv4 interface
   */
  virtual uint16_t GetMtu (uint32_t i) const = 0;
  /**
   * \param i index of ipv4 interface
   * \returns true if the underlying interface is in the "up" state,
   *          false otherwise.
   */
  virtual bool IsUp (uint32_t i) const = 0;
  /**
   * \param i index of ipv4 interface
   * 
   * Set the interface into the "up" state. In this state, it is
   * considered valid during ipv4 forwarding.
   */
  virtual void SetUp (uint32_t i) = 0;
  /**
   * \param i index of ipv4 interface
   *
   * Set the interface into the "down" state. In this state, it is
   * ignored during ipv4 forwarding.
   */
  virtual void SetDown (uint32_t i) = 0;
  
};

} // namespace ns3 

#endif /* I_IPV4_H */
