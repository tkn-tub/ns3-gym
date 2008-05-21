/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 Georgia Tech Research Corporation
 *               2007 INRIA
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
 * Authors: George F. Riley<riley@ece.gatech.edu>
 *          Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#ifndef __UDP_SOCKET_H__
#define __UDP_SOCKET_H__

#include "socket.h"
#include "ns3/traced-callback.h"
#include "ns3/callback.h"
#include "ns3/ptr.h"
#include "ns3/object.h"

namespace ns3 {

class Node;
class Packet;

/**
 * \brief (abstract) base class of all UdpSockets
 *
 * This class exists solely for hosting UdpSocket attributes that can
 * be reused across different implementations.
 */
class UdpSocket : public Socket
{
public:
 static TypeId GetTypeId (void);
 
  UdpSocket (void);
  virtual ~UdpSocket (void);

  virtual enum Socket::SocketErrno GetErrno (void) const = 0;
  virtual Ptr<Node> GetNode (void) const = 0;
  virtual int Bind () = 0;
  virtual int Close (void) = 0;
  virtual int ShutdownSend (void) = 0;
  virtual int ShutdownRecv (void) = 0;
  virtual int Connect (const Address &address) = 0;
  virtual int Send (Ptr<Packet> p) = 0;
  virtual uint32_t GetTxAvailable (void) const = 0;
  virtual int SendTo (Ptr<Packet> p, const Address &address) = 0;
  virtual Ptr<Packet> Recv (uint32_t maxSize, uint32_t flags) = 0;
  virtual uint32_t GetRxAvailable (void) const = 0;

private:
  // Indirect the attribute setting and getting through private virtual methods
  virtual void SetRcvBufSize (uint32_t size) = 0;
  virtual uint32_t GetRcvBufSize (void) const = 0;
  virtual void SetIpTtl (uint32_t ipTtl) = 0;
  virtual uint32_t GetIpTtl (void) const = 0;
  virtual void SetIpMulticastTtl (uint32_t ipTtl) = 0;
  virtual uint32_t GetIpMulticastTtl (void) const = 0;
  
};

} //namespace ns3

#endif /* UDP_SOCKET_H */


