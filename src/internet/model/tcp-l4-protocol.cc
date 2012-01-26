/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Georgia Tech Research Corporation
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
 * Author: Raj Bhattacharjea <raj.b@gatech.edu>
 */

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/nstime.h"
#include "ns3/boolean.h"
#include "ns3/object-vector.h"

#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/simulator.h"
#include "ns3/ipv4-route.h"

#include "tcp-l4-protocol.h"
#include "tcp-header.h"
#include "ipv4-end-point-demux.h"
#include "ipv4-end-point.h"
#include "ipv4-l3-protocol.h"
#include "tcp-socket-factory-impl.h"
#include "tcp-newreno.h"
#include "rtt-estimator.h"

#include <vector>
#include <sstream>
#include <iomanip>

NS_LOG_COMPONENT_DEFINE ("TcpL4Protocol");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (TcpL4Protocol);

//TcpL4Protocol stuff----------------------------------------------------------

#undef NS_LOG_APPEND_CONTEXT
#define NS_LOG_APPEND_CONTEXT                                   \
  if (m_node) { std::clog << Simulator::Now ().GetSeconds () << " [node " << m_node->GetId () << "] "; } 

/* see http://www.iana.org/assignments/protocol-numbers */
const uint8_t TcpL4Protocol::PROT_NUMBER = 6;

TypeId 
TcpL4Protocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpL4Protocol")
    .SetParent<Ipv4L4Protocol> ()
    .AddConstructor<TcpL4Protocol> ()
    .AddAttribute ("RttEstimatorType",
                   "Type of RttEstimator objects.",
                   TypeIdValue (RttMeanDeviation::GetTypeId ()),
                   MakeTypeIdAccessor (&TcpL4Protocol::m_rttTypeId),
                   MakeTypeIdChecker ())
    .AddAttribute ("SocketType",
                   "Socket type of TCP objects.",
                   TypeIdValue (TcpNewReno::GetTypeId ()),
                   MakeTypeIdAccessor (&TcpL4Protocol::m_socketTypeId),
                   MakeTypeIdChecker ())
    .AddAttribute ("SocketList", "The list of sockets associated to this protocol.",
                   ObjectVectorValue (),
                   MakeObjectVectorAccessor (&TcpL4Protocol::m_sockets),
                   MakeObjectVectorChecker<TcpSocketBase> ())
  ;
  return tid;
}

TcpL4Protocol::TcpL4Protocol ()
  : m_endPoints (new Ipv4EndPointDemux ())
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_LOG_LOGIC ("Made a TcpL4Protocol "<<this);
}

TcpL4Protocol::~TcpL4Protocol ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void 
TcpL4Protocol::SetNode (Ptr<Node> node)
{
  m_node = node;
}

/* 
 * This method is called by AddAgregate and completes the aggregation
 * by setting the node in the TCP stack, link it to the ipv4 stack and 
 * adding TCP socket factory to the node.
 */
void
TcpL4Protocol::NotifyNewAggregate ()
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
              Ptr<TcpSocketFactoryImpl> tcpFactory = CreateObject<TcpSocketFactoryImpl> ();
              tcpFactory->SetTcp (this);
              node->AggregateObject (tcpFactory);
              this->SetDownTarget (MakeCallback (&Ipv4::Send, ipv4));
            }
        }
    }
  Object::NotifyNewAggregate ();
}

int 
TcpL4Protocol::GetProtocolNumber (void) const
{
  return PROT_NUMBER;
}

void
TcpL4Protocol::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_sockets.clear ();

  if (m_endPoints != 0)
    {
      delete m_endPoints;
      m_endPoints = 0;
    }

  m_node = 0;
  m_downTarget.Nullify ();
  Ipv4L4Protocol::DoDispose ();
}

Ptr<Socket>
TcpL4Protocol::CreateSocket (TypeId socketTypeId)
{
  NS_LOG_FUNCTION_NOARGS ();
  ObjectFactory rttFactory;
  ObjectFactory socketFactory;
  rttFactory.SetTypeId (m_rttTypeId);
  socketFactory.SetTypeId (socketTypeId);
  Ptr<RttEstimator> rtt = rttFactory.Create<RttEstimator> ();
  Ptr<TcpSocketBase> socket = socketFactory.Create<TcpSocketBase> ();
  socket->SetNode (m_node);
  socket->SetTcp (this);
  socket->SetRtt (rtt);
  return socket;
}

Ptr<Socket>
TcpL4Protocol::CreateSocket (void)
{
  return CreateSocket (m_socketTypeId);
}

Ipv4EndPoint *
TcpL4Protocol::Allocate (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_endPoints->Allocate ();
}

Ipv4EndPoint *
TcpL4Protocol::Allocate (Ipv4Address address)
{
  NS_LOG_FUNCTION (this << address);
  return m_endPoints->Allocate (address);
}

Ipv4EndPoint *
TcpL4Protocol::Allocate (uint16_t port)
{
  NS_LOG_FUNCTION (this << port);
  return m_endPoints->Allocate (port);
}

Ipv4EndPoint *
TcpL4Protocol::Allocate (Ipv4Address address, uint16_t port)
{
  NS_LOG_FUNCTION (this << address << port);
  return m_endPoints->Allocate (address, port);
}

Ipv4EndPoint *
TcpL4Protocol::Allocate (Ipv4Address localAddress, uint16_t localPort,
                         Ipv4Address peerAddress, uint16_t peerPort)
{
  NS_LOG_FUNCTION (this << localAddress << localPort << peerAddress << peerPort);
  return m_endPoints->Allocate (localAddress, localPort,
                                peerAddress, peerPort);
}

void 
TcpL4Protocol::DeAllocate (Ipv4EndPoint *endPoint)
{
  NS_LOG_FUNCTION (this << endPoint);
  m_endPoints->DeAllocate (endPoint);
}

enum Ipv4L4Protocol::RxStatus
TcpL4Protocol::Receive (Ptr<Packet> packet,
                        Ipv4Header const &ipHeader,
                        Ptr<Ipv4Interface> incomingInterface)
{
  NS_LOG_FUNCTION (this << packet << ipHeader << incomingInterface);

  TcpHeader tcpHeader;
  if(Node::ChecksumEnabled ())
    {
      tcpHeader.EnableChecksums ();
      tcpHeader.InitializeChecksum (ipHeader.GetSource (), ipHeader.GetDestination (), PROT_NUMBER);
    }

  packet->PeekHeader (tcpHeader);

  NS_LOG_LOGIC ("TcpL4Protocol " << this
                                 << " receiving seq " << tcpHeader.GetSequenceNumber ()
                                 << " ack " << tcpHeader.GetAckNumber ()
                                 << " flags "<< std::hex << (int)tcpHeader.GetFlags () << std::dec
                                 << " data size " << packet->GetSize ());

  if(!tcpHeader.IsChecksumOk ())
    {
      NS_LOG_INFO ("Bad checksum, dropping packet!");
      return Ipv4L4Protocol::RX_CSUM_FAILED;
    }

  NS_LOG_LOGIC ("TcpL4Protocol "<<this<<" received a packet");
  Ipv4EndPointDemux::EndPoints endPoints =
    m_endPoints->Lookup (ipHeader.GetDestination (), tcpHeader.GetDestinationPort (),
                         ipHeader.GetSource (), tcpHeader.GetSourcePort (),incomingInterface);
  if (endPoints.empty ())
    {
      NS_LOG_LOGIC ("  No endpoints matched on TcpL4Protocol "<<this);
      std::ostringstream oss;
      oss<<"  destination IP: ";
      ipHeader.GetDestination ().Print (oss);
      oss<<" destination port: "<< tcpHeader.GetDestinationPort ()<<" source IP: ";
      ipHeader.GetSource ().Print (oss);
      oss<<" source port: "<<tcpHeader.GetSourcePort ();
      NS_LOG_LOGIC (oss.str ());

      if (!(tcpHeader.GetFlags () & TcpHeader::RST))
        {
          // build a RST packet and send
          Ptr<Packet> rstPacket = Create<Packet> ();
          TcpHeader header;
          if (tcpHeader.GetFlags () & TcpHeader::ACK)
            {
              // ACK bit was set
              header.SetFlags (TcpHeader::RST);
              header.SetSequenceNumber (header.GetAckNumber ());
            }
          else
            {
              header.SetFlags (TcpHeader::RST | TcpHeader::ACK);
              header.SetSequenceNumber (SequenceNumber32 (0));
              header.SetAckNumber (header.GetSequenceNumber () + SequenceNumber32 (1));
            }
          header.SetSourcePort (tcpHeader.GetDestinationPort ());
          header.SetDestinationPort (tcpHeader.GetSourcePort ());
          SendPacket (rstPacket, header, ipHeader.GetDestination (), ipHeader.GetSource ());
          return Ipv4L4Protocol::RX_ENDPOINT_CLOSED;
        }
      else
        {
          return Ipv4L4Protocol::RX_ENDPOINT_CLOSED;
        }
    }
  NS_ASSERT_MSG (endPoints.size () == 1, "Demux returned more than one endpoint");
  NS_LOG_LOGIC ("TcpL4Protocol "<<this<<" forwarding up to endpoint/socket");
  (*endPoints.begin ())->ForwardUp (packet, ipHeader, tcpHeader.GetSourcePort (), 
                                    incomingInterface);
  return Ipv4L4Protocol::RX_OK;
}

void
TcpL4Protocol::Send (Ptr<Packet> packet, 
                     Ipv4Address saddr, Ipv4Address daddr,
                     uint16_t sport, uint16_t dport, Ptr<NetDevice> oif)
{
  NS_LOG_FUNCTION (this << packet << saddr << daddr << sport << dport << oif);

  TcpHeader tcpHeader;
  tcpHeader.SetDestinationPort (dport);
  tcpHeader.SetSourcePort (sport);
  if(Node::ChecksumEnabled ())
    {
      tcpHeader.EnableChecksums ();
    }
  tcpHeader.InitializeChecksum (saddr,
                                daddr,
                                PROT_NUMBER);
  tcpHeader.SetFlags (TcpHeader::ACK);
  tcpHeader.SetAckNumber (SequenceNumber32 (0));

  packet->AddHeader (tcpHeader);

  Ptr<Ipv4> ipv4 = m_node->GetObject<Ipv4> ();
  if (ipv4 != 0)
    {
      Ipv4Header header;
      header.SetDestination (daddr);
      header.SetProtocol (PROT_NUMBER);
      Socket::SocketErrno errno_;
      Ptr<Ipv4Route> route;
      Ptr<NetDevice> oif (0); //specify non-zero if bound to a source address
      if (ipv4->GetRoutingProtocol () != 0)
        {
          route = ipv4->GetRoutingProtocol ()->RouteOutput (packet, header, oif, errno_);
        }
      else
        {
          NS_LOG_ERROR ("No IPV4 Routing Protocol");
          route = 0;
        }
      ipv4->Send (packet, saddr, daddr, PROT_NUMBER, route);
    }
}

void
TcpL4Protocol::SendPacket (Ptr<Packet> packet, const TcpHeader &outgoing,
                           Ipv4Address saddr, Ipv4Address daddr, Ptr<NetDevice> oif)
{
  NS_LOG_LOGIC ("TcpL4Protocol " << this
                                 << " sending seq " << outgoing.GetSequenceNumber ()
                                 << " ack " << outgoing.GetAckNumber ()
                                 << " flags " << std::hex << (int)outgoing.GetFlags () << std::dec
                                 << " data size " << packet->GetSize ());
  NS_LOG_FUNCTION (this << packet << saddr << daddr << oif);
  // XXX outgoingHeader cannot be logged

  TcpHeader outgoingHeader = outgoing;
  outgoingHeader.SetLength (5); //header length in units of 32bit words
  /* outgoingHeader.SetUrgentPointer (0); //XXX */
  if(Node::ChecksumEnabled ())
    {
      outgoingHeader.EnableChecksums ();
    }
  outgoingHeader.InitializeChecksum (saddr, daddr, PROT_NUMBER);

  packet->AddHeader (outgoingHeader);

  Ptr<Ipv4> ipv4 = 
    m_node->GetObject<Ipv4> ();
  if (ipv4 != 0)
    {
      Ipv4Header header;
      header.SetDestination (daddr);
      header.SetProtocol (PROT_NUMBER);
      Socket::SocketErrno errno_;
      Ptr<Ipv4Route> route;
      if (ipv4->GetRoutingProtocol () != 0)
        {
          route = ipv4->GetRoutingProtocol ()->RouteOutput (packet, header, oif, errno_);
        }
      else
        {
          NS_LOG_ERROR ("No IPV4 Routing Protocol");
          route = 0;
        }
      m_downTarget (packet, saddr, daddr, PROT_NUMBER, route);
    }
  else
    NS_FATAL_ERROR ("Trying to use Tcp on a node without an Ipv4 interface");
}

void
TcpL4Protocol::SetDownTarget (Ipv4L4Protocol::DownTargetCallback callback)
{
  m_downTarget = callback;
}

Ipv4L4Protocol::DownTargetCallback
TcpL4Protocol::GetDownTarget (void) const
{
  return m_downTarget;
}

} // namespace ns3

