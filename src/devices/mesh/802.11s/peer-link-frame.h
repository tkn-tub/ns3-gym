/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 */

#ifndef PEER_LINK_FRAME_START_H
#define PEER_LINK_FRAME_START_H
#include "ns3/header.h"
#include "ns3/supported-rates.h"
#include "ns3/ssid.h"
namespace ns3 {
class MeshWifiInterfaceMac;
class PeerLinkFrameStart : public Header
{
public:
  enum PeerLinkSubtype
  {
    PEER_LINK_OPEN = 0,
    PEER_LINK_CONFIRM,
    PEER_LINK_CLOSE
  };
  PeerLinkFrameStart ();
  struct PlinkFrameStartFields
  {
    PeerLinkSubtype subtype;
    uint16_t aid;
    SupportedRates rates;
    Ssid meshId;
  };
  void SetPlinkFrameStart(PlinkFrameStartFields);
  PlinkFrameStartFields GetPlinkFrameStart ();
  bool CheckPlinkFrameStart(Ptr<MeshWifiInterfaceMac> mac);
  /** Inherited from header:
   * \{
   */
  static  TypeId   GetTypeId ();
  virtual TypeId   GetInstanceTypeId () const;
  virtual void     Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize () const;
  virtual void     Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  /**
   * \}
   */
private:
  PlinkFrameStartFields m_fields;
};
} //namespace ns3
#endif
