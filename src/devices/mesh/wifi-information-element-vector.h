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
#include "ns3/header.h"

namespace ns3 {
class Packet;
/**
 * \ingroup mesh
 *
 * \brief Information element vector
 *
 * Implements a vector of WifiInformationElement's
 */
class WifiInformationElementVector : public Header
{
public:
  WifiInformationElementVector ();
  ~WifiInformationElementVector ();
  ///\name Inherited from Header
  //\{
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const;
  virtual uint32_t GetSerializedSize () const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;
  //\}
  /**
   * \brief Needed when you try to deserialize a lonely IE inside other header
   * \param start is the start of the buffer
   * \return deserialized bytes
   */
  virtual uint32_t DeserializeSingleIe (Buffer::Iterator start);
  void SetMaxSize (uint16_t size);
  typedef std::vector<Ptr<WifiInformationElement> >::iterator Iterator;
  Iterator Begin ();
  Iterator End ();
  bool AddInformationElement (Ptr<WifiInformationElement> element);
  Ptr<WifiInformationElement> FindFirst (enum WifiElementId id) const;
private:
  uint32_t GetSize () const;
  typedef std::vector<Ptr<WifiInformationElement> > IE_VECTOR;
  IE_VECTOR m_elements;
  /// Size in bytes (actually, max packet length)
  uint16_t m_maxSize;
  friend bool operator== (const WifiInformationElementVector & a, const WifiInformationElementVector & b);
};
bool operator== (const WifiInformationElementVector & a, const WifiInformationElementVector & b);
}
#endif
