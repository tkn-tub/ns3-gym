/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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

// Define a class that wraps a node pointer.  This allows numerous ns3
// objects to have access to a node, but without ambiguity of whether
// the owner should "delete" it.

// George F. Riley, Georgia Tech, Spring 2007

#ifndef __NODE_REFERENCE_H__
#define __NODE_REFERENCE_H__

namespace ns3 {

class Node;

class NodeReference {
public:
  NodeReference(const Node& n)
    : m_node(n) {}
  Node* GetNode() const { return (Node*)&m_node;}
public:
  const Node& m_node;
};

} // namespace ns3

#endif
