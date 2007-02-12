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

#include <cassert>
#include "ns3/packet.h"
#include "udp.h"
#include "udp-header.h"
#include "ipv4-end-point-demux.h"
#include "udp-end-point.h"
#include "udp-socket.h"
#include "node.h"
#include "ipv4.h"
#include "l3-demux.h"

namespace ns3 {

/* see http://www.iana.org/assignments/protocol-numbers */
const uint8_t Udp::UDP_PROTOCOL = 17;

Udp::Udp (Node *node)
  : m_node (node),
    m_endPoints (new Ipv4EndPointDemux<UdpEndPoint> ())
{}

Udp::~Udp ()
{
  delete m_endPoints;
}

UdpEndPoint *
Udp::Allocate (void)
{
  return m_endPoints->Allocate ();
}
UdpEndPoint *
Udp::Allocate (Ipv4Address address)
{
  return m_endPoints->Allocate (address);
}
UdpEndPoint *
Udp::Allocate (uint16_t port)
{
  return m_endPoints->Allocate (port);
}
UdpEndPoint *
Udp::Allocate (Ipv4Address address, uint16_t port)
{
  return m_endPoints->Allocate (address, port);
}
UdpEndPoint *
Udp::Allocate (Ipv4Address localAddress, uint16_t localPort,
               Ipv4Address peerAddress, uint16_t peerPort)
{
  return m_endPoints->Allocate (localAddress, localPort,
                                peerAddress, peerPort);
}

Udp* 
Udp::Copy(Node *node) const
{
  return new Udp (node);
}

void 
Udp::Receive(Packet& packet, 
             Ipv4Address const &source,
             Ipv4Address const &destination)
{
  UdpHeader udpHeader;
  packet.Peek (udpHeader);
  packet.Remove (udpHeader);
  UdpEndPoint *endPoint = m_endPoints->Lookup (destination, udpHeader.GetDestination (),
                                               source, udpHeader.GetSource ());
  if (endPoint == 0)
    {
      return;
    }
  UdpSocket *socket = endPoint->GetSocket ();
  socket->ForwardUp (packet, source, udpHeader.GetSource ());
  assert (socket != 0);
}

void
Udp::Send (Packet packet, 
           Ipv4Address saddr, Ipv4Address daddr, 
           uint16_t sport, uint16_t dport)
{
  UdpHeader udpHeader;
  udpHeader.SetDestination (dport);
  udpHeader.SetSource (sport);
  udpHeader.SetPayloadSize (packet.GetSize ());
  udpHeader.InitializeChecksum (saddr,
                               daddr,
                               UDP_PROTOCOL);

  packet.Add (udpHeader);

  Ipv4 *ipv4 = m_node->GetIpv4 ();
  if (ipv4 != 0)
    {
      ipv4->Send (packet, saddr, daddr, UDP_PROTOCOL);
    }
}


}; // namespace ns3

