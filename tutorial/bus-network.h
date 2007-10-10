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

#ifndef BUS_NETWORK_H
#define BUS_NETWORK_H

#include <list>
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/ipv4-address.h"
#include "ns3/data-rate.h"
#include "ns3/csma-channel.h"

namespace ns3 {

class BusNetwork
{
public:
  BusNetwork (
    Ipv4Mask      mask, 
    Ipv4Address   network, 
    Ipv4Address   startAddress, 
    DataRate      bps, 
    Time          delay, 
    uint32_t      n);

  virtual ~BusNetwork ();

  Ptr<Node> GetNode (uint32_t n);

private:
  std::vector<Ptr<Node> > m_nodes;
  Ptr<CsmaChannel> m_channel;
};

}; // namespace ns3

#endif /* BUS_NETWORK_H */
