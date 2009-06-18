/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */

#include "flame-protocol-mac.h"
#include "flame-protocol.h"
#include "flame-header.h"
#include "ns3/log.h"
namespace ns3 {
namespace flame {
NS_LOG_COMPONENT_DEFINE ("FlameProtocolMac");
FlameProtocolMac::FlameProtocolMac (uint32_t ifIndex, Ptr<FlameProtocol> protocol):
  m_protocol (protocol),
  m_ifIndex (ifIndex)
{
}
FlameProtocolMac::~FlameProtocolMac ()
{
}
void
FlameProtocolMac::SetParent (Ptr<MeshWifiInterfaceMac> parent)
{
  m_parent = parent;
}

bool
FlameProtocolMac::Receive (Ptr<Packet> packet, const WifiMacHeader & header)
{
  if (!header.IsData ())
    return true;
  FlameTag tag;
  if(packet->PeekPacketTag (tag))
  {
    NS_FATAL_ERROR ("FLAME tag is not supposed to be received by network");
  }
  tag.address = header.GetAddr2 ();
  packet->AddPacketTag (tag);
  return true;
}
bool
FlameProtocolMac::UpdateOutcomingFrame (Ptr<Packet> packet, WifiMacHeader & header, Mac48Address from, Mac48Address to)
{
  if(!header.IsData ())
    return true;
  FlameTag tag;
  if(!packet->RemovePacketTag (tag))
  {
    NS_FATAL_ERROR ("FLAME tag must exist here");
  }
  header.SetAddr1 (tag.address);
  return true;
}
uint8_t
FlameProtocolMac::GetCost(Mac48Address peerAddress) const
{
  uint32_t metric = m_parent->GetLinkMetric(peerAddress);
  return (metric > 255 ? 255 : (uint8_t)(metric & 0xff));
}
uint16_t
FlameProtocolMac::GetChannelId () const
{
  return m_parent->GetFrequencyChannel ();
}
void
FlameProtocolMac::Report (std::ostream & os) const
{
}
void
FlameProtocolMac::ResetStats ()
{
}

} //namespace flame
} //namespace ns3
