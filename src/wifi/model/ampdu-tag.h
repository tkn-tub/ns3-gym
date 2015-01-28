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
#ifndef AMPDU_TAG_H
#define AMPDU_TAG_H

#include "ns3/packet.h"

namespace ns3 {

class Tag;

/**
 * \ingroup wifi
 *
 * The aim of the AmpduTag is to provide means for a MAC to specify that a packet includes A-MPDU
 * since this is done in HT-SIG and there is no HT-SIG representation in ns-3
 */
class AmpduTag : public Tag
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  /**
   * Create a AmpduTag with the default =0 no Ampdu
   */
  AmpduTag ();
  /**
   * Set m_ampdu to 1.
   */
  void SetAmpdu (bool supported);
  /**
   * \param noofmpdus the number of MPDUs
   *
   * Set the number of MPDUs in the A-MPDU.
   */
  void SetNoOfMpdus (uint8_t noofmpdus);

  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual uint32_t GetSerializedSize () const;
  virtual void Print (std::ostream &os) const;

  /**
   * \return true if it is an A-MPDU,
   *         false otherwise.
   *
   * Returns m_ampdu
   */
  bool GetAmpdu (void) const;
  /**
   * \return the number of MPDUs in an A-MPDU
   *
   * Returns the number of MPDUs in an A-MPDU
   */
  uint8_t GetNoOfMpdus (void) const;

private:
  uint8_t m_ampdu;     //!< Flag whether it is an A-MPDU
  uint8_t m_noOfMpdus; //!< number of MPDUs in the A-MPDU
};

} // namespace ns3

#endif /* AMPDU_TAG_H */
