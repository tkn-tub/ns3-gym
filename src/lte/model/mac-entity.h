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

#ifndef MAC_ENTITY_H
#define MAC_ENTITY_H

#include "ns3/object.h"
#include <list>

namespace ns3 {

class Packet;
class LteNetDevice;
class AmcModule;

/**
 * \ingroup lte
 *
 * This class provides a basi implementation of the MAC
 * layer
 */
class MacEntity : public Object
{
public:
  static TypeId GetTypeId (void);

  MacEntity (void);
  virtual ~MacEntity (void);

  virtual void DoDispose (void);

  /** 
   * \brief Set the device where the mac entity is attached
   * \param d the device
   */
  void SetDevice (Ptr<LteNetDevice> d);
  /** 
   * \brief Get the device where the mac entity is attached
   * \return the pointer to the device
   */
  Ptr<LteNetDevice> GetDevice ();

  /**
   * \brief Set the AMC module
   * \param amcModule the AMC Module.
   */
  void SetAmcModule (Ptr<AmcModule> amcModule);
  /**
   * \brief Get the AMC module
   * \returns a pointer to the AMC Module.
   */
  Ptr<AmcModule> GetAmcModule (void) const;


private:
  Ptr<LteNetDevice> m_device;
  Ptr<AmcModule> m_amcModule;

};


} // namespace ns3

#endif /* MAC_ENTITY_H */
