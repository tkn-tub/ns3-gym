/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
#include "ipv4-end-point-demux.h"
#include "ipv4-l4-protocol.h"
#include "ipv4-interface.h"

#include "tcp-header.h"

#include "ns3/timer.h"
#include "sim_interface.h"
#include "nsc-tcp-socket-impl.h"

namespace ns3 {

class Node;
class Socket;
class TcpHeader;
/**
 * \ingroup nsctcp
 * 
 * \brief Nsc wrapper glue, to interface with the Ipv4 protocol underneath.
 */
class NscTcpL4Protocol : public Ipv4L4Protocol, ISendCallback, IInterruptCallback {
public:
  static const uint8_t PROT_NUMBER;
  static TypeId GetTypeId (void);
  /**
   * \brief Constructor
   */
  NscTcpL4Protocol ();
  virtual ~NscTcpL4Protocol ();

  void SetNode (Ptr<Node> node);
  void SetNscLibrary(const std::string &lib);

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
   * \brief Recieve a packet up the protocol stack
   * \param p The Packet to dump the contents into
   * \param source The source's Ipv4Address
   * \param destination The destinations Ipv4Address
   * \param incomingInterface The Ipv4Interface it was received on
   */
  virtual Ipv4L4Protocol::RxStatus Receive (Ptr<Packet> p,
                                            Ipv4Address const &source,
                                            Ipv4Address const &destination,
                                            Ptr<Ipv4Interface> incomingInterface);

  // NSC callbacks.
  // NSC invokes these hooks to interact with the simulator.
  // In any case, these methods are only to be called by NSC.
  //
  // send_callback is invoked by NSCs 'ethernet driver' to re-inject
  // a packet (i.e. an octet soup consisting of an IP Header, TCP Header
  // and user payload, if any), into ns-3.
  virtual void send_callback(const void *data, int datalen);
  // This is called by the NSC stack whenever something of interest
  // has happened, e.g. when data arrives on a socket, a listen socket
  // has a new connection pending, etc.
  virtual void wakeup();
  // This is called by the Linux stack RNG initialization.
  // Its also used by the cradle code to add a timestamp to
  // printk/printf/debug output.
  virtual void gettime(unsigned int *, unsigned int *);

protected:
  virtual void DoDispose (void);
private:
  Ptr<Node> m_node;
  Ipv4EndPointDemux *m_endPoints;
  ObjectFactory m_rttFactory;
private:
  void AddInterface (void);
  void SoftInterrupt (void);
  static ObjectFactory GetDefaultRttEstimatorFactory (void);
  friend class NscTcpSocketImpl;
  INetStack* m_nscStack;
  void *m_dlopenHandle;
  Timer m_softTimer;
  std::vector<Ptr<NscTcpSocketImpl> > m_sockets;
};

}; // namespace ns3

#endif /* NSC_TCP_L4_PROTOCOL_H */
