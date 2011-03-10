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
 *         Nicola Baldo <nbaldo@cttc.es>
 */

#ifndef UE_NET_DEVICE_H
#define UE_NET_DEVICE_H

#include "lte-net-device.h"
#include "ns3/event-id.h"
#include "ns3/mac48-address.h"
#include "ns3/traced-callback.h"
#include "ns3/nstime.h"
#include "ns3/log.h"
#include "lte-phy.h"
#include "lte-phy.h"


namespace ns3 {

class Packet;
class PacketBurst;
class Node;
class LtePhy;
class UeLtePhy;
class EnbNetDevice;
class LteUeMac;
class LteUeRrc;

/**
 * The UeNetDevice class implements the UE net device
 */
class UeNetDevice : public LteNetDevice
{
public:
  static TypeId GetTypeId (void);

  UeNetDevice (void);
  /**
   * \brief Create an UE net device
   * \param node
   * \param phy
   */
  UeNetDevice (Ptr<Node> node, Ptr<UeLtePhy> phy);

  virtual ~UeNetDevice (void);
  virtual void DoDispose ();


  Ptr<LteUeMac> GetMac (void);

  Ptr<LteUeRrc> GetRrc ();

  Ptr<UeLtePhy> GetPhy (void) const;

  /**
   * \brief Initialize the UE
   */

  void InitUeNetDevice (void);

  /**
   * \brief Set the targer eNB where the UE is registered
   * \param enb
   */
  void SetTargetEnb (Ptr<EnbNetDevice> enb);
  /**
   * \brief Get the targer eNB where the UE is registered
   * \return the pointer to the enb
   */
  Ptr<EnbNetDevice> GetTargetEnb (void);


private:
  bool DoSend (Ptr<Packet> packet,
               const Mac48Address& source,
               const Mac48Address& dest,
               uint16_t protocolNumber);

  void DoReceive (Ptr<Packet> p);

  Ptr<EnbNetDevice> m_targetEnb;

  Ptr<LteUeMac> m_mac;
  Ptr<UeLtePhy> m_phy;
  Ptr<LteUeRrc> m_rrc;
};

} // namespace ns3

#endif /* UE_NET_DEVICE_H */
