/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005, 2009 INRIA
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

#ifndef QOS_BLOCKED_DESTINATIONS_H
#define QOS_BLOCKED_DESTINATIONS_H

#include <list>
#include "ns3/mac48-address.h"

namespace ns3 {

/**
 * Keep track of destination address - TID pairs that are waiting
 * for a block ACK response.
 */
class QosBlockedDestinations
{
public:
  QosBlockedDestinations ();
  ~QosBlockedDestinations ();

  /**
   * Block the given destination address and TID from sending (e.g. pending
   * block ACK response).
   *
   * \param dest
   * \param tid
   */
  void Block (Mac48Address dest, uint8_t tid);
  /**
   * Un-block the given destination address and TID (e.g. block ACK
   * response received).
   *
   * \param dest
   * \param tid
   */
  void Unblock (Mac48Address dest, uint8_t tid);
  /**
   * Check if the given destination address and TID are blocked
   * from sending (e.g. pending block ACK response).
   *
   * \param dest
   * \param tid
   *
   * \return true if the given destination address and TID are blocked from sending,
   *         false otherwise
   */
  bool IsBlocked (Mac48Address dest, uint8_t tid) const;


private:
  /**
   * typedef for a list of <Mac48Address, TID> pair.
   */
  typedef std::list<std::pair<Mac48Address, uint8_t> > BlockedPackets;
  /**
   * typedef for an iterator of BlockedPackets
   */
  typedef std::list<std::pair<Mac48Address, uint8_t> >::iterator BlockedPacketsI;
  /**
   * typedef for a constan iterator of BlockedPackets
   */
  typedef std::list<std::pair<Mac48Address, uint8_t> >::const_iterator BlockedPacketsCI;
  BlockedPackets m_blockedQosPackets;
};

} //namespace ns3

#endif /* QOS_BLOCKED_DESTINATIONS_H */
