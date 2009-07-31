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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */

#ifndef IE_VECTOR_H
#define IE_VECTOR_H

#include "ns3/wifi-information-element.h"

namespace ns3 {
class Packet;
/**
 * \ingroup mesh
 *
 * \brief Information element vector
 *
 * Implements a vector of WifiInformationElement's
 */
class WifiInformationElementVector
{
public:
  WifiInformationElementVector ();
  ~WifiInformationElementVector ();
  void SetMaxSize (uint16_t size);
  typedef std::vector<Ptr<WifiInformationElement> >::iterator Iterator;
  Iterator Begin ();
  Iterator End ();
  bool AddInformationElement (Ptr<WifiInformationElement> element);
  Ptr<WifiInformationElement> FindFirst (enum WifiElementId id) const;
  static WifiInformationElementVector DeserializePacket (Ptr<Packet> packet);
  Ptr<Packet> MakePacket (bool sortByElementId = true);
private:
  uint32_t GetSize () const;
  /**
   * \brief Implements an empty information element: just elementId and length.
   * \details Needed to understand, which information element we need to deserialize now
   */
  class EmptyIe : public Header
  {
  public:
    EmptyIe ();
    virtual ~EmptyIe ();
    static TypeId GetTypeId ();
    TypeId GetInstanceTypeId () const;
    uint8_t GetLength ();
    uint8_t GetElementId ();
    virtual uint32_t GetSerializedSize () const;
    virtual void Serialize (Buffer::Iterator start) const;
    virtual uint32_t Deserialize (Buffer::Iterator start);
    virtual void Print (std::ostream &os) const;
  private:
    uint8_t m_elementId;
    uint8_t m_length;
  };
  typedef std::vector<Ptr<WifiInformationElement> > IE_VECTOR;
  IE_VECTOR m_elements;
  /// Size in bytes (actually, max packet length)
  uint16_t m_maxSize;
};
}
#endif
