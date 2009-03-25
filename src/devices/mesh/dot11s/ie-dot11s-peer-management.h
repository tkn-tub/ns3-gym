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

#include "ns3/dot11s-codes.h"
#include "ns3/wifi-information-element.h"

namespace ns3 {
namespace dot11s {
/**
 * \ingroup dot11s
 * \brief See 7.3.2.85 of draft 2.07
 */
class IePeerManagement : public WifiInformationElement
{
public:
    IePeerManagement ();
    enum Subtype {
      PEER_OPEN    = 0,
      PEER_CLOSE   = 1,
      PEER_CONFIRM = 2,
    };
  void   SetPeerOpen (uint16_t localLinkId);
  void   SetPeerClose (uint16_t localLinkID, uint16_t peerLinkId, dot11sReasonCode reasonCode);
  void   SetPeerConfirm (uint16_t localLinkID, uint16_t peerLinkId);

  dot11sReasonCode GetReasonCode () const;
  uint16_t  GetLocalLinkId () const;
  uint16_t  GetPeerLinkId () const;
  bool   SubtypeIsOpen () const;
  bool   SubtypeIsClose () const;
  bool   SubtypeIsConfirm () const ;
  uint8_t GetSubtype() const { return m_subtype;};
private:
  WifiElementId ElementId () const{
    return IE11S_PEER_LINK_MANAGEMENT;
  }
  uint8_t  GetInformationSize (void) const;
  void SerializeInformation (Buffer::Iterator i) const;
  uint8_t DeserializeInformation (Buffer::Iterator i, uint8_t length);
  void PrintInformation (std::ostream& os) const;
private:
  uint8_t   m_length;
  uint8_t   m_subtype;
  uint16_t  m_localLinkId;
  /**
   * Present within confirm and may be present in close
   */
  uint16_t  m_peerLinkId;
  /**
   * Present only within close frame
   */
  dot11sReasonCode m_reasonCode;
};

} // namespace dot11s
} //namespace ns3
#endif
