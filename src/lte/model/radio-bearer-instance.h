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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 */

#ifndef RADIO_BEARER_INSTANCE_H
#define RADIO_BEARER_INSTANCE_H

#include "ns3/object.h"
#include "lte-mac-queue.h"
#include "ns3/ipcs-classifier-record.h"

namespace ns3 {

class BearerQosParameters;
class RlcEntity;

/**
 * \ingroup lte
 *
 * This class implements the LTE radio bearer in which
 * a data flow between UE and eNB will be mapped.
 */
class RadioBearerInstance : public Object
{
public:
  static TypeId GetTypeId (void);

  RadioBearerInstance (void);
  virtual ~RadioBearerInstance (void);

  /**
   * The direction of the bearer
   */
  enum BearerDirection
  {
    DIRECTION_TYPE_UL, DIRECTION_TYPE_DL
  };

  /**
   * The bearer type
   */
  enum BearerType
  {
    /*
     As the mobile device is already known in the core network
     the following radio bearers are now established automatically:
     - A low priority signaling (message) bearer (SRB1)
     - A high priority signaling (message) bearer (SRB2)
     - A data radio bearer (DRB), i.e. a bearer for IP packets
     */
    BEARER_TYPE_SRB1, BEARER_TYPE_SRB2, BEARER_TYPE_DRB
  };

  /**
   * \brief Set direction of the bearer
   * \param direction the direction of the bearer
   */
  void SetBearerDirection (BearerDirection direction);
  /**
   * \brief Get direction of the bearer
   * \return the direction of the bearer
   */
  BearerDirection GetBearerDirection (void) const;

  /**
   * \brief Set the type of bearer
   * \param type the QoS type
   */
  void SetBearerType (BearerType type);
  /**
   * \brief Get the type of bearer
   * \return the type the bearer
   */
  BearerType GetBearerType (void) const;

  /**
   * \return the queue of the bearer
   */
  Ptr<LteMacQueue> GetQueue (void) const;


  /**
   * \brief enqueue a packet in the queue of the bearer
   * \param packet the packet to be enqueued
   */
  bool Enqueue (Ptr<Packet> packet);

  /**
   * \brief dequeue a packet from the queue of the bearer
   */
  Ptr<Packet> Dequeue (void);

  /**
   * \brief dequeue a packet from the queue of the bearer
   * \param availableByte number of bytes can be dequeued
   */
  Ptr<Packet> Dequeue (uint32_t availableByte);

  /**
   * \return true if the bearer has at least one packet in its queue, false otherwise
   */
  bool HasPackets (void) const;

  /**
   * \brief Set a list of QoS parameters of the Bearer
   * \param qosParameters the list of QoS parameters
   */
  void SetQosParameters (Ptr<BearerQosParameters> qosParameters);
  /**
   * \brief Get a list of QoS parameters of the Bearer
   * \return the pointer to the list of QoS parameters
   */
  Ptr<BearerQosParameters> GetQosParameters (void);

  /**
   * \brief Set the RLC entity of this bearer
   * \param rlc the RLC entity
   */
  void SetRlcEntity (Ptr<RlcEntity> rlc);
  /**
   * \brief Get the RLC entity if this bearer
   * \return the pointer to the RLC entity
   */
  Ptr<RlcEntity> GetRlcEntity (void);

  /**
   * \brief Set the ip classifier record
   * \param c the pointer to the ip classifier record
   */
  void SetIpcsClassifierRecord (IpcsClassifierRecord* c);

  /**
   * \brief Get the ip classifier record
   * \return the pointer to the ip classifier record
   */
  IpcsClassifierRecord* GetIpcsClassifierRecord (void);

private:
  virtual void DoDispose (void);

  Ptr<LteMacQueue> m_queue;

  Ptr<BearerQosParameters> m_qosParameters;

  BearerDirection m_direction;
  BearerType m_bearerType;

  Ptr<RlcEntity> m_rlcEntity;

  IpcsClassifierRecord* m_ipcsClassifierRecord;
};


} // namespace ns3

#endif /* RADIO_BEARER_INSTANCE_H */
