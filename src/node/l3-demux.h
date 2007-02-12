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
// Define the L3Protocols capability for ns3.
// George F. Riley, Georgia Tech, Fall 2006

// This object manages the different layer 3 protocols for any ns3
// node that has this capability.  

#ifndef L3_DEMUX_H
#define L3_DEMUX_H

#include <map>

namespace ns3 {

class L3Protocol;
class Node;

class L3Demux
{
public:
  L3Demux(Node *node);
  virtual ~L3Demux();
  L3Demux* Copy(Node *node) const;

  // Insert a new protocol
  ns3::L3Protocol* Insert(const ns3::L3Protocol&);
  // Look up a protocol by protocol number
  ns3::L3Protocol* Lookup(int);
  // Erase an entry
  void        Erase(ns3::L3Protocol*);
private:
  typedef std::map<int, ns3::L3Protocol*> L3Map_t;

  Node *m_node;
  L3Map_t m_protocols;
};

} //namespace ns3
#endif

