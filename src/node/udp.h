/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#ifndef UDP_H
#define UDP_H

#include <stdint.h>

#include "ns3/ipv4-address.h"
#include "ns3/packet.h"
#include "ipv4-l4-protocol.h"
#include "ipv4-end-point-demux.h"
#include "udp-end-point.h"

namespace ns3 {

class Udp : public Ipv4L4Protocol {
public:
  Udp ();
  virtual ~Udp ();

  UdpEndPoint *Allocate (void);
  UdpEndPoint *Allocate (Ipv4Address address);
  UdpEndPoint *Allocate (Ipv4Address address, uint16_t port);
  UdpEndPoint *Allocate (Ipv4Address localAddress, uint16_t localPort,
                         Ipv4Address peerAddress, uint16_t peerPort);

  void Send (Packet packet);
  // inherited from Ipv4L4Protocol
  virtual Ipv4L4Protocol* Copy() const;
  virtual void Receive(Packet& p, 
                       Ipv4Address const &source,
                       Ipv4Address const &destination);
 private:
  static const uint8_t UDP_PROTOCOL;
  //Ipv4 *m_ipv4;
  Ipv4EndPointDemux<UdpEndPoint> *m_endPoints;
};

}; // namespace ns3

#endif /* UDP_H */
