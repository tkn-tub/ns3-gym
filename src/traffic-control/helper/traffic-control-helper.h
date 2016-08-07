/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Universita' degli Studi di Napoli Federico II
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
 * Author: Stefano Avallone <stavallo@unina.it>
 */
#ifndef TRAFFIC_CONTROL_HELPER_H
#define TRAFFIC_CONTROL_HELPER_H

#include <string>
#include <vector>
#include <map>
#include "ns3/object-factory.h"
#include "ns3/net-device-container.h"
#include "ns3/queue-disc-container.h"

namespace ns3 {

class QueueDisc;

/**
 * \ingroup traffic-control
 *
 * This class stores object factories required to create a queue disc and all of
 * its components (packet filters, internal queues, classes).
 */
class QueueDiscFactory
{
public:
  /**
   * \brief Constructor
   *
   * \param factory the factory used to create this queue disc
   */
  QueueDiscFactory (ObjectFactory factory);

  virtual ~QueueDiscFactory () {}

  /**
   * \brief Add a factory to create an internal queue
   *
   * \param factory the factory used to create an internal queue
   */
  void AddInternalQueue (ObjectFactory factory);

  /**
   * \brief Add a factory to create a packet filter
   *
   * \param factory the factory used to create a packet filter
   */
  void AddPacketFilter (ObjectFactory factory);

  /**
   * \brief Add a factory to create a queue disc class
   *
   * \param factory the factory used to create a queue disc class
   * \return the class ID of the created queue disc class
   */
  uint16_t AddQueueDiscClass (ObjectFactory factory);

  /**
   * \brief Set the (child) queue disc to attach to a class
   *
   * \param classId the id of the class to attach a child queue disc to
   * \param handle the handle of the child queue disc to attach to the class
   */
  void SetChildQueueDisc (uint16_t classId, uint16_t handle);

  /**
   * \brief Create a queue disc with the currently stored configuration.
   *
   * \param queueDiscs the vector of queue discs held by the helper
   * \return the created queue disc
   */
  Ptr<QueueDisc> CreateQueueDisc (const std::vector<Ptr<QueueDisc> > & queueDiscs);

private:
  /**
   * \brief Default constructor
   *
   * Defined and unimplemented to avoid misuse
   */
  QueueDiscFactory ();

  /// Factory to create this queue disc
  ObjectFactory m_queueDiscFactory;
  /// Vector of factories to create internal queues
  std::vector<ObjectFactory> m_internalQueuesFactory;
  /// Vector of factories to create packet filters
  std::vector<ObjectFactory> m_packetFiltersFactory;
  /// Vector of factories to create queue disc classes
  std::vector<ObjectFactory> m_queueDiscClassesFactory;
  /// Map storing the associations between class IDs and child queue disc handles
  std::map<uint16_t, uint16_t> m_classIdChildHandleMap;
};


/**
 * \ingroup traffic-control
 *
 * \brief Build a set of QueueDisc objects
 *
 * This class can help to create QueueDisc objects and map them to
 * the corresponding devices. This map is stored at the Traffic Control
 * layer.
 */
class TrafficControlHelper
{
public:
  /**
   * Create a TrafficControlHelper to make life easier when creating QueueDisc
   * objects.
   */
  TrafficControlHelper ();
  virtual ~TrafficControlHelper () {}

  /**
   * \returns a new TrafficControlHelper with a default configuration
   *
   * The default configuration is a PfifoFastQueueDisc with three internal queues
   * of type DropTailQueue and size 1000 packets, and a PfifoFastIpv4PacketFilter.
   */
  static TrafficControlHelper Default (void);

  /**
   * Helper function used to set a root queue disc of the given type and with the
   * given attributes. To set the InternalQueueList, PacketFilterList and ChildQueueDiscList
   * attributes, use the AddInternalQueue, AddPacketFilter and AddChildQueueDisc methods.
   *
   * \param type the type of queue disc
   * \param n01 the name of the attribute to set on the queue disc
   * \param v01 the value of the attribute to set on the queue disc
   * \param n02 the name of the attribute to set on the queue disc
   * \param v02 the value of the attribute to set on the queue disc
   * \param n03 the name of the attribute to set on the queue disc
   * \param v03 the value of the attribute to set on the queue disc
   * \param n04 the name of the attribute to set on the queue disc
   * \param v04 the value of the attribute to set on the queue disc
   * \param n05 the name of the attribute to set on the queue disc
   * \param v05 the value of the attribute to set on the queue disc
   * \param n06 the name of the attribute to set on the queue disc
   * \param v06 the value of the attribute to set on the queue disc
   * \param n07 the name of the attribute to set on the queue disc
   * \param v07 the value of the attribute to set on the queue disc
   * \param n08 the name of the attribute to set on the queue disc
   * \param v08 the value of the attribute to set on the queue disc
   * \param n09 the name of the attribute to set on the queue disc
   * \param v09 the value of the attribute to set on the queue disc
   * \param n10 the name of the attribute to set on the queue disc
   * \param v10 the value of the attribute to set on the queue disc
   * \param n11 the name of the attribute to set on the queue disc
   * \param v11 the value of the attribute to set on the queue disc
   * \param n12 the name of the attribute to set on the queue disc
   * \param v12 the value of the attribute to set on the queue disc
   * \param n13 the name of the attribute to set on the queue disc
   * \param v13 the value of the attribute to set on the queue disc
   * \param n14 the name of the attribute to set on the queue disc
   * \param v14 the value of the attribute to set on the queue disc
   * \param n15 the name of the attribute to set on the queue disc
   * \param v15 the value of the attribute to set on the queue disc
   * \return the handle of the root queue disc (zero)
   */
  uint16_t SetRootQueueDisc (std::string type,
                             std::string n01 = "", const AttributeValue &v01 = EmptyAttributeValue (),
                             std::string n02 = "", const AttributeValue &v02 = EmptyAttributeValue (),
                             std::string n03 = "", const AttributeValue &v03 = EmptyAttributeValue (),
                             std::string n04 = "", const AttributeValue &v04 = EmptyAttributeValue (),
                             std::string n05 = "", const AttributeValue &v05 = EmptyAttributeValue (),
                             std::string n06 = "", const AttributeValue &v06 = EmptyAttributeValue (),
                             std::string n07 = "", const AttributeValue &v07 = EmptyAttributeValue (),
                             std::string n08 = "", const AttributeValue &v08 = EmptyAttributeValue (),
                             std::string n09 = "", const AttributeValue &v09 = EmptyAttributeValue (),
                             std::string n10 = "", const AttributeValue &v10 = EmptyAttributeValue (),
                             std::string n11 = "", const AttributeValue &v11 = EmptyAttributeValue (),
                             std::string n12 = "", const AttributeValue &v12 = EmptyAttributeValue (),
                             std::string n13 = "", const AttributeValue &v13 = EmptyAttributeValue (),
                             std::string n14 = "", const AttributeValue &v14 = EmptyAttributeValue (),
                             std::string n15 = "", const AttributeValue &v15 = EmptyAttributeValue ());

  /**
   * Helper function used to add the given number of internal queues (of the given
   * type and with the given attributes) to the queue disc having the given handle.
   *
   * \param handle the handle of the parent queue disc
   * \param count the number of queues to add
   * \param type the type of queue
   * \param n01 the name of the attribute to set on the queue
   * \param v01 the value of the attribute to set on the queue
   * \param n02 the name of the attribute to set on the queue
   * \param v02 the value of the attribute to set on the queue
   * \param n03 the name of the attribute to set on the queue
   * \param v03 the value of the attribute to set on the queue
   * \param n04 the name of the attribute to set on the queue
   * \param v04 the value of the attribute to set on the queue
   * \param n05 the name of the attribute to set on the queue
   * \param v05 the value of the attribute to set on the queue
   * \param n06 the name of the attribute to set on the queue
   * \param v06 the value of the attribute to set on the queue
   * \param n07 the name of the attribute to set on the queue
   * \param v07 the value of the attribute to set on the queue
   * \param n08 the name of the attribute to set on the queue
   * \param v08 the value of the attribute to set on the queue
   */
  void AddInternalQueues (uint16_t handle, uint16_t count, std::string type,
                          std::string n01 = "", const AttributeValue &v01 = EmptyAttributeValue (),
                          std::string n02 = "", const AttributeValue &v02 = EmptyAttributeValue (),
                          std::string n03 = "", const AttributeValue &v03 = EmptyAttributeValue (),
                          std::string n04 = "", const AttributeValue &v04 = EmptyAttributeValue (),
                          std::string n05 = "", const AttributeValue &v05 = EmptyAttributeValue (),
                          std::string n06 = "", const AttributeValue &v06 = EmptyAttributeValue (),
                          std::string n07 = "", const AttributeValue &v07 = EmptyAttributeValue (),
                          std::string n08 = "", const AttributeValue &v08 = EmptyAttributeValue ());

  /**
   * Helper function used to add a packet filter (of the given type and with
   * the given attributes) to the queue disc having the given handle.
   *
   * \param handle the handle of the parent queue disc
   * \param type the type of packet filter
   * \param n01 the name of the attribute to set on the packet filter
   * \param v01 the value of the attribute to set on the packet filter
   * \param n02 the name of the attribute to set on the packet filter
   * \param v02 the value of the attribute to set on the packet filter
   * \param n03 the name of the attribute to set on the packet filter
   * \param v03 the value of the attribute to set on the packet filter
   * \param n04 the name of the attribute to set on the packet filter
   * \param v04 the value of the attribute to set on the packet filter
   * \param n05 the name of the attribute to set on the packet filter
   * \param v05 the value of the attribute to set on the packet filter
   * \param n06 the name of the attribute to set on the packet filter
   * \param v06 the value of the attribute to set on the packet filter
   * \param n07 the name of the attribute to set on the packet filter
   * \param v07 the value of the attribute to set on the packet filter
   * \param n08 the name of the attribute to set on the packet filter
   * \param v08 the value of the attribute to set on the packet filter
   */
  void AddPacketFilter (uint16_t handle, std::string type,
                        std::string n01 = "", const AttributeValue &v01 = EmptyAttributeValue (),
                        std::string n02 = "", const AttributeValue &v02 = EmptyAttributeValue (),
                        std::string n03 = "", const AttributeValue &v03 = EmptyAttributeValue (),
                        std::string n04 = "", const AttributeValue &v04 = EmptyAttributeValue (),
                        std::string n05 = "", const AttributeValue &v05 = EmptyAttributeValue (),
                        std::string n06 = "", const AttributeValue &v06 = EmptyAttributeValue (),
                        std::string n07 = "", const AttributeValue &v07 = EmptyAttributeValue (),
                        std::string n08 = "", const AttributeValue &v08 = EmptyAttributeValue ());

  /**
   * Container type for Class IDs
   */
  typedef std::vector<uint16_t> ClassIdList;

  /**
   * Helper function used to add the given number of queue disc classes (of the given
   * type and with the given attributes) to the queue disc having the given handle.
   *
   * \param handle the handle of the parent queue disc
   * \param count the number of queue disc classes to add
   * \param type the type of queue disc class
   * \param n01 the name of the attribute to set on the queue disc class
   * \param v01 the value of the attribute to set on the queue disc class
   * \param n02 the name of the attribute to set on the queue disc class
   * \param v02 the value of the attribute to set on the queue disc class
   * \param n03 the name of the attribute to set on the queue disc class
   * \param v03 the value of the attribute to set on the queue disc class
   * \param n04 the name of the attribute to set on the queue disc class
   * \param v04 the value of the attribute to set on the queue disc class
   * \param n05 the name of the attribute to set on the queue disc class
   * \param v05 the value of the attribute to set on the queue disc class
   * \param n06 the name of the attribute to set on the queue disc class
   * \param v06 the value of the attribute to set on the queue disc class
   * \param n07 the name of the attribute to set on the queue disc class
   * \param v07 the value of the attribute to set on the queue disc class
   * \param n08 the name of the attribute to set on the queue disc class
   * \param v08 the value of the attribute to set on the queue disc class
   * \return the list of class IDs
   */
  ClassIdList AddQueueDiscClasses (uint16_t handle, uint16_t count, std::string type,
                                   std::string n01 = "", const AttributeValue &v01 = EmptyAttributeValue (),
                                   std::string n02 = "", const AttributeValue &v02 = EmptyAttributeValue (),
                                   std::string n03 = "", const AttributeValue &v03 = EmptyAttributeValue (),
                                   std::string n04 = "", const AttributeValue &v04 = EmptyAttributeValue (),
                                   std::string n05 = "", const AttributeValue &v05 = EmptyAttributeValue (),
                                   std::string n06 = "", const AttributeValue &v06 = EmptyAttributeValue (),
                                   std::string n07 = "", const AttributeValue &v07 = EmptyAttributeValue (),
                                   std::string n08 = "", const AttributeValue &v08 = EmptyAttributeValue ());

  /**
   * Helper function used to attach a child queue disc (of the given type and with
   * the given attributes) to a given class (included in the queue disc
   * having the given handle).
   *
   * \param handle the handle of the parent queue disc
   * \param classId the class ID of the class to attach the queue disc to
   * \param type the type of queue disc
   * \param n01 the name of the attribute to set on the queue disc
   * \param v01 the value of the attribute to set on the queue disc
   * \param n02 the name of the attribute to set on the queue disc
   * \param v02 the value of the attribute to set on the queue disc
   * \param n03 the name of the attribute to set on the queue disc
   * \param v03 the value of the attribute to set on the queue disc
   * \param n04 the name of the attribute to set on the queue disc
   * \param v04 the value of the attribute to set on the queue disc
   * \param n05 the name of the attribute to set on the queue disc
   * \param v05 the value of the attribute to set on the queue disc
   * \param n06 the name of the attribute to set on the queue disc
   * \param v06 the value of the attribute to set on the queue disc
   * \param n07 the name of the attribute to set on the queue disc
   * \param v07 the value of the attribute to set on the queue disc
   * \param n08 the name of the attribute to set on the queue disc
   * \param v08 the value of the attribute to set on the queue disc
   * \param n09 the name of the attribute to set on the queue disc
   * \param v09 the value of the attribute to set on the queue disc
   * \param n10 the name of the attribute to set on the queue disc
   * \param v10 the value of the attribute to set on the queue disc
   * \param n11 the name of the attribute to set on the queue disc
   * \param v11 the value of the attribute to set on the queue disc
   * \param n12 the name of the attribute to set on the queue disc
   * \param v12 the value of the attribute to set on the queue disc
   * \param n13 the name of the attribute to set on the queue disc
   * \param v13 the value of the attribute to set on the queue disc
   * \param n14 the name of the attribute to set on the queue disc
   * \param v14 the value of the attribute to set on the queue disc
   * \param n15 the name of the attribute to set on the queue disc
   * \param v15 the value of the attribute to set on the queue disc
   * \return the handle of the created child queue disc
   */
  uint16_t AddChildQueueDisc (uint16_t handle, uint16_t classId, std::string type,
                              std::string n01 = "", const AttributeValue &v01 = EmptyAttributeValue (),
                              std::string n02 = "", const AttributeValue &v02 = EmptyAttributeValue (),
                              std::string n03 = "", const AttributeValue &v03 = EmptyAttributeValue (),
                              std::string n04 = "", const AttributeValue &v04 = EmptyAttributeValue (),
                              std::string n05 = "", const AttributeValue &v05 = EmptyAttributeValue (),
                              std::string n06 = "", const AttributeValue &v06 = EmptyAttributeValue (),
                              std::string n07 = "", const AttributeValue &v07 = EmptyAttributeValue (),
                              std::string n08 = "", const AttributeValue &v08 = EmptyAttributeValue (),
                              std::string n09 = "", const AttributeValue &v09 = EmptyAttributeValue (),
                              std::string n10 = "", const AttributeValue &v10 = EmptyAttributeValue (),
                              std::string n11 = "", const AttributeValue &v11 = EmptyAttributeValue (),
                              std::string n12 = "", const AttributeValue &v12 = EmptyAttributeValue (),
                              std::string n13 = "", const AttributeValue &v13 = EmptyAttributeValue (),
                              std::string n14 = "", const AttributeValue &v14 = EmptyAttributeValue (),
                              std::string n15 = "", const AttributeValue &v15 = EmptyAttributeValue ());

  /**
   * Container type for Handlers
   */
  typedef std::vector<uint16_t> HandleList;

  /**
   * Helper function used to attach a child queue disc (of the given type and with
   * the given attributes) to each of the given classes (included in the queue disc
   * having the given handle).
   *
   * \param handle the handle of the parent queue disc
   * \param classes the class IDs of the classes to attach a queue disc to
   * \param type the type of queue disc
   * \param n01 the name of the attribute to set on the queue disc
   * \param v01 the value of the attribute to set on the queue disc
   * \param n02 the name of the attribute to set on the queue disc
   * \param v02 the value of the attribute to set on the queue disc
   * \param n03 the name of the attribute to set on the queue disc
   * \param v03 the value of the attribute to set on the queue disc
   * \param n04 the name of the attribute to set on the queue disc
   * \param v04 the value of the attribute to set on the queue disc
   * \param n05 the name of the attribute to set on the queue disc
   * \param v05 the value of the attribute to set on the queue disc
   * \param n06 the name of the attribute to set on the queue disc
   * \param v06 the value of the attribute to set on the queue disc
   * \param n07 the name of the attribute to set on the queue disc
   * \param v07 the value of the attribute to set on the queue disc
   * \param n08 the name of the attribute to set on the queue disc
   * \param v08 the value of the attribute to set on the queue disc
   * \param n09 the name of the attribute to set on the queue disc
   * \param v09 the value of the attribute to set on the queue disc
   * \param n10 the name of the attribute to set on the queue disc
   * \param v10 the value of the attribute to set on the queue disc
   * \param n11 the name of the attribute to set on the queue disc
   * \param v11 the value of the attribute to set on the queue disc
   * \param n12 the name of the attribute to set on the queue disc
   * \param v12 the value of the attribute to set on the queue disc
   * \param n13 the name of the attribute to set on the queue disc
   * \param v13 the value of the attribute to set on the queue disc
   * \param n14 the name of the attribute to set on the queue disc
   * \param v14 the value of the attribute to set on the queue disc
   * \param n15 the name of the attribute to set on the queue disc
   * \param v15 the value of the attribute to set on the queue disc
   * \return the list of handles of the created child queue discs
   */
  HandleList AddChildQueueDiscs (uint16_t handle, const ClassIdList &classes, std::string type,
                                 std::string n01 = "", const AttributeValue &v01 = EmptyAttributeValue (),
                                 std::string n02 = "", const AttributeValue &v02 = EmptyAttributeValue (),
                                 std::string n03 = "", const AttributeValue &v03 = EmptyAttributeValue (),
                                 std::string n04 = "", const AttributeValue &v04 = EmptyAttributeValue (),
                                 std::string n05 = "", const AttributeValue &v05 = EmptyAttributeValue (),
                                 std::string n06 = "", const AttributeValue &v06 = EmptyAttributeValue (),
                                 std::string n07 = "", const AttributeValue &v07 = EmptyAttributeValue (),
                                 std::string n08 = "", const AttributeValue &v08 = EmptyAttributeValue (),
                                 std::string n09 = "", const AttributeValue &v09 = EmptyAttributeValue (),
                                 std::string n10 = "", const AttributeValue &v10 = EmptyAttributeValue (),
                                 std::string n11 = "", const AttributeValue &v11 = EmptyAttributeValue (),
                                 std::string n12 = "", const AttributeValue &v12 = EmptyAttributeValue (),
                                 std::string n13 = "", const AttributeValue &v13 = EmptyAttributeValue (),
                                 std::string n14 = "", const AttributeValue &v14 = EmptyAttributeValue (),
                                 std::string n15 = "", const AttributeValue &v15 = EmptyAttributeValue ());

  /**
   * Helper function used to add a queue limits object to the transmission
   * queues of the devices
   *
   * \param type the type of queue
   * \param n01 the name of the attribute to set on the queue limits object
   * \param v01 the value of the attribute to set on the queue limits object
   * \param n02 the name of the attribute to set on the queue limits object
   * \param v02 the value of the attribute to set on the queue limits object
   * \param n03 the name of the attribute to set on the queue limits object
   * \param v03 the value of the attribute to set on the queue limits object
   * \param n04 the name of the attribute to set on the queue limits object
   * \param v04 the value of the attribute to set on the queue limits object
   * \param n05 the name of the attribute to set on the queue limits object
   * \param v05 the value of the attribute to set on the queue limits object
   * \param n06 the name of the attribute to set on the queue limits object
   * \param v06 the value of the attribute to set on the queue limits object
   * \param n07 the name of the attribute to set on the queue limits object
   * \param v07 the value of the attribute to set on the queue limits object
   * \param n08 the name of the attribute to set on the queue limits object
   * \param v08 the value of the attribute to set on the queue limits object
   */
  void SetQueueLimits (std::string type,
                       std::string n01 = "", const AttributeValue &v01 = EmptyAttributeValue (),
                       std::string n02 = "", const AttributeValue &v02 = EmptyAttributeValue (),
                       std::string n03 = "", const AttributeValue &v03 = EmptyAttributeValue (),
                       std::string n04 = "", const AttributeValue &v04 = EmptyAttributeValue (),
                       std::string n05 = "", const AttributeValue &v05 = EmptyAttributeValue (),
                       std::string n06 = "", const AttributeValue &v06 = EmptyAttributeValue (),
                       std::string n07 = "", const AttributeValue &v07 = EmptyAttributeValue (),
                       std::string n08 = "", const AttributeValue &v08 = EmptyAttributeValue ());

  /**
   * \param c set of devices
   * \returns a QueueDisc container with the queue discs installed on the devices
   *
   * This method creates a QueueDisc object of the type and with the
   * attributes configured by TrafficControlHelper::SetQueueDisc for
   * each device in the container. Then, stores the mapping between a
   * device and the associated queue disc into the traffic control layer
   * of the corresponding node.
   * This method creates the queue discs (along with their packet filters,
   * internal queues, classes) configured with the methods provided by this
   * class and installs them on each device in the given container. Additionally,
   * if configured, a queue limits object is installed on each transmission queue
   * of the devices.
   */
  QueueDiscContainer Install (NetDeviceContainer c);

  /**
   * \param d device
   * \returns a QueueDisc container with the queue discs installed on the device
   *
   * This method creates the queue discs (along with their packet filters,
   * internal queues, classes) configured with the methods provided by this
   * class and installs them on the given device. Additionally, if configured,
   * a queue limits object is installed on each transmission queue of the device.
   */
  QueueDiscContainer Install (Ptr<NetDevice> d);

  /**
   * \param c set of devices
   *
   * This method removes the root queue discs (and associated filters, classes
   * and queues) installed on the given devices.
   */
  void Uninstall (NetDeviceContainer c);

  /**
   * \param d device
   *
   * This method removes the root queue disc (and associated filters, classes
   * and queues) installed on the given device.
   */
  void Uninstall (Ptr<NetDevice> d);

private:
  /// QueueDisc factory, stores the configuration of all the queue discs
  std::vector<QueueDiscFactory> m_queueDiscFactory;
  /// Vector of all the created queue discs
  std::vector<Ptr<QueueDisc> > m_queueDiscs;
  /// Factory to create a queue limits object
  ObjectFactory m_queueLimitsFactory;
};

} // namespace ns3

#endif /* TRAFFIC_CONTROL_HELPER_H */
