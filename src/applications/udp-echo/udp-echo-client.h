/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2007 University of Washington
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
 */

#ifndef __UDP_ECHO_CLIENT_H__
#define __UDP_ECHO_CLIENT_H__

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"

namespace ns3 {

class Address;
class Socket;
class Packet;

class UdpEchoClient : public Application 
{
public:
  UdpEchoClient (Ptr<Node> n, Ipv4Address serverAddr, uint16_t serverPort,
    uint32_t count, Time interval, uint32_t size);

  virtual ~UdpEchoClient ();

protected:
  virtual void DoDispose (void);

private:
  void Construct (Ptr<Node> n, Ipv4Address serverAddr, uint16_t serverPort,
    uint32_t count, Time interval, uint32_t size);

  virtual void StartApplication (void);
  virtual void StopApplication (void);

  void ScheduleTransmit (Time dt);
  void Send (void);

  Ptr<Node> m_node;
  Ipv4Address m_serverAddress;
  uint16_t m_serverPort;
  uint32_t m_count;
  Time m_interval;
  uint32_t m_size;

  uint32_t m_sent;
  Ptr<Socket> m_socket;
  Address m_peer;
  EventId m_sendEvent;

};

} // namespace ns3

#endif // __UDP_ECHO_CLIENT_H__

