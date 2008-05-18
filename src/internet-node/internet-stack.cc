// -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*-
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
// Author: George F. Riley<riley@ece.gatech.edu>
//

#include "ns3/net-device.h"
#include "ns3/callback.h"
#include "ns3/node.h"

#include "ipv4-l4-demux.h"
#include "udp-l4-protocol.h"
#include "tcp-l4-protocol.h"
#include "ipv4-l3-protocol.h"
#include "arp-l3-protocol.h"
#include "udp-socket-factory-impl.h"
#include "tcp-impl.h"
#include "ipv4-impl.h"

namespace ns3 {

void 
AddInternetStack (Ptr<Node> node)
{
  Ptr<Ipv4L3Protocol> ipv4 = CreateObject<Ipv4L3Protocol> ();
  Ptr<ArpL3Protocol> arp = CreateObject<ArpL3Protocol> ();
  ipv4->SetNode (node);
  arp->SetNode (node);
  // XXX remove the PeekPointer below.
  node->RegisterProtocolHandler (MakeCallback (&Ipv4L3Protocol::Receive, PeekPointer (ipv4)), 
                                 Ipv4L3Protocol::PROT_NUMBER, 0);
  node->RegisterProtocolHandler (MakeCallback (&ArpL3Protocol::Receive, PeekPointer (arp)),
                                 ArpL3Protocol::PROT_NUMBER, 0);


  Ptr<Ipv4L4Demux> ipv4L4Demux = CreateObject<Ipv4L4Demux> ();
  Ptr<UdpL4Protocol> udp = CreateObject<UdpL4Protocol> ();
  Ptr<TcpL4Protocol> tcp = CreateObject<TcpL4Protocol> ();

  ipv4L4Demux->SetNode (node);
  udp->SetNode (node);
  tcp->SetNode (node);

  ipv4L4Demux->Insert (udp);
  ipv4L4Demux->Insert (tcp);

  Ptr<UdpSocketFactoryImpl> udpFactory = CreateObject<UdpSocketFactoryImpl> ();
  Ptr<TcpImpl> tcpImpl = CreateObject<TcpImpl> ();
  Ptr<Ipv4Impl> ipv4Impl = CreateObject<Ipv4Impl> ();

  udpFactory->SetUdp (udp);
  tcpImpl->SetTcp (tcp);
  ipv4Impl->SetIpv4 (ipv4);

  node->AggregateObject (ipv4);
  node->AggregateObject (arp);
  node->AggregateObject (ipv4Impl);
  node->AggregateObject (udpFactory);
  node->AggregateObject (tcpImpl);
  node->AggregateObject (ipv4L4Demux);
}

}//namespace ns3
