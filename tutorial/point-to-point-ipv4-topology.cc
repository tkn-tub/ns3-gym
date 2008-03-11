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

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/nstime.h"
#include "ns3/internet-node.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4.h"
#include "ns3/queue.h"
#include "ns3/drop-tail-queue.h"

#include "ns3/point-to-point-channel.h"
#include "ns3/point-to-point-net-device.h"
#include "point-to-point-ipv4-topology.h"

namespace ns3 {

  Ptr<PointToPointChannel>
PointToPointIpv4Topology::CreateChannel (
  const DataRate& bps,
  const Time& delay)
{
  return CreateObject<PointToPointChannel> ("BitRate", bps, "Delay", delay);
}

  uint32_t
PointToPointIpv4Topology::AddNetDevice (
  Ptr<Node> node,
  Ptr<PointToPointChannel> channel)
{
  NS_ASSERT (channel->GetNDevices () <= 1);

  Ptr<PointToPointNetDevice> nd = 
    CreateObject<PointToPointNetDevice> ("Node", node, 
                                             "Address", Mac48Address::Allocate ());
  node->AddDevice (nd);
  Ptr<Queue> q = CreateObject<DropTailQueue> ();
  nd->AddQueue(q);
  nd->Attach (channel);

  return nd->GetIfIndex ();
}

  uint32_t
PointToPointIpv4Topology::AddAddress (
  Ptr<Node> node,
  uint32_t netDeviceNumber,
  Ipv4Address address,
  Ipv4Mask mask)
{
  Ptr<NetDevice> nd = node->GetDevice(netDeviceNumber);
  Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
  uint32_t ifIndex = ipv4->AddInterface (nd);

  ipv4->SetAddress (ifIndex, address);
  ipv4->SetNetworkMask (ifIndex, mask);
  ipv4->SetUp (ifIndex);

  return ifIndex;
}

} // namespace ns3
