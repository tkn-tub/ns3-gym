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
//

#ifndef IPV4_H
#define IPV4_H

#include <list>
#include <stdint.h>
#include "ipv4-address.h"
#include "l3-protocol.h"

namespace ns3 {

class Packet;
class NetDevice;
class Ipv4Interface;
class Ipv4Address;
class Ipv4Header;
class Ipv4Route;
class Node;


/**
 * ::Send is always defined in subclasses.
 */
class Ipv4 : public L3Protocol 
{
public:
  static const uint16_t PROT_NUMBER;

  Ipv4(Node *node);
  virtual ~Ipv4 ();

  void SetDefaultTtl (uint8_t ttl);
    
  /* add route to host dest through host nextHop 
   * on interface.
   */
  void AddHostRouteTo (Ipv4Address dest, 
                       Ipv4Address nextHop, 
                       uint32_t interface);
  /* add route to host dest on interface.
   */
  void AddHostRouteTo (Ipv4Address dest, 
                       uint32_t interface);
  /* add route to network dest with netmask 
   * through host nextHop on interface
   */
  void AddNetworkRouteTo (Ipv4Address network, 
                          Ipv4Mask networkMask, 
                          Ipv4Address nextHop, 
                          uint32_t interface);
  /* add route to network dest with netmask 
   * on interface
   */
  void AddNetworkRouteTo (Ipv4Address network, 
                          Ipv4Mask networkMask, 
                          uint32_t interface);
  /* set the default route to host nextHop on
   * interface. 
   */
  void SetDefaultRoute (Ipv4Address nextHop, 
                        uint32_t interface);

  Ipv4Route *Lookup (Ipv4Address dest);

  uint32_t GetNRoutes (void);
  Ipv4Route *GetRoute (uint32_t i);
  void RemoveRoute (uint32_t i);
  
  uint32_t AddInterface (Ipv4Interface *interface);
  Ipv4Interface * GetInterface (uint32_t i);
  uint32_t GetNInterfaces (void) const;
  Ipv4Interface *FindInterfaceForDevice (NetDevice const*device);
  

  virtual Ipv4* Copy(Node *node) const;
  /**
   * Lower layer calls this method after calling L3Demux::Lookup
   * The ARP subclass needs to know from which NetDevice this
   * packet is coming to:
   *    - implement a per-NetDevice ARP cache
   *    - send back arp replies on the right device
   */
  virtual void Receive(Packet& p, NetDevice &device);

  void Send (Packet const &packet, Ipv4Address source, 
	     Ipv4Address destination, uint8_t protocol);

 private:
  void SendRealOut (Packet const &packet, Ipv4Header const &ip, Ipv4Route const &route);
  bool Forwarding (Packet const &packet, Ipv4Header &ipHeader, NetDevice &device);
  void ForwardUp (Packet p, Ipv4Header const&ip);

  typedef std::list<Ipv4Interface*> Ipv4InterfaceList;
  typedef std::list<Ipv4Route *> HostRoutes;
  typedef std::list<Ipv4Route *>::const_iterator HostRoutesCI;
  typedef std::list<Ipv4Route *>::iterator HostRoutesI;
  typedef std::list<Ipv4Route *> NetworkRoutes;
  typedef std::list<Ipv4Route *>::const_iterator NetworkRoutesCI;
  typedef std::list<Ipv4Route *>::iterator NetworkRoutesI;

  Ipv4InterfaceList m_interfaces;
  uint32_t m_nInterfaces;
  uint8_t m_defaultTtl;
  uint16_t m_identification;
  HostRoutes m_hostRoutes;
  NetworkRoutes m_networkRoutes;
  Ipv4Route *m_defaultRoute;
  Node *m_node;
};

} // Namespace ns3

#endif
