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
#include "ns3/net-device.h"
#include "ns3/callback.h"

#include "ipv4-l4-demux.h"
#include "internet-node.h"
#include "udp-l4-protocol.h"
#include "ipv4-l3-protocol.h"
#include "arp-l3-protocol.h"
#include "udp-impl.h"
#include "ipv4-impl.h"

namespace ns3 {


InternetNode::InternetNode()
{
  Construct ();
}

InternetNode::InternetNode(uint32_t systemId)
{
  Construct ();
}

InternetNode::~InternetNode ()
{}

void
InternetNode::Construct (void)
{
  Ptr<Ipv4L3Protocol> ipv4 = Create<Ipv4L3Protocol> (this);
  Ptr<ArpL3Protocol> arp = Create<ArpL3Protocol> (this);
  // XXX remove the PeekPointer below.
  RegisterProtocolHandler (MakeCallback (&Ipv4L3Protocol::Receive, PeekPointer (ipv4)), 
                           Ipv4L3Protocol::PROT_NUMBER, 0);
  RegisterProtocolHandler (MakeCallback (&ArpL3Protocol::Receive, PeekPointer (arp)),
                           ArpL3Protocol::PROT_NUMBER, 0);


  Ptr<Ipv4L4Demux> ipv4L4Demux = Create<Ipv4L4Demux> (this);
  Ptr<UdpL4Protocol> udp = Create<UdpL4Protocol> (this);
  ipv4L4Demux->Insert (udp);

  Ptr<UdpImpl> udpImpl = Create<UdpImpl> (udp);
  Ptr<Ipv4Impl> ipv4Impl = Create<Ipv4Impl> (ipv4);

  Object::AddInterface (ipv4);
  Object::AddInterface (arp);
  Object::AddInterface (ipv4Impl);
  Object::AddInterface (udpImpl);
  Object::AddInterface (ipv4L4Demux);
}


void
InternetNode::DoFillTraceResolver (CompositeTraceResolver &resolver)
{
  Node::DoFillTraceResolver (resolver);
  Ptr<Ipv4L3Protocol> ipv4 = QueryInterface<Ipv4L3Protocol> (Ipv4L3Protocol::iid);
  resolver.Add ("ipv4",
                MakeCallback (&Ipv4L3Protocol::CreateTraceResolver, PeekPointer (ipv4)),
                InternetNode::IPV4);
}

void 
InternetNode::DoDispose()
{
  Node::DoDispose ();
}

}//namespace ns3
