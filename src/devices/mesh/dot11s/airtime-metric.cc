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
uint32_t
AirtimeLinkMetricCalculator::CalculateMetric(Mac48Address peerAddress, Ptr<MeshWifiInterfaceMac> mac)
{
  WifiRemoteStation * station = mac->GetStationManager ()->Lookup(peerAddress);
  Ptr<Packet> test_frame = Create<Packet> (test_length);
  uint32_t rate = station->GetDataMode(test_frame, test_length+header_length).GetDataRate ();
  uint32_t payload_nanosec = (uint32_t)((double)(test_length*8)*1e9 / ((double)rate));
  uint32_t metric = (uint32_t)(((double)(payload_nanosec + overhead_nanosec))/102.4);
  return metric;
}
} //namespace dot11s
} //namespace ns3
