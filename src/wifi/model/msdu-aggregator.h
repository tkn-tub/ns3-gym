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
#ifndef MSDU_AGGREGATOR_H
#define MSDU_AGGREGATOR_H

#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/object.h"

#include "amsdu-subframe-header.h"

#include <list>

namespace ns3 {

class WifiMacHeader;

/**
 * \brief Abstract class that concrete msdu aggregators have to implement
 * \ingroup wifi
 */
class MsduAggregator : public Object
{
public:
  typedef std::list<std::pair<Ptr<Packet>, AmsduSubframeHeader> > DeaggregatedMsdus;
  typedef std::list<std::pair<Ptr<Packet>, AmsduSubframeHeader> >::const_iterator DeaggregatedMsdusCI;

  static TypeId GetTypeId (void);
  /* Adds <i>packet</i> to <i>aggregatedPacket</i>. In concrete aggregator's implementation is
   * specified how and if <i>packet</i> can be added to <i>aggregatedPacket</i>. If <i>packet</i>
   * can be added returns true, false otherwise.
   */
  virtual bool Aggregate (Ptr<const Packet> packet, Ptr<Packet> aggregatedPacket,
                          Mac48Address src, Mac48Address dest) = 0;

  static DeaggregatedMsdus Deaggregate (Ptr<Packet> aggregatedPacket);
};

}  // namespace ns3

#endif /* MSDU_AGGREGATOR_H */
