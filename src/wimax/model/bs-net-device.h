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
 * Authors: Jahanzeb Farooq <jahanzeb.farooq@sophia.inria.fr>
 *          Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 */

#ifndef WIMAX_BS_NET_DEVICE_H
#define WIMAX_BS_NET_DEVICE_H

#include "wimax-net-device.h"
#include "ns3/event-id.h"
#include "wimax-connection.h"
#include "ns3/nstime.h"
#include "ns3/mac48-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/bs-service-flow-manager.h"
#include "dl-mac-messages.h"
#include "ipcs-classifier.h"

namespace ns3 {

class Node;
class Packet;
class SSRecord;
class SSManager;
class BSScheduler;
class BurstProfileManager;
class BSLinkManager;
class UplinkScheduler;
class BsServiceFlowManager;

/**
 * \ingroup wimax
 */
class BaseStationNetDevice : public WimaxNetDevice
{
public:
  enum State
  {
    BS_STATE_DL_SUB_FRAME, BS_STATE_UL_SUB_FRAME, BS_STATE_TTG, BS_STATE_RTG
  };

  enum MacPreamble
  {
    SHORT_PREAMBLE = 1, LONG_PREAMBLE
  };

  static TypeId GetTypeId (void);
  BaseStationNetDevice (void);
  BaseStationNetDevice (Ptr<Node> node, Ptr<WimaxPhy> phy);
  BaseStationNetDevice (Ptr<Node> node,
                        Ptr<WimaxPhy> phy,
                        Ptr<UplinkScheduler> uplinkScheduler,
                        Ptr<BSScheduler> bsScheduler);
  ~BaseStationNetDevice (void);
  /**
   * \param initialRangInterval Time between Initial Ranging regions assigned by the BS
   */
  void SetInitialRangingInterval (Time initialRangInterval);
  /**
   * \brief initializes the BS net device and sets its parameters to the default values
   */
  void InitBaseStationNetDevice (void);
  /**
   * \returns Time between Initial Ranging regions assigned by the BS
   */
  Time GetInitialRangingInterval (void) const;
  /**
   * \param dcdInterval Time between transmission of DCD messages
   */
  void SetDcdInterval (Time dcdInterval);
  /**
   * \returns the Time between transmission of DCD messages
   */
  Time GetDcdInterval (void) const;
  /**
   * \param ucdInterval the Time between transmission of UCD messages
   */
  void SetUcdInterval (Time ucdInterval);
  /**
   * \returns Time between transmission of UCD messages
   */
  Time GetUcdInterval (void) const;
  /**
   * \param interval the Wait for DSA/DSC Acknowledge timeout
   */
  void SetIntervalT8 (Time interval);
  /**
   * \returns the Wait for DSA/DSC Acknowledge timeout
   */
  Time GetIntervalT8 (void) const;
  /**
   * \param maxRangCorrectionRetries the number of retries on contention Ranging Requests
   */
  void SetMaxRangingCorrectionRetries (uint8_t maxRangCorrectionRetries);
  /**
   * \returns the number of retries on contention Ranging Requests
   */
  uint8_t GetMaxRangingCorrectionRetries (void) const;
  /**
   * \param maxInvitedRangRetries the number of retries on contention Ranging
   */
  void SetMaxInvitedRangRetries (uint8_t maxInvitedRangRetries);
  /**
   * \returns the number of retries on contention Ranging
   */
  uint8_t GetMaxInvitedRangRetries (void) const;
  /**
   * \param rangReqOppSize The ranging opportunity size in symbols
   */
  void SetRangReqOppSize (uint8_t rangReqOppSize);
  /**
   * \returns The ranging opportunity size in symbols
   */
  uint8_t GetRangReqOppSize (void) const;
  /**
   * \param bwReqOppSize The bandwidth request opportunity size in symbols
   */
  void SetBwReqOppSize (uint8_t bwReqOppSize);
  /**
   * \returns The bandwidth request opportunity size in symbols
   */
  uint8_t GetBwReqOppSize (void) const;
  /**
   * \param dlSymbols the number of symbols in the downlink sub-frame
   */
  void SetNrDlSymbols (uint32_t dlSymbols);
  /**
   * \returns the number of symbols in the downlink sub-frame
   */
  uint32_t GetNrDlSymbols (void) const;
  /**
   * \param ulSymbols the number of symbols in the uplink sub-frame
   */
  void SetNrUlSymbols (uint32_t ulSymbols);
  /**
   * \returns the number of symbols in the uplink sub-frame
   */
  uint32_t GetNrUlSymbols (void) const;
  /**
   * \returns the number dcd messages already sent
   */
  uint32_t GetNrDcdSent (void) const;
  /**
   * \returns the number ucd messages already sent
   */
  uint32_t GetNrUcdSent (void) const;

  Time GetDlSubframeStartTime (void) const;
  Time GetUlSubframeStartTime (void) const;
  uint8_t GetRangingOppNumber (void) const;
  /**
   * \returns a pointer to the SS manager
   */
  Ptr<SSManager> GetSSManager (void) const;
  /**
   * \param ssManager the SS manager to be installed on the BS
   */
  void SetSSManager (Ptr<SSManager> ssManager);
  /**
   * \returns a pointer to the uplink scheduler installed on the device
   */
  Ptr<UplinkScheduler> GetUplinkScheduler (void) const;
  /**
   * \param ulScheduler the ulScheduler to be isnstalled on the BS
   */
  void SetUplinkScheduler (Ptr<UplinkScheduler> ulScheduler);
  /**
   * \returns a pointer to the link manager installed on the BS
   */
  Ptr<BSLinkManager> GetLinkManager (void) const;
  /**
   * \param bsSchedule the downlink scheduler to be installed on the BS
   */
  void SetBSScheduler (Ptr<BSScheduler> bsSchedule);
  /**
   * \returns The BS scheduler installed on the BS
   */
  Ptr<BSScheduler> GetBSScheduler (void) const;

  /**
   * \param linkManager The link manager installed on the BS
   */
  void SetLinkManager (Ptr<BSLinkManager> linkManager);
  /**
   * \returns a pointer to the classifier installed on the BS
   */
  Ptr<IpcsClassifier> GetBsClassifier (void) const;
  /**
   * \param classifier a classifier to be installed on the BS
   */
  void SetBsClassifier (Ptr<IpcsClassifier> classifier);

  Time GetPsDuration (void) const;
  Time GetSymbolDuration (void) const;

  void Start (void);
  void Stop (void);
  /**
   * \brief Enqueue a packet into a connection queue
   * \param packet the packet to be enqueued
   * \param hdrType the mac header type to be appended to the packet
   * \param connection the connection to be used
   */
  bool Enqueue (Ptr<Packet> packet, const MacHeaderType &hdrType, Ptr<WimaxConnection> connection);
  Ptr<WimaxConnection> GetConnection (Cid cid);

  void MarkUplinkAllocations (void);
  void MarkRangingOppStart (Time rangingOppStartTime);

  Ptr<BsServiceFlowManager> GetServiceFlowManager (void) const;
  void SetServiceFlowManager (Ptr<BsServiceFlowManager> );
private:
  void DoDispose (void);
  void StartFrame (void);
  void StartDlSubFrame (void);
  void EndDlSubFrame (void);
  void StartUlSubFrame (void);
  void EndUlSubFrame (void);
  void EndFrame (void);

  bool DoSend (Ptr<Packet> packet, const Mac48Address& source, const Mac48Address& dest, uint16_t protocolNumber);
  void DoReceive (Ptr<Packet> packet);
  /**
   * \brief creates the MAC management messages DL-MAP and UL-MAP
   */
  void CreateMapMessages (void);
  /**
   * \brief creates the channel descriptor MAC management messages DCD and UCD
   */
  void CreateDescriptorMessages (bool sendDcd, bool senUcd);
  void SendBursts (void);

  Ptr<Packet> CreateDlMap (void);
  Ptr<Packet> CreateDcd (void);
  Ptr<Packet> CreateUlMap (void);
  Ptr<Packet> CreateUcd (void);
  void SetDlBurstProfiles (Dcd *dcd);
  void SetUlBurstProfiles (Ucd *ucd);

  void MarkUplinkAllocationStart (Time allocationStartTime);
  void MarkUplinkAllocationEnd (Time allocationEndTime, Cid cid, uint8_t uiuc);
  void UplinkAllocationStart (void);
  void UplinkAllocationEnd (Cid cid, uint8_t uiuc);
  void RangingOppStart (void);

  // parameters defined in Table 342
  Time m_initialRangInterval; // in seconds
  Time m_dcdInterval; // in seconds
  Time m_ucdInterval; // in seconds
  Time m_intervalT8; // in milliseconds, wait for DSA/DSC Acknowledge timeout

  uint8_t m_maxRangCorrectionRetries;
  uint8_t m_maxInvitedRangRetries;
  uint8_t m_rangReqOppSize; // in symbols
  uint8_t m_bwReqOppSize; // in symbols

  uint32_t m_nrDlSymbols;
  uint32_t m_nrUlSymbols;

  // to keep track total number of a certain management messages sent by the BS
  uint32_t m_nrDlMapSent;
  uint32_t m_nrUlMapSent;
  // number of DCDs and UCDs sent even if same
  uint32_t m_nrDcdSent;
  uint32_t m_nrUcdSent;

  uint32_t m_dcdConfigChangeCount;
  uint32_t m_ucdConfigChangeCount;

  uint32_t m_framesSinceLastDcd;
  uint32_t m_framesSinceLastUcd;

  // uint32_t m_nrFrames; //temporarily defined in wimax-net-device, as static
  uint32_t m_nrDlFrames;
  uint32_t m_nrUlFrames;

  // to keep track if number of SSs have changed since the last frame
  uint16_t m_nrSsRegistered;

  uint16_t m_nrDlAllocations;
  uint16_t m_nrUlAllocations;

  Time m_dlSubframeStartTime;
  Time m_ulSubframeStartTime;

  uint8_t m_ulAllocationNumber; // to see UL burst number
  uint8_t m_rangingOppNumber; // current ranging TO number

  CidFactory *m_cidFactory;

  uint32_t m_allocationStartTime;

  Ptr<SSManager> m_ssManager;
  Ptr<UplinkScheduler> m_uplinkScheduler;
  Ptr<BSScheduler> m_scheduler;
  Ptr<BSLinkManager> m_linkManager;
  Ptr<IpcsClassifier> m_bsClassifier;
  Ptr<BsServiceFlowManager> m_serviceFlowManager;
  // same fields as in PHY, for quick access
  Time m_psDuration;
  Time m_symbolDuration;

  TracedCallback<Ptr<const Packet>, Mac48Address, Cid> m_traceBSRx;

  /**
   * The trace source fired when packets come into the "top" of the device
   * at the L3/L2 transition, before being queued for transmission.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_bsTxTrace;

  /**
   * The trace source fired when packets coming into the "top" of the device
   * are dropped at the MAC layer during transmission.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_bsTxDropTrace;

  /**
   * The trace source fired for packets successfully received by the device
   * immediately before being forwarded up to higher layers (at the L2/L3
   * transition).  This is a promiscuous trace.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_bsPromiscRxTrace;

  /**
   * The trace source fired for packets successfully received by the device
   * immediately before being forwarded up to higher layers (at the L2/L3
   * transition).  This is a non- promiscuous trace.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_bsRxTrace;

  /**
   * The trace source fired when packets coming into the "top" of the device
   * are dropped at the MAC layer during reception.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_bsRxDropTrace;
};

} // namespace ns3

#endif /* WIMAX_BS_NET_DEVICE_H */
