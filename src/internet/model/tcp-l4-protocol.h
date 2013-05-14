/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include "ns3/ipv6-address.h"
#include "ns3/ptr.h"
#include "ns3/object-factory.h"
#include "ip-l4-protocol.h"
#include "ns3/net-device.h"

namespace ns3 {

class Node;
class Socket;
class TcpHeader;
class Ipv4EndPointDemux;
class Ipv6EndPointDemux;
class Ipv4Interface;
class TcpSocketBase;
class Ipv4EndPoint;
class Ipv6EndPoint;

/**
 * \ingroup tcp
 * \brief A layer between the sockets interface and IP
 * 
 * This class allocates "endpoint" objects (ns3::Ipv4EndPoint) for TCP,
 * and SHOULD checksum packets its receives from the socket layer going down
 * the stack , but currently checksumming is disabled.  It also receives 
 * packets from IP, and forwards them up to the endpoints.
*/

class TcpL4Protocol : public IpL4Protocol {
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
  Ipv6EndPoint *Allocate6 (void);
  Ipv6EndPoint *Allocate6 (Ipv6Address address);
  Ipv6EndPoint *Allocate6 (uint16_t port);
  Ipv6EndPoint *Allocate6 (Ipv6Address address, uint16_t port);
  Ipv6EndPoint *Allocate6 (Ipv6Address localAddress, uint16_t localPort,
                           Ipv6Address peerAddress, uint16_t peerPort);

  void DeAllocate (Ipv4EndPoint *endPoint);
  void DeAllocate (Ipv6EndPoint *endPoint);

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
  void Send (Ptr<Packet> packet,
             Ipv6Address saddr, Ipv6Address daddr, 
             uint16_t sport, uint16_t dport, Ptr<NetDevice> oif = 0);
  /**
   * \brief Receive a packet up the protocol stack
   * \param p The Packet to dump the contents into
   * \param header IPv4 Header information
   * \param incomingInterface The Ipv4Interface it was received on
   */
  virtual enum IpL4Protocol::RxStatus Receive (Ptr<Packet> p,
                                                 Ipv4Header const &header,
                                                 Ptr<Ipv4Interface> incomingInterface);
  virtual enum IpL4Protocol::RxStatus Receive (Ptr<Packet> p,
                                                 Ipv6Header const &header,
                                                 Ptr<Ipv6Interface> interface);

  /**
   * \brief Receive an ICMP packet
   * \param icmpSource The IP address of the source of the packet.
   * \param icmpTtl The time to live from the IP header
   * \param icmpType The type of the message from the ICMP header
   * \param icmpCode The message code from the ICMP header
   * \param icmpInfo 32-bit integer carrying informational value of varying semantics.
   * \param payloadSource The IP source address from the IP header of the packet
   * \param payloadDestination The IP destination address from the IP header of the packet
   * \param payload Payload of the ICMP packet
   */
  virtual void ReceiveIcmp (Ipv4Address icmpSource, uint8_t icmpTtl,
                            uint8_t icmpType, uint8_t icmpCode, uint32_t icmpInfo,
                            Ipv4Address payloadSource,Ipv4Address payloadDestination,
                            const uint8_t payload[8]);
  virtual void ReceiveIcmp (Ipv6Address icmpSource, uint8_t icmpTtl,
                            uint8_t icmpType, uint8_t icmpCode, uint32_t icmpInfo,
                            Ipv6Address payloadSource,Ipv6Address payloadDestination,
                            const uint8_t payload[8]);

  // From IpL4Protocol
  virtual void SetDownTarget (IpL4Protocol::DownTargetCallback cb);
  virtual void SetDownTarget6 (IpL4Protocol::DownTargetCallback6 cb);
  // From IpL4Protocol
  virtual IpL4Protocol::DownTargetCallback GetDownTarget (void) const;
  virtual IpL4Protocol::DownTargetCallback6 GetDownTarget6 (void) const;

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
  Ipv6EndPointDemux *m_endPoints6;
  TypeId m_rttTypeId;
  TypeId m_socketTypeId;
private:
  friend class TcpSocketBase;
  void SendPacket (Ptr<Packet>, const TcpHeader &,
                   Ipv4Address, Ipv4Address, Ptr<NetDevice> oif = 0);
  void SendPacket (Ptr<Packet>, const TcpHeader &,
                   Ipv6Address, Ipv6Address, Ptr<NetDevice> oif = 0);
  TcpL4Protocol (const TcpL4Protocol &o);
  TcpL4Protocol &operator = (const TcpL4Protocol &o);

  std::vector<Ptr<TcpSocketBase> > m_sockets;
  IpL4Protocol::DownTargetCallback m_downTarget;
  IpL4Protocol::DownTargetCallback6 m_downTarget6;
};

} // namespace ns3

#endif /* TCP_L4_PROTOCOL_H */
