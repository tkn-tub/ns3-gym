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

#include "ns3/event-id.h"
#include "ns3/nstime.h"
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/node.h"

namespace ns3 {

class Node;
class RandomVariable;

/**
 * \addtogroup applications Applications
 *
 * Class ns3::Application can be used as a base class for ns3 applications.
 * Applications are associated with individual nodes.  Each node
 * holds a list of references (smart pointers) to its applications.
 * 
 * Conceptually, an application has zero or more ns3::Socket
 * objects associated with it, that are created using the Socket
 * creation API of the Kernel capability.  The Socket object
 * API is modeled after the
 * well-known BSD sockets interface, although it is somewhat 
 * simplified for use with ns3.  Further, any socket call that
 * would normally "block" in normal sockets will return immediately
 * in ns3.  A set of "upcalls" are defined that will be called when
 * the previous blocking call would normally exit.  THis is documented
 * in more detail Socket class in socket.h.
 *
 * The main purpose of the base class application public API is to  
 * provide a uniform way to start and stop applications.
 */

 /**
 * \brief The base class for all ns3 applications
 * 
 */
class Application : public Object
{
public:
  static TypeId GetTypeId (void);
  Application ();
  virtual ~Application ();
  
  /**
   * \brief Specify application start time
   * \param startTime Start time for this application,
   *        relative to the current simulation time.
   *
   * Applications start at various times in the simulation scenario.
   * The Start method specifies when the application should be
   * started.  The application subclasses should override the
   * private "StartApplication" method defined below, which is called at the
   * time specified, to cause the application to begin.
   */
  void Start (const Time& startTime);

  /** 
   * \brief Specify application start time.
   * \param startVariable the random variable to use to pick
   *        the real start time as a relative time, in units of
   *        seconds, relative to the current simulation time.
   */
  void Start (const RandomVariable& startVariable);
  
  /**
   * \brief Specify application stop time
   * \param stopTime Stop time for this application, relative to the
   *        current simulation time.
   *
   * Once an application has started, it is sometimes useful
   * to stop the application.  The Stop method specifies when an
   * application is to stop.  The application subclasses should override
   * the private StopApplication method, to be notified when that
   * time has come.
   */
  void Stop (const Time& stopTime);

  /**
   * \brief Specify application stop time
   * \param stopVariable the random variable to use to pick
   *        the real stop time, in units of seconds, 
   *        relative to the current simulation time.
   */
  void Stop (const RandomVariable& stopVariable);

  /**
   * \returns the Node to which this Application object is attached.
   */
  Ptr<Node> GetNode () const;

  /**
   * \param node the node to which this Application object is attached.
   */
  void SetNode (Ptr<Node> node);
  
private:
  /**
   * \brief Application specific startup code
   *
   * The StartApplication method is called at the start time specifed by Start
   * This method should be overridden by all or most application
   * subclasses.
   */
  virtual void StartApplication (void);

  /**
   * \brief Application specific shutdown code
   *
   * The StopApplication method is called at the stop time specifed by Stop
   * This method should be overridden by all or most application
   * subclasses.
   */
  virtual void StopApplication (void);
protected:
  virtual void DoDispose (void);
private:
  void ScheduleStart (const Time &time);
  void ScheduleStop (const Time &time);

  EventId         m_startEvent;
  EventId         m_stopEvent;
  Ptr<Node>       m_node;
};

} //namespace ns3
#endif
