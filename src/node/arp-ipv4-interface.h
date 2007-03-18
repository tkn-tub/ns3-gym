/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: 
 *  Mathieu Lacage <mathieu.lacage@sophia.inria.fr>,
 */
#ifndef ARP_IPV4_INTERFACE_H
#define ARP_IPV4_INTERFACE_H

#include "ipv4-interface.h"

namespace ns3 {

class Node;

class ArpIpv4Interface : public Ipv4Interface
{
 public:
  enum TraceType {
    NETDEVICE,
    ARP,
  };
  ArpIpv4Interface (Node *node, NetDevice *device);
  virtual ~ArpIpv4Interface ();

 private:
  virtual void SendTo (Packet p, Ipv4Address dest);
  virtual TraceResolver *DoCreateTraceResolver (TraceContext const &context);
  Node *m_node;
};

}//namespace ns3


#endif /* ARP_IPV4_INTERFACE_H */
