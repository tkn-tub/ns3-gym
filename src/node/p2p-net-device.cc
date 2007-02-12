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

// Implementation of a point-to-point network device
// George F. Riley, Georgia Tech, Spring 2007

#include "p2p-net-device.h"
#include "p2p-channel.h"

namespace ns3 {

P2PNetDevice::P2PNetDevice (Node &node, MacAddress const &addr)
  : NetDevice (node, addr)
{}

P2PNetDevice::~P2PNetDevice()
{ // Inform channel that we are destroyed
  m_channel->RemoveNetDevice(this);
}

void 
P2PNetDevice::SetRate (double rate)
{
  m_rate = rate;
}

void 
P2PNetDevice::Connect (P2PChannel *channel)
{
  m_channel = channel;
}

bool 
P2PNetDevice::SendTo (Packet& p, const MacAddress&)
{
  m_channel->Send (this, p, m_rate);
  return true;
}

void
P2PNetDevice::Receive(Packet p)
{
  ForwardUp (p);
}

void 
P2PNetDevice::TxComplete (void)
{}

}//namespace ns3  
