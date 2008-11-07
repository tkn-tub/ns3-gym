/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 Louis Pasteur University
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
 * Authors: Sebastien Vincent <vincent@clarinet.u-strasbg.fr>
 */

#ifndef ICMP_SOCKET_H
#define ICMP_SOCKET_H

#include "socket.h"
#include "ns3/traced-callback.h"
#include "ns3/callback.h"
#include "ns3/ptr.h"
#include "ns3/object.h"

namespace ns3 {

class Node;
class Packet;

/**
 * \brief (abstract) base class of all IcmpSockets (for IPv4 or IPv6).
 *
 * This class exists solely for hosting IcmpSocket attributes that can
 * be reused across different implementations.
 */
class IcmpSocket : public Socket
{
  public:
    /**
     * \brief Get the UID of this class.
     * \return UID
     */
    static TypeId GetTypeId (void);

    /**
     * \brief Constructor.
     */
    IcmpSocket (void);

    /**
     * \brief Destructor.
     */
    virtual ~IcmpSocket (void);

    /**
     * \brief Get the error.
     * \return the error.
     */
    virtual enum Socket::SocketErrno GetErrno (void) const = 0;

    /**
     * \brief Get the node.
     * \return the node
     */
    virtual Ptr<Node> GetNode (void) const = 0;

    /**
     * \brief Bind the socket.
     * \return 0 if OK, -1 otherwise
     */
    virtual int Bind (void) = 0;

    /**
     * \brief Bind the socket on "addr".
     * \param addr address
     * \return 0 if OK, -1 otherwise
     */
    virtual int Bind (const Address &addr) = 0;

    /**
     * \brief Close the socket.
     * \return 0 if OK, -1 otherwise
     */
    virtual int Close (void) = 0;

    /**
     * \brief Shutdown the socket on send.
     * \return 0 if OK, -1 otherwise
     */
    virtual int ShutdownSend (void) = 0;

    /**
     * \brief Shutdown the socket on receive.
     * \return 0 if OK, -1 otherwise
     */
    virtual int ShutdownRecv (void) = 0;

    /**
     * \brief Connect to another node.
     * \param addr address
     * \return 0 if OK, -1 otherwise
     */
    virtual int Connect (const Address &addr) = 0;

    /**
     * \brief Send a packet.
     * \param p the packet to send
     * \param flags flags
     * \return 0 if OK, -1 otherwise
     */		
    virtual int Send (Ptr<Packet> p, uint32_t flags) = 0;

    /**
     * \brief Get the maximum message size available.
     * \return maximum message size
     * \warning size of a message that could be sent is limited by the link MTU.
     */
    virtual uint32_t GetTxAvailable (void) const = 0;

    /**
     * \brief Send a packet to a node.
     * \param addr the address of the node
     * \param flags flags
     * \param p the packet to send
     * \return 0 if OK, -1 otherwise
     */
    virtual int SendTo (Ptr<Packet> p, uint32_t flags, const Address &addr) = 0;

    /**
     * \brief Receive method.
     * \param maxSize maximum size we want to return
     * \param flags flags
     * \return a packet with at maximum maxSize size
     */
    virtual Ptr<Packet> Recv (uint32_t maxSize, uint32_t flags) = 0;

    /**
     * \brief Receive method.
     * \param maxSize maximum size we want to return
     * \param flags flags
     * \param fromAddress sender address
     * \return a packet with at maximum maxSize size
     */
    virtual Ptr<Packet> RecvFrom (uint32_t maxSize, uint32_t flags, Address &fromAddress) = 0;

    /**
     * \brief Get the size we could receive.
     * \return size we could receive at one moment
     */
    virtual uint32_t GetRxAvailable (void) const = 0;

  private:
    /**
     * \brief Get the receive buffer size.
     * \return receive buffer size
     */
    virtual uint32_t GetRcvBufSize (void) const = 0;

    /**
     * \brief Set the receive buffer size.
     * \param rcvBufSize size to set
     */
    virtual void SetRcvBufSize (uint32_t rcvBufSize) = 0;

    /* FIXME : add ICMP basic attribute for socket */
    /* Indirect the attribute setting and getting through private virtual methods */
};

} /* namespace ns3 */

#endif /* ICMP_SOCKET_H */

