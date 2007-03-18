/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 Georgia Tech Research Corporation
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
 * Author: George F. Riley<riley@ece.gatech.edu>
 */

// Implementation for ns3 Application base class.
// George F. Riley, Georgia Tech, Fall 2006

#include "application.h"
#include "node.h"
#include "node-reference.h"
#include "ns3/nstime.h"
#include "ns3/random-variable.h"
#include "ns3/simulator.h"

using namespace std;

namespace ns3 {

#define nil 0

// Application Methods

// \brief Application Constructor
  Application::Application() : m_node(nil), m_startVar(nil), m_stopVar(nil),
                               m_start(false), m_stop(false)
{
}

Application::Application(const Application& o)
    : m_node(nil), m_startVar(nil), m_stopVar(nil),
      m_start(false), m_stop(false)
{ // Copy constructor
  if (o.GetNode())m_node = new NodeReference(*o.GetNode());
  // Copy the start and stop random variables if they exist
  if (o.m_startVar) m_startVar = o.m_startVar->Copy();
  if (o.m_stopVar)  m_stopVar  = o.m_stopVar->Copy();
  if (o.m_start)    ScheduleStart();
  if (o.m_stop)     ScheduleStop();
}

  
// \brief Application Destructor
Application::~Application()
{
  delete m_node;
  // Cancel the start/stop events if scheduled
  if (m_start) Simulator::Cancel(m_startEvent);
  if (m_stop)  Simulator::Cancel(m_stopEvent);
  // Delete the random variablse
  delete m_startVar;
  delete m_stopVar;
}
  
Application& Application::operator=(const Application& rhs)
{
  if (this == &rhs) return *this; // Self assignment
  delete m_node;
  m_node = nil;
  if (rhs.GetNode())m_node = new NodeReference(*rhs.GetNode());

  delete m_startVar;
  m_startVar = nil;
  if (rhs.m_startVar) m_startVar = rhs.m_startVar->Copy();
  
  delete m_stopVar;
  m_stopVar = nil;
  if (rhs.m_stopVar) m_stopVar = rhs.m_stopVar->Copy();
  
  m_start = false;
  if (rhs.m_start) ScheduleStart();
  if (rhs.m_stop)  ScheduleStop();
  return *this;
}
  
   
// \brief Specify application start time
// The virtual method STartApp will be called at the time
// specified by startTime.
// \param Time to start application (absolute time, from start of simulation)
void Application::Start(const Time& startTime)
{
  delete m_startVar;
  m_startVar = new ConstantVariable(startTime.GetSeconds());
  ScheduleStart();
}

void Application::Start(const RandomVariable& startVar)
{ // Start at random time
  delete m_startVar;
  m_startVar = startVar.Copy();
  ScheduleStart();
}

   
// \brief Specify application stop time
// The virtual method StopApp will be called at the time
// specified by stopTime.
// \param Time to stop application (absolute time, from start of simulation)
void Application::Stop(const Time& stopTime)
{
  delete m_stopVar;
  m_stopVar = new ConstantVariable(stopTime.GetSeconds());
  ScheduleStop();
}

void Application::Stop(const RandomVariable& stopVar)
{ // Stop at random time
  delete m_stopVar;
  m_stopVar = stopVar.Copy();
  ScheduleStop();
}

// \brief Assign this application to a given node
// Called by the application manager capability when adding
// an application to a node.
void Application::SetNode(const Node& n)
{
  delete m_node;
  m_node = new NodeReference(n);
}
  
Node* Application::GetNode() const
{
  return m_node->GetNode();
}

// Protected methods
// StartApp and StopApp will likely be overridden by application subclasses
void Application::StartApplication()
{ // Provide null functionality in case subclass is not interested
}

void Application::StopApplication()
{ // Provide null functionality in case subclass is not interested
}


// Private helpers
void Application::ScheduleStart()
{
  m_startEvent = Simulator::Schedule(Seconds(m_startVar->GetValue()) -
                                     Simulator::Now(),
                                     &Application::StartApplication, this);
  m_start = true;
}

void Application::ScheduleStop()
{
  m_stopEvent = Simulator::Schedule(Seconds(m_stopVar->GetValue()) -
                                     Simulator::Now(),
                                     &Application::StopApplication, this);
  m_stop = true;
}

} //namespace ns3
      
  
