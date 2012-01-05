/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007-2009 Strasbourg University
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
 * Author: Sebastien Vincent <vincent@clarinet.u-strasbg.fr>
 */

#ifndef IPV6_RAW_SOCKET_IMPL_H
#define IPV6_RAW_SOCKET_IMPL_H

#include <list>

#include "ns3/socket.h"
#include "ns3/ipv6-address.h"
#include "ns3/ipv6-header.h"

namespace ns3
{
 
class NetDevice;
class Node;

/**
 * \class Ipv6RawSocketImpl
 * \brief IPv6 raw socket.
 *
 * A RAW Socket typically is used to access specific IP layers not usually
 * available through L4 sockets, e.g., ICMP. The implementer should take
 * particular care to define the Ipv6RawSocketImpl Attributes, and in
 * particular the Protocol attribute.
 * Not setting it will result in a zero protocol at IP level (corresponding
 * to the HopByHop IPv6 Extension header, i.e., Ipv6ExtensionHopByHopHeader)
 * when sending data through the socket, which is probably not the intended
 * behavior.
 *
 * A correct example is (from src/applications/model/radvd.cc):
 * \code
   if (!m_socket)
     {
       TypeId tid = TypeId::LookupByName ("ns3::Ipv6RawSocketFactory");
       m_socket = Socket::CreateSocket (GetNode (), tid);

       NS_ASSERT (m_socket);

       m_socket->SetAttribute ("Protocol", UintegerValue(Ipv6Header::IPV6_ICMPV6));
       m_socket->SetRecvCallback (MakeCallback (&Radvd::HandleRead, this));
     }
 * \endcode
 *
 */
class Ipv6RawSocketImpl : public Socket
{
public:
  /**
   * \brief Get the type ID of this class.
   * \return type ID
   */
  static TypeId GetTypeId ();

  /**
   * \brief Constructor.
   */
  Ipv6RawSocketImpl ();

  /**
   * \brief Destructor.
   */
  virtual ~Ipv6RawSocketImpl ();

  /**
   * \brief Set the node.
   * \param node node to set
   */
  void SetNode (Ptr<Node> node);

  /**
   * \brief Get last error number.
   * \return error number
   */
  virtual enum Socket::SocketErrno GetErrno () const;

  /**
   * \brief Get socket type (NS3_SOCK_RAW)
   * \return socket type
   */
  virtual enum Socket::SocketType GetSocketType () const;

  /**
   * \brief Get node.
   * \return node associated with this raw socket.
   */
  virtual Ptr<Node> GetNode () const;

  /**
   * \brief Bind the socket to address.
   * \param address bind to this address
   * \return 0 if success, -1 otherwise
   */
  virtual int Bind (const Address& address);

  /**
   * \brief Bind socket.
   * \return 0 if success, -1 otherwise
   */
  virtual int Bind ();

  /**
   * \brief Get socket address.
   * \param address socket address if method success
   * \return 0 if success, -1 otherwise
   */
  virtual int GetSockName (Address& address) const;

  /**
   * \brief Close the socket.
   * \return 0 if success, -1 otherwise
   */
  virtual int Close ();

  /**
   * \brief Shutdown send capability.
   * \return 0 if success, -1 otherwise
   */
  virtual int ShutdownSend ();

  /**
   * \brief Shutdown receive capability.
   * \return 0 if success, -1 otherwise
   */
  virtual int ShutdownRecv ();

  /**
   * \brief Connect to address.
   * \param address address
   * \return 0 if success, -1 otherwise
   */
  virtual int Connect (const Address& address);

  /**
   * \brief Listen.
   * \return 0 if success, -1 otherwise
   */
  virtual int Listen ();

  /**
   * \brief Get TX size available.
   * \return TX size
   */
  virtual uint32_t GetTxAvailable () const;

  /**
   * \brief Get RX size available.
   * \return RX size
   */
  virtual uint32_t GetRxAvailable () const;

  /**
   * \brief Send a packet.
   * \param p packet to send
   * \param flags additionnal flags
   * \return 0 if success, -1 otherwise
   */
  virtual int Send (Ptr<Packet> p, uint32_t flags);

  /**
   * \brief Send a packet.
   * \param p packet to send
   * \param flags additionnal flags
   * \param toAddress destination address
   * \return 0 if success, -1 otherwise
   */
  virtual int SendTo (Ptr<Packet> p, uint32_t flags, const Address& toAddress);

  /**
   * \brief Receive packet.
   * \param maxSize maximum size
   * \param flags additionnal flags
   * \return packet received
   */
  virtual Ptr<Packet> Recv (uint32_t maxSize, uint32_t flags);

  /**
   * \brief Receive packet.
   * \param maxSize maximum size
   * \param flags additionnal flags
   * \param fromAddress source address
   * \return packet received
   */
  virtual Ptr<Packet> RecvFrom (uint32_t maxSize, uint32_t flags, Address& fromAddress);

  /**
   * \brief Set protocol field.
   * \param protocol protocol to set
   */
  void SetProtocol (uint16_t protocol);

  /**
   * \brief Forward up to receive method.
   * \param p packet
   * \param hdr IPv6 header
   * \param device device
   * \return true if forwarded, false otherwise
   */
  bool ForwardUp (Ptr<const Packet> p, Ipv6Header hdr, Ptr<NetDevice> device);

  virtual bool SetAllowBroadcast (bool allowBroadcast);
  virtual bool GetAllowBroadcast () const;

private:
  /**
   * \struct Data
   * \brief IPv6 raw data and additionnal information.
   */
  struct Data
  {
    Ptr<Packet> packet;   /**< Packet data */
    Ipv6Address fromIp;   /**< Source address */
    uint16_t fromProtocol;   /**< Protocol used */
  };

  /**
   * \brief Dispose object.
   */
  virtual void DoDispose ();

  /**
   * \brief Last error number.
   */
  enum Socket::SocketErrno m_err;

  /**
   * \brief Node.
   */
  Ptr<Node> m_node;

  /**
   * \brief Source address.
   */
  Ipv6Address m_src;

  /**
   * \brief Destination address.
   */
  Ipv6Address m_dst;

  /**
   * \brief Protocol.
   */
  uint16_t m_protocol;

  /**
   * \brief Packet waiting to be processed.
   */
  std::list<struct Data> m_data;

  /**
   * \brief Flag to shutdown send capability.
   */
  bool m_shutdownSend;

  /**
   * \brief Flag to shutdown receive capability.
   */
  bool m_shutdownRecv;

  /**
   * \brief ICMPv6 filter.
   */
  uint32_t m_icmpFilter;
};

} /* namespace ns3 */

#endif /* IPV6_RAW_SOCKET_IMPL_H */

