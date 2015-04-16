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

#include "fatal-error.h"
#include "system-thread.h"
#include "log.h"
#include <cstring>

/**
 * @file
 * @ingroup thread
 * System-independent thread class ns3::SystemThread definitions.
 */

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("SystemThread");

#ifdef HAVE_PTHREAD_H

SystemThread::SystemThread (Callback<void> callback)
  : m_callback (callback)
{
  NS_LOG_FUNCTION (this << &callback);
}

SystemThread::~SystemThread()
{
  NS_LOG_FUNCTION (this);
}

void
SystemThread::Start (void)
{
  NS_LOG_FUNCTION (this);

  int rc = pthread_create (&m_thread, NULL, &SystemThread::DoRun,
                           (void *)this);

  if (rc) 
    {
      NS_FATAL_ERROR ("pthread_create failed: " << rc << "=\"" << 
                      strerror (rc) << "\".");
    }
}

void
SystemThread::Join (void)
{
  NS_LOG_FUNCTION (this);

  void *thread_return;
  int rc = pthread_join (m_thread, &thread_return);
  if (rc) 
    {
      NS_FATAL_ERROR ("pthread_join failed: " << rc << "=\"" << 
                      strerror (rc) << "\".");
    }
}

void *
SystemThread::DoRun (void *arg)
{
  NS_LOG_FUNCTION (arg);

  SystemThread *self = static_cast<SystemThread *> (arg);
  self->m_callback ();

  return 0;
}
SystemThread::ThreadId 
SystemThread::Self (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return pthread_self ();
}

bool 
SystemThread::Equals (SystemThread::ThreadId id)
{
  NS_LOG_FUNCTION (id);
  return (pthread_equal (pthread_self (), id) != 0);
}

#endif /* HAVE_PTHREAD_H */

} // namespace ns3
