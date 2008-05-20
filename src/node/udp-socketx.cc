/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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

#include "ns3/object.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "udp-socketx.h"

NS_LOG_COMPONENT_DEFINE ("UdpSocketx");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (UdpSocketx);

TypeId
UdpSocketx::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UdpSocketx")
    .SetParent<Socket> ()
    .AddAttribute ("RcvBufSize",
                   "UdpSocket maximum receive buffer size (bytes)",
                   UintegerValue (0xffffffffl),
                   MakeUintegerAccessor (&UdpSocketx::GetRcvBufSize,
                                         &UdpSocketx::SetRcvBufSize),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("IpTtl",
                   "socket-specific TTL for unicast IP packets (if non-zero)",
                   UintegerValue (0),
                   MakeUintegerAccessor (&UdpSocketx::GetIpTtl,
                                         &UdpSocketx::SetIpTtl),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("IpMulticastTtl",
                   "socket-specific TTL for multicast IP packets (if non-zero)",
                   UintegerValue (0),
                   MakeUintegerAccessor (&UdpSocketx::GetIpMulticastTtl,
                                         &UdpSocketx::SetIpMulticastTtl),
                   MakeUintegerChecker<uint32_t> ())
    ;
  return tid;
}

UdpSocketx::UdpSocketx ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

UdpSocketx::~UdpSocketx ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

}; // namespace ns3
