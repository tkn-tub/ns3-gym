/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008,2009 INRIA, UDCAST
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
 * Author: Amine Ismail <amine.ismail@sophia.inria.fr>
 *                      <amine.ismail@udcast.com>
 *
 */

#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/address.h"
#include "packet-loss-counter.h"
namespace ns3 {
/**
 * \ingroup applications
 * \defgroup udpclientserver UdpClientServer
 */

/**
 * \ingroup udpclientserver
 * \class UdpServer
 * \brief A Udp server. Receives UDP packets from a remote host. UDP packets
 * carry a 32bits sequence number followed by a 64bits time stamp in their
 * payloads. The application uses, the sequence number to determine if a packet
 * is lost, and the time stamp to compute the delay
 */
class UdpServer : public Application
{
public:
  static TypeId GetTypeId (void);
  UdpServer ();
  virtual ~UdpServer ();
  /**
   * returns the number of lost packets
   * \return the number of lost packets
   */
  uint32_t GetLost (void) const;

  /**
   * \brief returns the number of received packets
   * \return the number of received packets
   */
  uint32_t GetReceived (void) const;

  /**
   * \return the size of the window used for checking loss.
   */
  uint16_t GetPacketWindowSize () const;

  /**
   * \brief Set the size of the window used for checking loss. This value should
   *  be a multiple of 8
   * \param size the size of the window used for checking loss. This value should
   *  be a multiple of 8
   */
  void SetPacketWindowSize (uint16_t size);
protected:
  virtual void DoDispose (void);

private:

  virtual void StartApplication (void);
  virtual void StopApplication (void);

  void HandleRead (Ptr<Socket> socket);

  uint16_t m_port;
  Ptr<Socket> m_socket;
  Ptr<Socket> m_socket6;
  Address m_local;
  uint32_t m_received;
  PacketLossCounter m_lossCounter;
};

} // namespace ns3

#endif /* UDP_SERVER_H */
