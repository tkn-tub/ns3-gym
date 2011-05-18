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

#ifndef RLC_ENTITY_H
#define RLC_ENTITY_H

#include "ns3/object.h"
#include <list>

namespace ns3 {


class Packet;
class LteNetDevice;
class RadioBearerInstance;

/**
 * \ingroup lte
 *
 * This class implements the RLC entity
 */
class RlcEntity : public Object
{
public:
  static TypeId GetTypeId (void);

  RlcEntity (void);

  /**
   * \brief Create the RLC entity
   * \param d the device where the RLC entity is created
   */
  RlcEntity (Ptr<LteNetDevice> d);
  virtual ~RlcEntity (void);
  virtual void DoDispose (void);


  /** 
   * \brief Set the device where the RLC entity is attached
   * \param d the device
   */
  void SetDevice (Ptr<LteNetDevice> d);
  /** 
   * \brief Get the device where the RLC entity is attached
   * \return the pointer to the device
   */
  Ptr<LteNetDevice> GetDevice ();

  /**
   * \brief Get A packet form the queue
   * \return a pointer to the packet
   */
  Ptr<Packet> Dequeue ();


  /** 
   * \brief Set the bearer where the rlc entity is attached 
   * \param b the radio bearer
   */
  void SetRadioBearer (Ptr<RadioBearerInstance> b);
  /** 
   * \brief Get the bearer where the rlc entity is attached
   * \return the pointer to the radio bearer
   */
  Ptr<RadioBearerInstance> GetRadioBearer (void);

private:
  Ptr<LteNetDevice> m_device;
  Ptr<RadioBearerInstance> m_bearer;

};


} // namespace ns3

#endif /* RLC_ENTITY_H */
