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
#include "udp.h"
#include "ns3/event-id.h"

#define nil 0

namespace ns3 {

class IPAddr;
class L4Protocol;
class RandomVariable;
class Socket;

#if 0
class Time;

//Doc:ClassXRef
class OnOffApplication : public Application {
  //Doc:Class Defines an application that uses the on--off data generator
  //Doc:Class model.

public:
  // Define a number of constructors for different needs
  // Endpoints, on/off rng, l4 protocol
  //Doc:Method
  OnOffApplication(const IPAddr&,    // Peer IP Address
                   PortId_t,         // Peer port
                   const RandomVariable&,     // Random variable for On time
                   const RandomVariable&,     // Random variable for Off time
                   double   = g_defaultRate,  // Data rate when on
                   uint32_t = g_defaultSize,  // Size of packets
                   const L4Protocol& = Udp(nil));// Layer 4 protocol to use
    //Doc:Desc Constructor specifying destination IP/Port, a single random
    //Doc:Desc number generator for both the {\em On} and {\em Off}
    //Doc:Desc distributions, and a layer 4 protocol object.  Optionally
    //Doc:Desc specifies bit rate (when {\em On} (defaults to a globally
    //Doc:Desc specified default rate), and optionally a packet size
    //Doc:Desc (defaults to a globally specified default size).  See
    //Doc:Desc {\tt SetDefaultRate} and {\tt SetDefaultSize} below.
    //Doc:Arg1 IPAddress of remote  endpoint (destination)
    //Doc:Arg2 Port number for remote endpoing (destination)
    //Doc:Arg3 Random number generator to use for {\em On} time period.
    //Doc:Arg4 Random number generator to use for both {\em Off} time period.
    //Doc:Arg5 Data rate to generate when {\em On}.
    //Doc:Arg6 Packet size.
    //Doc:Arg7 A layer 4 protocol object to copy.

  OnOffApplication(const OnOffApplication&); // Copy constructor
  virtual ~OnOffApplication();               // Destructor
  virtual void StartApp();    // Called at time specified by Start
  virtual void StopApp();     // Called at time specified by Stop
  virtual OnOffApplication* Copy() const;// Make a copy of the application
  virtual L4Protocol* GetL4() const { return m_l4Proto;}

  // Event handlers
  void StartSending();
  void StopSending();
  void SendPacket();

  //Doc:Method
  virtual void MaxBytes(uint32_t m) { m_maxBytes = m;}
    //Doc:Desc Specify a maximum number of bytes to send, after which the
    //Doc:Desc application shuts down.
    //Doc:Arg1 Maximum number of bytes to send.

public: // Static methods
  //Doc:Method
  static void DefaultRate(double r) { g_defaultRate = r;}
    //Doc:Desc Specify a default data rate to use for all On/Off applications
    //Doc:Desc by default
    //Doc:Arg1 The default data rate (when the application is {\em On}.

  //Doc:Method
  static void DefaultSize(uint32_t s) { g_defaultSize = s;}
    //Doc:Desc Specifies a default packet size to use for all
    //Doc:Desc On/Off applcations
    //Doc:Desc by default.
    //Doc:Arg1 The default packet size.

public:
  L4Protocol*     m_l4Proto;      // Points to the specified Layer 4 protocol
  Socket*         m_socket;       // Associated socket
  IPAddr*         m_peerIP;       // PeerIP
  PortId_t        m_peerPort;     // Peer port
  bool            m_connected;    // True if connected
  RandomVariable* m_onTime;       // rng for On Time
  RandomVariable* m_offTime;      // rng for Off Time
  double          m_cbrRate;      // Rate that data is generated
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
  static double   g_defaultRate;  // Default sending rate when on
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

#endif

} // namespace ns3

#endif

