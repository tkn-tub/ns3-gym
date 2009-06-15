/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/* 
 * Copyright (c) 2009 IITP RAS
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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 */

#ifndef AIRTIME_METRIC_H
#define AIRTIME_METRIC_H
#include "ns3/mesh-wifi-interface-mac.h"
namespace ns3 {
namespace dot11s {
/**
 * \ingroup dot11s
 * 
 * \brief airtime link metric calculator
 * 
 * \details Airtime link metric is defined in 11B.10 of 802.11s Draft D3.0 as:
 * 
 * airtime = (O + Bt/r)* (1 + average retry counter), where 
 * 
 * o  -- the PHY dependent channel access which includes frame headers, training sequences, 
 *       access protocol frames, etc. 
 * bt -- the test packet length in bits (8192 by default), 
 * r  -- the current bitrate of the packet,
 *  
 * Final result is expressed in units of 0.01 Time Unit = 10.24 us (as required by 802.11s draft)
 */
class AirtimeLinkMetricCalculator : public Object
{
  public:
    static TypeId GetTypeId ();
    uint32_t CalculateMetric(Mac48Address peerAddress, Ptr<MeshWifiInterfaceMac> mac);
  private:
    //\brief Overhead expressed in nanoseconds:DIFS+ 2* SIFS + 2*PREAMBLE + 2* ACK
    uint32_t m_overheadNanosec;
    ///\brief Bt value
    uint32_t m_testLength;
    ///\brief header length (used in overhead)
    uint16_t m_headerLength;
    ///\brief meshHeader length (6 octets usually)
    uint16_t m_meshHeaderLength;
};
} //namespace dot11s
} //namespace ns3
#endif
