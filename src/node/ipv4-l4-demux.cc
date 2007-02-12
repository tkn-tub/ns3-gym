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

// Define the layer 4 demultiplexer object for ns3.
// George F. Riley, Georgia Tech, Fall 2006

#include "ipv4-l4-demux.h"
#include "ipv4-l4-protocol.h"

namespace ns3 {

Ipv4L4Demux::Ipv4L4Demux (Node *node)
  : m_node (node)
{}

Ipv4L4Demux::~Ipv4L4Demux()
{}
Ipv4L4Demux* 
Ipv4L4Demux::Copy(Node *node) const
{
  Ipv4L4Demux * copy = new Ipv4L4Demux(node);
  for (L4List_t::const_iterator i = m_protocols.begin(); i != m_protocols.end(); ++i)
    {
      copy->Insert(*(*i));
    }
  return copy;
}
Ipv4L4Protocol* 
Ipv4L4Demux::Insert(const Ipv4L4Protocol&protocol)
{
  Ipv4L4Protocol* copy = protocol.Copy(m_node); // Make a copy of the protocol
  m_protocols.push_back (copy);
  return copy;
}
Ipv4L4Protocol* 
Ipv4L4Demux::Lookup(int protocolNumber)
{
  for (L4List_t::iterator i = m_protocols.begin(); i != m_protocols.end(); ++i)
    {
      if ((*i)->GetProtocolNumber () == protocolNumber)
	{
	  return *i;
	}
    }
  return 0;
}
void
Ipv4L4Demux::Erase(Ipv4L4Protocol*protocol)
{
  m_protocols.remove (protocol);
}



}//namespace ns3
