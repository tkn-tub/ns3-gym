/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Author: Gustavo Carneiro  <gjc@inescporto.pt>
 */
#ifndef BRIDGE_CHANNEL_H
#define BRIDGE_CHANNEL_H

#include "ns3/net-device.h"
#include "ns3/channel.h"
#include <vector>

namespace ns3 {

/**
 * \ingroup netdevice
 * 
 * \brief bridge net device for bridge things and testing
 */
class BridgeChannel : public Channel
{
public:
  static TypeId GetTypeId (void);
  BridgeChannel ();
  virtual ~BridgeChannel ();

  void AddChannel (Ptr<Channel> bridgedChannel);

  // virtual methods implementation, from Channel
  virtual uint32_t GetNDevices (void) const;
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const;

private:
  std::vector< Ptr<Channel> > m_bridgedChannels;

};

} // namespace ns3

#endif /* BRIDGE_CHANNEL_H */
