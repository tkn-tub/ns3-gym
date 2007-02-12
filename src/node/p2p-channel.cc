// -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*-
//
// Copyright (c) 2006 Georgia Tech Research Corporation
// All rights reserved.
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

// Implementation of simple point-to-point channel
// George F. Riley, Georgia Tech, Spring 2007

#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "p2p-channel.h"
#include "p2p-net-device.h"
#include "node.h"

namespace ns3 {

P2PChannel::P2PChannel(const Time& delay, double maxRate) 
  : m_nd1(0), m_nd2(0),
    m_delay (delay),
    m_maxRate (maxRate)
{
}

// Channels create compatible net devices
P2PNetDevice* P2PChannel::CreateNetDevice(Node &node, MacAddress address)
{
  // Create a new point-to-point network device
  P2PNetDevice* nd = new P2PNetDevice(node, address);
  nd->Connect (this);
  // Add to list of peers
  if (!m_nd1) m_nd1 = nd;
  else m_nd2 = nd;
  return nd;
}

void P2PChannel::RemoveNetDevice(NetDevice* nd)
{
  if (nd == m_nd1) m_nd1 = 0;
  if (nd == m_nd2) m_nd2 = 0;
  // Now if all removed, remove the channel as well
  if (!m_nd1 && !m_nd2)
    {
      delete this;
    }
}

void P2PChannel::Send(P2PNetDevice *device, Packet& p, double rate)
{ // Schedule a receive event at receiver
  // First calculate time in future
  double maxRate;
  if (rate < m_maxRate)
    {
      maxRate = rate;
    } 
  else
    {
      maxRate = m_maxRate;
    }
  Time txTime = Seconds (p.GetSize() * 8 / maxRate);
  Time rxTime = m_delay + txTime;
  P2PNetDevice *to, *from;
  if (device == m_nd1)
    {
      from = m_nd1;
      to = m_nd2;
    }
  else
    {
      from = m_nd2;
      to = m_nd1;
    }
  // Schedule the receive event at receiver
  Simulator::Schedule(rxTime, &P2PNetDevice::Receive, to, p);
  // Schedule the link free event
  Simulator::Schedule(txTime, &P2PNetDevice::TxComplete, from);

}

}//namespace ns3
