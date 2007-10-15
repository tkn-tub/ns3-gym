/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
 * All rights reserved.
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef MAC_PARAMETERS_H
#define MAC_PARAMETERS_H

#include <stdint.h>
#include "ns3/nstime.h"

namespace ns3 {

class MacParameters 
{
public:
  MacParameters ();

  void Initialize (Time ctsDelay, Time ackDelay);
  void SetSlotTime (Time slotTime);

  // XXX AP-specific
  Time GetBeaconInterval (void) const;

  Time GetPifs (void) const;
  Time GetSifs (void) const;
  Time GetSlotTime (void) const;

  uint32_t GetMaxSsrc (void) const;
  uint32_t GetMaxSlrc (void) const;
  uint32_t GetRtsCtsThreshold (void) const;
  uint32_t GetFragmentationThreshold (void) const;
  Time GetCtsTimeout (void) const;
  Time GetAckTimeout (void) const;
  Time GetMsduLifetime (void) const;
  Time GetMaxPropagationDelay (void) const;

  uint32_t GetMaxMsduSize (void) const;
  double GetCapLimit (void) const;
  double GetMinEdcaTrafficProportion (void) const;
private:
  void Initialize80211a (void);
  Time m_ctsTimeout;
  Time m_ackTimeout;
  Time m_sifs;
  Time m_pifs;
  Time m_slot;
  uint32_t m_maxSsrc;
  uint32_t m_maxSlrc;
  uint32_t m_rtsCtsThreshold;
  uint32_t m_fragmentationThreshold;
  Time m_maxPropagationDelay;
  static const double SPEED_OF_LIGHT; // m/s
};

}; // namespace ns3

#endif /* MAC_PARAMETERS_H */
