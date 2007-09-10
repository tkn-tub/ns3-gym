/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
 * All rights reserved.
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

  void SetRxCallback (Callback<void,const Packet &, Ipv4Address, uint16_t> callback);
  void SetDestroyCallback (Callback<void> callback);

  void ForwardUp (const Packet &p, Ipv4Address saddr, uint16_t sport);

private:
  Ipv4Address m_localAddr;
  uint16_t m_localPort;
  Ipv4Address m_peerAddr;
  uint16_t m_peerPort;
  Callback<void,const Packet &, Ipv4Address, uint16_t> m_rxCallback;
  Callback<void> m_destroyCallback;
};

}; // namespace ns3


#endif /* IPV4_END_POINT_H */
