/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008, 2009 INRIA, UDcast
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
 * Author: Jahanzeb Farooq <jahanzeb.farooq@sophia.inria.fr>
 *         Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 */

#ifndef SERVICE_FLOW_MANAGER_H
#define SERVICE_FLOW_MANAGER_H

#include <stdint.h>
#include "ns3/event-id.h"
#include "mac-messages.h"
#include "ns3/buffer.h"

namespace ns3 {

class Packet;
class ServiceFlow;
class WimaxNetDevice;
class SSRecord;
class WimaxConnection;

/**
 * \ingroup wimax
 * The same service flow manager class serves both for BS and SS though some functions are exclusive to only one of them.
 */
class ServiceFlowManager : public Object
{
public:
  enum ConfirmationCode // as per Table 384 (not all codes implemented)
  {
    CONFIRMATION_CODE_SUCCESS, CONFIRMATION_CODE_REJECT
  };

  static TypeId GetTypeId (void);

  ServiceFlowManager ();
  ~ServiceFlowManager (void);
  void DoDispose (void);

  void AddServiceFlow (ServiceFlow * serviceFlow);
  ServiceFlow* GetServiceFlow (uint32_t sfid) const;
  ServiceFlow* GetServiceFlow (Cid cid) const;
  std::vector<ServiceFlow*> GetServiceFlows (enum ServiceFlow::SchedulingType schedulingType) const;

  /**
   * \return true if all service flows are allocated, false otherwise
   */
  bool AreServiceFlowsAllocated ();
  /**
   * \param  serviceFlows the list of the service flows to be checked
   * \return true if all service flows are allocated, false otherwise
   */
  bool AreServiceFlowsAllocated (std::vector<ServiceFlow*>* serviceFlows);
  /**
   * \param  serviceFlows the list of the service flows to be checked
   * \return true if all service flows are allocated, false otherwise
   */
  bool AreServiceFlowsAllocated (std::vector<ServiceFlow*> serviceFlows);
  /**
   * \return the next service flow to be allocated
   */
  ServiceFlow* GetNextServiceFlowToAllocate ();

  /**
   * \return the number of all service flows
   */
  uint32_t GetNrServiceFlows (void) const;

  /**
   *\param SrcAddress the source ip address
   *\param DstAddress the destination ip address
   *\param SrcPort the source port
   *\param DstPort the destination port
   *\param Proto the protocol
   *\param dir the direction of the service flow
   *\return the service flow to which this ip flow is associated
   */
  ServiceFlow* DoClassify (Ipv4Address SrcAddress,
                           Ipv4Address DstAddress,
                           uint16_t SrcPort,
                           uint16_t DstPort,
                           uint8_t Proto,
                           ServiceFlow::Direction dir) const;
private:
  std::vector<ServiceFlow*> * m_serviceFlows;
};

} // namespace ns3

#endif /* SERVICE_FLOW_MANAGER_H */
