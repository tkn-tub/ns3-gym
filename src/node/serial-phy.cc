/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "ns3/debug.h"
#include "serial-phy.h"
#include "serial-net-device.h"
#include "serial-channel.h"
#include "queue.h"

NS_DEBUG_COMPONENT_DEFINE ("SerialPhy");

namespace ns3 {

SerialPhy::SerialPhy(Node* node, SerialNetDevice* netdevice) :
  m_node(node), m_netdevice(netdevice)
{
  NS_DEBUG ("SerialPhy::SerialPhy (" << node << ", " << netdevice << ")");
}

SerialPhy::~SerialPhy()
{
  NS_DEBUG ("SerialPhy::~SerialPhy ()");
}

void
SerialPhy::NotifyDataAvailable(void)
{
  NS_DEBUG ("SerialPhy::NotifyDataAvailable ()");

  Packet p;
  bool found = m_netdevice->GetQueue ()->Deque (p);
  if (found)
    {
#ifdef NOTYET
      struct NetDevicePacketDestAddress tag;
      p.PeekTag (tag);
      // send packet to address tag.address
#endif
      NS_DEBUG ("SerialPhy::NotifyDataAvailable (): Dequeued");
      m_netdevice->GetChannel()->Send(p, m_netdevice);
    }
}

void
SerialPhy::Receive (Packet& p)
{
  NS_DEBUG ("SerialPhy::Receive (" << &p << ")");

  m_netdevice->ForwardUp (p);
}

} // namespace ns3
