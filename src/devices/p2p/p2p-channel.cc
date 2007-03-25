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
#include "p2p-phy.h"
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
PointToPointChannel::Attach(PointToPointPhy *phy)
{
  NS_DEBUG("PointToPointChannel::Attach (" << phy << ")");
  NS_ASSERT(m_nDevices < N_DEVICES && "Only two devices permitted");
  NS_ASSERT(phy);

  m_link[m_nDevices].m_src = phy;
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

void
PointToPointChannel::TransmitCompleteEvent(Packet p, PointToPointPhy *src)
{
  NS_DEBUG("PointToPointChannel::TransmitCompleteEvent (" << &p << ", " << 
    src << ")");

  NS_ASSERT(m_link[0].m_state != INITIALIZING);
  NS_ASSERT(m_link[1].m_state != INITIALIZING);
  uint32_t wire = src == m_link[0].m_src ? 0 : 1;
  NS_ASSERT(m_link[wire].m_state == TRANSMITTING);

  m_link[wire].m_state = IDLE;
  
  NS_DEBUG("PointToPointChannel::TransmitCompleteEvent (): Receive()");

  m_link[wire].m_dst->Receive (p);
}

bool
PointToPointChannel::Propagate(Packet& p, PointToPointPhy* src)
{
  NS_DEBUG("PointToPointChannel::DoPropagate (" << &p << ", " << src << ")");

  NS_ASSERT(m_link[0].m_state != INITIALIZING);
  NS_ASSERT(m_link[1].m_state != INITIALIZING);

  uint32_t wire = src == m_link[0].m_src ? 0 : 1;

  if (m_link[wire].m_state == TRANSMITTING)
    {
      NS_DEBUG("PointToPointChannel::DoPropagate (): TRANSMITTING, return");
      return false;
    }

  m_link[wire].m_state = TRANSMITTING;

  Time txTime = Seconds (m_bps.CalculateTxTime(p.GetSize()));
  Time tEvent = txTime + m_delay;

  NS_DEBUG("PointToPointChannel::DoSend (): Schedule bps " << 
    m_bps.GetBitRate() << " txTime " << m_bps.CalculateTxTime(p.GetSize()) << 
    " prop delay " << m_delay << " Recv. delay " << tEvent);

  Simulator::Schedule (tEvent, &PointToPointChannel::TransmitCompleteEvent, 
                       this, p, src);
  return true;
}

uint32_t 
PointToPointChannel::GetNDevices (void) const
{
  return m_nDevices;
}
NetDevice *
PointToPointChannel::GetDevice (uint32_t i) const
{
  return m_link[i].m_src->GetDevice ();
}


} // namespace ns3
