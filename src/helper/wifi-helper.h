/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef WIFI_HELPER_H
#define WIFI_HELPER_H

#include <string>
#include "ns3/attribute.h"
#include "ns3/object-factory.h"
#include "ns3/node-container.h"
#include "ns3/net-device-container.h"

namespace ns3 {

class WifiChannel;

/**
 * \brief helps to create WifiNetDevice objects
 *
 * This class can help to create a large set of similar
 * WifiNetDevice objects and to configure a large set of
 * their parameters during creation.
 */
class WifiHelper
{
public:
  WifiHelper ();

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
  void SetRemoteStationManager (std::string type,
				std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
				std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
				std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
				std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
				std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
				std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
				std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
				std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());

  /**
   * \param type the type of ns3::WifiMac to create.
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
  void SetMac (std::string type,
	       std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
	       std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
	       std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
	       std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
	       std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
	       std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
	       std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
	       std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());

  /**
   * \param name the name of the attribute to set
   * \param value the value of the attribute to set
   *
   */
  void SetPhyAttribute (std::string n0, const AttributeValue &v0);



  /**
   * \param filename filename prefix to use for pcap files.
   * \param nodeid the id of the node to generate pcap output for.
   * \param deviceid the id of the device to generate pcap output for.
   *
   * Generate a pcap file which contains the link-level data observed
   * by the specified deviceid within the specified nodeid. The pcap
   * data is stored in the file prefix-nodeid-deviceid.pcap.
   *
   * This method should be invoked after the network topology has 
   * been fully constructed.
   */
  static void EnablePcap (std::string filename, uint32_t nodeid, uint32_t deviceid);
  /**
   * \param filename filename prefix to use for pcap files.
   * \param d container of devices of type ns3::WifiNetDevice
   *
   * Enable pcap output on each input device which is of the
   * ns3::WifiNetDevice type.
   */
  static void EnablePcap (std::string filename, NetDeviceContainer d);
  /**
   * \param filename filename prefix to use for pcap files.
   * \param n container of nodes.
   *
   * Enable pcap output on each device which is of the
   * ns3::WifiNetDevice type and which is located in one of the 
   * input nodes.
   */
  static void EnablePcap (std::string filename, NodeContainer n);
  /**
   * \param filename filename prefix to use for pcap files.
   *
   * Enable pcap output on each device which is of the
   * ns3::WifiNetDevice type
   */
  static void EnablePcapAll (std::string filename);

  /**
   * \param os output stream
   * \param nodeid the id of the node to generate ascii output for.
   * \param deviceid the id of the device to generate ascii output for.
   *
   * Enable ascii output on the specified deviceid within the
   * specified nodeid if it is of type ns3::WifiNetDevice and dump 
   * that to the specified stdc++ output stream.
   */
  static void EnableAscii (std::ostream &os, uint32_t nodeid, uint32_t deviceid);
  /**
   * \param os output stream
   * \param d device container
   *
   * Enable ascii output on each device which is of the
   * ns3::WifiNetDevice type and which is located in the input
   * device container and dump that to the specified
   * stdc++ output stream.
   */
  static void EnableAscii (std::ostream &os, NetDeviceContainer d);
  /**
   * \param os output stream
   * \param n node container
   *
   * Enable ascii output on each device which is of the
   * ns3::WifiNetDevice type and which is located in one
   * of the input node and dump that to the specified
   * stdc++ output stream.
   */
  static void EnableAscii (std::ostream &os, NodeContainer n);
  /**
   * \param os output stream
   *
   * Enable ascii output on each device which is of the
   * ns3::WifiNetDevice type and dump that to the specified
   * stdc++ output stream.
   */
  static void EnableAsciiAll (std::ostream &os);

  /**
   * \param c a set of nodes
   *
   * This method creates a simple ns3::WifiChannel (with a default
   * ns3::PropagationLossModel and ns3::PropagationDelayModel) and 
   * creates, for each of the input nodes, a new ns3::WifiNetDevice 
   * attached to this shared channel. Each ns3::WifiNetDevice is also
   * configured with an ns3::WifiRemoteStationManager, ns3::WifiMac, and,
   * ns3::WifiPhy, all of which are created based on the user-specified
   * attributes specified in WifiHelper::SetRemoteStationManager, 
   * WifiHelper::SetMac, and, WifiHelper::SetPhy.
   */
  NetDeviceContainer Install (NodeContainer c) const;
  /**
   * \param channel a channel to use
   * \param c a set of nodes
   *
   * For each of the input nodes, a new ns3::WifiNetDevice is attached 
   * to the shared input channel. Each ns3::WifiNetDevice is also
   * configured with an ns3::WifiRemoteStationManager, ns3::WifiMac, and,
   * ns3::WifiPhy, all of which are created based on the user-specified
   * attributes specified in WifiHelper::SetRemoteStationManager, 
   * WifiHelper::SetMac, and, WifiHelper::SetPhy.
   *
   * The user is expected to attach to the input channel a proper 
   * ns3::PropagationLossModel, and ns3::PropagationDelayModel.
   */
  NetDeviceContainer Install (NodeContainer c, Ptr<WifiChannel> channel) const;

private:
  ObjectFactory m_stationManager;
  ObjectFactory m_mac;
  UnsafeAttributeList m_phyAttributes;
};

} // namespace ns3

#endif /* WIFI_HELPER_H */
