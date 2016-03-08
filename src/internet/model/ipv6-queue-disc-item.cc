/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 Universita' degli Studi di Napoli Federico II
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
#include "ipv6-queue-disc-item.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Ipv6QueueDiscItem");

Ipv6QueueDiscItem::Ipv6QueueDiscItem (Ptr<Packet> p, const Address& addr,
                                      uint16_t protocol, const Ipv6Header & header)
  : QueueDiscItem (p, addr, protocol),
    m_header (header),
    m_headerAdded (false)
{
}

Ipv6QueueDiscItem::~Ipv6QueueDiscItem()
{
  NS_LOG_FUNCTION (this);
}

uint32_t Ipv6QueueDiscItem::GetPacketSize(void) const
{
  Ptr<Packet> p = GetPacket ();
  NS_ASSERT (p != 0);
  uint32_t ret = p->GetSize ();
  if (!m_headerAdded)
    {
      ret += m_header.GetSerializedSize ();
    }
  return ret;
}

const Ipv6Header&
Ipv6QueueDiscItem::GetHeader (void) const
{
  return m_header;
}

void Ipv6QueueDiscItem::AddHeader(void)
{
  NS_LOG_FUNCTION (this);

  NS_ASSERT_MSG (!m_headerAdded, "The header has been already added to the packet");
  Ptr<Packet> p = GetPacket ();
  NS_ASSERT (p != 0);
  p->AddHeader (m_header);
  m_headerAdded = true;
}

void
Ipv6QueueDiscItem::Print (std::ostream& os) const
{
  if (!m_headerAdded)
    {
      os << m_header << " ";
    }
  os << GetPacket () << " "
     << "Dst addr " << GetAddress () << " "
     << "proto " << (uint16_t) GetProtocol () << " "
     << "txq " << (uint8_t) GetTxQueueIndex ()
  ;
}

} // namespace ns3
