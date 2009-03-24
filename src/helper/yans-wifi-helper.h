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
#ifndef YANS_WIFI_HELPER_H
#define YANS_WIFI_HELPER_H

#include "wifi-helper.h"
#include "ns3/yans-wifi-channel.h"

namespace ns3 {

/**
 * \brief manage and create wifi channel objects for the yans model.
 *
 * The intent of this class is to make it easy to create a channel object
 * which implements the yans channel model. The yans channel model is described
 * in "Yet Another Network Simulator", http://cutebugs.net/files/wns2-yans.pdf
 */
class YansWifiChannelHelper
{
public:
  /**
   * Create a channel helper without any parameter set. The user must set
   * them all to be able to call Create later.
   */
  YansWifiChannelHelper ();

  /**
   * Create a channel helper in a default working state. By default, we create
   * a channel model with a propagation delay equal to a constant, the speed of light,
   * and a propagation loss based on a log distance model with a reference loss of 46.6777 dB
   * at reference distance of 1m.
   */
  static YansWifiChannelHelper Default (void);

  /**
   * \param name the name of the model to add
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
   * Add a propagation loss model to the set of currently-configured loss models.
   * This method is additive to allow you to construct complex propagation loss models
   * such as a log distance + jakes model, etc.
   */
  void AddPropagationLoss (std::string name,
			   std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
			   std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
			   std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
			   std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
			   std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
			   std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
			   std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
			   std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());
  /**
   * \param name the name of the model to set
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
   * Configure a propagation delay for this channel.
   */
  void SetPropagationDelay (std::string name,
			    std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
			    std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
			    std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
			    std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
			    std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
			    std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
			    std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
			    std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());

  /**
   * \returns a new channel
   *
   * Create a channel based on the configuration parameters set previously.
   */
  Ptr<YansWifiChannel> Create (void) const;

private:
  std::vector<ObjectFactory> m_propagationLoss;
  ObjectFactory m_propagationDelay;
};

/**
 * \brief Make it easy to create and manager PHY objects for the yans model.
 *
 * The yans PHY model is described in "Yet Another Network Simulator", 
 * http://cutebugs.net/files/wns2-yans.pdf
 *
 * The Pcap and ascii traces generated by the EnableAscii and EnablePcap methods defined
 * in this class correspond to PHY-level traces. 
 */
class YansWifiPhyHelper : public WifiPhyHelper
{
public:
  /**
   * Create a phy helper without any parameter set. The user must set
   * them all to be able to call Install later.
   */
  YansWifiPhyHelper ();

  /**
   * Create a phy helper in a default working state.
   */
  static YansWifiPhyHelper Default (void);

  /**
   * \param channel the channel to associate to this helper
   *
   * Every PHY created by a call to Install is associated to this channel.
   */
  void SetChannel (Ptr<YansWifiChannel> channel);
  /**
   * \param channelName The name of the channel to associate to this helper
   *
   * Every PHY created by a call to Install is associated to this channel.
   */
  void SetChannel (std::string channelName);
  /**
   * \param name the name of the attribute to set
   * \param v the value of the attribute
   *
   * Set an attribute of the underlying PHY object.
   */
  void Set (std::string name, const AttributeValue &v);
  /**
   * \param name the name of the error rate model to set.
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
   * Set the error rate model and its attributes to use when Install is called.
   */
  void SetErrorRateModel (std::string name,
                          std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
                          std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                          std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                          std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
                          std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
                          std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
                          std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
                          std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());

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
   * \param nd Net device on which you want to enable tracing.
   *
   * Enable pcap output on each input device which is of the
   * ns3::WifiNetDevice type.
   */
  static void EnablePcap (std::string filename, Ptr<NetDevice> nd);

  /**
   * \param filename filename prefix to use for pcap files.
   * \param ndName Name of net device on which you want to enable tracing.
   *
   * Enable pcap output on each input device which is of the
   * ns3::WifiNetDevice type.
   */
  static void EnablePcap (std::string filename, std::string ndName);

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
  /**
   * \param node the node on which we wish to create a wifi PHY
   * \param device the device within which this PHY will be created
   * \returns a newly-created PHY object.
   *
   * This method implements the pure virtual method defined in \ref ns3::WifiPhyHelper.
   */
  virtual Ptr<WifiPhy> Create (Ptr<Node> node, Ptr<WifiNetDevice> device) const;

  ObjectFactory m_phy;
  ObjectFactory m_errorRateModel;
  Ptr<YansWifiChannel> m_channel;
};

} // namespace ns3

#endif /* YANS_WIFI_HELPER_H */
