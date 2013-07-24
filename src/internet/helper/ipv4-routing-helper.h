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

#ifndef IPV4_ROUTING_HELPER_H
#define IPV4_ROUTING_HELPER_H

#include "ns3/ptr.h"
#include "ns3/nstime.h"
#include "ns3/output-stream-wrapper.h"

namespace ns3 {

class Ipv4RoutingProtocol;
class Node;

/**
 * \brief a factory to create ns3::Ipv4RoutingProtocol objects
 *
 * For each new routing protocol created as a subclass of 
 * ns3::Ipv4RoutingProtocol, you need to create a subclass of 
 * ns3::Ipv4RoutingHelper which can be used by 
 * ns3::InternetStackHelper::SetRoutingHelper and 
 * ns3::InternetStackHelper::Install.
 */
class Ipv4RoutingHelper
{
public:
  /*
   * Destroy an instance of an Ipv4RoutingHelper
   */
  virtual ~Ipv4RoutingHelper ();

  /**
   * \brief virtual constructor
   * \returns pointer to clone of this Ipv4RoutingHelper 
   * 
   * This method is mainly for internal use by the other helpers;
   * clients are expected to free the dynamic memory allocated by this method
   */
  virtual Ipv4RoutingHelper* Copy (void) const = 0;

  /**
   * \param node the node within which the new routing protocol will run
   * \returns a newly-created routing protocol
   */
  virtual Ptr<Ipv4RoutingProtocol> Create (Ptr<Node> node) const = 0;

  /**
   * \brief prints the routing tables of all nodes at a particular time.
   * \param printTime the time at which the routing table is supposed to be printed.
   * \param stream The output stream object to use 
   *
   * This method calls the PrintRoutingTable() method of the 
   * Ipv4RoutingProtocol stored in the Ipv4 object, for all nodes at the
   * specified time; the output format is routing protocol-specific.
   */
  void PrintRoutingTableAllAt (Time printTime, Ptr<OutputStreamWrapper> stream) const;

  /**
   * \brief prints the routing tables of all nodes at regular intervals specified by user.
   * \param printInterval the time interval for which the routing table is supposed to be printed.
   * \param stream The output stream object to use
   *
   * This method calls the PrintRoutingTable() method of the 
   * Ipv4RoutingProtocol stored in the Ipv4 object, for all nodes at the
   * specified time interval; the output format is routing protocol-specific.
   */
  void PrintRoutingTableAllEvery (Time printInterval, Ptr<OutputStreamWrapper> stream) const;

  /**
   * \brief prints the routing tables of a node at a particular time.
   * \param printTime the time at which the routing table is supposed to be printed.
   * \param node The node ptr for which we need the routing table to be printed
   * \param stream The output stream object to use
   *
   * This method calls the PrintRoutingTable() method of the 
   * Ipv4RoutingProtocol stored in the Ipv4 object, for the selected node 
   * at the specified time; the output format is routing protocol-specific.
   */
  void PrintRoutingTableAt (Time printTime, Ptr<Node> node, Ptr<OutputStreamWrapper> stream) const;

  /**
   * \brief prints the routing tables of a node at regular intervals specified by user.
   * \param printInterval the time interval for which the routing table is supposed to be printed.
   * \param node The node ptr for which we need the routing table to be printed
   * \param stream The output stream object to use
   *
   * This method calls the PrintRoutingTable() method of the 
   * Ipv4RoutingProtocol stored in the Ipv4 object, for the selected node 
   * at the specified interval; the output format is routing protocol-specific.
   */
  void PrintRoutingTableEvery (Time printInterval, Ptr<Node> node, Ptr<OutputStreamWrapper> stream) const;

  /**
   * \brief Request a specified routing protocol <T> from Ipv4RoutingProtocol protocol
   *
   * If protocol is Ipv4ListRouting, then protocol will be searched in the list,
   * otherwise a simple DynamicCast will be performed
   *
   * \param protocol Smart pointer to Ipv4RoutingProtocol object
   * \return a Smart Pointer to the requested protocol (zero if the protocol can't be found)
   */
  template<class T>
  static Ptr<T> GetRouting (Ptr<Ipv4RoutingProtocol> protocol);
  
private:
  void Print (Ptr<Node> node, Ptr<OutputStreamWrapper> stream) const;
  void PrintEvery (Time printInterval, Ptr<Node> node, Ptr<OutputStreamWrapper> stream) const;
};

} // namespace ns3


#endif /* IPV4_ROUTING_HELPER_H */
