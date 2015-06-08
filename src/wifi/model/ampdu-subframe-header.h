/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013
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
 * Author: Ghada Badawy <gbadawy@gmail.com>
 */
#ifndef AMPDU_SUBFRAME_HEADER_H
#define AMPDU_SUBFRAME_HEADER_H

#include "ns3/header.h"
#include "ns3/mac48-address.h"

namespace ns3 {

/**
 * \ingroup wifi
 *
 *
 */
class AmpduSubframeHeader : public Header
{
public:
  AmpduSubframeHeader ();
  virtual ~AmpduSubframeHeader ();

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

  /**
   * Set the CRC field.
   *
   * \param crc
   */
  void SetCrc (uint8_t crc);
  /**
   * Set the SIG field.
   *
   * \param crc
   */
  void SetSig ();
  /**
   * Set the length field.
   *
   * \param length
   */
  void SetLength (uint16_t length);
  /**
   * Return the CRC field.
   *
   * \return the CRC field
   */
  uint8_t GetCrc (void) const;
  /**
   * Return the SIG field.
   *
   * \return the SIG field
   */
  uint8_t GetSig (void) const;
  /**
   * Return the length field.
   *
   * \return the length field
   */
  uint16_t GetLength (void) const;

private:
  uint8_t m_crc;     //!< CRC field
  uint8_t m_sig;     //!< SIG field
  uint16_t m_length; //!< length field
};

} // namespace ns3

#endif /* AMPDU_SUBFRAME_HEADER_H */
