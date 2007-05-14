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

// Define an on/off data source application class

#ifndef __onoff_application_h__
#define __onoff_application_h__

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"

namespace ns3 {

class Ipv4Address;
class RandomVariable;
class Socket;
class DataRate;

class OnOffApplication : public Application {

public:
  OnOffApplication(Ptr<INode> n,
                   const Ipv4Address,  // Peer IP address
                   uint16_t,           // Peer port
                   const RandomVariable&,     // Random variable for On time
                   const RandomVariable&,     // Random variable for Off time
                   DataRate  = g_defaultRate,  // Data rate when on
                   uint32_t = g_defaultSize);  // Size of packets

  virtual ~OnOffApplication();               // Destructor

  void SetMaxBytes(uint32_t maxBytes);

  static void DefaultRate(uint64_t r) { g_defaultRate = r;}

  static void DefaultSize(uint32_t s) { g_defaultSize = s;}

protected:
  virtual void DoDispose (void);
private:
  // inherited from Application base class.
  virtual void StartApplication (void);    // Called at time specified by Start
  virtual void StopApplication (void);     // Called at time specified by Stop

  // Event handlers
  void StartSending();
  void StopSending();
  void SendPacket();

  Ptr<Socket>     m_socket;       // Associated socket
  Ipv4Address     m_peerIP;       // Peer IP address
  uint16_t        m_peerPort;     // Peer port
  bool            m_connected;    // True if connected
  RandomVariable* m_onTime;       // rng for On Time
  RandomVariable* m_offTime;      // rng for Off Time
  DataRate        m_cbrRate;      // Rate that data is generated
  uint32_t        m_pktSize;      // Size of packets
  uint32_t        m_residualBits; // Number of generated, but not sent, bits
  Time            m_lastStartTime;// Time last packet sent
  uint32_t        m_maxBytes;     // Limit total number of bytes sent
  uint32_t        m_totBytes;     // Total bytes sent so far
  EventId         m_startStopEvent;     // Event id for next start or stop event
  EventId         m_sendEvent;    // Eventid of pending "send packet" event
  bool            m_sending;      // True if currently in sending state
  
public:
  static DataRate   g_defaultRate;  // Default sending rate when on
  static uint32_t g_defaultSize;  // Default packet size

private:
  void ScheduleNextTx();
  void ScheduleStartEvent();
  void ScheduleStopEvent();
  void ConnectionSucceeded(Ptr<Socket>);
  void ConnectionFailed(Ptr<Socket>);
  void Ignore(Ptr<Socket>);
};

} // namespace ns3

#endif

