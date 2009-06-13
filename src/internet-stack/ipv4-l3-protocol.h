// -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*-
//
// Copyright (c) 2006 Georgia Tech Research Corporation
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: George F. Riley<riley@ece.gatech.edu>
//

#ifndef IPV4_L3_PROTOCOL_H
#define IPV4_L3_PROTOCOL_H

#include <list>
#include <stdint.h>
#include "ns3/ipv4-address.h"
#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/ipv4.h"
#include "ns3/traced-callback.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ipv4-static-routing-impl.h"

namespace ns3 {

class Packet;
class NetDevice;
class Ipv4Interface;
class Ipv4Address;
class Ipv4Header;
class Ipv4RoutingTableEntry;
class Ipv4Route;
class Node;
class Socket;
class Ipv4RawSocketImpl;
class Ipv4L4Protocol;
class Ipv4L4Protocol;
class Icmpv4L4Protocol;


/**
 * \brief Implement the Ipv4 layer.
 * 
 * This is the actual implementation of IP.  It contains APIs to send and
 * receive packets at the IP layer, as well as APIs for IP routing.
 */
class Ipv4L3Protocol : public Ipv4
{
public:
  static TypeId GetTypeId (void);
  static const uint16_t PROT_NUMBER;

  Ipv4L3Protocol();
  virtual ~Ipv4L3Protocol ();

  void SetNode (Ptr<Node> node);

  // functions defined in base class Ipv4

  void SetRoutingProtocol (Ptr<Ipv4RoutingProtocol> routingProtocol);
  Ptr<Ipv4RoutingProtocol> GetRoutingProtocol (void) const;

  Ptr<Socket> CreateRawSocket (void);
  void DeleteRawSocket (Ptr<Socket> socket);

  /**
   * \param protocol a template for the protocol to add to this L4 Demux.
   * \returns the L4Protocol effectively added.
   *
   * Invoke Copy on the input template to get a copy of the input
   * protocol which can be used on the Node on which this L4 Demux 
   * is running. The new L4Protocol is registered internally as
   * a working L4 Protocol and returned from this method.
   * The caller does not get ownership of the returned pointer.
   */
  void Insert(Ptr<Ipv4L4Protocol> protocol);
  /**
   * \param protocolNumber number of protocol to lookup
   *        in this L4 Demux
   * \returns a matching L4 Protocol
   *
   * This method is typically called by lower layers
   * to forward packets up the stack to the right protocol.
   * It is also called from NodeImpl::GetUdp for example.
   */
  Ptr<Ipv4L4Protocol> GetProtocol(int protocolNumber) const;
  /**
   * \param protocol protocol to remove from this demux.
   *
   * The input value to this method should be the value
   * returned from the Ipv4L4Protocol::Insert method.
   */
  void Remove (Ptr<Ipv4L4Protocol> protocol);

  /**
   * \param ttl default ttl to use
   *
   * When we need to send an ipv4 packet, we use this default
   * ttl value.
   */
  void SetDefaultTtl (uint8_t ttl);

  /**
   * Lower layer calls this method after calling L3Demux::Lookup
   * The ARP subclass needs to know from which NetDevice this
   * packet is coming to:
   *    - implement a per-NetDevice ARP cache
   *    - send back arp replies on the right device
   */
  void Receive( Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from,
                const Address &to, NetDevice::PacketType packetType);

  /**
   * \param packet packet to send
   * \param source source address of packet
   * \param destination address of packet
   * \param protocol number of packet
   * \param route route entry
   *
   * Higher-level layers call this method to send a packet
   * down the stack to the MAC and PHY layers.
   */
  void Send (Ptr<Packet> packet, Ipv4Address source, 
	     Ipv4Address destination, uint8_t protocol, Ptr<Ipv4Route> route);

  uint32_t AddInterface (Ptr<NetDevice> device);
  Ptr<Ipv4Interface> GetInterface (uint32_t i) const;
  uint32_t GetNInterfaces (void) const;

  int32_t GetInterfaceForAddress (Ipv4Address addr) const;
  int32_t GetInterfaceForPrefix (Ipv4Address addr, Ipv4Mask mask) const;
  int32_t GetInterfaceForDevice (Ptr<const NetDevice> device) const;

  bool AddAddress (uint32_t i, Ipv4InterfaceAddress address);
  Ipv4InterfaceAddress GetAddress (uint32_t interfaceIndex, uint32_t addressIndex) const;
  uint32_t GetNAddresses (uint32_t interface) const;
  bool RemoveAddress (uint32_t interfaceIndex, uint32_t addressIndex);

  void SetMetric (uint32_t i, uint16_t metric);
  uint16_t GetMetric (uint32_t i) const;
  uint16_t GetMtu (uint32_t i) const;
  bool IsUp (uint32_t i) const;
  void SetUp (uint32_t i);
  void SetDown (uint32_t i);

  Ptr<NetDevice> GetNetDevice (uint32_t i);

  void AddRoutingProtocol (Ptr<Ipv4RoutingProtocol> routingProtocol,
                           int16_t priority);

protected:

  virtual void DoDispose (void);
  /**
   * This function will notify other components connected to the node that a new stack member is now connected
   * This will be used to notify Layer 3 protocol of layer 4 protocol stack to connect them together.
   */
  virtual void NotifyNewAggregate ();
private:
  Ipv4L3Protocol(const Ipv4L3Protocol &);
  Ipv4L3Protocol &operator = (const Ipv4L3Protocol &);

  virtual void SetIpForward (bool forward);
  virtual bool GetIpForward (void) const;

  Ipv4Header BuildHeader (
            Ipv4Address source,
            Ipv4Address destination,
            uint8_t protocol,
            uint16_t payloadSize,
            uint8_t ttl,
            bool mayFragment);

  void
  SendRealOut (Ptr<Ipv4Route> route,
               Ptr<Packet> packet,
               Ipv4Header const &ipHeader);

  void 
  IpForward (Ptr<Ipv4Route> rtentry, 
             Ptr<const Packet> p, 
             const Ipv4Header &header);

  void
  IpMulticastForward (Ptr<Ipv4MulticastRoute> mrtentry, 
                      Ptr<const Packet> p, 
                      const Ipv4Header &header);

  void LocalDeliver (Ptr<const Packet> p, Ipv4Header const&ip, uint32_t iif);
  uint32_t AddIpv4Interface (Ptr<Ipv4Interface> interface);
  void SetupLoopback (void);
  Ptr<Icmpv4L4Protocol> GetIcmp (void) const;
  bool IsUnicast (Ipv4Address ad, Ipv4Mask interfaceMask) const;

  typedef std::list<Ptr<Ipv4Interface> > Ipv4InterfaceList;
  typedef std::list<Ptr<Ipv4RawSocketImpl> > SocketList;
  typedef std::list<Ptr<Ipv4L4Protocol> > L4List_t;

  bool m_ipForward;
  L4List_t m_protocols;
  Ipv4InterfaceList m_interfaces;
  uint32_t m_nInterfaces;
  uint8_t m_defaultTtl;
  uint16_t m_identification;
  Ptr<Node> m_node;
  TracedCallback<Ptr<const Packet>, uint32_t> m_txTrace;
  TracedCallback<Ptr<const Packet>, uint32_t> m_rxTrace;
  TracedCallback<Ptr<const Packet> > m_dropTrace;

  Ptr<Ipv4RoutingProtocol> m_routingProtocol;

  SocketList m_sockets;
};

} // Namespace ns3

#endif /* IPV4_L3_PROTOCOL_H */
