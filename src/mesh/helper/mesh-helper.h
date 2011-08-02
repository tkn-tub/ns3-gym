/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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


#ifndef MESH_HELPER_H
#define MESH_HELPER_H

#include "ns3/wifi-helper.h"
#include "ns3/mesh-stack-installer.h"

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
  /**
   * Construct a MeshHelper used to make life easier when creating 802.11s networks.
   */
  MeshHelper ();

  /**
   * Destroy a MeshHelper.
   */
  ~MeshHelper ();

  /**
   * \brief Set the helper to the default values for the MAC type,  remote
   * station manager and channel policy.
   */
  static MeshHelper Default ();

  /**
   * \param n0 the name of the attribute to set
   * \param v0 the value of the attribute to set
   * \param n1 the name of the attribute to set
   * \param v1 the value of the attribute to set
   * \param n2 the name of the attribute to set
   * \param v2 the value of the attribute to set
   * \param n3 the name of the attribute to set
   * \param v3 the value of the attribute to set
   * \param n4 the name of the attribute to set
   * \param v4 the value of the attribute to set
   * \param n5 the name of the attribute to set
   * \param v5 the value of the attribute to set
   * \param n6 the name of the attribute to set
   * \param v6 the value of the attribute to set
   * \param n7 the name of the attribute to set
   * \param v7 the value of the attribute to set
   *
   * All the attributes specified in this method should exist
   * in the requested mac.
   */
  void SetMacType (std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
                   std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                   std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                   std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
                   std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
                   std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
                   std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
                   std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());
  /**
   * \param type the type of ns3::WifiRemoteStationManager to create.
   * \param n0 the name of the attribute to set
   * \param v0 the value of the attribute to set
   * \param n1 the name of the attribute to set
   * \param v1 the value of the attribute to set
   * \param n2 the name of the attribute to set
   * \param v2 the value of the attribute to set
   * \param n3 the name of the attribute to set
   * \param v3 the value of the attribute to set
   * \param n4 the name of the attribute to set
   * \param v4 the value of the attribute to set
   * \param n5 the name of the attribute to set
   * \param v5 the value of the attribute to set
   * \param n6 the name of the attribute to set
   * \param v6 the value of the attribute to set
   * \param n7 the name of the attribute to set
   * \param v7 the value of the attribute to set
   *
   * All the attributes specified in this method should exist
   * in the requested station manager.
   */
  void
  SetRemoteStationManager (std::string type,
                           std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
                           std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                           std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                           std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
                           std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
                           std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
                           std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
                           std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());
  /**
   * Set PHY standard
   */
  void SetStandard (enum WifiPhyStandard standard);
  //void SetMeshId (std::string s); // XXX
  /** 
   *  \brief Spread/not spread frequency channels of MP interfaces. 
   * 
   *  If set to true different non-overlapping 20MHz frequency 
   *  channels will be assigned to different mesh point interfaces.
   */
  enum ChannelPolicy
  {
    SPREAD_CHANNELS,
    ZERO_CHANNEL
  };

  /**
   * \brief set the channel policy
   */
  void SetSpreadInterfaceChannels (ChannelPolicy);
  /**
   * \brief Set a number of interfaces in a mesh network
   * \param nInterfaces is the number of interfaces
   */
  void SetNumberOfInterfaces (uint32_t nInterfaces);

  /** 
   * \brief Install 802.11s mesh device & protocols on given node list
   * 
   * \param phyHelper           Wifi PHY helper
   * \param c               List of nodes to install
   * 
   * \return list of created mesh point devices, see MeshPointDevice
   */
  NetDeviceContainer
  Install (const WifiPhyHelper &phyHelper, NodeContainer c) const;
  /**
   * \param type the type of ns3::MeshStack.
   * \param n0 the name of the attribute to set
   * \param v0 the value of the attribute to set
   * \param n1 the name of the attribute to set
   * \param v1 the value of the attribute to set
   * \param n2 the name of the attribute to set
   * \param v2 the value of the attribute to set
   * \param n3 the name of the attribute to set
   * \param v3 the value of the attribute to set
   * \param n4 the name of the attribute to set
   * \param v4 the value of the attribute to set
   * \param n5 the name of the attribute to set
   * \param v5 the value of the attribute to set
   * \param n6 the name of the attribute to set
   * \param v6 the value of the attribute to set
   * \param n7 the name of the attribute to set
   * \param v7 the value of the attribute to set
   */
  void SetStackInstaller (std::string type,
                          std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
                          std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                          std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                          std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
                          std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
                          std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
                          std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
                          std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());

  /**
   * \brief Print statistics.
   */
  void Report (const ns3::Ptr<ns3::NetDevice>&, std::ostream&);

  /**
   * \brief Reset statistics.
   */
  void ResetStats (const ns3::Ptr<ns3::NetDevice>&);
private:
  /**
   * \internal
   * \returns a WifiNetDevice with ready-to-use interface
   */
  Ptr<WifiNetDevice> CreateInterface (const WifiPhyHelper &phyHelper, Ptr<Node> node, uint16_t channelId) const;
  uint32_t m_nInterfaces;
  ChannelPolicy m_spreadChannelPolicy;
  Ptr<MeshStack> m_stack;
  ObjectFactory m_stackFactory;
  ///\name Interface factory
  ///\{
  ObjectFactory m_mac;
  ObjectFactory m_stationManager;
  enum WifiPhyStandard m_standard;
  ///\}
};
} // namespace ns3

#endif /* MESH_HELPER_H */

