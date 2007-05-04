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
#ifndef I_ARP_PRIVATE_H
#define I_ARP_PRIVATE_H

#include "ns3/ns-unknown.h"
#include "ns3/ipv4-address.h"

namespace ns3 {

class NetDevice;
class MacAddress;
class Packet;
class Arp;

class IArpPrivate : public NsUnknown
{
public:
  static const uint32_t iid;
  IArpPrivate (Arp *arp);
  virtual ~IArpPrivate ();
  bool Lookup (Packet &p, Ipv4Address destination, 
	       NetDevice *device,
	       MacAddress *hardwareDestination);
protected:
  virtual void DoDispose (void);
private:
  Arp *m_arp;
};

} // namespace ns3

#endif /* I_ARP_PRIVATE_H */
