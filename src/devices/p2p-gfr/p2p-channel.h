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

// Definition of a simple point-to-point channel
// George F. Riley, Georgia Tech, Spring 2007

#ifndef P2P_CHANNEL_H
#define P2P_CHANNEL_H

#include "ns3/nstime.h"
#include "ns3/mac-address.h"

namespace ns3 {

class P2PNetDevice;
class NetDevice;
class Node;
class Packet;

class P2PChannel  {
public:
  P2PChannel(const Time& delay, double maxRate /* bits/s */);
  ~P2PChannel();

  P2PNetDevice* CreateNetDevice(Node *node, MacAddress address);
  void       RemoveNetDevice (NetDevice *device);
  void       Send (P2PNetDevice *device, Packet&p, double rate /* bits/s */);
private:
  // The two endpoints of this channel
  P2PNetDevice* m_nd1;
  P2PNetDevice* m_nd2;
  Time          m_delay;
  double        m_maxRate;
};

}//namespace ns3

#endif
