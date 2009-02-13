/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 */

#ifndef TAP_BRIDGE_HELPER_H
#define TAP_BRIDGE_HELPER_H

#include "net-device-container.h"
#include "ns3/object-factory.h"
#include <string>

namespace ns3 {

class Node;
class AttributeValue;

class TapBridgeHelper
{
public:
  TapBridgeHelper (Ipv4Address gateway);
  void SetAttribute (std::string n1, const AttributeValue &v1);
  Ptr<NetDevice> Install (Ptr<Node> node, Ptr<NetDevice> nd);
private:
  ObjectFactory m_deviceFactory;
};

} // namespace ns3


#endif /* TAP_BRIDGE_HELPER_H */
