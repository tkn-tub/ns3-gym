/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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

#include "ns3/log.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "ns3/mac48-address.h"
#include "ns3/packet.h"
#include "ipv4-loopback-interface.h"
#include "ipv4-l3-protocol.h"

NS_LOG_COMPONENT_DEFINE ("Ipv4LoopbackInterface");

namespace ns3 {

TypeId 
Ipv4LoopbackInterface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ipv4LoopbackInterface")
    .SetParent<Ipv4Interface> ()
    ;
  return tid;
}

Ipv4LoopbackInterface::Ipv4LoopbackInterface ()
  : m_node (0)
{
  NS_LOG_FUNCTION_NOARGS ();
}

Ipv4LoopbackInterface::~Ipv4LoopbackInterface ()
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT (m_node != 0);
}

Ptr<NetDevice> 
Ipv4LoopbackInterface::GetDevice (void) const
{
  return 0;
}

void 
Ipv4LoopbackInterface::SetNode (Ptr<Node> node)
{
  m_node = node;
}

void 
Ipv4LoopbackInterface::SendTo (Ptr<Packet> packet, Ipv4Address dest)
{
  NS_LOG_FUNCTION (this << packet << dest);

  Ptr<Ipv4L3Protocol> ipv4 = 
    m_node->GetObject<Ipv4L3Protocol> ();

  ipv4->Receive (0, packet, Ipv4L3Protocol::PROT_NUMBER, 
                 Mac48Address ("ff:ff:ff:ff:ff:ff"));
}

}//namespace ns3
