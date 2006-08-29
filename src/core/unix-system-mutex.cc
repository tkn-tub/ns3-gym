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

#include "system-mutex.h"

#include <pthread.h>
#include <cassert>

namespace ns3 {

class SystemMutexPrivate {
public:
	SystemMutexPrivate ();
	~SystemMutexPrivate ();
	void lock (void);
	void unlock (void);
private:
	pthread_mutex_t m_mutex;
};

SystemMutexPrivate::SystemMutexPrivate ()
{
	int retval;
	retval = pthread_mutex_init (&m_mutex, NULL);
	assert (retval == 0);
}
SystemMutexPrivate::~SystemMutexPrivate ()
{
	int retval;
	retval = pthread_mutex_destroy (&m_mutex);
	assert (retval == 0);
}
void 
SystemMutexPrivate::lock (void)
{
	pthread_mutex_lock (&m_mutex);
}
void 
SystemMutexPrivate::unlock (void)
{
	pthread_mutex_unlock (&m_mutex);
}

SystemMutex::SystemMutex ()
	: m_priv (new SystemMutexPrivate ())
{}
SystemMutex::~SystemMutex ()
{
	delete m_priv;
}
void 
SystemMutex::lock (void)
{
	m_priv->lock ();
}
void 
SystemMutex::unlock (void)
{
	m_priv->unlock ();
}



}; // namespace ns3
