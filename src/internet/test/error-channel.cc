/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Universita' di Firenze, Italy
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
 * Author: Tommaso Pecorella <tommaso.pecorella@unifi.it>
 */
#include "error-channel.h"
#include "ns3/simple-net-device.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ErrorChannel");

NS_OBJECT_ENSURE_REGISTERED (ErrorChannel);

TypeId 
ErrorChannel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ErrorChannel")
    .SetParent<Channel> ()
    .AddConstructor<ErrorChannel> ()
    ;
  return tid;
}

ErrorChannel::ErrorChannel ()
{
  jumping = false;
  jumpingState = 0;
}

void
ErrorChannel::SetJumpingTime(Time delay)
{
  jumpingTime = delay;
}

void
ErrorChannel::SetJumpingMode(bool mode)
{
  jumping = mode;
  jumpingState = 0;
}

void
ErrorChannel::Send (Ptr<Packet> p, uint16_t protocol,
                    Mac48Address to, Mac48Address from,
                    Ptr<SimpleNetDevice> sender)
{
  NS_LOG_FUNCTION (p << protocol << to << from << sender);
  for (std::vector<Ptr<SimpleNetDevice> >::const_iterator i = m_devices.begin (); i != m_devices.end (); ++i)
    {
      Ptr<SimpleNetDevice> tmp = *i;
      if (tmp == sender)
        {
          continue;
        }
      if( !jumping || jumpingState%2 )
        {
          Simulator::ScheduleWithContext (tmp->GetNode ()->GetId (), Seconds (0),
                                      &SimpleNetDevice::Receive, tmp, p->Copy (), protocol, to, from);
          jumpingState++;
        }
      else
        {
          Simulator::ScheduleWithContext (tmp->GetNode ()->GetId (), jumpingTime,
                                      &SimpleNetDevice::Receive, tmp, p->Copy (), protocol, to, from);
          jumpingState++;
        }
    }
}

void 
ErrorChannel::Add (Ptr<SimpleNetDevice> device)
{
  m_devices.push_back (device);
}

uint32_t 
ErrorChannel::GetNDevices (void) const
{
  return m_devices.size ();
}
Ptr<NetDevice> 
ErrorChannel::GetDevice (uint32_t i) const
{
  return m_devices[i];
}

NS_OBJECT_ENSURE_REGISTERED (BinaryErrorModel);

TypeId BinaryErrorModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BinaryErrorModel")
    .SetParent<ErrorModel> ()
    .AddConstructor<BinaryErrorModel> ()
    ;
  return tid;
}

BinaryErrorModel::BinaryErrorModel ()
{
  counter = 0;
}

BinaryErrorModel::~BinaryErrorModel ()
{
}


bool
BinaryErrorModel::DoCorrupt (Ptr<Packet> p)
{
  if (!IsEnabled ())
    {
      return false;
    }
  bool ret = counter%2;
  counter++;
  return ret;
}

void
BinaryErrorModel::Reset (void)
{
  DoReset();
}

void
BinaryErrorModel::DoReset (void)
{
  counter = 0;
}


} // namespace ns3
