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

#ifndef TCP_NEWRENO_H
#define TCP_NEWRENO_H

#include "tcp-socket-base.h"

namespace ns3 {

/**
 * \ingroup socket
 * \ingroup tcp
 *
 * \brief An implementation of a stream socket using TCP.
 *
 * This class contains the NewReno implementation of TCP, as of \RFC{2582}.
 */
class TcpNewReno : public TcpSocketBase
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  /**
   * Create an unbound tcp socket.
   */
  TcpNewReno (void);
  /**
   * \brief Copy constructor
   * \param sock the object to copy
   */
  TcpNewReno (const TcpNewReno& sock);
  virtual ~TcpNewReno (void);

protected:
  virtual Ptr<TcpSocketBase> Fork (void); // Call CopyObject<TcpNewReno> to clone me
  virtual void NewAck (SequenceNumber32 const& seq); // Inc cwnd and call NewAck() of parent
  virtual uint32_t GetSsThresh ();

private:
  uint32_t SlowStart (uint32_t segmentsAcked);
  void CongestionAvoidance (uint32_t segmentsAcked);
};

} // namespace ns3

#endif /* TCP_NEWRENO_H */
