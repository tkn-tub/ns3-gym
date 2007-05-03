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

#include "node.h"

namespace ns3 {

class Packet;

class InternetNode : public Node 
{
public:
  enum TraceType {
    IPV4,
    UDP,
    ARP,
  };
  InternetNode();
  virtual ~InternetNode ();
  virtual TraceResolver *CreateTraceResolver (TraceContext const &context);
  // Capability access
  virtual Ipv4 *           GetIpv4 (void) const;
  virtual Udp *            GetUdp (void) const;
  virtual Arp *            GetArp (void) const;

  void SetName(std::string name);
protected:
  virtual void DoDispose(void);
private:
  virtual void DoAddDevice (NetDevice *device) const;
  bool ReceiveFromDevice (NetDevice *device, const Packet &p, uint16_t protocolNumber) const;
  // Capabilities
  std::string      m_name;
};

}//namespace ns3

#endif /* INTERNET_NODE_H */
