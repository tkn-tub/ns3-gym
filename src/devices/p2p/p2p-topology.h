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

#include "ns3/ptr.h"

// The topology class consists of only static methods thar are used to
// create the topology and data flows for an ns3 simulation

namespace ns3 {

class PointToPointChannel;
class Node;
class IPAddr;
class DataRate;
class Queue;
//class Time;

/**
 * \brief A helper class to create Topologies based on the ns3::PointToPointNetDevice and 
 *        ns3::PointToPointChannel objects.
 *
 * XXX ??
 * I think that some of the methods below are not implemented.
 * If so, remove them.
 */
class PointToPointTopology {
public:
  /** 
   * Add a full-duplex point-to-point link between two nodes
   * with the specified IP addresses,  with specified maximum transmission rate
   * and propagation delay.
   */
  static Ptr<PointToPointChannel> AddPointToPointLink(
    Ptr<Node>, Ptr<Node>, const DataRate&, const Time&);

  static bool AddIpv4Addresses(
    Ptr<const PointToPointChannel>,
    Ptr<Node>, const Ipv4Address&,
    Ptr<Node>, const Ipv4Address&);

  /**
   * Get the connecting node n1 to node n2
   */
  static Ptr<PointToPointChannel> GetChannel(Ptr<Node>, Ptr<Node>);
  /**
   * Get the NetDevice connecting node n1 to n2
   */
  static Ptr<PointToPointNetDevice> GetNetDevice(Ptr<Node>, Ptr<Node>);
  /**
   * Get the queue associated with a link between two nodes
   */
  static Queue* GetQueue(Ptr<Node>, Ptr<Node>);
  /**
   * Set the queue associated with a link between two nodes
   */
  static Queue* SetQueue(Ptr<Node>, Ptr<Node>, const Queue&);
};

} // namespace ns3

#endif

