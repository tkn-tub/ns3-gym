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
class AirtimeLinkMetricCalculator : public Object
{
  public:
    static TypeId GetTypeId ();
    uint32_t CalculateMetric(Mac48Address peerAddress, Ptr<MeshWifiInterfaceMac> mac);
  private:
    
    uint32_t m_overheadNanosec;
    uint32_t m_testLength;
    uint16_t m_headerLength;
    uint16_t m_meshHeaderLength;
};
} //namespace dot11s
} //namespace ns3
#endif
