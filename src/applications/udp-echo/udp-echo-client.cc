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
#include "ns3/debug.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "udp-echo-client.h"

namespace ns3 {

NS_DEBUG_COMPONENT_DEFINE ("UdpEchoClient");

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
  NS_DEBUG ("UdpEchoClient::UdpEchoClient (" << n << ", " << serverAddress <<
    ", " << serverPort << ", " << count << ", " << interval <<
    ", " << size << ")");

  Construct (n, serverAddress, serverPort, count, interval, size);
}

UdpEchoClient::~UdpEchoClient()
{
  NS_DEBUG ("UdpEchoClient::~UdpEchoClient ()");
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
  NS_DEBUG ("UdpEchoClient::Construct (" << n << ", " << serverAddress <<
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
  NS_DEBUG ("UdpEchoClient::DoDispose ()");
  Application::DoDispose ();
}

void 
UdpEchoClient::StartApplication (void)
{
  NS_DEBUG ("UdpEchoClient::StartApplication ()");

  if (!m_socket)
    {
      InterfaceId iid = InterfaceId::LookupByName ("Udp");
      Ptr<SocketFactory> socketFactory = 
        GetNode ()->QueryInterface<SocketFactory> (iid);
      m_socket = socketFactory->CreateSocket ();
      m_socket->Bind ();
      m_socket->Connect (m_peer);
    }

  StopApplication ();
  ScheduleTransmit (Seconds(0.));
}

void 
UdpEchoClient::StopApplication ()
{
  NS_DEBUG ("UdpEchoClient::StopApplication ()");
}

void 
UdpEchoClient::ScheduleTransmit (Time dt)
{
  NS_DEBUG ("UdpEchoClient::ScheduleTransmit (" << dt << ")");
  m_sendEvent = Simulator::Schedule(dt, &UdpEchoClient::Send, this);
}

void 
UdpEchoClient::Send (void)
{
  NS_DEBUG ("UdpEchoClient::Send ()");

  NS_ASSERT (m_sendEvent.IsExpired ());

  Packet p (m_size);
  m_socket->Send (p);
  ++m_sent;

  NS_DEBUG ("UdpEchoClient::Send (): Sent " << m_size << " bytes to " <<
    m_serverAddress);

  if (m_sent < m_count) 
    {
      ScheduleTransmit (m_interval);
    }
}

} // Namespace ns3
