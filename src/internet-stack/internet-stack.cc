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
#include "ns3/core-config.h"

#include "udp-l4-protocol.h"
#include "tcp-l4-protocol.h"
#include "ipv4-l3-protocol.h"
#include "arp-l3-protocol.h"
#include "udp-socket-factory-impl.h"
#include "tcp-socket-factory-impl.h"
#include "ipv4-impl.h"
#include "ipv4-raw-socket-factory-impl.h"
#include "icmpv4-l4-protocol.h"
#ifdef NETWORK_SIMULATION_CRADLE
#include "nsc-tcp-socket-factory-impl.h"
#include "nsc-tcp-l4-protocol.h"
#endif

namespace ns3 {

static void
AddArpStack (Ptr<Node> node)
{
  Ptr<ArpL3Protocol> arp = CreateObject<ArpL3Protocol> ();
  arp->SetNode (node);
  node->AggregateObject (arp);
}

static void
AddUdpStack(Ptr<Node> node)
{
  Ptr<Ipv4L3Protocol> ipv4 = node->GetObject<Ipv4L3Protocol> ();
  Ptr<UdpL4Protocol> udp = CreateObject<UdpL4Protocol> ();
  udp->SetNode (node);
  ipv4->Insert (udp);
  Ptr<UdpSocketFactoryImpl> udpFactory = CreateObject<UdpSocketFactoryImpl> ();
  udpFactory->SetUdp (udp);
  node->AggregateObject (udpFactory);
}

static void
AddIcmpStack (Ptr<Node> node)
{
  Ptr<Ipv4L3Protocol> ipv4 = node->GetObject<Ipv4L3Protocol> ();
  Ptr<Icmpv4L4Protocol> icmp = CreateObject<Icmpv4L4Protocol> ();
  icmp->SetNode (node);
  ipv4->Insert (icmp);
  Ptr<Ipv4RawSocketFactoryImpl> rawFactory = CreateObject<Ipv4RawSocketFactoryImpl> ();
  node->AggregateObject (rawFactory);
}

static void
AddTcpStack(Ptr<Node> node)
{
  Ptr<Ipv4L3Protocol> ipv4 = node->GetObject<Ipv4L3Protocol> ();
  Ptr<TcpL4Protocol> tcp = CreateObject<TcpL4Protocol> ();
  tcp->SetNode (node);

  ipv4->Insert (tcp);

  Ptr<TcpSocketFactoryImpl> tcpFactory = CreateObject<TcpSocketFactoryImpl> ();
  tcpFactory->SetTcp (tcp);
  node->AggregateObject (tcpFactory);
}

static void
AddIpv4Stack(Ptr<Node> node)
{
  Ptr<Ipv4L3Protocol> ipv4 = CreateObject<Ipv4L3Protocol> ();
  ipv4->SetNode (node);
  node->AggregateObject (ipv4);
  Ptr<Ipv4Impl> ipv4Impl = CreateObject<Ipv4Impl> ();
  ipv4Impl->SetIpv4 (ipv4);
  node->AggregateObject (ipv4Impl);
}

void
AddInternetStack (Ptr<Node> node)
{
  AddArpStack (node);
  AddIpv4Stack (node);
  AddIcmpStack (node);
  AddUdpStack (node);
  AddTcpStack (node);
}

#ifdef NETWORK_SIMULATION_CRADLE
static void
AddNscStack(Ptr<Node> node, const std::string &soname)
{
  Ptr<Ipv4L3Protocol> ipv4 = node->GetObject<Ipv4L3Protocol> ();
  Ptr<NscTcpL4Protocol> tcp = CreateObject<NscTcpL4Protocol> ();
  tcp->SetNscLibrary(soname);
  tcp->SetNode (node);
  ipv4->Insert (tcp);
  Ptr<NscTcpSocketFactoryImpl> tcpFactory = CreateObject<NscTcpSocketFactoryImpl> ();
  tcpFactory->SetTcp (tcp);
  node->AggregateObject (tcpFactory);
}


void
AddNscInternetStack (Ptr<Node> node, const std::string &soname)
{
  AddArpStack (node);
  AddIpv4Stack (node);
  AddIcmpStack (node);
  AddUdpStack (node);
  AddNscStack (node, soname);
}
#else
void
AddNscInternetStack (Ptr<Node> node, const std::string &soname)
{
  NS_FATAL_ERROR ("NSC Not enabled on this platform.");
}
#endif
}//namespace ns3
