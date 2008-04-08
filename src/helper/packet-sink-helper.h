/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
#ifndef PACKET_SINK_HELPER_H
#define PACKET_SINK_HELPER_H

#include "ns3/object-factory.h"
#include "ns3/ipv4-address.h"
#include "node-container.h"
#include "application-container.h"

namespace ns3 {

class PacketSinkHelper
{
public:
  PacketSinkHelper (std::string protocol, Address address);

  void SetAttribute (std::string name, Attribute value);

  ApplicationContainer Install (NodeContainer c);
private:
  ObjectFactory m_factory;
};

} // namespace ns3

#endif /* PACKET_SINK_HELPER_H */
