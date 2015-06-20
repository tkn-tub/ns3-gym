/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Dean Armstrong
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
 * Author: Dean Armstrong <deanarm@gmail.com>
 */

#ifndef WIFI_INFORMATION_ELEMENT_H
#define WIFI_INFORMATION_ELEMENT_H

#include "ns3/header.h"
#include "ns3/simple-ref-count.h"

namespace ns3 {

/**
 * This type is used to represent an Information Element ID. An
 * enumeration would be tidier, but doesn't provide for the
 * inheritance that is currently preferable to cleanly support
 * pre-standard modules such as mesh. Maybe there is a nice way of
 * doing this with a class.
 *
 * Until such time as a better way of implementing this is dreamt up
 * and applied, developers will need to be careful to avoid
 * duplication of IE IDs in the defines below (and in files which
 * declare "subclasses" of WifiInformationElement). Sorry.
 */
typedef uint8_t WifiInformationElementId;


/**
 * Here we have definition of all Information Element IDs in IEEE
 * 802.11-2007. See the comments for WifiInformationElementId - this could
 * probably be done in a considerably tidier manner.
 */
#define IE_SSID                                ((WifiInformationElementId)0)
#define IE_SUPPORTED_RATES                     ((WifiInformationElementId)1)
#define IE_FH_PARAMETER_SET                    ((WifiInformationElementId)2)
#define IE_DS_PARAMETER_SET                    ((WifiInformationElementId)3)
#define IE_CF_PARAMETER_SET                    ((WifiInformationElementId)4)
#define IE_TIM                                 ((WifiInformationElementId)5)
#define IE_IBSS_PARAMETER_SET                  ((WifiInformationElementId)6)
#define IE_COUNTRY                             ((WifiInformationElementId)7)
#define IE_HOPPING_PATTERN_PARAMETERS          ((WifiInformationElementId)8)
#define IE_HOPPING_PATTERN_TABLE               ((WifiInformationElementId)9)
#define IE_REQUEST                             ((WifiInformationElementId)10)
#define IE_BSS_LOAD                            ((WifiInformationElementId)11)
#define IE_EDCA_PARAMETER_SET                  ((WifiInformationElementId)12)
#define IE_TSPEC                               ((WifiInformationElementId)13)
#define IE_TCLAS                               ((WifiInformationElementId)14)
#define IE_SCHEDULE                            ((WifiInformationElementId)15)
#define IE_CHALLENGE_TEXT                      ((WifiInformationElementId)16)
// 17 to 31 are reserved in 802.11-2007
#define IE_POWER_CONSTRAINT                    ((WifiInformationElementId)32)
#define IE_POWER_CAPABILITY                    ((WifiInformationElementId)33)
#define IE_TPC_REQUEST                         ((WifiInformationElementId)34)
#define IE_TPC_REPORT                          ((WifiInformationElementId)35)
#define IE_SUPPORTED_CHANNELS                  ((WifiInformationElementId)36)
#define IE_CHANNEL_SWITCH_ANNOUNCEMENT         ((WifiInformationElementId)37)
#define IE_MEASUREMENT_REQUEST                 ((WifiInformationElementId)38)
#define IE_MEASUREMENT_REPORT                  ((WifiInformationElementId)39)
#define IE_QUIET                               ((WifiInformationElementId)40)
#define IE_IBSS_DFS                            ((WifiInformationElementId)41)
#define IE_ERP_INFORMATION                     ((WifiInformationElementId)42)
#define IE_TS_DELAY                            ((WifiInformationElementId)43)
#define IE_TCLAS_PROCESSING                    ((WifiInformationElementId)44)
#define IE_HT_CAPABILITIES                     ((WifiInformationElementId)45)
#define IE_QOS_CAPABILITY                      ((WifiInformationElementId)46)
// 47 is reserved in 802.11-2007
#define IE_RSN                                 ((WifiInformationElementId)48)
// 49 is reserved in 802.11-2007
#define IE_EXTENDED_SUPPORTED_RATES            ((WifiInformationElementId)50)
// 51 to 126 are reserved in 802.11-2007
#define IE_EXTENDED_CAPABILITIES               ((WifiInformationElementId)127)
// 128 to 220 are reserved in 802.11-2007
#define IE_VENDOR_SPECIFIC                     ((WifiInformationElementId)221)
// 222 to 255 are reserved in 802.11-2007


/**
 * \brief Information element, as defined in 802.11-2007 standard
 * \ingroup wifi
 *
 * The IEEE 802.11 standard includes the notion of Information
 * Elements, which are encodings of management information to be
 * communicated between STAs in the payload of various frames of type
 * Management. Information Elements (IEs) have a common format, each
 * starting with a single octet - the Element ID, which indicates the
 * specific type of IE (a type to represent the options here is
 * defined as WifiInformationElementId). The next octet is a length field and
 * encodes the number of octets in the third and final field, which is
 * the IE Information field.
 *
 * The class ns3::WifiInformationElement provides a base for classes
 * which represent specific Information Elements. This class defines
 * pure virtual methods for serialisation
 * (ns3::WifiInformationElement::SerializeInformationField) and
 * deserialisation
 * (ns3::WifiInformationElement::DeserializeInformationField) of IEs, from
 * or to data members or other objects that simulation objects use to
 * maintain the relevant state.
 *
 * This class also provides an implementation of the equality
 * operator, which operates by comparing the serialised versions of
 * the two WifiInformationElement objects concerned.
 *
 * Elements are defined to have a common general format consisting of
 * a 1 octet Element ID field, a 1 octet length field, and a
 * variable-length element-specific information field. Each element is
 * assigned a unique Element ID as defined in this standard. The
 * Length field specifies the number of octets in the Information
 * field.
 *
 * This class is pure virtual and acts as base for classes which know
 * how to serialize specific IEs.
 */
class WifiInformationElement : public SimpleRefCount<WifiInformationElement>
{
public:
  virtual ~WifiInformationElement ();
  /**
   * Serialize entire IE including Element ID and length fields
   *
   * \param i an iterator which points to where the IE should be written.
   *
   * \return an iterator
   */
  Buffer::Iterator Serialize (Buffer::Iterator i) const;
  /**
   * Deserialize entire IE, which must be present. The iterator
   * passed in must be pointing at the Element ID (i.e., the very
   * first octet) of the correct type of information element,
   * otherwise this method will generate a fatal error.
   *
   * \param i an iterator which points to where the IE should be read.
   *
   * \return an iterator
   */
  Buffer::Iterator Deserialize (Buffer::Iterator i);
  /**
   * Deserialize entire IE if it is present. The iterator passed in
   * must be pointing at the Element ID of an information element. If
   * the Element ID is not the one that the given class is interested
   * in then it will return the same iterator.
   *
   * \param i an iterator which points to where the IE should be read.
   *
   * \return an iterator
   */
  Buffer::Iterator DeserializeIfPresent (Buffer::Iterator i);
  /**
   * Get the size of the serialized IE including Element ID and
   * length fields.
   *
   * \return the size of the serialized IE
   */
  uint16_t GetSerializedSize () const;

  // Each subclass must implement these pure virtual functions:
  /// Own unique Element ID
  virtual WifiInformationElementId ElementId () const = 0;
  /**
   * Length of serialized information (i.e., the length of the body
   * of the IE, not including the Element ID and length octets. This
   * is the value that will appear in the second octet of the entire
   * IE - the length field)
   *
   * \return the length of serialized information
   */
  virtual uint8_t GetInformationFieldSize () const = 0;
  /**
   * Serialize information (i.e., the body of the IE, not including
   * the Element ID and length octets)
   *
   * \param start an iterator which points to where the information should
   *        be written.
   */
  virtual void SerializeInformationField (Buffer::Iterator start) const = 0;
  /**
   * Deserialize information (i.e., the body of the IE, not including
   * the Element ID and length octets)
   *
   * \param start an iterator which points to where the information should be written.
   * \param length
   *
   * \return the number of bytes read
   */
  virtual uint8_t DeserializeInformationField (Buffer::Iterator start,
                                               uint8_t length) = 0;

  // In addition, a subclass may optionally override the following...
  /**
   * Generate human-readable form of IE
   *
   * \param os output stream
   */
  virtual void Print (std::ostream &os) const;
  /**
   * Compare information elements using Element ID
   *
   * \param a another information element to compare with
   *
   * \return true if the Element ID is less than the other IE Element ID,
   *         false otherwise
   */
  virtual bool operator< (WifiInformationElement const & a) const;
  /**
   * Compare two IEs for equality by ID & Length, and then through
   * memcmp of serialised version
   *
   * \param a another information element to compare with
   *
   * \return true if the two IEs are equal,
   *         false otherwise
   */
  virtual bool operator== (WifiInformationElement const & a) const;

};

} //namespace ns3

#endif /* WIFI_INFORMATION_ELEMENT_H */
