/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2004 Francisco J. Ros
 * Copyright (c) 2007 INESC Porto
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
 * Authors: Francisco J. Ros  <fjrm@dif.um.es>
 *          Gustavo J. A. M. Carneiro <gjc@inescporto.pt>
 */

#ifndef OLSR_AGENT_IMPL_H
#define OLSR_AGENT_IMPL_H

#include "olsr-header.h"
#include "ns3/test.h"
#include "olsr-state.h"
#include "olsr-repositories.h"

#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/socket.h"
#include "ns3/event-garbage-collector.h"
#include "ns3/timer.h"
#include "ns3/traced-callback.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/ipv4-static-routing.h"

#include <vector>
#include <map>


namespace ns3 {
namespace olsr {

///
/// \defgroup olsr OLSR Routing
/// This section documents the API of the ns-3 OLSR module. For a generic 
/// functional description, please refer to the ns-3 manual.

/// An %OLSR's routing table entry.
struct RoutingTableEntry
{
  Ipv4Address destAddr; ///< Address of the destination node.
  Ipv4Address nextAddr; ///< Address of the next hop.
  uint32_t interface; ///< Interface index
  uint32_t distance; ///< Distance in hops to the destination.

  RoutingTableEntry () : // default values
                         destAddr (), nextAddr (),
                         interface (0), distance (0) {};
};

class RoutingProtocol;

/// Testcase for MPR computation mechanism
class OlsrMprTestCase;

///
/// \ingroup olsr
///
/// \brief OLSR routing protocol for IPv4
///
class RoutingProtocol : public Ipv4RoutingProtocol
{
public:
  friend class OlsrMprTestCase;
  static TypeId GetTypeId (void);

  RoutingProtocol ();
  virtual ~RoutingProtocol ();

  ///
  /// \brief Set the OLSR main address to the first address on the indicated
  ///        interface
  /// \param interface IPv4 interface index
  ///
  void SetMainInterface (uint32_t interface);

  ///
  /// Dump the neighbor table, two-hop neighbor table, and routing table
  /// to logging output (NS_LOG_DEBUG log level).  If logging is disabled,
  /// this function does nothing.
  ///
  void Dump (void);

  /**
   * Return the list of routing table entries discovered by OLSR
   **/
  std::vector<RoutingTableEntry> GetRoutingTableEntries () const;

private:
  std::set<uint32_t> m_interfaceExclusions;
  Ptr<Ipv4StaticRouting> m_routingTableAssociation;

public:
  std::set<uint32_t> GetInterfaceExclusions () const
  {
    return m_interfaceExclusions;
  }
  void SetInterfaceExclusions (std::set<uint32_t> exceptions);

  /// Inject Association to be sent in HNA message
  void AddHostNetworkAssociation (Ipv4Address networkAddr, Ipv4Mask netmask);
  /// Removes Association sent in HNA message
  void RemoveHostNetworkAssociation (Ipv4Address networkAddr, Ipv4Mask netmask);

  /// Inject Associations from an Ipv4StaticRouting instance
  void SetRoutingTableAssociation (Ptr<Ipv4StaticRouting> routingTable);

protected:
  virtual void DoStart (void);
private:
  std::map<Ipv4Address, RoutingTableEntry> m_table; ///< Data structure for the routing table.

  Ptr<Ipv4StaticRouting> m_hnaRoutingTable;

  EventGarbageCollector m_events;
	
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
  /// HNA messages' emission interval.
  Time m_hnaInterval;
  /// Willingness for forwarding packets on behalf of other nodes.
  uint8_t m_willingness;

  /// Internal state with all needed data structs.
  OlsrState m_state;

  Ptr<Ipv4> m_ipv4;

  void Clear ();
  uint32_t GetSize () const { return m_table.size (); }
  void RemoveEntry (const Ipv4Address &dest);
  void AddEntry (const Ipv4Address &dest,
                 const Ipv4Address &next,
                 uint32_t interface,
                 uint32_t distance);
  void AddEntry (const Ipv4Address &dest,
                 const Ipv4Address &next,
                 const Ipv4Address &interfaceAddress,
                 uint32_t distance);
  bool Lookup (const Ipv4Address &dest,
               RoutingTableEntry &outEntry) const;
  bool FindSendEntry (const RoutingTableEntry &entry,
                      RoutingTableEntry &outEntry) const;

  // From Ipv4RoutingProtocol
  virtual Ptr<Ipv4Route> RouteOutput (Ptr<Packet> p,
                                      const Ipv4Header &header,
                                      Ptr<NetDevice> oif,
                                      Socket::SocketErrno &sockerr);
  virtual bool RouteInput (Ptr<const Packet> p,
                           const Ipv4Header &header,
                           Ptr<const NetDevice> idev,
                           UnicastForwardCallback ucb,
                           MulticastForwardCallback mcb,
                           LocalDeliverCallback lcb,
                           ErrorCallback ecb);
  virtual void NotifyInterfaceUp (uint32_t interface);
  virtual void NotifyInterfaceDown (uint32_t interface);
  virtual void NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address);
  virtual void NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address);
  virtual void SetIpv4 (Ptr<Ipv4> ipv4);
  virtual void PrintRoutingTable (Ptr<OutputStreamWrapper> stream) const;

  void DoDispose ();

  void SendPacket (Ptr<Packet> packet, const MessageList &containedMessages);

  /// Increments packet sequence number and returns the new value.
  inline uint16_t GetPacketSequenceNumber ();
  /// Increments message sequence number and returns the new value.
  inline uint16_t GetMessageSequenceNumber ();

  void RecvOlsr (Ptr<Socket> socket);

  void MprComputation ();
  void RoutingTableComputation ();
  Ipv4Address GetMainAddress (Ipv4Address iface_addr) const;
  bool UsesNonOlsrOutgoingInterface (const Ipv4RoutingTableEntry &route);

  // Timer handlers
  Timer m_helloTimer;
  void HelloTimerExpire ();

  Timer m_tcTimer;
  void TcTimerExpire ();

  Timer m_midTimer;
  void MidTimerExpire ();

  Timer m_hnaTimer;
  void HnaTimerExpire ();

  void DupTupleTimerExpire (Ipv4Address address, uint16_t sequenceNumber);
  bool m_linkTupleTimerFirstTime;
  void LinkTupleTimerExpire (Ipv4Address neighborIfaceAddr);
  void Nb2hopTupleTimerExpire (Ipv4Address neighborMainAddr, Ipv4Address twoHopNeighborAddr);
  void MprSelTupleTimerExpire (Ipv4Address mainAddr);
  void TopologyTupleTimerExpire (Ipv4Address destAddr, Ipv4Address lastAddr);
  void IfaceAssocTupleTimerExpire (Ipv4Address ifaceAddr);
  void AssociationTupleTimerExpire (Ipv4Address gatewayAddr, Ipv4Address networkAddr, Ipv4Mask netmask);

  void IncrementAnsn ();

  /// A list of pending messages which are buffered awaiting for being sent.
  olsr::MessageList m_queuedMessages;
  Timer m_queuedMessagesTimer; // timer for throttling outgoing messages

  void ForwardDefault (olsr::MessageHeader olsrMessage,
                       DuplicateTuple *duplicated,
                       const Ipv4Address &localIface,
                       const Ipv4Address &senderAddress);
  void QueueMessage (const olsr::MessageHeader &message, Time delay);
  void SendQueuedMessages ();
  void SendHello ();
  void SendTc ();
  void SendMid ();
  void SendHna ();

  void NeighborLoss (const LinkTuple &tuple);
  void AddDuplicateTuple (const DuplicateTuple &tuple);
  void RemoveDuplicateTuple (const DuplicateTuple &tuple);
  void LinkTupleAdded (const LinkTuple &tuple, uint8_t willingness);
  void RemoveLinkTuple (const LinkTuple &tuple);
  void LinkTupleUpdated (const LinkTuple &tuple, uint8_t willingness);
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
  void AddAssociationTuple (const AssociationTuple &tuple);
  void RemoveAssociationTuple (const AssociationTuple &tuple);

  void ProcessHello (const olsr::MessageHeader &msg,
                     const Ipv4Address &receiverIface,
                     const Ipv4Address &senderIface);
  void ProcessTc (const olsr::MessageHeader &msg,
                  const Ipv4Address &senderIface);
  void ProcessMid (const olsr::MessageHeader &msg,
                   const Ipv4Address &senderIface);
  void ProcessHna (const olsr::MessageHeader &msg,
                   const Ipv4Address &senderIface);

  void LinkSensing (const olsr::MessageHeader &msg,
                    const olsr::MessageHeader::Hello &hello,
                    const Ipv4Address &receiverIface,
                    const Ipv4Address &sender_iface);
  void PopulateNeighborSet (const olsr::MessageHeader &msg,
                            const olsr::MessageHeader::Hello &hello);
  void PopulateTwoHopNeighborSet (const olsr::MessageHeader &msg,
                                  const olsr::MessageHeader::Hello &hello);
  void PopulateMprSelectorSet (const olsr::MessageHeader &msg,
                               const olsr::MessageHeader::Hello &hello);

  int Degree (NeighborTuple const &tuple);
  /// Check that address is one of my interfaces
  bool IsMyOwnAddress (const Ipv4Address & a) const;

  Ipv4Address m_mainAddress;

  // One socket per interface, each bound to that interface's address
  // (reason: for OLSR Link Sensing we need to know on which interface
  // HELLO messages arrive)
  std::map< Ptr<Socket>, Ipv4InterfaceAddress > m_socketAddresses;

  TracedCallback <const PacketHeader &,
                  const MessageList &> m_rxPacketTrace;
  TracedCallback <const PacketHeader &,
                  const MessageList &> m_txPacketTrace;
  TracedCallback <uint32_t> m_routingTableChanged;

};

}
}  // namespace ns3

#endif /* OLSR_AGENT_IMPL_H */
