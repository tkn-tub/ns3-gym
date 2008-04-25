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
#include "ns3/uinteger.h"

#include "udp-echo-server.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("UdpEchoServerApplication");
NS_OBJECT_ENSURE_REGISTERED (UdpEchoServer);

TypeId
UdpEchoServer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UdpEchoServer")
    .SetParent<Application> ()
    .AddConstructor<UdpEchoServer> ()
    .AddAttribute ("Port", "Port on which we listen for incoming packets.",
                   UintegerValue (9),
                   MakeUintegerAccessor (&UdpEchoServer::m_port),
                   MakeUintegerChecker<uint16_t> ())
    ;
  return tid;
}

UdpEchoServer::UdpEchoServer ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

UdpEchoServer::~UdpEchoServer()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
UdpEchoServer::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  Application::DoDispose ();
}

void 
UdpEchoServer::StartApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  if (!m_socket)
    {
      TypeId tid = TypeId::LookupByName ("ns3::Udp");
      Ptr<SocketFactory> socketFactory = 
        GetNode ()->GetObject<SocketFactory> (tid);
      m_socket = socketFactory->CreateSocket ();
      InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), m_port);
      m_socket->Bind (local);
    }

  //m_socket->SetRecvCallback(MakeCallback(&UdpEchoServer::Receive, this));
  m_socket->SetRecv_Callback(MakeCallback(&UdpEchoServer::HandleRead, this));
}

void 
UdpEchoServer::StopApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();

  if (!m_socket) 
    {
      m_socket->SetRecvCallback(MakeNullCallback<void, Ptr<Socket>, 
        Ptr<Packet>, const Address &> ());
    }
}

void 
UdpEchoServer::HandleRead (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  uint32_t maxSize = std::numeric_limits<uint32_t>::max();
  uint32_t flags = 0;  // no flags
  while (packet = socket->Recv (maxSize, flags))
    {
      SocketRxAddressTag tag;
      bool found = packet->PeekTag (tag); 
      NS_ASSERT (found);
      Address from = tag.GetAddress ();
      packet->RemoveTag (tag);
      if (InetSocketAddress::IsMatchingType (from))
        {
          InetSocketAddress address = InetSocketAddress::ConvertFrom (from);
          NS_LOG_INFO ("Received " << packet->GetSize() << " bytes from " << 
            address.GetIpv4());

          NS_LOG_LOGIC ("Echoing packet");
          socket->SendTo (from, packet);
        }
    }
}

void
UdpEchoServer::Receive(
  Ptr<Socket> socket, 
  Ptr<Packet> packet,
  const Address &from) 
{
  NS_LOG_FUNCTION (this << socket << packet << from);

  if (InetSocketAddress::IsMatchingType (from))
    {
      InetSocketAddress address = InetSocketAddress::ConvertFrom (from);
      NS_LOG_INFO ("Received " << packet->GetSize() << " bytes from " << 
        address.GetIpv4());

      NS_LOG_LOGIC ("Echoing packet");
      socket->SendTo (from, packet);
    }
}

} // Namespace ns3
