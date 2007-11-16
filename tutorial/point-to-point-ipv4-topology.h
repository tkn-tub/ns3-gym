/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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

#ifndef POINT_TO_POINT_IPV4_TOPOLOGY_H
#define POINT_TO_POINT_IPV4_TOPOLOGY_H

#include "ns3/ptr.h"

namespace ns3 {

class PointToPointChannel;
class Node;
class Ipv4Address;
class Ipv4Mask;
class DataRate;

class PointToPointIpv4Topology {
public:
  static Ptr<PointToPointChannel> CreateChannel (
    const DataRate& dataRate, const Time& delay);

  static uint32_t AddNetDevice(
    Ptr<Node> node,
    Ptr<PointToPointChannel> channel);

  static uint32_t AddAddress(
    Ptr<Node> node,
    uint32_t ndIndex,
    Ipv4Address address,
    Ipv4Mask mask);
};

} // namespace ns3

#endif // POINT_TO_POINT_IPV4_TOPOLOGY_H

