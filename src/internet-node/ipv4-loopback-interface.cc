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
#include "ns3/empty-trace-resolver.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "ipv4-loopback-interface.h"
#include "ipv4-l3-protocol.h"

namespace ns3 {

Ipv4LoopbackInterface::Ipv4LoopbackInterface (Ptr<Node> node)
  : Ipv4Interface (0),
    m_node (node)
{}
Ipv4LoopbackInterface::~Ipv4LoopbackInterface ()
{}

TraceResolver *
Ipv4LoopbackInterface::DoCreateTraceResolver (TraceContext const &context)
{
  return new EmptyTraceResolver (context);
}

void 
Ipv4LoopbackInterface::SendTo (Packet packet, Ipv4Address dest)
{
  Ptr<Ipv4L3Protocol> ipv4 = m_node->QueryInterface<Ipv4L3Protocol> (Ipv4L3Protocol::iid);
  ipv4->Receive (GetDevice (), packet, Ipv4L3Protocol::PROT_NUMBER, GetDevice ()->GetAddress ());
}

}//namespace ns3
