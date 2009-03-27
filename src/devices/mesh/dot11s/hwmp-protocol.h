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
#include <vector>
#include <map>

namespace ns3 {
class MeshPointDevice;
class Packet;
class Mac48Address;
class EventId;
namespace dot11s {
class HwmpMacPlugin;
class HwmpRtable;
class IePreq;
class IePrep;
class IePerr;
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
  bool RequestRoute (
    uint32_t  sourceIface,
    const Mac48Address source,
    const Mac48Address destination,
    Ptr<Packet>  packet,
    uint16_t  protocolType,
    MeshL2RoutingProtocol::RouteReplyCallback  routeReply
  );
  bool Install (Ptr<MeshPointDevice>);
#if 0
  /**
   * \brief Disables port by index.
   * \details Needed for external modules like
   * clusterization.
   */
  void DisablePort (uint32_t port);
  /**
   * \brief enables port
   */
  void EnablePort (uint32_t port);
  /**
   * \brief Setting proative mode.
   * \details To enable proactive mode you must
   * set a root node. Choosing the route is made
   * in accordance with the following:
   *  1. Find 'reactive' route. If route is
   *  found - send a packet using this
   *  information
   *  2. If there is no reactive route -
   *  find proactive or 'default' route.
   * \attention Comparsion between proactive and
   * reactive routes is incorrect, because we
   * have metric to root MP in te first case and
   * metric to the destination in the second
   * case.
   * \param port is the port where proactive
   * mode should be activated
   */
  void SetRoot (uint32_t port);
  /**
   * \brief Disable root functionality at a
   * given port
   */
  void UnSetRoot (uint32_t port);
  /**
   * \brief HwmpProtocolState retrns to HwmpProtocol class all
   * routing information obtained from all HWMP
   * action frames
   */
  void ObtainRoutingInformation (
    HwmpProtocolState::INFO info
  );
  /**
   * \brief HwmpProtocol state noyifyes that neighbour
   * is dissapeared. HwmpProtocol state knows about peer
   * failure from MAC
   */
  void PeerFailure (Mac48Address peerAddress);
  void SetMaxTtl (uint8_t ttl);
  uint8_t  GetMaxTtl ();
  static const uint32_t MAX_SEQNO = 0xffffffff;
#endif
  //candidate queue is implemented inside the
  //protocol:
private:
  friend class HwmpMacPlugin;
  ///\brief interaction with HWMP MAC plugin
  void ReceivePreq(Ptr<IePreq> preq, Mac48Address from);
  void ReceivePrep(Ptr<IePreq> prep, Mac48Address from);
  void ReceivePerr(Ptr<IePreq> perr, Mac48Address from);
  ///\brief MAC-plugin asks wether the frame can be dropeed. Protocol
  //automatically updates seqno.
  //\returns true if frame can be dropped
  //\param uint32_t is the seqno
  //\param Mac48Address is the mesh source addrress of the frame
  bool DropDataFrame(uint32_t, Mac48Address);
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
private:
  //fields:
  std::map<uint32_t, Ptr<HwmpMacPlugin> > m_interfaces;
  uint32_t m_dataSeqno;
  uint32_t m_hwmpSeqno;
  uint32_t m_maxTtl;
  ///\brief Sequence number filters:
  std::map<Mac48Address, uint32_t,std::less<Mac48Address> >  m_lastDataSeqno;
  std::map<Mac48Address, uint32_t,std::less<Mac48Address> >  m_lastHwmpSeqno;
  ///\brief Routing table
  Ptr<HwmpRtable> m_rtable;
  ///\name Timers:
  //\{
  std::map<Mac48Address, EventId>  m_preqTimeouts;
  //\}
  ///\Packet Queue
  std::vector<QueuedPacket>  m_rqueue;
private:
  ///\name HWMP-protocol parameters
  ///\{
  uint8_t m_dot11MeshHWMPmaxPREQretries;
  Time m_dot11MeshHWMPnetDiameterTraversalTime;
  Time m_dot11MeshHWMPpreqMinInterval;
  Time m_dot11MeshHWMPperrMinInterval;
  Time m_dot11MeshHWMPactiveRootTimeout;
  Time m_dot11MeshHWMPactivePathTimeout;
  Time m_dot11MeshHWMPpathToRootInterval;
  Time m_dot11MeshHWMPrannInterval;
  ///\}
#if 0
  //devices and HWMP states:
  enum DeviceMode {
    REACTIVE,
    PROACTIVE,
    ROOT
  };
  std::vector<enum DeviceMode>   m_modes;
  //Proactive routines:
  /**
   * \brief Set port state as proactive.
   * \details mode is supposed to be proactive
   * when proatcive PREQ with a valid
   * information was received.
   */
  void  SetProactive (uint32_t port);
  /**
   * \brief Checks if the port is root, if true
   * - no default routes must be used at this
   * port
   */
  bool  IsRoot (uint32_t port);
    /**
   * \brief HwmpProtocolState need to know where to
   * retransmit PERR, only HWMP knows how to
   * retransmit it (broadcast/unicast) and only
   * HWMP has accessto routing table
   */
  std::vector<Mac48Address>  GetRetransmittersForFailedDestinations (
    std::vector<HwmpRtable::FailedDestination> failedDest,
    uint32_t port);
  /**
   * \brief Needed by HwmpProtocolState to find routes in case
   * of intermediate reply and choosing the
   * better route
   *
   */
  HwmpRtable::LookupResult  RequestRouteForAddress (const Mac48Address& destination);
  HwmpRtable::LookupResult  RequestRootPathForPort (uint32_t port);
#endif
};
} //namespace dot11s
} //namespace ns3
#endif
