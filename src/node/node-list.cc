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

#include "ns3/array-trace-resolver.h"
#include "ns3/trace-root.h"
#include "ns3/simulator.h"
#include "ns3/simulation-singleton.h"
#include "node-list.h"
#include "node.h"

namespace {
static class Initialization 
{
public:
  Initialization ()
  {
    ns3::TraceRoot::Register ("nodes", ns3::MakeCallback (&ns3::NodeList::CreateTraceResolver));
  }
} g_initialization;
}

namespace ns3 {

/**
 * The private node list used by the static-based API
 */
class NodeListPriv
{
public:
  NodeListPriv ();
  ~NodeListPriv ();

  uint32_t Add (Ptr<Node> node);
  NodeList::Iterator Begin (void);
  NodeList::Iterator End (void);
  TraceResolver *CreateTraceResolver (TraceContext const &context);
  Ptr<Node> GetNode (uint32_t n);
  uint32_t GetNNodes (void);

private:
  std::vector<Ptr<Node> > m_nodes;
};

NodeListPriv::NodeListPriv ()
{}
NodeListPriv::~NodeListPriv ()
{
  for (std::vector<Ptr<Node> >::iterator i = m_nodes.begin ();
       i != m_nodes.end (); i++)
    {
      Ptr<Node> node = *i;
      node->Dispose ();
      *i = 0;
    }
  m_nodes.erase (m_nodes.begin (), m_nodes.end ());
}


uint32_t
NodeListPriv::Add (Ptr<Node> node)
{
  uint32_t index = m_nodes.size ();
  m_nodes.push_back (node);
  return index;
  
}
NodeList::Iterator 
NodeListPriv::Begin (void)
{
  return m_nodes.begin ();
}
NodeList::Iterator 
NodeListPriv::End (void)
{
  return m_nodes.end ();
}
uint32_t 
NodeListPriv::GetNNodes (void)
{
  return m_nodes.size ();
}

Ptr<Node>
NodeListPriv::GetNode (uint32_t n)
{
  return m_nodes[n];
}


TraceResolver *
NodeListPriv::CreateTraceResolver (TraceContext const &context)
{
  ArrayTraceResolver<Ptr<Node> > *resolver =
    new ArrayTraceResolver<Ptr<Node> >
    (context, 
     MakeCallback (&NodeListPriv::GetNNodes, this),
     MakeCallback (&NodeListPriv::GetNode, this));
  return resolver;
}

}

/**
 * The implementation of the public static-based API
 * which calls into the private implementation through
 * the simulation singleton.
 */
namespace ns3 {

uint32_t
NodeList::Add (Ptr<Node> node)
{
  return SimulationSingleton<NodeListPriv>::Get ()->Add (node);
}
NodeList::Iterator 
NodeList::Begin (void)
{
  return SimulationSingleton<NodeListPriv>::Get ()->Begin ();
}
NodeList::Iterator 
NodeList::End (void)
{
  return SimulationSingleton<NodeListPriv>::Get ()->End ();
}
TraceResolver *
NodeList::CreateTraceResolver (TraceContext const &context)
{
  return SimulationSingleton<NodeListPriv>::Get ()->CreateTraceResolver (context);
}
Ptr<Node>
NodeList::GetNode (uint32_t n)
{
  return SimulationSingleton<NodeListPriv>::Get ()->GetNode (n);
}



}//namespace ns3
