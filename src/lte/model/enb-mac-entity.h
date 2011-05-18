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

#ifndef ENB_MAC_ENTITY_H
#define ENB_MAC_ENTITY_H

#include "ns3/object.h"
#include <list>
#include "mac-entity.h"

namespace ns3 {

class PacketScheduler;
class CqiIdealControlMessage;
class PdcchMapIdealControlMessage;

/**
 * \ingroup lte
 *
 * This class implements the MAC layer of the eNodeB device
 */
class EnbMacEntity : public MacEntity
{
public:
  static TypeId GetTypeId (void);

  EnbMacEntity (void);
  virtual ~EnbMacEntity (void);
  virtual void DoDispose (void);

  /**
   * \brief Set the uplink packet scheduler
   * \param s the packet scheduler for the uplink
   */
  void SetUplinkPacketScheduler (Ptr<PacketScheduler> s);
  /**
   * \brief Set the downlink packet scheduler
   * \param s the packet scheduler for the downlink
   */
  void SetDownlinkPacketScheduler (Ptr<PacketScheduler> s);
  /**
   * \brief Get the uplink packet scheduler
   * \return the pointer to the uplink packet scheduler
   */
  Ptr<PacketScheduler> GetUplinkPacketScheduler (void);
  /**
   * \brief Get the downlink packet scheduler
   * \return the pointer to the downlink packet scheduler
   */
  Ptr<PacketScheduler> GetDownlinkPacketScheduler (void);

  /**
   * \brief Receive a CQI ideal control message
   * \param msg the message
   */
  void ReceiveCqiIdealControlMessage  (Ptr<CqiIdealControlMessage> msg);

  /**
   * \brief Send the PDCCH control message
   * \param msg the message
   */
  void SendPdcchMapIdealControlMessage  (Ptr<PdcchMapIdealControlMessage> msg);

private:
  Ptr<PacketScheduler> m_uplinkScheduler;
  Ptr<PacketScheduler> m_downlinkScheduler;
};


} // namespace ns3

#endif /* ENB_MAC_ENTITY_H */
