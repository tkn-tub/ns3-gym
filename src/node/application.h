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

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

//
// \brief The base class for all ns3 applicationes
//
// Class Application is the base class for all ns3 applications.
// Applications are associated with individual nodes, and are created
// using the AddApplication method in the ApplicationManager capability.
// 
// Conceptually, an application has zero or more Socket
// objects associated with it, that are created using the Socket
// creation API of the Kernel capability.  The Socket object
// API is modeled after the
// well-known BSD sockets interface, although it is somewhat 
// simplified for use with ns3.  Further, any socket call that
// would normally "block" in normal sockets will return immediately
// in ns3.  A set of "upcalls" are defined that will be called when
// the previous blocking call would normally exit.  THis is documented
// in more detail Socket class in socket.h.
// 
// There is a second application class in ns3, called "ThreadedApplication"
// that implements a true sockets interface, which should be used
// when porting existing sockets code to ns3.  The true
// sockets approach is significantly
// less memory--efficient using private stacks for each defined application,
// so that approach should be used with care.  The design and implementation
// of the ThreadedApplication are still being discussed.

#include "ns3/event-id.h"
#include "ns3/nstime.h"

namespace ns3 {

class Node;
class RandomVariable;
  
class Application {
public:
  Application(Node *);
  Application(const Application&);  // Copy constructor
  Application& operator=(const Application&); // Assignment operator
  virtual ~Application();
  
  virtual Application* Copy() const = 0; // All applications must provide
  
  // \brief Specify application start time
  // Applications start at various times in the simulation scenario.
  // The Start method specifies when the application should be
  // started.  The application subclasses should override the
  // private "StartApplication" method defined below, which is called at the
  // time specified, to cause the application to begin.
  // \param Start time for this application, relative to the
  // current simulation time.
  void Start(const Time&);

  // \brief Same as above, but uses a random variable for start time
  // The random variable returns the desired start time in units of
  // Seconds.
  
void Start(const RandomVariable&);
  
  // \brief Specify application stop time
  // Once an application has started, it is sometimes useful
  // to stop the application.  The Stop method specifies when an
  // application is to stop.  The application subclasses should override
  // the private StopApplication method defined below, to cause the application
  // to stop.
  // \param Stop time for this application, relative to the
  // current simulation time.
  void Stop(const Time&);

  // \brief Same as above, but uses a random variable for stop time
  // The random variable returns the desired stop time in units of
  // Seconds.
  void Stop(const RandomVariable&);
  
  // \brief Attaches an application to a specific node
  // Specifies which node object this application is associated with.
  // \param Node object to associate with this application.
  void SetNode(Node *);

  // \brief Returns the pointer to the attached node.
  Node* PeekNode() const;
  
  // Members
  Node          * m_node;      // All applications have an associated node
  RandomVariable* m_startVar;  // Random variable for start time
  RandomVariable* m_stopVar;   // Random variable for stop time
  EventId         m_startEvent;// Event identifier for start event
  EventId         m_stopEvent; // Event identifier for the stop event
  bool            m_start;     // True if start event scheduled
  bool            m_stop;      // True if stop event scheduled
  
protected:
  // \brief Application specific startup code
  // The StartApplication method is called at the start time specifed by Start
  // This method should be overridden by all or most application
  // subclasses.
  virtual void StartApplication();

  // \brief Application specific shutdown code
  // The StopApplication method is called at the stop time specifed by Stop
  // This method should be overridden by all or most application
  // subclasses.
  virtual void StopApplication();

private:
  // Helpers
  void ScheduleStart();
  void ScheduleStop();
};

} //namespace ns3
#endif
