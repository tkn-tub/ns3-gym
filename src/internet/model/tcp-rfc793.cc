/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Adrian Sai-wah Tam
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
 * Author: Adrian Sai-wah Tam <adrian.sw.tam@gmail.com>
 */

#include "tcp-rfc793.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("TcpRfc793");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (TcpRfc793)
  ;

TypeId
TcpRfc793::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpRfc793")
    .SetParent<TcpSocketBase> ()
    .AddConstructor<TcpRfc793> ()
  ;
  return tid;
}

TcpRfc793::TcpRfc793 (void)
{
  NS_LOG_FUNCTION (this);
  SetDelAckMaxCount (0);  // Delayed ACK is not in RFC793
}

TcpRfc793::TcpRfc793 (const TcpRfc793& sock) : TcpSocketBase (sock)
{
}

TcpRfc793::~TcpRfc793 (void)
{
}

Ptr<TcpSocketBase>
TcpRfc793::Fork (void)
{
  return CopyObject<TcpRfc793> (this);
}

void
TcpRfc793::DupAck (const TcpHeader& t, uint32_t count)
{
}

void
TcpRfc793::SetSSThresh (uint32_t threshold)
{
  NS_LOG_WARN ("DoD TCP does not perform slow start");
}

uint32_t
TcpRfc793::GetSSThresh (void) const
{
  NS_LOG_WARN ("DoD TCP does not perform slow start");
  return 0;
}

void
TcpRfc793::SetInitialCwnd (uint32_t cwnd)
{
  NS_LOG_WARN ("DoD TCP does not have congestion window");
}

uint32_t
TcpRfc793::GetInitialCwnd (void) const
{
  NS_LOG_WARN ("DoD TCP does not have congestion window");
  return 0;
}

} // namespace ns3
