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

#ifndef RRC_ENTITY_H
#define RRC_ENTITY_H

#include "ns3/object.h"
#include <list>
#include "radio-bearer-instance.h"
#include <vector>

namespace ns3 {

class IpcsClassifierRecord;
class BearerManager;
class Bearer;

/**
 * \ingroup lte
 *
 * \brief This class implements the RRC entity
 */
class RrcEntity : public Object
{
public:
  static TypeId GetTypeId (void);

  RrcEntity (void);
  virtual ~RrcEntity (void);
  virtual void DoDispose (void);

  /**
   * \brief A cointainer of bearer with the same QoS requirements
   */
  typedef std::vector< Ptr<RadioBearerInstance> > BearersContainer;

  /**
   * \brief Get all GBR downlink bearers of this node
   * \return a pointer to the BearersContainer of downlink GBR bearers
   */
  BearersContainer* GetDownlinkGbrBearers (void) const;

  /**
   * \brief Get all NGBR downlink bearers of this node
   * \return a pointer to the BearersContainer of downlink NGBR bearers
   */
  BearersContainer* GetDownlinkNgbrBearers (void) const;

  /**
   * \brief Get all GBR uplink bearers of this node
   * \return a pointer to the BearersContainer of uplink GBR bearers
   */
  BearersContainer* GetUplinkGbrBearers (void) const;

  /**
   * \brief Get all NGBR uplink bearers of this node
   * \return a pointer to the BearersContainer of uplink NGBR bearers
   */
  BearersContainer* GetUplinkNgbrBearers (void) const;


  /**
   * \brief Add a bearer
   * \param  bearer tyhe bearer to be added
   */
  void AddDownlinkGbrBearer (Ptr<RadioBearerInstance> bearer);

  /**
   * \brief Add a bearer
   * \param  bearer tyhe bearer to be added
   */
  void AddDownlinkNgbrBearer (Ptr<RadioBearerInstance> bearer);

  /**
   * \brief Add a bearer
   * \param  bearer tyhe bearer to be added
   */
  void AddUplinkGbrBearer (Ptr<RadioBearerInstance> bearer);

  /**
   * \brief Add a bearer
   * \param  bearer tyhe bearer to be added
   */
  void AddUplinkNgbrBearer (Ptr<RadioBearerInstance> bearer);


  /**
   * \brief Classify an incoming packet into a proper bearer
   * \param p the packet to classify
   * \return a pointer to the selected bearer
   */
  Ptr<RadioBearerInstance> Classify (Ptr<Packet> p) const;

  /**
   * \brief Get a default bearer
   * \return a pointer to the defoult bearer
   */
  Ptr<RadioBearerInstance> GetDefaultBearer (void);

private:

  void DisposeAllElements (BearersContainer *c);

  BearersContainer* m_downlinkGbrBearersContainer;
  BearersContainer* m_downlinkNgbrBearersContainer;

  BearersContainer* m_uplinkGbrBearersContainer;
  BearersContainer* m_uplinkNgbrBearersContainer;

  Ptr<RadioBearerInstance> m_defaultBearer;

};


} // namespace ns3

#endif /* RRC_ENTITY_H */
