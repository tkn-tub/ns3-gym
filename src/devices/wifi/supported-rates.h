/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
#ifndef SUPPORTED_RATES_H
#define SUPPORTED_RATES_H

#include <stdint.h>
#include <ostream>
#include "ns3/buffer.h"

namespace ns3 {

class SupportedRates {
public:
  SupportedRates ();

  void AddSupportedRate (uint32_t bs);
  void SetBasicRate (uint32_t bs);

  bool IsSupportedRate (uint32_t bs) const;
  bool IsBasicRate (uint32_t bs) const;

  uint8_t GetNRates (void) const;
  uint32_t GetRate (uint8_t i) const;

  uint32_t GetSerializedSize (void) const;
  Buffer::Iterator Serialize (Buffer::Iterator start) const;
  Buffer::Iterator Deserialize (Buffer::Iterator start);
private:
  uint8_t m_nRates;
  uint8_t m_rates[8];
};

std::ostream &operator << (std::ostream &os, const SupportedRates &rates);

} // namespace ns3

#endif /* SUPPORTED_RATES_H */
