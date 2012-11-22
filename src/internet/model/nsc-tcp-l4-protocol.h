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

/**
 * \ingroup nsctcp
 * 
 * \brief Nsc wrapper glue, to interface with the Ipv4 protocol underneath.
 */
class NscTcpL4Protocol : public IpL4Protocol {
public:
  static const uint8_t PROT_NUMBER;
  static TypeId GetTypeId (void);
  /**
   * \brief Constructor
   */
  NscTcpL4Protocol ();
  virtual ~NscTcpL4Protocol ();

  void SetNode (Ptr<Node> node);
  void SetNscLibrary (const std::string &lib);
  std::string GetNscLibrary (void) const;
  virtual int GetProtocolNumber (void) const;
  virtual int GetVersion (void) const;

  /**
   * \return A smart Socket pointer to a NscTcpSocketImpl, allocated by this instance
   * of the TCP protocol
   */
  Ptr<Socket> CreateSocket (void);

  Ipv4EndPoint *Allocate (void);
  Ipv4EndPoint *Allocate (Ipv4Address address);
  Ipv4EndPoint *Allocate (uint16_t port);
  Ipv4EndPoint *Allocate (Ipv4Address address, uint16_t port);
  Ipv4EndPoint *Allocate (Ipv4Address localAddress, uint16_t localPort,
                          Ipv4Address peerAddress, uint16_t peerPort);

  void DeAllocate (Ipv4EndPoint *endPoint);

  /**
   * \brief Receive a packet up the protocol stack
   * \param p The Packet to dump the contents into
   * \param header IPv4 Header information
   * \param incomingInterface The Ipv4Interface it was received on
   */
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
  NscTcpL4Protocol (NscTcpL4Protocol const &);
  NscTcpL4Protocol& operator= (NscTcpL4Protocol const &);

  // NSC callbacks.
  // NSC invokes these hooks to interact with the simulator.
  // In any case, these methods are only to be called by NSC.
  //
  // send_callback is invoked by NSCs 'ethernet driver' to re-inject
  // a packet (i.e. an octet soup consisting of an IP Header, TCP Header
  // and user payload, if any), into ns-3.
  void send_callback (const void *data, int datalen);
  // This is called by the NSC stack whenever something of interest
  // has happened, e.g. when data arrives on a socket, a listen socket
  // has a new connection pending, etc.
  void wakeup ();
  // This is called by the Linux stack RNG initialization.
  // Its also used by the cradle code to add a timestamp to
  // printk/printf/debug output.
  void gettime (unsigned int *sec, unsigned int *usec);
  void AddInterface (void);
  void SoftInterrupt (void);
  friend class NscInterfaceImpl;
  friend class NscTcpSocketImpl;
  Ptr<Node> m_node;
  Ipv4EndPointDemux *m_endPoints;
  INetStack* m_nscStack;
  NscInterfaceImpl *m_nscInterface;
  void *m_dlopenHandle;
  std::string m_nscLibrary;
  Timer m_softTimer;
  std::vector<Ptr<NscTcpSocketImpl> > m_sockets;
  IpL4Protocol::DownTargetCallback m_downTarget;
};

} // namespace ns3

#endif /* NSC_TCP_L4_PROTOCOL_H */
