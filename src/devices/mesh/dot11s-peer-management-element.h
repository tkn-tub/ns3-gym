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


#ifndef MESH_PEER_MAN_ELEMENT
#define MESH_PEER_MAN_ELEMENT

#include <stdint.h>
#include "ns3/buffer.h"
#include "ns3/dot11s-codes.h"

namespace ns3 {
/**
 * \ingroup mesh
 */
class PeerLinkManagementElement
{
public:
  enum Subtype {
    PEER_OPEN    = 0,
    PEER_CLOSE   = 1,
    PEER_CONFIRM = 2,
  };
public:
  PeerLinkManagementElement ();

  void   SetPeerOpen(uint16_t localLinkId);
  void   SetPeerClose(uint16_t localLinkID, uint16_t peerLinkId, dot11sReasonCode reasonCode);
  void   SetPeerConfirm(uint16_t localLinkID, uint16_t peerLinkId);

  dot11sReasonCode GetReasonCode() const;
  uint16_t  GetLocalLinkId() const;
  uint16_t  GetPeerLinkId() const;
  bool   SubtypeIsOpen() const;
  bool   SubtypeIsClose() const;
  bool   SubtypeIsConfirm() const ;

  uint32_t  GetSerializedSize (void) const;
  Buffer::Iterator Serialize (Buffer::Iterator i) const;
  Buffer::Iterator Deserialize (Buffer::Iterator i);
private:
  static uint8_t ElementId() {
    return (uint8_t)IE11S_PEER_LINK_MANAGEMENT;
  }
  uint8_t   m_length;
  uint8_t   m_subtype;
  uint16_t  m_localLinkId; //always is present
  uint16_t  m_peerLinkId; //only in confirm and may be present in close frame
  dot11sReasonCode m_reasonCode; //only in close frame
};
} //namespace NS3
#endif
