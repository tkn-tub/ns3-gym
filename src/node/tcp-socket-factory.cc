/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Georgia Tech Research Corporation
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
 * Author: Raj Bhattacharjea <raj.b@gatech.edu>
 */
#include "tcp-socket-factory.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (TcpSocketFactory);

TypeId
TcpSocketFactory::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpSocketFactory")
    .SetParent<SocketFactory> ()
    .AddAttribute ("DefaultSegmentSize",
                   "Default TCP maximum segment size in bytes (may be adjusted based on MTU discovery)",
                   UintegerValue (536),
                   MakeUintegerAccessor (&TcpSocketFactory::m_defaultSegSize),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("DefaultAdvertisedWindowSize",
                   "Default TCP advertised window size (bytes)",
                   UintegerValue (0xffff),
                   MakeUintegerAccessor (&TcpSocketFactory::m_defaultAdvWin),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("DefaultSlowStartThreshold",
                   "Default TCP slow start threshold (bytes)",
                   UintegerValue (0xffff),
                   MakeUintegerAccessor (&TcpSocketFactory::m_defaultSsThresh),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("DefaultTxBufferSize",
                   "Default TCP maximum transmit buffer size (bytes)",
                   UintegerValue (0xffffffffl),
                   MakeUintegerAccessor (&TcpSocketFactory::m_defaultTxBuffer),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("DefaultRxBufferSize",
                   "Default TCP maximum receive buffer size (bytes)",
                   UintegerValue (0xffffffffl),
                   MakeUintegerAccessor (&TcpSocketFactory::m_defaultRxBuffer),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("DefaultInitialCongestionWindowSize",
                   "Default TCP initial congestion window size (segments)",
                   UintegerValue (1),
                   MakeUintegerAccessor (&TcpSocketFactory::m_defaultInitialCwnd),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("DefaultConnTimeout",
                   "Default TCP retransmission timeout when opening connection (seconds)",
                   UintegerValue (3),
                   MakeUintegerAccessor (&TcpSocketFactory::m_defaultConnTimeout),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("DefaultConnCount",
                   "Default number of connection attempts (SYN retransmissions) before returning failure",
                   UintegerValue (6),
                   MakeUintegerAccessor (&TcpSocketFactory::m_defaultConnCount),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("DefaultDelAckTimeout",
                   "Default timeout value for TCP delayed acks, in seconds",
                   DoubleValue (0.2),
                   MakeDoubleAccessor (&TcpSocketFactory::m_defaultDelAckTimeout),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("DefaultDelAckCount",
                   "Default number of packets to wait before sending a TCP ack",
                   UintegerValue (2),
                   MakeUintegerAccessor (&TcpSocketFactory::m_defaultDelAckCount),
                   MakeUintegerChecker<uint32_t> ())
     ;
  return tid;
}

uint32_t 
TcpSocketFactory::GetDefaultSegSize (void) const
{
  return m_defaultSegSize;
}
uint32_t 
TcpSocketFactory::GetDefaultAdvWin (void) const
{
  return m_defaultAdvWin;
}
uint32_t 
TcpSocketFactory::GetDefaultSsThresh (void) const
{
  return m_defaultSsThresh;
}
uint32_t 
TcpSocketFactory::GetDefaultTxBuffer (void) const
{
  return m_defaultTxBuffer;
}
uint32_t 
TcpSocketFactory::GetDefaultRxBuffer (void) const
{
  return m_defaultRxBuffer;
}
uint32_t 
TcpSocketFactory::GetDefaultInitialCwnd (void) const
{
  return m_defaultInitialCwnd;
}
uint32_t 
TcpSocketFactory::GetDefaultConnTimeout (void) const
{
  return m_defaultConnTimeout;
}
uint32_t 
TcpSocketFactory::GetDefaultConnCount (void) const
{
  return m_defaultConnCount;
}

double
TcpSocketFactory::GetDefaultDelAckTimeout (void) const
{
  return m_defaultDelAckTimeout;
}

uint32_t
TcpSocketFactory::GetDefaultDelAckCount (void) const
{
  return m_defaultDelAckCount;
}

} // namespace ns3
