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
#include "ie-dot11s-perr.h"

namespace ns3 {

class MeshWifiInterfaceMac;

namespace dot11s {

class HwmpProtocol;
class IePreq;
class IePrep;
class IePerr;

/**
 * \ingroup dot11s
 * 
 * \brief Interface MAC plugin for HWMP -- 802.11s routing protocol
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
  /// Update beacon is empty, because HWMP does not know anything about beacons
  void UpdateBeacon (MeshWifiBeacon & beacon) const {};
  //\}
  
private:
  friend class HwmpProtocol;
  
  ///\name Intercation with HWMP:
  //\{
  void SendPreq(IePreq preq);
  void SendPrep(IePrep prep, Mac48Address receiver);
  void SendOnePerr(IePerr perr, std::vector<Mac48Address> receivers);
  void RequestDestination (Mac48Address dest);
  //\}
  
  /// Sends one PREQ when PreqMinInterval after last PREQ expires (if any PREQ exists in rhe queue)
  void SendOnePreq ();
  void SendPerr ();
private:
  Ptr<MeshWifiInterfaceMac> m_parent;
  uint32_t m_ifIndex;
  Ptr<HwmpProtocol> m_protocol;
  
  ///\name PREQ queue and PREQ timer:
  //\{
  EventId  m_preqTimer;
  std::vector<IePreq>  m_preqQueue;
  std::vector<IePreq>::iterator  m_myPreq;
  //\}
  ///\name PERR timer and stored path error
  //{
  EventId m_perrTimer;
  struct MyPerr {
    IePerr perr;
    std::vector<Mac48Address> receivers;
  };
  MyPerr m_myPerr;
};

#if 0
class HwmpMacPlugin : public MeshWifiInterfaceMacPlugin {
public:
  private:
  //true means that we can add a destination to
  //existing PREQ element
  //False means that we must send
  void  SendPrep (
    Mac48Address dst, //dst is PREQ's originator address
    Mac48Address src, //src is PREQ's destination address
    Mac48Address retransmitter,
    uint32_t initMetric,
    uint32_t dsn,/* taken form PREQ*/
    uint32_t originatorDsn, //taken from rtable or as m_myDsn ++;
    uint32_t lifetime //taken from PREQ
  );
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
   IeDot11sPerr  m_myPerr;
  EventId  m_perrTimer;
  void  SendOnePerr ();
};
#endif
} //namespace dot11s
} //namespace ns3
#endif
