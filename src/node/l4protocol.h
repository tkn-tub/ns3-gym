/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

// NS3 - Layer 4 Protocol base class
// George F. Riley, Georgia Tech, Spring 2007

#ifndef __l4proto_h__
#define __l4proto_h__

#include "protocol.h"

namespace ns3 {

class Node;
  
class L4Protocol : public Protocol {
public:
  L4Protocol() : m_node(0) {}
  virtual Layer_t Layer () const { return 4; }
  virtual L4Protocol* Copy() const = 0;
  // L4 protocols have an associated Node object
public:
  Node* m_node;
};

} // Namespace ns3

#endif
