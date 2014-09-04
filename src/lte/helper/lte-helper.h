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

#ifndef LTE_HELPER_H
#define LTE_HELPER_H

#include <ns3/config.h>
#include <ns3/simulator.h>
#include <ns3/names.h>
#include <ns3/net-device.h>
#include <ns3/net-device-container.h>
#include <ns3/node.h>
#include <ns3/node-container.h>
#include <ns3/eps-bearer.h>
#include <ns3/phy-stats-calculator.h>
#include <ns3/phy-tx-stats-calculator.h>
#include <ns3/phy-rx-stats-calculator.h>
#include <ns3/mac-stats-calculator.h>
#include <ns3/radio-bearer-stats-calculator.h>
#include <ns3/radio-bearer-stats-connector.h>
#include <ns3/epc-tft.h>
#include <ns3/mobility-model.h>

namespace ns3 {


class LteUePhy;
class LteEnbPhy;
class SpectrumChannel;
class EpcHelper;
class PropagationLossModel;
class SpectrumPropagationLossModel;

/**
 * \ingroup lte
 * \brief Creation and configuration of LTE entities
 */
class LteHelper : public Object
{
public:
  LteHelper (void);
  virtual ~LteHelper (void);

  static TypeId GetTypeId (void);
  virtual void DoDispose (void);


  /** 
   * Set the EpcHelper to be used to setup the EPC network in
   * conjunction with the setup of the LTE radio access network 
   *
   * \note if no EpcHelper is ever set, then LteHelper will default
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
   * \param type the type of pathloss model to be used for the eNBs
   */
  void SetPathlossModelType (std::string type);

  /**
   * set an attribute for the pathloss model to be created
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
   *
   * \return the scheduler type
   */
  std::string GetSchedulerType () const; 

  /**
   * set an attribute for the scheduler to be created
   * 
   * \param n the name of the attribute
   * \param v the value of the attribute
   */
  void SetSchedulerAttribute (std::string n, const AttributeValue &v);

  /**
   *
   * \param type the type of FFR algorithm to be used for the eNBs
   */
  void SetFfrAlgorithmType (std::string type);

  /**
   *
   * \return the FFR algorithm type
   */
  std::string GetFfrAlgorithmType () const;

  /**
   * set an attribute for the FFR algorithm to be created
   *
   * \param n the name of the attribute
   * \param v the value of the attribute
   */
  void SetFfrAlgorithmAttribute (std::string n, const AttributeValue &v);

  /**
   *
   * \param type the type of handover algorithm to be used for the eNBs
   */
  void SetHandoverAlgorithmType (std::string type);

  /**
   *
   * \return the handover algorithm type
   */
  std::string GetHandoverAlgorithmType () const;

  /**
   * set an attribute for the handover algorithm to be created
   *
   * \param n the name of the attribute
   * \param v the value of the attribute
   */
  void SetHandoverAlgorithmAttribute (std::string n, const AttributeValue &v);

  /**
   * set an attribute for the LteEnbNetDevice to be created
   * 
   * \param n the name of the attribute
   * \param v the value of the attribute
   */
  void SetEnbDeviceAttribute (std::string n, const AttributeValue &v);

  /** 
   * 
   * \param type the type of AntennaModel to be used for the eNBs
   */
  void SetEnbAntennaModelType (std::string type);

  /**
   * set an attribute for the AntennaModel to be used for the eNBs
   * 
   * \param n the name of the attribute
   * \param v the value of the attribute
   */
  void SetEnbAntennaModelAttribute (std::string n, const AttributeValue &v);

  /**
   * set an attribute for the LteUeNetDevice to be created
   *
   * \param n the name of the attribute
   * \param v the value of the attribute
   */
  void SetUeDeviceAttribute (std::string n, const AttributeValue &v);

  /** 
   * 
   * \param type the type of AntennaModel to be used for the UEs
   */
  void SetUeAntennaModelType (std::string type);

  /**
   * set an attribute for the AntennaModel to be used for the UEs
   * 
   * \param n the name of the attribute
   * \param v the value of the attribute
   */
  void SetUeAntennaModelAttribute (std::string n, const AttributeValue &v);

  /** 
   * 
   * \param type the type of SpectrumChannel to be used for the UEs
   */
  void SetSpectrumChannelType (std::string type);

  /**
   * set an attribute for the SpectrumChannel to be used for the UEs
   * 
   * \param n the name of the attribute
   * \param v the value of the attribute
   */
  void SetSpectrumChannelAttribute (std::string n, const AttributeValue &v);

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
   * \brief Enables automatic attachment of a set of UE devices to a suitable
   *        cell using Idle mode initial cell selection procedure.
   * \param ueDevices the set of UE devices to be attached
   *
   * By calling this, the UE will start the initial cell selection procedure at
   * the beginning of simulation. In addition, the function also instructs each
   * UE to immediately enter CONNECTED mode and activates the default EPS
   * bearer.
   *
   * If this function is called when the UE is in a situation where entering
   * CONNECTED mode is not possible (e.g. before the simulation begin), then the
   * UE will attempt to connect at the earliest possible time (e.g. after it
   * camps to a suitable cell).
   *
   * Note that this function can only be used in EPC-enabled simulation.
   */
  void Attach (NetDeviceContainer ueDevices);

  /**
   * \brief Enables automatic attachment of a UE device to a suitable cell
   *        using Idle mode initial cell selection procedure.
   * \param ueDevice the UE device to be attached
   *
   * By calling this, the UE will start the initial cell selection procedure at
   * the beginning of simulation. In addition, the function also instructs the
   * UE to immediately enter CONNECTED mode and activates the default EPS
   * bearer.
   *
   * If this function is called when the UE is in a situation where entering
   * CONNECTED mode is not possible (e.g. before the simulation begin), then the
   * UE will attempt to connect at the earliest possible time (e.g. after it
   * camps to a suitable cell).
   *
   * Note that this function can only be used in EPC-enabled simulation.
   */
  void Attach (Ptr<NetDevice> ueDevice);

  /**
   * \brief Manual attachment of a set of UE devices to the network via a given
   *        eNodeB.
   * \param ueDevices the set of UE devices to be attached
   * \param enbDevice the destination eNodeB device
   *
   * In addition, the function also instructs each UE to immediately enter
   * CONNECTED mode and activates the default EPS bearer.
   *
   * The function can be used in both LTE-only and EPC-enabled simulations.
   * Note that this function will disable Idle mode initial cell selection
   * procedure.
   */
  void Attach (NetDeviceContainer ueDevices, Ptr<NetDevice> enbDevice);

  /**
   * \brief Manual attachment of a UE device to the network via a given eNodeB.
   * \param ueDevice the UE device to be attached
   * \param enbDevice the destination eNodeB device
   *
   * In addition, the function also instructs the UE to immediately enter
   * CONNECTED mode and activates the default EPS bearer.
   *
   * The function can be used in both LTE-only and EPC-enabled simulations.
   * Note that this function will disable Idle mode initial cell selection
   * procedure.
   */
  void Attach (Ptr<NetDevice> ueDevice, Ptr<NetDevice> enbDevice);

  /** 
   * \brief Manual attachment of a set of UE devices to the network via the
   *        closest eNodeB (with respect to distance) among those in the set.
   * \param ueDevices the set of UE devices to be attached
   * \param enbDevices the set of eNodeB devices to be considered
   * 
   * This function finds among the eNodeB set the closest eNodeB for each UE,
   * and then invokes manual attachment between the pair.
   * 
   * Users are encouraged to use automatic attachment (Idle mode cell selection)
   * instead of this function.
   * 
   * \sa LteHelper::Attach(NetDeviceContainer ueDevices);
   */
  void AttachToClosestEnb (NetDeviceContainer ueDevices, NetDeviceContainer enbDevices);

  /** 
   * \brief Manual attachment of a UE device to the network via the closest
   *        eNodeB (with respect to distance) among those in the set.
   * \param ueDevice the UE device to be attached
   * \param enbDevices the set of eNodeB devices to be considered
   *
   * This function finds among the eNodeB set the closest eNodeB for the UE,
   * and then invokes manual attachment between the pair.
   * 
   * Users are encouraged to use automatic attachment (Idle mode cell selection)
   * instead of this function.
   *
   * \sa LteHelper::Attach(Ptr<NetDevice> ueDevice);
   */
  void AttachToClosestEnb (Ptr<NetDevice> ueDevice, NetDeviceContainer enbDevices);

  /**
   * Activate a dedicated EPS bearer on a given set of UE devices
   *
   * \param ueDevices the set of UE devices
   * \param bearer the characteristics of the bearer to be activated
   * \param tft the Traffic Flow Template that identifies the traffic to go on this bearer
   */
  void ActivateDedicatedEpsBearer (NetDeviceContainer ueDevices, EpsBearer bearer, Ptr<EpcTft> tft);

  /**
   * Activate a dedicated EPS bearer on a given UE device
   *
   * \param ueDevice the UE device
   * \param bearer the characteristics of the bearer to be activated
   * \param tft the Traffic Flow Template that identifies the traffic to go on this bearer
   */
  void ActivateDedicatedEpsBearer (Ptr<NetDevice> ueDevice, EpsBearer bearer, Ptr<EpcTft> tft);


  /**
   * Create an X2 interface between all the eNBs in a given set
   *
   * \param enbNodes the set of eNB nodes
   */
  void AddX2Interface (NodeContainer enbNodes);

  /**
   * Create an X2 interface between two eNBs
   *
   * \param enbNode1 one eNB of the X2 interface
   * \param enbNode2 the other eNB of the X2 interface
   */
  void AddX2Interface (Ptr<Node> enbNode1, Ptr<Node> enbNode2);

  /**
   * \brief Manually trigger an X2-based handover of a UE between two eNBs at a
   *        specific simulation time.
   * \param hoTime when the Handover is initiated
   * \param ueDev the UE that hands off, must be of the type LteUeNetDevice
   * \param sourceEnbDev source eNB, must be of the type LteEnbNetDevice
   *                     (originally the UE is attached to this eNB)
   * \param targetEnbDev target eNB, must be of the type LteEnbNetDevice
   *                     (the UE is finally connected to this eNB)
   *
   * \warning Requires the use of EPC mode. See SetEpcHelper() method.
   */
  void HandoverRequest (Time hoTime, Ptr<NetDevice> ueDev,
                        Ptr<NetDevice> sourceEnbDev, Ptr<NetDevice> targetEnbDev);


  /** 
   * Call ActivateDataRadioBearer (ueDevice, bearer) for each UE
   * device in a given set
   * 
   * \param ueDevices the set of UE devices
   * \param bearer
   */
  void ActivateDataRadioBearer (NetDeviceContainer ueDevices,  EpsBearer bearer);

  /** 
   * Activate a Data Radio Bearer for a simplified LTE-only simulation
   * without EPC. This method will schedule the actual activation of
   * the bearer so that it happens after the UE got connected.
   * 
   * \param ueDevice the device of the UE for which the radio bearer
   * is to be activated
   * \param bearer the characteristics of the bearer to be activated
   */
  void ActivateDataRadioBearer (Ptr<NetDevice> ueDevice,  EpsBearer bearer);

  /** 
   * 
   * 
   * \param model the fading model to be used
   */
  void SetFadingModel (std::string model);

  /**
   * set an attribute of the fading model
   *
   * \param n the name of the attribute
   * \param v the value of the attribute
   */
  void SetFadingModelAttribute (std::string n, const AttributeValue &v);

  /**
   * Enables logging for all components of the LENA architecture
   *
   */
  void EnableLogComponents (void);

  /**
   * Enables trace sinks for PHY, MAC, RLC and PDCP. To make sure all nodes are
   * traced, traces should be enabled once all UEs and eNodeBs are in place and
   * connected, just before starting the simulation.
   */
  void EnableTraces (void);

  /**
   * Enable trace sinks for PHY layer
   */
  void EnablePhyTraces (void);



  /**
   * Enable trace sinks for DL PHY layer
   */
  void EnableDlPhyTraces (void);

  /**
   * Enable trace sinks for UL PHY layer
   */
  void EnableUlPhyTraces (void);

  /**
   * Enable trace sinks for DL transmission PHY layer
   */
  void EnableDlTxPhyTraces (void);

  /**
   * Enable trace sinks for UL transmission PHY layer
   */
  void EnableUlTxPhyTraces (void);

  /**
   * Enable trace sinks for DL reception PHY layer
   */
  void EnableDlRxPhyTraces (void);

  /**
   * Enable trace sinks for UL reception PHY layer
   */
  void EnableUlRxPhyTraces (void);

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
   * 
   * \return the RLC stats calculator object
   */
  Ptr<RadioBearerStatsCalculator> GetRlcStats (void);

  /**
   * Enable trace sinks for PDCP layer
   */
  void EnablePdcpTraces (void);

  /** 
   * 
   * \return the PDCP stats calculator object
   */
  Ptr<RadioBearerStatsCalculator> GetPdcpStats (void);

  enum LteEpsBearerToRlcMapping_t {RLC_SM_ALWAYS = 1,
                                   RLC_UM_ALWAYS = 2,
                                   RLC_AM_ALWAYS = 3,
                                   PER_BASED = 4};

  /**
  * Assign a fixed random variable stream number to the random variables
  * used by this model. Return the number of streams (possibly zero) that
  * have been assigned. The Install() method should have previously been
  * called by the user.
  *
  * \param c NetDeviceContainer of the set of net devices for which the 
  *          LteNetDevice should be modified to use a fixed stream
  * \param stream first stream index to use
  * \return the number of stream indices assigned by this helper
  */
  int64_t AssignStreams (NetDeviceContainer c, int64_t stream);

protected:
  // inherited from Object
  virtual void DoInitialize (void);

private:
  /**
   * \brief Create an eNodeB device (LteEnbNetDevice) on the given node.
   * \param n the node where the device is to be installed
   * \return pointer to the created device
   */
  Ptr<NetDevice> InstallSingleEnbDevice (Ptr<Node> n);

  /**
   * \brief Create a UE device (LteUeNetDevice) on the given node.
   * \param n the node where the device is to be installed
   * \return pointer to the created device
   */
  Ptr<NetDevice> InstallSingleUeDevice (Ptr<Node> n);

  /**
   * \brief The actual function to trigger a manual handover.
   * \param ueDev the UE that hands off, must be of the type LteUeNetDevice
   * \param sourceEnbDev source eNB, must be of the type LteEnbNetDevice
   *                     (originally the UE is attached to this eNB)
   * \param targetEnbDev target eNB, must be of the type LteEnbNetDevice
   *                     (the UE is finally connected to this eNB)
   *
   * This method is normally scheduled by HandoverRequest() to run at a specific
   * time where a manual handover is desired by the simulation user.
   */
  void DoHandoverRequest (Ptr<NetDevice> ueDev,
                          Ptr<NetDevice> sourceEnbDev,
                          Ptr<NetDevice> targetEnbDev);

  Ptr<SpectrumChannel> m_downlinkChannel;
  Ptr<SpectrumChannel> m_uplinkChannel;

  Ptr<Object> m_downlinkPathlossModel;
  Ptr<Object> m_uplinkPathlossModel;

  ObjectFactory m_schedulerFactory;
  ObjectFactory m_ffrAlgorithmFactory;
  ObjectFactory m_handoverAlgorithmFactory;
  ObjectFactory m_propagationModelFactory;
  ObjectFactory m_enbNetDeviceFactory;
  ObjectFactory m_enbAntennaModelFactory;
  ObjectFactory m_ueNetDeviceFactory;
  ObjectFactory m_ueAntennaModelFactory;

  ObjectFactory m_dlPathlossModelFactory;
  ObjectFactory m_ulPathlossModelFactory;

  ObjectFactory m_channelFactory;

  std::string m_fadingModelType;
  ObjectFactory m_fadingModelFactory;
  Ptr<SpectrumPropagationLossModel> m_fadingModule;
  bool m_fadingStreamsAssigned;

  Ptr<PhyStatsCalculator> m_phyStats;
  Ptr<PhyTxStatsCalculator> m_phyTxStats;
  Ptr<PhyRxStatsCalculator> m_phyRxStats;
  Ptr<MacStatsCalculator> m_macStats;
  Ptr<RadioBearerStatsCalculator> m_rlcStats;
  Ptr<RadioBearerStatsCalculator> m_pdcpStats;
  RadioBearerStatsConnector m_radioBearerStatsConnector;

  Ptr<EpcHelper> m_epcHelper;

  uint64_t m_imsiCounter;
  uint16_t m_cellIdCounter;

  bool m_useIdealRrc;
  bool m_isAnrEnabled;

  bool m_usePdschForCqiGeneration;
};


} // namespace ns3



#endif // LTE_HELPER_H
