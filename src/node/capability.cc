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

namespace ns3 {

Capability::Capability(Node* n)
  : m_node (n)
{
  m_node->Ref ();
}

Capability::~Capability ()
{
  if (m_node != 0)
    {
      m_node->Unref ();
    }
}

// Copy constructor
Capability::Capability(const Capability& o)
  : m_node (o.m_node)
{
  if (m_node != 0)
    {
      m_node->Ref ();
    }
}

// Assignment operator
Capability& Capability::operator=(const Capability& rhs)
{
  if (this == &rhs) return *this;  // Self assignment
  if (m_node != 0)
    {
      m_node->Unref ();
    }
  m_node = rhs.m_node;
  if (m_node != 0)
    {
      m_node->Ref ();
    }
  return *this;
}

// SetNode should be overridden by any capability subclass
// that manages other objects needing node information, such
// as the ApplicationList.
void Capability::SetNode(Node *node)
{
  if (m_node != 0)
    {
      m_node->Unref ();
    }
  m_node = node;
  if (m_node != 0)
    {
      m_node->Ref ();
    }
}


Node* Capability::GetNode() const
{
  if (!m_node) return 0;
  m_node->Ref ();
  return m_node;
}

} // namespace ns3

