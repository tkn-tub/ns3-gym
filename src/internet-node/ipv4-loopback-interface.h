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
#ifndef IPV4_LOOPBACK_INTERFACE_H
#define IPV4_LOOPBACK_INTERFACE_H

#include "ipv4-interface.h"
#include "ns3/ptr.h"

namespace ns3 {

class INode;

class Ipv4LoopbackInterface : public Ipv4Interface 
{
 public:
  Ipv4LoopbackInterface (Ptr<INode> node);
  virtual ~Ipv4LoopbackInterface ();

 private:
  virtual void SendTo (Packet p, Ipv4Address dest);
  virtual TraceResolver *DoCreateTraceResolver (TraceContext const &context);

  Ptr<INode> m_node;
};

}//namespace ns3


#endif /* IPV4_LOOPBACK_INTERFACE_H */
