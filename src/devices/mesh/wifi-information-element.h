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
 * Author: Pavel Boyko <boyko@iitp.ru>
 */

#ifndef WIFIINFORMATIONELEMENT_H_
#define WIFIINFORMATIONELEMENT_H_

#include "ns3/header.h"
#include "ns3/ref-count-base.h"

namespace ns3 {

/**
 * \ingroup mesh
 * 
 * \brief Enum of all known information element id. 
 * 
 * For now only 802.11s (mesh) related elements are supported here (so 11S prefix), 
 * but this can change in future.
 */
enum WifiElementId {
  IE11S_MESH_CONFIGURATION = 18,
  IE11S_MESH_ID,
  IE11S_LINK_METRIC_REPORT,
  IE11S_CONGESTION_NOTIFICATION,
  IE11S_PEER_LINK_MANAGEMENT,
  IE11S_MESH_CHANNEL_SWITCH_ANNOUNCEMENT,
  IE11S_MESH_TIM,
  IE11S_AWAKE_WINDOW,
  IE11S_SYNCHRONIZATION_PROTOCOL,
  IE11S_BEACON_TIMING,
  IE11S_MDAOP_SETUP_REQUEST,
  IE11S_MDAOP_SETUP_REPLY,
  IE11S_MDAOP_ADVERTISEMENT,
  IE11S_MDAOP_SET_TEARDOWN,
  IE11S_CONNECTIVITY_REPORT,
  IE11S_PORTAL_ANNOUNCEMENT,
  IE11S_RANN,
  IE11S_PREQ,
  IE11S_PREP,
  IE11S_PERR,
  IE11S_PROXY_UPDATE,
  IE11S_PROXY_UPDATE_CONFIRMATION,
  IE11S_MSCIE,
  IE11S_MSAIE,
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
class WifiInformationElement : public Header, 
                               public RefCountBase     // need this to use Ptr<WifiInformationElement> 
{
public:
  /// Support object system
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const;
  
  /// virtual d-tor for subclasses
  virtual ~WifiInformationElement () {}
  
  ///\name Inherited from Header
  //\{
  /**
   * \return the expected size of the header.
   *
   * This method is used by Packet::AddHeader
   * to store a header into the byte buffer of a packet. This method
   * should return the number of bytes which are needed to store
   * the full header data by Serialize.
   */
  virtual uint32_t GetSerializedSize () const;
  /**
   * \param start an iterator which points to where the header should
   *        be written.
   *
   * This method is used by Packet::AddHeader to
   * store a header into the byte buffer of a packet.
   * The data written
   * is expected to match bit-for-bit the representation of this
   * header in a real network.
   */
  virtual void Serialize (Buffer::Iterator start) const;
  /**
   * \param start an iterator which points to where the header should
   *        written.
   * \return the number of bytes read.
   *
   * This method is used by Packet::RemoveHeader to
   * re-create a header from the byte buffer of a packet. 
   * The data read is expected to
   * match bit-for-bit the representation of this header in real
   * networks.
   */
  virtual uint32_t Deserialize (Buffer::Iterator start);
  /**
   * This method is used by Packet::Print to print the 
   * content of a trailer as ascii data to a c++ output stream.
   * Although the trailer is free to format its output as it
   * wishes, it is recommended to follow a few rules to integrate
   * with the packet pretty printer: start with flags, small field 
   * values located between a pair of parens. Values should be separated 
   * by whitespace. Follow the parens with the important fields, 
   * separated by whitespace.
   * i.e.: (field1 val1 field2 val2 field3 val3) field4 val4 field5 val5
   */
  virtual void Print (std::ostream &os) const;
  //\}
    
protected:
  ///\name Each subclass must implement 
  //\{
  /// Own unique Element ID
  virtual WifiElementId ElementId () const = 0;
  /// Length of serialized information
  virtual uint8_t GetInformationSize () const = 0;
  /// Serialize information
  virtual void SerializeInformation (Buffer::Iterator start) const = 0;
  /// Deserialize information
  virtual uint8_t DeserializeInformation (Buffer::Iterator start, uint8_t length) = 0;
  /// Print information
  virtual void PrintInformation (std::ostream &os) const = 0;
  //\}
  
  /// Compare information elements using Element ID
  friend bool operator< (WifiInformationElement const & a, WifiInformationElement const & b);
};

/// Compare information elements using Element ID
bool operator< (WifiInformationElement const & a, WifiInformationElement const & b);
 
}  // namespace ns3
#endif /* WIFIINFORMATIONELEMENT_H_ */
