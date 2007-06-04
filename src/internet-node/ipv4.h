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
#include "ns3/callback-trace-source.h"
#include "ns3/array-trace-resolver.h"
#include "ns3/ipv4-address.h"
#include "ns3/ptr.h"
#include "l3-protocol.h"

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


class Ipv4 : public L3Protocol 
{
public:
  static const uint16_t PROT_NUMBER;

  enum TraceType {
    TX,
    RX,
    DROP,
    INTERFACES,
  };
  typedef ArrayTraceResolver<Ipv4Interface>::Index InterfaceIndex;

  Ipv4(Ptr<Node> node);
  virtual ~Ipv4 ();

  /**
   * \param context the trace context to use to construct the
   *        TraceResolver to return
   * \returns a TraceResolver which can resolve all traces
   *          performed in this object. The caller must
   *          delete the returned object.
   */
  virtual TraceResolver *CreateTraceResolver (TraceContext const &context);

  /**
   * \param ttl default ttl to use
   *
   * When we need to send an ipv4 packet, we use this default
   * ttl value.
   */
  void SetDefaultTtl (uint8_t ttl);

  /**
   * \param device the device to match
   * \returns the matching interface, zero if not found.
   *
   * Try to find an Ipv4Interface whose NetDevice is equal to
   * the input NetDevice.
   */
  Ipv4Interface *FindInterfaceForDevice (Ptr<const NetDevice> device);

  /**
   * Lower layer calls this method after calling L3Demux::Lookup
   * The ARP subclass needs to know from which NetDevice this
   * packet is coming to:
   *    - implement a per-NetDevice ARP cache
   *    - send back arp replies on the right device
   */
  virtual void Receive(Packet& p, Ptr<NetDevice> device);

  /**
   * \param packet packet to send
   * \param source source address of packet
   * \param destination address of packet
   * \param protocol number of packet
   *
   * Higher-level layers call this method to send a packet
   * down the stack to the MAC and PHY layers.
   */
  void Send (Packet const &packet, Ipv4Address source, 
	     Ipv4Address destination, uint8_t protocol);


    
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

  Ipv4Route *Lookup (Ipv4Address dest);
  uint32_t GetNRoutes (void);
  Ipv4Route *GetRoute (uint32_t i);
  void RemoveRoute (uint32_t i);

  uint32_t AddInterface (Ptr<NetDevice> device);
  Ipv4Interface * GetInterface (uint32_t i) const;
  uint32_t GetNInterfaces (void) const;

  
  void SetAddress (uint32_t i, Ipv4Address address);
  void SetNetworkMask (uint32_t i, Ipv4Mask mask);
  Ipv4Mask GetNetworkMask (uint32_t t) const;
  Ipv4Address GetAddress (uint32_t i) const;
  uint16_t GetMtu (uint32_t i) const;
  bool IsUp (uint32_t i) const;
  void SetUp (uint32_t i);
  void SetDown (uint32_t i);


protected:
  virtual void DoDispose (void);
private:
  void SendRealOut (Packet const &packet, Ipv4Header const &ip, Ipv4Route const &route);
  bool Forwarding (Packet const &packet, Ipv4Header &ipHeader, Ptr<NetDevice> device);
  void ForwardUp (Packet p, Ipv4Header const&ip);
  uint32_t AddIpv4Interface (Ipv4Interface *interface);
  void SetupLoopback (void);
  TraceResolver *InterfacesCreateTraceResolver (TraceContext const &context) const;

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
  Ptr<Node> m_node;
  CallbackTraceSource<Packet const &, uint32_t> m_txTrace;
  CallbackTraceSource<Packet const &, uint32_t> m_rxTrace;
  CallbackTraceSource<Packet const &> m_dropTrace;
};

} // Namespace ns3

#endif
