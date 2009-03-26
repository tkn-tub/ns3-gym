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
#include <map>

namespace ns3 {
class MeshPointDevice;
class Packet;
class Mac48Address;
namespace dot11s {
class HwmpMacPlugin;
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
  void ReceivePreq(Ptr<IePreq> preq);
  void ReceivePrep(Ptr<IePreq> prep);
  void ReceivePerr(Ptr<IePreq> perr);
  ///\brief MAC-plugin asks wether the frame can be dropeed. Protocol
  //automatically updates seqno.
  //\returns true if frame can be dropped
  //\param uint32_t is the seqno
  //\param Mac48Address is the mesh source addrress of the frame
  bool DropDataFrame(uint32_t, Mac48Address);
private:
  void  SetMaxQueueSize (int maxPacketsPerDestination);
  int  m_maxQueueSize;
  bool  QueuePacket (MeshL2RoutingProtocol::QueuedPacket packet);
  MeshL2RoutingProtocol::QueuedPacket  DequeuePacket (Mac48Address dst);
  void  SendAllPossiblePackets (Mac48Address dst);
private:
  //fields:
  std::map<uint32_t, Ptr<HwmpMacPlugin> > m_interfaces;
  uint32_t m_dataSeqno;
  uint32_t m_hwmpSeqno;
  uint32_t m_maxTtl;
  ///\brief Sequence number filters:
  std::map<Mac48Address, uint32_t,std::less<Mac48Address> >  m_lastDataSeqno;
  std::map<Mac48Address, uint32_t,std::less<Mac48Address> >  m_lastHwmpSeqno;

#if 0
  std::map<Mac48Address, std::queue<QueuedPacket> >  m_rqueue;
  //devices and HWMP states:
  enum DeviceState {
    ENABLED,
    DISABLED
  };
  enum DeviceMode {
    REACTIVE,
    PROACTIVE,
    ROOT
  };
  std::vector<enum DeviceState>  m_states;
  std::vector<enum DeviceMode>   m_modes;
  std::vector<Ptr<HwmpProtocolState> >   m_hwmpStates;
  //Routing table:
  Ptr<HwmpRtable>  m_rtable;
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
   * \brief Interaction with HwmpProtocolState class -
   * request for starting routing discover
   * procedure (reactive route discovery!)
   * \param Mac48Address is destination to be
   * resolved
   */
  std::vector< Callback<void, Mac48Address> >  m_requestCallback;
  /**
   * \brief Callback that shall be executed when
   * need to send Path error
   * \param std::vector<Mac48Address> is the
   * list of unreachable destinations
   * \param std::vector<Mac48Address> is
   * receivers of PERR
   */
  std::vector<Callback<void,std::vector<HwmpRtable::FailedDestination> > >  m_pathErrorCallback;
  void StartPathErrorProcedure (
    std::vector<HwmpRtable::FailedDestination> destinations,
    uint32_t port);
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

  /**
   * \attention mesh seqno is processed at HWMP
   */
  uint32_t m_seqno;
  std::map<Mac48Address, uint32_t/**/>  m_seqnoDatabase;
  //Timers:
  /**
   * /brief checks when last preq was initiated, returns
   * false when retry has not expired
   */
  bool  ShouldSendPreq (Mac48Address dst);
  /**
   * \brief Generates PREQ retry when route is
   * still unresolved after first PREQ
   * If number of retries greater than
   * dot11sParameters::dot11MeshHWMPmaxPREQretries,
   * we return all packets to upper layers
   */
  void  RetryPathDiscovery (Mac48Address dst, uint8_t numOfRetry);
  /**
   * Keeps PREQ retry timers for every
   * destination
   */
  std::map<Mac48Address, EventId>  m_timeoutDatabase;
  /**
   * Configurable parameters:
   */
  uint8_t  m_maxTtl;
  bool     m_broadcastPerr;
#endif
};
} //namespace dot11s
} //namespace ns3
#endif
