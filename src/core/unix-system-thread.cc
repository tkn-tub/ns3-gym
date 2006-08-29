/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
/*
 * Copyright (c) 2006 INRIA
 * All rights reserved.
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "system-thread.h"

#include <pthread.h>
#include <cassert>

namespace ns3 {

class SystemThreadPrivate {
public:
	SystemThreadPrivate (SystemThread *thread);
	~SystemThreadPrivate ();
private:
	static void *pthread_run (void *thread);
	pthread_t m_system_thread;
	SystemThread *m_thread;
};

SystemThreadPrivate::SystemThreadPrivate (SystemThread *thread)
	: m_thread (thread)
{
	int retval;
	retval = pthread_create (&m_system_thread,
				 NULL,
				 SystemThreadPrivate::pthread_run,
				 this);
	assert (retval == 0);
}
SystemThreadPrivate::~SystemThreadPrivate ()
{}
void *
SystemThreadPrivate::pthread_run (void *thread)
{
	SystemThreadPrivate *self = reinterpret_cast<SystemThreadPrivate *> (thread);
	self->m_thread->real_run ();
	return thread;
}

SystemThread::SystemThread ()
	: m_priv (new SystemThreadPrivate (this))
{}

SystemThread::~SystemThread ()
{
	delete m_priv;
}

}; // namespace ns3

