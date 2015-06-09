/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008 INRIA
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
 * Author: Jahanzeb Farooq <jahanzeb.farooq@sophia.inria.fr>
 */

#ifndef DL_FRAME_PREFIX_IE_H
#define DL_FRAME_PREFIX_IE_H

#include <stdint.h>
#include "ns3/header.h"

namespace ns3 {

/**
 * \ingroup wimax
 *\brief This class implements  the DL Frame Prefix IE as described by IEEE-802.16 standard
 *\brief The DL Frame Prefix IE is contained in DLFP (Downlink Frame Prefix) in OFDM PHY
 */
class DlFramePrefixIe
{
public:
  DlFramePrefixIe ();
  ~DlFramePrefixIe ();

  void SetRateId (uint8_t rateId);
  void SetDiuc (uint8_t diuc);
  void SetPreamblePresent (uint8_t preamblePresent);
  void SetLength (uint16_t length);
  void SetStartTime (uint16_t startTime);

  uint8_t GetRateId (void) const;
  uint8_t GetDiuc (void) const;
  uint8_t GetPreamblePresent (void) const;
  uint16_t GetLength (void) const;
  uint16_t GetStartTime (void) const;

  uint16_t GetSize (void) const;

  Buffer::Iterator Write (Buffer::Iterator start) const;
  Buffer::Iterator Read (Buffer::Iterator start);
private:
  uint8_t m_rateId;
  uint8_t m_diuc;
  uint8_t m_preamblePresent;
  uint16_t m_length;
  uint16_t m_startTime;

  // shall actually contain m_startTime if DIUC is 0. see Table 225, page 452

};

} // namespace ns3

#endif /* DL_FRAME_PREFIX_IE_H */

#ifndef OFDM_DOWNLINK_FRAME_PREFIX_H
#define OFDM_DOWNLINK_FRAME_PREFIX_H

#include <stdint.h>
#include "ns3/header.h"
#include "ns3/mac48-address.h"

namespace ns3 {

class OfdmDownlinkFramePrefix : public Header
{
public:
  OfdmDownlinkFramePrefix ();
  ~OfdmDownlinkFramePrefix ();

  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);
  
  void SetBaseStationId (Mac48Address baseStationId);
  void SetFrameNumber (uint32_t frameNumber);
  void SetConfigurationChangeCount (uint8_t configurationChangeCount);
  void AddDlFramePrefixElement (DlFramePrefixIe dlFramePrefixElement);
  void SetHcs (uint8_t hcs);

  Mac48Address GetBaseStationId (void) const;
  uint32_t GetFrameNumber (void) const;
  uint8_t GetConfigurationChangeCount (void) const;
  std::vector<DlFramePrefixIe> GetDlFramePrefixElements (void) const;
  uint8_t GetHcs (void) const;

  std::string GetName (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
private:
  Mac48Address m_baseStationId;
  uint32_t m_frameNumber; // shall actually be only 4 LSB of the same field in OFDM DCD Channel Encodings
  uint8_t m_configurationChangeCount; // shall actually be only 4 LSB of the same field in DCD
  std::vector<DlFramePrefixIe> m_dlFramePrefixElements; // vector of dl frame prefix elements
  uint8_t m_hcs; // Header Check Sequence
};

} // namespace ns3

#endif /* OFDM_DOWNLINK_FRAME_PREFIX_H */
