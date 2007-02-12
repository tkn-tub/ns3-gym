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

// Define the layer 4 demultiplexer object for ns3.
// George F. Riley, Georgia Tech, Fall 2006

#ifndef IPV4_L4_DEMUX_H
#define IPV4_L4_DEMUX_H

#include <list>


namespace ns3 {

class Ipv4L4Protocol;
class Node;

class Ipv4L4Demux {
public:
  Ipv4L4Demux (Node *node);
  virtual ~Ipv4L4Demux();
  Ipv4L4Demux* Copy(Node *node) const;
  Ipv4L4Protocol* Insert(const Ipv4L4Protocol&);
  Ipv4L4Protocol* Lookup(int protocolNumber);
  void        Erase(Ipv4L4Protocol*);
private:
  typedef std::list<Ipv4L4Protocol*> L4List_t;
  L4List_t m_protocols;
  Node *m_node;
};

} //namespace ns3
#endif
