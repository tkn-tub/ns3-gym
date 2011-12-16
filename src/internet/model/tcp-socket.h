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

#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include "ns3/socket.h"
#include "ns3/traced-callback.h"
#include "ns3/callback.h"
#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/nstime.h"

namespace ns3 {

class Node;
class Packet;

/* Names of the 11 TCP states */
typedef enum {
  CLOSED,       // 0
  LISTEN,       // 1
  SYN_SENT,     // 2
  SYN_RCVD,     // 3
  ESTABLISHED,  // 4
  CLOSE_WAIT,   // 5
  LAST_ACK,     // 6
  FIN_WAIT_1,   // 7
  FIN_WAIT_2,   // 8
  CLOSING,      // 9
  TIME_WAIT,   // 10
  LAST_STATE
} TcpStates_t;

/**
 * \ingroup socket
 *
 * \brief (abstract) base class of all TcpSockets
 *
 * This class exists solely for hosting TcpSocket attributes that can
 * be reused across different implementations.
 */
class TcpSocket : public Socket
{
public:
  static TypeId GetTypeId (void);
 
  TcpSocket (void);
  virtual ~TcpSocket (void);

  // Literal names of TCP states for use in log messages */
  static const char* const TcpStateName[LAST_STATE];

private:
  // Indirect the attribute setting and getting through private virtual methods
  virtual void SetSndBufSize (uint32_t size) = 0;
  virtual uint32_t GetSndBufSize (void) const = 0;
  virtual void SetRcvBufSize (uint32_t size) = 0;
  virtual uint32_t GetRcvBufSize (void) const = 0;
  virtual void SetSegSize (uint32_t size) = 0;
  virtual uint32_t GetSegSize (void) const = 0;
  virtual void SetSSThresh (uint32_t threshold) = 0;
  virtual uint32_t GetSSThresh (void) const = 0;
  virtual void SetInitialCwnd (uint32_t count) = 0;
  virtual uint32_t GetInitialCwnd (void) const = 0;
  virtual void SetConnTimeout (Time timeout) = 0;
  virtual Time GetConnTimeout (void) const = 0;
  virtual void SetConnCount (uint32_t count) = 0;
  virtual uint32_t GetConnCount (void) const = 0;
  virtual void SetDelAckTimeout (Time timeout) = 0;
  virtual Time GetDelAckTimeout (void) const = 0;
  virtual void SetDelAckMaxCount (uint32_t count) = 0;
  virtual uint32_t GetDelAckMaxCount (void) const = 0;
  virtual void SetTcpNoDelay (bool noDelay) = 0;
  virtual bool GetTcpNoDelay (void) const = 0;
  virtual void SetPersistTimeout (Time timeout) = 0;
  virtual Time GetPersistTimeout (void) const = 0;

};

} // namespace ns3

#endif /* TCP_SOCKET_H */


