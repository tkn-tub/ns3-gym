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

#ifndef MPDU_AGGREGATOR_H
#define MPDU_AGGREGATOR_H

#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/object.h"
#include "ampdu-subframe-header.h"
#include <list>

namespace ns3 {

class WifiMacHeader;

/**
 * \brief Abstract class that concrete mpdu aggregators have to implement
 * \ingroup wifi
 */
class MpduAggregator : public Object
{
public:
  /**
   * A list of deaggregated packets and their A-MPDU subframe headers.
   */
  typedef std::list<std::pair<Ptr<Packet>, AmpduSubframeHeader> > DeaggregatedMpdus;
  /**
   * A constant iterator for a list of deaggregated packets and their A-MPDU subframe headers.
   */
  typedef std::list<std::pair<Ptr<Packet>, AmpduSubframeHeader> >::const_iterator DeaggregatedMpdusCI;

  static TypeId GetTypeId (void);

  virtual void SetMaxAmpduSize (uint32_t maxSize) = 0;
  virtual uint32_t GetMaxAmpduSize (void) const = 0;
  /**
   * \param packet Packet we have to insert into <i>aggregatedPacket</i>.
   * \param aggregatedPacket Packet that will contain <i>packet</i>, if aggregation is possible.
   *
   * \return true if <i>packet</i> can be aggregated to <i>aggregatedPacket</i>, false otherwise.
   *
   * Adds <i>packet</i> to <i>aggregatedPacket</i>. In concrete aggregator's implementation is
   * specified how and if <i>packet</i> can be added to <i>aggregatedPacket</i>.
   */
  virtual bool Aggregate (Ptr<const Packet> packet, Ptr<Packet> aggregatedPacket) = 0;
  /**
  * This method performs a VHT single MPDU aggregation.
  */
  virtual void AggregateVhtSingleMpdu (Ptr<const Packet> packet, Ptr<Packet> aggregatedPacket) = 0;
  /**
   * Adds A-MPDU subframe header and padding to each MPDU that is part of an A-MPDU before it is sent.
   */
  virtual void AddHeaderAndPad (Ptr<Packet> packet, bool last, bool vhtSingleMpdu) = 0;
  /**
   * \param packetSize size of the packet we want to insert into <i>aggregatedPacket</i>.
   * \param aggregatedPacket packet that will contain the packet of size <i>packetSize</i>, if aggregation is possible.
   * \param blockAckSize size of the piggybacked block ack request
   *
   * \return true if the packet of size <i>packetSize</i> can be aggregated to <i>aggregatedPacket</i>, false otherwise.
   *
   * This method is used to determine if a packet could be aggregated to an A-MPDU without exceeding the maximum packet size.
   */
  virtual bool CanBeAggregated (uint32_t packetSize, Ptr<Packet> aggregatedPacket, uint8_t blockAckSize) = 0;
  /**
   * \return padding that must be added to the end of an aggregated packet
   *
   * Calculates how much padding must be added to the end of an aggregated packet, after that a new packet is added.
   * Each A-MPDU subframe is padded so that its length is multiple of 4 octets.
   */
  virtual uint32_t CalculatePadding (Ptr<const Packet> packet) = 0;
  /**
   * Deaggregates an A-MPDU by removing the A-MPDU subframe header and padding.
   *
   * \return list of deaggragted packets and their A-MPDU subframe headers
   */
  static DeaggregatedMpdus Deaggregate (Ptr<Packet> aggregatedPacket);
};

}  //namespace ns3

#endif /* MPDU_AGGREGATOR_H */
