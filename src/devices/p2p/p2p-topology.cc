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
#include "ns3/debug.h"
#include "ns3/fatal-error.h"
#include "ns3/nstime.h"
#include "ns3/internet-node.h"
#include "ns3/ipv4-address.h"
#include "ns3/i-ipv4.h"
#include "ns3/queue.h"

#include "p2p-channel.h"
#include "p2p-net-device.h"
#include "p2p-topology.h"

namespace ns3 {

Ptr<PointToPointChannel>
PointToPointTopology::AddPointToPointLink(
  Ptr<Node> n1,
  Ptr<Node> n2,
  const DataRate& bps,
  const Time& delay)
{
  Ptr<PointToPointChannel> channel = Create<PointToPointChannel> (bps, delay);

  Ptr<PointToPointNetDevice> net1 = Create<PointToPointNetDevice> (n1);

  Ptr<Queue> q = Queue::CreateDefault ();
  net1->AddQueue(q);
  net1->Attach (channel);
  
  Ptr<PointToPointNetDevice> net2 = Create<PointToPointNetDevice> (n2);

  q = Queue::CreateDefault ();
  net2->AddQueue(q);
  net2->Attach (channel);

  return channel;
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
  
  Ptr<IIpv4> ip1 = n1->QueryInterface<IIpv4> (IIpv4::iid);
  uint32_t index1 = ip1->AddInterface (nd1);

  ip1->SetAddress (index1, addr1);
  ip1->SetNetworkMask (index1, netmask);
  ip1->SetUp (index1);

  Ptr<IIpv4> ip2 = n2->QueryInterface<IIpv4> (IIpv4::iid);
  uint32_t index2 = ip2->AddInterface (nd2);

  ip2->SetAddress (index2, addr2);
  ip2->SetNetworkMask (index2, netmask);
  ip2->SetUp (index2);
  
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
  Ptr<IIpv4> ip1 = nd1->GetNode ()->QueryInterface<IIpv4> (IIpv4::iid);
  Ptr<IIpv4> ip2 = nd2->GetNode ()->QueryInterface<IIpv4> (IIpv4::iid);
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
 
