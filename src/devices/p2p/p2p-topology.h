/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2006 Georgia Tech Research Corporation
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: George F. Riley<riley@ece.gatech.edu>
//
// Topology helper for ns3.
// George F. Riley, Georgia Tech, Spring 2007

#ifndef __POINT_TO_POINT_TOPOLOGY_H__
#define __POINT_TO_POINT_TOPOLOGY_H__

// The topology class consists of only static methods thar are used to
// create the topology and data flows for an ns3 simulation

namespace ns3 {

class PointToPointChannel;
class Node;
class IPAddr;
class DataRate;
class Queue;

/**
 * \brief A helper class to create Topologies based on the 
 * ns3::PointToPointNetDevice and  ns3::PointToPointChannel objects.
 */
class PointToPointTopology {
public:
  /** 
   * \param n1 Node
   * \param n2 Node
   * \param rate Maximum transmission link rate 
   * \param delay one-way propagation delay 
   * \return Pointer to the underlying PointToPointChannel
   * 
   * Add a full-duplex point-to-point link between two nodes
   * and attach PointToPointNetDevices to the resulting
   * PointToPointChannel.  
   */
  static PointToPointChannel* AddPointToPointLink(
    Node* n1, Node* n2, const DataRate& rate, const Time& delay);

  /** 
   * \param chan PointToPointChannel to use
   * \param n1 Node
   * \param addr1 Ipv4 Address for n1
   * \param n2 Node
   * \param addr2 Ipv4 Address for n2
   * 
   * Add Ipv4Addresses to the Ipv4 interfaces associated with the 
   * two PointToPointNetDevices on the provided PointToPointChannel
   */
  static void AddIpv4Addresses(
    const PointToPointChannel* chan,
    Node* n1, const Ipv4Address& addr1,
    Node* n2, const Ipv4Address& addr2);

  /**
   * \param chan PointToPointChannel to use
   * \param n1 Node
   * \param n2 Node
   * 
   * For the given PointToPointChannel, for each Node, add an 
   * IPv4 host route to the IPv4 address of the peer node.  
   */
  static void AddIpv4Routes (Node*, Node*, const PointToPointChannel*);
};

} // namespace ns3

#endif

