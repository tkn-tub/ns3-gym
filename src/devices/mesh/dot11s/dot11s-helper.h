/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 *         Pavel Boyko <boyko@iitp.ru>
 */


#ifndef _MESHWIFIHELPER_H
#define _MESHWIFIHELPER_H

#include "ns3/wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/nstime.h"
#include "ns3/peer-management-protocol.h"
#include "ns3/hwmp-protocol.h"

namespace ns3 {
namespace dot11s {

class WifiChannel;

/** 
 * \ingroup dot11s
 * 
 * \brief Helper to create IEEE 802.11s mesh networks
 */
class MeshWifiHelper
{
public:
  MeshWifiHelper (); 
  /// Set mesh SSID
  void SetSsid (const Ssid  &);
  /// Set maximum random start delay
  void SetRandomStartDelay (Time delay);
  /** 
   * \brief Install 802.11s mesh device & protocols on given node
   * 
   * \param phy                 Wifi PHY helper
   * \param nodes               List of nodes to install
   * \param nInterfaces         Number of mesh point radio interfaces (= WiFi NICs)
   * 
   * \return list of created mesh point devices, see MeshPointDevice
   */
  NetDeviceContainer Install (const WifiPhyHelper &phyHelper, NodeContainer c, uint32_t nInterfaces = 1) const;
  /** 
   * \brief Install 802.11s mesh device & protocols on given node
   * 
   * \param phy                 Wifi PHY helper
   * \param node                Node to install
   * \param nInterfaces         Number of mesh point radio interfaces (= WiFi NICs)
   * 
   * \return list of created mesh point devices, see MeshPointDevice
   */ 
  NetDeviceContainer Install (const WifiPhyHelper &phy, Ptr<Node> node, uint32_t nInterfaces = 1) const;
  
private:
  Ssid m_ssid;
  Time m_randomStartDelay;
  
  /// Create single mesh interface NIC
  Ptr<WifiNetDevice> CreateInterface (const WifiPhyHelper &phyHelper, Ptr<Node> node) const;
  
};
} // namespace dot11s
} //namespace ns3

#endif /* _MESHWIFIHELPER_H */

