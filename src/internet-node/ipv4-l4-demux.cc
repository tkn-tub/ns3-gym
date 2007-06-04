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

#include <sstream>
#include "ns3/composite-trace-resolver.h"
#include "ns3/i-node.h"
#include "ipv4-l4-demux.h"
#include "ipv4-l4-protocol.h"

namespace ns3 {

const InterfaceId Ipv4L4Demux::iid = MakeInterfaceId ("Ipv4L4Demux", Object::iid);

Ipv4L4Demux::Ipv4L4Demux (Ptr<Node> node)
  : m_node (node)
{
  SetInterfaceId (Ipv4L4Demux::iid);
}

Ipv4L4Demux::~Ipv4L4Demux()
{}

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

TraceResolver *
Ipv4L4Demux::CreateTraceResolver (TraceContext const &context)
{
  CompositeTraceResolver *resolver = new CompositeTraceResolver (context);
  for (L4List_t::const_iterator i = m_protocols.begin(); i != m_protocols.end(); ++i)
    {
      Ptr<Ipv4L4Protocol> protocol = *i;
      std::string protValue;
      std::ostringstream oss (protValue);
      oss << (*i)->GetProtocolNumber ();
      Ipv4L4ProtocolTraceType protocolNumber = (*i)->GetProtocolNumber ();
      resolver->Add (protValue,
                     MakeCallback (&Ipv4L4Protocol::CreateTraceResolver, PeekPointer (protocol)),
                     protocolNumber);
    }
  return resolver;
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
