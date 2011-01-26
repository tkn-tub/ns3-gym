/*
 * Copyright (c) 2008 University of Washington
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Leonard Tracy <lentracy@u.washington.edu>
 *
 *
 */

#ifndef UANHELPER_H_
#define UANHELPER_H_

#include <string>
#include "ns3/attribute.h"
#include "ns3/object-factory.h"
#include "ns3/node-container.h"
#include "ns3/net-device-container.h"
#include "ns3/uan-net-device.h"

namespace ns3 {

class UanChannel;

class UanHelper
{
public:
  UanHelper ();
  virtual ~UanHelper ();

  /**
   * \param type the type of ns3::UanMac to create.
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
   * \param phyType the type of ns3::UanPhy to create.
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
   * in the requested phy.
   */
  void SetPhy (std::string phyType,
               std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
               std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
               std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
               std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
               std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
               std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
               std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
               std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());


  /**
   * \param type the type of ns3::Transducer to create
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
   * in the requested transducer.
   */
  void SetTransducer (std::string type,
                      std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
                      std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                      std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                      std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
                      std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
                      std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
                      std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
                      std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());
  /**
   * \param os output stream
   * \param nodeid the id of the node to generate ascii output for.
   * \param deviceid the id of the device to generate ascii output for.
   *
   * Enable ascii output on the specified deviceid within the
   * specified nodeid if it is of type ns3::UanNetDevice and dump
   * that to the specified stdc++ output stream.
   */
  static void EnableAscii (std::ostream &os, uint32_t nodeid, uint32_t deviceid);
  /**
   * \param os output stream
   * \param d device container
   *
   * Enable ascii output on each device which is of the
   * ns3::UanNetDevice type and which is located in the input
   * device container and dump that to the specified
   * stdc++ output stream.
   */
  static void EnableAscii (std::ostream &os, NetDeviceContainer d);
  /**
   * \param os output stream
   * \param n node container
   *
   * Enable ascii output on each device which is of the
   * ns3::UanNetDevice type and which is located in one
   * of the input node and dump that to the specified
   * stdc++ output stream.
   */
  static void EnableAscii (std::ostream &os, NodeContainer n);
  /**
   * \param os output stream
   *
   * Enable ascii output on each device which is of the
   * ns3::UanNetDevice type and dump that to the specified
   * stdc++ output stream.
   */
  static void EnableAsciiAll (std::ostream &os);

  /**
   * \param c a set of nodes
   *
   * This method creates a simple ns3::UanChannel (with a default
   * ns3::UanNoiseModelDefault and ns3::UanPropModelIdeal) and
   * creates, for each of the input nodes, a new ns3::UanNetDevice
   * attached to this shared channel. Each ns3::UanNetDevice is also
   * configured with an ns3::UanTransducerHd, ns3::UanMac, and,
   * ns3::UanPhy, all of which are created based on the user-specified
   * attributes specified in UanHelper::SetTransducer,
   * UanHelper::SetMac, and, UanHelper::SetPhy.
   */

  NetDeviceContainer Install (NodeContainer c) const;
  /**
   * \param channel a channel to use
   * \param c a set of nodes
   *
   * For each of the input nodes, a new ns3::UanNetDevice is attached
   * to the shared input channel. Each ns3::UanNetDevice is also
   * configured with an ns3::UanTransducerHd, a ns3::UanMac, and ns3::UanPhy,
   * all of which are created based on the user-specified attributes
   * specified in UanHelper::SetTransducer, UanHelper::SetMac, and
   * UanHelper::SetPhy.
   */
  NetDeviceContainer Install (NodeContainer c, Ptr<UanChannel> channel) const;

  /**
   * \param node a node where to install the uan components
   *
   * Create a default uan stack with:
   * - default channel, ideal propagation and default noise model
   * - default physical layer, with UanPhyGen
   * - default transducer, half duplex acoustic modem with UanTransducerHd
   * - default MAC layer, with UanMacAloha
   *
   * Channel, physical layer, transducer and mac layer are added to the
   * UanNetDevice and then added to the node.
   */
  Ptr<UanNetDevice> Install (Ptr<Node> node, Ptr<UanChannel> channel) const;
private:
  ObjectFactory m_device;
  ObjectFactory m_mac;
  ObjectFactory m_phy;
  ObjectFactory m_transducer;

};


} // end namespace ns3

#endif /* UANHELPER_H_ */
