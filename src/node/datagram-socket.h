/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H

#include <stdint.h>
#include "ns3/callback.h"
#include "ipv4-address.h"

namespace ns3 {

class UdpEndPoint;
class Node;
class Packet;
class Udp;

/**
 * Before any data is sent, the socket must be bound with
 * one of the Bind methods.
 * If none of these Bind methods are called prior to
 * one of the ::Send methods, the socket is implicitely
 * bound to a random port and to all interfaces.
 */
class DatagramSocket 
{
public:
  /**
   * Create an unbound udp socket.
   */
  DatagramSocket (Node *node);
  ~DatagramSocket ();

  /** 
   * Allocate a free port number and
   * bind this socket to this port number on all
   * interfaces of this system.
   *
   * return 0 on success, -1 on failure.
   */
  int Bind (void);
  /** 
   * Allocate a free port number and
   * bind this socket to this port number on the
   * specified interface.
   *
   * return 0 on success, -1 on failure.
   */
  int Bind (Ipv4Address address);

  /**
   * Bind this socket to this port number
   * on all interfaces of this system.
   *
   * return 0 on success, -1 on failure.
   */
  int Bind (uint16_t port); 

  /**
   * Bind this socket to this port number
   * on the interface specified by address.
   *
   * return 0 on success, -1 on failure.
   */
  int Bind (Ipv4Address address, uint16_t port);


  /**
   * Set the default destination address and port
   * number for all packets outgoing from this socket.
   */
  void SetDefaultDestination (Ipv4Address daddr, uint16_t dport);

  /**
   * Send dummy data to default destination
   */
  void SendDummy (uint32_t size);
  /**
   * Send dummy data to specified destination
   */
  void SendDummyTo (uint32_t size, Ipv4Address daddr, uint16_t dport);

  void Send (uint8_t const*buffer, uint32_t size);
  void SendTo (uint8_t const*buffer, uint32_t size,
               Ipv4Address daddr, uint16_t dport);

  /**
   * When a packet is received by this socket, it invokes the "dummy callback" which
   * forwards to the application the number of bytes received and from who they
   * were received.
   */
  void SetDummyRxCallback (Callback<void,DatagramSocket*,uint32_t,Ipv4Address,uint16_t> cb);
  /**
   * When a packet is received by this socket, it invokes the "normal callback" which
   * forwards to the application the buffer of bytes received and from who they
   * were received. The application is responsible for copying that buffer if it wants 
   * to keep track of it.
   */
  void SetRxCallback (Callback<void,DatagramSocket*,uint8_t const*,uint32_t,Ipv4Address,uint16_t> cb);
  /** 
   * Return pointer to node
   */ 
   Node* GetNode(void) const;

private:
  friend class Udp;
  // invoked by Udp class
  void ForwardUp (Packet &p, Ipv4Address saddr, uint16_t sport);
  Udp *GetUdp (void) const;

  UdpEndPoint *m_endPoint;
  Node *m_node;
  Ipv4Address m_defaultAddress;
  uint16_t m_defaultPort;
  Callback<void,DatagramSocket*,uint32_t,Ipv4Address,uint16_t> m_dummyRxCallback;
  Callback<void,DatagramSocket*,uint8_t const*,uint32_t,Ipv4Address,uint16_t> m_rxCallback;
};

}//namespace ns3

#endif /* UDP_SOCKET_H */
