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
//class PointToPointNetDevice;
//class Queue;
//class Rate;
//class Time;
  
class PointToPointTopology {
public:
  // Manage point to point links

  // Add a full-duplex point-to-point link between two nodes
  // with the specified IP addresses,  with specified maximum transmission rate
  // and propagation delay.
  static PointToPointChannel* AddPointToPointLink(
    Node*, const Ipv4Address&, 
    Node*, const Ipv4Address&,
    // const Rate&,
    uint64_t,
    const Time&);

  // Get the connecting node n1 to node n2
  static Channel* GetChannel(Node*, Node*);
  // Get the NetDevice connecting node n1 to n2
  static NetDevice* GetNetDevice(Node*, Node*);
  /// Get the queue associated with a link between two nodes
  static Queue* GetQueue(Node*, Node*);
  // Set the queue associated with a link between two nodes
  static Queue* SetQueue(Node*, Node*, const Queue&);
};

} // namespace ns3

#endif

