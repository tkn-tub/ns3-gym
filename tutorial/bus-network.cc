/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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
 */

#include "ns3/mac48-address.h"
#include "ns3/csma-net-device.h"
#include "ns3/csma-topology.h"
#include "ns3/csma-ipv4-topology.h"

#include "bus-network.h"
#include "ipv4-address-generator.h"

namespace ns3 {

BusNetwork::BusNetwork (
  Ipv4Mask      mask, 
  Ipv4Address   network, 
  Ipv4Address   startAddress, 
  DataRate      bps, 
  Time          delay, 
  uint32_t      n) 
{
  Ipv4AddressGenerator::SeedNetwork (mask, network);
  Ipv4AddressGenerator::SeedAddress (mask, startAddress);

  m_channel = CsmaTopology::CreateCsmaChannel (bps, delay);

  for (uint32_t i = 0; i < n; ++i)
    {
      Ptr<Node> node = Create<InternetNode> ();
      uint32_t nd = CsmaIpv4Topology::AddIpv4CsmaNetDevice (node, m_channel, 
        Mac48Address::Allocate ());
      Ipv4Address address = Ipv4AddressGenerator::AllocateAddress (mask, 
        network);
      CsmaIpv4Topology::AddIpv4Address (node, nd, address, mask);
      m_nodes.push_back (node);
    }
}

BusNetwork::~BusNetwork ()
{
  m_nodes.erase (m_nodes.begin (), m_nodes.end ());
}

  Ptr<Node>
BusNetwork::GetNode (uint32_t n)
{
  return m_nodes[n];
}

}; // namespace ns3
