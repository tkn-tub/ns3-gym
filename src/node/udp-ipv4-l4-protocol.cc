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

// NS3 - Layer 4 Protocol base class
// George F. Riley, Georgia Tech, Spring 2007

#include "udp-ipv4-l4-protocol.h"
#include "node.h"
#include "udp.h"

namespace ns3 {

/* see http://www.iana.org/assignments/protocol-numbers */
const uint8_t UdpIpv4L4Protocol::UDP_PROTOCOL = 17;


UdpIpv4L4Protocol::UdpIpv4L4Protocol(Node *node)
  : Ipv4L4Protocol (UDP_PROTOCOL, 2),
    m_node (node)
{}
UdpIpv4L4Protocol::~UdpIpv4L4Protocol ()
{}

UdpIpv4L4Protocol*
UdpIpv4L4Protocol::Copy(Node *node) const
{
  return new UdpIpv4L4Protocol (node);
}
void 
UdpIpv4L4Protocol::Receive(Packet& p, 
                           Ipv4Address const &source,
                           Ipv4Address const &destination)
{
  if (m_node->GetUdp () != 0)
    {
      m_node->GetUdp ()->Receive (p, source, destination);
    }
}

}//namespace ns3
