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

#ifndef L3_PROTOCOL_H
#define L3_PROTOCOL_H

namespace ns3 {

class Packet;
class NetDevice;
class Node;
class TraceResolver;
class TraceContext;

/**
 * ::Send is always defined in subclasses.
 */
class L3Protocol {
public:
  L3Protocol(int protocolNumber, int version);
  virtual ~L3Protocol ();
    
  int GetProtocolNumber (void) const;
  int GetVersion() const;

  virtual L3Protocol* Copy(Node *node) const = 0;

  virtual TraceResolver *CreateTraceResolver (TraceContext const &context) = 0;
  /**
   * Lower layer calls this method after calling L3Demux::Lookup
   * The ARP subclass needs to know from which NetDevice this
   * packet is coming to:
   *    - implement a per-NetDevice ARP cache
   *    - send back arp replies on the right device
   */
  virtual void Receive(Packet& p, NetDevice *device) = 0;

 private:
  int m_protocolNumber;
  int m_version;
};

} // Namespace ns3

#endif
