/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * All rights reserved.
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
#ifndef UDP_IMPL_H
#define UDP_IMPL_H

#include "ns3/udp.h"
#include "ns3/ptr.h"

namespace ns3 {

class UdpL4Protocol;

/**
 * \brief Object to create UDP socket instances 
 *
 * This class implements the API for UDP sockets.
 * It is a socket factory (deriving from class SocketFactory) and can
 * also hold global variables used to initialize newly created sockets, 
 * such as values that are set through the sysctl or proc interfaces in Linux.
 */
class UdpImpl : public Udp
{
public:
  UdpImpl (Ptr<UdpL4Protocol> udp);
  virtual ~UdpImpl ();

  /**
   * \return smart pointer to Socket
   * 
   * Implements a method to create a UdpImpl-based socket and return
   * a base class smart pointer to the socket.
   */
  virtual Ptr<Socket> CreateSocket (void);

protected:
  virtual void DoDispose (void);
private:
  Ptr<UdpL4Protocol> m_udp;
};

} // namespace ns3

#endif /* UDP_IMPL_H */
