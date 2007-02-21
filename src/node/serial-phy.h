/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
 * All rights reserved.
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
 *
 * Author: Craig Dowell <craigdo@ee.washington.edu>
 */

#ifndef SERIAL_PHY_H
#define SERIAL_PHY_H

#include <string.h>
#include "internet-node.h"
#include "serial-net-device.h"
#include "ns3/packet.h"

namespace ns3 {

class SerialPhy {
public:
  SerialPhy(Node* node, SerialNetDevice* netdevice);
  virtual ~SerialPhy();
  virtual void NotifyDataAvailable (void);
  void Receive (Packet& p);

private:
  Node* m_node;
  SerialNetDevice* m_netdevice;
};

} // namespace ns3

#endif // SERIAL_PHY_H
