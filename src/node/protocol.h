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
// NS3 - Protocol base class
// George F. Riley.  Georgia Tech, Spring 2007

// Class protocol is for any protocol stack layer implementation

#include <stdint.h>

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

namespace ns3{

typedef int   Layer_t;        // Protocol layer number
typedef int   Proto_t;        // Protocol identifier
typedef int   Version_t;      // Which version of protocol
typedef int   Priority_t;     // Protocol priority
typedef int32_t  PortId_t;
#define NO_PORT ((PortId_t)-1)
typedef uint32_t Size_t;

// All Protocol subclasses must implement the Layer, Proto, and
// Version methods, although they are free to indicate that the
// particular value if meaningless by returning -1.  For example,
// most layer 2 protocols do not include a "version number", nor
// a "protocol number".  The Proto values must be used
// if the protocol is to be inserted into the protocol graph.
//
// Additionally, all protocols must be able to copy themselves, using
// the virtual Copy method.  THis is used when copying protocol graphs
// when creating Node objects from a pre-configured node.  See the
// code in Node::Create() and Node::Prototype() in the node implementation.
class Protocol {
public:
  Protocol() { }
  virtual ~Protocol() { }
  virtual Layer_t   GetLayer() const = 0;          // Get protocol layer
  virtual Proto_t   GetProtocolNumber() const = 0; // Get proto number
  virtual Version_t GetVersion() const = 0;        // Get proto version number
  virtual Protocol* Copy() const = 0;              // Copy this protocol
};

} //namespace ns3
#endif

