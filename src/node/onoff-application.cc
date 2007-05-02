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

#include "ipv4-address.h"
#include "node.h"
#include "node-reference.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"
#include "onoff-application.h"
#include "ns3/random-variable.h"
#include "socket.h"
#include "ns3/simulator.h"
#include "udp.h"

using namespace std;

namespace ns3 {

// Defaults for rate/size
DataRate OnOffApplication::g_defaultRate = DataRate(500000);
uint32_t OnOffApplication::g_defaultSize = 512;

// Constructors

  OnOffApplication::OnOffApplication(Node * n, 
                                     const Ipv4Address  rip,   // Remote IP addr
                                     uint16_t       rport, // Remote port
                                     const  RandomVariable& ontime,
                                     const  RandomVariable& offtime,
                                     DataRate  rate,
                                     uint32_t size)
    :  Application(n), 
      m_socket(0),      // Socket allocated on Start
      m_peerIP(rip),
      m_peerPort(rport),
      m_connected(false),
      m_onTime(ontime.Copy()),
      m_offTime(offtime.Copy()),
      m_cbrRate(rate),
      m_pktSize(size), 
      m_residualBits(0),
      m_lastStartTime((HighPrecision)0),
      m_maxBytes(0xffffffff),
      m_totBytes(0),
      m_startStopScheduled(false),
      m_sendScheduled(false)
{
}

OnOffApplication::OnOffApplication(Node * n, const OnOffApplication& c)
  : Application(n), 
    m_socket(0),
    m_peerIP(c.m_peerIP),
    m_peerPort(c.m_peerPort),
    m_connected(c.m_connected),
    m_onTime(c.m_onTime->Copy()),
    m_offTime(c.m_offTime->Copy()),
    m_cbrRate(c.m_cbrRate),
    m_pktSize(c.m_pktSize),
    m_residualBits(c.m_residualBits),
    m_lastStartTime(c.m_lastStartTime),
    m_maxBytes(c.m_maxBytes),
    m_totBytes(c.m_totBytes),
    m_startStopScheduled(false),
    m_sendScheduled(false)
{
}

OnOffApplication::~OnOffApplication()
{
  delete m_socket;
  delete m_onTime;
  delete m_offTime;
}

#ifdef REMOVE_THIS
// Handler Methods
void OnOffApplication::Handle(Event* e, Time_t t)
{
  AppOOEvent* ev = (AppOOEvent*)e;
  switch (ev->event) {
    case AppOOEvent::SEND_PKT :
      {
        SendPkt(false);         // Send the packet
        return;
      }
    case AppOOEvent::START_GEN :
      {
        DEBUG0((cout << "StartGen at " << Simulator::Now() << endl));
        lastStartTime = Simulator::Now();
        ScheduleNextTx();
        Time_t onInterval = onTime->Value();
        pendingOO->event = AppOOEvent::STOP_GEN;
         // Schedule the stop event
        Simulator::Schedule(pendingOO, onInterval, this);
        return;
      }
    case AppOOEvent::STOP_GEN :
      {
        DEBUG0((cout << "StopGen at " << Simulator::Now() << endl));
        if (totBytes < maxBytes)
          { // Only schedule if not execeeded maxBytes
            Time_t offInterval = offTime->Value();
            pendingOO->event = AppOOEvent::START_GEN;
            // Schedule the start event
            Simulator::Schedule(pendingOO, offInterval, this);
          }
        if (pendingEvent)
          {
            // Calculate residual bits since last packet sent
            residualBits += (uint32_t)(cbrRate*(Simulator::Now()-lastStartTime));
            Simulator::Cancel(pendingEvent);
            delete pendingEvent;
            pendingEvent = 0;
          }
        return;
      }
  }
  Application::Handle(e, t);  // May be application event
}
#endif

// Application Methods
void OnOffApplication::StartApplication()    // Called at time specified by Start
{
  // Create the socket if not already
  if (!m_socket)
    { // Create the socket using the specified layer 4 protocol
#ifdef NOTYET
      m_socket = PeekNode()->GetKernel()->CreateGenericSocket(*m_l4Proto);
      m_socket->Bind();  // Choose any available port local port
      m_socket->Connect(*m_peerIP, m_peerPort,
                        MakeCallback(&OnOffApplication::ConnectionSucceeded,
                                     this),
                        MakeCallback(&OnOffApplication::ConnectionFailed,
                                     this));
#endif
      m_socket = PeekNode ()->GetUdp ()->CreateSocket ();
      m_socket->Connect (m_peerIP, m_peerPort);
    }
  StopApplication();                         // Insure no pending event
  // If we are not yet connected, there is nothing to do here
  // The ConnectionComplete upcall will start timers at that time
  //if (!m_connected) return;
  ScheduleStartEvent();
}

void OnOffApplication::StopApplication()     // Called at time specified by Stop
{
  if (m_startStopScheduled)
    { // Cancel the startStop event
      Simulator::Cancel(m_startStopEvent);
      m_startStopScheduled = false;
    }
  if (m_sendScheduled)
    { // Cancel the pending send packet event
      Simulator::Cancel(m_sendEvent);
      m_sendScheduled = false;
      // Calculate residual bits since last packet sent
      Time delta(Simulator::Now() - m_lastStartTime);
      m_residualBits += (uint32_t)(m_cbrRate.GetBitRate() * delta.GetSeconds());
    }
}

OnOffApplication* OnOffApplication::Copy() const
{
  return new OnOffApplication(*this);
}

// Event handlers
void OnOffApplication::StartSending()
{
  m_startStopScheduled = true;
  ScheduleNextTx();  // Schedule the send packet event
}

void OnOffApplication::StopSending()
{
  m_startStopScheduled = true;
  if (m_sendScheduled) Simulator::Cancel(m_sendEvent);
}

// Private helpers
void OnOffApplication::ScheduleNextTx()
{
  if (m_totBytes < m_maxBytes)
    {
      uint32_t bits = m_pktSize * 8 - m_residualBits;
      Time nextTime(Seconds (bits / 
        static_cast<double>(m_cbrRate.GetBitRate()))); // Time till next packet
      m_sendScheduled = true;
      m_sendEvent = Simulator::Schedule(nextTime, &OnOffApplication::SendPacket, this);
    }
  else
    { // All done, cancel any pending events
      StopApplication();
    }
}

void OnOffApplication::ScheduleStartEvent()
{  // Schedules the event to start sending data (switch to the "On" state)
  Time offInterval = Seconds(m_offTime->GetValue());
  m_startStopEvent = Simulator::Schedule(offInterval, &OnOffApplication::StartSending, this);
  m_startStopScheduled = true;
}

void OnOffApplication::ScheduleStopEvent()
{  // Schedules the event to stop sending data (switch to "Off" state)
  Time onInterval = Seconds(m_onTime->GetValue());
  Simulator::Schedule(onInterval, &OnOffApplication::StopSending, this);
  m_startStopScheduled = true;
}

  
void OnOffApplication::SendPacket()
{
  NS_ASSERT (m_sendScheduled);
  m_sendScheduled = false;
  m_socket->Send(0, m_pktSize);
#ifdef NOTYET
  m_socket->Send(0, m_pktSize); // Send the packet
#endif
  m_totBytes += m_pktSize;
  m_lastStartTime = Simulator::Now();
  m_residualBits = 0;
  ScheduleNextTx();
}

void OnOffApplication::ConnectionSucceeded(Socket*)
{
  m_connected = true;
  ScheduleStartEvent();
}
  
void OnOffApplication::ConnectionFailed(Socket*)
{
  cout << "OnOffApplication, Connection Failed" << endl;
}

} // Namespace ns3
