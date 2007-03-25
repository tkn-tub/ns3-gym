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

#include <iostream>
#include <cassert>
#include "ns3/debug.h"
#include "ns3/queue.h"
#include "ns3/composite-trace-resolver.h"
#include "p2p-net-device.h"
#include "p2p-channel.h"
#include "p2p-phy.h"

NS_DEBUG_COMPONENT_DEFINE ("SerialNetDevice");

namespace ns3 {


SerialNetDevice::SerialNetDevice(Node* node) : 
  NetDevice(node, MacAddress("00:00:00:00:00:00"))
{
  NS_DEBUG ("SerialNetDevice::SerialNetDevice (" << node << ")");

  // BUGBUG FIXME
  //
  // You _must_ support broadcast to get any sort of packet from the ARP layer.
  EnableBroadcast (MacAddress ("ff:ff:ff:ff:ff:ff"));
  EnableMulticast();
  EnablePointToPoint();
  SetMtu(512); // bytes

  m_phy = new SerialPhy(node, this);
}

SerialNetDevice::~SerialNetDevice()
{
  NS_DEBUG ("SerialNetDevice::~SerialNetDevice ()");
  delete m_phy;
}


bool
SerialNetDevice::SendTo (Packet& p, const MacAddress& dest)
{
  NS_DEBUG ("SerialNetDevice::SendTo (" << &p << ", " << &dest << ")");

  assert (IsLinkUp ());

#ifdef NOTYET
    struct NetDevicePacketDestAddress tag;
    tag.address = address;
    p.AddTag (tag);
#endif
    if (m_queue->Enqueue(p) )
      {
        NotifyDataAvailable ();
        return true;
      }
    return false;
}

TraceResolver *
SerialNetDevice::DoCreateTraceResolver (TraceContext const &context)
{
  CompositeTraceResolver *resolver = new CompositeTraceResolver (context);
  resolver->Add ("queue", 
                 MakeCallback (&Queue::CreateTraceResolver, m_queue),
                 SerialNetDevice::QUEUE);
  resolver->Add ("rx",
                 m_rxTrace,
                 SerialNetDevice::RX);
  return resolver;
}

bool
SerialNetDevice::Attach (SerialChannel* ch)
{
  NS_DEBUG ("SerialNetDevice::Attach (" << &ch << ")");

  m_channel = ch;
  m_phy->Attach (m_channel);
  /* 
   * For now, this device is up whenever a channel is attached to it.
   * In fact, it should become up only when the second device
   * is attached to the channel. So, there should be a way for
   * a SerialChannel to notify both of its attached devices
   * that the channel is 'complete', hence that the devices are
   * up, hence that they can call NotifyLinkUp. 
   */
  NotifyLinkUp ();
  return true;
}

void
SerialNetDevice::AddQueue (Queue* q)
{
  NS_DEBUG ("SerialNetDevice::AddQueue (" << q << ")");

  m_queue = q;
}

void
SerialNetDevice::Receive (Packet& p)
{
  // ignore return value for now.
  NS_DEBUG ("SerialNetDevice::Receive (" << &p << ")");

  m_rxTrace (p);
  ForwardUp (p);
}

void
SerialNetDevice::NotifyDataAvailable(void)
{
  NS_DEBUG ("SerialNetDevice::NotifyDataAvailable ()");

  Packet p;
  bool found = GetQueue ()->Dequeue (p);
  if (found)
    {
#ifdef NOTYET
      struct NetDevicePacketDestAddress tag;
      p.PeekTag (tag);
      // send packet to address tag.address
#endif
      NS_DEBUG ("SerialNetDevice::NotifyDataAvailable (): Dequeued");
      m_phy->Send(p);
    }
}

Queue* 
SerialNetDevice::GetQueue(void) const 
{ 
    return m_queue;
}

SerialChannel* 
SerialNetDevice::GetChannel(void) const 
{ 
    return m_channel;
}

} // namespace ns3
