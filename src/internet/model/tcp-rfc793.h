/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Adrian Sai-wah Tam
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
 * Author: Adrian Sai-wah Tam <adrian.sw.tam@gmail.com>
 */

#ifndef TCP_RFC793_H
#define TCP_RFC793_H

#include "tcp-socket-base.h"

namespace ns3 {

/**
 * \ingroup socket
 * \ingroup tcp
 *
 * \brief An implementation of a stream socket using TCP.
 *
 * This class contains an RFC793 implementation of TCP, as well as a sockets
 * interface for talking to TCP.  This serves as a base for other TCP functions
 * where the sliding window mechanism is handled here.  This class provides
 * connection orientation and sliding window flow control.
 */
class TcpRfc793 : public TcpSocketBase
{
public:
  static TypeId GetTypeId (void);
  /**
   * Create an unbound tcp socket.
   */
  TcpRfc793 (void);
  TcpRfc793 (const TcpRfc793& sock);
  virtual ~TcpRfc793 (void);

protected:
  virtual Ptr<TcpSocketBase> Fork (); // Call CopyObject<TcpRfc793> to clone me
  virtual void DupAck (const TcpHeader& t, uint32_t count);
  virtual void     SetSSThresh (uint32_t threshold);
  virtual uint32_t GetSSThresh (void) const;
  virtual void     SetInitialCwnd (uint32_t cwnd);
  virtual uint32_t GetInitialCwnd (void) const;
};

} // namespace ns3

#endif /* TCP_RFC793_H */
