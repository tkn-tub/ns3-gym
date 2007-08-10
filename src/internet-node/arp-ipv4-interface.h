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
#include "ns3/ptr.h"

namespace ns3 {

class Node;

/**
 * \brief an Ipv4 Interface which uses ARP
 *
 * If you need to use ARP on top of a specific NetDevice, you
 * can use this Ipv4Interface subclass to wrap it for the Ipv4 class
 * when calling Ipv4::AddInterface.
 */
class ArpIpv4Interface : public Ipv4Interface
{
 public:
  ArpIpv4Interface (Ptr<Node> node, Ptr<NetDevice> device);
  virtual ~ArpIpv4Interface ();

 private:
  virtual void SendTo (Packet p, Ipv4Address dest);
  virtual TraceResolver *DoCreateTraceResolver (TraceContext const &context);
  Ptr<Node> m_node;
};

}//namespace ns3


#endif /* ARP_IPV4_INTERFACE_H */
