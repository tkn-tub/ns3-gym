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
#include <string.h>
#include <signal.h>
#include "fatal-error.h"
#include "system-thread.h"
#include "log.h"

NS_LOG_COMPONENT_DEFINE ("SystemThread");

namespace ns3 {

//
// Private implementation class for the SystemThread class.  The deal is
// that we export the SystemThread class to the user.  The header just 
// declares a class and its members.  There is a forward declaration for
// a private implementation class there and a member declaration.  Thus
// there is no knowledge of the implementation in the exported header.
//
// We provide an implementation class for each operating system.  This is
// the Unix implementation of the SystemThread.
//
// In order to use the SystemThread, you will include "system-thread.h" and
// get the implementation by linking unix-system-thread.cc (if you are running
// a Posix system).
//
class SystemThreadImpl
{
public:
  SystemThreadImpl (Callback<void> callback);

  void Start (void);
  void Join (void);

private:
  static void *DoRun (void *arg);
  Callback<void> m_callback;
  pthread_t m_thread;
  void *    m_ret;
};

SystemThreadImpl::SystemThreadImpl (Callback<void> callback)
  : m_callback (callback)
{
  NS_LOG_FUNCTION_NOARGS ();
  // Make sure we have a SIGALRM handler which does not terminate
  // our process.
  struct sigaction act;
  act.sa_flags = 0;
  sigemptyset (&act.sa_mask);
  act.sa_handler = SIG_IGN;
  sigaction (SIGALRM, &act, 0);
}

  void
SystemThreadImpl::Start (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  int rc = pthread_create (&m_thread, NULL, &SystemThreadImpl::DoRun,
    (void *)this);

  if (rc) 
    {
      NS_FATAL_ERROR ("pthread_create failed: " << rc << "=\"" << 
        strerror(rc) << "\".");
    }
}

  void 
SystemThreadImpl::Join (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  // send a SIGALRM signal on the target thread to make sure that it
  // will unblock.
  pthread_kill (m_thread, SIGALRM);
  void *thread_return;
  int rc = pthread_join (m_thread, &thread_return);
  if (rc) 
    {
      NS_FATAL_ERROR ("pthread_join failed: " << rc << "=\"" << 
        strerror(rc) << "\".");
    }
}

  void *
SystemThreadImpl::DoRun (void *arg)
{
  NS_LOG_FUNCTION_NOARGS ();

  SystemThreadImpl *self = static_cast<SystemThreadImpl *> (arg);
  self->m_callback ();

  return 0;
}

//
// Remember that we just export the delcaration of the SystemThread class to
// the user.  There is no code to implement the SystemThread methods.  We
// have to do that here.  We just vector the calls to our implementation 
// class above.
//
SystemThread::SystemThread (Callback<void> callback) 
  : m_impl (new SystemThreadImpl (callback)),
    m_count (1)
{
  NS_LOG_FUNCTION_NOARGS ();
}
  
SystemThread::~SystemThread() 
{
  NS_LOG_FUNCTION_NOARGS ();
  delete m_impl;
}
  
  void 
SystemThread::Start (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_impl->Start ();
}
  
  void 
SystemThread::Join (void) 
{
  NS_LOG_FUNCTION_NOARGS ();
  m_impl->Join ();
}  

} // namespace ns3
