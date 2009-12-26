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

#ifndef IE_VECTOR_H
#define IE_VECTOR_H

#include "ns3/header.h"
#include "ns3/simple-ref-count.h"

namespace ns3 {
class Packet;
/**
 * \ingroup mesh
 *
 * \brief Enum of all known information element id (aka tags).
 *
 * For now only 802.11s (mesh) related elements are supported here (so 11S prefix),
 * but this can change in future.
 *
 * Note that 802.11s element ids are not yet officially assigned, we use ones
 * compatible with open80211s (http://o11s.org/) implementation.
 */
enum WifiElementId {
  /* begin of open80211s-compatible IDs */
  IE11S_MESH_CONFIGURATION              = 51,
  IE11S_MESH_ID                         = 52,
  /* end of open80211s-compatible IDs */
  IE11S_LINK_METRIC_REPORT              = 20,
  IE11S_CONGESTION_NOTIFICATION,
  /* begin of open80211s-compatible IDs */
  IE11S_PEERING_MANAGEMENT              = 55,
  /* end of open80211s-compatible IDs */
  IE11S_SUPP_MBSS_REG_CLASSES_CHANNELS  = 23,
  IE11S_MESH_CHANNEL_SWITCH_ANNOUNCEMENT,
  IE11S_MESH_TIM,
  IE11S_AWAKE_WINDOW,
  IE11S_BEACON_TIMING,
  IE11S_MCCAOP_SETUP_REQUEST,
  IE11S_MCCAOP_SETUP_REPLY,
  IE11S_MCCAOP_ADVERTISEMENT,
  IE11S_MCCAOP_RESERVATION_TEARDOWN,
  IE11S_PORTAL_ANNOUNCEMENT,
  IE11S_RANN                            = 67,
  /* begin of open80211s-compatible IDs */
  IE11S_PREQ                            = 68,
  IE11S_PREP                            = 69,
  IE11S_PERR                            = 70,
  /* end of open80211s-compatible IDs */
  IE11S_PROXY_UPDATE                    = 37,
  IE11S_PROXY_UPDATE_CONFIRMATION,
  IE11S_ABBREVIATED_HANDSHAKE,
  IE11S_MESH_PEERING_PROTOCOL_VERSION   = 74,
};

/**
 * \ingroup mesh
 *
 * \brief Information element, as defined in 802.11-2007 standard
 *
 * Elements are defined to have a common general format consisting of a 1 octet Element ID field, a 1 octet
 * length field, and a variable-length element-specific information field. Each element is assigned a unique
 * Element ID as defined in this standard. The Length field specifies the number of octets in the Information
 * field.
 */
class WifiInformationElement : public SimpleRefCount<WifiInformationElement>
{
public:
  virtual ~WifiInformationElement ();
  ///\name Each subclass must implement
  //\{
  virtual void Print (std::ostream &os) const = 0;
  /// Own unique Element ID
  virtual WifiElementId ElementId () const = 0;
  /// Length of serialized information
  virtual uint8_t GetInformationSize () const = 0;
  /// Serialize information
  virtual void SerializeInformation (Buffer::Iterator start) const = 0;
  /// Deserialize information
  virtual uint8_t DeserializeInformation (Buffer::Iterator start, uint8_t length) = 0;
  //\}

  /// Compare information elements using Element ID
  friend bool operator< (WifiInformationElement const & a, WifiInformationElement const & b);
  /// Compare two IE by ID & Length, than it calls DynamicCast and compares the body of elements
  virtual bool operator== (WifiInformationElement const & a) { return false; }
};

/// Compare information elements using Element ID
bool operator< (WifiInformationElement const & a, WifiInformationElement const & b);

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
  Ptr<WifiInformationElement> FindFirst (enum WifiElementId id) const;
private:
  typedef std::vector<Ptr<WifiInformationElement> > IE_VECTOR;
  /// Current number of bytes
  uint32_t GetSize () const;
  IE_VECTOR m_elements;
  /// Size in bytes (actually, max packet length)
  uint16_t m_maxSize;
  friend bool operator== (const WifiInformationElementVector & a, const WifiInformationElementVector & b);
};
bool operator== (const WifiInformationElementVector & a, const WifiInformationElementVector & b);
}
#endif
