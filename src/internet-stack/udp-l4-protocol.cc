/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/boolean.h"
#include "ns3/ipv4-route.h"

#include "udp-l4-protocol.h"
#include "udp-header.h"
#include "udp-socket-factory-impl.h"
#include "ipv4-end-point-demux.h"
#include "ipv4-end-point.h"
#include "ipv4-l3-protocol.h"
#include "udp-socket-impl.h"

NS_LOG_COMPONENT_DEFINE ("UdpL4Protocol");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (UdpL4Protocol);

/* see http://www.iana.org/assignments/protocol-numbers */
const uint8_t UdpL4Protocol::PROT_NUMBER = 17;

TypeId 
UdpL4Protocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UdpL4Protocol")
    .SetParent<Ipv4L4Protocol> ()
    .AddConstructor<UdpL4Protocol> ()
    ;
  return tid;
}

UdpL4Protocol::UdpL4Protocol ()
  : m_endPoints (new Ipv4EndPointDemux ())
{
  NS_LOG_FUNCTION_NOARGS ();
}

UdpL4Protocol::~UdpL4Protocol ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void 
UdpL4Protocol::SetNode (Ptr<Node> node)
{
  m_node = node;
}

/*
 * This method is called by AddAgregate and completes the aggregation
 * by setting the node in the udp stack and link it to the ipv4 object
 * present in the node along with the socket factory
 */
void
UdpL4Protocol::NotifyNewAggregate ()
{  
  bool is_not_initialized = (m_node == 0);
  Ptr<Node>node = this->GetObject<Node> ();
  Ptr<Ipv4L3Protocol> ipv4 = this->GetObject<Ipv4L3Protocol> ();
  if (is_not_initialized && node!= 0 && ipv4 != 0)
    {
      this->SetNode (node);
      ipv4->Insert (this);
      Ptr<UdpSocketFactoryImpl> udpFactory = CreateObject<UdpSocketFactoryImpl> ();
      udpFactory->SetUdp (this);
      node->AggregateObject (udpFactory);
    }
  Object::NotifyNewAggregate ();
}

int 
UdpL4Protocol::GetProtocolNumber (void) const
{
  return PROT_NUMBER;
}


void
UdpL4Protocol::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_endPoints != 0)
    {
      delete m_endPoints;
      m_endPoints = 0;
    }
  m_node = 0;
  Ipv4L4Protocol::DoDispose ();
}

Ptr<Socket>
UdpL4Protocol::CreateSocket (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ptr<UdpSocketImpl> socket = CreateObject<UdpSocketImpl> ();
  socket->SetNode (m_node);
  socket->SetUdp (this);
  return socket;
}

Ipv4EndPoint *
UdpL4Protocol::Allocate (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_endPoints->Allocate ();
}

Ipv4EndPoint *
UdpL4Protocol::Allocate (Ipv4Address address)
{
  NS_LOG_FUNCTION (this << address);
  return m_endPoints->Allocate (address);
}

Ipv4EndPoint *
UdpL4Protocol::Allocate (uint16_t port)
{
  NS_LOG_FUNCTION (this << port);
  return m_endPoints->Allocate (port);
}

Ipv4EndPoint *
UdpL4Protocol::Allocate (Ipv4Address address, uint16_t port)
{
  NS_LOG_FUNCTION (this << address << port);
  return m_endPoints->Allocate (address, port);
}
Ipv4EndPoint *
UdpL4Protocol::Allocate (Ipv4Address localAddress, uint16_t localPort,
                         Ipv4Address peerAddress, uint16_t peerPort)
{
  NS_LOG_FUNCTION (this << localAddress << localPort << peerAddress << peerPort);
  return m_endPoints->Allocate (localAddress, localPort,
                                peerAddress, peerPort);
}

void 
UdpL4Protocol::DeAllocate (Ipv4EndPoint *endPoint)
{
  NS_LOG_FUNCTION (this << endPoint);
  m_endPoints->DeAllocate (endPoint);
}

void 
UdpL4Protocol::ReceiveIcmp (Ipv4Address icmpSource, uint8_t icmpTtl,
                            uint8_t icmpType, uint8_t icmpCode, uint32_t icmpInfo,
                            Ipv4Address payloadSource,Ipv4Address payloadDestination,
                            const uint8_t payload[8])
{
  NS_LOG_FUNCTION (this << icmpSource << icmpTtl << icmpType << icmpCode << icmpInfo 
                   << payloadSource << payloadDestination);
  uint16_t src, dst;
  src = payload[0] << 8;
  src |= payload[1];
  dst = payload[2] << 8;
  dst |= payload[3];

  Ipv4EndPoint *endPoint = m_endPoints->SimpleLookup (payloadSource, src, payloadDestination, dst);
  if (endPoint != 0)
    {
      endPoint->ForwardIcmp (icmpSource, icmpTtl, icmpType, icmpCode, icmpInfo);
    }
  else
    {
      NS_LOG_DEBUG ("no endpoint found source=" << payloadSource <<
                    ", destination="<<payloadDestination<<
                    ", src=" << src << ", dst=" << dst);
    }
}

enum Ipv4L4Protocol::RxStatus
UdpL4Protocol::Receive(Ptr<Packet> packet, 
                       Ipv4Address const &source,
                       Ipv4Address const &destination,
                       Ptr<Ipv4Interface> interface)
{
  NS_LOG_FUNCTION (this << packet << source << destination);
  UdpHeader udpHeader;
  if(Node::ChecksumEnabled ())
  {
    udpHeader.EnableChecksums();
  }

  udpHeader.InitializeChecksum (source, destination, PROT_NUMBER);

  packet->RemoveHeader (udpHeader);

  if(!udpHeader.IsChecksumOk ())
  {
    NS_LOG_INFO("Bad checksum : dropping packet!");
    return Ipv4L4Protocol::RX_CSUM_FAILED;
  }

  Ipv4EndPointDemux::EndPoints endPoints =
    m_endPoints->Lookup (destination, udpHeader.GetDestinationPort (),
                         source, udpHeader.GetSourcePort (), interface);
  if (endPoints.empty ())
    {
      NS_LOG_LOGIC ("RX_ENDPOINT_UNREACH");
      return Ipv4L4Protocol::RX_ENDPOINT_UNREACH;
    }
  for (Ipv4EndPointDemux::EndPointsI endPoint = endPoints.begin ();
       endPoint != endPoints.end (); endPoint++)
    {
      (*endPoint)->ForwardUp (packet->Copy (), source, udpHeader.GetSourcePort ());
    }
  return Ipv4L4Protocol::RX_OK;
}

void
UdpL4Protocol::Send (Ptr<Packet> packet, 
                     Ipv4Address saddr, Ipv4Address daddr, 
                     uint16_t sport, uint16_t dport)
{
  NS_LOG_FUNCTION (this << packet << saddr << daddr << sport << dport);

  UdpHeader udpHeader;
  if(Node::ChecksumEnabled ())
  {
    udpHeader.EnableChecksums();
    udpHeader.InitializeChecksum (saddr,
                                  daddr,
                                  PROT_NUMBER);
  }
  udpHeader.SetDestinationPort (dport);
  udpHeader.SetSourcePort (sport);

  packet->AddHeader (udpHeader);

  Ptr<Ipv4L3Protocol> ipv4 = m_node->GetObject<Ipv4L3Protocol> ();
  if (ipv4 != 0)
    {
      NS_LOG_LOGIC ("Sending to IP");
      // Send with null route
      ipv4->Send (packet, saddr, daddr, PROT_NUMBER, 0);
    }
}

void
UdpL4Protocol::Send (Ptr<Packet> packet, 
                     Ipv4Address saddr, Ipv4Address daddr, 
                     uint16_t sport, uint16_t dport, Ptr<Ipv4Route> route)
{
  NS_LOG_FUNCTION (this << packet << saddr << daddr << sport << dport);

  UdpHeader udpHeader;
  if(Node::ChecksumEnabled ())
  {
    udpHeader.EnableChecksums();
    udpHeader.InitializeChecksum (saddr,
                                  daddr,
                                  PROT_NUMBER);
  }
  udpHeader.SetDestinationPort (dport);
  udpHeader.SetSourcePort (sport);

  packet->AddHeader (udpHeader);

  Ptr<Ipv4L3Protocol> ipv4 = m_node->GetObject<Ipv4L3Protocol> ();
  if (ipv4 != 0)
    {
      NS_LOG_LOGIC ("Sending to IP");
      ipv4->Send (packet, saddr, daddr, PROT_NUMBER, route);
    }
}


}; // namespace ns3

