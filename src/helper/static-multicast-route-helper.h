/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Washington
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
 * Author: Tom Henderson <tomhend@u.washington.edu>
 */
#ifndef STATIC_MULTICAST_ROUTE_HELPER_H
#define STATIC_MULTICAST_ROUTE_HELPER_H

#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"
#include "ns3/node.h"
#include "ns3/net-device.h"
#include "node-container.h"
#include "net-device-container.h"

namespace ns3 {

class StaticMulticastRouteHelper
{
public:
  StaticMulticastRouteHelper ();

  void AddMulticastRoute (Ptr<Node>, Ipv4Address source, Ipv4Address group,  
    Ptr<NetDevice> input, NetDeviceContainer output);

  void SetDefaultMulticastRoute (Ptr<Node> n, Ptr<NetDevice> nd);

  void JoinMulticastGroup (Ptr<Node> n, Ipv4Address source, Ipv4Address group);

};

} // namespace ns3

#endif /* STATIC_MULTICAST_ROUTE_HELPER_H */
