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
#ifndef CSMA_HELPER_H
#define CSMA_HELPER_H

#include <string>
#include <ostream>
#include "ns3/attribute.h"
#include "ns3/object-factory.h"
#include "ns3/net-device-container.h"
#include "ns3/node-container.h"
#include "ns3/csma-channel.h"
#include "ns3/deprecated.h"

namespace ns3 {

class Packet;
class PcapWriter;

/**
 * \brief build a set of CsmaNetDevice objects
 */
class CsmaHelper
{
public:
  CsmaHelper ();

  /**
   * \param type the type of queue
   * \param n1 the name of the attribute to set on the queue
   * \param v1 the value of the attribute to set on the queue
   * \param n2 the name of the attribute to set on the queue
   * \param v2 the value of the attribute to set on the queue
   * \param n3 the name of the attribute to set on the queue
   * \param v3 the value of the attribute to set on the queue
   * \param n4 the name of the attribute to set on the queue
   * \param v4 the value of the attribute to set on the queue
   *
   * Set the type of queue to create and associated to each
   * CsmaNetDevice created through CsmaHelper::Install.
   */
  void SetQueue (std::string type,
                 std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                 std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                 std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
                 std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue ());

  /**
   * \param n1 the name of the attribute to set
   * \param v1 the value of the attribute to set
   *
   * Set these attributes on each ns3::CsmaNetDevice created
   * by CsmaHelper::Install
   */
  void SetDeviceAttribute (std::string n1, const AttributeValue &v1);

  /**
   * \param n1 the name of the attribute to set
   * \param v1 the value of the attribute to set
   *
   * Set these attributes on each ns3::CsmaChannel created
   * by CsmaHelper::Install
   */
  void SetChannelAttribute (std::string n1, const AttributeValue &v1);

  void SetDeviceParameter (std::string n1, const AttributeValue &v1) NS_DEPRECATED;
  void SetChannelParameter (std::string n1, const AttributeValue &v1) NS_DEPRECATED;

  /**
   * \param filename filename prefix to use for pcap files.
   * \param nodeid the id of the node to generate pcap output for.
   * \param deviceid the id of the device to generate pcap output for.
   * \param promiscuous If true capture all possible packets available at the device.
   *
   * Generate a pcap file which contains the link-level data observed
   * by the specified deviceid within the specified nodeid. The pcap
   * data is stored in the file prefix-nodeid-deviceid.pcap.
   *
   * This method should be invoked after the network topology has 
   * been fully constructed.
   */
  static void EnablePcap (std::string filename, uint32_t nodeid, uint32_t deviceid, bool promiscuous);

  /**
   * \param filename filename prefix to use for pcap files.
   * \param nd Net device in which you want to enable tracing.
   * \param promiscuous If true capture all possible packets available at the device.
   *
   * Enable pcap output the indicated net device.
   */
  static void EnablePcap (std::string filename, Ptr<NetDevice> nd, bool promiscuous);

  /**
   * \param filename filename prefix to use for pcap files.
   * \param ndName The name of the net device in which you want to enable tracing.
   * \param promiscuous If true capture all possible packets available at the device.
   *
   * Enable pcap output the indicated net device.
   */
  static void EnablePcap (std::string filename, std::string ndName, bool promiscuous);

  /**
   * \param filename filename prefix to use for pcap files.
   * \param d container of devices of type ns3::CsmaNetDevice
   * \param promiscuous If true capture all possible packets available at the device.
   *
   * Enable pcap output on each input device which is of the ns3::CsmaNetDevice type.
   */
  static void EnablePcap (std::string filename, NetDeviceContainer d, bool promiscuous);

  /**
   * \param filename filename prefix to use for pcap files.
   * \param n container of nodes.
   * \param promiscuous If true capture all possible packets available at the device.
   *
   * Enable pcap output on each device which is of the
   * ns3::CsmaNetDevice type and which is located in one of the 
   * input nodes.
   */
  static void EnablePcap (std::string filename, NodeContainer n, bool promiscuous);
  /**
   * \param filename filename prefix to use for pcap files.
   * \param promiscuous If true capture all possible packets available at the device.
   *
   * Enable pcap output on each device which is of the
   * ns3::CsmaNetDevice type
   */
  static void EnablePcapAll (std::string filename, bool promiscuous);

  /**
   * \param os output stream
   * \param nodeid the id of the node to generate ascii output for.
   * \param deviceid the id of the device to generate ascii output for.
   *
   * Enable ascii output on the specified deviceid within the
   * specified nodeid if it is of type ns3::CsmaNetDevice and dump 
   * that to the specified stdc++ output stream.
   */
  static void EnableAscii (std::ostream &os, uint32_t nodeid, uint32_t deviceid);
  /**
   * \param os output stream
   * \param d device container
   *
   * Enable ascii output on each device which is of the
   * ns3::CsmaNetDevice type and which is located in the input
   * device container and dump that to the specified
   * stdc++ output stream.
   */
  static void EnableAscii (std::ostream &os, NetDeviceContainer d);
  /**
   * \param os output stream
   * \param n node container
   *
   * Enable ascii output on each device which is of the
   * ns3::CsmaNetDevice type and which is located in one
   * of the input node and dump that to the specified
   * stdc++ output stream.
   */
  static void EnableAscii (std::ostream &os, NodeContainer n);
  /**
   * \param os output stream
   *
   * Enable ascii output on each device which is of the
   * ns3::CsmaNetDevice type and dump that to the specified
   * stdc++ output stream.
   */
  static void EnableAsciiAll (std::ostream &os);

  /**
   * This method creates an ns3::CsmaChannel with the attributes configured by
   * CsmaHelper::SetChannelAttribute, an ns3::CsmaNetDevice with the attributes
   * configured by CsmaHelper::SetDeviceAttribute and then adds the device
   * to the node and attaches the channel to the device.
   *
   * \param node The node to install the device in
   * \returns A containter holding the added net device.
   */
  NetDeviceContainer Install (Ptr<Node> node) const;

  /**
   * This method creates an ns3::CsmaChannel with the attributes configured by
   * CsmaHelper::SetChannelAttribute, an ns3::CsmaNetDevice with the attributes
   * configured by CsmaHelper::SetDeviceAttribute and then adds the device
   * to the node and attaches the channel to the device.
   *
   * \param name The name of the node to install the device in
   * \returns A containter holding the added net device.
   */
  NetDeviceContainer Install (std::string name) const;

  /**
   * This method creates an ns3::CsmaNetDevice with the attributes configured by
   * CsmaHelper::SetDeviceAttribute and then adds the device to the node and 
   * attaches the provided channel to the device.
   *
   * \param node The node to install the device in
   * \param channel The channel to attach to the device.
   * \returns A containter holding the added net device.
   */
  NetDeviceContainer Install (Ptr<Node> node, Ptr<CsmaChannel> channel) const;

  /**
   * This method creates an ns3::CsmaNetDevice with the attributes configured by
   * CsmaHelper::SetDeviceAttribute and then adds the device to the node and 
   * attaches the provided channel to the device.
   *
   * \param node The node to install the device in
   * \param channelName The name of the channel to attach to the device.
   * \returns A containter holding the added net device.
   */
  NetDeviceContainer Install (Ptr<Node> node, std::string channelName) const;

  /**
   * This method creates an ns3::CsmaNetDevice with the attributes configured by
   * CsmaHelper::SetDeviceAttribute and then adds the device to the node and 
   * attaches the provided channel to the device.
   *
   * \param nodeName The name of the node to install the device in
   * \param channel The chanel to attach to the device.
   * \returns A containter holding the added net device.
   */
  NetDeviceContainer Install (std::string nodeName, Ptr<CsmaChannel> channel) const;

  /**
   * This method creates an ns3::CsmaNetDevice with the attributes configured by
   * CsmaHelper::SetDeviceAttribute and then adds the device to the node and 
   * attaches the provided channel to the device.
   *
   * \param nodeName The name of the node to install the device in
   * \param channelName The name of the chanel to attach to the device.
   * \returns A containter holding the added net device.
   */
  NetDeviceContainer Install (std::string nodeName, std::string channelName) const;

  /**
   * This method creates an ns3::CsmaChannel with the attributes configured by
   * CsmaHelper::SetChannelAttribute.  For each Ptr<node> in the provided
   * container: it creates an ns3::CsmaNetDevice (with the attributes 
   * configured by CsmaHelper::SetDeviceAttribute); adds the device to the 
   * node; and attaches the channel to the device.
   *
   * \param c The NodeContainer holding the nodes to be changed.
   * \returns A containter holding the added net devices.
   */
  NetDeviceContainer Install (const NodeContainer &c) const;

  /**
   * For each Ptr<node> in the provided container, this method creates an 
   * ns3::CsmaNetDevice (with the attributes configured by 
   * CsmaHelper::SetDeviceAttribute); adds the device to the node; and attaches 
   * the provided channel to the device.
   *
   * \param c The NodeContainer holding the nodes to be changed.
   * \param channel The channel to attach to the devices.
   * \returns A containter holding the added net devices.
   */
  NetDeviceContainer Install (const NodeContainer &c, Ptr<CsmaChannel> channel) const;

  /**
   * For each Ptr<node> in the provided container, this method creates an 
   * ns3::CsmaNetDevice (with the attributes configured by 
   * CsmaHelper::SetDeviceAttribute); adds the device to the node; and attaches 
   * the provided channel to the device.
   *
   * \param c The NodeContainer holding the nodes to be changed.
   * \param channelName The name of the channel to attach to the devices.
   * \returns A containter holding the added net devices.
   */
  NetDeviceContainer Install (const NodeContainer &c, std::string channelName) const;

  /**
   * \brief Make a star network topology.
   *
   * Given a pointer to a node that  will become the hub of the star, and a 
   * NodeContainer containing pointers to the nodes that will become the 
   * spokes; we construct CSMA net devices on the hub (corresponding to the 
   * spokes) and store them in the hubDevices NetDeviceContainer.  We add a 
   * net device to each spoke node and store them in the spokeDevices 
   * NetDeviceContainer.  A CSMA is created for each spoke.
   *
   * Usually when one thinks of a star network, one thinks of point-to-point
   * links.  We're just using a single pair of devices on a multi-point-to-point
   * network "drops" as the link.  You are free to add any number of other 
   * devices on the link if you want.
   *
   * The ordering of the devices in the hubDevices container is according to
   * the order of the spokes container -- that is, hubDevices[0] will be the
   * net device used on the hub that talks to spokes[0].  the container entry
   * spokeDevices[0] will have the device that hubDevices[0] talks to -- those
   * two devices are the ones that connect hub to spokes[0].
   *
   * \param hub The central node of the star network
   * \param spokes A NodeContainer of the nodes that will be the spoke (leaf)
   *               nodes
   * \param hubDevices A NetDeviceContainer that will be filled with pointers
   *                   to the point-to-point net devices created on the hub.
   * \param spokeDevices A NetDeviceContainer that will be filled with pointers
   *                    to the point-to-point net devices created on each of 
   *                    the spokes.
   */
  void InstallStar (Ptr<Node> hub, NodeContainer spokes, 
                    NetDeviceContainer& hubDevices, NetDeviceContainer& spokeDevices);

  /**
   * \brief Make a star network topology.
   *
   * Given a pointer to a node that  will become the hub of the star, and a 
   * NodeContainer containing pointers to the nodes that will become the 
   * spokes; we construct CSMA net devices on the hub (corresponding to the 
   * spokes) and store them in the hubDevices NetDeviceContainer.  We add a 
   * net device to each spoke node and store them in the spokeDevices 
   * NetDeviceContainer.  A CSMA is created for each spoke.
   *
   * Usually when one thinks of a star network, one thinks of point-to-point
   * links.  We're just using a single pair of devices on a multi-point-to-point
   * network "drops" as the link.  You are free to add any number of other 
   * devices on the link if you want.
   *
   * The ordering of the devices in the hubDevices container is according to
   * the order of the spokes container -- that is, hubDevices[0] will be the
   * net device used on the hub that talks to spokes[0].  the container entry
   * spokeDevices[0] will have the device that hubDevices[0] talks to -- those
   * two devices are the ones that connect hub to spokes[0].
   *
   * \param hubName The name of the central node of the star network
   * \param spokes A NodeContainer of the nodes that will be the spoke (leaf)
   *               nodes
   * \param hubDevices A NetDeviceContainer that will be filled with pointers
   *                   to the point-to-point net devices created on the hub.
   * \param spokeDevices A NetDeviceContainer that will be filled with pointers
   *                     to the point-to-point net devices created on each of 
   *                     the spokes.
   */
  void InstallStar (std::string hubName, NodeContainer spokes, 
                    NetDeviceContainer& hubDevices, NetDeviceContainer& spokeDevices);

private:
  Ptr<NetDevice> InstallPriv (Ptr<Node> node, Ptr<CsmaChannel> channel) const;

  static void SniffEvent (Ptr<PcapWriter> writer, Ptr<const Packet> packet);

  static void AsciiRxEvent (std::ostream *os, std::string path, Ptr<const Packet> packet);
  static void AsciiEnqueueEvent (std::ostream *os, std::string path, Ptr<const Packet> packet);
  static void AsciiDequeueEvent (std::ostream *os, std::string path, Ptr<const Packet> packet);
  static void AsciiDropEvent (std::ostream *os, std::string path, Ptr<const Packet> packet);

  ObjectFactory m_queueFactory;
  ObjectFactory m_deviceFactory;
  ObjectFactory m_channelFactory;
};

} // namespace ns3

#endif /* CSMA_HELPER_H */
