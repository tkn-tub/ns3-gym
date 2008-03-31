// -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*-
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

#ifndef IPV4_L4_PROTOCOL_H
#define IPV4_L4_PROTOCOL_H

#include "ns3/object.h"
#include "ipv4-interface.h"

namespace ns3 {

class Packet;
class Ipv4Address;
class TraceResolver;
class TraceContext;

/**
 * \brief L4 Protocol base class 
 *
 * If you want to implement a new L4 protocol, all you have to do is
 * implement a subclass of this base class and add it to an L4Demux.
 */  
class Ipv4L4Protocol : public Object
{
public:
  static TypeId GetTypeId (void);

  virtual ~Ipv4L4Protocol ();

  /**
   * \returns the protocol number of this protocol.
   */
  virtual int GetProtocolNumber (void) const = 0;
  /**
   * \returns the version number of this protocol.
   */
  virtual int GetVersion (void) const = 0;

  /**
   * \param p packet to forward up
   * \param source source address of packet received
   * \param destination address of packet received
   * \param incomingInterface the Ipv4Interface on which the packet arrived
   * 
   * Called from lower-level layers to send the packet up
   * in the stack. 
   */
  virtual void Receive(Ptr<Packet> p, 
                       Ipv4Address const &source,
                       Ipv4Address const &destination,
                       Ptr<Ipv4Interface> incomingInterface) = 0;
};

} // Namespace ns3

#endif
