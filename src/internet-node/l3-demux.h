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
#include "ns3/interface.h"
#include "ns3/ptr.h"

namespace ns3 {

class L3Protocol;
class Node;
class TraceResolver;
class TraceContext;

/**
 * \brief L3 Demux 
 */
class L3Demux : public NsUnknown
{
public:
  static const Iid iid;
  typedef int ProtocolTraceType;
  L3Demux(Ptr<Node> node);
  virtual ~L3Demux();

  /**
   * \param context the trace context to use to construct the
   *        TraceResolver to return
   * \returns a TraceResolver which can resolve all traces
   *          performed in this object. The caller must
   *          delete the returned object.
   */
  TraceResolver *CreateTraceResolver (TraceContext const &context) const;


  /**
   * \param protocol a template for the protocol to add to this L3 Demux.
   *
   * Invoke Copy on the input template to get a copy of the input
   * protocol which can be used on the Node on which this L3 Demux 
   * is running. The new L3Protocol is registered internally as
   * a working L3 Protocol and returned from this method.
   * The caller does not get ownership of the returned pointer.
   */
  void Insert(Ptr<L3Protocol> protocol);
  /**
   * \param protocolNumber number of protocol to lookup
   *        in this L4 Demux
   * \returns a matching L3 Protocol
   *
   * This method is typically called by lower layers
   * to forward packets up the stack to the right protocol.
   * It is also called from InternetNode::GetIpv4 for example.
   */
  Ptr<L3Protocol> GetProtocol (int protocolNumber);
protected:
  virtual void DoDispose (void);
private:
  typedef std::map<int, Ptr<ns3::L3Protocol> > L3Map_t;

  Ptr<Node> m_node;
  L3Map_t m_protocols;
};

} //namespace ns3
#endif

