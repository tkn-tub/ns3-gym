/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2006 Georgia Tech Research Corporation
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: George F. Riley<riley@ece.gatech.edu>
//

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "ns3/callback.h"
#include "ns3/ptr.h"
#include "ipv4-address.h"
#include "ns3/object.h"
#include <stdint.h>

namespace ns3 {

class Node;

/**
 * \brief Define a Socket API based on the BSD Socket API.
 *
 * Contrary to the original BSD socket API, this API is asynchronous:
 * it does not contain blocking calls. This API also does not use
 * the dreaded BSD sockaddr_t type. Other than that, it tries to stick
 * to the BSD API to make it easier those who know the BSD API to use
 * this API.
 */
class Socket : public Object
{
public:
  virtual ~Socket();

  enum SocketErrno {
    ENOTERROR,
    EISCONN,
    ENOTCONN,
    EMSGSIZE,
    EAGAIN,
    ESHUTDOWN,
    EOPNOTSUPP,
    SOCKET_ERRNO_LAST
  };

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
   * Allocate a free port number and
   * bind this socket to this port number on all
   * interfaces of this system.
   *
   * \returns 0 on success, -1 on failure.
   */
  virtual int Bind (void) = 0;

  /** 
   * Allocate a free port number and
   * bind this socket to this port number on the
   * specified interface.
   *
   * \param address address of interface to bind to.
   * \returns 0 on success, -1 on failure.
   */
  virtual int Bind (Ipv4Address address) = 0;

  /**
   * Bind this socket to this port number
   * on all interfaces of this system.
   *
   * \param port port to bind to on all interfaces
   * \returns 0 on success, -1 on failure.
   */
  virtual int Bind (uint16_t port) = 0; 

  /**
   * Bind this socket to this port number
   * on the interface specified by address.
   *
   * \param address address of interface to bind to.
   * \param port port to bind to on specified interface
   * \returns 0 on success, -1 on failure.
   */
  virtual int Bind (Ipv4Address address, uint16_t port) = 0;

  /** 
   * \brief Close a socket.
   * \param closeCompleted Callback invoked when the close operation is
   *        completed.
   *
   * After the Close call, the socket is no longer valid, and cannot
   * safely be used for subsequent operations.
   */
  void Close(Callback<void, Ptr<Socket> > closeCompleted = MakeCallback (&Socket::DummyCallbackVoidSocket));

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
   * \param address IP Address of remote.
   * \param portNumber Port number of remote
   * \param connectionSucceeded this callback is invoked when the connection request
   *        initiated by the user is successfully completed. The callback is passed
   *        back a pointer to the same socket object.
   * \param connectionFailed this callback is invoked when the connection request
   *        initiated by the user is unsuccessfully completed. The callback is passed
   *        back a pointer to the same socket object. 
   * \param halfClose XXX When exactly is this callback invoked ? If it invoked when the
   *        other side closes the connection ? Or when I call Close ?
   */
  void Connect(const Ipv4Address & address,
               uint16_t portNumber,
               Callback<void, Ptr<Socket> > connectionSucceeded = MakeCallback(&Socket::DummyCallbackVoidSocket),
               Callback<void, Ptr<Socket> > connectionFailed = MakeCallback(&Socket::DummyCallbackVoidSocket),
               Callback<void, Ptr<Socket> > halfClose = MakeCallback(&Socket::DummyCallbackVoidSocket));
    
  /**
   * \brief Accept connection requests from remote hosts
   * \param connectionRequest Callback for connection request from peer. 
   *        This user callback is passed a pointer to this socket, the 
   *        ip address and the port number of the connection originator. 
   *        This callback must return true to accept the incoming connection,
   *        false otherwise. If the connection is accepted, the 
   *        "newConnectionCreated" callback will be invoked later to give access
   *        to the user to the socket created to match this new connection. If the
   *        user does not explicitely specify this callback, all incoming 
   *        connections will be refused.
   * \param newConnectionCreated Callback for new connection: when a new
   *        is accepted, it is created and the corresponding socket is passed
   *        back to the user through this callback. This user callback is passed
   *        a pointer to the new socket, and the ip address and port number
   *        of the connection originator.
   * \param closeRequested Callback for connection close request from peer.
   *        XXX: when is this callback invoked ?
   */
  int Accept(Callback<bool, Ptr<Socket>, const Ipv4Address&, uint16_t> connectionRequest = 
             MakeCallback(&Socket::RefuseAllConnections),
             Callback<void, Ptr<Socket>, const Ipv4Address&, uint16_t> newConnectionCreated = 
             MakeCallback (&Socket::DummyCallbackVoidSocketIpv4AddressUi16),
             Callback<void, Ptr<Socket> > closeRequested = MakeCallback (&Socket::DummyCallbackVoidSocket));

  /**
   * \brief Send data (or dummy data) to the remote host
   * \param buffer Data to send (nil if dummy data).
   * \param size Number of bytes to send.
   * \param dataSent Data sent callback.
   * \returns -1 in case of error or the number of bytes copied in the 
   *          internal buffer and accepted for transmission.
   */
  int Send (const uint8_t* buffer,
            uint32_t size,
            Callback<void, Ptr<Socket>, uint32_t> dataSent = MakeCallback (&Socket::DummyCallbackVoidSocketUi32));
  
  /**
   * \brief Send data to a specified peer.
   * \param address IP Address of remote host
   * \param port port number
   * \param buffer Data to send (nil if dummy data).
   * \param size Number of bytes to send.
   * \param dataSent Data sent callback.
   * \returns -1 in case of error or the number of bytes copied in the 
   *          internal buffer and accepted for transmission.
   */
  int SendTo(const Ipv4Address &address,
             uint16_t port,
             const uint8_t *buffer,
             uint32_t size,
             Callback<void, Ptr<Socket>, uint32_t> dataSent = MakeCallback (&Socket::DummyCallbackVoidSocketUi32));
  
  /**
   * \brief Receive data
   * \param receivedData Invoked whenever new data is received.
   *
   * If you wish to transport only dummy packets, this method is not a very
   * efficient way to receive these dummy packets: it will trigger a memory
   * allocation to hold the dummy memory into a buffer which can be passed
   * to the user. Instead, consider using the RecvDummy method.
   */
  void Recv(Callback<void, Ptr<Socket>, const uint8_t*, uint32_t,const Ipv4Address&, uint16_t> receivedData = 
            MakeCallback (&Socket::DummyCallbackVoidSocketBufferUi32Ipv4AddressUi16));
  
  /**
   * \brief Receive data
   * \param receivedData Invoked whenever new data is received.
   *
   * This method is included because it is vastly more efficient than the 
   * Recv method when you use dummy payload.
   */
  void RecvDummy(Callback<void, Ptr<Socket>, uint32_t,const Ipv4Address&, uint16_t> receivedData =
                 MakeCallback (&Socket::DummyCallbackVoidSocketUi32Ipv4AddressUi16));

private:
  virtual void DoClose(Callback<void, Ptr<Socket> > closeCompleted) = 0;
  virtual void DoConnect(const Ipv4Address & address,
                         uint16_t portNumber,
                         Callback<void, Ptr<Socket> > connectionSucceeded,
                         Callback<void, Ptr<Socket> > connectionFailed,
                         Callback<void, Ptr<Socket> > halfClose) = 0;
  virtual int DoAccept(Callback<bool, Ptr<Socket>, const Ipv4Address&, uint16_t> connectionRequest,
                       Callback<void, Ptr<Socket>, const Ipv4Address&, uint16_t> newConnectionCreated,
                       Callback<void, Ptr<Socket> > closeRequested) = 0;
  virtual int DoSend (const uint8_t* buffer,
                    uint32_t size,
                    Callback<void, Ptr<Socket>, uint32_t> dataSent) = 0;
  virtual int DoSendTo(const Ipv4Address &address,
                       uint16_t port,
                       const uint8_t *buffer,
                       uint32_t size,
                       Callback<void, Ptr<Socket>, uint32_t> dataSent) = 0;
  virtual void DoRecv(Callback<void, Ptr<Socket>, const uint8_t*, uint32_t,const Ipv4Address&, uint16_t> receive) = 0;
  virtual void DoRecvDummy(Callback<void, Ptr<Socket>, uint32_t,const Ipv4Address&, uint16_t>) = 0;


  static bool RefuseAllConnections (Ptr<Socket> socket, const Ipv4Address& address, uint16_t port);
  static void DummyCallbackVoidSocket (Ptr<Socket> socket);
  static void DummyCallbackVoidSocketUi32 (Ptr<Socket> socket, uint32_t);
  static void DummyCallbackVoidSocketUi32Ipv4AddressUi16 (Ptr<Socket> socket, uint32_t, const Ipv4Address &, uint16_t);
  static void DummyCallbackVoidSocketBufferUi32Ipv4AddressUi16 (Ptr<Socket> socket, const uint8_t *, uint32_t, 
                                                                const Ipv4Address &, uint16_t);
  static void DummyCallbackVoidSocketIpv4AddressUi16 (Ptr<Socket> socket, const Ipv4Address &, uint16_t);
};

} //namespace ns3

#endif /* SOCKET_H */


