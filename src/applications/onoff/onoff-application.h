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

// ns3 - On/Off Data Source Application class
// George F. Riley, Georgia Tech, Spring 2007
// Adapted from ApplicationOnOff in GTNetS.

#ifndef __onoff_application_h__
#define __onoff_application_h__

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/data-rate.h"
#include "ns3/callback-trace-source.h"
#include "ns3/random-variable.h"

namespace ns3 {

class Address;
class RandomVariable;
class Socket;

/**
 * \brief Generate traffic to a single destination according to an
 *        OnOff pattern.
 *
 * This traffic follows an On/Off pattern: after Application::StartApplication
 * is called, "On" and "Off" states alternate. The duration of each of
 * these states is determined with the onTime and the offTime random
 * variables. During the "Off" state, no traffic is generated.
 * During the "On" state, cbr traffic is generated. This cbr traffic is
 * characterized by the specified "data rate" and "packet size".
 */
class OnOffApplication : public Application 
{
public:
  /**
   * \param n node associated to this application
   * \param remote remote ip address
   * \param tid
   * \param ontime on time random variable
   * \param offtime off time random variable
   */
  OnOffApplication(Ptr<Node> n,
                   const Address &remote,
                   std::string tid,
                   const RandomVariable& ontime,
                   const RandomVariable& offtime);

  /**
   * \param n node associated to this application
   * \param remote remote ip address
   * \param tid
   * \param ontime on time random variable
   * \param offtime off time random variable
   * \param rate data rate when on
   * \param size size of packets when sending data.
   */
  OnOffApplication(Ptr<Node> n,
                   const Address &remote,
                   std::string tid,
                   const RandomVariable& ontime,
                   const RandomVariable& offtime,
                   DataRate  rate,
                   uint32_t size);

  virtual ~OnOffApplication();

  void SetMaxBytes(uint32_t maxBytes);

  /**
   * \param r the data rate
   *
   * Set the data rate to use for every OnOffApplication for which
   * the user does not specify an explicit data rate.
   */
  static void SetDefaultRate(const DataRate & r);

  /**
   * \param size the packet size
   *
   * Set the packet size to use for every OnOffApplication for
   * which the user does not specify an explicit packet size.
   */
  static void SetDefaultSize (uint32_t size);

protected:
  virtual void DoDispose (void);
private:
  // inherited from Application base class.
  virtual void StartApplication (void);    // Called at time specified by Start
  virtual void StopApplication (void);     // Called at time specified by Stop

  void Construct (Ptr<Node> n,
                  const Address &remote,
                  std::string tid,
                  const RandomVariable& ontime,
                  const RandomVariable& offtime,
                  uint32_t size);


  // Event handlers
  void StartSending();
  void StopSending();
  void SendPacket();

  Ptr<Socket>     m_socket;       // Associated socket
  Address         m_peer;         // Peer address
  bool            m_connected;    // True if connected
  RandomVariable m_onTime;       // rng for On Time
  RandomVariable m_offTime;      // rng for Off Time
  DataRate        m_cbrRate;      // Rate that data is generated
  uint32_t        m_pktSize;      // Size of packets
  uint32_t        m_residualBits; // Number of generated, but not sent, bits
  Time            m_lastStartTime;// Time last packet sent
  uint32_t        m_maxBytes;     // Limit total number of bytes sent
  uint32_t        m_totBytes;     // Total bytes sent so far
  EventId         m_startStopEvent;     // Event id for next start or stop event
  EventId         m_sendEvent;    // Eventid of pending "send packet" event
  bool            m_sending;      // True if currently in sending state
  std::string     m_tid;
  CallbackTraceSource<Ptr<const Packet> > m_txTrace;
  
private:
  virtual Ptr<TraceResolver> GetTraceResolver (void) const;
  void ScheduleNextTx();
  void ScheduleStartEvent();
  void ScheduleStopEvent();
  void ConnectionSucceeded(Ptr<Socket>);
  void ConnectionFailed(Ptr<Socket>);
  void Ignore(Ptr<Socket>);
};

} // namespace ns3

#endif

