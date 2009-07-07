/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 1997, 1998 Carnegie Mellon University.
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
 * Authors: The AODV code developed by the CMU/MONARCH group was optimized and
 * tuned by Samir Das and Mahesh Marina, University of Cincinnati.
 * The work was partially done in Sun Microsystems.
 *
 * Ported to ns-3 by Elena Borovkova <borovkovaes@iitp.ru>
 */
#ifndef AODVROUTINGPROTOCOL_H_
#define AODVROUTINGPROTOCOL_H_

#include "aodv-rtable.h"
#include "aodv-rqueue.h"
#include "aodv-packet.h"

#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/socket.h"
#include "ns3/timer.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4-routing-protocol.h"
#include <map>

namespace ns3
{
namespace aodv
{
/**
 * \ingroup aodv
 * \brief AODV Neighbor Cache Entry
 * TODO move inside RoutingProtocol
 */
class AODV_Neighbor
{
  friend class AODV;
  friend class aodv_rt_entry;

public:
  AODV_Neighbor(Ipv4Address const & a, Time exp = Seconds(0)) : nb_addr(a), nb_expire(exp) {}
  bool operator==(AODV_Neighbor const & n) const
  {
    return (nb_addr == n.nb_addr)&&(nb_expire == n.nb_expire);
  }

protected:
  Ipv4Address nb_addr;
  Time nb_expire;
};

/**
 * \ingroup aodv
 * 
 * Broadcast ID Cache entry
 * TODO move inside RoutingProtocol
 */
struct BroadcastID 
{
  Ipv4Address src;
  uint32_t    id;
  /// now + BCAST_ID_SAVE s
  Time        expire;         

  BroadcastID (Ipv4Address i, uint32_t b, Time t) : src(i), id(b), expire(t) {}
};

/**
 * \ingroup aodv
 * \brief AODV routing protocol
 */
class RoutingProtocol : public Ipv4RoutingProtocol
{
public:
  static TypeId GetTypeId (void);

  RoutingProtocol();
  virtual ~RoutingProtocol();
  virtual void DoDispose ();
  
  ///\name From Ipv4RoutingProtocol
  //\{
  Ptr<Ipv4Route> RouteOutput (Ptr<Packet> p, const Ipv4Header &header, uint32_t oif, Socket::SocketErrno &sockerr);
  bool RouteInput (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev,
                           UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                           LocalDeliverCallback lcb, ErrorCallback ecb);  
  virtual void NotifyInterfaceUp (uint32_t interface);
  virtual void NotifyInterfaceDown (uint32_t interface);
  virtual void NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address);
  virtual void NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address);
  virtual void SetIpv4 (Ptr<Ipv4> ipv4);
  //\}
  
private:
  ///\name Protocol parameters. TODO document
  //\{
  Time BCAST_ID_SAVE;
  Time HELLO_INTERVAL;
  uint32_t ALLOWED_HELLO_LOSS;
  Time BAD_LINK_LIFETIME;
  Time MaxHelloInterval; //        (1.25 * HELLO_INTERVAL)
  Time MinHelloInterval; //        (0.75 * HELLO_INTERVAL)
  Time FREQUENCY;
  //\}
  
  /// IP protocol
  Ptr<Ipv4> m_ipv4;
  /// UDP socket per each IP interface, map socket -> iface
  std::map< Ptr<Socket>, Ipv4Address > m_socketAddresses;
  /// Routing table
  aodv_rtable rtable;
  /// A "drop-front" queue used by the routing layer to buffer packets to which it does not have a route.
  aodv_rqueue rqueue;
  /// List of neighbors (aka neighbors cache). TODO: separate list for each interface??? 
  std::vector<AODV_Neighbor> nbhead;
  /// Broadcast sequence numbers cache
  std::vector<BroadcastID> bihead;
  /// Preq broadcast ID
  uint32_t bid;
  /// Sequence Number (???)
  uint32_t seqno;


private:
  /// Start protocol operation
  void Start ();
  
  /// Receive and process control packet
  void RecvAodv (Ptr<Socket> s);
  /// Receive RREQ
  void RecvRequest (Ptr<Packet> p);
  /// Receive RREP
  void RecvReply (Ptr<Packet> p);
  /// Receive RERR
  void RecvError (Ptr<Packet> p);
  
  ///\name Timers. TODO comment each one
  //\{
  Timer  btimer;
  void BroadcastTimerExpire ();
  Timer htimer;
  void HelloTimerExpire ();
  Timer ntimer;
  void NeighborTimerExpire ();
  Timer rtimer;
  void RouteCacheTimerExpire ();
  Timer lrtimer;
  void LocalRepairTimerExpire ();
  //\}
};

}
}
#endif /* AODVROUTINGPROTOCOL_H_ */
