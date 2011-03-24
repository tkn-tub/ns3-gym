/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */

#ifndef LENA_HELPER_H
#define LENA_HELPER_H

#include <ns3/log.h>
#include <ns3/config.h>
#include <ns3/simulator.h>
#include <ns3/names.h>
#include <ns3/net-device.h>
#include <ns3/net-device-container.h>
#include <ns3/node.h>
#include <ns3/node-container.h>
#include <ns3/eps-bearer.h>

namespace ns3 {


class LteUePhy;
class LteEnbPhy;
class SpectrumChannel;


/**
 * Creation and configuration of LTE entities
 *
 */
class LenaHelper
{
public:
  LenaHelper (void);
  ~LenaHelper (void);


  /**
   * \todo to be implemented
   *
   * \param name
   * \param value
   */
  void SetEnbDeviceAttribute (std::string name, const AttributeValue &value);

  /**
   * \todo to be implemented
   *
   * \param name
   * \param value
   */
  void SetUeDeviceAttribute (std::string name, const AttributeValue &value);


  /**
   * create a set of eNB devices
   *
   * \param c the node container where the devices are to be installed
   *
   * \return the NetDeviceContainer with the newly created devices
   */
  NetDeviceContainer InstallEnbDevice (NodeContainer c);

  /**
   * create a set of UE devices
   *
   * \param c the node container where the devices are to be installed
   *
   * \return the NetDeviceContainer with the newly created devices
   */
  NetDeviceContainer InstallUeDevice (NodeContainer c);

  /**
   * Attach a set of UE devices to a single eNB device
   *
   * \param ueDevices
   * \param enbDevice
   */
  void Attach (NetDeviceContainer ueDevices, Ptr<NetDevice> enbDevice);

  /**
   * Attach a UE device to an eNB device
   *
   * \param ueDevice
   * \param enbDevice
   */
  void Attach (Ptr<NetDevice> ueDevice, Ptr<NetDevice> enbDevice);

  /**
   * Activate an EPS bearer on a given set of UE devices
   *
   * \param ueDevices the set of UE devices
   * \param bearer the characteristics of the bearer to be activated
   */
  void ActivateEpsBearer (NetDeviceContainer ueDevices, EpsBearer bearer);

  /**
   * Activate an EPS bearer on a given UE device
   *
   * \param ueDevices the set of UE devices
   * \param bearer the characteristics of the bearer to be activated
   */
  void ActivateEpsBearer (Ptr<NetDevice> ueDevice, EpsBearer bearer);
  
  
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
  * in the requested scheduler.
  */
  void SetScheduler (std::string type,
                                std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
                                std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                                std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                                std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
                                std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
                                std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
                                std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
                                std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());
  
  
  

  /**
   * Enables logging for all components of the LENA architecture
   *
   */
  void EnableLogComponents (void);

private:
  Ptr<NetDevice> InstallSingleEnbDevice (Ptr<Node> n);
  Ptr<NetDevice> InstallSingleUeDevice (Ptr<Node> n);

  Ptr<SpectrumChannel> m_downlinkChannel;
  Ptr<SpectrumChannel> m_uplinkChannel;
  
  //ObjectFactory m_scheduler;
};


} // namespace ns3



#endif // LENA_HELPER_H
