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

// Implement the basic Node object for ns3.
// George F. Riley, Georgia Tech, Fall 2006

#include "node.h"

namespace ns3{

Node::Node()
  : m_id(-1), m_sid(0)
{}
  
Node::~Node ()
{}

void 
Node::SetNodeId(Id_t id)
{ 
  m_id = id;
} 

void   
Node::SetSystemId(SystemId_t s )
{
  m_sid = s;
}

L3Demux*
Node::GetL3Demux() const
{
  return 0;
}
Ipv4L4Demux*
Node::GetIpv4L4Demux() const
{
  return 0;
}

NetDeviceList*
Node::GetNetDeviceList() const
{
  return 0;
}

Ipv4 *
Node::GetIpv4 (void) const
{
  return 0;
}
Udp *
Node::GetUdp (void) const
{
  return 0;
}



}//namespace ns3
