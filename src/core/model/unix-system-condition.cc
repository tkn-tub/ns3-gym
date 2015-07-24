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

#include <pthread.h>
#include <cerrno> // for ETIMEDOUT
#include <sys/time.h>

#include "fatal-error.h"
#include "system-condition.h"
#include "log.h"


/**
 * \file
 * \ingroup thread
 * Thread conditional wait implementation for Unix-like systems.
 */

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("SystemCondition");

/**
 * \ingroup thread
 * Implementation of SystemCondition for Unix-like systems.
 */
class SystemConditionPrivate {
public:
  /// Conversion from ns to s.
  static const uint64_t NS_PER_SEC = (uint64_t)1000000000;

  /** Constructor. */
  SystemConditionPrivate ();
  /** Destructor. */
  ~SystemConditionPrivate ();

  /**
   * Set the condition.
   *
   * \param [in] condition The new condition value.
   */
  void SetCondition (bool condition);
  /**
   * Get the condition value.
   *
   * \returns The condition value.
   */
  bool GetCondition (void);
  /** Signal the condition. */
  void Signal (void);
  /** Broadcast the condition. */
  void Broadcast (void);
  /**
   * Unset the condition, then wait for another thread
   * to set it with SetCondition. */
  void Wait (void);
  /**
   * Unset the condition, then wait for a limited amount of wall-clock
   * time for another thread to set it with SetCondition.
   *
   * \param [in] ns Maximum time to wait, in ns.
   * \returns \c true if the condition timed out; \c false if the other
   * thread set it.
   */
  bool TimedWait (uint64_t ns);

private:
  /** Mutex controlling access to the condition. */
  pthread_mutex_t m_mutex;
  /** The pthread condition variable. */
  pthread_cond_t  m_cond;
  /** The condition state. */
  bool m_condition;
};

SystemConditionPrivate::SystemConditionPrivate ()
{
  NS_LOG_FUNCTION (this);

  m_condition = false;

  pthread_mutexattr_t mAttr;
  pthread_mutexattr_init (&mAttr);
//
// Linux and OS X (at least) have, of course chosen different names for the 
// error checking flags just to make life difficult.
//
#if defined (PTHREAD_MUTEX_ERRORCHECK_NP)
  pthread_mutexattr_settype (&mAttr, PTHREAD_MUTEX_ERRORCHECK_NP);
#else
  pthread_mutexattr_settype (&mAttr, PTHREAD_MUTEX_ERRORCHECK);
#endif
  pthread_mutex_init (&m_mutex, &mAttr);

  pthread_condattr_t cAttr;
  pthread_condattr_init (&cAttr);
  pthread_condattr_setpshared (&cAttr, PTHREAD_PROCESS_PRIVATE);
  pthread_cond_init (&m_cond, &cAttr);
}

SystemConditionPrivate::~SystemConditionPrivate() 
{
  NS_LOG_FUNCTION (this);
  pthread_mutex_destroy (&m_mutex);
  pthread_cond_destroy (&m_cond);
}
	
void
SystemConditionPrivate::SetCondition (bool condition)
{
  NS_LOG_FUNCTION (this << condition);
  m_condition = condition;
}
	
bool
SystemConditionPrivate::GetCondition (void)
{
  NS_LOG_FUNCTION (this);
  return m_condition;
}
	
void
SystemConditionPrivate::Signal (void)
{
  NS_LOG_FUNCTION (this);

  pthread_mutex_lock (&m_mutex);
  pthread_cond_signal (&m_cond);
  pthread_mutex_unlock (&m_mutex);
}
	
void
SystemConditionPrivate::Broadcast (void)
{
  NS_LOG_FUNCTION (this);

  pthread_mutex_lock (&m_mutex);
  pthread_cond_broadcast (&m_cond);
  pthread_mutex_unlock (&m_mutex);
}

void
SystemConditionPrivate::Wait (void)
{
  NS_LOG_FUNCTION (this);

  pthread_mutex_lock (&m_mutex);
  m_condition = false;
  while (m_condition == false)
    {
      pthread_cond_wait (&m_cond, &m_mutex);
    }
  pthread_mutex_unlock (&m_mutex);
}

bool
SystemConditionPrivate::TimedWait (uint64_t ns)
{
  NS_LOG_FUNCTION (this << ns);

  struct timespec ts;
  ts.tv_sec = ns / NS_PER_SEC;
  ts.tv_nsec = ns % NS_PER_SEC;

  struct timeval tv;
  gettimeofday (&tv, NULL);

  ts.tv_sec += tv.tv_sec;
  ts.tv_nsec += tv.tv_usec * 1000;
  if (ts.tv_nsec > (int64_t)NS_PER_SEC)
    {
      ++ts.tv_sec;
      ts.tv_nsec %= NS_PER_SEC;
    }

  int rc;

  pthread_mutex_lock (&m_mutex);
  while (m_condition == false)
    {
      rc = pthread_cond_timedwait (&m_cond, &m_mutex, &ts);
      if (rc == ETIMEDOUT)
        {
          pthread_mutex_unlock (&m_mutex); 
          return true;
        }
    }
  pthread_mutex_unlock (&m_mutex);
  return false;
}
	
SystemCondition::SystemCondition() 
  : m_priv (new SystemConditionPrivate ())
{
  NS_LOG_FUNCTION (this);;
}

SystemCondition::~SystemCondition () 
{
  NS_LOG_FUNCTION (this);
  delete m_priv;
}

void
SystemCondition::SetCondition (bool condition) 
{
  NS_LOG_FUNCTION (this << condition);
  m_priv->SetCondition (condition);
}

bool
SystemCondition::GetCondition (void) 
{
  NS_LOG_FUNCTION (this);
  return m_priv->GetCondition ();
}

void
SystemCondition::Signal (void) 
{
  NS_LOG_FUNCTION (this);
  m_priv->Signal ();
}

void
SystemCondition::Broadcast (void) 
{
  NS_LOG_FUNCTION (this);
  m_priv->Broadcast ();
}

void
SystemCondition::Wait (void) 
{
  NS_LOG_FUNCTION (this);
  m_priv->Wait ();
}

bool
SystemCondition::TimedWait (uint64_t ns) 
{
  NS_LOG_FUNCTION (this << ns);
  return m_priv->TimedWait (ns);
}

} // namespace ns3
