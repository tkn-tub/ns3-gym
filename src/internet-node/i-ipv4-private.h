/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef I_IPV4_PRIVATE_H
#define I_IPV4_PRIVATE_H

#include "ns3/ns-unknown.h"
#include "ns3/ipv4-address.h"
#include "ns3/ptr.h"
#include <stdint.h>

namespace ns3 {

class Packet;
class Ipv4;
class TraceContext;
class TraceResolver;
class Ipv4Interface;
class NetDevice;

class IIpv4Private : public NsUnknown
{
public:
  static const Iid iid;
  IIpv4Private (Ipv4 *ipv4);
  virtual ~IIpv4Private ();

  TraceResolver *CreateTraceResolver (TraceContext const &context);
  void Send (Packet const &packet, Ipv4Address source, 
	     Ipv4Address destination, uint8_t protocol);
  Ptr<Ipv4Interface> FindInterfaceForDevice (NetDevice const*device);
  void Receive(Packet& p, NetDevice *device);
protected:
  virtual void DoDispose (void);
private:
  Ipv4 *m_ipv4;
};

} // namespace ns3

#endif /* I_IPV4_PRIVATE_H */
