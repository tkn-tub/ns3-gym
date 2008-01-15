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
// Define a basic "Internet" node, with a protocol stack (l3 and l4),
// network device list, process list, and routing.

#ifndef INTERNET_NODE_H
#define INTERNET_NODE_H

#include <list>
#include <string>

#include "ns3/node.h"

namespace ns3 {

/**
 * \ingroup internetNode
 *
 * \section InternetNode Overview
 *
 * The InternetNode module contains an implementation of TCP, UDP, and
 * IPv4.  ns-3 Applications sit above this module, and ns-3 NetDevices
 * sit below it...
 *
 * InternetNode is implemented as a subclass of Node but this may be
 * refactored in the future to 
 */

/*
 * \brief Container class for various TCP/IP objects and interfaces
 * aggregated to a Node.
 *
 * This class exists primarily to assemble the layer-3/4 stack of a Node 
 * from constituent parts, including implementations of TCP, IPv4, UDP, 
 * and ARP.  It provides only constructors and destructors as its public
 * API.  Internally, the various protocols are instantiated, aggregated
 * to a Node, and plumbed together.
 */
class InternetNode : public Node 
{
public:
  InternetNode();
  InternetNode(uint32_t systemId);
  virtual ~InternetNode ();

protected:
  virtual void DoDispose(void);
  virtual Ptr<TraceResolver> GetTraceResolver (void) const;
private:
  void Construct (void);
};

}//namespace ns3

#endif /* INTERNET_NODE_H */
