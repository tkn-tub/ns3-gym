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

// Define the basic Node object for ns3.
// George F. Riley, Georgia Tech, Fall 2006

#ifndef __NODE_H__
#define __NODE_H__

#include <vector>

#include "ns3/interface.h"

namespace ns3 {

class TraceContext;
class TraceResolver;
class NetDevice;

class Node : public Interface
{
public:
  static const InterfaceId iid;

  Node();
  Node(uint32_t); // Specify which system for a distributed simulation
  virtual ~Node();

  virtual TraceResolver *CreateTraceResolver (TraceContext const &context) = 0;

  uint32_t GetId (void) const;
  uint32_t GetSystemId (void) const;
  void SetSystemId(uint32_t s);

  uint32_t AddDevice (Ptr<NetDevice> device);
  Ptr<NetDevice> GetDevice (uint32_t index) const;
  uint32_t GetNDevices (void) const;

protected:
  virtual void DoDispose (void);
private:
  virtual void DoAddDevice (Ptr<NetDevice> device) const = 0;

  uint32_t    m_id;         // Node id for this node
  uint32_t    m_sid;        // System id for this node
  std::vector<Ptr<NetDevice> > m_devices;
};

} //namespace ns3
#endif
