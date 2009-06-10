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
 */


#ifndef MESH_PERING_PROTOCOL_H
#define MESH_PEERING_PROTOCOL_H

#include "ns3/wifi-information-element.h"

namespace ns3 {
namespace dot11s {
class IePeeringProtocol : public WifiInformationElement
{
public:
  IePeeringProtocol ();
private:
  WifiElementId ElementId () const
  {
    return IE11S_MESH_PEERING_PROTOCOL_VERSION;
  }
  uint8_t  GetInformationSize () const;
  void SerializeInformation (Buffer::Iterator i) const;
  uint8_t DeserializeInformation (Buffer::Iterator i, uint8_t length);
  void PrintInformation (std::ostream& os) const;
private:
  uint8_t m_protocol;
};
} // namespace dot11s
} //namespace ns3
#endif
