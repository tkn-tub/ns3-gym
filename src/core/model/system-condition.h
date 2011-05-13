/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Washington
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
 */

#ifndef SYSTEM_CONDITION_H
#define SYSTEM_CONDITION_H

#include "ptr.h"

namespace ns3 { 
	
class SystemConditionPrivate;

/**
 * @brief A class which provides a relatively platform-independent 
 * conditional-wait thread synchronization primitive.
 *
 * It is often desirable to have a mechanism by which a thread can suspend its
 * execution and relinquish the process until some condition to becomes true.
 * We provide platform-independent access to this OS-dependent capability with
 * the SystemCondition class.
 *
 * There are two ways to tell the underlying primitive that the condition has
 * become true:  Signal and Broadcast.  Signal will only wake up one thread
 * waiting on the condition (according to the OS scheduling policy); 
 * Broadcast will wake up all of the threads waiting on the condition 
 * (cf. "The Thundering Herd").
 * 
 * In order to wait for the underlying condition, you also have two
 * alternatives:  Wait and TimedWait.  The Wait call will wait forever for the
 * condition to become true; but the TimedWait has a timeout.
 *
 * The condition underlying this class is a simple boolean variable.  It is
 * set to false in each call to Wait and TimedWait.  It is set to true in each
 * call to Signal and Broadcast.  This is a fairly simple-minded condition
 * designed for 
 *
 * A typical use case will be to call Wait() or TimedWait() in one thread
 * context and put the processor to sleep until an event happens somewhere 
 * else that 
 */
class SystemCondition
{
public:
  SystemCondition ();
  ~SystemCondition ();

  /**
   * Set the value of the underlying condition.
   * \param condition value
   */
  void SetCondition (bool condition);

  /**
   * Get the value of the underlying condition.
   */
  bool GetCondition (void);

  /**
   * Release one thread if waiting for the condition to be true.  If you want 
   * a waiting thread to return, you should have done a SetCondition (true)
   * prior to calling.
   */
  void Signal (void);

  /**
   * Release all threads waiting for the condition to be true.  If you want 
   * all waiting threads to return, you should have done a SetCondition (true)
   * prior to calling.
   */
  void Broadcast (void);

  /**
   * Wait, possibly forever, for the condition to be true.
   */
  void Wait (void);
	
  /**
   * Wait a maximum of ns nanoseconds for the condition to be true.  If the
   * wait times out, return true else return false.
   * \param ns maximum of nanoseconds to wait
   */
  bool TimedWait (uint64_t ns);
	

private:
  SystemConditionPrivate * m_priv;
};

} //namespace ns3

#endif /* SYSTEM_CONDITION_H */


