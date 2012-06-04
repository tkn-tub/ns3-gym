/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Manuel Requena <manuel.requena@cttc.es> (Based on lte-helper.h)
 */

#ifndef LTE_SIMPLE_HELPER_H
#define LTE_SIMPLE_HELPER_H

#include "ns3/net-device-container.h"
#include "ns3/simple-channel.h"
#include "ns3/node-container.h"
#include "ns3/radio-bearer-stats-calculator.h"

#include "ns3/lte-pdcp.h"
#include "ns3/lte-rlc.h"
#include "ns3/lte-rlc-um.h"
#include "ns3/lte-rlc-am.h"

namespace ns3 {

class LteTestRrc;
class LteTestMac;

/**
 * Creation and configuration of LTE entities
 *
 */
class LteSimpleHelper : public Object
{
public:
  LteSimpleHelper (void);
  virtual ~LteSimpleHelper (void);

  static TypeId GetTypeId (void);
  virtual void DoDispose (void);


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
   * Enables logging for all components of the LENA architecture
   *
   */
  void EnableLogComponents (void);

  /**
   * Enables trace sinks for MAC, RLC and PDCP
   */
  void EnableTraces (void);


  /**
   * Enable trace sinks for RLC layer
   */
  void EnableRlcTraces (void);

  /**
   * Enable trace sinks for DL RLC layer
   */
  void EnableDlRlcTraces (void);

  /**
   * Enable trace sinks for UL RLC layer
   */
  void EnableUlRlcTraces (void);


  /**
   * Enable trace sinks for PDCP layer
   */
  void EnablePdcpTraces (void);

  /**
   * Enable trace sinks for DL PDCP layer
   */
  void EnableDlPdcpTraces (void);

  /**
   * Enable trace sinks for UL PDCP layer
   */
  void EnableUlPdcpTraces (void);


protected:
  // inherited from Object
  virtual void DoStart (void);

private:
  Ptr<NetDevice> InstallSingleEnbDevice (Ptr<Node> n);
  Ptr<NetDevice> InstallSingleUeDevice (Ptr<Node> n);

  Ptr<SimpleChannel> m_phyChannel;

public:

  Ptr<LteTestRrc> m_enbRrc;
  Ptr<LteTestRrc> m_ueRrc;

  Ptr<LteTestMac> m_enbMac;
  Ptr<LteTestMac> m_ueMac;

private:

  Ptr<LtePdcp>    m_enbPdcp;
  Ptr<LteRlc>     m_enbRlc;

  Ptr<LtePdcp>    m_uePdcp;
  Ptr<LteRlc>     m_ueRlc;

  ObjectFactory   m_enbDeviceFactory;
  ObjectFactory   m_ueDeviceFactory;

  enum LteRlcEntityType_t {RLC_UM = 1,
                           RLC_AM = 2 } m_lteRlcEntityType;

};


} // namespace ns3


#endif // LTE_SIMPLE_HELPER_H
