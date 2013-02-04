// -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*-
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
#include <map>
#include <vector>
#include <stdint.h>
#include "ns3/ipv4-address.h"
#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/ipv4.h"
#include "ns3/traced-callback.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/nstime.h"
#include "ns3/simulator.h"

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
class IpL4Protocol;
class Icmpv4L4Protocol;


/**
 * \brief Implement the Ipv4 layer.
 * 
 * This is the actual implementation of IP.  It contains APIs to send and
 * receive packets at the IP layer, as well as APIs for IP routing.
 *
 * This class contains two distinct groups of trace sources.  The
 * trace sources 'Rx' and 'Tx' are called, respectively, immediately
 * after receiving from the NetDevice and immediately before sending
 * to a NetDevice for transmitting a packet.  These are low level
 * trace sources that include the Ipv4Header already serialized into
 * the packet.  In contrast, the Drop, SendOutgoing, UnicastForward,
 * and LocalDeliver trace sources are slightly higher-level and pass
 * around the Ipv4Header as an explicit parameter and not as part of
 * the packet.
 *
 * IP fragmentation and reassembly is handled at this level.
 * At the moment the fragmentation does not handle IP option headers,
 * and in particular the ones that shall not be fragmented.
 * Moreover, the actual implementation does not mimic exactly the Linux
 * kernel. Hence it is not possible, for instance, to test a fragmentation
 * attack.
 */
class Ipv4L3Protocol : public Ipv4
{
public:
  static TypeId GetTypeId (void);
  static const uint16_t PROT_NUMBER;

  Ipv4L3Protocol();
  virtual ~Ipv4L3Protocol ();

  /**
   * \enum DropReason
   * \brief Reason why a packet has been dropped.
   */
  enum DropReason 
  {
    DROP_TTL_EXPIRED = 1,   /**< Packet TTL has expired */
    DROP_NO_ROUTE,   /**< No route to host */
    DROP_BAD_CHECKSUM,   /**< Bad checksum */
    DROP_INTERFACE_DOWN,   /**< Interface is down so can not send packet */
    DROP_ROUTE_ERROR,   /**< Route error */
    DROP_FRAGMENT_TIMEOUT /**< Fragment timeout exceeded */
  };

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
  void Insert (Ptr<IpL4Protocol> protocol);
  /**
   * \param protocolNumber number of protocol to lookup
   *        in this L4 Demux
   * \returns a matching L4 Protocol
   *
   * This method is typically called by lower layers
   * to forward packets up the stack to the right protocol.
   */
  Ptr<IpL4Protocol> GetProtocol (int protocolNumber) const;
  /**
   * \param protocol protocol to remove from this demux.
   *
   * The input value to this method should be the value
   * returned from the Ipv4L4Protocol::Insert method.
   */
  void Remove (Ptr<IpL4Protocol> protocol);

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
   * \param device network device
   * \param p the packet
   * \param protocol protocol value
   * \param from address of the correspondant
   * \param to address of the destination
   * \param packetType type of the packet
   */
  void Receive ( Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from,
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
  /**
   * \param packet packet to send
   * \param ipHeader IP Header
   * \param route route entry
   *
   * Higher-level layers call this method to send a packet with IPv4 Header
   * (Intend to be used with IpHeaderInclude attribute.)
   */
  void SendWithHeader (Ptr<Packet> packet, Ipv4Header ipHeader, Ptr<Ipv4Route> route);

  uint32_t AddInterface (Ptr<NetDevice> device);
  Ptr<Ipv4Interface> GetInterface (uint32_t i) const;
  uint32_t GetNInterfaces (void) const;

  int32_t GetInterfaceForAddress (Ipv4Address addr) const;
  int32_t GetInterfaceForPrefix (Ipv4Address addr, Ipv4Mask mask) const;
  int32_t GetInterfaceForDevice (Ptr<const NetDevice> device) const;
  bool IsDestinationAddress (Ipv4Address address, uint32_t iif) const;

  bool AddAddress (uint32_t i, Ipv4InterfaceAddress address);
  Ipv4InterfaceAddress GetAddress (uint32_t interfaceIndex, uint32_t addressIndex) const;
  uint32_t GetNAddresses (uint32_t interface) const;
  bool RemoveAddress (uint32_t interfaceIndex, uint32_t addressIndex);
  Ipv4Address SelectSourceAddress (Ptr<const NetDevice> device,
                                   Ipv4Address dst, Ipv4InterfaceAddress::InterfaceAddressScope_e scope);


  void SetMetric (uint32_t i, uint16_t metric);
  uint16_t GetMetric (uint32_t i) const;
  uint16_t GetMtu (uint32_t i) const;
  bool IsUp (uint32_t i) const;
  void SetUp (uint32_t i);
  void SetDown (uint32_t i);
  bool IsForwarding (uint32_t i) const;
  void SetForwarding (uint32_t i, bool val);

  Ptr<NetDevice> GetNetDevice (uint32_t i);

protected:

  virtual void DoDispose (void);
  /**
   * This function will notify other components connected to the node that a new stack member is now connected
   * This will be used to notify Layer 3 protocol of layer 4 protocol stack to connect them together.
   */
  virtual void NotifyNewAggregate ();
private:
  friend class Ipv4L3ProtocolTestCase;
  Ipv4L3Protocol(const Ipv4L3Protocol &);
  Ipv4L3Protocol &operator = (const Ipv4L3Protocol &);

  // class Ipv4 attributes
  virtual void SetIpForward (bool forward);
  virtual bool GetIpForward (void) const;
  virtual void SetWeakEsModel (bool model);
  virtual bool GetWeakEsModel (void) const;

  Ipv4Header BuildHeader (
    Ipv4Address source,
    Ipv4Address destination,
    uint8_t protocol,
    uint16_t payloadSize,
    uint8_t ttl,
    uint8_t tos,
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
  void RouteInputError (Ptr<const Packet> p, const Ipv4Header & ipHeader, Socket::SocketErrno sockErrno);

  uint32_t AddIpv4Interface (Ptr<Ipv4Interface> interface);
  void SetupLoopback (void);

  /**
   * \brief Get ICMPv4 protocol.
   * \return Icmpv4L4Protocol pointer
   */
  Ptr<Icmpv4L4Protocol> GetIcmp (void) const;
  bool IsUnicast (Ipv4Address ad, Ipv4Mask interfaceMask) const;

  /**
   * \brief Fragment a packet
   * \param packet the packet
   * \param outIfaceMtu the MTU of the interface
   * \param listFragments the list of fragments
   */
  void DoFragmentation (Ptr<Packet> packet, uint32_t outIfaceMtu, std::list<Ptr<Packet> >& listFragments);

  /**
   * \brief Process a packet fragment
   * \param packet the packet
   * \param fragmentSize the size of the fragment
   * \param iif Input Interface
   * \return true is the fragment completed the packet
   */
  bool ProcessFragment (Ptr<Packet>& packet, Ipv4Header & ipHeader, uint32_t iif);

  /**
   * \brief Process the timeout for packet fragments
   * \param key representing the packet fragments
   * \param ipHeader the IP header of the original packet
   * \param iif Input Interface
   */
  void HandleFragmentsTimeout ( std::pair<uint64_t, uint32_t> key, Ipv4Header & ipHeader, uint32_t iif);

  typedef std::vector<Ptr<Ipv4Interface> > Ipv4InterfaceList;
  typedef std::list<Ptr<Ipv4RawSocketImpl> > SocketList;
  typedef std::list<Ptr<IpL4Protocol> > L4List_t;

  bool m_ipForward;
  bool m_weakEsModel;
  L4List_t m_protocols;
  Ipv4InterfaceList m_interfaces;
  uint8_t m_defaultTos;
  uint8_t m_defaultTtl;
  uint16_t m_identification;
  Ptr<Node> m_node;

  TracedCallback<const Ipv4Header &, Ptr<const Packet>, uint32_t> m_sendOutgoingTrace;
  TracedCallback<const Ipv4Header &, Ptr<const Packet>, uint32_t> m_unicastForwardTrace;
  TracedCallback<const Ipv4Header &, Ptr<const Packet>, uint32_t> m_localDeliverTrace;

  // The following two traces pass a packet with an IP header
  TracedCallback<Ptr<const Packet>, Ptr<Ipv4>,  uint32_t> m_txTrace;
  TracedCallback<Ptr<const Packet>, Ptr<Ipv4>, uint32_t> m_rxTrace;
  // <ip-header, payload, reason, ifindex> (ifindex not valid if reason is DROP_NO_ROUTE)
  TracedCallback<const Ipv4Header &, Ptr<const Packet>, DropReason, Ptr<Ipv4>, uint32_t> m_dropTrace;

  Ptr<Ipv4RoutingProtocol> m_routingProtocol;

  SocketList m_sockets;

  /**
   * \class Fragments
   * \brief A Set of Fragment belonging to the same packet (src, dst, identification and proto)
   */
  class Fragments : public SimpleRefCount<Fragments>
  {
public:
    /**
     * \brief Constructor.
     */
    Fragments ();

    /**
     * \brief Destructor.
     */
    ~Fragments ();

    /**
     * \brief Add a fragment.
     * \param fragment the fragment
     * \param fragmentOffset the offset of the fragment
     * \param moreFragment the bit "More Fragment"
     */
    void AddFragment (Ptr<Packet> fragment, uint16_t fragmentOffset, bool moreFragment);

    /**
     * \brief If all fragments have been added.
     * \returns true if the packet is entire
     */
    bool IsEntire () const;

    /**
     * \brief Get the entire packet.
     * \return the entire packet
     */
    Ptr<Packet> GetPacket () const;

    /**
     * \brief Get the complete part of the packet.
     * \return the part we have comeplete
     */
    Ptr<Packet> GetPartialPacket () const;

private:
    /**
     * \brief True if other fragments will be sent.
     */
    bool m_moreFragment;

    /**
     * \brief The current fragments.
     */
    std::list<std::pair<Ptr<Packet>, uint16_t> > m_fragments;

  };

  typedef std::map< std::pair<uint64_t, uint32_t>, Ptr<Fragments> > MapFragments_t;
  typedef std::map< std::pair<uint64_t, uint32_t>, EventId > MapFragmentsTimers_t;

  /**
   * \brief The hash of fragmented packets.
   */
  MapFragments_t       m_fragments;
  Time                 m_fragmentExpirationTimeout;
  MapFragmentsTimers_t m_fragmentsTimers;

};

} // Namespace ns3

#endif /* IPV4_L3_PROTOCOL_H */
