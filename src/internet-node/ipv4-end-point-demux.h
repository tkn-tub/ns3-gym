/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#ifndef IPV4_END_POINT_DEMUX_H
#define IPV4_END_POINT_DEMUX_H

#include <stdint.h>
#include <list>
#include "ns3/ipv4-address.h"

namespace ns3 {

class Ipv4EndPoint;

class Ipv4EndPointDemux {
public:
  Ipv4EndPointDemux ();
  ~Ipv4EndPointDemux ();

  bool LookupPortLocal (uint16_t port);
  bool LookupLocal (Ipv4Address addr, uint16_t port);
  Ipv4EndPoint *Lookup (Ipv4Address daddr, 
                        uint16_t dport, 
                        Ipv4Address saddr, 
                        uint16_t sport);

  Ipv4EndPoint *Allocate (void);
  Ipv4EndPoint *Allocate (Ipv4Address address);
  Ipv4EndPoint *Allocate (uint16_t port);
  Ipv4EndPoint *Allocate (Ipv4Address address, uint16_t port);
  Ipv4EndPoint *Allocate (Ipv4Address localAddress, 
                          uint16_t localPort,
                          Ipv4Address peerAddress, 
                          uint16_t peerPort);

  void DeAllocate (Ipv4EndPoint *endPoint);

 private:
  uint16_t AllocateEphemeralPort (void);
  typedef std::list<Ipv4EndPoint *> EndPoints;
  typedef std::list<Ipv4EndPoint *>::iterator EndPointsI;

  uint16_t m_ephemeral;
  EndPoints m_endPoints;
};

} // namespace ns3

#endif /* IPV4_END_POINTS_H */
