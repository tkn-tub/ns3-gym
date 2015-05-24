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
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  static const uint8_t PROT_NUMBER; //!< protocol number (0x6)

  TcpL4Protocol ();
  virtual ~TcpL4Protocol ();

  /**
   * Set node associated with this stack
   * \param node the node
   */
  void SetNode (Ptr<Node> node);

  virtual int GetProtocolNumber (void) const;

  /**
   * \brief Create a TCP socket
   * \return A smart Socket pointer to a TcpSocket allocated by this instance
   * of the TCP protocol
   */
  Ptr<Socket> CreateSocket (void);
  /**
   * \brief Create a TCP socket
   * \return A smart Socket pointer to a TcpSocket allocated by this instance
   * of the TCP protocol
   *
   * \warning using a socketTypeId other than TCP is a bad idea.
   *
   * \param socketTypeId the socket TypeId
   */
  Ptr<Socket> CreateSocket (TypeId socketTypeId);

  /**
   * \brief Allocate an IPv4 Endpoint
   * \return the Endpoint
   */
  Ipv4EndPoint *Allocate (void);
  /**
   * \brief Allocate an IPv4 Endpoint
   * \param address address to use
   * \return the Endpoint
   */
  Ipv4EndPoint *Allocate (Ipv4Address address);
  /**
   * \brief Allocate an IPv4 Endpoint
   * \param port port to use
   * \return the Endpoint
   */
  Ipv4EndPoint *Allocate (uint16_t port);
  /**
   * \brief Allocate an IPv4 Endpoint
   * \param address address to use
   * \param port port to use
   * \return the Endpoint
   */
  Ipv4EndPoint *Allocate (Ipv4Address address, uint16_t port);
  /**
   * \brief Allocate an IPv4 Endpoint
   * \param localAddress local address to use
   * \param localPort local port to use
   * \param peerAddress remote address to use
   * \param peerPort remote port to use
   * \return the Endpoint
   */
  Ipv4EndPoint *Allocate (Ipv4Address localAddress, uint16_t localPort,
                          Ipv4Address peerAddress, uint16_t peerPort);
  /**
   * \brief Allocate an IPv6 Endpoint
   * \return the Endpoint
   */
  Ipv6EndPoint *Allocate6 (void);
  /**
   * \brief Allocate an IPv6 Endpoint
   * \param address address to use
   * \return the Endpoint
   */
  Ipv6EndPoint *Allocate6 (Ipv6Address address);
  /**
   * \brief Allocate an IPv6 Endpoint
   * \param port port to use
   * \return the Endpoint
   */
  Ipv6EndPoint *Allocate6 (uint16_t port);
  /**
   * \brief Allocate an IPv6 Endpoint
   * \param address address to use
   * \param port port to use
   * \return the Endpoint
   */
  Ipv6EndPoint *Allocate6 (Ipv6Address address, uint16_t port);
  /**
   * \brief Allocate an IPv6 Endpoint
   * \param localAddress local address to use
   * \param localPort local port to use
   * \param peerAddress remote address to use
   * \param peerPort remote port to use
   * \return the Endpoint
   */
  Ipv6EndPoint *Allocate6 (Ipv6Address localAddress, uint16_t localPort,
                           Ipv6Address peerAddress, uint16_t peerPort);

  /**
   * \brief Remove an IPv4 Endpoint.
   * \param endPoint the end point to remove
   */
  void DeAllocate (Ipv4EndPoint *endPoint);
  /**
   * \brief Remove an IPv6 Endpoint.
   * \param endPoint the end point to remove
   */
  void DeAllocate (Ipv6EndPoint *endPoint);

  /**
   * \brief Send a packet via TCP (IPv4)
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
   * \brief Send a packet via TCP (IPv6)
   * \param packet The packet to send
   * \param saddr The source Ipv4Address
   * \param daddr The destination Ipv4Address
   * \param sport The source port number
   * \param dport The destination port number
   * \param oif The output interface bound. Defaults to null (unspecified).
   */
  void Send (Ptr<Packet> packet,
             Ipv6Address saddr, Ipv6Address daddr, 
             uint16_t sport, uint16_t dport, Ptr<NetDevice> oif = 0);


  virtual enum IpL4Protocol::RxStatus Receive (Ptr<Packet> p,
                                               Ipv4Header const &header,
                                               Ptr<Ipv4Interface> incomingInterface);
  virtual enum IpL4Protocol::RxStatus Receive (Ptr<Packet> p,
                                               Ipv6Header const &header,
                                               Ptr<Ipv6Interface> incomingInterface);

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
  Ptr<Node> m_node; //!< the node this stack is associated with
  Ipv4EndPointDemux *m_endPoints; //!< A list of IPv4 end points.
  Ipv6EndPointDemux *m_endPoints6; //!< A list of IPv6 end points.
  TypeId m_rttTypeId; //!< The RTT Estimator TypeId
  TypeId m_socketTypeId; //!< The socket TypeId
private:
  friend class TcpSocketBase;

  /**
   * \brief Send a packet via TCP (IPv4)
   * \param packet The packet to send
   * \param outgoing The TCP header
   * \param saddr The source Ipv4Address
   * \param daddr The destination Ipv4Address
   * \param oif The output interface bound. Defaults to null (unspecified).
   */
  void SendPacket (Ptr<Packet> packet, const TcpHeader &outgoing,
                   Ipv4Address saddr, Ipv4Address daddr, Ptr<NetDevice>  oif = 0);

  /**
   * \brief Send a packet via TCP (IPv4)
   * \param packet The packet to send
   * \param outgoing The TCP header
   * \param saddr The source Ipv4Address
   * \param daddr The destination Ipv4Address
   * \param oif The output interface bound. Defaults to null (unspecified).
   */
  void SendPacket (Ptr<Packet> packet, const TcpHeader &outgoing,
                   Ipv6Address saddr, Ipv6Address daddr, Ptr<NetDevice> oif = 0);

  /**
   * \brief Copy constructor
   *
   * Defined and not implemented to avoid misuse
   */
  TcpL4Protocol (const TcpL4Protocol &);
  /**
   * \brief Copy constructor
   *
   * Defined and not implemented to avoid misuse
   * \returns
   */
  TcpL4Protocol &operator = (const TcpL4Protocol &);

  std::vector<Ptr<TcpSocketBase> > m_sockets;      //!< list of sockets
  IpL4Protocol::DownTargetCallback m_downTarget;   //!< Callback to send packets over IPv4
  IpL4Protocol::DownTargetCallback6 m_downTarget6; //!< Callback to send packets over IPv6
};

} // namespace ns3

#endif /* TCP_L4_PROTOCOL_H */
