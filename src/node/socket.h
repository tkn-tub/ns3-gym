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

// Define the Sockets API for ns3.
// George F. Riley, Georgia Tech, Fall 2006

// The Socket class defines an API based on the well-known
// BSD sockets.  However, in a simulation environment, it is difficult
// to implement the semantics "blocking" calls, such as recv.
// The ns3 API will always return immediately from the blocking calls,
// with an upcall at a later time when the call would unblock.
// All blocking calls have a parameters with callbacks
// to the desired upcall.  However, the design supports the upcall
// even if the owner of the socket has not posted the corresponding
// blocking call, which eases implementation slightly.
//
// The ns3 class "Process"is anticipated to be a common way to create
// and manipulate sockets, so a set of member functions are provided
// in class Process to give a syntactically easy way to access the
// sockets with default upcall virtual functions for each of the
// defined blocking calls.  See the definition and implementation
// Process in process.h for details.
//
// As designed, the Socket class is a virtual base class, which defines
// the services that each layer 4 protocol must provide.

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "ns3/callback.h"
#include "protocol.h"

namespace ns3{

class IPAddr;
class L4Protocol;
class Process;

// \brief Define the BSD Sockets API
class Socket {
public:
  Socket();  // Constructor
  virtual ~Socket() {}
private:
  // These static methods are default, do-nothing callbacks.  They are
  // provided as default arguments to the various socket calls, so callers who
  // are not interested in a particular callback can ignore the
  // parameters.  Since these are used as formal parameters to MakeCallbac,
  // each must have a unique name (they can't be disambiguated by argument
  // list).  The naming convention is a single letter indicating the
  // return type (V = void for example) and a single letter for each
  // argument type (S = Socket for exampls).
  static void CBIgnoreVS(Socket*) {}
  static void CBIgnoreVSIP(Socket*, const IPAddr&, PortId_t) {}
  static void CBIgnoreVSU(Socket*, uint32_t) {}
  static void CBIgnoreVSCU(Socket*, char*, uint32_t) {}
  static bool CBIgnoreBSIP(Socket*, const IPAddr&, PortId_t){ return true;}
  static void CBIgnoreVSCUIP(Socket*, char*, uint32_t, const IPAddr&, PortId_t);
  
public:
  // Define the BSD Socket API, with some slight variations.

  // \brief Bind the socket to a particular port number
  // \param Port number to bind. If zero is specified, an available
  // transient port is assigned.
  // \return Port number assigned, or -1 if port already in use.

  virtual PortId_t Bind(PortId_t = 0);

  // \brief Close a socket.
  // After the Close call, the socket is no longer valid, and cannot
  // safely be used for subsequent operations.
  // \param Callback for close completed.
  virtual void Close(ns3::Callback<void, Socket*>
                     = MakeCallback(&Socket::CBIgnoreVS));

  // \brief Initiate a connection to a remote host
  // \param IP Address of remote.
  // \param Port number of remote
  // \param Callback for connection succeeded
  // \param Callback for connection failed
  // \param Callback for connection close requested by peer
  virtual void Connect(const IPAddr&,          // IPAddress of peer
               PortId_t,                       // Port number
               ns3::Callback<void, Socket*> =  // COnnection succeeded callback,
                       MakeCallback(&Socket::CBIgnoreVS),
               ns3::Callback<void, Socket*> =  // Connection failed callback
                       MakeCallback(&Socket::CBIgnoreVS),
               ns3::Callback<void, Socket*> =  // CloseRequested callback
                       MakeCallback(&Socket::CBIgnoreVS));
  
  // \brief Specify the number of pending connections to buffer
  // while not blocked on accept.  We include this for completeness,
  // but with the callback approach to connection acceptance this
  // does nothing.
  virtual void Listen(uint32_t);
  
  // \brief Accept connection requests from remote hosts
  // \param Callback for connection request from peer
  // \param Callback for new connection
  // \param Callback for connection close request from peer
  virtual void Accept(
      // Connection request callback
      ns3::Callback<bool, Socket*, const IPAddr&, PortId_t> = 
           MakeCallback(&Socket::CBIgnoreBSIP),
      // New connection callback
      ns3::Callback<void, Socket*, const IPAddr&, PortId_t> = 
           MakeCallback(&Socket::CBIgnoreVSIP),
      // CloseRequested callback
      ns3::Callback<void, Socket*> = 
           MakeCallback(&Socket::CBIgnoreVS));

  // \brief Send data (or dummy data) to the remote host
  // \param Data to send (nil if dummy data).
  // NOTE.  The data (if any) is copied by this call, so pointers
  // to local data is allowed and processed correctly.
  // \param Number of bytes to send.
  // \param Data sent callback.  A suitable "ignore" callback
  // is used if not specified.
  virtual void Send(char*,                   // Data to send
                    uint32_t,                // Number of bytes
                    ns3::Callback<void, Socket*, uint32_t> = 
                         MakeCallback(&CBIgnoreVSU)); // Sent cb

  // \brief Send data to a specified peer.
  // \param IP Address of remote host
  // \param port number
  // \param Data to send (nil if dummy data).
  // NOTE.  The data (if any) is copied by this call, so pointers
  // to local data is allowed and processed correctly.
  // \param Number of bytes to send.
  // \param Data sent callback
  virtual void SendTo(const IPAddr&,           // IP Address of remote peer
                      PortId_t,                // Port number of remote peer
                      char*,                   // Data to send
                      uint32_t,                // Number of bytes
                      ns3::Callback<void, Socket*, uint32_t> = 
                      MakeCallback(&Socket::CBIgnoreVSU)); // Sent cb

  // \brief Receive data
  // Note that due to the callback nature of the ns3 implementation,
  // there is no data pointer provided here.  Rather, the callback
  // will specify where the data can be found, and the callback should
  // copy it to a local buffer if needed at that time.
  // \param Received data callback.
  virtual void Recv(ns3::Callback<void, Socket*, char*, uint32_t,
                    const IPAddr&, PortId_t> = 
                    MakeCallback(&Socket::CBIgnoreVSCUIP));

private:
// upcalls
// The members below are all callbacks, created with MakeCallback,
  ns3::Callback<void, Socket*> m_cbCloseComplete;      // Socket Closed upcall
  ns3::Callback<void, Socket*> m_cbCloseRequested;     // Close req by peer
  ns3::Callback<void, Socket*> m_cbConnectionSucceeded;// Connection successful
  ns3::Callback<void, Socket*> m_cbConnectionFailed;   // Connection failed

  // \brief For TCP sockets, the cbSent upcall call is made when the data packet
  // is sent, but not acknowledged.  For UDP sockets, it is called
  // immediately.
  ns3::Callback<void, Socket*, uint32_t> m_cbSent;       // Data delivered

  // \brief For the cbRecv callback, the char* to the data may be nil if
  // no actual data is present.
  ns3::Callback<void, Socket*, char*, uint32_t,
                const IPAddr&, PortId_t> m_cbRecv;

  // \brief The cbConnectionRequest callback is called when a socket has
  // called Listen, and indicates a remote peer has sent a connection
  // request.  The callback can return false to indicate the connection
  // is to be rejected, for example in a peer-to-peer application that
  // wants to limit the number of connections at any one time.
  ns3::Callback<bool, Socket*, const IPAddr&, PortId_t> m_cbConnectionRequest;

  // \brief The cbNewConnection callback is called when a connection request
  // from a peer has been received and accepted by the process owning
  // this socket.  Note the Socket* passed to the upcall is a new
  // socket.  The original socket using accept is still accepting connections.
  ns3::Callback<void, Socket*, const IPAddr&, PortId_t> m_cbNewConnection;

};

} //namespace ns3

#endif


