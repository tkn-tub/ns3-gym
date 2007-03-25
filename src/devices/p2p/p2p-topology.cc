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

#include "ns3/debug.h"
#include "ns3/assert.h"

#include "ns3/nstime.h"

#include "ns3/internet-node.h"
#include "ns3/ipv4-address.h"
#include "ns3/drop-tail.h"
#include "ns3/arp-ipv4-interface.h"
#include "ns3/ipv4.h"

#include "p2p-channel.h"
#include "p2p-net-device.h"
#include "p2p-topology.h"

#define nil 0

namespace ns3 {

SerialChannel *
SerialTopology::AddSerialLink(
  Node* a,
  const Ipv4Address& addra,
  Node* b,
  const Ipv4Address& addrb,
  uint64_t bps,
  const Time& delay)
{
  SerialChannel* channel = new SerialChannel(bps, delay);

  // Duplex link is assumed to be subnetted as a /30
  // May run this unnumbered in the future?
  Ipv4Mask netmask("255.255.255.252");
  NS_ASSERT (netmask.IsMatch(addra,addrb));

  DropTailQueue* dtqa = new DropTailQueue();

  SerialNetDevice* neta = new SerialNetDevice(a);
  neta->AddQueue(dtqa);
  Ipv4Interface *interfA = new ArpIpv4Interface (a, neta);
  uint32_t indexA = a->GetIpv4 ()->AddInterface (interfA);
  neta->Attach (channel);

  interfA->SetAddress (addra);
  interfA->SetNetworkMask (netmask);
  interfA->SetUp ();

  DropTailQueue* dtqb = new DropTailQueue();

  SerialNetDevice* netb = new SerialNetDevice(b);
  netb->AddQueue(dtqb);
  Ipv4Interface *interfB = new ArpIpv4Interface (b, netb);
  uint32_t indexB = b->GetIpv4 ()->AddInterface (interfB);
  netb->Attach (channel);

  interfB->SetAddress (addrb);
  interfB->SetNetworkMask (netmask);
  interfB->SetUp ();

  a->GetIpv4 ()->AddHostRouteTo (addrb, indexA);
  b->GetIpv4 ()->AddHostRouteTo (addra, indexB);

  return channel;
}


#if 0
P2PChannel* Topology::AddDuplexLink(Node* n1, const IPAddr& ip1, 
                                    Node* n2, const IPAddr& ip2,
                                    const Rate& rate, const Time& delay)
{
  // First get the NetDeviceList capability from each node
  NetDeviceList* ndl1 = n1->GetNetDeviceList();
  NetDeviceList* ndl2 = n2->GetNetDeviceList();
  if (!ndl1 || !ndl2) return nil;  // Both ends must have NetDeviceList
  // Get the net devices
  P2PNetDevice* nd1 = ndl1->Add(P2PNetDevice(n1, rate, nil));
  P2PNetDevice* nd2 = ndl2->Add(P2PNetDevice(n1, rate, nd1->GetChannel()));
  // Not implemented yet.  Add appropriate layer 2 protocol for
  // the net devices.
  // Get the L3 proto for node 1 and configure it with this device
  L3Demux*    l3demux1 = n1->GetL3Demux();
  L3Protocol* l3proto1 = nil;
  // If the node 1 l3 demux exists, find the coresponding l3 protocol
  if (l3demux1) l3proto1 = l3demux1->Lookup(ip1.L3Proto());
  // If the l3 protocol exists, configure this net device.  Use a mask
  // of all ones, since there is only one device on the remote end
  // of this link
  if (l3proto1) l3proto1->AddNetDevice(nd1, ip1, ip1.GetMask(ip1.Size()*8));
  // Same for node 2
  L3Demux*    l3demux2 = n2->GetL3Demux();
  L3Protocol* l3proto2 = nil;
  // If the node 2 l3 demux exists, find the coresponding l3 protocol
  if (l3demux2) l3proto2 = l3demux2->Lookup(ip2.L3Proto());
  if (l3proto2) l3proto2->AddNetDevice(nd2, ip2, ip2.GetMask(ip2.Size()*8));
  return dynamic_cast<P2PChannel*>(nd1->GetChannel());  // Always succeeds
}

// Get the net device connecting node n1 to n2.  For topologies where
// there are possibly multiple devices connecting n1 and n2 (for example
// wireless with two devices on different channels) this will return
// the first one found.
NetDevice* Topology::GetNetDevice(Node* n1, Node* n2)
{
  // First get the NetDeviceList capability from node 1
  NetDeviceList* ndl1 = n1->GetNetDeviceList();
  if (!ndl1) return 0; // No devices, return nil
  // Get the list of devices
  const NetDeviceList::NetDevices_t& dlist = ndl1->GetAll();
  for (NetDeviceList::NetDevices_t::const_iterator i = dlist.Begin();
       i != dlist.End(); ++i)
    { // Check each device
      NetDevice* nd = *i; // next device
      Channel* c = nd->GetChannel();
      if (!c) continue; // No channel
      if (c->NodeIsPeer(n2)) return nd; // found it
    }
  return 0; // None found
}
  
// Get the channel connecting node n1 to node n2
Channel* Topology::GetChannel(Node* n1, Node* n2)
{
  NetDevice* nd = GetNetDevice(n1, n2);
  if (!nd) return 0; // No net device, so no channel
  return nd->GetChannel();
}

Queue* Topology::GetQueue(Node* n1, Node* n2)
{
  NetDevice* nd = GetNetDevice(n1, n2);
  if (!nd) return 0; // No net device, so in queue
  return nd->GetQueue();
}

Queue* Topology::SetQueue(Node* n1, Node* n2, const Queue& q)
{
  NetDevice* nd = GetNetDevice(n1, n2);
  if (!nd) return 0; // No net device, can't set queue
  // Add the specified queue to the netdevice
  return nd->SetQueue(q);
}

#endif

} // namespace ns3
 
