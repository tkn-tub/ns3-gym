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

#include "serial-channel.h"
#include "serial-net-device.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/debug.h"

NS_DEBUG_COMPONENT_DEFINE ("SerialChannel");

namespace ns3 {

//
// By default, you get a channel with the name "Serial Channel" that has an
// "infitely" fast transmission speed and zero delay.
//
SerialChannel::SerialChannel()
: 
  Channel ("Serial Channel"), 
  m_bps (UINT64_MAX), 
  m_delay (Seconds(0)),
  m_nDevices(0)
{
  NS_DEBUG("SerialChannel::SerialChannel ()");
}

SerialChannel::SerialChannel(
  std::string name, 
  DataRate bps, 
  Time delay)
: 
  Channel (name), 
  m_bps (bps), 
  m_delay (delay),
  m_nDevices(0)
{
  NS_DEBUG("SerialChannel::SerialChannel (" << name << ", " << bps << ", " << 
    delay << ")");
}

  void
SerialChannel::Attach(SerialNetDevice *device)
{
  NS_DEBUG("SerialChannel::Attach (" << device << ")");
  NS_ASSERT(m_nDevices < N_DEVICES && "Only two devices permitted");
  NS_ASSERT(device);

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

  void
SerialChannel::TransmitCompleteEvent(Packet &p, SerialNetDevice *src)
{
  NS_DEBUG("SerialChannel::TransmitCompleteEvent (" << &p << ", " << 
    src << ")");

  NS_ASSERT(m_link[0].m_state != INITIALIZING);
  NS_ASSERT(m_link[1].m_state != INITIALIZING);
  uint32_t wire = src == m_link[0].m_src ? 0 : 1;
  NS_ASSERT(m_link[wire].m_state == TRANSMITTING);

  m_link[wire].m_state = IDLE;
  
  NS_DEBUG("SerialChannel::TransmitCompleteEvent (): Receive()");

  m_link[wire].m_dst->Receive (p);
}

  bool
SerialChannel::Propagate(Packet& p, SerialNetDevice* src)
{
  NS_DEBUG("SerialChannel::DoPropagate (" << &p << ", " << src << ")");

  NS_ASSERT(m_link[0].m_state != INITIALIZING);
  NS_ASSERT(m_link[1].m_state != INITIALIZING);

  uint32_t wire = src == m_link[0].m_src ? 0 : 1;

  if (m_link[wire].m_state == TRANSMITTING)
    {
      NS_DEBUG("SerialChannel::DoPropagate (): TRANSMITTING, return");
      return false;
    }

  m_link[wire].m_state = TRANSMITTING;

//
// I believe Raj has a method in the DataRate class to do this.  Should use
// it when it is available.
//
  Time tEvent = Simulator::Now () + 
    Seconds (static_cast<double> (p.GetSize() * 8) / 
    static_cast<double> (m_bps)) + m_delay;

  NS_DEBUG("SerialChannel::DoSend (): Schedule Receive at " << tEvent);

#if 0
  Simulator::Schedule (tEvent, &SerialChannel::TransmitCompleteEvent, this, 
    p, src);
#else
  TransmitCompleteEvent (p, src);
#endif
  return true;
}

} // namespace ns3
