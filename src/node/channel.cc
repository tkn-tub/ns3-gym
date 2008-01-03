/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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

#include "ns3/log.h"
#include "channel.h"
#include "net-device.h"

NS_LOG_COMPONENT_DEFINE ("Channel");

namespace ns3 {

InterfaceId 
Channel::iid (void)
{
  static InterfaceId iid = InterfaceId ("Channel")
    .SetParent<Object> ();
  return iid;
}

Channel::Channel ()
  : m_name("Channel")
{
  NS_LOG_FUNCTION;
}

Channel::Channel (std::string name)
  : m_name(name)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAMS (this << name);
}

Channel::~Channel ()
{
  NS_LOG_FUNCTION;
}

  void
Channel::SetName(std::string name)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAMS (this << name);
  m_name = name;
}

  std::string
Channel::GetName(void)
{
  NS_LOG_FUNCTION;
  return m_name;
}

  Ptr<NetDevice>
Channel::GetDevice (uint32_t i) const
{
  return DoGetDevice (i);
}

} // namespace ns3
