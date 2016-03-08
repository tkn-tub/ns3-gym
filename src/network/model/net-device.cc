/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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

#include "ns3/object.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "net-device.h"
#include "packet.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("NetDevice");

QueueItem::QueueItem (Ptr<Packet> p)
{
  m_packet = p;
}

QueueItem::~QueueItem()
{
  NS_LOG_FUNCTION (this);
  m_packet = 0;
}

Ptr<Packet>
QueueItem::GetPacket (void) const
{
  return m_packet;
}

uint32_t
QueueItem::GetPacketSize (void) const
{
  NS_ASSERT (m_packet != 0);
  return m_packet->GetSize ();
}

void
QueueItem::Print (std::ostream& os) const
{
  os << GetPacket();
}

std::ostream & operator << (std::ostream &os, const QueueItem &item)
{
  item.Print (os);
  return os;
}


NS_OBJECT_ENSURE_REGISTERED (NetDevice);

TypeId NetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NetDevice")
    .SetParent<Object> ()
    .SetGroupName("Network")
  ;
  return tid;
}

NetDevice::~NetDevice ()
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3
