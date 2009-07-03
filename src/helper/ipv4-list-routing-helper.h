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
#ifndef IPV4_LIST_ROUTING_HELPER_H
#define IPV4_LIST_ROUTING_HELPER_H

#include "ipv4-routing-helper.h"
#include <stdint.h>
#include <list>

namespace ns3 {

/**
 * \brief Helper class that adds ns3::Ipv4ListRouting objects
 *
 * This class is expected to be used in conjunction with 
 * ns3::InternetStackHelper::SetRoutingHelper
 */
class Ipv4ListRoutingHelper : public Ipv4RoutingHelper
{
public:
  Ipv4ListRoutingHelper();
  /**
   * \param routing a routing helper
   * \param priority the priority of the associated helper
   *
   * Store in the internal list a reference to the input routing helper
   * and associated priority. These helpers will be used later by
   * the ns3::Ipv4ListRoutingHelper::Create method to create
   * an ns3::Ipv4ListRouting object and add in it routing protocols
   * created with the helpers.
   */
  void Add (const Ipv4RoutingHelper &routing, int16_t priority);
  /**
   * \param node the node on which the routing protocol will run
   * \returns a newly-created routing protocol
   *
   * This method will be called by ns3::InternetStackHelper::Install
   */
  virtual Ptr<Ipv4RoutingProtocol> Create (Ptr<Node> node) const;
private:
  std::list<std::pair<const Ipv4RoutingHelper *,int16_t> > m_list;
};

} // namespace ns3

#endif /* IPV4_LIST_ROUTING_HELPER_H */
