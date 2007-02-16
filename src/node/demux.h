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
// Define the Protocols capability for ns3.
// George F. Riley, Georgia Tech, Fall 2006

// This object manages the different layer 3 protocols for any ns3
// node that has this capability.  

// Note;  changed class name to Demux-- tomh

#ifndef __DEMUX_H__
#define __DEMUX_H__

#include <map>

#include "capability.h"
#include "l3-protocol.h"

namespace ns3 {

// Use a map structure for l3 protocol lookup
typedef std::map<int, L3Protocol*> DemuxMap_t;

class Demux : public Capability
{
public:
  Demux() {}
  Demux(const Demux&);
  virtual ~Demux();
  virtual Demux* Copy() const;

  // Insert a new protocol
  void Insert(L3Protocol*, int proto);
  // Look up a protocol by protocol number
  L3Protocol* Lookup(int);

private:
  DemuxMap_t Protocols;
};

}; // namespace ns3
   
#endif

