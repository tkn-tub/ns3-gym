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

#include "system-semaphore.h"

#include <semaphore.h>
#include <cassert>
#include <iostream>
#include "errno.h"
#include "string.h"

namespace yans {

class SystemSemaphorePrivate {
public:
	SystemSemaphorePrivate (uint32_t init);
	void post (void);
	void post (uint32_t n);
	void wait (void);
	void wait (uint32_t n);
private:
	sem_t m_sem;
};

SystemSemaphorePrivate::SystemSemaphorePrivate (uint32_t init)
{
	int retval = sem_init (&m_sem, 0, init);
	if (retval == -1) {
		std::cout << "sem init " << this << " " << strerror (errno) << std::endl;
	}
}
void 
SystemSemaphorePrivate::post (void)
{
	int retval = sem_post (&m_sem);
	if (retval == -1) {
		std::cout << "sem post " << this << " " << strerror (errno) << std::endl;
	}
}
void 
SystemSemaphorePrivate::wait (void)
{
	int retval;
	do {
		retval = sem_wait (&m_sem);
	} while (retval == -1 && errno == EINTR);
	if (retval == -1) {
		std::cout << "sem wait " << this << " " << strerror (errno) << std::endl;
	}
}
void 
SystemSemaphorePrivate::post (uint32_t n)
{
	for (uint32_t i = 0; i < n; i++) {
		post ();
	}
}
void 
SystemSemaphorePrivate::wait (uint32_t n)
{
	for (uint32_t i = 0; i < n; i++) {
		wait ();
	}
}

SystemSemaphore::SystemSemaphore (uint32_t init)
	: m_priv (new SystemSemaphorePrivate (init))
{}
SystemSemaphore::~SystemSemaphore ()
{
	delete m_priv;
}
void 
SystemSemaphore::post (void)
{
	m_priv->post ();
}
void 
SystemSemaphore::post (uint32_t n)
{
	m_priv->post (n);
}
void 
SystemSemaphore::wait (void)
{
	m_priv->wait ();
}
void 
SystemSemaphore::wait (uint32_t n)
{
	m_priv->wait (n);
}



}; // namespace yans
