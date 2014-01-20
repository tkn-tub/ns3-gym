/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef SIMPLE_CHANNEL_H
#define SIMPLE_CHANNEL_H

#include "ns3/channel.h"
#include "mac48-address.h"
#include <vector>

namespace ns3 {

class SimpleNetDevice;
class Packet;

/**
 * \ingroup channel
 * \brief A simple channel, for simple things and testing
 */
class SimpleChannel : public Channel
{
public:
  static TypeId GetTypeId (void);
  SimpleChannel ();

  /**
   * A packet is sent by a net device.  A receive event will be 
   * scheduled for all net device connected to the channel other 
   * than the net device who sent the packet
   *
   * \param p packet to be sent
   * \param protocol protocol number
   * \param to address to send packet to
   * \param from address the packet is coming from
   * \param sender netdevice who sent the packet
   *
   */
  virtual void Send (Ptr<Packet> p, uint16_t protocol, Mac48Address to, Mac48Address from,
                     Ptr<SimpleNetDevice> sender);

  /**
   * Attached a net device to the channel.
   *
   * \param device the device to attach to the channel
   */ 
  virtual void Add (Ptr<SimpleNetDevice> device);

  // inherited from ns3::Channel
  virtual uint32_t GetNDevices (void) const;
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const;

private:
  std::vector<Ptr<SimpleNetDevice> > m_devices;
};

} // namespace ns3

#endif /* SIMPLE_CHANNEL_H */
