// -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*-
//
// Copyright (c) 2006 Georgia Tech Research Corporation
// All rights reserved.
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
// Implementation of the InternetNode class for ns3.
// George F. Riley, Georgia Tech, Fall 2006

#include "ns3/composite-trace-resolver.h"

#include "net-device-list.h"
#include "l3-demux.h"
#include "ipv4-l4-demux.h"
#include "internet-node.h"
#include "udp.h"
#include "ipv4.h"
#include "arp.h"
#include "ipv4-loopback-interface.h"

namespace ns3 {

InternetNode::InternetNode()
{
  // Instantiate the capabilities
  m_netDevices = new NetDeviceList();
  m_l3Demux = new L3Demux(this);
  m_ipv4L4Demux = new Ipv4L4Demux(this);
  m_l3Demux->Insert (Ipv4 (this));
  m_l3Demux->Insert (Arp (this));
  m_ipv4L4Demux->Insert (Udp (this));
  SetupLoopback ();
}

InternetNode::InternetNode (InternetNode const &o)
{
  m_netDevices = new NetDeviceList ();
  m_l3Demux = o.m_l3Demux->Copy (this);
  m_ipv4L4Demux = o.m_ipv4L4Demux->Copy (this);
  SetupLoopback ();  
}
InternetNode const &
InternetNode::operator = (InternetNode const &o)
{
  delete m_netDevices;
  delete m_l3Demux;
  delete m_ipv4L4Demux;
  m_netDevices = new NetDeviceList ();
  m_l3Demux = o.m_l3Demux->Copy (this);
  m_ipv4L4Demux = o.m_ipv4L4Demux->Copy (this);
  SetupLoopback ();  
  return *this;
}

InternetNode::~InternetNode ()
{
  delete m_netDevices;
  delete m_l3Demux;
  delete m_ipv4L4Demux;
}

  void
InternetNode::SetName (std::string name)
{
  m_name = name;
}

void
InternetNode::SetupLoopback (void)
{
  Ipv4LoopbackInterface * interface = new Ipv4LoopbackInterface (this);
  interface->SetAddress (Ipv4Address::GetLoopback ());
  interface->SetNetworkMask (Ipv4Mask::GetLoopback ());
  uint32_t index = GetIpv4 ()->AddInterface (interface);
  GetIpv4 ()->AddHostRouteTo (Ipv4Address::GetLoopback (), index);
  interface->SetUp ();
}

// Copy this node
InternetNode* 
InternetNode::Copy() const
{
  InternetNode *copy = new InternetNode (*this);
   return copy;
}

TraceResolver *
InternetNode::CreateTraceResolver (TraceContext const &context)
{
  CompositeTraceResolver *resolver = new CompositeTraceResolver (context);
  resolver->Add ("ipv4",
                 MakeCallback (&Ipv4::CreateTraceResolver, GetIpv4 ()),
                 InternetNode::IPV4);
  resolver->Add ("arp",
                 MakeCallback (&Arp::CreateTraceResolver, GetArp ()),
                 InternetNode::ARP);
  resolver->Add ("udp",
                 MakeCallback (&Udp::CreateTraceResolver, GetUdp ()),
                 InternetNode::UDP);
  return resolver;
}


NetDeviceList*   
InternetNode::GetNetDeviceList() const
{
  return m_netDevices;
}

L3Demux*     
InternetNode::GetL3Demux() const
{
  return m_l3Demux;
}

Ipv4L4Demux*     
InternetNode::GetIpv4L4Demux() const
{
  return m_ipv4L4Demux;
}

Ipv4 *
InternetNode::GetIpv4 (void) const
{
  return static_cast<Ipv4*> (m_l3Demux->Lookup (Ipv4::PROT_NUMBER));
}
Udp *
InternetNode::GetUdp (void) const
{
  return static_cast<Udp*> (m_ipv4L4Demux->Lookup (Udp::PROT_NUMBER));
}

Arp *
InternetNode::GetArp (void) const
{
  return static_cast<Arp*> (m_l3Demux->Lookup (Arp::PROT_NUMBER));
}


}//namespace ns3
