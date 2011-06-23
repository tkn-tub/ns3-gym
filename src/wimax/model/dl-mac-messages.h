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

#ifndef DCD_CHANNEL_ENCODINGS_H
#define DCD_CHANNEL_ENCODINGS_H

#include <stdint.h>
#include "ns3/buffer.h"
#include <list>

namespace ns3 {

/**
 * \ingroup wimax
 * \brief This class implements the DCD channel encodings as described by "IEEE Standard for
 * Local and metropolitan area networks Part 16: Air Interface for Fixed Broadband Wireless Access Systems"
 * 11.4.1 DCD channel encodings, page 659
 *
  */
class DcdChannelEncodings
{
public:
  DcdChannelEncodings (void);
  virtual ~DcdChannelEncodings (void);

  void SetBsEirp (uint16_t bs_eirp);
  void SetEirxPIrMax (uint16_t rss_ir_max);
  void SetFrequency (uint32_t frequency);

  uint16_t GetBsEirp (void) const;
  uint16_t GetEirxPIrMax (void) const;
  uint32_t GetFrequency (void) const;

  uint16_t GetSize (void) const;

  Buffer::Iterator Write (Buffer::Iterator start) const;
  Buffer::Iterator Read (Buffer::Iterator start);
private:
  virtual Buffer::Iterator DoWrite (Buffer::Iterator start) const = 0;
  virtual Buffer::Iterator DoRead (Buffer::Iterator start) = 0;

  uint16_t m_bsEirp;
  uint16_t m_eirXPIrMax;
  uint32_t m_frequency;

};

}
; // namespace ns3

#endif /* DCD_CHANNEL_ENCODINGS_H */

// ----------------------------------------------------------------------------------------------------------

#ifndef OFDM_DCD_CHANNEL_ENCODINGS_H
#define OFDM_DCD_CHANNEL_ENCODINGS_H

#include <stdint.h>
#include "ns3/mac48-address.h"

namespace ns3 {

class OfdmDcdChannelEncodings : public DcdChannelEncodings
{
  /**
   * \brief This class implements the DCD channel encodings as described by "IEEE Standard for
   * Local and metropolitan area networks Part 16: Air Interface for Fixed Broadband Wireless Access Systems"
   * 11.4.1 DCD channel encodings, page 659
   *
   */

public:
  OfdmDcdChannelEncodings (void);
  ~OfdmDcdChannelEncodings (void);

  void SetChannelNr (uint8_t channelNr);
  void SetTtg (uint8_t ttg);
  void SetRtg (uint8_t rtg);

  void SetBaseStationId (Mac48Address baseStationId);
  void SetFrameDurationCode (uint8_t frameDurationCode);
  void SetFrameNumber (uint32_t frameNumber);

  uint8_t GetChannelNr (void) const;
  uint8_t GetTtg (void) const;
  uint8_t GetRtg (void) const;

  Mac48Address GetBaseStationId (void) const;
  uint8_t GetFrameDurationCode (void) const;
  uint32_t GetFrameNumber (void) const;

  uint16_t GetSize (void) const;
private:
  virtual Buffer::Iterator DoWrite (Buffer::Iterator start) const;
  virtual Buffer::Iterator DoRead (Buffer::Iterator start);

  uint8_t m_channelNr;
  uint8_t m_ttg;
  uint8_t m_rtg;

  uint32_t m_chnlSwitchFrameNr;
  Mac48Address m_baseStationId;
  uint8_t m_frameDurationCode;
  uint32_t m_frameNumber;
};

}
; // namespace ns3

#endif /* OFDM_DCD_CHANNEL_ENCODINGS_H */

// ----------------------------------------------------------------------------------------------------------

#ifndef OFDM_DL_BURST_PROFILE_H
#define OFDM_DL_BURST_PROFILE_H

#include <stdint.h>
#include "ns3/buffer.h"

namespace ns3 {

class OfdmDlBurstProfile
{
  /**
   * \brief This class implements the OFDM Downlink burst profile descriptor as described by "IEEE Standard for
   * Local and metropolitan area networks Part 16: Air Interface for Fixed Broadband Wireless Access Systems"
   * 8.2.1.10 Burst profile formats page 416
   *
   */
public:
  enum Diuc
  {
    DIUC_STC_ZONE = 0,
    DIUC_BURST_PROFILE_1,
    DIUC_BURST_PROFILE_2,
    DIUC_BURST_PROFILE_3,
    DIUC_BURST_PROFILE_4,
    DIUC_BURST_PROFILE_5,
    DIUC_BURST_PROFILE_6,
    DIUC_BURST_PROFILE_7,
    DIUC_BURST_PROFILE_8,
    DIUC_BURST_PROFILE_9,
    DIUC_BURST_PROFILE_10,
    DIUC_BURST_PROFILE_11,
    // 12 is reserved
    DIUC_GAP = 13,
    DIUC_END_OF_MAP
  };

  OfdmDlBurstProfile (void);
  ~OfdmDlBurstProfile (void);

  void SetType (uint8_t type);
  void SetLength (uint8_t length);
  void SetDiuc (uint8_t diuc);

  void SetFecCodeType (uint8_t fecCodeType);

  uint8_t GetType (void) const;
  uint8_t GetLength (void) const;
  uint8_t GetDiuc (void) const;

  uint8_t GetFecCodeType (void) const;

  uint16_t GetSize (void) const;

  Buffer::Iterator Write (Buffer::Iterator start) const;
  Buffer::Iterator Read (Buffer::Iterator start);
private:
  uint8_t m_type;
  uint8_t m_length;
  uint8_t m_diuc;

  // TLV Encoded information
  uint8_t m_fecCodeType;
};

}
; // namespace ns3

#endif /* OFDM_DL_BURST_PROFILE_H */

// ----------------------------------------------------------------------------------------------------------

#ifndef DCD_H
#define DCD_H

#include <stdint.h>
#include "ns3/header.h"
#include <vector>

namespace ns3 {

class Dcd : public Header
{
  /**
    * \brief This class implements Downlink channel descriptor as described by "IEEE Standard for
    * Local and metropolitan area networks Part 16: Air Interface for Fixed Broadband Wireless Access Systems"
    * 6.3.2.3.1 Downlink Channel Descriptor (DCD) message, page 45
    *
    */
public:
  Dcd (void);
  virtual ~Dcd (void);

  void SetConfigurationChangeCount (uint8_t configurationChangeCount);
  void SetChannelEncodings (OfdmDcdChannelEncodings channelEncodings);
  void AddDlBurstProfile (OfdmDlBurstProfile dlBurstProfile);
  void SetNrDlBurstProfiles (uint8_t nrDlBurstProfiles);

  uint8_t GetConfigurationChangeCount (void) const;
  OfdmDcdChannelEncodings GetChannelEncodings (void) const;
  std::vector<OfdmDlBurstProfile> GetDlBurstProfiles (void) const;
  uint8_t GetNrDlBurstProfiles (void) const;

  std::string GetName (void) const;
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
private:
  uint8_t m_reserved; // changed as per the amendment 802.16e-2005
  uint8_t m_configurationChangeCount;
  OfdmDcdChannelEncodings m_channelEncodings; // TLV Encoded information for the overall channel
  std::vector<OfdmDlBurstProfile> m_dlBurstProfiles; // vector of download burst profiles

  uint8_t m_nrDlBurstProfiles;
};

}
; // namespace ns3

#endif /* DCD_H */

// ----------------------------------------------------------------------------------------------------------

#ifndef OFDM_DL_MAP_IE_H
#define OFDM_DL_MAP_IE_H

#include <stdint.h>
#include "cid.h"

namespace ns3 {

class OfdmDlMapIe
{
  /**
    * \brief This class implements DL-map ie as described by "IEEE Standard for
    * Local and metropolitan area networks Part 16: Air Interface for Fixed Broadband Wireless Access Systems"
    * 6.3.2.3.43.6 Compact DL-MAP IE page 109
    *
    */
public:
  OfdmDlMapIe (void);
  ~OfdmDlMapIe (void);

  void SetCid (Cid cid);
  void SetDiuc (uint8_t diuc);
  void SetPreamblePresent (uint8_t preamblePresent);
  void SetStartTime (uint16_t startTime);

  Cid GetCid (void) const;
  uint8_t GetDiuc (void) const;
  uint8_t GetPreamblePresent (void) const;
  uint16_t GetStartTime (void) const;

  uint16_t GetSize (void) const;

  Buffer::Iterator Write (Buffer::Iterator start) const;
  Buffer::Iterator Read (Buffer::Iterator start);
private:
  Cid m_cid;
  uint8_t m_diuc;
  uint8_t m_preamblePresent;
  uint16_t m_startTime;

};

}
; // namespace ns3

#endif /* OFDM_DL_MAP_IE_H */

// ----------------------------------------------------------------------------------------------------------

#ifndef DL_MAP_H
#define DL_MAP_H

#include <stdint.h>
#include <vector>
#include "ns3/header.h"
#include "ns3/mac48-address.h"

namespace ns3 {

class DlMap : public Header
{
  /**
    * \brief This class implements DL-map ie as described by "IEEE Standard for
    * Local and metropolitan area networks Part 16: Air Interface for Fixed Broadband Wireless Access Systems"
    * 8.2.1.8.1 Compressed DL-MAP, page 402
    *
    */

public:
  DlMap (void);
  virtual ~DlMap (void);

  void SetDcdCount (uint8_t dcdCount);
  void SetBaseStationId (Mac48Address baseStationID);
  void AddDlMapElement (OfdmDlMapIe dlMapElement);

  uint8_t GetDcdCount (void) const;
  Mac48Address GetBaseStationId (void) const;
  std::list<OfdmDlMapIe> GetDlMapElements (void) const;

  std::string GetName (void) const;
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
private:
  uint8_t m_dcdCount;
  Mac48Address m_baseStationId;
  std::list<OfdmDlMapIe> m_dlMapElements;
  // m_paddingNibble; //fields to be implemented later on:
};

}
; // namespace ns3

#endif /* DL_MAP_H */
