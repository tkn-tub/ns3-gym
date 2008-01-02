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

//
// Topology helper for ns3.
// George F. Riley, Georgia Tech, Spring 2007

#include <algorithm>
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/fatal-error.h"
#include "ns3/nstime.h"
#include "ns3/internet-node.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4.h"
#include "ns3/queue.h"

#include "point-to-point-channel.h"
#include "point-to-point-net-device.h"
#include "point-to-point-topology.h"

namespace ns3 {

Ptr<PointToPointChannel>
PointToPointTopology::AddPointToPointLink(
  Ptr<Node> n1,
  Ptr<Node> n2,
  const DataRate& bps,
  const Time& delay)
{
  Ptr<PointToPointChannel> channel = CreateObject<PointToPointChannel> (bps, delay);

  Ptr<PointToPointNetDevice> net1 = CreateObject<PointToPointNetDevice> (n1);

  Ptr<Queue> q = Queue::CreateDefault ();
  net1->AddQueue(q);
  net1->Attach (channel);
  
  Ptr<PointToPointNetDevice> net2 = CreateObject<PointToPointNetDevice> (n2);

  q = Queue::CreateDefault ();
  net2->AddQueue(q);
  net2->Attach (channel);

  return channel;
}

Ptr<PointToPointNetDevice> 
PointToPointTopology::GetNetDevice (Ptr<Node> n, Ptr<PointToPointChannel> chan)
{
  Ptr<PointToPointNetDevice> found = 0;

  // The PointToPoint channel is used to find the relevant NetDevice
  NS_ASSERT (chan->GetNDevices () == 2);
  Ptr<PointToPointNetDevice> nd1 = chan->GetDevice (0);
  Ptr<PointToPointNetDevice> nd2 = chan->GetDevice (1);
  if ( nd1->GetNode ()->GetId () == n->GetId () ) 
    {
      found = nd1;
    }
  else if ( nd2->GetNode ()->GetId () == n->GetId () ) 
    {
      found = nd2;
    }
  else
    {
      NS_ASSERT (found);
    }
  return found;
}

void
PointToPointTopology::AddIpv4Addresses(
  Ptr<const PointToPointChannel> chan,
  Ptr<Node> n1, const Ipv4Address& addr1,
  Ptr<Node> n2, const Ipv4Address& addr2)
{

  // Duplex link is assumed to be subnetted as a /30
  // May run this unnumbered in the future?
  Ipv4Mask netmask("255.255.255.252");
  NS_ASSERT (netmask.IsMatch(addr1,addr2));

  // The PointToPoint channel is used to find the relevant NetDevices
  NS_ASSERT (chan->GetNDevices () == 2);
  Ptr<NetDevice> nd1 = chan->GetDevice (0);
  Ptr<NetDevice> nd2 = chan->GetDevice (1);
  // Make sure that nd1 belongs to n1 and nd2 to n2
  if ( (nd1->GetNode ()->GetId () == n2->GetId () ) && 
       (nd2->GetNode ()->GetId () == n1->GetId () ) )
    {
      std::swap(nd1, nd2);
    }
  NS_ASSERT (nd1->GetNode ()->GetId () == n1->GetId ());
  NS_ASSERT (nd2->GetNode ()->GetId () == n2->GetId ());
  
  Ptr<Ipv4> ip1 = n1->QueryInterface<Ipv4> ();
  uint32_t index1 = ip1->AddInterface (nd1);

  ip1->SetAddress (index1, addr1);
  ip1->SetNetworkMask (index1, netmask);
  ip1->SetUp (index1);

  Ptr<Ipv4> ip2 = n2->QueryInterface<Ipv4> ();
  uint32_t index2 = ip2->AddInterface (nd2);

  ip2->SetAddress (index2, addr2);
  ip2->SetNetworkMask (index2, netmask);
  ip2->SetUp (index2);
  
}

void
PointToPointTopology::SetIpv4Metric(
  Ptr<const PointToPointChannel> chan,
  Ptr<Node> n1, Ptr<Node> n2, uint16_t metric)
{

  // The PointToPoint channel is used to find the relevant NetDevices
  NS_ASSERT (chan->GetNDevices () == 2);
  Ptr<NetDevice> nd1 = chan->GetDevice (0);
  Ptr<NetDevice> nd2 = chan->GetDevice (1);
  // Make sure that nd1 belongs to n1 and nd2 to n2
  if ( (nd1->GetNode ()->GetId () == n2->GetId () ) && 
       (nd2->GetNode ()->GetId () == n1->GetId () ) )
    {
      std::swap(nd1, nd2);
    }
  NS_ASSERT (nd1->GetNode ()->GetId () == n1->GetId ());
  NS_ASSERT (nd2->GetNode ()->GetId () == n2->GetId ());
  
  // The NetDevice ifIndex does not correspond to the
  // ifIndex used by Ipv4.  Therefore, we have to iterate
  // through the NetDevices until we find the Ipv4 ifIndex
  // that corresponds to NetDevice nd1
  // Get interface indexes for both nodes corresponding to the right channel
  uint32_t index = 0;
  bool found = false;
  Ptr<Ipv4> ip1 = n1->QueryInterface<Ipv4> ();
  for (uint32_t i = 0; i < ip1->GetNInterfaces (); i++)
    {
      if (ip1 ->GetNetDevice (i) == nd1)
        {
          index = i;
          found = true;
        }
    }
  NS_ASSERT(found);
  ip1->SetMetric (index, metric);

  index = 0;
  found = false;
  Ptr<Ipv4> ip2 = n2->QueryInterface<Ipv4> ();
  for (uint32_t i = 0; i < ip2->GetNInterfaces (); i++)
    {
      if (ip2 ->GetNetDevice (i) == nd2)
        {
          index = i;
          found = true;
        }
    }
  NS_ASSERT(found);
  ip2->SetMetric (index, metric);
}

void
PointToPointTopology::AddIpv4Routes (
  Ptr<Node> n1, Ptr<Node> n2, Ptr<const PointToPointChannel> chan)
{ 
  // The PointToPoint channel is used to find the relevant NetDevices
  NS_ASSERT (chan->GetNDevices () == 2);
  Ptr<NetDevice> nd1 = chan->GetDevice (0);
  Ptr<NetDevice> nd2 = chan->GetDevice (1);

  // Assert that n1 is the Node owning one of the two NetDevices
  // and make sure that nd1 corresponds to it
  if (nd1->GetNode ()->GetId () == n1->GetId ())
    {
      ; // Do nothing
    }
  else if (nd2->GetNode ()->GetId () == n1->GetId ())
    {
      std::swap(nd1, nd2);
    }
  else
    {
      NS_FATAL_ERROR("P2PTopo: Node does not contain an interface on Channel");
    }

   // Assert that n2 is the Node owning one of the two NetDevices
   // and make sure that nd2 corresponds to it
  if (nd2->GetNode ()->GetId () != n2->GetId ())
    {
      NS_FATAL_ERROR("P2PTopo: Node does not contain an interface on Channel");
    }

  // Assert that both are Ipv4 nodes
  Ptr<Ipv4> ip1 = nd1->GetNode ()->QueryInterface<Ipv4> ();
  Ptr<Ipv4> ip2 = nd2->GetNode ()->QueryInterface<Ipv4> ();
  NS_ASSERT(ip1 != 0 && ip2 != 0);

  // Get interface indexes for both nodes corresponding to the right channel
  uint32_t index1 = 0;
  bool found = false;
  for (uint32_t i = 0; i < ip1->GetNInterfaces (); i++)
    {
      if (ip1 ->GetNetDevice (i) == nd1)
        {
          index1 = i;
          found = true;
        }
    }
  NS_ASSERT(found);

  uint32_t index2 = 0;
  found = false;
  for (uint32_t i = 0; i < ip2->GetNInterfaces (); i++)
    {
      if (ip2 ->GetNetDevice (i) == nd2)
        {
          index2 = i;
          found = true;
        }
    }
  NS_ASSERT(found);

  ip1->AddHostRouteTo (ip2-> GetAddress (index2), index1);
  ip2->AddHostRouteTo (ip1-> GetAddress (index1), index2); 
}

} // namespace ns3
 
