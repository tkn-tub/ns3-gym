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

#ifndef SS_RECORD_H
#define SS_RECORD_H

#include <stdint.h>
#include <ostream>
#include "ns3/mac48-address.h"
#include "wimax-connection.h"
#include "wimax-phy.h"
#include "wimax-net-device.h"
#include "ns3/ipv4-address.h"
#include <stdint.h>
#include "service-flow.h"

namespace ns3 {

class ServiceFlow;

/**
 * \ingroup wimax
 * \brief This class is used by the base station to store some information related to subscriber station in the cell.
 */
class SSRecord
{
public:
  SSRecord (void);
  SSRecord (Mac48Address macAddress);
  SSRecord (Mac48Address macAddress, Ipv4Address IPaddress);
  ~SSRecord (void);

  void SetBasicCid (Cid basicCid);
  Cid GetBasicCid (void) const;

  void SetPrimaryCid (Cid primaryCid);
  Cid GetPrimaryCid (void) const;

  void SetMacAddress (Mac48Address macAddress);
  Mac48Address GetMacAddress (void) const;

  uint8_t GetRangingCorrectionRetries (void) const;
  void ResetRangingCorrectionRetries (void);
  void IncrementRangingCorrectionRetries (void);
  uint8_t GetInvitedRangRetries (void) const;
  void ResetInvitedRangingRetries (void);
  void IncrementInvitedRangingRetries (void);
  void SetModulationType (WimaxPhy::ModulationType modulationType);
  WimaxPhy::ModulationType GetModulationType (void) const;

  void SetRangingStatus (WimaxNetDevice::RangingStatus rangingStatus);
  WimaxNetDevice::RangingStatus GetRangingStatus (void) const;

  void EnablePollForRanging (void);
  void DisablePollForRanging (void);
  bool GetPollForRanging (void) const;

  bool GetAreServiceFlowsAllocated (void) const;

  void SetPollMeBit (bool pollMeBit);
  bool GetPollMeBit (void) const;

  void AddServiceFlow (ServiceFlow *serviceFlow);
  std::vector<ServiceFlow*> GetServiceFlows (enum ServiceFlow::SchedulingType schedulingType) const;
  bool GetHasServiceFlowUgs (void) const;
  bool GetHasServiceFlowRtps (void) const;
  bool GetHasServiceFlowNrtps (void) const;
  bool GetHasServiceFlowBe (void) const;

  void SetSfTransactionId (uint16_t sfTransactionId);
  uint16_t GetSfTransactionId (void) const;

  void SetDsaRspRetries (uint8_t dsaRspRetries);
  void IncrementDsaRspRetries (void);
  uint8_t GetDsaRspRetries (void) const;

  void SetDsaRsp (DsaRsp dsaRsp);
  DsaRsp GetDsaRsp (void) const;
  void SetIsBroadcastSS (bool);
  bool GetIsBroadcastSS (void);

  Ipv4Address GetIPAddress (void);
  void SetIPAddress (Ipv4Address IPaddress);
  void SetAreServiceFlowsAllocated (bool val);
private:
  void Initialize (void);

  Mac48Address m_macAddress;
  Ipv4Address m_IPAddress;

  Cid m_basicCid;
  Cid m_primaryCid;

  uint8_t m_rangingCorrectionRetries;
  uint8_t m_invitedRangingRetries;

  WimaxPhy::ModulationType m_modulationType; // least robust burst profile (modulation type) for this SS
  WimaxNetDevice::RangingStatus m_rangingStatus;
  bool m_pollForRanging;
  bool m_areServiceFlowsAllocated;
  bool m_pollMeBit; // if PM (poll me) bit set for this SS
  bool m_broadcast;

  std::vector<ServiceFlow*> *m_serviceFlows;

  // fields for service flow creation
  uint16_t m_sfTransactionId;
  uint8_t m_dsaRspRetries;
  DsaRsp m_dsaRsp;
};

} // namespace ns3

#endif /* SS_RECORD_H */
