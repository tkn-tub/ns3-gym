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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */

#ifndef MESH_ID_H
#define MESH_ID_H

#include <stdint.h>
#include "ns3/buffer.h"
#include "ns3/wifi-information-element.h"

namespace ns3 {
namespace dot11s {
/**
 * \brief a IEEE 802.11s Mesh ID 7.3.287 of 802.11s draft 3.0
 *
 */
class IeMeshId : public WifiInformationElement
{
public:
  // broadcast meshId
  IeMeshId ();
  IeMeshId (std::string s);

  bool IsEqual (IeMeshId const &o) const;
  bool IsBroadcast (void) const;

  uint32_t GetLength (void) const;
  char *PeekString (void) const;
private:
  WifiElementId ElementId () const{
    return IE11S_MESH_ID;
  };
  void SerializeInformation (Buffer::Iterator i) const;
  uint8_t DeserializeInformation (Buffer::Iterator start, uint8_t length);
  void PrintInformation (std::ostream& os) const;
  uint8_t GetInformationSize () const;
private:
  uint8_t m_meshId[33];
  friend bool operator== (const IeMeshId & a, const IeMeshId & b);
};

std::ostream &operator << (std::ostream &os, const IeMeshId &meshId);

/**
 * \class ns3::IeMeshIdValue
 * \brief hold objects of type ns3::IeMeshId
 */

ATTRIBUTE_HELPER_HEADER (IeMeshId);
} //namespace dot11s
} // namespace ns3
#endif /* MESH_ID_H */
