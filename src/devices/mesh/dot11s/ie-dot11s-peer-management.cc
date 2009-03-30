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


#include "ie-dot11s-peer-management.h"
#include "ns3/assert.h"


//NS_LOG_COMPONENT_DEFINE ("MeshPeerLinkManagementelement");

namespace ns3 {
namespace dot11s {

IePeerManagement::IePeerManagement ():
    m_length (0),
    m_subtype (PEER_OPEN),
    m_localLinkId (0),
    m_peerLinkId (0),
    m_reasonCode (REASON11S_RESERVED)
{}


void
IePeerManagement::SetPeerOpen (uint16_t localLinkId)
{
  m_length = 3;
  m_subtype = PEER_OPEN;
  m_localLinkId = localLinkId;
}
void
IePeerManagement::SetPeerClose (uint16_t localLinkId, uint16_t peerLinkId, PmpReasonCode reasonCode)
{
  m_length = 7;
  m_subtype = PEER_CLOSE;
  m_localLinkId = localLinkId;
  m_peerLinkId = peerLinkId;
  m_reasonCode = reasonCode;
}

void
IePeerManagement::SetPeerConfirm (uint16_t localLinkId, uint16_t peerLinkId)
{
  m_length = 5;
  m_subtype = PEER_CONFIRM;
  m_localLinkId = localLinkId;
  m_peerLinkId = peerLinkId;
}

PmpReasonCode
IePeerManagement::GetReasonCode () const
{
  return m_reasonCode;
}

uint16_t
IePeerManagement::GetLocalLinkId () const
{
  return m_localLinkId;
}

uint16_t
IePeerManagement::GetPeerLinkId () const
{
  return m_peerLinkId;
}

uint8_t
IePeerManagement::GetInformationSize (void) const
{
  return m_length;
}

bool
IePeerManagement::SubtypeIsOpen () const
{
  return (m_subtype == PEER_OPEN);
}
bool
IePeerManagement::SubtypeIsClose () const
{
  return (m_subtype == PEER_CLOSE);
}
bool
IePeerManagement::SubtypeIsConfirm () const
{
  return (m_subtype == PEER_CONFIRM);
}

void
IePeerManagement::SerializeInformation (Buffer::Iterator i) const
{
  i.WriteU8 (m_subtype);
  i.WriteHtonU16 (m_localLinkId);
  if (m_length > 3)
    i.WriteHtonU16 (m_peerLinkId);
  if (m_length > 5)
    i.WriteHtonU16 (m_reasonCode);
}
uint8_t
IePeerManagement::DeserializeInformation (Buffer::Iterator start, uint8_t length)
{
  Buffer::Iterator i = start;
  m_subtype  = i.ReadU8 ();
  m_length = length;
  if (m_subtype == PEER_OPEN)
    NS_ASSERT (length == 3);
  if (m_subtype == PEER_CONFIRM)
    NS_ASSERT (length == 5);
  if (m_subtype == PEER_CLOSE)
    NS_ASSERT (length == 7);
  m_localLinkId  = i.ReadNtohU16 ();
  if (m_length > 3)
    m_peerLinkId = i.ReadNtohU16 ();
  if (m_length > 5)
    m_reasonCode = (PmpReasonCode)i.ReadNtohU16 ();
  return i.GetDistanceFrom (start);
}
void
IePeerManagement::PrintInformation (std::ostream& os) const
{
  //TODO
}
  
} // namespace dot11s
} //namespace ns3

