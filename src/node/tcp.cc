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
#include "tcp.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (Tcp);

TypeId
Tcp::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Tcp")
    .SetParent<SocketFactory> ()
   .AddAttribute ("TcpDefaultSegmentSize",
                  "Default TCP maximum segment size in bytes (may be adjusted based on MTU discovery)",
                  UintegerValue (536),
                  MakeUintegerAccessor (&Tcp::m_defaultSegSize),
                  MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("TcpDefaultAdvertisedWindowSize",
                   "Default TCP advertised window size (bytes)",
                   UintegerValue (0xffff),
                   MakeUintegerAccessor (&Tcp::m_defaultAdvWin),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("TcpDefaultSlowStartThreshold",
                   "Default TCP slow start threshold (bytes)",
                   UintegerValue (0xffff),
                   MakeUintegerAccessor (&Tcp::m_defaultSsThresh),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("TcpDefaultTxBufferSize",
                   "Default TCP maximum transmit buffer size (bytes)",
                   UintegerValue (0xffffffffl),
                   MakeUintegerAccessor (&Tcp::m_defaultTxBuffer),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("TcpDefaultRxBufferSize",
                   "Default TCP maximum receive buffer size (bytes)",
                   UintegerValue (0xffffffffl),
                   MakeUintegerAccessor (&Tcp::m_defaultRxBuffer),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("TcpDefaultInitialCongestionWindowSize",
                   "Default TCP initial congestion window size (segments)",
                   UintegerValue (1),
                   MakeUintegerAccessor (&Tcp::m_defaultInitialCwnd),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("TcpDefaultConnTimeout",
                   "Default TCP retransmission timeout when opening connection (seconds)",
                   UintegerValue (3),
                   MakeUintegerAccessor (&Tcp::m_defaultConnTimeout),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("TcpDefaultConnCount",
                   "Default number of connection attempts (SYN retransmissions) before returning failure",
                   UintegerValue (6),
                   MakeUintegerAccessor (&Tcp::m_defaultConnCount),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("TcpDefaultDelAckTimeout",
                   "Default timeout value for TCP delayed acks, in seconds",
                   DoubleValue (0.2),
                   MakeDoubleAccessor (&Tcp::m_defaultDelAckTimeout),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("TcpDefaultDelAckCount",
                   "Default number of packets to wait before sending a TCP ack",
                   UintegerValue (2),
                   MakeUintegerAccessor (&Tcp::m_defaultDelAckCount),
                   MakeUintegerChecker<uint32_t> ())
     ;
  return tid;
}

uint32_t 
Tcp::GetDefaultSegSize (void) const
{
  return m_defaultSegSize;
}
uint32_t 
Tcp::GetDefaultAdvWin (void) const
{
  return m_defaultAdvWin;
}
uint32_t 
Tcp::GetDefaultSsThresh (void) const
{
  return m_defaultSsThresh;
}
uint32_t 
Tcp::GetDefaultTxBuffer (void) const
{
  return m_defaultTxBuffer;
}
uint32_t 
Tcp::GetDefaultRxBuffer (void) const
{
  return m_defaultRxBuffer;
}
uint32_t 
Tcp::GetDefaultInitialCwnd (void) const
{
  return m_defaultInitialCwnd;
}
uint32_t 
Tcp::GetDefaultConnTimeout (void) const
{
  return m_defaultConnTimeout;
}
uint32_t 
Tcp::GetDefaultConnCount (void) const
{
  return m_defaultConnCount;
}

double
Tcp::GetDefaultDelAckTimeout (void) const
{
  return m_defaultDelAckTimeout;
}

uint32_t
Tcp::GetDefaultDelAckCount (void) const
{
  return m_defaultDelAckCount;
}

} // namespace ns3
