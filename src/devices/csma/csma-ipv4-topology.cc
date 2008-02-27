/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2007 Emmanuelle Laprise
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
// Author: Emmanuelle Laprise <emmanuelle.laprise@bluekazoo.ca>
//

#include <algorithm>
#include "ns3/assert.h"
#include "ns3/fatal-error.h"
#include "ns3/nstime.h"
#include "ns3/internet-node.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4.h"
#include "ns3/queue.h"
#include "ns3/string.h"

#include "csma-channel.h"
#include "csma-net-device.h"
#include "csma-ipv4-topology.h"

namespace ns3 {

uint32_t
CsmaIpv4Topology::AddIpv4CsmaNetDevice(
  Ptr<Node> node,
  Ptr<CsmaChannel> channel,
  Mac48Address addr)
{
  Ptr<Queue> q = Queue::CreateDefault ();

  // assume full-duplex
  Ptr<CsmaNetDevice> nd = CreateObjectWith<CsmaNetDevice> ("Node", node, 
                                                           "Address", addr, 
                                                           "EncapsulationMode", String ("IpArp"));
  node->AddDevice (nd);

  nd->AddQueue(q);
  nd->Attach (channel);
  return nd->GetIfIndex ();
}


void
CsmaIpv4Topology::AddIpv4LlcCsmaNode(Ptr<Node> n1,
                                     Ptr<CsmaChannel> ch,
                                     Mac48Address addr)
{
  Ptr<Queue> q = Queue::CreateDefault ();

  Ptr<CsmaNetDevice> nd0 = CreateObjectWith<CsmaNetDevice> ("Node", n1, 
                                                            "Address", addr,
                                                            "EncapsulationMode", String ("Llc"));
  n1->AddDevice (nd0);
  nd0->SetSendEnable (true);
  nd0->SetReceiveEnable (false);
  nd0->AddQueue(q);
  nd0->Attach (ch);

  Ptr<CsmaNetDevice> nd1 = CreateObjectWith<CsmaNetDevice> ("Node", n1, 
                                                            "Address", addr,
                                                            "EncapsulationMode", String ("Llc"));
  n1->AddDevice (nd1);
  nd1->SetSendEnable (false);
  nd1->SetReceiveEnable (true);
  nd1->AddQueue(q);
  nd1->Attach (ch);
}

void
CsmaIpv4Topology::AddIpv4RawCsmaNode(Ptr<Node> n1,
                                     Ptr<CsmaChannel> ch,
                                     Mac48Address addr)
{
  Ptr<Queue> q = Queue::CreateDefault ();

  Ptr<CsmaNetDevice> nd0 = CreateObjectWith<CsmaNetDevice> ("Node", n1, 
                                                            "Address", addr,
                                                            "EncapsulationMode", String ("Raw"));
  n1->AddDevice (nd0);
  nd0->SetSendEnable (true);
  nd0->SetReceiveEnable (false);
  nd0->AddQueue(q);
  nd0->Attach (ch);

  Ptr<CsmaNetDevice> nd1 = CreateObjectWith<CsmaNetDevice> ("Node", n1, 
                                                            "Address", addr,
                                                            "EncapsulationMode", String ("Raw"));
  n1->AddDevice (nd1);
  nd1->SetSendEnable (false);
  nd1->SetReceiveEnable (true);

  nd1->AddQueue(q);
  nd1->Attach (ch);
}

uint32_t
CsmaIpv4Topology::AddIpv4Address(
  Ptr<Node>             node,
  uint32_t              netDeviceNumber,
  const Ipv4Address     address,
  const Ipv4Mask        mask,
  uint16_t              metric)
{
  Ptr<NetDevice> nd = node->GetDevice(netDeviceNumber);

  Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
  uint32_t ifIndex = ipv4->AddInterface (nd);

  ipv4->SetAddress (ifIndex, address);
  ipv4->SetNetworkMask (ifIndex, mask);
  ipv4->SetMetric (ifIndex, metric);
  ipv4->SetUp (ifIndex);
  return ifIndex;
}

void
CsmaIpv4Topology::AddIpv4Routes (
  Ptr<NetDevice> nd1, Ptr<NetDevice> nd2)
{ 
  // Assert that both are Ipv4 nodes
  Ptr<Ipv4> ip1 = nd1->GetNode ()->GetObject<Ipv4> ();
  Ptr<Ipv4> ip2 = nd2->GetNode ()->GetObject<Ipv4> ();
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
  NS_ASSERT (found);

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
  NS_ASSERT (found);

  ip1->AddHostRouteTo (ip2-> GetAddress (index2), index1);
  ip2->AddHostRouteTo (ip1-> GetAddress (index1), index2); 
}

} // namespace ns3
 
