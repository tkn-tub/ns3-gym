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

#ifndef BANDWIDTH_MANAGER_H
#define BANDWIDTH_MANAGER_H

#include <stdint.h>
#include "wimax-net-device.h"
#include "ul-job.h"
#include "bs-uplink-scheduler.h"
#include "cid.h"

/*
 The same bandwidth manager class serves both for BS and SS though some functions are exclusive to only one of them.
 */

namespace ns3 {

class SSRecord;
class ServiceFlow;
class UlJob;
class UplinkScheduler;

/**
 * \ingroup wimax
 * \brief This class manage the bandwidth request and grant mechanism.
 * The bandwidth request and grant mechanism is supported by the Bandwidth
 * Manager. Both BS and SS maintain a bandwidth manager. Furthermore BS's
 * bandwidth manager works together with the uplink scheduler to determine
 * total bandwidth available and allocation size for each service flow.
 * Bandwidth request mechanism is a key feature of the WiMAX scheduler
 * since all three non-UGS services explicitly request for bandwidth by
 * sending a bandwidth request to BS.
 */
class BandwidthManager : public Object
{
public:
  BandwidthManager (Ptr<WimaxNetDevice> device);
  ~BandwidthManager (void);
  void DoDispose (void);

  uint32_t CalculateAllocationSize (const SSRecord *ssRecord, const ServiceFlow *serviceFlow);
  ServiceFlow* SelectFlowForRequest (uint32_t &bytesToRequest);
  void SendBandwidthRequest (uint8_t uiuc, uint16_t allocationSize);
  void ProcessBandwidthRequest (const BandwidthRequestHeader &bwRequestHdr);
  void SetSubframeRatio (void);
  uint32_t GetSymbolsPerFrameAllocated (void);
private:
  BandwidthManager (const BandwidthManager &);
  BandwidthManager& operator= (const BandwidthManager &);

  Ptr<WimaxNetDevice> m_device;
  uint16_t m_nrBwReqsSent;
};

} // namespace ns3

#endif /* BANDWIDTH_MANAGER_H */
