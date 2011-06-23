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
 * Author: Giuseppe Piro <g.piro@poliba.it>
 */

#ifndef UPLINK_SCHEDULER_RTPS_H
#define UPLINK_SCHEDULER_RTPS_H

#include <stdint.h>
#include "ul-mac-messages.h"
#include "ns3/nstime.h"
#include "wimax-phy.h"
#include "bs-uplink-scheduler.h"

namespace ns3 {

class BaseStationNetDevice;
class SSRecord;
class ServiceFlow;

/**
 * \ingroup wimax
 * \brief This class implements a simple uplink scheduler for rtPS flows.
 *
 * The UL-rtPS-Scheduler work in this way:
 * At the beginning of the UL sub-frame some symbols are allocated
 * to the rangingInterval Messages, DSA Messages, UGS grants and
 * UGS/rtPS/nrtPS/BE poll.
 * Then, the remaining available symbols are allocated to the all
 * rtPS flows according to their requestedBandwidth.
 * The bandwidth saturation control has been implemented to
 * redistribute the resource if the sum of requestedBandwidth exceed
 * the available bandwidth.
 * At the end, if there are some free symbols, their are allocated to
 * nrtPS and BE Connections.
 */

class UplinkSchedulerRtps : public UplinkScheduler
{
public:
  UplinkSchedulerRtps ();
  UplinkSchedulerRtps (Ptr<BaseStationNetDevice> bs);
  ~UplinkSchedulerRtps (void);

  static TypeId GetTypeId (void);

  std::list<OfdmUlMapIe> GetUplinkAllocations (void) const;

  /**
   * Determines if channel descriptors sent in the current frame are
   * required to be updated
   */
  void GetChannelDescriptorsToUpdate (bool&, bool&, bool&, bool&);
  uint32_t CalculateAllocationStartTime (void);
  void AddUplinkAllocation (OfdmUlMapIe &ulMapIe,
                            const uint32_t &allocationSize,
                            uint32_t &symbolsToAllocation,
                            uint32_t &availableSymbols);
  /**
   * \brief Schedule function.
   */
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
  /**
   * \brief Uplink Scheduler for rtPS connections.
   *
   * \param symbolsToAllocation symbols to allocation for UpLink Subframe
   * \param availableSymbols available symbols for rtPS flows
   *
   * This method represent the UL Scheduler for rtPS  connections.
   * The scheduler is designed to serve all rtPS connections that
   * have at least one packet to transmit, every UL Subframe.
   * The methos selects all rtPS connection that have al least one
   * packet to transmit. To each rtPS connections are allocated,
   * at the beginning, the bandwidth to transmit a quota of data equal
   * to (RequestedBandwidth - GrantedBandwidth).
   * If the sum of all pre-allocated bandwidth exceed the available
   * symbols for rtPS connections, the Channel Saturation Control function
   * is called to redistribute the available bandwidth.
   */
  void ULSchedulerRTPSConnection (uint32_t &symbolsToAllocation, uint32_t &availableSymbols);
  void AllocateInitialRangingInterval (uint32_t &symbolsToAllocation, uint32_t &availableSymbols);
  void SetupServiceFlow (SSRecord *ssRecord, ServiceFlow *serviceFlow);

  void ProcessBandwidthRequest (const BandwidthRequestHeader &bwRequestHdr);

  void InitOnce (void);

  void OnSetRequestedBandwidth (ServiceFlowRecord *sfr);

private:
  std::list<OfdmUlMapIe> m_uplinkAllocations;

};

} // namespace ns3

#endif /* UPLINK_SCHEDULER_RTPS_H */
