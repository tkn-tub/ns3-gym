/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005, 2009 INRIA
 * Copyright (c) 2009 MIRKO BANCHI
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Mirko Banchi <mk.banchi@gmail.com>
 *          Stefano Avallone <stavallo@unina.it>
 */

#include "ns3/simulator.h"
#include "wifi-mac-queue-item.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("WifiMacQueueItem");

WifiMacQueueItem::WifiMacQueueItem (Ptr<const Packet> p, const WifiMacHeader & header)
  : m_packet (p),
    m_header (header),
    m_tstamp (Simulator::Now ())
{
}

WifiMacQueueItem::~WifiMacQueueItem ()
{
}

Ptr<const Packet>
WifiMacQueueItem::GetPacket (void) const
{
  return m_packet;
}

const WifiMacHeader&
WifiMacQueueItem::GetHeader (void) const
{
  return m_header;
}

Mac48Address
WifiMacQueueItem::GetDestinationAddress (void) const
{
  return m_header.GetAddr1 ();
}

Time
WifiMacQueueItem::GetTimeStamp (void) const
{
  return m_tstamp;
}

uint32_t
WifiMacQueueItem::GetSize (void) const
{
  return m_packet->GetSize () + m_header.GetSerializedSize ();
}

NS_OBJECT_TEMPLATE_CLASS_DEFINE (Queue, WifiMacQueueItem);

} //namespace ns3
