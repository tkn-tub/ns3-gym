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

/**
 * \ingroup tcp
 * \brief Names of the 11 TCP states
 *
 * \todo This should be a member of TcpSocket.
 */
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
 * \ingroup tcp
 * TracedValue Callback signature for TcpStates_t
 *
 * \param [in] oldValue original value of the traced variable
 * \param [in] newValue new value of the traced variable
 */
typedef void (* TcpStatesTracedValueCallback)(const TcpStates_t oldValue,
                                              const TcpStates_t newValue);

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
  /**
   * Get the type ID.
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
 
  TcpSocket (void);
  virtual ~TcpSocket (void);

  /**
   * \brief Literal names of TCP states for use in log messages
   */
  static const char* const TcpStateName[LAST_STATE];

private:
  // Indirect the attribute setting and getting through private virtual methods

  /**
   * \brief Set the send buffer size.
   * \param size the buffer size (in bytes)
   */
  virtual void SetSndBufSize (uint32_t size) = 0;

  /**
   * \brief Get the send buffer size.
   * \returns the buffer size (in bytes)
   */
  virtual uint32_t GetSndBufSize (void) const = 0;

  /**
   * \brief Set the receive buffer size.
   * \param size the buffer size (in bytes)
   */
  virtual void SetRcvBufSize (uint32_t size) = 0;

  /**
   * \brief Get the receive buffer size.
   * \returns the buffer size (in bytes)
   */
  virtual uint32_t GetRcvBufSize (void) const = 0;

  /**
   * \brief Set the segment size.
   * \param size the segment size (in bytes)
   */
  virtual void SetSegSize (uint32_t size) = 0;

  /**
   * \brief Get the segment size.
   * \returns the segment size (in bytes)
   */
  virtual uint32_t GetSegSize (void) const = 0;

  /**
   * \brief Set the initial Slow Start Threshold.
   * \param threshold the Slow Start Threshold (in bytes)
   */
  virtual void SetInitialSSThresh (uint32_t threshold) = 0;

  /**
   * \brief Get the initial Slow Start Threshold.
   * \returns the Slow Start Threshold (in bytes)
   */
  virtual uint32_t GetInitialSSThresh (void) const = 0;

  /**
   * \brief Set the initial Congestion Window.
   * \param cwnd the initial congestion window (in bytes)
   */
  virtual void SetInitialCwnd (uint32_t cwnd) = 0;

  /**
   * \brief Get the initial Congestion Window.
   * \returns the initial congestion window (in bytes)
   */
  virtual uint32_t GetInitialCwnd (void) const = 0;

  /**
   * \brief Set the connection timeout.
   * \param timeout the connection timeout
   */
  virtual void SetConnTimeout (Time timeout) = 0;

  /**
   * \brief Get the connection timeout.
   * \returns the connection timeout
   */
  virtual Time GetConnTimeout (void) const = 0;

  /**
   * \brief Set the number of connection retries before giving up.
   * \param count the number of connection retries
   */
  virtual void SetConnCount (uint32_t count) = 0;

  /**
   * \brief Get the number of connection retries before giving up.
   * \returns the number of connection retries
   */
  virtual uint32_t GetConnCount (void) const = 0;

  /**
   * \brief Set the time to delay an ACK.
   * \param timeout the time to delay an ACK
   */
  virtual void SetDelAckTimeout (Time timeout) = 0;

  /**
   * \brief Get the time to delay an ACK.
   * \returns the time to delay an ACK
   */
  virtual Time GetDelAckTimeout (void) const = 0;

  /**
   * \brief Set the number of packet to fire an ACK before delay timeout.
   * \param count the umber of packet to fire an ACK before delay timeout
   */
  virtual void SetDelAckMaxCount (uint32_t count) = 0;

  /**
   * \brief Get the number of packet to fire an ACK before delay timeout.
   * \returns the number of packet to fire an ACK before delay timeout
   */
  virtual uint32_t GetDelAckMaxCount (void) const = 0;

  /**
   * \brief Enable/Disable Nagle's algorithm.
   * \param noDelay true to DISABLE Nagle's algorithm
   */
  virtual void SetTcpNoDelay (bool noDelay) = 0;

  /**
   * \brief Check if Nagle's algorithm is enabled or not.
   * \returns true if Nagle's algorithm is DISABLED
   */
  virtual bool GetTcpNoDelay (void) const = 0;

  /**
   * \brief Set the timout for persistent connection
   *
   * When the timout expires, send 1-byte data to probe for the window
   * size at the receiver when the local knowledge tells that the
   * receiver has zero window size
   *
   * \param timeout the persistent timout
   */
  virtual void SetPersistTimeout (Time timeout) = 0;

  /**
   * \brief Get the timout for persistent connection
   *
   * When the timout expires, send 1-byte data to probe for the window
   * size at the receiver when the local knowledge tells that the
   * receiver has zero window size
   *
   * \returns the persistent timout
   */
  virtual Time GetPersistTimeout (void) const = 0;

};

} // namespace ns3

#endif /* TCP_SOCKET_H */


