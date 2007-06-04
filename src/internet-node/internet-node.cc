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

#include "l3-demux.h"
#include "ipv4-l4-demux.h"
#include "internet-node.h"
#include "udp.h"
#include "ipv4-l3-protocol.h"
#include "arp-l3-protocol.h"
#include "udp-impl.h"
#include "arp-private.h"
#include "ipv4-impl.h"
#include "ipv4-private.h"

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
  Ptr<Udp> udp = Create<Udp> (this);

  Ptr<L3Demux> l3Demux = Create<L3Demux> (this);
  Ptr<Ipv4L4Demux> ipv4L4Demux = Create<Ipv4L4Demux> (this);

  l3Demux->Insert (ipv4);
  l3Demux->Insert (arp);
  ipv4L4Demux->Insert (udp);

  Ptr<IUdpImpl> udpImpl = Create<IUdpImpl> (udp);
  Ptr<IArpPrivate> arpPrivate = Create<IArpPrivate> (arp);
  Ptr<IIpv4Impl> ipv4Impl = Create<IIpv4Impl> (ipv4);
  Ptr<IIpv4Private> ipv4Private = Create<IIpv4Private> (ipv4);

  Object::AddInterface (ipv4Private);
  Object::AddInterface (ipv4Impl);
  Object::AddInterface (arpPrivate);
  Object::AddInterface (udpImpl);
  Object::AddInterface (l3Demux);
  Object::AddInterface (ipv4L4Demux);
}


TraceResolver *
InternetNode::DoCreateTraceResolver (TraceContext const &context)
{
  CompositeTraceResolver *resolver = new CompositeTraceResolver (context);
  Ptr<IIpv4Private> ipv4 = QueryInterface<IIpv4Private> (IIpv4Private::iid);
  resolver->Add ("ipv4",
                 MakeCallback (&IIpv4Private::CreateTraceResolver, PeekPointer (ipv4)),
                 InternetNode::IPV4);

  return resolver;
}

void 
InternetNode::DoDispose()
{
  Node::DoDispose ();
}

void 
InternetNode::DoAddDevice (Ptr<NetDevice> device) const
{
  device->SetReceiveCallback (MakeCallback (&InternetNode::ReceiveFromDevice, this));
}

bool
InternetNode::ReceiveFromDevice (Ptr<NetDevice> device, const Packet &p, uint16_t protocolNumber) const
{
  Ptr<L3Demux> demux = QueryInterface<L3Demux> (L3Demux::iid);
  Ptr<L3Protocol> target = demux->GetProtocol (protocolNumber);
  if (target != 0) 
    {
      Packet packet = p;
      target->Receive(packet, device);
      return true;
    }
  return false;
}


}//namespace ns3
