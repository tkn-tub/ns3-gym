/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */

#ifndef NSC_TCP_L4_PROTOCOL_H
#define NSC_TCP_L4_PROTOCOL_H

#include <stdint.h>

#include "ns3/packet.h"
#include "ns3/ipv4-address.h"
#include "ns3/ptr.h"
#include "ns3/object-factory.h"
#include "ns3/timer.h"
#include "ip-l4-protocol.h"

struct INetStack;

namespace ns3 {

class Node;
class Socket;
class Ipv4EndPointDemux;
class Ipv4Interface;
class NscTcpSocketImpl;
class Ipv4EndPoint;
class NscInterfaceImpl;
class NetDevice;

/**
 * \ingroup nsctcp
 * 
 * \brief Nsc wrapper glue, to interface with the Ipv4 protocol underneath.
 */
class NscTcpL4Protocol : public IpL4Protocol {
public:
  static const uint8_t PROT_NUMBER; //!< protocol number (0x6)
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  NscTcpL4Protocol ();
  virtual ~NscTcpL4Protocol ();

  /**
   * Set node associated with this stack
   * \param node the node
   */
  void SetNode (Ptr<Node> node);

  /**
   * Set the NSC library to be used
   * \param lib the library path
   */
  void SetNscLibrary (const std::string &lib);

  /**
   * Get the NSC library being used
   * \returns the library path
   */
  std::string GetNscLibrary (void) const;
  virtual int GetProtocolNumber (void) const;

  /**
   * Get the NSC version
   * \returns the NSC version
   */
  virtual int GetVersion (void) const;

  /**
   * \return A smart Socket pointer to a NscTcpSocketImpl, allocated by this instance
   * of the TCP protocol
   */
  Ptr<Socket> CreateSocket (void);

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
   * \param boundNetDevice Bound NetDevice (if any)
   * \param port port to use
   * \return the Endpoint
   */
  Ipv4EndPoint *Allocate (Ptr<NetDevice> boundNetDevice, uint16_t port);
  /**
   * \brief Allocate an IPv4 Endpoint
   * \param boundNetDevice Bound NetDevice (if any)
   * \param address address to use
   * \param port port to use
   * \return the Endpoint
   */
  Ipv4EndPoint *Allocate (Ptr<NetDevice> boundNetDevice, Ipv4Address address, uint16_t port);
  /**
   * \brief Allocate an IPv4 Endpoint
   * \param boundNetDevice Bound NetDevice (if any)
   * \param localAddress local address to use
   * \param localPort local port to use
   * \param peerAddress remote address to use
   * \param peerPort remote port to use
   * \return the Endpoint
   */
  Ipv4EndPoint *Allocate (Ptr<NetDevice> boundNetDevice,
                          Ipv4Address localAddress, uint16_t localPort,
                          Ipv4Address peerAddress, uint16_t peerPort);


  /**
   * \brief Remove an IPv4 Endpoint.
   * \param endPoint the end point to remove
   */
  void DeAllocate (Ipv4EndPoint *endPoint);

  virtual IpL4Protocol::RxStatus Receive (Ptr<Packet> p,
                                          Ipv4Header const &header,
                                          Ptr<Ipv4Interface> incomingInterface);
  virtual IpL4Protocol::RxStatus Receive (Ptr<Packet> p,
                                          Ipv6Header const &header,
                                          Ptr<Ipv6Interface> interface);

  // From IpL4Protocol
  virtual void SetDownTarget (IpL4Protocol::DownTargetCallback cb);
  virtual void SetDownTarget6 (IpL4Protocol::DownTargetCallback6 cb);
  // From IpL4Protocol
  virtual IpL4Protocol::DownTargetCallback GetDownTarget (void) const;
  virtual IpL4Protocol::DownTargetCallback6 GetDownTarget6 (void) const;
protected:
  virtual void DoDispose (void);
  virtual void NotifyNewAggregate ();
private:
  /**
   * \brief Copy constructor
   *
   * Defined and not implemented to avoid misuse
   */
  NscTcpL4Protocol (NscTcpL4Protocol const &);
  /**
   * \brief Copy constructor
   *
   * Defined and not implemented to avoid misuse
   * \returns
   */
  NscTcpL4Protocol& operator= (NscTcpL4Protocol const &);

  // NSC callbacks.
  // NSC invokes these hooks to interact with the simulator.
  // In any case, these methods are only to be called by NSC.

  /**
   * \brief Invoked by NSCs 'ethernet driver' to re-inject a packet into ns-3.
   *
   * A packet is an octet soup consisting of an IP Header, TCP Header
   * and user payload, if any
   *
   * \param data the data
   * \param datalen the data length
   */
  void send_callback (const void *data, int datalen);
  /**
   * \brief Called by the NSC stack whenever something of interest has happened
   *
   * Examples: when data arrives on a socket, a listen socket
   * has a new connection pending, etc.
   */
  void wakeup ();
  /**
   * \brief Called by the Linux stack RNG initialization
   *
   * Its also used by the cradle code to add a timestamp to
   * printk/printf/debug output.
   * \param sec seconds
   * \param usec microseconds
   */
  void gettime (unsigned int *sec, unsigned int *usec);
  /**
   * \brief Add an interface
   *
   * Actually NSC only supports one interface per node (\bugid{1398})
   */
  void AddInterface (void);

  /**
   * \brief Provide a "soft" interrupt to NSC
   */
  void SoftInterrupt (void);
  /**
   * \brief NscInterfaceImpl friend class.
   * \relates NscInterfaceImpl
   */
  friend class NscInterfaceImpl;
  /**
   * \brief NscTcpSocketImpl friend class.
   * \relates NscTcpSocketImpl
   */
  friend class NscTcpSocketImpl;

  Ptr<Node> m_node; //!< the node this stack is associated with
  Ipv4EndPointDemux *m_endPoints; //!< A list of IPv4 end points.
  INetStack* m_nscStack; //!< the NSC stack.
  NscInterfaceImpl *m_nscInterface; //!< the NSC Interface.
  void *m_dlopenHandle; //!< dynamic library handle.
  std::string m_nscLibrary; //!< path to the NSC library.
  Timer m_softTimer; //!< Soft interrupt timer
  std::vector<Ptr<NscTcpSocketImpl> > m_sockets; //!< list of sockets
  IpL4Protocol::DownTargetCallback m_downTarget; //!< Callback to send packets over IPv4
};

} // namespace ns3

#endif /* NSC_TCP_L4_PROTOCOL_H */
