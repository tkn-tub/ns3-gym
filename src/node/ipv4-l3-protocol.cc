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
// Author: George F. Riley <riley@ece.gatech.edu>
//

// NS3 - Layer 3 Protocol base class
// George F. Riley, Georgia Tech, Spring 2007

#include "ipv4-l3-protocol.h"
#include "ipv4.h"
#include "node.h"

namespace ns3 {

Ipv4L3Protocol::Ipv4L3Protocol (Node *node)
  : L3Protocol (0x0800, 4),
    m_node (node)
{}
Ipv4L3Protocol::~Ipv4L3Protocol ()
{}

Ipv4L3Protocol *
Ipv4L3Protocol::Copy (Node *node) const
{
  Ipv4L3Protocol *copy = new Ipv4L3Protocol (node);
  return copy;
}
void 
Ipv4L3Protocol::Receive(Packet& p, NetDevice &device)
{
  Ipv4 *ipv4 = m_node->GetIpv4 ();
  if (ipv4 != 0)
    {
      ipv4->Receive (p, device);
    }
}



}//namespace ns3
