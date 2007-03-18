/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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
 * Authors: 
 *  Mathieu Lacage <mathieu.lacage@sophia.inria.fr>,
 *  Tom Henderson <tomh@tomh.org>
 */
#ifndef IPV4_INTERFACE_H
#define IPV4_INTERFACE_H

#include <list>
#include "ipv4-address.h"

namespace ns3 {

class NetDevice;
class Packet;
class TraceResolver;
class TraceContext;

/**
 * \brief The IPv4 representation of a network interface
 *
 * This class roughly corresponds to the struct in_device
 * of Linux; the main purpose is to provide address-family
 * specific information (addresses) about an interface.
 *
 * This class defines two APIs:
 *  - the public API which is expected to be used by both 
 *    the IPv4 layer and the user during forwarding and 
 *    configuration.
 *  - the private API which is expected to be implemented
 *    by subclasses of this base class. One such subclass 
 *    will be a Loopback interface which loops every
 *    packet sent back to the ipv4 layer. Another such 
 *    subclass typically contains the Ipv4 <-> MAC address
 *    translation logic which will use most of the time the
 *    ARP/RARP protocols.
 */
class Ipv4Interface 
{
public:
  /**
   * By default, Ipv4 interface are created in the "down" state
   * with ip address 192.168.0.1 and a matching mask. Before
   * becoming useable, the user must invoke SetUp on them
   * once the final Ipv4 address and mask has been set.
   */
  Ipv4Interface (NetDevice *nd);
  virtual ~Ipv4Interface();

  TraceResolver *CreateTraceResolver (TraceContext const &context);
  NetDevice *GetDevice (void) const;

  void SetAddress (Ipv4Address a);
  void SetNetworkMask (Ipv4Mask mask);

  Ipv4Address GetBroadcast (void) const;
  Ipv4Mask GetNetworkMask (void) const;
  Ipv4Address GetAddress (void) const;

  /**
   * This function a pass-through to NetDevice GetMtu, modulo
   * the  LLC/SNAP header i.e., ipv4MTU = NetDeviceMtu - LLCSNAPSIZE
   */
  uint16_t GetMtu (void) const;

  /**
   * These are IP interface states and may be distinct from 
   * NetDevice states, such as found in real implementations
   * (where the device may be down but IP interface state is still up).
   */
  bool IsUp (void) const;
  bool IsDown (void) const;
  void SetUp (void);
  void SetDown (void);

  /**
   * Packet typically received from above will require some
   * handling before calling SendTo()
   */ 
  void Send(Packet p, Ipv4Address dest);


 private:
  virtual void SendTo (Packet p, Ipv4Address dest) = 0;
  virtual TraceResolver *DoCreateTraceResolver (TraceContext const &context) = 0;
  NetDevice* m_netdevice;
  bool m_ifup;
  Ipv4Address m_address;
  Ipv4Mask m_netmask;
};

}; // namespace ns3

#endif
