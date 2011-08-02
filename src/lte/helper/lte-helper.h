/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro <peppe.piro@gmail.com>, <g.piro@poliba.it>
 */


#ifndef LTE_HELPER_H
#define LTE_HELPER_H

#include <string>
#include "ns3/object-factory.h"
#include "ns3/node-container.h"
#include "ns3/net-device-container.h"
#include "ns3/lte-net-device.h"
#include "ns3/enb-net-device.h"
#include "ns3/ue-net-device.h"
#include "ns3/packet-scheduler.h"
#include "ns3/single-model-spectrum-channel.h"
#include "ns3/lte-phy.h"
#include "ns3/ue-phy.h"
#include "ns3/enb-phy.h"
#include <ns3/mobility-model.h>


namespace ns3 {


/**
 * \ingroup lte
 * \brief helps to manage and create LteNetDevice objects
 *
 * This class can help to create a LteNetDevice objects 
 * and to configure their attributes during creation.
 */
class LteHelper 
{
public:
  /**
   * Net Device Type
   * Distinguish a user equipment (UE) device from eNodeB (eNB) device
   */
  enum NetDeviceType
  {
    DEVICE_TYPE_USER_EQUIPMENT, /**< UE device */
    DEVICE_TYPE_ENODEB          /**< eNB device */
  };

  /**
   * \brief Create a Lte helper in an empty state.
   */
  LteHelper (void);
  ~LteHelper (void);


  /**
  * \brief Add mobility model to a physical device
  * \param phy the physical device
  * \param m the mobility model
  */
  void AddMobility (Ptr<LtePhy> phy, Ptr<MobilityModel>m);


  /**
   * \param c a set of nodes
   * \param type device type to create
   */
  NetDeviceContainer Install (NodeContainer c, NetDeviceType type);


  /**
   * \brief register UEs to the target eNB
   * \param ue the UE that will registered to the eNB
   * \param enb the eNB where the UE will registered
   */
  void RegisterUeToTheEnb (Ptr<UeNetDevice> ue, Ptr<EnbNetDevice> enb);

  /**
   * Helper to enable all LTE log components with one statement
   */
  void EnableLogComponents (void);

  /**
  * \brief Create a downlink channel realization between eNB and UE
  * \param enbMobility the enb mobility model
  * \param ueMobility the ue mobility model
  * \param phy the physical layer of the UE
  */
  void AddDownlinkChannelRealization (Ptr<MobilityModel> enbMobility, 
                                      Ptr<MobilityModel> ueMobility,
                                      Ptr<LtePhy> phy);


private:
  /**
   * \brief Create a PHY layer for a LTE device
   * \param dlChannel the downlink channel
   * \param ulChannel the uplink channel
   * \return the pointer to the LTE physical layer
   */
  Ptr<LtePhy> CreatePhy (Ptr<SpectrumChannel> dlChannel, Ptr<SpectrumChannel> ulChannel, NetDeviceType t);

  /**
   * \brief Create a PHY layer for a LTE device
   * \return the pointer to the LTE physical layer
   */
  Ptr<LtePhy> CreatePhy (NetDeviceType t);

  /**
   * \brief Create a PHY layer for a UE LTE device
   * \return the pointer to the UE LTE physical layer
   */
  Ptr<UeLtePhy> CreateUePhy (void);

  /**
   * \brief Create a PHY layer for a eNB LTE device
   * \return the pointer to the eNB LTE physical layer
   */
  Ptr<EnbLtePhy> CreateEnbPhy (void);


  Ptr<SingleModelSpectrumChannel> m_downlinkChannel;
  Ptr<SingleModelSpectrumChannel> m_uplinkChannel;
};

} // namespace ns3

#endif /* LTE_HELPER_H */
