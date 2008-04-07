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
#ifndef POINT_TO_POINT_HELPER_H
#define POINT_TO_POINT_HELPER_H

#include "ns3/object-factory.h"
#include "ns3/net-device-container.h"
#include "ns3/node-container.h"
#include <string>

namespace ns3 {

class Queue;
class NetDevice;
class Node;
class PcapWriter;

/**
 * \brief build a set of PointToPointNetDevice objects
 */
class PointToPointHelper
{
public:
  // by default, create queues of type DropTailQueue.
  PointToPointHelper ();

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
   * PointToPointNetDevice created through PointToPointHelper::Install.
   */
  void SetQueue (std::string type,
		 std::string n1 = "", Attribute v1 = Attribute (),
		 std::string n2 = "", Attribute v2 = Attribute (),
		 std::string n3 = "", Attribute v3 = Attribute (),
		 std::string n4 = "", Attribute v4 = Attribute ());

  /**
   * \param name the name of the attribute to set
   * \param value the value of the attribute to set
   *
   * Set these parameters on each ns3::PointToPointNetDevice created
   * by PointToPointHelper::Install
   */
  void SetDeviceParameter (std::string name, Attribute value);
  /**
   * \param name the name of the attribute to set
   * \param value the value of the attribute to set
   *
   * Set these parameters on each ns3::PointToPointChannel created
   * by PointToPointHelper::Install
   */
  void SetChannelParameter (std::string name, Attribute value);

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
   * \param d container of devices of type ns3::PointToPointNetDevice
   *
   * Enable pcap output on each input device which is of the
   * ns3::PointToPointNetDevice type.
   */
  static void EnablePcap (std::string filename, NetDeviceContainer d);
  /**
   * \param filename filename prefix to use for pcap files.
   * \param n container of nodes.
   *
   * Enable pcap output on each device which is of the
   * ns3::PointToPointNetDevice type and which is located in one of the 
   * input nodes.
   */
  static void EnablePcap (std::string filename, NodeContainer n);
  /**
   * \param filename filename prefix to use for pcap files.
   *
   * Enable pcap output on each device which is of the
   * ns3::PointToPointNetDevice type
   */
  static void EnablePcap (std::string filename);

  /**
   * \param os output stream
   * \param nodeid the id of the node to generate ascii output for.
   * \param deviceid the id of the device to generate ascii output for.
   *
   * Enable ascii output on the specified deviceid within the
   * specified nodeid if it is of type ns3::PointToPointNetDevice and dump 
   * that to the specified stdc++ output stream.
   */
  static void EnableAscii (std::ostream &os, uint32_t nodeid, uint32_t deviceid);
  /**
   * \param os output stream
   * \param d device container
   *
   * Enable ascii output on each device which is of the
   * ns3::PointToPointNetDevice type and which is located in the input
   * device container and dump that to the specified
   * stdc++ output stream.
   */
  static void EnableAscii (std::ostream &os, NetDeviceContainer d);
  /**
   * \param os output stream
   * \param n node container
   *
   * Enable ascii output on each device which is of the
   * ns3::PointToPointNetDevice type and which is located in one
   * of the input node and dump that to the specified
   * stdc++ output stream.
   */
  static void EnableAscii (std::ostream &os, NodeContainer n);
  /**
   * \param os output stream
   *
   * Enable ascii output on each device which is of the
   * ns3::PointToPointNetDevice type and dump that to the specified
   * stdc++ output stream.
   */
  static void EnableAscii (std::ostream &os);

  /**
   * \param c a set of nodes
   *
   * This method creates a ns3::PointToPointChannel with the
   * attributes configured by PointToPointHelper::SetChannelParameter,
   * then, for each node in the input container, we create a 
   * ns3::PointToPointNetDevice with the requested parameters, 
   * a queue for this ns3::NetDevice, and associate the resulting 
   * ns3::NetDevice with the ns3::Node and ns3::PointToPointChannel.
   */
  NetDeviceContainer Install (NodeContainer c);
  /**
   * \param a first node
   * \param b second node
   *
   * Saves you from having to construct a temporary NodeContainer.
   */
  NetDeviceContainer Install (Ptr<Node> a, Ptr<Node> b);

private:
  void EnablePcap (Ptr<Node> node, Ptr<NetDevice> device, Ptr<Queue> queue);
  void EnableAscii (Ptr<Node> node, Ptr<NetDevice> device);
  static void RxEvent (Ptr<PcapWriter> writer, Ptr<const Packet> packet);
  static void EnqueueEvent (Ptr<PcapWriter> writer, Ptr<const Packet> packet);
  static void AsciiEnqueueEvent (std::ostream *os, std::string path, Ptr<const Packet> packet);
  static void AsciiDequeueEvent (std::ostream *os, std::string path, Ptr<const Packet> packet);
  static void AsciiDropEvent (std::ostream *os, std::string path, Ptr<const Packet> packet);
  static void AsciiRxEvent (std::ostream *os, std::string path, Ptr<const Packet> packet);
  ObjectFactory m_queueFactory;
  ObjectFactory m_channelFactory;
  ObjectFactory m_deviceFactory;
};


} // namespace ns3

#endif /* POINT_TO_POINT_HELPER_H */
