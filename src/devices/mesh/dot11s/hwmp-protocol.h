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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 */


#ifndef HWMP_PROTOCOL_H
#define HWMP_PROTOCOL_H

#include "ns3/mesh-l2-routing-protocol.h"
#include "ns3/nstime.h"
#include "ns3/ie-dot11s-perr.h"
#include "ns3/event-id.h"
#include <vector>
#include <map>

namespace ns3 {
class MeshPointDevice;
class Packet;
class Mac48Address;
namespace dot11s {
class HwmpMacPlugin;
class HwmpRtable;
class IePreq;
class IePrep;
/**
 * \ingroup mesh
 */
class HwmpProtocol : public MeshL2RoutingProtocol
{
public:
  static TypeId GetTypeId ();
  HwmpProtocol ();
  ~HwmpProtocol ();
  void DoDispose ();
  bool RequestRoute (uint32_t  sourceIface, const Mac48Address source, const Mac48Address destination,
      Ptr<Packet>  packet, uint16_t  protocolType, RouteReplyCallback  routeReply);
  bool Install (Ptr<MeshPointDevice>);
private:
  friend class HwmpMacPlugin;
  ///\brief Like RequestRoute, but for unicast packets
  bool ForwardUnicast (uint32_t  sourceIface, const Mac48Address source, const Mac48Address destination,
      Ptr<Packet>  packet, uint16_t  protocolType, RouteReplyCallback  routeReply);
  ///\name interaction with HWMP MAC plugin
  //\{
  void ReceivePreq(Ptr<IePreq> preq, Mac48Address from);
  void ReceivePrep(Ptr<IePreq> prep, Mac48Address from);
  void ReceivePerr(Ptr<IePreq> perr, Mac48Address from);
  ///\brief forms a path error information element when list of
  //destination fails on a given interface
  //\param uint32_t is an interface ID, where route has failed
  void MakePathError (Mac48Address, uint32_t);
  ///\return list of addresses where a PERR/PREQ should be sent to
  std::vector<Mac48Address> GetPerrReceivers (std::vector<IePerr::FailedDestination> failedDest, uint32_t port);
  std::vector<Mac48Address> GetPreqReceivers ();
  ///\brief MAC-plugin asks wether the frame can be dropeed. Protocol
  //automatically updates seqno.
  //\returns true if frame can be dropped
  //\param uint32_t is the seqno
  //\param Mac48Address is the mesh source addrress of the frame
  bool DropDataFrame(uint32_t, Mac48Address);
  ///\}
private:
  ///\name Methods related to Queue/Dequeue procedures
  ///\{
  uint16_t m_maxQueueSize;
  bool QueuePacket (MeshL2RoutingProtocol::QueuedPacket packet);
  QueuedPacket  DequeueFirstPacketByDst (Mac48Address dst);
  QueuedPacket  DequeueFirstPacket ();
  void ReactivePathResolved (Mac48Address dst);
  void ProactivePathResolved ();
  ///\}
  ///\name Methods responsible for Path discovery retry procedure:
  //\{
  //\brief checks when the last path discovery procedure was started
  //for a given destination. If the retry counter has not achieved the
  //maximum level - preq should not be sent
  bool  ShouldSendPreq (Mac48Address dst);
  //\brief Generates PREQ retry when retry timeout has expired and
  //route is still unresolved. When PREQ retry has achieved the
  //maximum level - retry mechanish should be cancelled
  void  RetryPathDiscovery (Mac48Address dst, uint8_t numOfRetry);
  ///\}
  ///\name Proactive Preq routines:
  ///\{
  void SetRoot ();
  void UnsetRoot ();
  void SendProactivePreq ();
  ///\}
private:
  //fields:
  typedef std::map<uint32_t, Ptr<HwmpMacPlugin> > PLUGINS;
  PLUGINS m_interfaces;
  uint32_t m_dataSeqno;
  uint32_t m_hwmpSeqno;
  uint32_t m_preqId;
  ///\brief Sequence number filters:
  std::map<Mac48Address, uint32_t,std::less<Mac48Address> > m_lastDataSeqno;
  std::map<Mac48Address, uint32_t,std::less<Mac48Address> > m_lastHwmpSeqno;
  ///\brief Routing table
  Ptr<HwmpRtable> m_rtable;
  ///\name Timers:
  //\{
  std::map<Mac48Address, EventId> m_preqTimeouts;
  EventId m_proactivePreqTimer;
  //\}
  ///\Packet Queue
  std::vector<QueuedPacket> m_rqueue;
private:
  ///\name HWMP-protocol parameters (attributes of GetTypeId)
  ///\{
  uint8_t m_dot11MeshHWMPmaxPREQretries;
  Time m_dot11MeshHWMPnetDiameterTraversalTime;
  Time m_dot11MeshHWMPpreqMinInterval;
  Time m_dot11MeshHWMPperrMinInterval;
  Time m_dot11MeshHWMPactiveRootTimeout;
  Time m_dot11MeshHWMPactivePathTimeout;
  Time m_dot11MeshHWMPpathToRootInterval;
  Time m_dot11MeshHWMPrannInterval;
  bool m_isRoot;
  uint8_t m_maxTtl;
  uint8_t m_unicastPerrThreshold;
  uint8_t m_unicastPreqThreshold;
  bool m_doFlag;
  bool m_rfFlag;
  ///\}
  ///\name Methods needed by HwmpMacLugin to access protocol
  //parameters:
  ///\{
  bool GetDoFlag ();
  bool GetRfFlag ();
  Time GetPreqMinInterval ();
  Time GetPerrMinInterval ();
  uint8_t GetMaxTtl ();
  uint32_t GetNextPreqId ();
  uint32_t GetNextHwmpSeqno ();
  ///\}
};
} //namespace dot11s
} //namespace ns3
#endif
