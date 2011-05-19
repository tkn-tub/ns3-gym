/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
 * Copyright (c) 2009 MIRKO BANCHI
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
 * Author: Mirko Banchi <mk.banchi@gmail.com>
 */
#ifndef WIFI_HELPER_H
#define WIFI_HELPER_H

#include <string>
#include "ns3/attribute.h"
#include "ns3/object-factory.h"
#include "ns3/node-container.h"
#include "ns3/net-device-container.h"
#include "ns3/wifi-phy-standard.h"
#include "ns3/trace-helper.h"

namespace ns3 {

class WifiPhy;
class WifiMac;
class WifiNetDevice;
class Node;

/**
 * \brief create PHY objects
 *
 * This base class must be implemented by new PHY implementation which wish to integrate
 * with the \ref ns3::WifiHelper class.
 */
class WifiPhyHelper
{
public:
  virtual ~WifiPhyHelper ();

  /**
   * \param node the node on which the PHY object will reside
   * \param device the device within which the PHY object will reside
   * \returns a new PHY object.
   *
   * Subclasses must implement this method to allow the ns3::WifiHelper class
   * to create PHY objects from ns3::WifiHelper::Install.
   */
  virtual Ptr<WifiPhy> Create (Ptr<Node> node, Ptr<WifiNetDevice> device) const = 0;
};

/**
 * \brief create MAC objects
 *
 * This base class must be implemented by new MAC implementation which wish to integrate
 * with the \ref ns3::WifiHelper class.
 */
class WifiMacHelper
{
public:
  virtual ~WifiMacHelper ();
  /**
   * \returns a new MAC object.
   *
   * Subclasses must implement this method to allow the ns3::WifiHelper class
   * to create MAC objects from ns3::WifiHelper::Install.
   */
  virtual Ptr<WifiMac> Create (void) const = 0;
};

/**
 * \brief helps to create WifiNetDevice objects
 *
 * This class can help to create a large set of similar
 * WifiNetDevice objects and to configure a large set of
 * their attributes during creation.
 */
class WifiHelper
{
public:
  /**
   * Create a Wifi helper in an empty state: all its parameters
   * must be set before calling ns3::WifiHelper::Install
   */
  WifiHelper ();

  /**
   * \returns a new WifiHelper in a default state
   *
   * The default state is defined as being an Adhoc MAC layer with an ARF rate control algorithm
   * and both objects using their default attribute values. By default, configure MAC and PHY
   * for 802.11a.
   */
  static WifiHelper Default (void);

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
   * \param phy the PHY helper to create PHY objects
   * \param mac the MAC helper to create MAC objects
   * \param c the set of nodes on which a wifi device must be created
   * \returns a device container which contains all the devices created by this method.
   */
  NetDeviceContainer Install (const WifiPhyHelper &phy,
                              const WifiMacHelper &mac, NodeContainer c) const;
  /**
   * \param phy the PHY helper to create PHY objects
   * \param mac the MAC helper to create MAC objects
   * \param node the node on which a wifi device must be created
   * \returns a device container which contains all the devices created by this method.
   */
  NetDeviceContainer Install (const WifiPhyHelper &phy,
                              const WifiMacHelper &mac, Ptr<Node> node) const;
  /**
   * \param phy the PHY helper to create PHY objects
   * \param mac the MAC helper to create MAC objects
   * \param nodeName the name of node on which a wifi device must be created
   * \returns a device container which contains all the devices created by this method.
   */
  NetDeviceContainer Install (const WifiPhyHelper &phy,
                              const WifiMacHelper &mac, std::string nodeName) const;

  /**
   * \param standard the phy standard to configure during installation
   *
   * By default, all objects are configured for 802.11a
   */
  void SetStandard (enum WifiPhyStandard standard);

  /**
   * Helper to enable all WifiNetDevice log components with one statement
   */
  static void EnableLogComponents (void);

private:
  ObjectFactory m_stationManager;
  enum WifiPhyStandard m_standard;
};

} // namespace ns3

#endif /* WIFI_HELPER_H */
