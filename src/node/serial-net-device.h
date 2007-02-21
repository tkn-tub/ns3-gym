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

#ifndef SERIAL_NET_DEVICE_H
#define SERIAL_NET_DEVICE_H

#include <string.h>
#include "mac-address.h"
#include "internet-node.h"
#include "net-device.h"
#include "ns3/callback.h"
#include "ns3/packet.h"

namespace ns3 {

class SerialChannel;
class SerialPhy;
class Queue;

class SerialNetDevice : public NetDevice {
friend class SerialPhy;
public:
  SerialNetDevice(Node* node, const MacAddress& addr);
  virtual ~SerialNetDevice();

private:
  // Don't let the compiler slip in copy and assignment construction
  SerialNetDevice(const SerialNetDevice&);
  SerialNetDevice&operator=(const SerialNetDevice&);

public:
  bool Attach(SerialChannel* ch);
  void AddQueue(Queue *);
  // called by ChannelSerial
  void Receive (Packet& p);

protected:
  Queue* GetQueue(void) const; 
  SerialChannel* GetChannel(void) const;

private:
  virtual void NotifyDataAvailable (void);
  virtual bool SendTo (Packet& p, const MacAddress& dest);

  SerialPhy* m_phy;
  SerialChannel* m_channel;
  Queue* m_queue;

};

}; // namespace ns3

#endif // SERIAL_NET_DEVICE_H

