/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * All rights reserved.
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
 * Authors: 
 *  Mathieu Lacage <mathieu.lacage@sophia.inria.fr>,
 */
#include "p2p-ipv4-interface.h"

#include "ns3/packet.h"
#include "ns3/net-device.h"
#include "ns3/composite-trace-resolver.h"
#include "ns3/ipv4.h"

namespace ns3 {

PointToPointIpv4Interface::PointToPointIpv4Interface (Node *node, NetDevice *device)
  : Ipv4Interface (device),
    m_node (node)
{}
PointToPointIpv4Interface::~PointToPointIpv4Interface ()
{}

void 
PointToPointIpv4Interface::SendTo (Packet p, Ipv4Address dest)
{
  GetDevice ()->Send (p, GetDevice ()->GetBroadcast (), Ipv4::PROT_NUMBER);
}

TraceResolver *
PointToPointIpv4Interface::DoCreateTraceResolver (TraceContext const &context)
{
  CompositeTraceResolver *resolver = new CompositeTraceResolver (context);
  resolver->Add ("netdevice",
                 MakeCallback (&NetDevice::CreateTraceResolver, GetDevice ()),
                 PointToPointIpv4Interface::NETDEVICE);
  return resolver;

}

}//namespace ns3
