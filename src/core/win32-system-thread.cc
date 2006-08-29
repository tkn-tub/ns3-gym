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
#include <windows.h>
#include <cassert>

namespace ns3 {

class SystemThreadPrivate {
public:
	SystemThreadPrivate (SystemThread *thread);
	~SystemThreadPrivate ();
private:
	static void *thread_run (void *thread);
	SystemThread *m_thread;
	HANDLE m_system_thread;
};

SystemThreadPrivate::SystemThreadPrivate (SystemThread *thread)
	: m_thread (thread)
{
	m_system_thread = CreateThread(NULL, 0, 
				       (LPTHREAD_START_ROUTINE)SystemThreadPrivate::thread_run, 
				       this, 0, 0);
	assert (m_system_thread != NULL);
}
SystemThreadPrivate::~SystemThreadPrivate ()
{}
void *
SystemThreadPrivate::thread_run (void *thread)
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

