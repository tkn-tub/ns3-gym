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
 *
 * Author:  Tom Henderson (tomhend@u.washington.edu)
 */

#ifndef __packet_sink_h__
#define __packet_sink_h__

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/callback-trace-source.h"
#include "ns3/address.h"

namespace ns3 {

class Address;
class Socket;
class Packet;

/**
 * \brief Receive and consume traffic generated to an IP address and port
 *
 * This application was written to complement OnOffApplication, but it
 * is more general so a PacketSink name was selected.  Functionally it is
 * important to use in multicast situations, so that reception of the layer-2
 * multicast frames of interest are enabled, but it is also useful for
 * unicast as an example of how you can write something simple to receive
 * packets at the application layer.  Also, if an IP stack generates 
 * ICMP Port Unreachable errors, receiving applications will be needed.
 *
 * The constructor specifies the Address (IP address and port) and the 
 * transport protocol to use.   A virtual Receive () method is installed 
 * as a callback on the receiving socket.  By default, when logging is
 * enabled, it prints out the size of packets and their address, but
 * we intend to also add a tracing source to Receive() at a later date.
 */
class PacketSink : public Application 
{
public:
  /**
   * \param n node associated to this application
   * \param local local address to bind to
   * \param tid string to identify transport protocol of interest
   */
  PacketSink (Ptr<Node> n,
              const Address &local,
              std::string tid);

  virtual ~PacketSink ();

protected:
  virtual void DoDispose (void);
private:
  // inherited from Application base class.
  virtual void StartApplication (void);    // Called at time specified by Start
  virtual void StopApplication (void);     // Called at time specified by Stop
  // inherited from Object base class.
  virtual Ptr<TraceResolver> GetTraceResolver (void) const;

  void Construct (Ptr<Node> n,
                  const Address &local,
                  std::string tid);

  virtual void Receive (Ptr<Socket> socket, Ptr<Packet> packet, const Address& from);
  virtual void CloseConnection (Ptr<Socket> socket);
  virtual bool AcceptConnectionRequest (Ptr<Socket> socket, const Address &from);

  Ptr<Socket>     m_socket;       // Associated socket
  Address         m_local;        // Local address to bind to
  std::string     m_tid;          // Protocol name (e.g., "Udp")
  CallbackTraceSource<Ptr<const Packet>, const Address &> m_rxTrace;
  
};

} // namespace ns3

#endif

