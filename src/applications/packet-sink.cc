/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2006 Georgia Tech Research Corporation
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
#include "ns3/address.h"
#include "ns3/debug.h"
#include "ns3/inet-socket-address.h"
#include "ns3/node.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "packet-sink.h"

using namespace std;

namespace ns3 {

// Constructors

PacketSink::PacketSink (Ptr<Node> n, 
                        const Address &local,
                        std::string iid,
                        bool quiet)
  :  Application(n)
{
  Construct (n, local, iid, quiet);
}

void
PacketSink::Construct (Ptr<Node> n, 
                       const Address &local,
                       std::string iid,
                       bool quiet)
{
  m_socket = 0;
  m_local = local;
  m_iid = iid;
  m_quiet = quiet;
}

PacketSink::~PacketSink()
{}

void
PacketSink::SetQuiet()
{
  m_quiet = true;
}

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
      InterfaceId iid = InterfaceId::LookupByName (m_iid);
      Ptr<SocketFactory> socketFactory = 
        GetNode ()->QueryInterface<SocketFactory> (iid);
      m_socket = socketFactory->CreateSocket ();
      m_socket->Bind (m_local);
    }
  m_socket->SetRecvCallback((Callback<void, Ptr<Socket>, const Packet &,
    const Address &>) MakeCallback(&PacketSink::Receive, this));
}

void PacketSink::StopApplication()     // Called at time specified by Stop
{
  if (!m_socket) 
    {
      m_socket->SetRecvCallback((Callback<void, Ptr<Socket>, const Packet &,
                               const Address &>) NULL);
 
    }
}

// This callback body suggested by Joe Kopena's wiki
void PacketSink::Receive(Ptr<Socket> socket, const Packet &packet,
                       const Address &from) 
{
  if (!m_quiet)
    {
      if (InetSocketAddress::IsMatchingType (from))
        {
          InetSocketAddress address = InetSocketAddress::ConvertFrom (from);
          NS_DEBUG_UNCOND ( __PRETTY_FUNCTION__ << ": Received " << 
            packet.GetSize() << " bytes from " << address.GetIpv4() << " [" 
            << address << "]---'" << packet.PeekData() << "'");
        }
    }
}

} // Namespace ns3
