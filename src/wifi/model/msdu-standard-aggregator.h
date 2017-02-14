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

#ifndef MSDU_STANDARD_AGGREGATOR_H
#define MSDU_STANDARD_AGGREGATOR_H

#include "msdu-aggregator.h"

namespace ns3 {

/**
 * \ingroup wifi
 * Standard MSDU aggregator
 *
 */
class MsduStandardAggregator : public MsduAggregator
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  MsduStandardAggregator ();
  ~MsduStandardAggregator ();

  /**
   * Sets the maximum A-MSDU size in bytes.
   * Value 0 means that MSDU aggregation is disabled.
   *
   * \param maxSize the maximum A-MSDU size in bytes.
   */
  void SetMaxAmsduSize (uint32_t maxSize);
  /**
   * Returns the maximum A-MSDU size in bytes.
   * Value 0 means that MSDU aggregation is disabled.
   *
   * \return the maximum A-MSDU size in bytes.
   */
  uint32_t GetMaxAmsduSize (void) const;
  /**
   * \param packet Packet we have to insert into <i>aggregatedPacket</i>.
   * \param aggregatedPacket Packet that will contain <i>packet</i>, if aggregation is possible,
   * \param src Source address of <i>packet</i>.
   * \param dest Destination address of <i>packet</i>.
   *
   * \return true if the packet can be aggregated,
   *         false otherwise
   *
   * This method performs an MSDU aggregation.
   * Returns true if <i>packet</i> can be aggregated to <i>aggregatedPacket</i>, false otherwise.
   */
  bool Aggregate (Ptr<const Packet> packet, Ptr<Packet> aggregatedPacket, Mac48Address src, Mac48Address dest) const;


private:
  /**
   * Calculates how much padding must be added to the end of aggregated packet,
   * after that a new packet is added.
   * Each A-MSDU subframe is padded so that its length is multiple of 4 octets.
   *
   * \param packet
   *
   * \return the number of octets required for padding
   */
  uint32_t CalculatePadding (Ptr<const Packet> packet) const;

  uint32_t m_maxAmsduLength; ///< maximum AMSDU length
};

} //namespace ns3

#endif /* MSDU_STANDARD_AGGREGATOR_H */
