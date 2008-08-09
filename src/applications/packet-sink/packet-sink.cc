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
 *
 * Author:  Tom Henderson (tomhend@u.washington.edu)
 */
#include "ns3/address.h"
#include "ns3/log.h"
#include "ns3/inet-socket-address.h"
#include "ns3/node.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/udp-socket-factory.h"
#include "packet-sink.h"

using namespace std;

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("PacketSink");
NS_OBJECT_ENSURE_REGISTERED (PacketSink);

TypeId 
PacketSink::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PacketSink")
    .SetParent<Application> ()
    .AddConstructor<PacketSink> ()
    .AddAttribute ("Local", "The Address on which to Bind the rx socket.",
                   AddressValue (),
                   MakeAddressAccessor (&PacketSink::m_local),
                   MakeAddressChecker ())
    .AddAttribute ("Protocol", "The type id of the protocol to use for the rx socket.",
                   TypeIdValue (UdpSocketFactory::GetTypeId ()),
                   MakeTypeIdAccessor (&PacketSink::m_tid),
                   MakeTypeIdChecker ())
    .AddTraceSource ("Rx", "A packet has been received",
                     MakeTraceSourceAccessor (&PacketSink::m_rxTrace))
    ;
  return tid;
}

PacketSink::PacketSink ()
{
  NS_LOG_FUNCTION (this);
  m_socket = 0;
}

PacketSink::~PacketSink()
{
  NS_LOG_FUNCTION (this);
}

void
PacketSink::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_socket = 0;

  // chain up
  Application::DoDispose ();
}


// Application Methods
void PacketSink::StartApplication()    // Called at time specified by Start
{
  NS_LOG_FUNCTION (this);
  // Create the socket if not already
  if (!m_socket)
    {
      m_socket = Socket::CreateSocket (GetNode(), m_tid);
      m_socket->Bind (m_local);
      m_socket->Listen (0);
    }

  m_socket->SetRecvCallback (MakeCallback(&PacketSink::HandleRead, this));
  m_socket->SetAcceptCallback (
            MakeNullCallback<bool, Ptr<Socket>, const Address &> (),
            MakeCallback(&PacketSink::HandleAccept, this));
}

void PacketSink::StopApplication()     // Called at time specified by Stop
{
  NS_LOG_FUNCTION (this);
  while(!m_socketList.empty()) //these are accepted sockets, close them
  {
    Ptr<Socket> acceptedSocket = m_socketList.front();
    m_socketList.pop_front();
    acceptedSocket->Close();
  }
  if (m_socket) 
    {
      m_socket->Close ();
      m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }
}

void PacketSink::HandleRead (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);
  Ptr<Packet> packet;
  Address from;
  while (packet = socket->RecvFrom (from))
    {
      if (InetSocketAddress::IsMatchingType (from))
        {
          InetSocketAddress address = InetSocketAddress::ConvertFrom (from);
          NS_LOG_INFO ("Received " << packet->GetSize() << " bytes from " << 
            address.GetIpv4() << " [" << address << "]---'" << 
            packet->PeekData() << "'");
        }    
      m_rxTrace (packet, from);
    }
}

void PacketSink::HandleAccept (Ptr<Socket> s, const Address& from)
{
  NS_LOG_FUNCTION (this << s << from);
  s->SetRecvCallback (MakeCallback(&PacketSink::HandleRead, this));
  m_socketList.push_back (s);
}

} // Namespace ns3
