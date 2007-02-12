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

// NS3 - Layer 3 Protocol base class
// George F. Riley, Georgia Tech, Spring 2007
#ifndef ARP_L3_PROTOCOL_H
#define ARP_L3_PROTOCOL_H

#include "l3-protocol.h"

namespace ns3 {

class Node;
class NetDevice;
class Packet;

class ArpL3Protocol : public L3Protocol
{
 public:
  ArpL3Protocol (Node *node);
  virtual ~ArpL3Protocol ();

  virtual ArpL3Protocol *Copy (Node *node) const;
  virtual void Receive(Packet& p, NetDevice &device);
private:
  Node *m_node;
};

}//namespace ns3

#endif /* ARP_L3_PROTOCOL_H */
