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

#include <pthread.h>
#include <cstring>
#include <cerrno> // for strerror

#include "fatal-error.h"
#include "system-mutex.h"
#include "log.h"


NS_LOG_COMPONENT_DEFINE ("SystemMutex");

namespace ns3 {

class SystemMutexPrivate {
public: 
  SystemMutexPrivate ();
  ~SystemMutexPrivate ();
	
  void Lock (void);
  void Unlock (void);
private:
  pthread_mutex_t m_mutex;
};

SystemMutexPrivate::SystemMutexPrivate ()
{
  NS_LOG_FUNCTION (this);

  pthread_mutexattr_t attr;
  pthread_mutexattr_init (&attr);
//
// Make this an error checking mutex.  This will check to see if the current
// thread already owns the mutex before trying to lock it.  Instead of 
// deadlocking it returns an error.  It will also check to make sure a thread
// has previously called pthread_mutex_lock when it calls pthread_mutex_unlock.
//
// Linux and OS X (at least) have, of course chosen different names for the 
// error checking flags just to make life difficult.
//
#if defined (PTHREAD_MUTEX_ERRORCHECK_NP)
  pthread_mutexattr_settype (&attr, PTHREAD_MUTEX_ERRORCHECK_NP);
#else
  pthread_mutexattr_settype (&attr, PTHREAD_MUTEX_ERRORCHECK);
#endif
  pthread_mutex_init (&m_mutex, &attr);
}

SystemMutexPrivate::~SystemMutexPrivate() 
{
  NS_LOG_FUNCTION (this);
  pthread_mutex_destroy (&m_mutex);
}
	
void
SystemMutexPrivate::Lock (void)
{
  NS_LOG_FUNCTION (this);

  int rc = pthread_mutex_lock (&m_mutex);
  if (rc != 0) 
    {
      NS_FATAL_ERROR ("SystemMutexPrivate::Lock()"
                      "pthread_mutex_lock failed: " << rc << " = \"" <<
                      std::strerror (rc) << "\"");
    }
}
	
void
SystemMutexPrivate::Unlock (void) 
{
  NS_LOG_FUNCTION (this);

  int rc = pthread_mutex_unlock (&m_mutex);
  if (rc != 0)
    {
      NS_FATAL_ERROR ("SystemMutexPrivate::Unlock()"
                      "pthread_mutex_unlock failed: " << rc << " = \"" <<
                      std::strerror (rc) << "\"");
    }
}

SystemMutex::SystemMutex() 
  : m_priv (new SystemMutexPrivate ())
{
  NS_LOG_FUNCTION (this);
}

SystemMutex::~SystemMutex() 
{
  NS_LOG_FUNCTION (this);
  delete m_priv;
}

void
SystemMutex::Lock ()
{
  NS_LOG_FUNCTION (this);
  m_priv->Lock ();
}

void
SystemMutex::Unlock ()
{
  NS_LOG_FUNCTION (this);
  m_priv->Unlock ();
}

CriticalSection::CriticalSection (SystemMutex &mutex)
  : m_mutex (mutex)
{
  NS_LOG_FUNCTION (this << &mutex);
  m_mutex.Lock ();
}

CriticalSection::~CriticalSection ()
{
  NS_LOG_FUNCTION (this);
  m_mutex.Unlock ();
}

} // namespace ns3
