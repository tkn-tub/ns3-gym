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
#include "simple-channel.h"
#include "simple-net-device.h"
#include "ns3/packet.h"

namespace ns3 {

TypeId 
SimpleChannel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SimpleChannel")
    .SetParent<Channel> ()
    .AddConstructor<SimpleChannel> ()
    ;
  return tid;
}

SimpleChannel::SimpleChannel ()
{}

void
SimpleChannel::Send (Ptr<Packet> p, uint16_t protocol, 
		     Mac48Address to, Mac48Address from,
		     Ptr<SimpleNetDevice> sender)
{
  for (std::vector<Ptr<SimpleNetDevice> >::const_iterator i = m_devices.begin (); i != m_devices.end (); ++i)
    {
      Ptr<SimpleNetDevice> tmp = *i;
      if (tmp == sender)
	{
	  continue;
	}
      tmp->Receive (p->Copy (), protocol, to, from);
    }
}

void 
SimpleChannel::Add (Ptr<SimpleNetDevice> device)
{
  m_devices.push_back (device);
}

uint32_t 
SimpleChannel::GetNDevices (void) const
{
  return m_devices.size ();
}
Ptr<NetDevice> 
SimpleChannel::GetDevice (uint32_t i) const
{
  return m_devices[i];
}

} // namespace ns3
