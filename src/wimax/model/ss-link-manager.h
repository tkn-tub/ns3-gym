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
#include "ss-net-device.h"
#include "ns3/event-id.h"
#include "cid.h"
#include "mac-messages.h"
#include "ns3/nstime.h"

namespace ns3 {

/**
 * \ingroup wimax
 * \brief this class implements the link manager of subscriber station net device. An ss link manager is
 * responsible for link scanning and ranging
 */
class SSLinkManager : public Object
{
public:
  SSLinkManager (Ptr<SubscriberStationNetDevice> ss);
  ~SSLinkManager (void);
  void DoDispose (void);

  void SetBsEirp (uint16_t bs_eirp);
  void SetEirXPIrMax (uint16_t eir_x_p_ir_max);
  void SetRangingIntervalFound (bool rangingIntervalFound);
  bool GetRangingIntervalFound (void) const;
  void SetNrRangingTransOpps (uint8_t nrRangingTransOpps);
  void SetRangingCW (uint8_t rangingCW);
  void IncrementNrInvitedPollsRecvd (void);
  EventId GetDlMapSyncTimeoutEvent (void);

  void PerformRanging (Cid cid, RngRsp rngrsp);
  void StartScanning (SubscriberStationNetDevice::EventType type,
                      bool deleteParameters);
  void SendRangingRequest (uint8_t uiuc, uint16_t allocationSize);
  void StartContentionResolution (void);
  void PerformBackoff (void);
  bool IsUlChannelUsable (void);
  void ScheduleScanningRestart (Time interval,
                                SubscriberStationNetDevice::EventType eventType,
                                bool deleteUlParameters, EventId &eventId);
private:
  SSLinkManager (const SSLinkManager &);
  SSLinkManager & operator= (const SSLinkManager &);

  void EndScanning (bool status, uint64_t frequency);
  void StartSynchronizing (void);
  bool SearchForDlChannel (uint8_t channel);
  void SelectRandomBackoff (void);
  void IncreaseRangingRequestCW (void);
  void ResetRangingRequestCW (void);
  void DeleteUplinkParameters (void);
  void AdjustRangingParameters (const RngRsp &rngrsp);
  void NegotiateBasicCapabilities (void);
  uint16_t CalculateMaxIRSignalStrength (void);
  uint16_t GetMinTransmitPowerLevel (void);

  Ptr<SubscriberStationNetDevice> m_ss;

  WimaxNetDevice::RangingStatus m_rangingStatus;
  // initial ranging parameters obtained from DCD (in channel encodings)
  uint16_t m_bsEirp;
  uint16_t m_eirXPIrMax; // initial ranging maximum equivalent isotropic received power at BS
  uint16_t m_pTxIrMax; // maximum transmit signal strength for initial ranging calculated by SS

  uint8_t m_initRangOppNumber; // Initial Ranging opportunity (1–255) in which SS transmitted the RNG_REQ
  uint8_t m_contentionRangingRetries;
  uint32_t m_rngReqFrameNumber; // frame number in which SS sent RNG_REQ message
  RngReq m_rngreq;

  uint8_t m_dlChnlNr; // indicates the channel/frequency currently the SS is scanning
  uint64_t m_frequency; // frequency on which it is currently operating, i.e., where scanning was successful
  bool m_rangingIntervalFound;

  // stats members
  uint16_t m_nrRngReqsSent;
  uint16_t m_nrRngRspsRecvd;
  uint16_t m_nrInvitedPollsRecvd;

  uint8_t m_rangingCW;
  uint8_t m_rangingBO;
  uint8_t m_nrRangingTransOpps;
  bool m_isBackoffSet;
  uint8_t m_rangingAnomalies;

  EventId m_waitForRngRspEvent, m_dlMapSyncTimeoutEvent;
};

} // namespace ns3

#endif /* LINK_MANAGER_H */
