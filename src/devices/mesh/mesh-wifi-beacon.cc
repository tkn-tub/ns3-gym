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
 * Author: Pavel Boyko <boyko@iitp.ru>
 */

#include "ns3/mesh-wifi-beacon.h"
#include <algorithm>

namespace ns3 {
  
MeshWifiBeacon::MeshWifiBeacon(Ssid ssid, SupportedRates rates, uint64_t us)
{
  m_header.SetSsid (ssid);
  m_header.SetSupportedRates (rates);
  m_header.SetBeaconIntervalUs (us);
}

void MeshWifiBeacon::AddInformationElement(const WifiInformationElement * ie)
{
  m_elements.push_back(ie);
}

namespace {
/// aux sorter
struct IEComparator
{
  bool operator()(const WifiInformationElement * a, const WifiInformationElement * b) const
  {
    return (*a < *b);
  }
};
}

Ptr<Packet> MeshWifiBeacon::CreatePacket()
{
  Ptr<Packet> packet = Create<Packet> ();
  
  std::sort(m_elements.begin(), m_elements.end(), IEComparator());
  
  std::vector<const WifiInformationElement *>::const_reverse_iterator i;
  for(i = m_elements.rbegin(); i != m_elements.rend(); ++i)
  {
    packet->AddHeader(**i);
  }
  
  packet->AddHeader(Header());
  
  return packet;
}
  
} // namespace 

