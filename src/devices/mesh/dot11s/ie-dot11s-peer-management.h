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

#include "ns3/wifi-information-element.h"

namespace ns3 {
namespace dot11s {

/**
 * \ingroup dot11s
 * \brief Codes used by 802.11s Peer Management Protocol 
 */
enum PmpReasonCode {
  REASON11S_PEER_LINK_CANCELLED,
  REASON11S_MESH_MAX_PEERS,
  REASON11S_MESH_CAPABILITY_POLICY_VIOLATION,
  REASON11S_MESH_CLOSE_RCVD,
  REASON11S_MESH_MAX_RETRIES,
  REASON11S_MESH_CONFIRM_TIMEOUT,
  REASON11S_MESH_SECURITY_ROLE_NEGOTIATION_DIFFERS,
  REASON11S_MESH_SECURITY_AUTHENTICATION_IMPOSSIBLE,
  REASON11S_MESH_SECURITY_FAILED_VERIFICATION,
  REASON11S_MESH_INVALID_GTK,
  REASON11S_MESH_MISMATCH_GTK,
  REASON11S_MESH_INCONSISTENT_PARAMETERS,
  REASON11S_MESH_CONFIGURATION_POLICY_VIOLATION,
  REASON11S_RESERVED,
};
  
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
  void   SetPeerClose (uint16_t localLinkID, uint16_t peerLinkId, PmpReasonCode reasonCode);
  void   SetPeerConfirm (uint16_t localLinkID, uint16_t peerLinkId);

  PmpReasonCode GetReasonCode () const;
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
  PmpReasonCode m_reasonCode;
  friend bool operator== (const IePeerManagement & a, const IePeerManagement & b);
};
bool operator== (const IePeerManagement & a, const IePeerManagement & b);
} // namespace dot11s
} //namespace ns3
#endif
