/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 *          Aleksey Kovalenko <kovalenko@iitp.ru>
 */


#include "ns3/dot11s-peer-management-element.h"
#include "ns3/assert.h"


//NS_LOG_COMPONENT_DEFINE ("MeshPeerLinkManagementelement");

namespace ns3 {

PeerLinkManagementElement::PeerLinkManagementElement ():
    m_length(0),
    m_subtype(PEER_OPEN),
    m_localLinkId(0),
    m_peerLinkId(0),
    m_reasonCode(REASON11S_RESERVED)
{}


void
PeerLinkManagementElement::SetPeerOpen(uint16_t localLinkId)
{
  m_length = 3;
  m_subtype = PEER_OPEN;
  m_localLinkId = localLinkId;
}
void
PeerLinkManagementElement::SetPeerClose(uint16_t localLinkId, uint16_t peerLinkId, dot11sReasonCode reasonCode)
{
  m_length = 7;
  m_subtype = PEER_CLOSE;
  m_localLinkId = localLinkId;
  m_peerLinkId = peerLinkId;
  m_reasonCode = reasonCode;
}

void
PeerLinkManagementElement::SetPeerConfirm(uint16_t localLinkId, uint16_t peerLinkId)
{
  m_length = 5;
  m_subtype = PEER_CONFIRM;
  m_localLinkId = localLinkId;
  m_peerLinkId = peerLinkId;
}

dot11sReasonCode
PeerLinkManagementElement::GetReasonCode() const
  {
    return m_reasonCode;
  }

uint16_t
PeerLinkManagementElement::GetLocalLinkId() const
  {
    return m_localLinkId;
  }

uint16_t
PeerLinkManagementElement::GetPeerLinkId() const
  {
    return m_peerLinkId;
  }

uint32_t
PeerLinkManagementElement::GetSerializedSize (void) const
  {
    return m_length+2;
  }

bool
PeerLinkManagementElement::SubtypeIsOpen() const
  {
    return (m_subtype == PEER_OPEN);
  }
bool
PeerLinkManagementElement::SubtypeIsClose() const
  {
    return (m_subtype == PEER_CLOSE);
  }
bool
PeerLinkManagementElement::SubtypeIsConfirm() const
  {
    return (m_subtype == PEER_CONFIRM);
  }

Buffer::Iterator
PeerLinkManagementElement::Serialize (Buffer::Iterator i) const
  {
    i.WriteU8(ElementId());
    i.WriteU8(m_length);
    i.WriteU8(m_subtype);
    i.WriteHtonU16(m_localLinkId);
    if (m_length > 3)
      i.WriteHtonU16(m_peerLinkId);
    if (m_length > 5)
      i.WriteHtonU16(m_reasonCode);
    return i;
  }
Buffer::Iterator
PeerLinkManagementElement::Deserialize (Buffer::Iterator i)
{
  NS_ASSERT(ElementId() == i.ReadU8());
  m_length   = i.ReadU8();
  m_subtype  = i.ReadU8();
  m_localLinkId  = i.ReadNtohU16();
  if (m_length > 3)
    m_peerLinkId = i.ReadNtohU16();
  if (m_length > 5)
    m_reasonCode = (dot11sReasonCode)i.ReadNtohU16();
  return i;
}
} //namespace NS3
