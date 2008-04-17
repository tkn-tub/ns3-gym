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
#include "ns3/udp.h"
#include "packet-sink.h"

using namespace std;

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("PacketSinkApplication");
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
                   TypeIdValue (Udp::GetTypeId ()),
                   MakeTypeIdAccessor (&PacketSink::m_tid),
                   MakeTypeIdChecker ())
    .AddTraceSource ("Rx", "A packet has been received",
                     MakeTraceSourceAccessor (&PacketSink::m_rxTrace))
    ;
  return tid;
}

PacketSink::PacketSink ()
{
  m_socket = 0;
}

PacketSink::~PacketSink()
{}

void
PacketSink::DoDispose (void)
{
  m_socket = 0;

  // chain up
  Application::DoDispose ();
}


// Application Methods
void PacketSink::StartApplication()    // Called at time specified by Start
{
  // Create the socket if not already
  if (!m_socket)
    {
      Ptr<SocketFactory> socketFactory = 
        GetNode ()->GetObject<SocketFactory> (m_tid);
      m_socket = socketFactory->CreateSocket ();
      m_socket->Bind (m_local);
      m_socket->Listen (0);
    }

  m_socket->SetRecvCallback (MakeCallback(&PacketSink::Receive, this));
  m_socket->SetAcceptCallback (
            MakeNullCallback<bool, Ptr<Socket>, const Address &> (),
            MakeNullCallback<void, Ptr<Socket>, const Address&> (),
            MakeCallback(&PacketSink::CloseConnection, this) );
}

void PacketSink::StopApplication()     // Called at time specified by Stop
{
  if (m_socket) 
    {
      m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket>, 
        Ptr<Packet>, const Address &> ());
    }
}

// This LOG output inspired by the application on Joseph Kopena's wiki
void PacketSink::Receive(Ptr<Socket> socket, Ptr<Packet> packet,
                       const Address &from) 
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

void PacketSink::CloseConnection (Ptr<Socket> socket)
{
  socket->Close ();
}

} // Namespace ns3
