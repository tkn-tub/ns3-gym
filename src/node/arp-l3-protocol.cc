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

// NS3 - Layer 3 Protocol base class
// George F. Riley, Georgia Tech, Spring 2007

#include "arp-l3-protocol.h"
#include "arp.h"
#include "node.h"

namespace ns3 {

ArpL3Protocol::ArpL3Protocol (Node *node)
  : L3Protocol (0x0806, 0/* XXX: correct version number ? */ ),
    m_node (node)
{}
ArpL3Protocol::~ArpL3Protocol ()
{}

ArpL3Protocol *
ArpL3Protocol::Copy (Node *node) const
{
  return new ArpL3Protocol (node);
}
void 
ArpL3Protocol::Receive(Packet& p, NetDevice &device)
{
  Arp * arp = m_node->GetArp ();
  if (arp != 0)
    {
      arp->Receive (p, &device);
    }
}

}//namespace ns3
