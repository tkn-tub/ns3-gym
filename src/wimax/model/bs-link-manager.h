/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008,2009 INRIA, UDcast
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
 * Authors: Jahanzeb Farooq <jahanzeb.farooq@sophia.inria.fr>
 *          Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 *                               <amine.ismail@UDcast.com>
 */

#ifndef LINK_MANAGER_H
#define LINK_MANAGER_H

#include <stdint.h>
#include "wimax-net-device.h"
#include "bs-net-device.h"
#include "ns3/event-id.h"
#include "cid.h"
#include "mac-messages.h"

namespace ns3 {

/**
 * \ingroup wimax
 */
class BSLinkManager : public Object
{
public:
  BSLinkManager (Ptr<BaseStationNetDevice> bs);
  ~BSLinkManager (void);
  /**
   * \return the ranging opportunities to allocate
   */
  uint8_t CalculateRangingOppsToAllocate (void);
  /**
   * \return an available downlink channel
   */
  uint64_t SelectDlChannel (void);
  /**
   * \brief process a ranging request message
   * \param cid the connection identifier in which the ranging message was received
   * \param rngreq the ranging request message
   */

  void ProcessRangingRequest (Cid cid, RngReq rngreq);
  /**
   * \brief Verifies at the end of an invited ranging interval if SS sent ranging message in it or not
   */
  void VerifyInvitedRanging (Cid cid, uint8_t uiuc);
private:
  BSLinkManager (const BSLinkManager &);
  BSLinkManager & operator= (const BSLinkManager &);

  void PerformRanging (Cid cid, RngReq rngreq);
  void PerformInitialRanging (Cid cid, RngReq *rngreq, RngRsp *rngrsp);
  void PerformInvitedRanging (Cid cid, RngRsp *rngrsp);

  void SetParametersToAdjust (RngRsp *rngrsp);
  void AbortRanging (Cid cid, RngRsp *rngrsp, SSRecord *ssRecord, bool isNewSS);
  void AcceptRanging (Cid cid, RngRsp *rngrsp, SSRecord *ssRecord);
  void ContinueRanging (Cid cid, RngRsp *rngrsp, SSRecord *ssRecord);
  void ScheduleRngRspMessage (Cid cid, RngRsp *rngrsp);
  void DeallocateCids (Cid cid);

  bool ChangeDlChannel (void);
  uint32_t GetNewDlChannel (void);
  uint8_t GetSignalQuality (void);
  bool IsRangingAcceptable (void);

  Ptr<BaseStationNetDevice> m_bs;

  // ranging parameters
  uint32_t m_signalQuality;
  uint8_t m_signalQualityThreshold;
  int tries; // variable to test multiple RNG-REQ by the SS for example undecodable at BS or lost RNG-RSP
};

} // namespace ns3

#endif /* LINK_MANAGER_H */
