/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 The Georgia Institute of Technology 
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
 * Authors: Josh Pelkey <jpelkey@gatech.edu>
 */

#ifndef IPV4_NIX_VECTOR_HELPER_H
#define IPV4_NIX_VECTOR_HELPER_H

#include "ns3/object-factory.h"
#include "ns3/ipv4-routing-helper.h"

namespace ns3 {

/**
 * \brief Helper class that adds Nix-vector routing to nodes.
 *
 * This class is expected to be used in conjunction with 
 * ns3::InternetStackHelper::SetRoutingHelper
 *
 */
class Ipv4NixVectorHelper : public Ipv4RoutingHelper
{
public:
  /*
   * Construct an Ipv4NixVectorHelper to make life easier while adding Nix-vector
   * routing to nodes.
   */
  Ipv4NixVectorHelper ();

  /**
   * \brief Construct an Ipv4NixVectorHelper from another previously 
   * initialized instance (Copy Constructor).
   */
  Ipv4NixVectorHelper (const Ipv4NixVectorHelper &);

  /**
   * \internal
   * \returns pointer to clone of this Ipv4NixVectorHelper 
   * 
   * This method is mainly for internal use by the other helpers;
   * clients are expected to free the dynamic memory allocated by this method
   */
  Ipv4NixVectorHelper* Copy (void) const;

  /**
  * \param node the node on which the routing protocol will run
  * \returns a newly-created routing protocol
  *
  * This method will be called by ns3::InternetStackHelper::Install
  */
  virtual Ptr<Ipv4RoutingProtocol> Create (Ptr<Node> node) const;

private:
  /**
   * \internal
   * \brief Assignment operator declared private and not implemented to disallow
   * assignment and prevent the compiler from happily inserting its own.
   */
  Ipv4NixVectorHelper &operator = (const Ipv4NixVectorHelper &o);

  ObjectFactory m_agentFactory;
};
} // namespace ns3

#endif /* IPV4_NIX_VECTOR_HELPER_H */
