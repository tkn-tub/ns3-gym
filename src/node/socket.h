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

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "ns3/callback.h"
#include "ns3/ptr.h"
#include "ns3/tag.h"
#include "ns3/object.h"
#include "address.h"
#include <stdint.h>

namespace ns3 {


class Node;
class Packet;

/**
 * \brief Define a Socket API based on the BSD Socket API.
 *
 * In contrast to the original BSD socket API, this API is asynchronous:
 * it does not contain blocking calls. It also uses class ns3::Packet
 * as a fancy byte buffer, allowing data to be passed across the API
 * using an ns3::Packet instead of a raw data pointer.  Other than that, 
 * it tries to stick to the BSD API to make it easier for those who know 
 * the BSD API to use this API.
 */
class Socket : public Object
{
public:
  Socket (void);
  virtual ~Socket (void);

  enum SocketErrno {
    ERROR_NOTERROR,
    ERROR_ISCONN,
    ERROR_NOTCONN,
    ERROR_MSGSIZE,
    ERROR_AGAIN,
    ERROR_SHUTDOWN,
    ERROR_OPNOTSUPP,
    ERROR_AFNOSUPPORT,
    ERROR_INVAL,
    ERROR_BADF,
    ERROR_NOROUTETOHOST,
    SOCKET_ERRNO_LAST
  };

  /**
   * This method wraps the creation of sockets that is performed
   * by a socket factory on a given node based on a TypeId.
   * 
   * \return A smart pointer to a newly created socket.
   * 
   * \param node The node on which to create the socket
   * \param tid The TypeId of the socket to create
   */
  static Ptr<Socket> CreateSocket (Ptr<Node> node, TypeId tid);

  /**
   * \return the errno associated to the last call which failed in this
   *         socket. Each socket's errno is initialized to zero
   *         when the socket is created.
   */
  virtual enum Socket::SocketErrno GetErrno (void) const = 0;

  /**
   * \returns the node this socket is associated with.
   */
  virtual Ptr<Node> GetNode (void) const = 0;

  /**
   * \param closeCompleted Callback invoked when the close operation is
   *        completed.
   */
  void SetCloseCallback (Callback<void, Ptr<Socket> > closeCompleted);

  /**
   * \param connectionSucceeded this callback is invoked when the 
   *        connection request initiated by the user is successfully 
   *        completed. The callback is passed  back a pointer to 
   *        the same socket object.
   * \param connectionFailed this callback is invoked when the 
   *        connection request initiated by the user is unsuccessfully 
   *        completed. The callback is passed back a pointer to the 
   *        same socket object. 
   * \param halfClose XXX When exactly is this callback invoked? If 
   *        it invoked when the other side closes the connection ? 
   *        Or when I call Close ?
   */
  void SetConnectCallback (Callback<void, Ptr<Socket> > connectionSucceeded,
                           Callback<void,  Ptr<Socket> > connectionFailed,
                           Callback<void,  Ptr<Socket> > halfClose);
  /**
   * \brief Accept connection requests from remote hosts
   * \param connectionRequest Callback for connection request from peer. 
   *        This user callback is passed a pointer to this socket, the 
   *        ip address and the port number of the connection originator. 
   *        This callback must return true to accept the incoming connection,
   *        false otherwise. If the connection is accepted, the 
   *        "newConnectionCreated" callback will be invoked later to 
   *        give access to the user to the socket created to match 
   *        this new connection. If the user does not explicitly 
   *        specify this callback, all incoming  connections will be refused.
   * \param newConnectionCreated Callback for new connection: when a new
   *        is accepted, it is created and the corresponding socket is passed
   *        back to the user through this callback. This user callback is 
   *        passed a pointer to the new socket, and the ip address and 
   *        port number of the connection originator.
   * \param closeRequested Callback for connection close request from peer.
   *        XXX: when is this callback invoked ?
   */
  void SetAcceptCallback (Callback<bool, Ptr<Socket>, 
                            const Address &> connectionRequest,
                          Callback<void, Ptr<Socket>, 
                            const Address&> newConnectionCreated,
                          Callback<void, Ptr<Socket> > closeRequested);
  /**
   * \brief Notify application when a packet has been sent from transport 
   *        protocol (non-standard socket call)
   * \param dataSent Callback for the event that data is sent from the
   *        underlying transport protocol.  This callback is passed a
   *        pointer to the socket, and the number of bytes sent.
   * \returns whether or not this socket supports this callback.  Note 
   *        that this is a non-standard socket call.  Some socket 
   *        implementations in ns-3 may not support this call, so the
   *        user should check this return value to confirm that the
   *        callback is supported.
   */
  virtual bool SetDataSentCallback (Callback<void, Ptr<Socket>, uint32_t> dataSent);
  /**
   * \brief Notify application when space in transmit buffer is added
   *
   *        This callback is intended to notify a 
   *        socket that would have been blocked in a blocking socket model
   *        that space is available in the transmit buffer and that it
   *        can call Send() again.  
   *
   * \param sendCb Callback for the event that the socket transmit buffer
   *        fill level has decreased.  This callback is passed a pointer to
   *        the socket, and the number of bytes available for writing
   *        into the buffer (an absolute value).  If there is no transmit
   *        buffer limit, a maximum-sized integer is always returned.
   */
  void SetSendCallback (Callback<void, Ptr<Socket>, uint32_t> sendCb);
  /**
   * \brief Notify application when new data is available to be read.
   *
   *        This callback is intended to notify a socket that would
   *        have been blocked in a blocking socket model that data
   *        is available to be read.
   */
  void SetRecvCallback (Callback<void, Ptr<Socket> >);
  /** 
   * \param address the address to try to allocate
   * \returns 0 on success, -1 on failure.
   *
   * Allocate a local endpoint for this socket.
   */
  virtual int Bind (const Address &address) = 0;

  /** 
   * Allocate a local endpoint for this socket.
   *
   * \returns 0 on success, -1 on failure.
   */
  virtual int Bind () = 0;

  /** 
   * \brief Close a socket.
   *
   * After the Close call, the socket is no longer valid, and cannot
   * safely be used for subsequent operations.
   */
  virtual int Close (void) = 0;

  /**
   * \returns zero on success, -1 on failure.
   *
   * Do not allow any further Send calls. This method is typically
   * implemented for Tcp sockets by a half close.
   */
  virtual int ShutdownSend (void) = 0;

  /**
   * \returns zero on success, -1 on failure.
   *
   * Do not allow any further Recv calls. This method is typically
   * implemented for Tcp sockets by a half close.
   */
  virtual int ShutdownRecv (void) = 0;

  /**
   * \brief Initiate a connection to a remote host
   * \param address Address of remote.
   */
  virtual int Connect (const Address &address) = 0;
    
  /**
   * \brief Listen for incoming connections.
   * \param queueLimit maximum number of incoming request to queue
   * \returns XXX an error code
   */
  virtual int Listen (uint32_t queueLimit);

  /**
   * \brief Send data (or dummy data) to the remote host
   *
   * This function matches closely in semantics to the send() function
   * call in the standard C library (libc):
   *   ssize_t send (int s, const void *msg, size_t len, int flags);
   * except that the function call is asynchronous.
   * 
   * In a typical blocking sockets model, this call would block upon
   * lack of space to hold the message to be sent.  In ns-3 at this
   * API, the call returns immediately in such a case, but the callback
   * registered with SetSendCallback() is invoked when the socket
   * has space (when it conceptually unblocks); this is an asynchronous
   * I/O model for send().
   * 
   * This variant of Send() uses class ns3::Packet to encapsulate
   * data, rather than providing a raw pointer and length field.  
   * This allows an ns-3 application to attach tags if desired (such
   * as a flow ID) and may allow the simulator to avoid some data
   * copies.  Despite the appearance of sending Packets on a stream
   * socket, just think of it as a fancy byte buffer with streaming
   * semantics.    
   *
   * If either the message buffer within the Packet is too long to pass 
   * atomically through the underlying protocol (for datagram sockets), 
   * or the message buffer cannot entirely fit in the transmit buffer
   * (for stream sockets), -1 is returned and SocketErrno is set 
   * to ERROR_MSGSIZE.  If the packet does not fit, the caller can
   * split the Packet (based on information obtained from 
   * GetTxAvailable) and reattempt to send the data.
   *
   * \param p ns3::Packet to send
   * \returns the number of bytes accepted for transmission if no error
   *          occurs, and -1 otherwise.
   */
  virtual int Send (Ptr<Packet> p) = 0;
  
  /**
   * \brief Returns the number of bytes which can be sent in a single call
   * to Send. 
   * 
   * For datagram sockets, this returns the number of bytes that
   * can be passed atomically through the underlying protocol.
   *
   * For stream sockets, this returns the available space in bytes
   * left in the transmit buffer.
   */
  virtual uint32_t GetTxAvailable (void) const = 0;

  /**
   * \brief Send data (or dummy data) to the remote host
   * \param buf A pointer to a raw byte buffer of some data to send.  If this 
   * is 0, we send dummy data whose size is specified by the second parameter
   * \param size the number of bytes to copy from the buffer
   * 
   * This is provided so as to have an API which is closer in appearance 
   * to that of real network or BSD sockets.  
   */
  int Send (const uint8_t* buf, uint32_t size);
  
  /**
   * \brief Send data to a specified peer.
   * \param p packet to send
   * \param address IP Address of remote host
   * \returns -1 in case of error or the number of bytes copied in the 
   *          internal buffer and accepted for transmission.
   */
  virtual int SendTo (Ptr<Packet> p, const Address &address) = 0;

  /**
   * \brief Send data to a specified peer.
   * \param buf A pointer to a raw byte buffer of some data to send.  If this 
   * is 0, we send dummy data whose size is specified by the third parameter
   * \param size the number of bytes to copy from the buffer
   * \param address IP Address of remote host
   * \returns -1 in case of error or the number of bytes copied in the 
   *          internal buffer and accepted for transmission.
   *
   * This is provided so as to have an API which is closer in appearance 
   * to that of real network or BSD sockets.
   */
  int SendTo (const uint8_t* buf, uint32_t size, const Address &address); 

  /**
   * \brief Read a single packet from the socket
   * \param maxSize reader will accept packet up to maxSize
   * \param flags Socket recv flags
   * \returns Ptr<Packet> of the next in-sequence packet.  Returns
   * 0 if the socket cannot return a next in-sequence packet conforming
   * to the maxSize and flags.
   */
  virtual Ptr<Packet> Recv (uint32_t maxSize, uint32_t flags) = 0;
  /**
   * \brief Read a single packet from the socket
   *
   *      Overloaded version of Recv(maxSize, flags) with maxSize
   *      implicitly set to maximum sized integer, and flags set to zero.
   *
   * \returns Ptr<Packet> of the next in-sequence packet.  Returns
   * 0 if the socket cannot return a next in-sequence packet.
   */
   Ptr<Packet> Recv (void);
  /**
   * Return number of bytes which can be returned from one or 
   * multiple calls to Recv.
   * Must be possible to call this method from the Recv callback.
   */
  virtual uint32_t GetRxAvailable (void) const = 0;
 
protected:
  void NotifyCloseCompleted (void);
  void NotifyConnectionSucceeded (void);
  void NotifyConnectionFailed (void);
  void NotifyHalfClose (void);
  bool NotifyConnectionRequest (const Address &from);
  void NotifyNewConnectionCreated (Ptr<Socket> socket, const Address &from);
  void NotifyCloseRequested (void);
  void NotifyDataSent (uint32_t size);
  void NotifySend (uint32_t spaceAvailable);
  void NotifyDataRecv (void);

  Callback<void,Ptr<Socket> >    m_closeCompleted;
  Callback<void, Ptr<Socket> >   m_connectionSucceeded;
  Callback<void, Ptr<Socket> >   m_connectionFailed;
  Callback<void, Ptr<Socket> >   m_halfClose;
  Callback<void, Ptr<Socket> >   m_closeRequested;
  Callback<bool, Ptr<Socket>, const Address &>   m_connectionRequest;
  Callback<void, Ptr<Socket>, const Address&>    m_newConnectionCreated;
  Callback<void, Ptr<Socket>, uint32_t>          m_dataSent;
  Callback<void, Ptr<Socket>, uint32_t >         m_sendCb;
  Callback<void, Ptr<Socket> > m_receivedData;

};

/**
 * \brief This class implements a tag that carries the source address
 * of a packet across the receiving socket interface.
 */
class SocketRxAddressTag : public Tag
{
public:
  SocketRxAddressTag ();
  static uint32_t GetUid (void);
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator i) const;
  uint32_t Deserialize (Buffer::Iterator i);

  void SetAddress (Address addr);
  Address GetAddress (void) const;
private:
  Address m_address;
};

/**
 * \brief This class implements a tag that carries the socket-specific
 * TTL of a packet to the IP layer
 */
class SocketIpTtlTag : public Tag
{
public:
  SocketIpTtlTag ();
  static uint32_t GetUid (void);
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator i) const;
  uint32_t Deserialize (Buffer::Iterator i);

  void SetTtl (uint8_t ttl);
  uint8_t GetTtl (void) const;
private:
  uint8_t m_ttl;
};

} //namespace ns3

#endif /* SOCKET_H */


