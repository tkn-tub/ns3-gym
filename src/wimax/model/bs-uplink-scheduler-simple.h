/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008 INRIA
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
 */

#ifndef UPLINK_SCHEDULER_SIMPLE_H
#define UPLINK_SCHEDULER_SIMPLE_H

#include <stdint.h>
#include "ul-mac-messages.h"
#include "ns3/nstime.h"
#include "wimax-phy.h"
#include "bs-uplink-scheduler.h"
#include "service-flow.h"

namespace ns3 {

class BaseStationNetDevice;
class SSRecord;
class ServiceFlow;

/**
 * \ingroup wimax
 */
class UplinkSchedulerSimple : public UplinkScheduler
{
public:
  UplinkSchedulerSimple (void);
  UplinkSchedulerSimple (Ptr<BaseStationNetDevice> bs);
  ~UplinkSchedulerSimple (void);

  static TypeId GetTypeId (void);

  std::list<OfdmUlMapIe> GetUplinkAllocations (void) const;

  /**
   * \brief Determines if channel descriptors sent in the current frame are
   * required to be updated
   */
  void GetChannelDescriptorsToUpdate (bool&, bool&, bool&, bool&);
  uint32_t CalculateAllocationStartTime (void);
  void AddUplinkAllocation (OfdmUlMapIe &ulMapIe,
                            const uint32_t &allocationSize,
                            uint32_t &symbolsToAllocation,
                            uint32_t &availableSymbols);
  void Schedule (void);
  void ServiceUnsolicitedGrants (const SSRecord *ssRecord,
                                 enum ServiceFlow::SchedulingType schedulingType,
                                 OfdmUlMapIe &ulMapIe,
                                 const WimaxPhy::ModulationType modulationType,
                                 uint32_t &symbolsToAllocation,
                                 uint32_t &availableSymbols);
  void ServiceBandwidthRequests (const SSRecord *ssRecord,
                                 enum ServiceFlow::SchedulingType schedulingType,
                                 OfdmUlMapIe &ulMapIe,
                                 const WimaxPhy::ModulationType modulationType,
                                 uint32_t &symbolsToAllocation,
                                 uint32_t &availableSymbols);
  bool ServiceBandwidthRequests (ServiceFlow *serviceFlow,
                                 enum ServiceFlow::SchedulingType schedulingType,
                                 OfdmUlMapIe &ulMapIe,
                                 const WimaxPhy::ModulationType modulationType,
                                 uint32_t &symbolsToAllocation,
                                 uint32_t &availableSymbols);
  void AllocateInitialRangingInterval (uint32_t &symbolsToAllocation, uint32_t &availableSymbols);
  void SetupServiceFlow (SSRecord *ssRecord, ServiceFlow *serviceFlow);

  void ProcessBandwidthRequest (const BandwidthRequestHeader &bwRequestHdr);

  void InitOnce (void);

  void OnSetRequestedBandwidth (ServiceFlowRecord *sfr);

private:
  std::list<OfdmUlMapIe> m_uplinkAllocations;

};

} // namespace ns3

#endif /* UPLINK_SCHEDULER_SIMPLE_H */
