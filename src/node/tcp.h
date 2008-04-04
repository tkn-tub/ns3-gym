/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Georgia Tech Research Corporation
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
 * Author: Raj Bhattacharjea <raj.b@gatech.edu>
 */
#ifndef TCP_H
#define TCP_H

#include "socket-factory.h"

namespace ns3 {

class Socket;

/**
 * \brief API to create TCP socket instances 
 *
 * This abstract class defines the API for TCP sockets.
 * This class also holds the global default variables used to
 * initialize newly created sockets, such as values that are
 * set through the sysctl or proc interfaces in Linux.

 * All TCP implementations must provide an implementation of CreateSocket
 * below, and should make use of the default values configured below.
 * 
 * \see TcpImpl
 *
 */
class Tcp : public SocketFactory
{
public:
  static TypeId GetTypeId (void);

  virtual Ptr<Socket> CreateSocket (void) = 0;

  uint32_t GetDefaultSegSize (void) const;
  uint32_t GetDefaultAdvWin (void) const;
  uint32_t GetDefaultSsThresh (void) const;
  uint32_t GetDefaultTxBuffer (void) const;
  uint32_t GetDefaultRxBuffer (void) const;
  uint32_t GetDefaultInitialCwnd (void) const;
  uint32_t GetDefaultConnTimeout (void) const;
  uint32_t GetDefaultConnCount (void) const;
  double   GetDefaultDelAckTimeout (void) const;
  uint32_t GetDefaultDelAckCount (void) const;

private:
  uint32_t  m_defaultSegSize;
  uint32_t  m_defaultAdvWin; 
  uint32_t  m_defaultSsThresh;
  uint32_t  m_defaultTxBuffer;
  uint32_t  m_defaultRxBuffer;
  uint32_t  m_defaultInitialCwnd;
  uint32_t  m_defaultConnTimeout;
  uint32_t  m_defaultConnCount;
  double    m_defaultDelAckTimeout;
  uint32_t  m_defaultDelAckCount;

};

} // namespace ns3

#endif /* TCP_H */
