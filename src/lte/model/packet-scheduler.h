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

#ifndef PACKET_SCHEDULER_H
#define PACKET_SCHEDULER_H


#include <ns3/nstime.h>
#include <ns3/object.h>
#include <list>
#include <ns3/ptr.h>


namespace ns3 {

class EnbNetDevice;
class UeNetDevice;
class MacEntity;

/**
 * \ingroup lte
 *
 * This class represents the basic implementation of the LTE packet scheduler
 */
class PacketScheduler : public Object
{

public:
  PacketScheduler ();

  /**
   * Create a packet scheduler
   * \param enb the device where the packet scheduler works
   */
  PacketScheduler (Ptr<EnbNetDevice> enb);
  virtual ~PacketScheduler ();
  virtual void DoDispose (void);

  static TypeId GetTypeId (void);

  /**
   * \brief Set the device where the scheduler works
   * \param enb the device where the scheduler works
   */
  void SetDevice (Ptr<EnbNetDevice> enb);
  /**
   * \brief Get the device where the scheduler works
   * \return the pointer to the device where the scheduler works
   */
  Ptr<EnbNetDevice> GetDevice ();

  /**
   * \brief Set the MAC entity of the device where the scheduler works
   * \param mac the mac entity 
   */
  void SetMacEntity (Ptr<MacEntity> mac);
  /**
   * \brief Get the MAC entity of the device where the scheduler works
   * \return the pointer to the mac the mac entity 
   */
  Ptr<MacEntity> GetMacEntity (void);

  /**
   * \brief run the scheduling algorithm
   */
  void RunPacketScheduler (void);

  /**
   * \brief run a particular scheduling strategy for both uplink and downlink
   */
  virtual void DoRunPacketScheduler (void) = 0;


private:
  Ptr<EnbNetDevice> m_enb;
  Ptr<MacEntity> m_macEntity;
};


}

#endif /* PACKET_SCHEDULER_H */
