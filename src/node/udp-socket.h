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
 * \ingroup socket
 *
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


