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
// Define the base class for all node capabilities.
// George F. Riley, Georgia Tech, Fall 2006

#ifndef __CAPABILITY_H__
#define __CAPABILITY_H__

// All capabilities must implement a copy method, to allow node subclasses
// to have a pointer to any subclass of the capability and still copy
// correctly.  Capabilities also have a pointer to the owning node.
// The node pointer is not owned by the capability, and is not deleted
// by the capability destructor.

#define nil 0

namespace ns3 {
class Node;
class NodeReference;
  
class Capability 
{
public:
  Capability();
  Capability(Node* n);
  Capability(const Capability&); // Copy constructor
  virtual Capability& operator=(const Capability&); // Assignment operator
  virtual ~Capability();
  virtual Capability* Copy(Node&) const = 0;
  virtual void SetNode(Node&);
  virtual Node* GetNode() const;
private:
  NodeReference*   m_node;  // Node on which this capability is assigned
};

}  //namespace ns3
#endif
