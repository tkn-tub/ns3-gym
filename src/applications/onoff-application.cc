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

#include "ns3/ipv4-address.h"
#include "ns3/i-node.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"
#include "ns3/random-variable.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/i-udp.h"
#include "onoff-application.h"

using namespace std;

namespace ns3 {

// Defaults for rate/size
DataRate OnOffApplication::g_defaultRate = DataRate(500000);
uint32_t OnOffApplication::g_defaultSize = 512;

// Constructors

  OnOffApplication::OnOffApplication(Ptr<INode> n, 
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
      m_totBytes(0)
{}

OnOffApplication::~OnOffApplication()
{}

void 
OnOffApplication::SetMaxBytes(uint32_t maxBytes)
{
  m_maxBytes = maxBytes;
}

void
OnOffApplication::DoDispose (void)
{
  m_socket = 0;
  delete m_onTime;
  delete m_offTime;

  m_onTime = 0;
  m_offTime = 0;

  // chain up
  Application::DoDispose ();
}


// Application Methods
void OnOffApplication::StartApplication()    // Called at time specified by Start
{
  // Create the socket if not already
  if (!m_socket)
    {
      Ptr<IUdp> udp = GetINode ()->QueryInterface<IUdp> (IUdp::iid);
      m_socket = udp->CreateSocket ();
      m_socket->Bind ();
      m_socket->Connect (m_peerIP, m_peerPort);
    }
  // Insure no pending event
  StopApplication();
  // If we are not yet connected, there is nothing to do here
  // The ConnectionComplete upcall will start timers at that time
  //if (!m_connected) return;
  ScheduleStartEvent();
}

void OnOffApplication::StopApplication()     // Called at time specified by Stop
{
  if (m_sendEvent.IsRunning ())
    { // Cancel the pending send packet event
      // Calculate residual bits since last packet sent
      Time delta(Simulator::Now() - m_lastStartTime);
      m_residualBits += (uint32_t)(m_cbrRate.GetBitRate() * delta.GetSeconds());
    }
  Simulator::Cancel(m_sendEvent);
  Simulator::Cancel(m_startStopEvent);
}

// Event handlers
void OnOffApplication::StartSending()
{
  ScheduleNextTx();  // Schedule the send packet event
}

void OnOffApplication::StopSending()
{
  Simulator::Cancel(m_sendEvent);
}

// Private helpers
void OnOffApplication::ScheduleNextTx()
{
  if (m_totBytes < m_maxBytes)
    {
      uint32_t bits = m_pktSize * 8 - m_residualBits;
      Time nextTime(Seconds (bits / 
        static_cast<double>(m_cbrRate.GetBitRate()))); // Time till next packet
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
}

void OnOffApplication::ScheduleStopEvent()
{  // Schedules the event to stop sending data (switch to "Off" state)
  Time onInterval = Seconds(m_onTime->GetValue());
  Simulator::Schedule(onInterval, &OnOffApplication::StopSending, this);
}

  
void OnOffApplication::SendPacket()
{
  NS_ASSERT (m_sendEvent.IsExpired ());
  m_socket->Send(0, m_pktSize);
  m_totBytes += m_pktSize;
  m_lastStartTime = Simulator::Now();
  m_residualBits = 0;
  ScheduleNextTx();
}

void OnOffApplication::ConnectionSucceeded(Ptr<Socket>)
{
  m_connected = true;
  ScheduleStartEvent();
}
  
void OnOffApplication::ConnectionFailed(Ptr<Socket>)
{
  cout << "OnOffApplication, Connection Failed" << endl;
}

} // Namespace ns3
