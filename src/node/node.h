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

#include "ns3/ns-unknown.h"

namespace ns3 {

class ApplicationList;
class Ipv4;
class Udp;
class Arp;

class TraceContext;
class TraceResolver;
class NetDevice;

class Node : public NsUnknown
{
public:
  static const uint32_t iid;

  Node();
  Node(uint32_t); // Specify which system for a distributed simulation
  virtual ~Node();

  virtual TraceResolver *CreateTraceResolver (TraceContext const &context) = 0;

  uint32_t GetId (void) const;
  uint32_t GetSystemId (void) const;
  void SetSystemId(uint32_t s);

  uint32_t AddDevice (NetDevice *device);
  NetDevice *GetDevice (uint32_t index) const;
  uint32_t GetNDevices (void) const;

protected:
  virtual void DoDispose (void);
private:
  virtual void DoAddDevice (NetDevice *device) const = 0;

public:
  // Virtual "Getters" for each capability.
  // These exist to allow owners of a generic Node pointer to get
  // a pointer to the underlying capability, a pointer to a "NULL"
  // capability if one exists, or the nil pointer if not.
  // Each of these has a default behavior of returning a null capability
  // of the correct type if one exists, or the nil pointer if no
  // null capability exists.
  virtual ApplicationList* GetApplicationList() const;
  virtual Ipv4 *           GetIpv4 (void) const;
  virtual Udp *            GetUdp (void) const;
  virtual Arp *            GetArp (void) const;
  
private:
  uint32_t    m_id;         // Node id for this node
  uint32_t    m_sid;        // System id for this node
  std::vector<NetDevice *> m_devices;
};

} //namespace ns3
#endif
