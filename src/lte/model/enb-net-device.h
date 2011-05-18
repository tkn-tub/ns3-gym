/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 */

#ifndef ENB_NET_DEVICE_H
#define ENB_NET_DEVICE_H

#include "lte-net-device.h"
#include "ns3/event-id.h"
#include "ns3/mac48-address.h"
#include "ns3/traced-callback.h"
#include "ns3/nstime.h"
#include "ns3/log.h"
#include "lte-phy.h"
#include <vector>

namespace ns3 {

class Packet;
class PacketBurst;
class Node;
class LtePhy;
class UeManager;
class UeNetDevice;
class EnbMacEntity;


/**
 * \ingroup lte
 *
 * The eNodeB device implementation
 */
class EnbNetDevice : public LteNetDevice
{
public:
  static TypeId GetTypeId (void);

  EnbNetDevice (void);
  /**
   * \brief Create eNB net device
   * \param node the network node
   * \param phy the physical object attache dto it
   */
  EnbNetDevice (Ptr<Node> node, Ptr<LtePhy> phy);

  virtual ~EnbNetDevice (void);
  virtual void DoDispose (void);

  /**
   * \brief Initialize all parameters of this device
   */
  void InitEnbNetDevice (void);

  /**
   * \brief Starts the run of this device 
   */
  void Start (void);
  /**
   * \brief Stops the run of this device 
   */
  void Stop (void);

  /**
   * \brief Set the UE manager
   * \param m the UE manager
   */
  void SetUeManager (Ptr<UeManager> m);
  /**
   * \brief Get the UE manager
   * \return the pointer to the UE manager
   */
  Ptr<UeManager> GetUeManager (void);

  /**
   * \brief Set the MAC entity
   * \param m the MAC entity
   */
  void SetMacEntity (Ptr<EnbMacEntity> m);
  /**
   * \brief Get the MAC entity
   * \return the pointer to the MAC entity
   */
  Ptr<EnbMacEntity> GetMacEntity (void);


  /**
   * \brief Start packet transmission.
   * This functipon will called at the end of downlink scheduling
   * to start the transmission of the burst of packet created by the
   * packet-scheduler.
   */
  void StartTransmission (void);

  /**
   * \brief Send a pachet burst to the physical layer
   * \param p the packet burst
   * \return
   */
  bool SendPacket (Ptr<PacketBurst> p);

  /**
   * \brief Send the PDCCH ideal mesages under an
   * ideal control channel
   */
  void SendIdealPdcchMessage (void);



private:
  bool DoSend (Ptr<Packet> packet,
               const Mac48Address& source,
               const Mac48Address& dest,
               uint16_t protocolNumber);

  void DoReceive (Ptr<Packet> p);

  Ptr<UeManager> m_ueManager;

  Ptr<EnbMacEntity> m_macEntity;
};

} // namespace ns3

#endif /* ENB_NET_DEVICE_H */
