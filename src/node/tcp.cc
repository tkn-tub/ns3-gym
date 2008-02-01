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

namespace ns3 {

NumericDefaultValue<uint32_t>       Tcp::defaultSegSize
("TcpDefaultSegmentSize",
"Default TCP maximum segment size in bytes (may be adjusted based on MTU discovery)",
536);

NumericDefaultValue<uint32_t>       Tcp::defaultAdvWin
("TcpDefaultAdvertisedWindowSize",
"Default TCP advertised window size (bytes)",
0xffff);

NumericDefaultValue<uint32_t>       Tcp::defaultSSThresh
("TcpDefaultSlowStartThreshold",
"Default TCP slow start threshold (bytes)",
0xffff);

NumericDefaultValue<uint32_t>       Tcp::defaultTxBuffer
("TcpDefaultTxBufferSize",
"Default TCP maximum transmit buffer size (bytes)",
0xffffffffl);

NumericDefaultValue<uint32_t>       Tcp::defaultRxBuffer
("TcpDefaultRxBufferSize",
"Default TCP maximum receive buffer size (bytes)",
0xffffffffl);

NumericDefaultValue<uint32_t>       Tcp::defaultInitialCWnd
("TcpDefaultInitialCongestionWindowSize",
"Default TCP initial congestion window size (segments)",
1);

NumericDefaultValue<uint32_t>       Tcp::defaultConnTimeout
("TcpDefaultConnTimeout",
"Default TCP retransmission timeout when opening connection (seconds)",
6);

NumericDefaultValue<uint32_t>       Tcp::defaultConnCount
("TcpDefaultConnCount",
"Default number of connection attempts (SYN retransmissions) before returning failure",
3);

NS_OBJECT_ENSURE_REGISTERED (Tcp);

TypeId
Tcp::GetTypeId (void)
{
  static TypeId tid = TypeId ("Tcp")
    .SetParent<SocketFactory> ();
  return tid;
}


} // namespace ns3
