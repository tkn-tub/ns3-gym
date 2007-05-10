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
#ifndef NODE_LIST_H
#define NODE_LIST_H

#include <vector>
#include "ns3/array-trace-resolver.h"
#include "ns3/ptr.h"

namespace ns3 {

class Node;
class TraceResolver;
class TraceContext;

class NodeList
{
public:
  typedef ArrayTraceResolver<Node>::Index NodeIndex;
  typedef std::vector< Ptr<Node> >::iterator Iterator;

  static uint32_t Add (Ptr<Node> node);
  static Iterator Begin (void);
  static Iterator End (void);
  static TraceResolver *CreateTraceResolver (TraceContext const &context);

  static Ptr<Node> GetNode (uint32_t n);
};

}//namespace ns3


#endif /* NODE_LIST_H */
