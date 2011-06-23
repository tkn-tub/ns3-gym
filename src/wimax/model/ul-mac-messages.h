/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008,2009 INRIA, UDcast
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
 * Authors: Jahanzeb Farooq <jahanzeb.farooq@sophia.inria.fr>
 *          Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 *                               <amine.ismail@UDcast.com>
 */


#ifndef UCD_CHANNEL_ENCODINGS_H
#define UCD_CHANNEL_ENCODINGS_H

#include <stdint.h>
#include "ns3/buffer.h"
#include <list>

namespace ns3 {

/**
 * \ingroup wimax
 * \brief This class implements the UCD channel encodings as described by "IEEE Standard for
 * Local and metropolitan area networks Part 16: Air Interface for Fixed Broadband Wireless Access Systems"
 * 11.3.1 UCD channel encodings, page 651
 *
 */
class UcdChannelEncodings
{
public:
  UcdChannelEncodings (void);
  virtual ~UcdChannelEncodings (void);

  void SetBwReqOppSize (uint16_t bwReqOppSize);
  void SetRangReqOppSize (uint16_t rangReqOppSize);
  void SetFrequency (uint32_t frequency);

  uint16_t GetBwReqOppSize (void) const;
  uint16_t GetRangReqOppSize (void) const;
  uint32_t GetFrequency (void) const;

  uint16_t GetSize (void) const;

  Buffer::Iterator Write (Buffer::Iterator start) const;
  Buffer::Iterator Read (Buffer::Iterator start);
private:
  virtual Buffer::Iterator DoWrite (Buffer::Iterator start) const = 0;
  virtual Buffer::Iterator DoRead (Buffer::Iterator start) = 0;


  uint16_t m_bwReqOppSize;
  uint16_t m_rangReqOppSize;
  uint32_t m_frequency;
};

} // namespace ns3

#endif /* UCD_CHANNEL_ENCODINGS_H */

// ----------------------------------------------------------------------------------------------------------

#ifndef OFDM_UCD_CHANNEL_ENCODINGS_H
#define OFDM_UCD_CHANNEL_ENCODINGS_H

#include <stdint.h>

namespace ns3 {

class OfdmUcdChannelEncodings : public UcdChannelEncodings
{
  /**
   * \brief This class implements the UCD channel encodings as described by "IEEE Standard for
   * Local and metropolitan area networks Part 16: Air Interface for Fixed Broadband Wireless Access Systems"
   * 11.3.1 UCD channel encodings, page 651
   *
   */
public:
  OfdmUcdChannelEncodings (void);
  ~OfdmUcdChannelEncodings (void);

  void SetSbchnlReqRegionFullParams (uint8_t sbchnlReqRegionFullParams);
  void SetSbchnlFocContCodes (uint8_t sbchnlFocContCodes);

  uint8_t GetSbchnlReqRegionFullParams (void) const;
  uint8_t GetSbchnlFocContCodes (void) const;

  uint16_t GetSize (void) const;
private:
  virtual Buffer::Iterator DoWrite (Buffer::Iterator start) const;
  virtual Buffer::Iterator DoRead (Buffer::Iterator start);

  uint8_t m_sbchnlReqRegionFullParams;
  uint8_t m_sbchnlFocContCodes;
};

} // namespace ns3

#endif /* OFDM_UCD_CHANNEL_ENCODINGS_H */

// ----------------------------------------------------------------------------------------------------------

#ifndef OFDM_UL_BURST_PROFILE_H
#define OFDM_UL_BURST_PROFILE_H

#include <stdint.h>
#include "ns3/buffer.h"

namespace ns3 {

class OfdmUlBurstProfile
{
  /**
   * \brief This class implements the UL burst profile as described by "IEEE Standard for
   * Local and metropolitan area networks Part 16: Air Interface for Fixed Broadband Wireless Access Systems"
   * 11.3.1.1 Uplink burst profile encodings, page 655
   *
   */
public:
  enum Uiuc
  {
    UIUC_INITIAL_RANGING = 1,
    UIUC_REQ_REGION_FULL,
    UIUC_REQ_REGION_FOCUSED,
    UIUC_FOCUSED_CONTENTION_IE,
    UIUC_BURST_PROFILE_5,
    UIUC_BURST_PROFILE_6,
    UIUC_BURST_PROFILE_7,
    UIUC_BURST_PROFILE_8,
    UIUC_BURST_PROFILE_9,
    UIUC_BURST_PROFILE_10,
    UIUC_BURST_PROFILE_11,
    UIUC_BURST_PROFILE_12,
    UIUC_SUBCH_NETWORK_ENTRY,
    UIUC_END_OF_MAP
  };

  OfdmUlBurstProfile (void);
  ~OfdmUlBurstProfile (void);

  void SetType (uint8_t type);
  void SetLength (uint8_t length);
  void SetUiuc (uint8_t uiuc);
  void SetFecCodeType (uint8_t fecCodeType);

  uint8_t GetType (void) const;
  uint8_t GetLength (void) const;
  uint8_t GetUiuc (void) const;
  uint8_t GetFecCodeType (void) const;

  uint16_t GetSize (void) const;

  Buffer::Iterator Write (Buffer::Iterator start) const;
  Buffer::Iterator Read (Buffer::Iterator start);
private:
  uint8_t m_type;
  uint8_t m_length;
  uint8_t m_uiuc;

  // TLV Encoded information
  uint8_t m_fecCodeType;
};

} // namespace ns3

#endif /* OFDM_UL_BURST_PROFILE_H */

// ----------------------------------------------------------------------------------------------------------

#ifndef UCD_H
#define UCD_H

#include <stdint.h>
#include "ns3/header.h"
#include <vector>

namespace ns3 {

class Ucd : public Header
{
  /**
   * \brief This class implements the UCD message as described by "IEEE Standard for
   * Local and metropolitan area networks Part 16: Air Interface for Fixed Broadband Wireless Access Systems"
   * 6.3.2.3.3 Uplink Channel Descriptor (UCD) message, page 47
   *
   */
public:
  Ucd (void);
  virtual ~Ucd (void);

  void SetConfigurationChangeCount (uint8_t ucdCount);
  void SetRangingBackoffStart (uint8_t rangingBackoffStart);
  void SetRangingBackoffEnd (uint8_t rangingBackoffEnd);
  void SetRequestBackoffStart (uint8_t requestBackoffStart);
  void SetRequestBackoffEnd (uint8_t requestBackoffEnd);
  void SetChannelEncodings (OfdmUcdChannelEncodings channelEncodings);
  void AddUlBurstProfile (OfdmUlBurstProfile ulBurstProfile);
  void SetNrUlBurstProfiles (uint8_t nrUlBurstProfiles);

  uint8_t GetConfigurationChangeCount (void) const;
  uint8_t GetRangingBackoffStart (void) const;
  uint8_t GetRangingBackoffEnd (void) const;
  uint8_t GetRequestBackoffStart (void) const;
  uint8_t GetRequestBackoffEnd (void) const;
  OfdmUcdChannelEncodings GetChannelEncodings (void) const;
  std::vector<OfdmUlBurstProfile> GetUlBurstProfiles (void) const;
  uint8_t GetNrUlBurstProfiles (void) const;

  std::string GetName (void) const;
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
private:
  uint8_t m_configurationChangeCount;
  uint8_t m_rangingBackoffStart;
  uint8_t m_rangingBackoffEnd;
  uint8_t m_requestBackoffStart;
  uint8_t m_requestBackoffEnd;
  OfdmUcdChannelEncodings m_channelEncodings; // TLV Encoded information for the overall channel
  std::vector<OfdmUlBurstProfile> m_ulBurstProfiles;

  uint8_t m_nrUlBurstProfiles;
};

} // namespace ns3

#endif /* UCD_H */

// ----------------------------------------------------------------------------------------------------------

#ifndef OFDM_UL_MAP_IE_H
#define OFDM_UL_MAP_IE_H

#include <stdint.h>
#include "ns3/header.h"
#include "cid.h"

namespace ns3 {

class OfdmUlMapIe
{
  /**
   * \brief This class implements the UL-MAP_IE message as described by "IEEE Standard for
   * Local and metropolitan area networks Part 16: Air Interface for Fixed Broadband Wireless Access Systems"
   * 6.3.2.3.43.7 UL-MAP_IE, page 115
   *
   */

public:
  OfdmUlMapIe (void);
  ~OfdmUlMapIe (void);

  void SetCid (Cid cid);
  void SetStartTime (uint16_t startTime);
  void SetSubchannelIndex (uint8_t subchannelIndex);
  void SetUiuc (uint8_t uiuc);
  void SetDuration (uint16_t duration);
  void SetMidambleRepetitionInterval (uint8_t midambleRepetitionInterval);

  Cid GetCid (void) const;
  uint16_t GetStartTime (void) const;
  uint8_t GetSubchannelIndex (void) const;
  uint8_t GetUiuc (void) const;
  uint16_t GetDuration (void) const;
  uint8_t GetMidambleRepetitionInterval (void) const;

  uint16_t GetSize (void) const;

  Buffer::Iterator Write (Buffer::Iterator start) const;
  Buffer::Iterator Read (Buffer::Iterator start);
private:
  Cid m_cid;
  uint16_t m_startTime;
  uint8_t m_subchannelIndex;
  uint8_t m_uiuc;
  uint16_t m_duration;
  uint8_t m_midambleRepetitionInterval;

  // fields to be implemented later on:
  // m_focusedContentionIe()
  // m_subchannelizedNetworkEntryIe()
  // m_ulExtendedIe()
  // m_paddingNibble;
};

} // namespace ns3

#endif /* OFDM_UL_MAP_IE_H */

// ----------------------------------------------------------------------------------------------------------

#ifndef UL_MAP_H
#define UL_MAP_H

#include <stdint.h>
#include "ns3/header.h"
#include <vector>

namespace ns3 {

class UlMap : public Header
{
  /**
   * \brief This class implements the UL-MAP_IE message as described by "IEEE Standard for
   * Local and metropolitan area networks Part 16: Air Interface for Fixed Broadband Wireless Access Systems"
   * 8.2.1.8.2 Compressed UL-MAP, page 404
   *
   */
public:
  UlMap (void);
  virtual ~UlMap (void);

  void SetUcdCount (uint8_t ucdCount);
  void SetAllocationStartTime (uint32_t allocationStartTime);
  void AddUlMapElement (OfdmUlMapIe ulMapElement);

  uint8_t GetUcdCount (void) const;
  uint32_t GetAllocationStartTime (void) const;
  std::list<OfdmUlMapIe> GetUlMapElements (void) const;

  std::string GetName (void) const;
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
private:
  uint8_t m_reserved; // changed as per the amendment 802.16e-2005

  uint8_t m_ucdCount;
  uint32_t m_allocationStartTime;
  std::list<OfdmUlMapIe> m_ulMapElements;
};

} // namespace ns3

#endif /* UL_MAP_H */

