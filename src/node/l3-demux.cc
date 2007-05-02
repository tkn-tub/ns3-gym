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
// Implement the L3Protocols capability for ns3.
// George F. Riley, Georgia Tech, Fall 2006
#include <sstream>
#include <string>
#include "ns3/composite-trace-resolver.h"
#include "l3-demux.h"
#include "l3-protocol.h"
#include "node.h"

namespace ns3 {

L3Demux::L3Demux (Node *node)
  : m_node (node)
{
  m_node->Ref ();
}

L3Demux::~L3Demux()
{
  Dispose ();
}

void
L3Demux::Dispose (void)
{
  for (L3Map_t::iterator i = m_protocols.begin(); i != m_protocols.end(); ++i)
    {
      i->second->Dispose ();
      delete i->second;
    }
  m_protocols.clear ();
  if (m_node != 0)
    {
      m_node->Unref ();
      m_node = 0;
    }
}

TraceResolver *
L3Demux::CreateTraceResolver (TraceContext const &context) const
{
  CompositeTraceResolver *resolver = new CompositeTraceResolver (context);
  for (L3Map_t::const_iterator i = m_protocols.begin(); i != m_protocols.end(); ++i)
    {
      std::string protValue;
      std::ostringstream oss (protValue);
      oss << i->second->GetProtocolNumber ();
      ProtocolTraceType context = i->second->GetProtocolNumber ();
      resolver->Add (protValue, 
                     MakeCallback (&L3Protocol::CreateTraceResolver, i->second),
                     context);
    }
  return resolver;
}
  
L3Demux* L3Demux::Copy(Node *node) const
{
  L3Demux *copy = new L3Demux (node);
  for (L3Map_t::const_iterator i = m_protocols.begin(); i != m_protocols.end(); ++i)
    {
      copy->Insert(*i->second);
    }

  return copy;
}
  
L3Protocol* L3Demux::Insert(const L3Protocol& l3p)
{
  L3Protocol* l = l3p.Copy(m_node); // Make a copy of the protocol
  m_protocols.insert(L3Map_t::value_type(l3p.GetProtocolNumber (), l));
  return l;
}

L3Protocol* L3Demux::Lookup(int p)
{ // Look up a protocol by protocol number
  L3Map_t::iterator i = m_protocols.find(p);
  if (i == m_protocols.end()) return 0;  // Not found
  return i->second; // Return the protocol
}

} //namespace ns3  
  
