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
// correctly.

namespace ns3 {

const int nil=0;

class Node;

class Capability 
{
public:
  Capability() : pNode(nil) {}
  virtual ~Capability() {}
  virtual Capability* Copy() const = 0;
  Node*   pNode;  // Node on which this capability is assigned
};

}; // namespace ns3
    
#endif
