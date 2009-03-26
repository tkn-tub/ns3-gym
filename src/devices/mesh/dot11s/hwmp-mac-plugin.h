/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */


#ifndef HWMP_STATE_H
#define HWMP_STATE_H

#include "ns3/mesh-wifi-interface-mac-plugin.h"

namespace ns3 {
class MeshWifiInterfaceMac;
namespace dot11s {
class HwmpProtocol;
class IePreq;
class IePrep;
class IePerr;
/**
 * \ingroup mesh
 */
class HwmpMacPlugin : public MeshWifiInterfaceMacPlugin
{
  public:
    HwmpMacPlugin (uint32_t, Ptr<HwmpProtocol>);
    ~HwmpMacPlugin ();
    ///\name Inherited from MAC plugin
    //\{
    void SetParent (Ptr<MeshWifiInterfaceMac> parent);
    bool Receive (Ptr<Packet> packet, const WifiMacHeader & header);
    bool UpdateOutcomingFrame (Ptr<Packet> packet, WifiMacHeader & header, Mac48Address from, Mac48Address to) const;
    ///\brief Update beacon is empty, because HWMP does not know
    //anything about beacons
    void UpdateBeacon (MeshWifiBeacon & beacon) const {};
    //\}
  private:
    friend class HwmpProtocol;
    ///\brief Interaction with protocol:
    void SendPreq(Ptr<IePreq> preq);
    void SendPrep(Ptr<IePreq> prep);
    void SendPerr(Ptr<IePreq> perr);
  private:
    Ptr<MeshWifiInterfaceMac> m_parent;
    uint32_t m_ifIndex;
    Ptr<HwmpProtocol> m_protocol;
};
#if 0
class HwmpMacPlugin : public MeshWifiInterfaceMacPlugin {
public:
  static TypeId GetTypeId ();
  HwmpMacPlugin ();
  ~HwmpMacPlugin ();

  /**
   * \brief Interface with HWMP - Hwmp can only
   * request address and collect routing
   * information
   */
  void SetRequestRouteCallback (
    Callback<HwmpRtable::LookupResult, const Mac48Address&> cb);
  void SetRequestRootPathCallback (
    Callback<HwmpRtable::LookupResult, uint32_t> cb);

  enum InfoType {
    INFO_PREQ,
    INFO_PREP,
    INFO_PERR,
    INFO_PROACTIVE,
    INFO_NEW_PEER,
    INFO_FAILED_PEER
  };
  typedef struct RoutingInfo {
    Mac48Address me;
    Mac48Address destination;
    Mac48Address source;
    Mac48Address nextHop;
    Mac48Address prevHop;
    uint32_t outPort;
    uint32_t metric;
    std::vector<Mac48Address>  failedDestinations;
    uint32_t dsn;
    Time  lifetime;
    enum InfoType type;
  } INFO;
  void SetRoutingInfoCallback (
    Callback<void, INFO> cb
  );
  void SetRetransmittersOfPerrCallback (
    Callback<std::vector<Mac48Address>, std::vector<HwmpRtable::FailedDestination>, uint32_t> cb);
  void RequestDestination (Mac48Address dest);
  void SendPathError (std::vector<HwmpRtable::FailedDestination> destinations);
  void SetAssociatedIfaceId (uint32_t interface);
  uint32_t  GetAssociatedIfaceId ();
  //Mac interaction:
  void SetMac (Ptr<MeshWifiMac> mac);
  void SetSendPreqCallback (
    Callback<void, const IeDot11sPreq&> cb);
  void SetSendPrepCallback (
    Callback<void, const IeDot11sPrep&> cb);
  void SetSendPerrCallback (
    Callback<void, const IeDot11sPerr&, std::vector<Mac48Address> > cb);
  void ReceivePreq (IeDot11sPreq&, const Mac48Address& from, const uint32_t& metric);
  void ReceivePrep (IeDot11sPrep&, const Mac48Address& from, const uint32_t& metric);
  void ReceivePerr (IeDot11sPerr&, const Mac48Address& from);
  void PeerStatus (
    const Mac48Address peerAddress,
    const bool status,
    const uint32_t metric
  );
  //Proactive mode routines:
  bool SetRoot ();
  void UnSetRoot ();
  //external handling:
  void Disable ();
  void Enable ();
  //DEBUG purpose:
  Mac48Address GetAddress ();
private:
  static const uint32_t MAX_PREQ_ID = 0xffffffff;
  static const uint32_t MAX_DSN = 0xffffffff;
  //information about associated port:
  Mac48Address m_address;
  //index when HWMP state is attached
  uint32_t m_ifIndex;
  //timers for PREQ and PREP:
  EventId  m_preqTimer;
  void  SendOnePreq ();
  std::vector<IeDot11sPreq>  m_preqQueue;
  //true means that we can add a destination to
  //existing PREQ element
  //False means that we must send
  EventId  m_prepTimer;
  void  SendPrep (
    Mac48Address dst, //dst is PREQ's originator address
    Mac48Address src, //src is PREQ's destination address
    Mac48Address retransmitter,
    uint32_t initMetric,
    uint32_t dsn,/* taken form PREQ*/
    uint32_t originatorDsn, //taken from rtable or as m_myDsn ++;
    uint32_t lifetime //taken from PREQ
  );
  std::vector<IeDot11sPreq>::iterator  m_myPreq;
  //HWMP interaction callbacks:
  Callback<void, INFO>  m_routingInfoCallback;
  Callback<std::vector<Mac48Address>, std::vector<HwmpRtable::FailedDestination>, uint32_t>  m_retransmittersOfPerrCallback;
  Callback<HwmpRtable::LookupResult, const Mac48Address&>          m_requestRouteCallback;
  Callback<HwmpRtable::LookupResult, uint32_t>                     m_requestRootPathCallback;
  //Mac interaction callbacks:
  Callback<void, const IeDot11sPreq&>                              m_preqCallback;
  Callback<void, const IeDot11sPrep&, const Mac48Address&>         m_prepCallback;
  Callback<void, const IeDot11sPerr&, std::vector<Mac48Address> >  m_perrCallback;
  //HwmpCounters:
  uint32_t m_preqId;
  uint32_t m_myDsn;
  //Seqno and metric database
  std::map<Mac48Address, uint32_t>  m_dsnDatabase;
  std::map<Mac48Address, uint32_t>  m_preqMetricDatabase;
  //Disable/enable functionality
  bool  m_disabled;
  //Proactive PREQ mechanism:
  EventId  m_proactivePreqTimer;
  void  SendProactivePreq ();
  /**
   * \brief Two PERRs may not be sent closer to
   * each other than
   * dot11MeshHWMPperrMinInterval, so, each HWMP
   * state collects all unreachable destinations
   * and once in dot11MeshHWMPperrMinInterval
   * should send PERR, and PERR element should
   * be cleared
   */
  IeDot11sPerr  m_myPerr;
  std::vector<Mac48Address>   m_myPerrReceivers;
  void  AddPerrReceiver (Mac48Address receiver);
  EventId  m_perrTimer;
  void  SendOnePerr ();
  //Configurable parameters:
  uint8_t  m_maxTtl;
};
#endif
} //namespace dot11s
} //namespace ns3
#endif
