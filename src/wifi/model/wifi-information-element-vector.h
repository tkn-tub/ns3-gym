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
 *          Pavel Boyko <boyko.iitp.ru>
 */

#ifndef WIFI_INFORMATION_ELEMENT_VECTOR_H
#define WIFI_INFORMATION_ELEMENT_VECTOR_H

#include "ns3/header.h"
#include "ns3/simple-ref-count.h"

#include "ns3/wifi-information-element.h"


namespace ns3 {

/**
 * \brief Information element vector
 * \ingroup wifi
 *
 * Implements a vector of WifiInformationElements.
 * Information elements typically come in groups, and the
 * WifiInformationElementVector class provides a representation of a
 * series of IEs, and the facility for serialisation to and
 * deserialisation from the over-the-air format.
 */
class WifiInformationElementVector : public Header
{
public:
  WifiInformationElementVector ();
  ~WifiInformationElementVector ();
  ///\name Inherited from Header
  // \{
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const;
  virtual uint32_t GetSerializedSize () const;
  virtual void Serialize (Buffer::Iterator start) const;
  /**
   * \attention When you use RemoveHeader, WifiInformationElementVector supposes, that
   * all buffer consists of information elements
   * @param start
   * @return
   */
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;
  //\}
  /**
   * \brief Needed when you try to deserialize a lonely IE inside other header
   * \param start is the start of the buffer
   * \return deserialized bytes
   */
  virtual uint32_t DeserializeSingleIe (Buffer::Iterator start);
  ///Set maximum size to control overflow of the max packet length
  void SetMaxSize (uint16_t size);
  /// As soon as this is a vector, we define an Iterator
  typedef std::vector<Ptr<WifiInformationElement> >::iterator Iterator;
  /// Returns Begin of the vector
  Iterator Begin ();
  /// Returns End of the vector
  Iterator End ();
  /// add an IE, if maxSize has exceeded, returns false
  bool AddInformationElement (Ptr<WifiInformationElement> element);
  /// vector of pointers to information elements is the body of IeVector
  Ptr<WifiInformationElement> FindFirst (WifiInformationElementId id) const;

  virtual bool operator== (const WifiInformationElementVector & a) const;
protected:
  typedef std::vector<Ptr<WifiInformationElement> > IE_VECTOR;
  /// Current number of bytes
  uint32_t GetSize () const;
  IE_VECTOR m_elements;
  /// Size in bytes (actually, max packet length)
  uint16_t m_maxSize;
};

}
#endif /* WIFI_INFORMATION_ELEMENT_VECTOR_H */
