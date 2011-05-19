/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Georgia Tech Research Corporation
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
 * Author: Raj Bhattacharjea <raj.b@gatech.edu>
 */

#ifndef TCP_L4_PROTOCOL_H
#define TCP_L4_PROTOCOL_H

#include <stdint.h>

#include "ns3/packet.h"
#include "ns3/ipv4-address.h"
#include "ns3/ptr.h"
#include "ns3/object-factory.h"
#include "ipv4-l4-protocol.h"
#include "ns3/net-device.h"

namespace ns3 {

class Node;
class Socket;
class TcpHeader;
class Ipv4EndPointDemux;
class Ipv4Interface;
class TcpSocketBase;
class Ipv4EndPoint;

/**
 * \ingroup tcp
 * \brief A layer between the sockets interface and IP
 * 
 * This class allocates "endpoint" objects (ns3::Ipv4EndPoint) for TCP,
 * and SHOULD checksum packets its receives from the socket layer going down
 * the stack , but currently checksumming is disabled.  It also receives 
 * packets from IP, and forwards them up to the endpoints.
*/

class TcpL4Protocol : public Ipv4L4Protocol {
public:
  static TypeId GetTypeId (void);
  static const uint8_t PROT_NUMBER;
  /**
   * \brief Constructor
   */
  TcpL4Protocol ();
  virtual ~TcpL4Protocol ();

  void SetNode (Ptr<Node> node);

  virtual int GetProtocolNumber (void) const;

  /**
   * \return A smart Socket pointer to a TcpSocket allocated by this instance
   * of the TCP protocol
   */
  Ptr<Socket> CreateSocket (void);
  Ptr<Socket> CreateSocket (TypeId socketTypeId);

  Ipv4EndPoint *Allocate (void);
  Ipv4EndPoint *Allocate (Ipv4Address address);
  Ipv4EndPoint *Allocate (uint16_t port);
  Ipv4EndPoint *Allocate (Ipv4Address address, uint16_t port);
  Ipv4EndPoint *Allocate (Ipv4Address localAddress, uint16_t localPort,
                          Ipv4Address peerAddress, uint16_t peerPort);

  void DeAllocate (Ipv4EndPoint *endPoint);

  /**
   * \brief Send a packet via TCP
   * \param packet The packet to send
   * \param saddr The source Ipv4Address
   * \param daddr The destination Ipv4Address
   * \param sport The source port number
   * \param dport The destination port number
   * \param oif The output interface bound. Defaults to null (unspecified).
   */
  void Send (Ptr<Packet> packet,
             Ipv4Address saddr, Ipv4Address daddr, 
             uint16_t sport, uint16_t dport, Ptr<NetDevice> oif = 0);
  /**
   * \brief Receive a packet up the protocol stack
   * \param p The Packet to dump the contents into
   * \param header IPv4 Header information
   * \param incomingInterface The Ipv4Interface it was received on
   */
  virtual enum Ipv4L4Protocol::RxStatus Receive (Ptr<Packet> p,
                                                 Ipv4Header const &header,
                                                 Ptr<Ipv4Interface> incomingInterface);

  // From Ipv4L4Protocol
  virtual void SetDownTarget (Ipv4L4Protocol::DownTargetCallback cb);
  // From Ipv4L4Protocol
  virtual Ipv4L4Protocol::DownTargetCallback GetDownTarget (void) const;

protected:
  virtual void DoDispose (void);
  /* 
   * This function will notify other components connected to the node that a new stack member is now connected
   * This will be used to notify Layer 3 protocol of layer 4 protocol stack to connect them together.
   */
  virtual void NotifyNewAggregate ();
private:
  Ptr<Node> m_node;
  Ipv4EndPointDemux *m_endPoints;
  TypeId m_rttTypeId;
  TypeId m_socketTypeId;
private:
  friend class TcpSocketBase;
  void SendPacket (Ptr<Packet>, const TcpHeader &,
                   Ipv4Address, Ipv4Address, Ptr<NetDevice> oif = 0);
  TcpL4Protocol (const TcpL4Protocol &o);
  TcpL4Protocol &operator = (const TcpL4Protocol &o);

  std::vector<Ptr<TcpSocketBase> > m_sockets;
  Ipv4L4Protocol::DownTargetCallback m_downTarget;
};

}; // namespace ns3

#endif /* TCP_L4_PROTOCOL_H */
