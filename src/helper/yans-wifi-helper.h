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
#ifndef YANS_WIFI_PHY_HELPER_H
#define YANS_WIFI_PHY_HELPER_H

#include "wifi-helper.h"
#include "ns3/yans-wifi-channel.h"

namespace ns3 {

class YansWifiChannelHelper
{
public:
  YansWifiChannelHelper ();

  static YansWifiChannelHelper Default (void);

  void AddPropagationLoss (std::string name,
			   std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
			   std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
			   std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
			   std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
			   std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
			   std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
			   std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
			   std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());
  void SetPropagationDelay (std::string name,
			    std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
			    std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
			    std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
			    std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
			    std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
			    std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
			    std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
			    std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());

  Ptr<YansWifiChannel> Create (void) const;

private:
  std::vector<ObjectFactory> m_propagationLoss;
  ObjectFactory m_propagationDelay;
};

class YansWifiPhyHelper : public WifiPhyHelper
{
public:
  YansWifiPhyHelper ();

  static YansWifiPhyHelper Default (void);

  void SetChannel (Ptr<YansWifiChannel> channel);
  void Set (std::string name, const AttributeValue &v);
  void SetErrorRateModel (std::string name,
                          std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
                          std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                          std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                          std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
                          std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
                          std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
                          std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
                          std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());

  virtual Ptr<WifiPhy> Create (Ptr<Node> node, Ptr<WifiNetDevice> device) const;

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

private:
  ObjectFactory m_phy;
  ObjectFactory m_errorRateModel;
  Ptr<YansWifiChannel> m_channel;
};

} // namespace ns3

#endif /* YANS_WIFI_PHY_HELPER_H */
