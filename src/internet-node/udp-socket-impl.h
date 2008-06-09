/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef UDP_SOCKET_IMPL_H
#define UDP_SOCKET_IMPL_H

#include <stdint.h>
#include <queue>
#include "ns3/callback.h"
#include "ns3/traced-callback.h"
#include "ns3/socket.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"
#include "ns3/udp-socket.h"

namespace ns3 {

class Ipv4EndPoint;
class Node;
class Packet;
class UdpL4Protocol;

class UdpSocketImpl : public UdpSocket
{
public:
  static TypeId GetTypeId (void);
  /**
   * Create an unbound udp socket.
   */
  UdpSocketImpl ();
  virtual ~UdpSocketImpl ();

  void SetNode (Ptr<Node> node);
  void SetUdp (Ptr<UdpL4Protocol> udp);

  virtual enum SocketErrno GetErrno (void) const;
  virtual Ptr<Node> GetNode (void) const;
  virtual int Bind (void);
  virtual int Bind (const Address &address);
  virtual int Close (void);
  virtual int ShutdownSend (void);
  virtual int ShutdownRecv (void);
  virtual int Connect(const Address &address);
  virtual int Listen (uint32_t queueLimit);
  virtual uint32_t GetTxAvailable (void) const;
  virtual int Send (Ptr<Packet> p, uint32_t flags);
  virtual int SendTo (Ptr<Packet> p, uint32_t flags, const Address &address);
  virtual uint32_t GetRxAvailable (void) const;
  virtual Ptr<Packet> Recv (uint32_t maxSize, uint32_t flags);
  virtual Ptr<Packet> RecvFrom (uint32_t maxSize, uint32_t flags,
    Address &fromAddress);

private:
  // Attributes set through UdpSocket base class 
  virtual void SetRcvBufSize (uint32_t size);
  virtual uint32_t GetRcvBufSize (void) const;
  virtual void SetIpTtl (uint32_t ipTtl);
  virtual uint32_t GetIpTtl (void) const;
  virtual void SetIpMulticastTtl (uint32_t ipTtl);
  virtual uint32_t GetIpMulticastTtl (void) const;

  friend class UdpSocketFactory;
  // invoked by Udp class
  int FinishBind (void);
  void ForwardUp (Ptr<Packet> p, Ipv4Address ipv4, uint16_t port);
  void Destroy (void);
  int DoSend (Ptr<Packet> p);
  int DoSendTo (Ptr<Packet> p, const Address &daddr);
  int DoSendTo (Ptr<Packet> p, Ipv4Address daddr, uint16_t dport);

  Ipv4EndPoint *m_endPoint;
  Ptr<Node> m_node;
  Ptr<UdpL4Protocol> m_udp;
  Ipv4Address m_defaultAddress;
  uint16_t m_defaultPort;
  Callback<void,Ptr<Socket>,uint32_t,const Address &> m_dummyRxCallback;
  Callback<void,Ptr<Socket>,uint8_t const*,uint32_t,const Address &> m_rxCallback;
  TracedCallback<Ptr<const Packet> > m_dropTrace;

  enum SocketErrno m_errno;
  bool m_shutdownSend;
  bool m_shutdownRecv;
  bool m_connected;

  std::queue<Ptr<Packet> > m_deliveryQueue;
  uint32_t m_rxAvailable;
  
  // Socket attributes
  uint32_t m_rcvBufSize;
  uint32_t m_ipTtl;
  uint32_t m_ipMulticastTtl;

};

}//namespace ns3

#endif /* UDP_SOCKET_IMPL_H */
