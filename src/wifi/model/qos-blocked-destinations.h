/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

class QosBlockedDestinations
{
public:
  QosBlockedDestinations ();
  ~QosBlockedDestinations ();

  void Block (Mac48Address dest, uint8_t tid);
  void Unblock (Mac48Address dest, uint8_t tid);
  bool IsBlocked (Mac48Address dest, uint8_t tid) const;

private:
  typedef std::list<std::pair<Mac48Address, uint8_t> > BlockedPackets;
  typedef std::list<std::pair<Mac48Address, uint8_t> >::iterator BlockedPacketsI;
  typedef std::list<std::pair<Mac48Address, uint8_t> >::const_iterator BlockedPacketsCI;
  BlockedPackets m_blockedQosPackets;
};

} //namespace ns3

#endif /* QOS_BLOCKED_DESTINATIONS_H */
