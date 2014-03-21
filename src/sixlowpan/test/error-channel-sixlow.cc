/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Universita' di Firenze, Italy
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
#include "error-channel-sixlow.h"
#include "ns3/simple-net-device.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("ErrorChannelSixlow");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (ErrorChannelSixlow);

TypeId
ErrorChannelSixlow::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ErrorChannelSixlow")
    .SetParent<Channel> ()
    .AddConstructor<ErrorChannelSixlow> ()
  ;
  return tid;
}

ErrorChannelSixlow::ErrorChannelSixlow ()
{
  m_jumpingTime = Seconds (0.5);
  m_jumping = false;
  m_jumpingState = 0;
  m_duplicateTime = Seconds (0.1);
  m_duplicate = false;
}

void
ErrorChannelSixlow::SetJumpingTime (Time delay)
{
  m_jumpingTime = delay;
}

void
ErrorChannelSixlow::SetJumpingMode (bool mode)
{
  m_jumping = mode;
  m_jumpingState = 0;
}

void
ErrorChannelSixlow::SetDuplicateTime (Time delay)
{
  m_duplicateTime = delay;
}

void
ErrorChannelSixlow::SetDuplicateMode (bool mode)
{
  m_duplicate = mode;
  m_duplicateState = 0;
}


void
ErrorChannelSixlow::Send (Ptr<Packet> p, uint16_t protocol,
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
      if (m_jumping)
        {
          if (m_jumpingState % 2)
            {
              Simulator::ScheduleWithContext (tmp->GetNode ()->GetId (), Seconds (0),
                                              &SimpleNetDevice::Receive, tmp, p->Copy (), protocol, to, from);
            }
          else
            {
              Simulator::ScheduleWithContext (tmp->GetNode ()->GetId (), m_jumpingTime,
                                              &SimpleNetDevice::Receive, tmp, p->Copy (), protocol, to, from);
            }
          m_jumpingState++;
        }
      else if (m_duplicate)
        {
          if (m_duplicateState % 2)
            {
              Simulator::ScheduleWithContext (tmp->GetNode ()->GetId (), Seconds (0),
                                              &SimpleNetDevice::Receive, tmp, p->Copy (), protocol, to, from);
            }
          else
            {
              Simulator::ScheduleWithContext (tmp->GetNode ()->GetId (), Seconds (0),
                                              &SimpleNetDevice::Receive, tmp, p->Copy (), protocol, to, from);
              Simulator::ScheduleWithContext (tmp->GetNode ()->GetId (), m_duplicateTime,
                                              &SimpleNetDevice::Receive, tmp, p->Copy (), protocol, to, from);
            }
          m_duplicateState++;
        }
      else
        {
          Simulator::ScheduleWithContext (tmp->GetNode ()->GetId (), Seconds (0),
                                          &SimpleNetDevice::Receive, tmp, p->Copy (), protocol, to, from);
        }
    }
}

void
ErrorChannelSixlow::Add (Ptr<SimpleNetDevice> device)
{
  m_devices.push_back (device);
}

uint32_t
ErrorChannelSixlow::GetNDevices (void) const
{
  return m_devices.size ();
}
Ptr<NetDevice>
ErrorChannelSixlow::GetDevice (uint32_t i) const
{
  return m_devices[i];
}

NS_OBJECT_ENSURE_REGISTERED (BinaryErrorSixlowModel);

TypeId BinaryErrorSixlowModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BinaryErrorSixlowModel")
    .SetParent<ErrorModel> ()
    .AddConstructor<BinaryErrorSixlowModel> ()
  ;
  return tid;
}

BinaryErrorSixlowModel::BinaryErrorSixlowModel ()
{
  m_counter = 0;
}

BinaryErrorSixlowModel::~BinaryErrorSixlowModel ()
{
}


bool
BinaryErrorSixlowModel::DoCorrupt (Ptr<Packet> p)
{
  if (!IsEnabled ())
    {
      return false;
    }
  bool ret = m_counter % 2;
  m_counter++;
  return ret;
}

void
BinaryErrorSixlowModel::Reset (void)
{
  DoReset ();
}

void
BinaryErrorSixlowModel::DoReset (void)
{
  m_counter = 0;
}


} // namespace ns3
