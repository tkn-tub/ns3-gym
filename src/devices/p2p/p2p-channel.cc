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

#include "p2p-channel.h"
#include "p2p-net-device.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/debug.h"

NS_DEBUG_COMPONENT_DEFINE ("PointToPointChannel");

namespace ns3 {

//
// By default, you get a channel with the name "PointToPoint Channel" that 
// has an "infitely" fast transmission speed and zero delay.
PointToPointChannel::PointToPointChannel()
: 
  Channel ("PointToPoint Channel"), 
  m_bps (DataRate(0xffffffff)),
  m_delay (Seconds(0)),
  m_nDevices(0)
{
  NS_DEBUG("PointToPointChannel::PointToPointChannel ()");
}

PointToPointChannel::PointToPointChannel(
  const DataRate& bps, 
  const Time& delay)
: 
  Channel ("PointToPoint Channel"), 
  m_bps (bps),
  m_delay (delay),
  m_nDevices(0)
{
  NS_DEBUG("PointToPointChannel::PointToPointChannel (" << Channel::GetName() 
    << ", " << bps.GetBitRate() << ", " << delay << ")");
}

PointToPointChannel::PointToPointChannel(
  const std::string& name,
  const DataRate& bps, 
  const Time& delay)
: 
  Channel (name),
  m_bps (bps), 
  m_delay (delay),
  m_nDevices(0)
{
  NS_DEBUG("PointToPointChannel::PointToPointChannel (" << name << ", " << 
    bps.GetBitRate() << ", " << delay << ")");
}

  void
PointToPointChannel::Attach(Ptr<PointToPointNetDevice> device)
{
  NS_DEBUG("PointToPointChannel::Attach (" << device << ")");
  NS_ASSERT(m_nDevices < N_DEVICES && "Only two devices permitted");
  NS_ASSERT(device != 0);

  m_link[m_nDevices].m_src = device;
  ++m_nDevices;
//
// If we have both devices connected to the channel, then finish introducing
// the two halves and set the links to IDLE.
//
  if (m_nDevices == N_DEVICES)
    {
      m_link[0].m_dst = m_link[1].m_src;
      m_link[1].m_dst = m_link[0].m_src;
      m_link[0].m_state = IDLE;
      m_link[1].m_state = IDLE;
    }
}

bool
PointToPointChannel::TransmitStart(Packet& p, Ptr<PointToPointNetDevice> src)
{
  NS_DEBUG ("PointToPointChannel::TransmitStart (" << &p << ", " << src << 
            ")");
  NS_DEBUG ("PointToPointChannel::TransmitStart (): UID is " << 
            p.GetUid () << ")");

  NS_ASSERT(m_link[0].m_state != INITIALIZING);
  NS_ASSERT(m_link[1].m_state != INITIALIZING);

  uint32_t wire = src == m_link[0].m_src ? 0 : 1;

  if (m_link[wire].m_state == TRANSMITTING)
    {
      NS_DEBUG("PointToPointChannel::TransmitStart (): **** ERROR ****");
      NS_DEBUG("PointToPointChannel::TransmitStart (): state TRANSMITTING");
      return false;
    }

  NS_DEBUG("PointToPointChannel::TransmitStart (): switch to TRANSMITTING");
  m_link[wire].m_state = TRANSMITTING;
  return true;
}

bool
PointToPointChannel::TransmitEnd(Packet& p, Ptr<PointToPointNetDevice> src)
{
  NS_DEBUG("PointToPointChannel::TransmitEnd (" << &p << ", " << src << ")");
  NS_DEBUG ("PointToPointChannel::TransmitEnd (): UID is " << 
            p.GetUid () << ")");

  NS_ASSERT(m_link[0].m_state != INITIALIZING);
  NS_ASSERT(m_link[1].m_state != INITIALIZING);

  uint32_t wire = src == m_link[0].m_src ? 0 : 1;

  NS_ASSERT(m_link[wire].m_state == TRANSMITTING);

  m_link[wire].m_state = PROPAGATING;
//
// The sender is going to free the packet as soon as it has been transmitted.
// We need to copy it to get a reference so it won't e deleted.
//
// Above is not correct (GFR).  The event scheduler makes a copy
// of the packet
//  Packet packet = p;
  NS_DEBUG ("PointToPointChannel::TransmitEnd (): Schedule event in " << 
            m_delay.GetSeconds () << "sec");
  Simulator::Schedule (m_delay,
                       &PointToPointChannel::PropagationCompleteEvent,
                       this, p, src);
  return true;
}

void
PointToPointChannel::PropagationCompleteEvent(
  Packet p, 
  Ptr<PointToPointNetDevice> src)
{
  NS_DEBUG("PointToPointChannel::PropagationCompleteEvent (" << &p << ", " << 
    src << ")");
  NS_DEBUG ("PointToPointChannel::PropagationCompleteEvent (): UID is " << 
    p.GetUid () << ")");

  uint32_t wire = src == m_link[0].m_src ? 0 : 1;
  NS_ASSERT(m_link[wire].m_state == PROPAGATING);
  m_link[wire].m_state = IDLE;

  NS_DEBUG ("PointToPointChannel::PropagationCompleteEvent (): Receive");
  m_link[wire].m_dst->Receive (p);
}


uint32_t 
PointToPointChannel::GetNDevices (void) const
{
  return m_nDevices;
}

Ptr<NetDevice>
PointToPointChannel::GetDevice (uint32_t i) const
{
  NS_ASSERT(i < 2);
  return m_link[i].m_src;
}

  DataRate
PointToPointChannel::GetDataRate (void)
{
  return m_bps;
}

  Time
PointToPointChannel::GetDelay (void)
{
  return m_delay;
}


} // namespace ns3
