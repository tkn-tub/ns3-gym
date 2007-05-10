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

#include "ns3/assert.h"
#include "ns3/packet.h"
#include "ns3/empty-trace-resolver.h"
#include "ns3/node.h"

#include "udp.h"
#include "udp-header.h"
#include "ipv4-end-point-demux.h"
#include "ipv4-end-point.h"
#include "ipv4.h"
#include "i-ipv4-private.h"
#include "l3-demux.h"
#include "udp-socket.h"

namespace ns3 {

/* see http://www.iana.org/assignments/protocol-numbers */
const uint8_t Udp::PROT_NUMBER = 17;

Udp::Udp (Ptr<Node> node)
  : Ipv4L4Protocol (PROT_NUMBER, 2),
    m_node (node),
    m_endPoints (new Ipv4EndPointDemux ())
{}

Udp::~Udp ()
{}

TraceResolver *
Udp::CreateTraceResolver (TraceContext const &context)
{
  return new EmptyTraceResolver (context);
}

void
Udp::DoDispose (void)
{
  if (m_endPoints != 0)
    {
      delete m_endPoints;
      m_endPoints = 0;
    }
  m_node = 0;
  Ipv4L4Protocol::DoDispose ();
}

Ptr<Socket>
Udp::CreateSocket (void)
{
  Ptr<Socket> socket = new UdpSocket (m_node, this);
  return socket;
}

Ipv4EndPoint *
Udp::Allocate (void)
{
  return m_endPoints->Allocate ();
}
Ipv4EndPoint *
Udp::Allocate (Ipv4Address address)
{
  return m_endPoints->Allocate (address);
}
Ipv4EndPoint *
Udp::Allocate (uint16_t port)
{
  return m_endPoints->Allocate (port);
}
Ipv4EndPoint *
Udp::Allocate (Ipv4Address address, uint16_t port)
{
  return m_endPoints->Allocate (address, port);
}
Ipv4EndPoint *
Udp::Allocate (Ipv4Address localAddress, uint16_t localPort,
               Ipv4Address peerAddress, uint16_t peerPort)
{
  return m_endPoints->Allocate (localAddress, localPort,
                                peerAddress, peerPort);
}

void 
Udp::DeAllocate (Ipv4EndPoint *endPoint)
{
  m_endPoints->DeAllocate (endPoint);
}

void 
Udp::Receive(Packet& packet, 
             Ipv4Address const &source,
             Ipv4Address const &destination)
{
  UdpHeader udpHeader;
  packet.RemoveHeader (udpHeader);
  Ipv4EndPoint *endPoint = m_endPoints->Lookup (destination, udpHeader.GetDestination (),
                                                source, udpHeader.GetSource ());
  if (endPoint == 0)
    {
      return;
    }
  endPoint->ForwardUp (packet, source, udpHeader.GetSource ());
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
                               PROT_NUMBER);

  packet.AddHeader (udpHeader);

  Ptr<IIpv4Private> ipv4 = m_node->QueryInterface<IIpv4Private> (IIpv4Private::iid);
  if (ipv4 != 0)
    {
      ipv4->Send (packet, saddr, daddr, PROT_NUMBER);
    }
}


}; // namespace ns3

