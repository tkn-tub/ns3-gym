/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 Sébastien Deronne
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
 * Author: Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#ifndef HT_OPERATIONS_H
#define HT_OPERATIONS_H

#include <stdint.h>
#include "ns3/buffer.h"
#include "ns3/attribute-helper.h"
#include "ns3/wifi-information-element.h"

/**
 * This defines the maximum number of supported MCSs that a STA is
 * allowed to have. Currently this number is set for IEEE 802.11n
 */
#define MAX_SUPPORTED_MCS  (77)

namespace ns3 {

enum HtProtectionType
{
  NO_PROTECTION,
  NON_MEMBER_PROTECTION,
  TWENTY_MHZ_PROTECTION,
  MIXED_MODE_PROTECTION
};

/**
 * \brief The HT Operations Information Element
 * \ingroup wifi
 *
 * This class knows how to serialise and deserialise 
 * the HT Operations Information Element
 *
 * \see attribute_HtOperations
 */
class HtOperations: public WifiInformationElement
{
public:
  HtOperations ();
  void SetHtSupported (uint8_t htsupported);
  
  /**
   * Set the Primary Channel field in the HT Operations information element.
   *
   * \param ctrl the Primary Channel field in the HT Operations information element
   */
  void SetPrimaryChannel (uint8_t ctrl);
  /**
   * Set the Information Subset 1 field in the HT Operations information element.
   *
   * \param ctrl the Information Subset 1 field in the HT Operations information element
   */
  void SetInformationSubset1 (uint8_t ctrl);
  /**
   * Set the Information Subset 2 field in the HT Operations information element.
   *
   * \param ctrl the Information Subset 2 field in the HT Operations information element
   */
  void SetInformationSubset2 (uint16_t ctrl);
  /**
   * Set the Information Subset 3 field in the HT Operations information element.
   *
   * \param ctrl the Information Subset 3 field in the HT Operations information element
   */
  void SetInformationSubset3 (uint16_t ctrl);
  /**
   * Set the Basic MCS Set field in the HT Operations information element.
   *
   * \param ctrl1 the first 64 bytes of the Basic MCS Set field in the HT Operations information element
   * \param ctrl2 the last 64 bytes of the Basic MCS Set field in the HT Operations information element
   */
  void SetBasicMcsSet (uint64_t ctrl1, uint64_t ctrl2);

  void SetSecondaryChannelOffset (uint8_t secondarychanneloffset);
  void SetStaChannelWidth (uint8_t stachannelwidth);
  void SetRifsMode (uint8_t rifsmode);

  void SetHtProtection (uint8_t htprotection);
  void SetNonGfHtStasPresent (uint8_t nongfhtstaspresent);
  void SetObssNonHtStasPresent (uint8_t obssnonhtstaspresent);

  void SetDualBeacon (uint8_t dualbeacon);
  void SetDualCtsProtection (uint8_t dualctsprotection);
  void SetStbcBeacon (uint8_t stbcbeacon);
  void SetLSigTxopProtectionFullSupport (uint8_t lsigtxopprotectionfullsupport);
  void SetPcoActive (uint8_t pcoactive);
  void SetPhase (uint8_t pcophase);

  void SetRxMcsBitmask (uint8_t index);
  void SetRxHighestSupportedDataRate (uint16_t maxsupportedrate);
  void SetTxMcsSetDefined (uint8_t txmcssetdefined);
  void SetTxRxMcsSetUnequal (uint8_t txrxmcssetunequal);
  void SetTxMaxNSpatialStreams (uint8_t maxtxspatialstreams);
  void SetTxUnequalModulation (uint8_t txunequalmodulation);
  
  /*
   * Return the Primary Channel field in the HT Operations information element.
   *
   * \return the Primary Channel field in the HT Operations information element
   */
  uint8_t GetPrimaryChannel (void) const;
  /*
   * Return the Information Subset 1 field in the HT Operations information element.
   *
   * \return the Information Subset 1 field in the HT Operations information element
   */
  uint8_t GetInformationSubset1 (void) const;
  /*
   * Return the Information Subset 2 field in the HT Operations information element.
   *
   * \return the Information Subset 2 field in the HT Operations information element
   */
  uint16_t GetInformationSubset2 (void) const;
  /*
   * Return the Information Subset 3 field in the HT Operations information element.
   *
   * \return the Information Subset 3 field in the HT Operations information element
   */
  uint16_t GetInformationSubset3 (void) const;
  /*
   * Return the first 64 bytes of the Basic MCS Set field in the HT Operations information element.
   *
   * \return the first 64 bytes of the Basic MCS Set field in the HT Operations information element
   */
  uint64_t GetBasicMcsSet1 (void) const;
  /*
   * Return the last 64 bytes of the Basic MCS Set field in the HT Operations information element.
   *
   * \return the last 64 bytes of the Basic MCS Set field in the HT Operations information element
   */
  uint64_t GetBasicMcsSet2 (void) const;
  
  uint8_t GetSecondaryChannelOffset (void) const;
  uint8_t GetStaChannelWidth (void) const;
  uint8_t GetRifsMode (void) const;
  
  uint8_t GetHtProtection (void) const;
  uint8_t GetNonGfHtStasPresent (void) const;
  uint8_t GetObssNonHtStasPresent (void) const;
  
  uint8_t GetDualBeacon (void) const;
  uint8_t GetDualCtsProtection (void) const;
  uint8_t GetStbcBeacon (void) const;
  uint8_t GetLSigTxopProtectionFullSupport (void) const;
  uint8_t GetPcoActive (void) const;
  uint8_t GetPhase (void) const;
  
  bool IsSupportedMcs (uint8_t mcs);
  uint16_t GetRxHighestSupportedDataRate (void) const;
  uint8_t GetTxMcsSetDefined (void) const;
  uint8_t GetTxRxMcsSetUnequal (void) const;
  uint8_t GetTxMaxNSpatialStreams (void) const;
  uint8_t GetTxUnequalModulation (void) const;

  WifiInformationElementId ElementId () const;
  uint8_t GetInformationFieldSize () const;
  void SerializeInformationField (Buffer::Iterator start) const;
  uint8_t DeserializeInformationField (Buffer::Iterator start,
                                       uint8_t length);
  /**
   * This information element is a bit special in that it is only
   * included if the STA is an HT STA. To support this we
   * override the Serialize and GetSerializedSize methods of
   * WifiInformationElement.
   *
   * \param start
   *
   * \return an iterator
   */
  Buffer::Iterator Serialize (Buffer::Iterator start) const;
  /**
   * Return the serialized size of this HT Operations IE.
   *
   * \return the serialized size of this HT Operations IE
   */
  uint16_t GetSerializedSize () const;


private:
  uint8_t m_primaryChannel;

  //HT Information Subset 1
  uint8_t m_secondaryChannelOffset;
  uint8_t m_staChannelWidth;
  uint8_t m_rifsMode;
  uint8_t m_reservedInformationSubset1;

  //HT Information Subset 2
  uint8_t m_htProtection;
  uint8_t m_nonGfHtStasPresent;
  uint8_t m_reservedInformationSubset2_1;
  uint8_t m_obssNonHtStasPresent;
  uint8_t m_reservedInformationSubset2_2;

  //HT Information Subset 3
  uint8_t m_reservedInformationSubset3_1;
  uint8_t m_dualBeacon;
  uint8_t m_dualCtsProtection;
  uint8_t m_stbcBeacon;
  uint8_t m_lSigTxopProtectionFullSupport;
  uint8_t m_pcoActive;
  uint8_t m_pcoPhase;
  uint8_t m_reservedInformationSubset3_2;

  //Basic MCS Set field
  uint8_t m_reservedMcsSet1;
  uint16_t m_rxHighestSupportedDataRate;
  uint8_t m_reservedMcsSet2;
  uint8_t m_txMcsSetDefined;
  uint8_t m_txRxMcsSetUnequal;
  uint8_t m_txMaxNSpatialStreams;
  uint8_t m_txUnequalModulation;
  uint32_t m_reservedMcsSet3;
  uint8_t m_rxMcsBitmask[MAX_SUPPORTED_MCS];

  //This is used to decide whether this element should be added to the frame or not
  uint8_t m_htSupported;
};

std::ostream &operator << (std::ostream &os, const HtOperations &htoperations);
std::istream &operator >> (std::istream &is, HtOperations &htoperations);

ATTRIBUTE_HELPER_HEADER (HtOperations);

} //namespace ns3

#endif /* HT_OPERATIONS_H */
