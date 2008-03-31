/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
#ifndef ON_OFF_HELPER_H
#define ON_OFF_HELPER_H

#include <stdint.h>
#include <string>
#include "ns3/object-factory.h"
#include "ns3/address.h"
#include "ns3/attribute.h"
#include "ns3/net-device.h"
#include "node-container.h"
#include "application-container.h"

namespace ns3 {

class OnOffHelper
{
public:
  OnOffHelper ();

  void SetUdpRemote (Ipv4Address ip, uint16_t port);
  void SetTcpRemote (Ipv4Address ip, uint16_t port);
  void SetPacketRemote (Ptr<NetDevice> source, Address destination, uint16_t protocol);
  
  void SetAppAttribute (std::string name, Attribute value);

  ApplicationContainer Build (NodeContainer c);

private:
  std::string m_protocol;
  Address m_remote;
  ObjectFactory m_factory;
};

} // namespace ns3

#endif /* ON_OFF_HELPER_H */

