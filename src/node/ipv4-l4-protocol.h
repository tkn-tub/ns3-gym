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

// NS3 - Layer 4 Protocol base class
// George F. Riley, Georgia Tech, Spring 2007

#ifndef IPV4_L4_PROTOCOL_H
#define IPV4_L4_PROTOCOL_H


namespace ns3 {

class Node;
class Packet;
class Ipv4Address;
class TraceResolver;
class TraceContext;

/**
 * \brief L4 Protocol base class 
 *
 * All subclasses must implement:
 *   - Ipv4L4Protocol::Copy
 *   - Ipv4L4Protocol::CreateTraceResolver
 *
 * If you want to implement a new L4 protocol, all you have to do is
 * implement a subclass of this base class and add it to an L4Demux.
 */  
class Ipv4L4Protocol {
public:
  Ipv4L4Protocol(int protocolNumber, int version);
  virtual ~Ipv4L4Protocol ();

  virtual void Dispose (void) = 0;

  /**
   * \returns the protocol number of this protocol.
   */
  int GetProtocolNumber (void) const;
  /**
   * \returns the version number of this protocol.
   */
  int GetVersion() const;

  /**
   * \param node the node on which the copy should be running
   * \returns a new instance of this L4 Protocol.
   *
   * Perform a deep copy of the L4 Protocol
   */
  virtual Ipv4L4Protocol* Copy(Node *node) const = 0;
  virtual TraceResolver *CreateTraceResolver (TraceContext const &context) = 0;

  /**
   * \param p packet to forward up
   * \param source source address of packet received
   * \param destination address of packet received
   * 
   * Called from lower-level layers to send the packet up
   * in the stack. 
   */
  virtual void Receive(Packet& p, 
                       Ipv4Address const &source,
                       Ipv4Address const &destination) = 0;

 private:
  int m_protocolNumber;
  int m_version;
};

} // Namespace ns3

#endif
