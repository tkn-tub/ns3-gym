/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */

#ifndef MESH_INTERFACE_HELPER_H
#define MESH_INTERFACE_HELPER_H
#include "ns3/wifi-helper.h"
#include "ns3/wifi-net-device.h"
#include "ns3/mesh-wifi-interface-mac.h"
namespace ns3 {

class MeshInterfaceHelper : public WifiMacHelper
{
public:
  MeshInterfaceHelper ();
  virtual ~MeshInterfaceHelper ();
  /**
   * Create a mac helper in a default working state.
   */
  static MeshInterfaceHelper Default (void);
  /**
   * \param type the type of ns3::WifiMac to create.
   * \param n%d the name of the attribute to set
   * \param v%d the value of the attribute to set
   *
   * All the attributes specified in this method should exist
   * in the requested mac.
   */
  void SetType (std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
                std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
                std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
                std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
                std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
                std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());
  /**
   * \param type the type of ns3::WifiMac to create.
   * \param n%d the name of the attribute to set
   * \param v%d the value of the attribute to set
   */
  void SetBeParameters ( std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
                         std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                         std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                         std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue ());
  /**
   * \param type the type of ns3::WifiMac to create.
   * \param n%d the name of the attribute to set
   * \param v%d the value of the attribute to set
   */
  void SetVoParameters ( std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
                         std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                         std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                         std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue ());
  /**
   * \param type the type of ns3::WifiRemoteStationManager to create.
   * \param n%d the name of the attribute to set
   * \param v%d the value of the attribute to set
   *
   * All the attributes specified in this method should exist
   * in the requested station manager.
   */
  void SetRemoteStationManager (std::string type,
                                std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
                                std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                                std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                                std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
                                std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
                                std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
                                std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
                                std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());
  void SetMeshId (std::string s);
  /**
   * \returns a WifiNetDevice with ready-to-use interface
   */
  Ptr<WifiNetDevice> CreateInterface (const WifiPhyHelper &phyHelper, Ptr<Node> node, uint16_t channelId) const;
  /**
   * \brief Report statistics:
   */
  static void Report (const Ptr<WifiNetDevice>& device, std::ostream& os);
  static void ResetStats (const Ptr<WifiNetDevice>& device);
private:
  /**
   * \returns a newly-created MAC object.
   *
   * This method implements the pure virtual method defined in \ref ns3::WifiMacHelper.
   */
  Ptr<WifiMac> Create (void) const;

  ObjectFactory m_mac;
  ObjectFactory m_Be;
  ObjectFactory m_Vo;
  ObjectFactory m_stationManager;
};

} //namespace ns3

#endif /* MESH_INTERFACE_HELPER */
