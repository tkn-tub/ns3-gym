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
#include "node-list.h"
#include "node.h"

namespace ns3 {

uint32_t NodeList::g_nextId = 0;

void
NodeList::Add (Node *node)
{
  GetNodes ()->push_back (node);
  node->SetId(g_nextId++);
}
NodeList::Iterator 
NodeList::Begin (void)
{
  return GetNodes ()->begin ();
}
NodeList::Iterator 
NodeList::End (void)
{
  return GetNodes ()->end ();
}

std::vector<Node *> *
NodeList::GetNodes (void)
{
  static bool firstTime = true;
  if (firstTime)
    {
      TraceRoot::Register ("nodes", MakeCallback (&NodeList::CreateTraceResolver));
      firstTime = false;
    }
  static std::vector<Node *> nodes;
  return &nodes;
}
uint32_t 
NodeList::GetNNodes (void)
{
  return GetNodes ()->size ();
}
Node *
NodeList::GetNode (uint32_t n)
{
  return (*GetNodes ())[n];
}

TraceResolver *
NodeList::CreateTraceResolver (TraceContext const &context)
{
  ArrayTraceResolver<Node> *resolver =
    new ArrayTraceResolver<Node>
    (context, 
     MakeCallback (&NodeList::GetNNodes),
     MakeCallback (&NodeList::GetNode));
  return resolver;
}

}//namespace ns3
