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
 */

// The queue base class does not have any limit based on the number
// of packets or number of bytes. It is, conceptually, infinite 
// by default. Only subclasses define limitations.
// The base class implements tracing and basic statistics calculations.

#ifndef CHANNEL_SERIAL_H
#define CHANNEL_SERIAL_H

#include <list>
#include "serial-net-device.h"
#include "ns3/packet.h"

namespace ns3 {

// Simple SerialChannel class
class SerialChannel {
public:
  bool Send(Packet& p, SerialNetDevice *caller);
  void Attach (SerialNetDevice* nd);
private:
  typedef std::list<SerialNetDevice *> NetDevices;
  typedef std::list<SerialNetDevice *>::const_iterator  NetDevicesCI;
  NetDevices m_devices;
};

} // namespace ns3

#endif /* CHANNEL_SERIAL__H */
