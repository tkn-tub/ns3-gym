// -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*-
//
// Copyright (c) 2006 Georgia Tech Research Corporation
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

#include <sstream>
#include "ns3/node.h"
#include "ns3/object-vector.h"
#include "ipv4-l4-demux.h"
#include "ipv4-l4-protocol.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (Ipv4L4Demux);

TypeId 
Ipv4L4Demux::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ipv4L4Demux")
    .SetParent<Object> ()
    .AddAttribute ("Protocols", "The set of protocols registered with this demux.",
                   ObjectVector (),
                   MakeObjectVectorAccessor (&Ipv4L4Demux::m_protocols),
                   MakeObjectVectorChecker<Ipv4L4Protocol> ())
    ;
  return tid;
}

Ipv4L4Demux::Ipv4L4Demux ()
{}

Ipv4L4Demux::~Ipv4L4Demux()
{}

void 
Ipv4L4Demux::SetNode (Ptr<Node> node)
{
  m_node = node;
}

void
Ipv4L4Demux::DoDispose (void)
{
  for (L4List_t::iterator i = m_protocols.begin(); i != m_protocols.end(); ++i)
    {
      (*i)->Dispose ();
      *i = 0;
    }
  m_protocols.clear ();
  m_node = 0;
  Object::DoDispose ();
}

void
Ipv4L4Demux::Insert(Ptr<Ipv4L4Protocol> protocol)
{
  m_protocols.push_back (protocol);
}
Ptr<Ipv4L4Protocol>
Ipv4L4Demux::GetProtocol(int protocolNumber)
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
Ipv4L4Demux::Remove (Ptr<Ipv4L4Protocol> protocol)
{
  m_protocols.remove (protocol);
}



}//namespace ns3
