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
#include "ns3/callback.h"
#include "ns3/ptr.h"
#include "ns3/socket.h"

namespace ns3 {

class Node;
class Packet;
class NetDevice;
class PacketSocketAddress;

/**
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
 */
class PacketSocket : public Socket
{
public:
  PacketSocket (Ptr<Node> node);
  virtual ~PacketSocket ();

  virtual enum SocketErrno GetErrno (void) const;
  virtual Ptr<Node> GetNode (void) const;
  virtual int Bind (void);
  virtual int Bind (const Address & address);
  virtual int ShutdownSend (void);
  virtual int ShutdownRecv (void);

private:
  virtual int DoClose(Callback<void, Ptr<Socket> > closeCompleted);
  virtual int DoConnect(const Address & address,
                        Callback<void, Ptr<Socket> > connectionSucceeded,
                        Callback<void, Ptr<Socket> > connectionFailed,
                        Callback<void, Ptr<Socket> > halfClose);
  virtual int DoAccept(Callback<bool, Ptr<Socket>, const Address&> connectionRequest,
                       Callback<void, Ptr<Socket>, const Address&> newConnectionCreated,
                       Callback<void, Ptr<Socket> > closeRequested);
  virtual int DoSend (const uint8_t* buffer,
                      uint32_t size,
                      Callback<void, Ptr<Socket>, uint32_t> dataSent);
  virtual int DoSendTo(const Address &address,
                       const uint8_t *buffer,
                       uint32_t size,
                       Callback<void, Ptr<Socket>, uint32_t> dataSent);
  virtual void DoRecv(Callback<void, Ptr<Socket>, const uint8_t*, uint32_t,const Address&> receive);
  virtual void DoRecvDummy(Callback<void, Ptr<Socket>, uint32_t,const Address&>);

private:
  void Init (void);
  void ForwardUp (Ptr<NetDevice> device, const Packet &packet, 
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
  Callback<void,Ptr<Socket>,uint32_t,const Address &> m_dummyRxCallback;
  Callback<void,Ptr<Socket>,uint8_t const*,uint32_t, const Address &> m_rxCallback;
  enum SocketErrno m_errno;
  bool m_shutdownSend;
  bool m_shutdownRecv;
  enum State m_state;
  uint16_t m_protocol;
  uint32_t m_device;
  Address m_destAddr; /// Default destination address
};

}//namespace ns3

#endif /* PACKET_SOCKET_H */


