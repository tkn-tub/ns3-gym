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

#include <ns3/config.h>
#include <ns3/simulator.h>
#include <ns3/names.h>
#include <ns3/net-device.h>
#include <ns3/net-device-container.h>
#include <ns3/node.h>
#include <ns3/node-container.h>
#include <ns3/eps-bearer.h>
#include <ns3/mac-stats-calculator.h>
#include <ns3/rlc-stats-calculator.h>
#include <ns3/lte-tft.h>
#include <ns3/trace-fading-loss-model.h>

namespace ns3 {


class LteUePhy;
class LteEnbPhy;
class SpectrumChannel;
class EpcHelper;
class PropagationLossModel;


/**
 * Creation and configuration of LTE entities
 *
 */
class LenaHelper : public Object
{
public:
  LenaHelper (void);
  virtual ~LenaHelper (void);

  static TypeId GetTypeId (void);
  virtual void DoDispose (void);


  /** 
   * Set the EpcHelper to be used to setup the EPC network in
   * conjunction with the setup of the LTE radio access network 
   *
   * \note if no EpcHelper is ever set, then LenaHelper will default
   * to creating an LTE-only simulation with no EPC, using LteRlcSm as
   * the RLC model, and without supporting any IP networking. In other
   * words, it will be a radio-level simulation involving only LTE PHY
   * and MAC and the FF Scheduler, with a saturation traffic model for
   * the RLC.
   * 
   * \param h a pointer to the EpcHelper to be used
   */
  void SetEpcHelper (Ptr<EpcHelper> h);

  /** 
   * 
   * 
   * \param type the type of propagation model to be used for the eNBs
   */
  void SetPropagationModelType (std::string type);

  /**
   * set an attribute for the propagation model to be created
   * 
   * \param n the name of the attribute
   * \param v the value of the attribute
   */
  void SetPathlossModelAttribute (std::string n, const AttributeValue &v);

  /** 
   * 
   * \param type the type of scheduler to be used for the eNBs
   */
  void SetSchedulerType (std::string type);

  /**
   * set an attribute for the scheduler to be created
   * 
   * \param n the name of the attribute
   * \param v the value of the attribute
   */
  void SetSchedulerAttribute (std::string n, const AttributeValue &v);

  /**
   * set an attribute for the LteEnbNetDevice to be created
   * 
   * \param n the name of the attribute
   * \param v the value of the attribute
   */
  void SetEnbDeviceAttribute (std::string n, const AttributeValue &v);

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
   * \param tft the Traffic Flow Template that identifies the traffic to go on this bearer
   */
  void ActivateEpsBearer (NetDeviceContainer ueDevices, EpsBearer bearer, Ptr<LteTft> tft);

  /**
   * Activate an EPS bearer on a given UE device
   *
   * \param ueDevices the set of UE devices
   * \param bearer the characteristics of the bearer to be activated
   * \param tft the Traffic Flow Template that identifies the traffic to go on this bearer
   */
  void ActivateEpsBearer (Ptr<NetDevice> ueDevice, EpsBearer bearer, Ptr<LteTft> tft);

  /** 
   * 
   * \param bearer the specification of an EPS bearer
   * 
   * \return the type of RLC that is to be created for the given EPS bearer
   */
  TypeId GetRlcType (EpsBearer bearer);

  /** 
   * 
   * 
   * \param type the fading modul to be used
   */
  void SetFadingModel (std::string model);

  /**
   * set an attribute of the fading model
   */
  void SetFadingModelAttribute (std::string n, const AttributeValue &v);

  /**
   * Enables logging for all components of the LENA architecture
   *
   */
  void EnableLogComponents (void);

  /**
   * Enable trace sinks for MAC layer
   */
  void EnableMacTraces (void);

  /**
   * Enable trace sinks for DL MAC layer
   */
  void EnableDlMacTraces (void);

  /**
   * Enable trace sinks for UL MAC layer
   */
  void EnableUlMacTraces (void);

  /**
   * Enable trace sinks for RLC layer
   */
  void EnableRlcTraces (void);

  /**
   * Enable trace sinks for DL RLC layer
   */
  void EnableDlRlcTraces (void);

  /**
   * Enable trace sinks for UL MAC layer
   */
  void EnableUlRlcTraces (void);

  /**
   * Set the output directory for the MAC/RLC trace
   */
  void SetTraceDirectory (std::string path);

  Ptr<RlcStatsCalculator> GetRlcStats (void);

protected:
  // inherited from Object
  virtual void DoStart (void);

private:
  Ptr<NetDevice> InstallSingleEnbDevice (Ptr<Node> n);
  Ptr<NetDevice> InstallSingleUeDevice (Ptr<Node> n);

  Ptr<SpectrumChannel> m_downlinkChannel;
  Ptr<SpectrumChannel> m_uplinkChannel;
  
  Ptr<Object> m_downlinkPropagationModel;
  Ptr<Object> m_uplinkPropagationModel;

  ObjectFactory m_schedulerFactory;
  ObjectFactory m_propagationModelFactory;
  ObjectFactory m_enbNetDeviceFactory;

  ObjectFactory m_dlPropagationModelFactory;
  ObjectFactory m_ulPropagationModelFactory;

  std::string m_fadingModelType;
  ObjectFactory m_fadingModelFactory;
  
  Ptr<TraceFadingLossModel> m_fadingModule;
  
  Ptr<MacStatsCalculator> m_macStats;
  Ptr<RlcStatsCalculator> m_rlcStats;
  enum LteEpsBearerToRlcMapping_t {RLC_SM_ALWAYS = 1,
                                   RLC_UM_ALWAYS = 2,
                                   RLC_AM_ALWAYS = 3,
                                   PER_BASED = 4} m_epsBearerToRlcMapping;

  Ptr<EpcHelper> m_epcHelper;
  
};


} // namespace ns3



#endif // LENA_HELPER_H
