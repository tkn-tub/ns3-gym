/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage.inria.fr>
 */

#ifndef SYSTEM_MUTEX_H
#define SYSTEM_MUTEX_H

#include "ptr.h"

/**
 * @file
 * @ingroup thread
 * System-independent mutex primitive, ns3::SystemMutex,
 * and ns3::CriticalSection.
 */

namespace ns3 { 
	
class SystemMutexPrivate;

/**
 * @ingroup thread
 * @brief A class which provides a relatively platform-independent Mutual
 * Exclusion thread synchronization primitive.
 *
 * When more than one thread needs to access a shared resource (data structure
 * or device), the system needs to provide a way to serialize access to the
 * resource.  An operating system will typically provide a Mutual Exclusion
 * primitive to provide that capability.  We provide plattorm-independent
 * access to the OS-dependent capability with the SystemMutex class.
 *
 * There are two operations:  Lock and Unlock.  Lock allows an executing 
 * SystemThread to attempt to acquire ownership of the Mutual Exclusion 
 * object.  If the SystemMutex object is not owned by another thread, then
 * ownership is granted to the calling SystemThread and Lock returns 
 * immediately,  However, if the SystemMutex is already owned by another
 * SystemThread, the calling SystemThread is blocked until the current owner
 * releases the SystemMutex by calling Unlock.
 *
 * @see CriticalSection
 */
class SystemMutex 
{
public:
  SystemMutex ();
  ~SystemMutex ();

  /**
   * Acquire ownership of the Mutual Exclusion object.
   */
  void Lock ();

  /**
   * Release ownership of the Mutual Exclusion object.
   */
  void Unlock ();
	
private:
  /** The (system-dependent) implementation. */
  SystemMutexPrivate * m_priv;
};

/**
 * @brief A class which provides a simple way to implement a Critical Section.
 *
 * When more than one SystemThread needs to access a shared resource, we
 * control access by acquiring a SystemMutex.  The CriticalSection class uses
 * the C++ scoping rules to automatically perform the required Lock and Unlock
 * operations on the shared SystemMutex to implement a Critical Section.
 *
 * If one wants to treat an entire method call as a critical section, one would
 * do something like,
 * @code
 *   Class::Method ()
 *   {
 *     CriticalSection cs (mutex);
 *     ...
 *   }
 * @endcode
 * In this case, the critical section is entered when the CriticalSection 
 * object is created, and the critical section is exited when the 
 * CriticalSection object goes out of scope at the end of the method.
 *
 * Finer granularity is achieved by using local scope blocks.
 * @code
 *   Class::Method ()
 *   {
 *     ...
 *     {
 *       CriticalSection cs (mutex);
 *     }
 *     ...
 *   }
 * @endcode
 * Here, the critical section is entered partway through the method when the
 * CriticalSection object is created in the local scope block (the braces).
 * The critical section is exited when the CriticalSection object goes out of
 * scope at the end of block.
 *
 * @see SystemMutex
 */
class CriticalSection
{
public:
  /**
   * Construct with the required SystemMutex.
   *
   * @param [in] mutex The mutex.
   */
  CriticalSection (SystemMutex &mutex);
  /** Destructor */
  ~CriticalSection ();
private:
  SystemMutex &m_mutex;  /**< The mutex. */
};

} // namespace ns3

#endif /* SYSTEM_MUTEX_H */
