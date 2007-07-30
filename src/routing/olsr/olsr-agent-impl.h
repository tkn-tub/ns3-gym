/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/***************************************************************************
 *   Copyright (C) 2004 by Francisco J. Ros                                *
 *   fjrm@dif.um.es                                                        *
 *                                                                         *
 *   Modified on 2007 for NS-3 by Gustavo J. A. M. Carneiro, INESC Porto   *
 *                                <gjc@inescporto.pt>                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef __OLSR_AGENT_IMPL_H__
#define __OLSR_AGENT_IMPL_H__

#include <vector>

#include "olsr-agent.h"
#include "olsr-header.h"
#include "olsr-state.h"

#include "routing-table.h"
#include "repositories.h"

#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/socket.h"
#include "ns3/single-event.h"
#include "ns3/event-collector.h"


namespace ns3 {

using namespace olsr;


class OlsrAgentImpl : public OlsrAgent
{
  friend class OlsrTest;

public:
  OlsrAgentImpl (Ptr<Node> node);

  virtual void Start ();
  virtual void SetMainInterface (uint32_t interface);

private:
  EventCollector m_events;

  /// Address of the routing agent.
  Ipv4Address m_routingAgentAddr;
	
  /// Packets sequence number counter.
  uint16_t m_packetSequenceNumber;
  /// Messages sequence number counter.
  uint16_t m_messageSequenceNumber;
  /// Advertised Neighbor Set sequence number.
  uint16_t m_ansn;
  
  /// HELLO messages' emission interval.
  Time m_helloInterval;
  /// TC messages' emission interval.
  Time m_tcInterval;
  /// MID messages' emission interval.
  Time m_midInterval;
  /// Willingness for forwarding packets on behalf of other nodes.
  uint8_t m_willingness;
  /// Determines if layer 2 notifications are enabled or not.
  bool m_useL2Notifications;
	
  /// Routing table.
  Ptr<RoutingTable> m_routingTable;
  /// Internal state with all needed data structs.
  OlsrState m_state;

  Ptr<Ipv4> m_ipv4;
	
protected:
  void DoDispose ();
  void SendPacket (Packet packet);
	
  /// Increments packet sequence number and returns the new value.
  inline uint16_t GetPacketSequenceNumber ();
  /// Increments message sequence number and returns the new value.
  inline uint16_t GetMessageSequenceNumber ();
	
  void RecvOlsr (Ptr<Socket> socket,
                 const uint8_t *dataPtr, uint32_t dataSize,
                 const Ipv4Address &sourceAddr,
                 uint16_t sourcePort);

  void MprComputation ();
  void RoutingTableComputation ();
  Ipv4Address GetMainAddress (Ipv4Address iface_addr);

  // Timer handlers
  SingleEvent m_helloTimer;
  void HelloTimerExpire ();
  
  SingleEvent m_tcTimer;
  void TcTimerExpire ();

  SingleEvent m_midTimer;
  void MidTimerExpire ();

  void DupTupleTimerExpire (DuplicateTuple tuple);
  bool m_linkTupleTimerFirstTime;
  void LinkTupleTimerExpire (LinkTuple tuple);
  void Nb2hopTupleTimerExpire (TwoHopNeighborTuple tuple);
  void MprSelTupleTimerExpire (MprSelectorTuple tuple);
  void TopologyTupleTimerExpire (TopologyTuple tuple);
  void IfaceAssocTupleTimerExpire (IfaceAssocTuple tuple);

  /// A list of pending messages which are buffered awaiting for being sent.
  std::vector<Packet> m_queuedMessages;
  SingleEvent m_queuedMessagesTimer; // timer for throttling outgoing messages

  void ForwardDefault (OlsrMessageHeader olsrMessage,
                       Packet messagePayload,
                       DuplicateTuple *duplicated,
                       const Ipv4Address &localIface,
                       const Ipv4Address &senderAddress);
  void QueueMessage (Packet message, Time delay);
  void SendQueuedMessages ();
  void SendHello ();
  void SendTc ();
  void SendMid ();

  void NeighborLoss (const LinkTuple &tuple);
  void AddDuplicateTuple (const DuplicateTuple &tuple);
  void RemoveDuplicateTuple (const DuplicateTuple &tuple);
  LinkTuple & AddLinkTuple (const LinkTuple &tuple, uint8_t willingness);
  void RemoveLinkTuple (const LinkTuple &tuple);
  void LinkTupleUpdated (const LinkTuple &tuple);
  void AddNeighborTuple (const NeighborTuple &tuple);
  void RemoveNeighborTuple (const NeighborTuple &tuple);
  void AddTwoHopNeighborTuple (const TwoHopNeighborTuple &tuple);
  void RemoveTwoHopNeighborTuple (const TwoHopNeighborTuple &tuple);
  void AddMprSelectorTuple (const MprSelectorTuple  &tuple);
  void RemoveMprSelectorTuple (const MprSelectorTuple &tuple);
  void AddTopologyTuple (const TopologyTuple &tuple);
  void RemoveTopologyTuple (const TopologyTuple &tuple);
  void AddIfaceAssocTuple (const IfaceAssocTuple &tuple);
  void RemoveIfaceAssocTuple (const IfaceAssocTuple &tuple);

  void ProcessHello (const OlsrMessageHeader &msg,
                     const OlsrHelloMessageHeader &hello,
                     const Ipv4Address &receiverIface,
                     const Ipv4Address &senderIface);
  void ProcessTc (const OlsrMessageHeader &msg,
                  const OlsrTcMessageHeader &tc,
                  const Ipv4Address &senderIface);
  void ProcessMid (const OlsrMessageHeader &msg,
                   const OlsrMidMessageHeader &mid,
                   const Ipv4Address &senderIface);

  void LinkSensing (const OlsrMessageHeader &msg,
                    const OlsrHelloMessageHeader &hello,
                    const Ipv4Address &receiverIface,
                    const Ipv4Address &sender_iface);
  void PopulateNeighborSet (const OlsrMessageHeader &msg,
                            const OlsrHelloMessageHeader &hello);
  void PopulateTwoHopNeighborSet (const OlsrMessageHeader &msg,
                                  const OlsrHelloMessageHeader &hello);
  void PopulateMprSelectorSet (const OlsrMessageHeader &msg,
                               const OlsrHelloMessageHeader &hello);

  int Degree (NeighborTuple const &tuple);

  Ipv4Address m_mainAddress;
  Ptr<Socket> m_receiveSocket; // UDP socket for receving OSLR packets
  Ptr<Socket> m_sendSocket; // UDP socket for sending OSLR packets
};

} // namespace ns3

#endif
