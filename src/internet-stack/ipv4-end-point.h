/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#ifndef IPV4_END_POINT_H
#define IPV4_END_POINT_H

#include <stdint.h>
#include "ns3/ipv4-address.h"
#include "ns3/callback.h"

namespace ns3 {

class Header;
class Packet;

/**
 * \brief A representation of an internet endpoint/connection
 *
 * This class provides an internet four-tuple (source and destination ports 
 * and addresses).  These are used in the ns3::Ipv4EndPointDemux as targets
 * of lookups.  The class also has a callback for notification to higher
 * layers that a packet from a lower layer was received.  In the ns3
 * internet-stack, these notifications are automatically registered to be
 * received by the corresponding socket.
 */

class Ipv4EndPoint {
public:
  Ipv4EndPoint (Ipv4Address address, uint16_t port);
  ~Ipv4EndPoint ();

  Ipv4Address GetLocalAddress (void);
  void SetLocalAddress (Ipv4Address address);
  uint16_t GetLocalPort (void);
  Ipv4Address GetPeerAddress (void);
  uint16_t GetPeerPort (void);

  void SetPeer (Ipv4Address address, uint16_t port);

  // Called from socket implementations to get notified about important events.
  void SetRxCallback (Callback<void,Ptr<Packet>, Ipv4Address, uint16_t> callback);
  void SetIcmpCallback (Callback<void,Ipv4Address,uint8_t,uint8_t,uint8_t,uint32_t> callback);
  void SetDestroyCallback (Callback<void> callback);

  // Called from an L4Protocol implementation to notify an endpoint of a
  // packet reception.
  void ForwardUp (Ptr<Packet> p, Ipv4Address saddr, uint16_t sport);
  // Called from an L4Protocol implementation to notify an endpoint of
  // an icmp message reception.
  void ForwardIcmp (Ipv4Address icmpSource, uint8_t icmpTtl, 
                    uint8_t icmpType, uint8_t icmpCode,
                    uint32_t icmpInfo);

private:
  void DoForwardUp (Ptr<Packet> p, Ipv4Address saddr, uint16_t sport);
  void DoForwardIcmp (Ipv4Address icmpSource, uint8_t icmpTtl, 
                      uint8_t icmpType, uint8_t icmpCode,
                      uint32_t icmpInfo);
  Ipv4Address m_localAddr;
  uint16_t m_localPort;
  Ipv4Address m_peerAddr;
  uint16_t m_peerPort;
  Callback<void,Ptr<Packet>, Ipv4Address, uint16_t> m_rxCallback;
  Callback<void,Ipv4Address,uint8_t,uint8_t,uint8_t,uint32_t> m_icmpCallback;
  Callback<void> m_destroyCallback;
};

}; // namespace ns3


#endif /* IPV4_END_POINT_H */
