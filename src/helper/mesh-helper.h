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
#include "ns3/mesh-stack-installer.h"
#include "ns3/nstime.h"
#include "ns3/mesh-interface-helper.h"

namespace ns3 {

class WifiChannel;

/** 
 * \ingroup dot11s
 * 
 * \brief Helper to create IEEE 802.11s mesh networks
 */
class MeshHelper
{
public:
  MeshHelper (); 
  /** 
   *  \brief Spread/not spread frequency channels of MP interfaces. 
   * 
   *  If set to true different non-overlaping 20MHz frequency 
   *  channels will be assigned to different mesh point interfaces.
   */ 
  void SetSpreadInterfaceChannels (bool); 
  
  /** 
   * \brief Install 802.11s mesh device & protocols on given node list
   * 
   * \param phy                 Wifi PHY helper
   * \param nodes               List of nodes to install
   * \param roots               List of root mesh points
   * \param nInterfaces         Number of mesh point radio interfaces (= WiFi NICs)
   * 
   * \return list of created mesh point devices, see MeshPointDevice
   */
  NetDeviceContainer Install (const WifiPhyHelper &phyHelper, const MeshInterfaceHelper &interfaceHelper, NodeContainer c, uint32_t nInterfaces = 1) const;
  /** 
   * \brief Install 802.11s mesh device & protocols on given node
   * 
   * \param phy                 Wifi PHY helper
   * \param node                Node to install
   * \param roots               List of root mesh points
   * \param nInterfaces         Number of mesh point radio interfaces (= WiFi NICs)
   * 
   * \return list of created mesh point devices, see MeshPointDevice
   */ 
  NetDeviceContainer Install (const WifiPhyHelper &phy, const MeshInterfaceHelper &interfaceHelper, Ptr<Node> node, uint32_t nInterfaces = 1) const;
  /**
   * \param type the type of ns3::MeshStack.
   *
   * All the attributes specified in this method should exist
   * in the requested station manager.
   */
  void SetStackInstaller (std::string type);
  void Report (const ns3::Ptr<ns3::NetDevice>&, std::ostream&);
  void ResetStats (const ns3::Ptr<ns3::NetDevice>&);
private:
  bool m_spreadInterfaceChannels;
  Ptr<MeshStack> m_stack;
  ObjectFactory m_stackFactory;
};
} //namespace ns3

#endif /* _MESHWIFIHELPER_H */

