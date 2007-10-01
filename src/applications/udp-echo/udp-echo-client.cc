/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2007 University of Washington
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
 */
#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "udp-echo-client.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("UdpEchoClientApplication");

UdpEchoClient::UdpEchoClient (
  Ptr<Node> n,
  Ipv4Address serverAddress,
  uint16_t serverPort,
  uint32_t count,
  Time interval,
  uint32_t size)
: 
  Application(n)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAM ("(" << n << ", " << serverAddress <<
    ", " << serverPort << ", " << count << ", " << interval <<
    ", " << size << ")");

  Construct (n, serverAddress, serverPort, count, interval, size);
}

UdpEchoClient::~UdpEchoClient()
{
  NS_LOG_FUNCTION;
}

void
UdpEchoClient::Construct (
  Ptr<Node> n,
  Ipv4Address serverAddress,
  uint16_t serverPort,
  uint32_t count,
  Time interval,
  uint32_t size)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAM ("(" << n << ", " << serverAddress <<
    ", " << serverPort << ", " << count << ", " << interval <<
    ", " << size << ")");

  m_node = n;
  m_serverAddress = serverAddress;
  m_serverPort = serverPort;
  m_count = count;
  m_interval = interval;
  m_size = size;

  m_sent = 0;
  m_socket = 0;
  m_peer = InetSocketAddress (serverAddress, serverPort);
  m_sendEvent = EventId ();
}

void
UdpEchoClient::DoDispose (void)
{
  NS_LOG_FUNCTION;
  Application::DoDispose ();
}

void 
UdpEchoClient::StartApplication (void)
{
  NS_LOG_FUNCTION;

  if (!m_socket)
    {
      InterfaceId iid = InterfaceId::LookupByName ("Udp");
      Ptr<SocketFactory> socketFactory = 
        GetNode ()->QueryInterface<SocketFactory> (iid);
      m_socket = socketFactory->CreateSocket ();
      m_socket->Bind ();
      m_socket->Connect (m_peer);
    }

  m_socket->SetRecvCallback(MakeCallback(&UdpEchoClient::Receive, this));

  ScheduleTransmit (Seconds(0.));
}

void 
UdpEchoClient::StopApplication ()
{
  NS_LOG_FUNCTION;

  if (!m_socket) 
    {
      m_socket->SetRecvCallback(MakeNullCallback<void, Ptr<Socket>, Ptr<Packet>,
                                const Address &> ());
    }

  Simulator::Cancel(m_sendEvent);
}

void 
UdpEchoClient::ScheduleTransmit (Time dt)
{
  NS_LOG_FUNCTION;
  m_sendEvent = Simulator::Schedule(dt, &UdpEchoClient::Send, this);
}

void 
UdpEchoClient::Send (void)
{
  NS_LOG_FUNCTION;

  NS_ASSERT (m_sendEvent.IsExpired ());

  Ptr<Packet> p = Create<Packet> (m_size);
  m_socket->Send (p);
  ++m_sent;

  NS_LOG_INFO ("Sent " << m_size << " bytes to " << m_serverAddress);

  if (m_sent < m_count) 
    {
      ScheduleTransmit (m_interval);
    }
}

void
UdpEchoClient::Receive(
  Ptr<Socket> socket, 
  Ptr<Packet> packet,
  const Address &from) 
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAM ("(" << socket << ", " << packet << ", " << from << ")");

  if (InetSocketAddress::IsMatchingType (from))
    {
      InetSocketAddress address = InetSocketAddress::ConvertFrom (from);
      NS_LOG_INFO ("Received " << packet->GetSize() << " bytes from " << 
        address.GetIpv4());
    }
}


} // Namespace ns3
