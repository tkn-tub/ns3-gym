/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 MIRKO BANCHI
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
 * Author: Mirko Banchi <mk.banchi@gmail.com>
 */

#ifndef AMSDU_SUBFRAME_HEADER_H
#define AMSDU_SUBFRAME_HEADER_H

#include "ns3/header.h"
#include "ns3/mac48-address.h"

namespace ns3 {

/**
 * \ingroup wifi
 */
class AmsduSubframeHeader : public Header
{
public:
  AmsduSubframeHeader ();
  virtual ~AmsduSubframeHeader ();

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  TypeId GetInstanceTypeId (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);

  /**
   * Set destination address function
   * \param to the destination MAC address
   */
  void SetDestinationAddr (Mac48Address to);
  /**
   * Set source address function
   * \param to the source MAC address
   */
  void SetSourceAddr (Mac48Address to);
  /**
   * Set length function
   * \param length the length
   */
  void SetLength (uint16_t length);
  /**
   * Get destination address function
   * \returns the desitnation MAC address
   */
  Mac48Address GetDestinationAddr (void) const;
  /**
   * Get source address function
   * \returns the source MAC address
   */
  Mac48Address GetSourceAddr (void) const;
  /**
   * Get length function
   * \returns the length
   */
  uint16_t GetLength (void) const;

private:
  Mac48Address m_da; ///< destination address
  Mac48Address m_sa; ///< source addresss
  uint16_t m_length; ///< length
};

} //namespace ns3

#endif /* AMSDU_SUBFRAME_HEADER_H */
