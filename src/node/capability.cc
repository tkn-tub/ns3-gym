// -*- Mode:NS3 -*-
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

// Implementation of the capability base class.

#include "capability.h"
#include "node.h"
#include "node-reference.h"

namespace ns3 {
  
Capability::Capability()
  : m_node(0)
{ // Nothing else needed
}

Capability::Capability(Node* n)
{
  m_node = new NodeReference(*n);
}

// Copy constructor
Capability::Capability(const Capability& o)
{
  m_node = new NodeReference(*o.GetNode());
}


// Assignment operator
Capability& Capability::operator=(const Capability& rhs)
{
  if (this == &rhs) return *this;  // Self assignment
  delete m_node;
  m_node = new NodeReference(*rhs.GetNode());
  return *this;
}


  
Capability::~Capability()
{
  delete m_node;
}

// SetNode should be overridden by any capability subclass
// that manages other objects needing node information, such
// as the ApplicationList.
void Capability::SetNode(Node& n)
{
  delete m_node;
  m_node = new NodeReference(n);
}


Node* Capability::GetNode() const
{
  if (!m_node) return 0;
  return m_node->GetNode();
}

} // namespace ns3

