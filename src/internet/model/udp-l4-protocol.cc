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
#include "ns3/object-vector.h"
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
    .AddAttribute ("SocketList", "The list of sockets associated to this protocol.",
                   ObjectVectorValue (),
                   MakeObjectVectorAccessor (&UdpL4Protocol::m_sockets),
                   MakeObjectVectorChecker<UdpSocketImpl> ())
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
  if (m_node == 0)
    {
      Ptr<Node> node = this->GetObject<Node> ();
      if (node != 0)
        {
          Ptr<Ipv4> ipv4 = this->GetObject<Ipv4> ();
          if (ipv4 != 0)
            {
              this->SetNode (node);
              ipv4->Insert (this);
              Ptr<UdpSocketFactoryImpl> udpFactory = CreateObject<UdpSocketFactoryImpl> ();
              udpFactory->SetUdp (this);
              node->AggregateObject (udpFactory);
              this->SetDownTarget (MakeCallback (&Ipv4::Send, ipv4));
            }
        }
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
  for (std::vector<Ptr<UdpSocketImpl> >::iterator i = m_sockets.begin (); i != m_sockets.end (); i++)
    {
      *i = 0;
    }
  m_sockets.clear ();

  if (m_endPoints != 0)
    {
      delete m_endPoints;
      m_endPoints = 0;
    }
  m_node = 0;
  m_downTarget.Nullify ();
/*
 = MakeNullCallback<void,Ptr<Packet>, Ipv4Address, Ipv4Address, uint8_t, Ptr<Ipv4Route> > ();
*/
  Ipv4L4Protocol::DoDispose ();
}

Ptr<Socket>
UdpL4Protocol::CreateSocket (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ptr<UdpSocketImpl> socket = CreateObject<UdpSocketImpl> ();
  socket->SetNode (m_node);
  socket->SetUdp (this);
  m_sockets.push_back (socket);
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
UdpL4Protocol::Receive (Ptr<Packet> packet,
                        Ipv4Header const &header,
                        Ptr<Ipv4Interface> interface)
{
  NS_LOG_FUNCTION (this << packet << header);
  UdpHeader udpHeader;
  if(Node::ChecksumEnabled ())
    {
      udpHeader.EnableChecksums ();
    }

  udpHeader.InitializeChecksum (header.GetSource (), header.GetDestination (), PROT_NUMBER);

  packet->RemoveHeader (udpHeader);

  if(!udpHeader.IsChecksumOk ())
    {
      NS_LOG_INFO ("Bad checksum : dropping packet!");
      return Ipv4L4Protocol::RX_CSUM_FAILED;
    }

  NS_LOG_DEBUG ("Looking up dst " << header.GetDestination () << " port " << udpHeader.GetDestinationPort ()); 
  Ipv4EndPointDemux::EndPoints endPoints =
    m_endPoints->Lookup (header.GetDestination (), udpHeader.GetDestinationPort (),
                         header.GetSource (), udpHeader.GetSourcePort (), interface);
  if (endPoints.empty ())
    {
      NS_LOG_LOGIC ("RX_ENDPOINT_UNREACH");
      return Ipv4L4Protocol::RX_ENDPOINT_UNREACH;
    }
  for (Ipv4EndPointDemux::EndPointsI endPoint = endPoints.begin ();
       endPoint != endPoints.end (); endPoint++)
    {
      (*endPoint)->ForwardUp (packet->Copy (), header, udpHeader.GetSourcePort (), 
                              interface);
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
      udpHeader.EnableChecksums ();
      udpHeader.InitializeChecksum (saddr,
                                    daddr,
                                    PROT_NUMBER);
    }
  udpHeader.SetDestinationPort (dport);
  udpHeader.SetSourcePort (sport);

  packet->AddHeader (udpHeader);

  m_downTarget (packet, saddr, daddr, PROT_NUMBER, 0);
}

void
UdpL4Protocol::Send (Ptr<Packet> packet, 
                     Ipv4Address saddr, Ipv4Address daddr, 
                     uint16_t sport, uint16_t dport, Ptr<Ipv4Route> route)
{
  NS_LOG_FUNCTION (this << packet << saddr << daddr << sport << dport << route);

  UdpHeader udpHeader;
  if(Node::ChecksumEnabled ())
    {
      udpHeader.EnableChecksums ();
      udpHeader.InitializeChecksum (saddr,
                                    daddr,
                                    PROT_NUMBER);
    }
  udpHeader.SetDestinationPort (dport);
  udpHeader.SetSourcePort (sport);

  packet->AddHeader (udpHeader);

  m_downTarget (packet, saddr, daddr, PROT_NUMBER, route);
}

void
UdpL4Protocol::SetDownTarget (Ipv4L4Protocol::DownTargetCallback callback)
{
  m_downTarget = callback;
}

Ipv4L4Protocol::DownTargetCallback
UdpL4Protocol::GetDownTarget (void) const
{
  return m_downTarget;
}

}; // namespace ns3

