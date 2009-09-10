/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

#ifndef IPV6_STATIC_ROUTING_HELPER_H
#define IPV6_STATIC_ROUTING_HELPER_H

#include "ns3/ipv6.h"
#include "ns3/ipv6-static-routing.h"
#include "ns3/ptr.h"
#include "ns3/ipv6-address.h"
#include "ns3/node.h"
#include "ns3/net-device.h"
#include "ipv6-routing-helper.h"
#include "node-container.h"
#include "net-device-container.h"

namespace ns3 {

/**
 * \brief Helper class that adds ns3::Ipv6StaticRouting objects
 *
 * This class is expected to be used in conjunction with 
 * ns3::InternetStackHelper::SetRoutingHelper
 */
class Ipv6StaticRoutingHelper : public Ipv6RoutingHelper
{
public:
  Ipv6StaticRoutingHelper ();

  /**
   * \param node the node on which the routing protocol will run
   * \returns a newly-created routing protocol
   *
   * This method will be called by ns3::InternetStackHelper::Install
   */
  virtual Ptr<Ipv6RoutingProtocol> Create (Ptr<Node> node) const;

  Ptr<Ipv6StaticRouting> GetStaticRouting (Ptr<Ipv6> ipv6) const;

  void AddMulticastRoute (Ptr<Node> n, Ipv6Address source, Ipv6Address group,  
    Ptr<NetDevice> input, NetDeviceContainer output);
  void AddMulticastRoute (std::string n, Ipv6Address source, Ipv6Address group,  
    Ptr<NetDevice> input, NetDeviceContainer output);
  void AddMulticastRoute (Ptr<Node> n, Ipv6Address source, Ipv6Address group,  
    std::string inputName, NetDeviceContainer output);
  void AddMulticastRoute (std::string nName, Ipv6Address source, Ipv6Address group,  
    std::string inputName, NetDeviceContainer output);

#if 0
  /**
   * \brief Add a default route to the static routing protocol to forward
   *        packets out a particular interface
   */
  void SetDefaultMulticastRoute (Ptr<Node> n, Ptr<NetDevice> nd);
  void SetDefaultMulticastRoute (Ptr<Node> n, std::string ndName);
  void SetDefaultMulticastRoute (std::string nName, Ptr<NetDevice> nd);
  void SetDefaultMulticastRoute (std::string nName, std::string ndName);
#endif
};

} // namespace ns3

#endif /* IPV6_STATIC_ROUTING_HELPER_H */

