/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007-2009 Strasbourg University
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
 * Author: Sebastien Vincent <vincent@clarinet.u-strasbg.fr>
 */

#include "ns3/uinteger.h"

#include "ipv6-l4-protocol.h"

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED (Ipv6L4Protocol);

TypeId Ipv6L4Protocol::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Ipv6L4Protocol")
    .SetParent<Object> ()
    .AddAttribute ("ProtocolNumber", "The IPv6 protocol number.",
                   UintegerValue (0),
                   MakeUintegerAccessor (&Ipv6L4Protocol::GetProtocolNumber),
                   MakeUintegerChecker<int> ())
  ;
  return tid;
}

Ipv6L4Protocol::~Ipv6L4Protocol ()
{
}

void Ipv6L4Protocol::ReceiveIcmp (Ipv6Address icmpSource, uint8_t icmpTtl,
                                  uint8_t icmpType, uint8_t icmpCode, uint32_t icmpInfo,
                                  Ipv6Address payloadSource, Ipv6Address payloadDestination,
                                  const uint8_t* payload)
{}

} /* namespace ns3 */

