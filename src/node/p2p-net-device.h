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

// Definition for a Point-to-Point network device
// George F. Riley, Georgia Tech, Spring 2007

#ifndef P2P_NET_DEVICE_H
#define P2P_NET_DEVICE_H

#include "net-device.h"

namespace ns3 {

class P2PChannel;

class P2PNetDevice : public NetDevice {
public:
  P2PNetDevice(Node *node, MacAddress const &addr);
  virtual ~P2PNetDevice();

  void SetRate (double rate);
  void Connect (P2PChannel *channel);
  void Receive(Packet p);
  void TxComplete (void);
 private:
  virtual bool SendTo (Packet& p, const MacAddress& dest);
  double m_rate;
  P2PChannel *m_channel;
};

}//namespace ns3

#endif /* P2P_NET_DEVICE_H */

