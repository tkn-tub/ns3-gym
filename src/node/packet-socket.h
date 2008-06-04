/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Emmanuelle Laprise, INRIA
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
 * Authors: Emmanuelle Laprise <emmanuelle.laprise@bluekazoo.ca>,
 *          Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef PACKET_SOCKET_H
#define PACKET_SOCKET_H

#include <stdint.h>
#include <queue>
#include "ns3/callback.h"
#include "ns3/traced-callback.h"
#include "ns3/ptr.h"
#include "ns3/socket.h"

namespace ns3 {

class Node;
class Packet;
class NetDevice;
class PacketSocketAddress;

/**
 * \ingroup socket
 *
 * \brief A PacketSocket is a link between an application and a net device.
 *
 * A PacketSocket can be used to connect an application to a net
 * device. The application provides the buffers of data, the socket
 * conserts them to a raw packet and the net device then adds the
 * protocol specific headers and trailers. This socket type
 * is very similar to the linux and BSD "packet" sockets.
 *
 * Here is a summary of the semantics of this class:
 * - Bind: Bind uses only the protocol and device fields of the 
 *       PacketSocketAddress. If none are provided, Bind uses 
 *       zero for both, which means that the socket is bound
 *       to all protocols on all devices on the node.
 *
 * - Connect: uses only the protocol, device and "physical address" 
 *       field of the PacketSocketAddress. It is used to set the default
 *       destination address for outgoing packets.
 *
 * - Send: send the input packet to the underlying NetDevices
 *       with the default destination address. The socket must
 *       be bound and connected.
 *
 * - SendTo: uses the protocol, device, and "physical address" 
 *       fields of the PacketSocketAddress. The device value is 
 *       used to specialize the packet transmission to a single 
 *       device, the protocol value specifies the protocol of this
 *       packet only and the "physical address" field is used to override the 
 *       default destination address. The socket must be bound.
 *
 * - Recv: The address represents the address of the packer originator.
 *       The fields "physical address", device, and protocol are filled.
 *
 * - Accept: not allowed
 *
 * - Listen: returns -1 (OPNOTSUPP)
 */
class PacketSocket : public Socket
{
public:
  static TypeId GetTypeId (void);

  PacketSocket ();
  virtual ~PacketSocket ();

  void SetNode (Ptr<Node> node);

  virtual enum SocketErrno GetErrno (void) const;
  virtual Ptr<Node> GetNode (void) const;
  virtual int Bind (void);
  virtual int Bind (const Address & address);
  virtual int Close (void);
  virtual int ShutdownSend (void);
  virtual int ShutdownRecv (void);
  virtual int Connect(const Address &address);
  virtual int Listen(uint32_t queueLimit);
  virtual int Send (Ptr<Packet> p);
  virtual uint32_t GetTxAvailable (void) const;

  virtual int SendTo(Ptr<Packet> p, const Address &address);

  virtual Ptr<Packet> Recv (uint32_t maxSize, uint32_t flags);
  virtual uint32_t GetRxAvailable (void) const;

private:
  void ForwardUp (Ptr<NetDevice> device, Ptr<Packet> packet, 
                  uint16_t protocol, const Address &from);
  int DoBind (const PacketSocketAddress &address);
  virtual void DoDispose (void);

  enum State {
    STATE_OPEN,
    STATE_BOUND,     // open and bound
    STATE_CONNECTED, // open, bound and connected
    STATE_CLOSED
  };
  Ptr<Node> m_node;
  enum SocketErrno m_errno;
  bool m_shutdownSend;
  bool m_shutdownRecv;
  enum State m_state;
  uint16_t m_protocol;
  bool m_isSingleDevice;
  uint32_t m_device;
  Address m_destAddr; /// Default destination address

  std::queue<Ptr<Packet> > m_deliveryQueue;
  uint32_t m_rxAvailable;
  
  TracedCallback<Ptr<const Packet> > m_dropTrace;
  
  // Socket options (attributes)
  uint32_t m_rcvBufSize;

};

}//namespace ns3

#endif /* PACKET_SOCKET_H */


