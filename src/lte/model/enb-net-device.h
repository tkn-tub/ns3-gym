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
 * Author: Marco Miozzo <marco.miozzo@cttc.es> : Update to FF API Architecture
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
class EnbLtePhy;
class LteEnbMac;
class LteEnbRrc;
class RrFfMacScheduler;


/**
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
  EnbNetDevice (Ptr<Node> node, Ptr<EnbLtePhy> phy);

  virtual ~EnbNetDevice (void);
  virtual void DoDispose (void);

  /**
   * \brief Initialize all parameters of this device
   */
  void InitEnbNetDevice (void);

  /**
   * \return a pointer to the MAC 
   */
  Ptr<LteEnbMac> GetMac (void);

  /**
   * \return a pointer to the physical layer.
   */
  Ptr<EnbLtePhy> GetPhy (void) const;



  Ptr<LteEnbRrc> GetRrc ();

  /**
   * \brief Send the PDCCH ideal mesages under an
   * ideal control channel
   */
  void SendIdealPdcchMessage (void);


  /** 
   * 
   * \return the Cell Identifier of this eNB
   */
  uint16_t GetCellId ();

private:
  bool DoSend (Ptr<Packet> packet,
               const Mac48Address& source,
               const Mac48Address& dest,
               uint16_t protocolNumber);

  void DoReceive (Ptr<Packet> p);

  // Ptr<UeManager> m_ueManager;

  Ptr<LteEnbMac> m_mac;

  Ptr<EnbLtePhy> m_phy;

  Ptr<LteEnbRrc> m_rrc;

  Ptr<RrFfMacScheduler> m_scheduler;

  uint16_t m_cellId; /**< Cell Identifer. Part of the CGI, see TS 29.274, section 8.21.1  */ 

  static uint16_t m_cellIdCounter; 
};

} // namespace ns3

#endif /* ENB_NET_DEVICE_H */
