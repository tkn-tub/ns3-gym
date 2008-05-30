/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Washington
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
 */

#ifndef IPV4_ADDRESS_GENERATOR_H
#define IPV4_ADDRESS_GENERATOR_H

#include "ns3/ipv4-address.h"

namespace ns3 {

/**
 * \ingroup address
 *
 * \brief This generator assigns addresses sequentially from a provided
 * network address; used in topology code.
 */
class Ipv4AddressGenerator {
public:
  static void Init (const Ipv4Address net, const Ipv4Mask mask, 
    const Ipv4Address addr = "0.0.0.1");

  static Ipv4Address NextNetwork (const Ipv4Mask mask);
  static Ipv4Address GetNetwork (const Ipv4Mask mask);

  static void InitAddress (const Ipv4Address addr, const Ipv4Mask mask);
  static Ipv4Address NextAddress (const Ipv4Mask mask);
  static Ipv4Address GetAddress (const Ipv4Mask mask);

  static void Reset (void);
  static bool AddAllocated (const Ipv4Address addr);

  static void TestMode (void);
};

}; // namespace ns3

#endif /* IPV4_ADDRESS_GENERATOR_H */
