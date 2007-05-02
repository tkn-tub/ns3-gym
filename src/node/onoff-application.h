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

#include "application.h"
#include "ns3/event-id.h"

namespace ns3 {

class Ipv4Address;
class RandomVariable;
class Socket;
class DataRate;

class OnOffApplication : public Application {

public:
  OnOffApplication(const Node& n,
                   const Ipv4Address,  // Peer IP address
                   uint16_t,           // Peer port
                   const RandomVariable&,     // Random variable for On time
                   const RandomVariable&,     // Random variable for Off time
                   DataRate  = g_defaultRate,  // Data rate when on
                   uint32_t = g_defaultSize);  // Size of packets

  OnOffApplication(const Node& n, const OnOffApplication&); // Copy constructor
  virtual ~OnOffApplication();               // Destructor
  virtual void StartApplication();    // Called at time specified by Start
  virtual void StopApplication();     // Called at time specified by Stop
  virtual OnOffApplication* Copy() const;// Make a copy of the application

  // Event handlers
  void StartSending();
  void StopSending();
  void SendPacket();

  virtual void MaxBytes(uint32_t m) { m_maxBytes = m;}

public: // Static methods
  static void DefaultRate(uint64_t r) { g_defaultRate = r;}

  static void DefaultSize(uint32_t s) { g_defaultSize = s;}

public:
  Socket        * m_socket;       // Associated socket
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
  bool            m_startStopScheduled; // True if start or stop event scheduled
  EventId         m_startStopEvent;     // Event id for next start or stop event
  bool            m_sendScheduled;// True of send event scheduled
  EventId         m_sendEvent;    // Eventid of pending "send packet" event
  bool            m_sending;      // True if currently in sending state
  
public:
  static DataRate   g_defaultRate;  // Default sending rate when on
  static uint32_t g_defaultSize;  // Default packet size

private:
  void ScheduleNextTx();
  void ScheduleStartEvent();
  void ScheduleStopEvent();
  void ConnectionSucceeded(Socket*);
  void ConnectionFailed(Socket*);
  void Ignore(Socket*);
protected:
};

} // namespace ns3

#endif

