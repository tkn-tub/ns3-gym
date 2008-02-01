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
#ifndef UDP_H
#define UDP_H

#include "socket-factory.h"

namespace ns3 {

class Socket;

/**
 * \brief API to create UDP socket instances 
 *
 * This abstract class defines the API for UDP sockets.
 * This class also can hold the global default variables used to
 * initialize newly created sockets, such as values that are
 * set through the sysctl or proc interfaces in Linux.

 * All UDP implementations must provide an implementation of CreateSocket
 * below.
 * 
 * \see UdpImpl
 */
class Udp : public SocketFactory
{
public:
  static TypeId GetTypeId (void);

  Udp ();

  /**
   * \return smart pointer to Socket
   * 
   * API for creating socket instances; must be implemented by UDP 
   * implementations..
   */
  virtual Ptr<Socket> CreateSocket (void) = 0;
};

} // namespace ns3

#endif /* UDP_H */
