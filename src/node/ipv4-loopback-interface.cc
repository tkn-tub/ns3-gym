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
#include "ipv4-loopback-interface.h"
#include "net-device.h"
#include "node.h"
#include "ipv4.h"

namespace ns3 {

class Ipv4DummyNetDevice : public NetDevice
{
public:
  Ipv4DummyNetDevice (Node *node);
  Node *PeekNode (void) const;
private:
  virtual bool SendTo (Packet& p, const MacAddress& dest);
};

Ipv4DummyNetDevice::Ipv4DummyNetDevice (Node *node)
  : NetDevice (node, MacAddress ())
{}
Node *
Ipv4DummyNetDevice::PeekNode (void) const
{
  return GetNode ();
}
bool 
Ipv4DummyNetDevice::SendTo (Packet& p, const MacAddress& dest)
{
  return false;
}


Ipv4LoopbackInterface::Ipv4LoopbackInterface (Node *node)
  : Ipv4Interface (new Ipv4DummyNetDevice (node))
{
}
Ipv4LoopbackInterface::~Ipv4LoopbackInterface ()
{
  delete GetDevice ();
}

Node *
Ipv4LoopbackInterface::GetNode (void) const
{
  return static_cast<Ipv4DummyNetDevice *> (GetDevice ())->PeekNode ();
}

void 
Ipv4LoopbackInterface::SendTo (Packet packet, Ipv4Address dest)
{
  m_node->GetIpv4 ()->Receive (packet, *(this->GetDevice ()));
}

}//namespace ns3
