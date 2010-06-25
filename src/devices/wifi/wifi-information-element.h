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
 * Authors: Dean Armstrong <deanarm@gmail.com>
 */

#ifndef WIFI_INFORMATION_ELEMENT_H
#define WIFI_INFORMATION_ELEMENT_H

#include "ns3/header.h"
#include "ns3/simple-ref-count.h"

/**
 * \ingroup wifi
 *
 * \subsection wifi_ie Information Elements
 *
 * The IEEE 802.11 standard includes the notion of Information
 * Elements, which are encodings of management information to be
 * communicated between STAs in the payload of various frames of type
 * Management. Information Elements (IEs) have a common format, each
 * starting with a single octet - the Element ID, which indicates the
 * specific type of IE (a type to represent the options here is
 * defined as WifiElementId). The next octet is a length field and
 * encodes the number of octets in the third and final field, which is
 * the IE Information field.
 *
 * The class ns3::WifiInformationElement provides a base for classes
 * which represent specific Information Elements. This class defines
 * pure virtual methods for serialisation
 * (ns3::WifiInformationElement::SerializeInformation) and
 * deserialisation
 * (ns3::WifiInformationElement::DeserializeInformation) of IEs, from
 * or to data members or other objects that simulation objects use to
 * maintain the relevant state.
 *
 * This class also provides an implementation of the equality
 * operator, which operates by comparing the serialised versions of
 * the two WifiInformationElement objects concerned.
 */

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
 * duplication of IE IDs in the #definitions below (and in files which
 * declare "subclasses" of WifiInformationElement). Sorry.
 */
typedef uint8_t WifiElementId;


/**
 * Here we have definition of all Information Element IDs in IEEE
 * 802.11-2007. See the comments for WifiElementId - this could
 * probably be done in a considerably tidier manner.
 */
#define IE_SSID                                ((WifiElementId)0)
#define IE_SUPPORTED_RATES                     ((WifiElementId)1)
#define IE_FH_PARAMETER_SET                    ((WifiElementId)2)
#define IE_DS_PARAMETER_SET                    ((WifiElementId)3)
#define IE_CF_PARAMETER_SET                    ((WifiElementId)4)
#define IE_TIM                                 ((WifiElementId)5)
#define IE_IBSS_PARAMETER_SET                  ((WifiElementId)6)
#define IE_COUNTRY                             ((WifiElementId)7)
#define IE_HOPPING_PATTERN_PARAMETERS          ((WifiElementId)8)
#define IE_HOPPING_PATTERN_TABLE               ((WifiElementId)9)
#define IE_REQUEST                             ((WifiElementId)10)
#define IE_BSS_LOAD                            ((WifiElementId)11)
#define IE_EDCA_PARAMETER_SET                  ((WifiElementId)12)
#define IE_TSPEC                               ((WifiElementId)13)
#define IE_TCLAS                               ((WifiElementId)14)
#define IE_SCHEDULE                            ((WifiElementId)15)
#define IE_CHALLENGE_TEXT                      ((WifiElementId)16)
// 17 to 31 are reserved in 802.11-2007
#define IE_POWER_CONSTRAINT                    ((WifiElementId)32)
#define IE_POWER_CAPABILITY                    ((WifiElementId)33)
#define IE_TPC_REQUEST                         ((WifiElementId)34)
#define IE_TPC_REPORT                          ((WifiElementId)35)
#define IE_SUPPORTED_CHANNELS                  ((WifiElementId)36)
#define IE_CHANNEL_SWITCH_ANNOUNCEMENT         ((WifiElementId)37)
#define IE_MEASUREMENT_REQUEST                 ((WifiElementId)38)
#define IE_MEASUREMENT_REPORT                  ((WifiElementId)39)
#define IE_QUIET                               ((WifiElementId)40)
#define IE_IBSS_DFS                            ((WifiElementId)41)
#define IE_ERP_INFORMATION                     ((WifiElementId)42)
#define IE_TS_DELAY                            ((WifiElementId)43)
#define IE_TCLAS_PROCESSING                    ((WifiElementId)44)
// 45 is reserved in 802.11-2007
#define IE_QOS_CAPABILITY                      ((WifiElementId)46)
// 47 is reserved in 802.11-2007
#define IE_RSN                                 ((WifiElementId)48)
// 49 is reserved in 802.11-2007
#define IE_EXTENDED_SUPPORTED_RATES            ((WifiElementId)50)
// 51 to 126 are reserved in 802.11-2007
#define IE_EXTENDED_CAPABILITIES               ((WifiElementId)127)
// 128 to 220 are reserved in 802.11-2007
#define IE_VENDOR_SPECIFIC                     ((WifiElementId)221)
// 222 to 255 are reserved in 802.11-2007


/**
 * \brief Information element, as defined in 802.11-2007 standard
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
  virtual uint8_t DeserializeInformation (Buffer::Iterator start,
                                          uint8_t length) = 0;
  //\}

  /// Compare information elements using Element ID
  virtual bool operator< (WifiInformationElement const & a) const;
  /// Compare two IEs for equality by ID & Length, and then through
  /// memcmp of serialised version
  virtual bool operator== (WifiInformationElement const & a) const;
};

}
#endif /* WIFI_INFORMATION_ELEMENT_H */
