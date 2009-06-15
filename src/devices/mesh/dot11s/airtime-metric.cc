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

#include "airtime-metric.h"
#include "ns3/wifi-remote-station-manager.h"
#include "ns3/wifi-mode.h"
namespace ns3 {
namespace dot11s {
NS_OBJECT_ENSURE_REGISTERED (AirtimeLinkMetricCalculator);
TypeId
AirtimeLinkMetricCalculator::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::dot11s::AirtimeLinkMetricCalculator")
    .SetParent<Object> ()
    .AddConstructor<AirtimeLinkMetricCalculator> ()
    .AddAttribute ("overheadNanosec",
        "Overhead expressed in nanoseconds:DIFS+ 2* SIFS + 2* PREAMBLE + 2* ACK",
        UintegerValue (108000),
        MakeUintegerAccessor (&AirtimeLinkMetricCalculator::m_overheadNanosec),
        MakeUintegerChecker<uint32_t> (1)
        )
    .AddAttribute ("testLengt",
        "Rate should be estimated using test length.",
        UintegerValue (1024),
        MakeUintegerAccessor (&AirtimeLinkMetricCalculator::m_testLength),
        MakeUintegerChecker<uint16_t> (1)
        )
    .AddAttribute ("dot11HeaderLengt",
        "Length of the 802.11 header",
        UintegerValue (36),
        MakeUintegerAccessor (&AirtimeLinkMetricCalculator::m_headerLength),
        MakeUintegerChecker<uint16_t> (0)
        )
    .AddAttribute ("dot11sHeaderLength",
        "Length of the mesh header",
        UintegerValue (6),
        MakeUintegerAccessor (&AirtimeLinkMetricCalculator::m_meshHeaderLength),
        MakeUintegerChecker<uint16_t> (0)
        );
  return tid;
} 

uint32_t
AirtimeLinkMetricCalculator::CalculateMetric(Mac48Address peerAddress, Ptr<MeshWifiInterfaceMac> mac)
{
  /* Airtime link metric is defined in 11B.10 of 802.11s Draft D3.0 as:
   * 
   * airtime = (O + Bt/r)* (1 + average retry counter), where 
   * o  -- the PHY dependent channel access which includes frame headers, training sequences, 
   *       access protocol frames, etc. 
   * bt -- the test packet length in bits (8192 by default), 
   * r  -- the current bitrate of the packet,
   *  
   * Final result is expressed in units of 0.01 Time Unit = 10.24 us (as required by 802.11s draft)
   */
  
  const double sec2ns = 1e9;   // seconds -> nanoseconds conversion factor
  const double ns2tu  = 10240; // nanoseconds -> 0.01 TU conversion factor
  
  WifiRemoteStation * station = mac->GetStationManager ()->Lookup(peerAddress);
  NS_ASSERT(station != 0);
  Ptr<Packet> test_frame = Create<Packet> (m_testLength+ m_headerLength+ m_meshHeaderLength);
  uint32_t rate = station->GetDataMode(test_frame, m_testLength + m_headerLength + m_meshHeaderLength).GetDataRate ();
  uint32_t payload_nanosec = (uint32_t) ((double) ((m_testLength + m_meshHeaderLength) * 8 /*octets -> bits*/) * sec2ns / ((double)rate));
  uint32_t header_nanosec = (uint32_t)(
      (double) (m_headerLength * 8 /*octets -> bits*/ * sec2ns) / ((double) mac->GetStationManager () -> GetBasicMode (0).GetDataRate ())
      );
  uint32_t metric = (uint32_t) (
      ((double) (payload_nanosec + header_nanosec + m_overheadNanosec)) / ns2tu * (station->GetAvgSlrc () + 1)
      );
  return metric;
}
} //namespace dot11s
} //namespace ns3
